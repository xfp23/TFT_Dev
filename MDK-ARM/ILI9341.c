#include "ILI9341.h"

#ifndef ON
#define ON (true)
#endif

#ifndef OFF
#define OFF (false)
#endif

/**
 * @brief Construct a new WRDATA object
 * 写寄存器函数
 *
 * @param ILI
 */
void ILI93_WR_REG(ILI9341_Class_t *ILI,ILI93_Register_t data)
{
    uint8_t byte = (uint8_t)data;
    // 拉低数据引脚
    HAL_GPIO_WritePin(ILI->Hardware.RS.Port,ILI->Hardware.RS.Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ILI->Hardware.CS.Port,ILI->Hardware.CS.Pin,GPIO_PIN_RESET);
    if (ILI->spi.DMA == OFF)
    {
    HAL_SPI_Transmit(ILI->spi.handle, &byte, 1, ILI->spi.Timeout);
    HAL_GPIO_WritePin(ILI->Hardware.RS.Port,ILI->Hardware.RS.Pin,GPIO_PIN_SET);
    return;
    }
    HAL_SPI_Transmit_DMA(ILI->spi.handle,&byte,1);
    HAL_GPIO_WritePin(ILI->Hardware.CS.Port,ILI->Hardware.CS.Pin,GPIO_PIN_SET);
   
}

/**
 * @brief Construct a new WDATA object
 * 写数据函数
 *
 */
void ILI93_WR_DATA(ILI9341_Class_t *ILI,ILI93_Command_t data)
{
    uint8_t byte  = (uint8_t)data;
    // 拉高写数据
    HAL_GPIO_WritePin(ILI->Hardware.RS.Port,ILI->Hardware.RS.Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI->Hardware.CS.Port,ILI->Hardware.CS.Pin,GPIO_PIN_RESET);
    if (ILI->spi.DMA == OFF)
    {
        HAL_SPI_Transmit(ILI->spi.handle, &byte, 1, ILI->spi.Timeout);
        return;
    }
    HAL_SPI_Transmit_DMA(ILI->spi.handle,  &byte, 1);
    //HAL_GPIO_WritePin(ILI->Hardware.RS.Port,ILI->Hardware.RS.Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ILI->Hardware.CS.Port,ILI->Hardware.CS.Pin,GPIO_PIN_SET);
}

/**
 * @brief Construct a new RDATA object
 * 读数据函数
 * @param ILI
 */
static void RDATA(ILI9341_Class_t *ILI)
{
    if(ILI->spi.DMA == OFF)
    {
        HAL_SPI_Receive(ILI->spi.handle,ILI->spi.buf,ILI->spi.size,ILI->spi.Timeout);
        return;
    }
    HAL_SPI_Receive_DMA(ILI->spi.handle,ILI->spi.buf,ILI->spi.size);
}



void ILI93_WriteRAM_Prepare(ILI9341_Class_t *ILI)
{
	ILI93_WR_REG(ILI,WR_MEMORY);
}	


void ILI93_Init(ILI9341_Class_t *ILI)
{
    HAL_GPIO_WritePin(ILI->Hardware.LED.Port,ILI->Hardware.LED.Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI->Hardware.RST.Port,ILI->Hardware.RST.Pin,GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(ILI->Hardware.RST.Port,ILI->Hardware.RST.Pin,GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(ILI->Hardware.RST.Port,ILI->Hardware.RST.Pin,GPIO_PIN_SET);
	ILI93_WR_REG(ILI,EXTC_CT);  
	ILI93_WR_DATA(ILI,EXTC_DISABLE); 
	ILI93_WR_DATA(ILI,EXTC_PARAM2); 
	ILI93_WR_DATA(ILI,EXTC_PARAM3); 
 
	ILI93_WR_REG(ILI,PWR_SEQ_CT);  
	ILI93_WR_DATA(ILI,SOFT_START_HOLD_1); 
	ILI93_WR_DATA(ILI,POWER_SEQ_4); 
	ILI93_WR_DATA(ILI,POWER_PARAM3); 
	ILI93_WR_DATA(ILI,DDVDH_ENABLE); 
 
	ILI93_WR_REG(ILI,DRIVE_TIM_CT);  
	ILI93_WR_DATA(ILI,DEFAULT_TIM_1unit); 
	ILI93_WR_DATA(ILI,EQ0_CR0); 
	ILI93_WR_DATA(ILI,DEFAULT_CHARGE_2UINT); 

	ILI93_WR_REG(ILI,POWER_A_CT);  
	ILI93_WR_DATA(ILI,POWERA_PARAM1); 
	ILI93_WR_DATA(ILI,POWERA_PARAM2); 
	ILI93_WR_DATA(ILI,POWERA_PARAM3); 
	ILI93_WR_DATA(ILI,VD_1_6); 
	ILI93_WR_DATA(ILI,VBC_5_6); 
	
	ILI93_WR_REG(ILI,PUMP_RATION_CT);  
	ILI93_WR_DATA(ILI,DDVDH_2XVCL); 
 
	ILI93_WR_REG(ILI,DRIVER_TIMB_CT);  
	ILI93_WR_DATA(ILI,TIMB_PARAM1); 
	ILI93_WR_DATA(ILI,TIMB_PARAM2); 

	ILI93_WR_REG(ILI,POWER_CT1);       //Power control 
	ILI93_WR_DATA(ILI,VRH_3_8V);     //VRH[5:0] 
 
	ILI93_WR_REG(ILI,POWER_CT2);       //Power control 
	ILI93_WR_DATA(ILI,POWER_CT2_PARAM);     //SAP[2:0];BT[3:0] 
 
	ILI93_WR_REG(ILI,VCOM_CT1);       //VCM control 
	ILI93_WR_DATA(ILI,VCOM_CT1_PARAM1);   //22
	ILI93_WR_DATA(ILI,VCOM_CT1_PARAM2);   //35
 
	ILI93_WR_REG(ILI,VCOM_CT2);       //VCM control2 
	ILI93_WR_DATA(ILI,VCOM_CT2_PARAM);  //AF

	ILI93_WR_REG(ILI,DISPLAY_INVER);

	ILI93_WR_REG(ILI,MEMORY_ACCESS_CT);       // Memory Access Control 
	ILI93_WR_DATA(ILI,MEMORY_ACCESS_PARAM); 

	ILI93_WR_REG(ILI,DISPLAY_FUN_CT);  
	ILI93_WR_DATA(ILI,SET_NO_DISPLAY_SCAN_INTERVAL); 
	ILI93_WR_DATA(ILI,DISPLAY_FUN_PARAM2); 

	ILI93_WR_REG(ILI,PIXEL_FORMAT_SET);       
	ILI93_WR_DATA(ILI,PIXEL_FORMAT); 

	ILI93_WR_REG(ILI,INTERFACE_CT);  //Interface Control
	ILI93_WR_DATA(ILI,INTERFACE_CT_PARAM1); 
	ILI93_WR_DATA(ILI,INTERFACE_CT_PARAM2);  //MCU

	ILI93_WR_REG(ILI,FRAM_RATE_CT);       //VCM control 
	ILI93_WR_DATA(ILI,FOSC_0); 
	ILI93_WR_DATA(ILI,FRAM_70HZ); 
 
	ILI93_WR_REG(ILI,CORRECTION);       // 3Gamma Function Disable 
	ILI93_WR_DATA(ILI,DISENABLE_3G); 
 
	ILI93_WR_REG(ILI,GAMMA_SET);       //Gamma curve selected 
	ILI93_WR_DATA(ILI,GAMMA_CURVE_1); 
 
	ILI93_WR_REG(ILI,POSI_GAMMA_CORRECT);       //Set Gamma 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM1); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM2); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM3); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM4); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM5); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM6); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM7); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM8); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM9); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM10); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM11); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM12); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM13); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM14); 
	ILI93_WR_DATA(ILI,POSI_GAMMA_PARAM15); 
 
	ILI93_WR_REG(ILI,NEGA_GAMMA_CORRECT);       //Set Gamma 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM1); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM2); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM3); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM4); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM5); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM6); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM7); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM8); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM9); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM10); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM11); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM12); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM13); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM14); 
	ILI93_WR_DATA(ILI,NEGA_GAMMA_PARAM15); 
	ILI93_WR_REG(ILI,SLEEP_OUT);       //Exit Sleep 
	HAL_Delay(120); 
	ILI93_WR_REG(ILI,DISPLAY_ON);       //Display on 
	HAL_Delay(12);
}