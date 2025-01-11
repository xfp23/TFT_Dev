#ifndef LCD_H
#define LCD_H

/**
 * @file LCD.h
 * @author xfp23 (@github.com/xfp23)
 * @brief 
 * @version 0.1
 * @date 2025-01-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "CTP.h" // 触摸驱动
#include "ILI9341.h" // 驱动芯片驱动


#ifdef __cplusplus
extern "C"
{
#endif
	
typedef enum 
{
    BLACK  = 0x0000,     // 黑色 RGB(0, 0, 0)  
    WHITE  = 0xFFFF,     // 白色 RGB(255, 255, 255)  
    RED    = 0xF800,     // 红色 RGB(255, 0, 0)  
    BLUE   = 0x001F,     // 蓝色 RGB(0, 0, 255)  
    GREEN  = 0x07E0,     // 绿色 RGB(0, 255, 0)  
    YELLOW = 0xFFE0,     // 黄色 RGB(255, 255, 0)  
    CYAN   = 0x07FF,     // 青色 RGB(0, 255, 255)  
    MAGENTA= 0xF81F,     // 品红 RGB(255, 0, 255)  
    GRAY   = 0x8410,     // 灰色 RGB(169, 169, 169)  
    ORANGE = 0xFD20,     // 橙色 RGB(255, 165, 0)  
    PURPLE = 0x8010,     // 紫色 RGB(128, 0, 128)  
    PINK   = 0xF81F,     // 粉红 RGB(255, 192, 203)  
    LIME   = 0x07E0,     // 青柠色 RGB(0, 255, 0)  
    BROWN  = 0xBC40,     // 棕色 RGB(139, 69, 19)  
} LCD_Color_t;



typedef struct
{
    CTP_Class_t touch;       // 触摸信号对象
    ILI9341_Class_t display; // 显示对象
    uint16_t width;          // 屏幕宽度
    uint16_t height;         // 屏幕高度
} LCD_Class_t;               // 屏幕对象


/**
 * @brief LCD初始化
 * 
 * @param lcd 
 */
extern void LCD_Init(LCD_Class_t * lcd);

#ifdef __cplusplus
}
#endif
#endif
