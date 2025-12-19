#include "stm32f10x.h"                  // Device header
uint16_t ADC_Data[4];
void AD_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//初始化DMA，配置DMA参数 
	DMA_InitTypeDef   DMA_InitStruct;  							// 定义DMA初始化结构体变量，用于配置DMA各项参数
	DMA_InitStruct.DMA_BufferSize=4;                    		 // 设置DMA传输缓冲区大小为4（单位为半字）
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;        		 // 设置传输方向：外设作为数据源（外设→内存）
	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;                		// 使能存储器到存储器（M2M）传输模式
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)ADC_Data; 			// 设置内存基地址
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; // 设置内存数据宽度
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;    			// 使能存储器内存地址自增
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;              			// 设置传输模式
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR; 			// 设置外设基地址
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;	 // 设置外设数据宽度
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable; 		// 使能外设地址不自增
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;      			// 设置DMA通道优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);              			// 根据配置初始化DMA1的通道

	
    // 初始化GPIO（模拟输入模式）
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;       // 模拟输入模式，无需配置速度
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // 模拟输入模式下速度配置无效，可省略
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // ADC时钟源分频（PCLK2=72MHz，分频后为12MHz，符合ADC最大时钟要求）
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//选择ADC1通道和配置序列
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
	
	
    // 初始化ADC外设
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;  // 转换模式:单次或者连续
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  // 右对齐
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 无外部触发，软件触发
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  // 独立模式
    ADC_InitStruct.ADC_NbrOfChannel = 4;             // 转换通道数量
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;       // 扫描模式
    ADC_Init(ADC1, &ADC_InitStruct);
   
	
    // 使能ADC，DMA外设
    ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
    // ADC校准（必须执行）
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);  // 等待复位校准完成
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);       // 等待校准完成
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);          // 软件触发转换
}


