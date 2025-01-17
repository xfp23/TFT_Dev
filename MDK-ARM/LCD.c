// #include "spi.h"
#include "LCD.h"
#include <stdarg.h>
#include <math.h>
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
 * @brief 清除整个屏幕
 * 
 * @param lcd 对象指针
 * @param color 颜色
 */
void LCD_Fillclear(LCD_Class_t *lcd, LCD_Color_t color)
{
    // 提取 RGB565 格式中的颜色值
    // 将 RGB565 转换为 3 字节（扩展为 G、B、R）
    uint8_t data[3] = {
        (uint8_t)(color >> 8),  // 提取红色高 5 位并移位，填充为 8 位
        (uint8_t)color,  // 提取绿色高 6 位并移位，填充为 8 位
        0x00  // 提取蓝色高 5 位并移位，填充为 8 位
    };
	lcd->display.spi.size = 2;
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
            if (lcd->display.spi.DMA == true)
            {
                HAL_SPI_Transmit_DMA(lcd->display.spi.handle, data, lcd->display.spi.size);
            }
            else
            {
				lcd->display.spi.size = 3;
                HAL_SPI_Transmit(lcd->display.spi.handle, data, lcd->display.spi.size, lcd->display.spi.Timeout);
            }
        }
    }

    // 拉高 CS 结束传输
    HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port, lcd->display.Hardware.CS.Pin, GPIO_PIN_SET);
}


///**
//* @brief 清除特定区域屏幕
//* @brief 
//*/
//void LCD_clear(LCD_Class_t *lcd, uint16_t x,uint16_t y,LCD_Color_t color)
//{
//	
//}
/**
*@brief 设置屏幕显示方向
*/
void LCD_SetOrientation(LCD_Class_t *lcd,LCD_Dir_t direction)
{
   static uint16_t height_buf = 0;	//lcd->height;
   static uint16_t width_buf = 0; 	//lcd->width;
   static bool init = false ;
	if(!init)
	{
		init = true;
		height_buf = lcd->height;
		width_buf = lcd->width;
	}
    switch(direction)
    {
        case LCD_DIR_0 :
        lcd->height = height_buf;
        lcd->width = width_buf;
        break;
        case LCD_DIR_90 :
        lcd->height = width_buf;
        lcd->width = height_buf;
        break;
        case LCD_DIR_180 :
        lcd->height = height_buf;
        lcd->width = width_buf;
        break;
        case LCD_DIR_270 :
        lcd->height = width_buf;
        lcd->width = height_buf;
        break;
    }
	ILI93_WR_REG(&lcd->display,SET_DIRECTION);
	ILI93_WR_DATA(&lcd->display,(ILI93_Command_t)direction);
}

void LCD_Init(LCD_Class_t * lcd)
{ 
	CTP_Init(&lcd->touch);
    ILI93_Init(&lcd->display);
	LCD_SetOrientation(lcd,LCD_DIR_0);
    LCD_Fillclear(lcd,WHITE); // 白色
}


void LCD_Reset(LCD_Class_t * lcd)
{
    HAL_GPIO_WritePin(lcd->display.Hardware.RST.Port,lcd->display.Hardware.RST.Pin,GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(lcd->display.Hardware.RST.Port,lcd->display.Hardware.RST.Pin,GPIO_PIN_RESET);
	HAL_Delay(100);	
	HAL_GPIO_WritePin(lcd->display.Hardware.RST.Port,lcd->display.Hardware.RST.Pin,GPIO_PIN_SET);
	HAL_Delay(50);
}


void Lcd_WriteData_16Bit(LCD_Class_t * lcd,uint16_t Data)
{	
    uint8_t data[2] = { (uint8_t)(Data >> 8) ,(uint8_t)Data};
   HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port,lcd->display.Hardware.CS.Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(lcd->display.Hardware.RS.Port,lcd->display.Hardware.RS.Pin,GPIO_PIN_SET);
  HAL_SPI_Transmit_DMA(lcd->display.spi.handle,data,2);
  HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port,lcd->display.Hardware.CS.Pin,GPIO_PIN_SET);
}
/**
*@brief 在屏幕上显示一个点
*@param LCD_Class_t 屏幕对象
*@param 点的横坐标
*@param 点的纵坐标
*@param 点的颜色
*/
void Lcd_Drawpoint(LCD_Class_t *lcd, uint16_t x, uint16_t y,LCD_Color_t color)
{
	LCD_SetCursor(lcd, x, y);
	Lcd_WriteData_16Bit(lcd, (uint16_t)color);	
}

/**
 * @brief 画板应用程序
 * 
 * @param lcd 
 * @param x 
 * @param y 
 */
void Applacation_Drawboard(LCD_Class_t *lcd, uint16_t x, uint16_t y)
{
    static uint16_t last_x = 0, last_y = 0;  // 用静态变量保存上次的点坐标
    uint8_t size = 6;  // 点的大小，6个像素组成一个点

    // 如果是第一次调用（初始化点），直接设置last_x和last_y为当前点
    if (last_x == 0 && last_y == 0) {
        last_x = x;
        last_y = y;
    }

    // 计算前一个点和当前点之间的距离
    int dx = x - last_x;
    int dy = y - last_y;
    int distance = (int)sqrt(dx * dx + dy * dy);  // 计算两个点之间的距离

    // 如果距离较小，直接连接两个点
    if (distance <= size) {
        for (int i = -size; i <= size; i++) {
            for (int j = -size; j <= size; j++) {
                if ((x + i >= 0) && (y + j >= 0) && (x + i < lcd->width) && (y + j < lcd->height)) {
                    if (i * i + j * j <= size * size) {
                        LCD_SetCursor(lcd, x + i, y + j);
                        Lcd_WriteData_16Bit(lcd, (uint16_t)BLACK);
                    }
                }
            }
        }
    }
    else {
        // 如果距离较远，则按直线连接前后两个点，填充这条直线
        float slope = (float)dy / dx;  // 计算斜率
        for (int i = 0; i < distance; i++) {
            int new_x = last_x + i * dx / distance;
            int new_y = last_y + i * dy / distance;
            
            // 以粗点的方式绘制每个连接点
            for (int j = -size; j <= size; j++) {
                for (int k = -size; k <= size; k++) {
                    if ((new_x + j >= 0) && (new_y + k >= 0) && (new_x + j < lcd->width) && (new_y + k < lcd->height)) {
                        if (j * j + k * k <= size * size) {
                            LCD_SetCursor(lcd, new_x + j, new_y + k);
                            Lcd_WriteData_16Bit(lcd, (uint16_t)BLACK);
                        }
                    }
                }
            }
        }
    }

    // 更新上一次的点坐标
    last_x = x;
    last_y = y;
}

/**
* @brief lcd屏幕画直线
* @param LCD_Class_t 屏幕对象 
* @param 起始X坐标
* @param 起始Y坐标
* @param 终点x坐标
* @param 终点y坐标
* @param ... 可选参数，控制线条的粗细程度,默认为1
* @param LCD_Color_t 颜色
*/
void LCD_DrawLine(LCD_Class_t *lcd, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, LCD_Color_t color, ...)
{
    int line_kness = 1;
    va_list args;
    va_start(args, color);
    line_kness = va_arg(args, int);
    va_end(args);

    if (line_kness <= 0) line_kness = 1;

    int dx = end_x - start_x;
    int dy = end_y - start_y;
    int dx2 = 2 * abs(dx);
    int dy2 = 2 * abs(dy);
    int x = start_x;
    int y = start_y;
    int x_incr = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int y_incr = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;

    if (abs(dx) > abs(dy)) {
        int error = dy2 - abs(dx);
        while (x != end_x) {
            for (int i = -line_kness / 2; i <= line_kness / 2; i++) {
                Lcd_Drawpoint(lcd, x + i, y, color);
            }
            if (error > 0) {
                y += y_incr;
                error -= dx2;
            }
            x += x_incr;
            error += dy2;
        }
    } else {
        int error = dx2 - abs(dy);
        while (y != end_y) {
            for (int i = -line_kness / 2; i <= line_kness / 2; i++) {
                Lcd_Drawpoint(lcd, x, y + i, color);
            }
            if (error > 0) {
                x += x_incr;
                error -= dy2;
            }
            y += y_incr;
            error += dx2;
        }
    }

    for (int i = -line_kness / 2; i <= line_kness / 2; i++) {
        Lcd_Drawpoint(lcd, x + i, y, color);
    }
}

/**
* @brief LCD画空心圆
* @param LCD_Class_t lcd对象
* @param x   圆心的x坐标
* @param y   圆心的y坐标
* @param rad 圆的半径
* @param color 圆的颜色
* @param ...  可选参数,圆的线条粗细程度
*/
void LCD_DrawCircle(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t rad, LCD_Color_t color, ...)
{
    int line_kness = 1;
    va_list args;
    va_start(args, color);
    line_kness = va_arg(args, int);
    va_end(args);

    if (line_kness <= 0) line_kness = 1;

    int x_pos = rad, y_pos = 0;
    int decision = 1 - rad; // 初始决策参数

    while (x_pos >= y_pos)
    {
        // 如果线条粗细大于1，绘制圆边缘附近的点来加粗线条
        for (int i = -line_kness + 1; i <= line_kness - 1; i++) 
        {
            // 画8个对称点，并通过偏移让线条加粗
            Lcd_Drawpoint(lcd, x + x_pos + i, y + y_pos, color);
            Lcd_Drawpoint(lcd, x - x_pos + i, y + y_pos, color);
            Lcd_Drawpoint(lcd, x + x_pos + i, y - y_pos, color);
            Lcd_Drawpoint(lcd, x - x_pos + i, y - y_pos, color);
            Lcd_Drawpoint(lcd, x + y_pos + i, y + x_pos, color);
            Lcd_Drawpoint(lcd, x - y_pos + i, y + x_pos, color);
            Lcd_Drawpoint(lcd, x + y_pos + i, y - x_pos, color);
            Lcd_Drawpoint(lcd, x - y_pos + i, y - x_pos, color);
        }

        y_pos++;
        if (decision <= 0)
        {
            decision += 2 * y_pos + 1;
        }
        else
        {
            x_pos--;
            decision += 2 * (y_pos - x_pos) + 1;
        }
    }
}


/**
* @brief LCD画实心圆
* @param LCD_Class_t lcd对象
* @param x   圆心的x坐标
* @param y   圆心的y坐标
* @param rad 圆的半径
* @param color 圆的颜色
*/
void LCD_DrawFille_Circle(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t rad, LCD_Color_t color)
{
    int x_pos = rad, y_pos = 0;
    int decision = 1 - rad; // 初始决策参数

    while (x_pos >= y_pos)
    {
        // 绘制每一行的点
        for (int i = x - x_pos; i <= x + x_pos; i++)
        {
            Lcd_Drawpoint(lcd, i, y + y_pos, color);
            Lcd_Drawpoint(lcd, i, y - y_pos, color);
        }
        for (int i = x - y_pos; i <= x + y_pos; i++)
        {
            Lcd_Drawpoint(lcd, i, y + x_pos, color);
            Lcd_Drawpoint(lcd, i, y - x_pos, color);
        }

        y_pos++;
        if (decision <= 0)
        {
            decision += 2 * y_pos + 1;
        }
        else
        {
            x_pos--;
            decision += 2 * (y_pos - x_pos) + 1;
        }
    }
}
