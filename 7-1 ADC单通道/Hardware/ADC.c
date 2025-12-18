#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 初始化GPIO（模拟输入模式）
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;       // 模拟输入模式，无需配置速度
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // 模拟输入模式下速度配置无效，可省略
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // ADC时钟源分频（PCLK2=72MHz，分频后为12MHz，符合ADC最大时钟要求）
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
	//选择ADC通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	
    // 初始化ADC外设
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  // 单次转换模式（修正大小写）
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  // 右对齐
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 无外部触发，软件触发
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  // 独立模式
    ADC_InitStruct.ADC_NbrOfChannel = 1;             // 转换通道数量
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;       // 非扫描模式（修正大小写）
    ADC_Init(ADC1, &ADC_InitStruct);
    
    // 使能ADC外设
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC校准（必须执行）
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);  // 等待复位校准完成
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);       // 等待校准完成
}

uint16_t AD_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);          // 软件触发转换
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);  // 等待转换完成
    return ADC_GetConversionValue(ADC1);             // 返回转换结果
}
