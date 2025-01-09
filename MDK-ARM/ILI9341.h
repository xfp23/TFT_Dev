#ifndef ILI9341_H
#define ILI9341_H

/**
 * @file FT6336G_Drive.h
 * @author xfp23 (@github.com/xfp23)
 * @brief 此文件是电容触摸屏驱动芯片ILI9341的驱动,提供底层的屏幕显示驱动接口
 * @version 0.1
 * @date 2025-01-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */
 
#include "CTP.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define EQ_ENABLE   0x10
#define EQ_DISABLE  0x00
#define CR_DISABLE  0x00
#define CR_ENABLE   0x01


#define NODISPLAY_SCAN_NORMAL 0x00 // 正常扫描
#define NODISPLAY_SCAN_INTERVAL 0x08 // 间隔扫描

// 选择液晶类型是常白还是常黑
#define REV_NORMAL_BLACK 0X00 
#define REV_NORMAL_WHITE 0X80

// 显示控制设置
#define BCTRL_ON    (0x01 << 6) // 亮度控制块打开，显示亮度会根据亮度寄存器的值和其他参数设置。
#define BCTRL_OFF   (0x00 << 6) // 亮度控制块关闭，此时亮度寄存器（DBV[7..0]）被强制设置为 00h
#define DD_ON       (0x01 << 3) // 开启调光功能，显示亮度会逐渐变化
#define DD_OFF      (0x00 << 3) // 关闭调光功能，亮度直接根据亮度寄存器设置。
#define BL_OFF      (0x00 << 2) // 完全关闭背光电路，即使显示屏的其他亮度设置仍启用，屏幕也不会发光（背光电路强制关闭）。
#define BL_ON       (0x01 << 2) // 打开背光电路，显示屏发光
typedef enum 
{
 DISABLE = 0x00, // 通用禁用开关
 ENABLE = 0X01, // 通用使能开关

// EXTC_CT 寄存器
EXTC_DISABLE = DISABLE, // 扩展命令集禁用
EXTC_ENABLE = ENABLE,  // 扩展命令集使能

//POWER_CT 寄存器
 SOFT_START_HOLD_3 = 0x44, // 软启动保持3帧
 SOFT_START_HOLD_2 = 0x54, // 软启动保持3帧
 SOFT_START_HOLD_1 = 0x64, // 软启动保持3帧 （默认）
 SOFT_START_DISENABLE = 0x74, // 禁用软启动

POWER_SEQ_1 = 0X00, // 第一阶段使用
POWER_SEQ_2 = 0X01, // 第二阶段使用
POWER_SEQ_3 = 0X02, // 第三阶段使用
POWER_SEQ_4 = 0X03, // 第四阶段使用 （默认）

DDVDH_ENABLE = 0x81, // 启用DDVDH （default)
DDVDH_DISABLE = 0X01, // 禁用

// DRIVE_TIM_CT 寄存器
DEFAULT_NONTIM = 0x84, // 默认非重叠时间
DEFAULT_TIM_1unit = 0x85, 
//parameter:CR timing control

// 写下面这个参数
EQ0_CR0 = EQ_DISABLE | CR_DISABLE,
EQ1_CR0 = EQ_ENABLE | CR_DISABLE,
EQ0_CR1 = EQ_DISABLE | CR_ENABLE,
EQ1_CR1 = EQ_ENABLE | CR_ENABLE,

DEFAULT_CHARGE = 0x7A,// (: default pre-charge timing) 参数：预充电时序控制
DEFAULT_CHARGE_1UINT = 0X79,
DEFAULT_CHARGE_2UINT = 0X78,


//POWER_A_CT 寄存器
POWERA_PARAM1 = 0x39,

POWERA_PARAM2 = 0x2C,

POWERA_PARAM3 = 0x00,

VD_1_55 = 0x30,
VD_1_4 = 0x31,
VD_1_5 = 0x32,
VD_1_65 = 0x33,
VD_1_6 = 0x34, //(default)
VD_1_7 = 0x35,

VBC_5_8 = 0x00,
VBC_5_7 = 0x01,
VBC_5_6 = 0x02,
VBC_5_5 = 0x03,
VBC_5_4 = 0x04,
VBC_5_3 = 0x05,
VBC_5_2 = 0x06, //(default)

// PUMP_RATION_CT 寄存器
DDVDH_2XVCL = 0x20, // (default)
DDVDH_3XVCL = 0x30,

//DRIVER_TIMB_CT 寄存器
TIMB_PARAM1 = 0X00,

TIMB_PARAM2 = 0x00,

// power_CT1 寄存器
VRH_3V3 = 0X09,
VRH_5V = 0x2B,
VRH_3_8V = 0x13, // (default)

//POWER_CT2 寄存器
POWER_CT2_PARAM = 0x13,

//VCOM_CT1 寄存器
VCOM_CT1_PARAM1 = 0x22,
VCOM_CT1_PARAM2 = 0x35,

// VCOM_CT2 寄存器
VCOM_CT2_PARAM =   0xBD,

// DISPLAY_INVER 寄存器
DISPLAY_INVER_OFF = 0x20, // 关闭镜像翻转命令，开启直接发送寄存器

//MEMORY_ACCESS_CT 寄存器
MEMORY_ACCESS_PARAM = 0x08,

//DISPLAY_FUN_CT 寄存器
//设置非显示区域的扫描模式。
SET_NO_DISPLAY_SCAN_NORMAL = NODISPLAY_SCAN_NORMAL | 0x02,
SET_NO_DISPLAY_SCAN_INTERVAL = NODISPLAY_SCAN_INTERVAL | 0x02, // 间隔扫描 （default)

DISPLAY_FUN_PARAM2 = 0xA2,

// PIXEL_FORMAT_SET 寄存器
PIXEL_FORMAT =  0x55,

// INTERFACE_CT 寄存器
INTERFACE_CT_PARAM1 = 0x01,
INTERFACE_CT_PARAM1 = 0x30,

// FRAM_RATE_CT 寄存器
FOSC_0 = 0x00,  //:正常模式下内部时钟分频比 (default)
FOSC_2 = 0x01,
F0SC_4 = 0X02,
FOSC_8 = 0X03,

FRAME_119HZ = 0x10,
FRAM_112HZ = 0x11,
FRAM_90HZ = 0x15,
FRAM_70HZ = 0x1B,
FRAM_60HZ = 0X1F,

//CORRECTION 寄存器
ENABLE_3G = ENABLE,
DISENABLE = DISABLE,

// GAMMA_SET 寄存器
GAMMA_CURVE_1 = 0x01, //Gamma curve 1 (G2.2) 只有这一个值可用

// POSI_GAMMA_CORRECT 寄存器
POSI_GAMMA_PARAM1 = 0x0F, 

POSI_GAMMA_PARAM2 = 0x35,

POSI_GAMMA_PARAM3 = 0x31,

POSI_GAMMA_PARAM4 = 0x0B,

POSI_GAMMA_PARAM5 = 0x0E,

POSI_GAMMA_PARAM6 = 0x06,

POSI_GAMMA_PARAM7 = 0x49,

POSI_GAMMA_PARAM8 = 0xA7,

POSI_GAMMA_PARAM9 = 0x33,

POSI_GAMMA_PARAM10 = 0x07,
 
POSI_GAMMA_PARAM11 = 0x0F,

POSI_GAMMA_PARAM12 = 0x03,

POSI_GAMMA_PARAM13 = 0x0C,

POSI_GAMMA_PARAM14 = 0x0A,

POSI_GAMMA_PARAM15 = 0x00,

// NEGA_GAMMA_CORRECT 寄存器
NEGA_GAMMA_PARAM1 = 0x00,

NEGA_GAMMA_PARAM2 = 0x0A,

NEGA_GAMMA_PARAM3 = 0x0F,

NEGA_GAMMA_PARAM4 = 0x04,

NEGA_GAMMA_PARAM5 = 0x11,

NEGA_GAMMA_PARAM6 = 0x08,

NEGA_GAMMA_PARAM7 = 0x36,

NEGA_GAMMA_PARAM8 = 0x58,

NEGA_GAMMA_PARAM9 = 0x4D,

NEGA_GAMMA_PARAM10 = 0x07,

NEGA_GAMMA_PARAM11 = 0x10,

NEGA_GAMMA_PARAM12 = 0x0C,

NEGA_GAMMA_PARAM13 = 0x32,

NEGA_GAMMA_PARAM14 = 0x34,

NEGA_GAMMA_PARAM15 = 0x0F,

// SET_DIRECTION 寄存器
ROTATE_0 =    0x08, // 0度旋转
ROTATE_90 =   0x68, // 90度旋转
ROTATE_180 =  0xC8, // 180度旋转
ROTATE_270 =  0xA8, // 270度旋转

// WRITE_DISPLAY_CT 寄存器
DISPLAY_OFF = BCTRL_OFF | DD_OFF | BL_OFF,    // 亮度控制关闭，调光功能关闭，背光关闭
DISPLAY_DIM = BCTRL_ON | DD_ON | BL_ON,      // 显示模块打开，调光功能启用，背光开启
DISPLAY_BRIGHT = BCTRL_ON | DD_OFF | BL_ON,  // 显示模块打开，调光功能关闭，背光开启
DISPLAY_DARK = BCTRL_ON | DD_ON | BL_OFF,    // 显示模块打开，调光功能启用，背光关闭
DISPLAY_MINIMAL = BCTRL_OFF | DD_OFF | BL_ON // 显示模块关闭，调光功能关闭 ，背光开启
}ILI93_Command_t;

typedef enum
{
EXTC_CT = 0xCF, // 扩展命令集启用 0 : 禁用  1：启用 请连接 EXTC 至 VDDI 来读/写扩展寄存器 (RB0h~RCFh, RE0h~RFFh)
POWER_CT = 0xC1,// 设置升压电路中使用的系数。选择适合工作电压的最佳升压系数。为了降低功耗，设置较小的系数 应该默认写入0x30
PWR_SEQ_CT = 0xED, // 上电顺序控制寄存器,需要写入四个参数 
DRIVE_TIM_CT = 0xE8,// 设备时序控制
POWER_A_CT = 0xCB,         //Power control A
PUMP_RATION_CT = 0xF7, // 泵比控制
DRIVER_TIMB_CT = 0xEA,//4 Driver timing control B
power_CT1 = 0xC0, //: ：设置GVDD电平，该电平是VCOM电平和灰阶电压电平的参考电平。
POWER_CT2 = 0xC1, // ：设置升压电路中使用的系数
VCOM_CT1 = 0xC5, // VCOM1 控制
VCOM_CT2 = 0xC7, // VCOM2 控制
// Display Inversion ON
DISPLAY_INVER = 0x21, // 该命令用于进入显示反转模式。该命令不改变帧存储器的内容。从帧存储器到显示器的每一位都被反转。此命令不会更改任何其他状态。
                    //要退出显示反转模式，应写入显示反转 OFF 命令 (20h)。
MEMORY_ACCESS_CT = 0x36,//内存访问控制
DISPLAY_FUN_CT = 0xB6,// 显示功能控制
PIXEL_FORMAT_SET = 0x3A, // 像素格式设置
INTERFACE_CT = 0xF6,// Interface Control
FRAM_RATE_CT = 0xB1, // 帧率速率控制 
CORRECTION = 0xF2, //  伽马校正
GAMMA_SET = 0x26,//Gamma Set//该命令用于为当前显示选择所需的 Gamma 曲线。最多 4 条固定伽玛曲线被选中。通过设置参数中的适当位来选择曲线
POSI_GAMMA_CORRECT = 0xE0, //Positive Gamma Correction (E0h)  正伽马矫正
NEGA_GAMMA_CORRECT = 0xE1,// Negative Gamma Correction (E1h) 负伽马矫正
SLEEP_OUT = 0x11,//推出休眠模式 当模块已处于睡眠模式时，此命令无效。休眠模式只能由休眠模式退出 
                 //在发送下一个命令之前需要等待 5 毫秒，这是为了给电源电压留出时间和时钟电路稳定
                // 发送 Sleep In 命令后需要等待 120 毫秒（处于 Sleep Out 模式时）
DISPLAY_ON = 0x29, //该命令用于从 DISPLAY OFF 模式恢复。帧存储器的输出被启用。该命令不改变帧存储器的内容。该命令不会改变任何其他状态
DISPLAY_OFF = 0x28, //该命令用于进入DISPLAY OFF 模式。在此模式下，帧存储器的输出被禁用并且为空白已插入页面。该命令不改变帧存储器的内容。
                    // 此命令不会更改任何其他状态。显示屏上不会出现异常可见效果。
SET_DIRECTION = 0x36,// 设置屏幕方向
WRITE_DISPLAY_BRIGHT = 0x51, //Write Display Brightness 写入屏幕亮度，范围0x00~0xff
READ_DISPLAY_BRIGHT = 0x52, // 读取屏幕亮度
WRITE_DISPLAY_CT = 0x53,// 写入屏幕亮度控制寄存器
}ILI93_Register_t; // 芯片寄存器


#ifdef __cplusplus
}
#endif

#endif


