// #include "spi.h"
#include "LCD.h"

#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

uint8_t SPI_WriteByte(SPI_TypeDef* SPIx, uint8_t Byte)
{
    // 等待直到 SPI 的发送缓冲区空 (TXE) 标志被设置
    while((SPIx->SR & SPI_I2S_FLAG_TXE) == RESET); // 等待发送缓冲区空

    // 将字节数据写入 SPI 数据寄存器
    SPIx->DR = Byte; // 写数据到 SPI 数据寄存器

    // 等待直到 SPI 接收缓冲区有数据 (RXNE) 标志被设置
    while((SPIx->SR & SPI_I2S_FLAG_RXNE) == RESET); // 等待接收缓冲区有数据

    // 返回接收到的数据
    return SPIx->DR; // 返回接收到的数据
}

void LCD_SetWindows(LCD_Class_t * lcd,uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	ILI93_WR_REG(&lcd->display,SET_X_ADDRESS);	
	ILI93_WR_DATA(&lcd->display,xStar>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&xStar);		
	ILI93_WR_DATA(&lcd->display,xEnd>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&xEnd);

	ILI93_WR_REG(&lcd->display,SET_Y_ADDRESS);	
	ILI93_WR_DATA(&lcd->display,yStar>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&yStar);		
	ILI93_WR_DATA(&lcd->display,yEnd>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&yEnd);
    ILI93_WriteRAM_Prepare(&lcd->display);
		
}   
	
void LCD_SetCursor(LCD_Class_t * lcd,uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_SetWindows(lcd,Xpos,Ypos,Xpos,Ypos);	
} 

/**
 * @brief 清屏
 * 
 * @param lcd 对象指针
 * @param color 颜色
 */
void LCD_clear(LCD_Class_t *lcd, LCD_Color_t color)
{
    // 提取 RGB565 格式中的颜色值
    // 将 RGB565 转换为 3 字节（扩展为 G、B、R）
    uint8_t data[3] = {
        (uint8_t)((color & 0xF800) >> 8),  // 提取红色高 5 位并移位，填充为 8 位
        (uint8_t)((color & 0x07E0) >> 3),  // 提取绿色高 6 位并移位，填充为 8 位
        (uint8_t)((color & 0x001F) << 3)   // 提取蓝色高 5 位并移位，填充为 8 位
    };
	lcd->display.spi.size = 3;
    // 设置绘图窗口为整个屏幕
    LCD_SetWindows(lcd, 0, 0, lcd->width - 1, lcd->height - 1);

    // 拉低 CS 以开始传输
    HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port, lcd->display.Hardware.CS.Pin, GPIO_PIN_RESET);
    // 设置 RS 引脚以指定写入数据
    HAL_GPIO_WritePin(lcd->display.Hardware.RS.Port, lcd->display.Hardware.RS.Pin, GPIO_PIN_SET);

    // 遍历整个屏幕并填充颜色
    for (int i = 0; i < lcd->height; i++)
    {
        for (int j = 0; j < lcd->width; j++)
        {
            // 发送数据，RGB565 格式只需 2 字节
//            if (lcd->display.spi.DMA == true)
//            {
//                HAL_SPI_Transmit_DMA(lcd->display.spi.handle, data, lcd->display.spi.size);
//            }
//            else
//            {
//                HAL_SPI_Transmit(lcd->display.spi.handle, data, lcd->display.spi.size, lcd->display.spi.Timeout);
//            }
			SPI_WriteByte(SPI1,color>>8);
			SPI_WriteByte(SPI1,color);
//			SPI_WriteByte(SPI1,color);
        }
    }

    // 拉高 CS 结束传输
    HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port, lcd->display.Hardware.CS.Pin, GPIO_PIN_SET);
}


void LCD_Init(LCD_Class_t * lcd)
{ 
    ILI93_Init(&lcd->display);
    LCD_clear(lcd,WHITE); // 白色
	HAL_Delay(200);
//	LCD_clear(lcd,BLACK); // 黑色
//	HAL_Delay(200);
//	LCD_clear(lcd,BLUE);  // 蓝色
//	HAL_Delay(200);
//	LCD_clear(lcd,RED);   // 红色
//	HAL_Delay(200);
//	LCD_clear(lcd,GREEN);  // 绿色
//	HAL_Delay(200); 
//	LCD_clear(lcd,CYAN); //青色
//	HAL_Delay(200);
//	LCD_clear(lcd,MAGENTA); // 品红
//	HAL_Delay(200);
//	LCD_clear(lcd,LIME); // 青柠色
//	HAL_Delay(200);
}

