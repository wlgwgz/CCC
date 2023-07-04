/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: SmartHmi_commu_info.h
 文件描述: 通讯信息定义
 创建人  : hzy
 创建日期: 2016.07.15
*******************************************************************************/

#ifndef SHCI_H
#define SHCI_H

#ifdef  SHCI_GLOBAL
    #define SHCI_EXT
#else
    #define SHCI_EXT extern
#endif

/*

    注意:
    ① 后板只能使用此文件，不能修改此文件。若有改动，则要在前板上进行升级，再同步此文件到后板。
    ② 由于前板对后板为一对多的关系，新增或修改需充分考虑向前兼容性!

*/


/***********************************************************************************
                                显示辅助信息
***********************************************************************************/
#define DISP_PCB_NUM            0       // 0 模块数
#define DISP_CMP_NUM            1       // 1 压机数
#define DISP_MAC_STA            2       // 2 机组状态
#define DISP_TMP_CURT           3       // 3 当前温度
#define DISP_TMP_SET            4       // 4 设定温度
#define DISP_SET_UNIT           5       // 5 温度单位
#define DISP_MODE_VALID         6       // 6 可设模式
                                        //   当9~16位为0xff：从参数读取可设模式，5~8为参数组号，1~4位为参数项号；
                                        //   当9~16位非0xff：从该辅助信息读取可设模式，辅助信息为位变量（可用15种模式）
#define DISP_MODE_SET           7       // 7 设定模式
#define DISP_MODE_RUN           8       // 8 运行模式
#define DISP_TIMING_USE         9       // 9 [!不用]定时使用(TRUE:使用,FALSE:不用)
#define DISP_TIMING_GRP         10      // 10 [!不用]定时组数(0表示使用前板的定时逻辑)
#define DISP_INIT_STATE         11      // 11 参数初始化状态
#define DISP_LMT_ON             12      // 12 使用期限到期(TRUE:到期,FALSE:未到)
#define DISP_LMT_SET_DAY        13      // 13 使用期限设定天数
#define DISP_LMT_RUN_HOUR       14      // 14 使用期限已运行小时数
#define DISP_LMT_LEFT_HOUR      15      // 15 使用期限剩余小时数
#define DISP_ERR_NUM            16      // 16 当前故障数
#define DISP_HIS_NUM            17      // 17 历史故障数
#define DISP_CHK_AV_NUM         18      // 18 [!不用]查询界面 模拟量个数
#define DISP_CHK_DV_NUM         19      // 19 [!不用]查询界面 开关量个数
#define DISP_CHK_EX_NUM         20      // 20 [!不用]查询界面 附加量个数
#define DISP_PARA_GRP_NUM       21      // 21 参数组数量
#define DISP_PASS_GRP_NUM       22      // 22 密码组数
#define DISP_DEFRO_USE          23      // 23 [!不用]允许手动除霜模块数
#define DISP_WIFI_USE           24      // 24 无线功能(TRUE:使用,FALSE:不用)
#define DISP_PRE_HEAT_TIME_MINT 25      // 25 预热剩余分钟数
#define DISP_TMP_SET_MIN        26      // 26 设定温度最小值(最大最小值同为0时前板不作范围判断)
#define DISP_TMP_SET_MAX        27      // 27 设定温度最大值
#define DISP_BOOT_DOWNLOAD      28      // 28 [!不用]是否允许升级后板程序(TRUE:允许,FALSE:不许)
#define DISP_TMP_OUT            29      // 29 当前系统出温
#define DISP_TMP_IN             30      // 30 当前系统回温
#define DISP_CTRL_OBJ           31      // 31 控制对象(0表示控出温，1表示控回温)
#define DISP_CHK_SYS_AV_NUM     32      // 32 系统模块模拟量个数
#define DISP_CHK_CMP_AV_STEP    33      // 33 压机模拟量步长
#define DISP_CHK_DO_STEP        34      // 34 DO输出步长
#define DISP_CHK_DI_STEP        35      // 35 DI输入步长
#define DISP_CHK_SYS_DO_NUM     36      // 36 系统开关量输出个数
#define DISP_SYS_COMP_RUN_NUM   37      // 37 系统已运行压机数
#define DISP_MODE_SET_INDEX     38      // 38 [!不用]设定模式索引(高位组号，低位项号)
#define DISP_TEST_EN            39      // 39 调试模式使用否
#define DISP_FORCE_DFRST_INFO_1 40      // 40 强制除霜显示信息1（2021.03.03增加除霜信息结构体后已弃用）
#define DISP_FORCE_DFRST_INFO_2 41      // 41 强制除霜显示信息2（2021.03.03增加除霜信息结构体后已弃用）
#define DISP_TMP_SET_DOT        42      // 42 设定温度小数点
#define DISP_CHK_USER_DO        43      // 43 用户查询的DO输出
#define DISP_PARA_GRP_SIZE      44      // 44 参数组大小
#define DISP_LMT_SET_MAX        45      // 45 使用期限设置天数最大值
#define DISP_HAVE_MNL_DEFRO     46      // 46 是否有手动除霜功能
#define DISP_RUN_ALLOW_SET_MODE 47      // 47 运行过程是否允许修改模式
#define DISP_HAVE_START_CODE    48      // 48 是否有启动机组密码（4.1寸屏早期遗留，固定FALSE）2021.03.03弃用
#define DISP_START_CODE         49      // 49 启动机组密码（4.1寸屏早期遗留，固定0）2021.03.03弃用
#define DISP_DISP_FG            50      // 50 辅助处理标志（位处理）
                                        //    bit0：是否支持单压机手动除霜
                                        //    bit1：是否有湿度控制
                                        //    bit2：是否有启动机组密码
                                        //    bit3：是否有初始化参数密码
                                        //    bit4：是否有跳过预热功能
                                        //    bit5：是否有快速预热功能
                                        //    bit6: 是否有BMS设置界面
                                        //    bit7: 中英文(0:中文  1:英文)
                                        //    bit8: 是否禁止中英文切换
                                        //    bit9: 是否为邦普变频器
                                        //    bit10:是否有密码查询界面
                                        //    bit11:是否有地暖烘干功能
                                        //    bit12:是否有强开辅助电热
                                        //    bit13:是否有强开水箱电热
                                        //    bit14:是否有强制制热水模式
                                        //    bit15:是否有假日模式设置界面
#define DISP_ALARM_SOUND_SET    51      // 51 蜂鸣器报警模式(0不响 1长响 2只响十秒 3周期十秒)
#define DISP_MODE_TYPE          52      // 52 厂家模式(0单热 1单冷 2热泵 3热水)
#define DISP_TMP_HOT            53      // 53 当前热水温度
#define DISP_HUMI_MODE_VALID    54      // 54 湿度可设模式
                                        //    当9~16位为0xff：从参数读取可设模式，5~8为参数组号，1~4位为参数项号；
                                        //    当9~16位非0xff：从该辅助信息读取可设模式，辅助信息为位变量（可用15种模式）
#define DISP_HUMI_MODE_SET      55      // 55 湿度设定模式
#define DISP_HUMI_MODE_RUN      56      // 56 湿度运行模式
#define DISP_HUMI_CURT          57      // 57 当前湿度
#define DISP_HUMI_SET           58      // 58 设定湿度
#define DISP_HUMI_SET_MIN       59      // 59 湿度设定最小值
#define DISP_HUMI_SET_MAX       60      // 60 湿度设定最大值
#define DISP_DISP_FG_EX         61      // 61 辅助处理标志(位处理)DISP_DISP_FG不够用了
                                        //    bit0: 是否有风盘信息界面
                                        //    bit1: 是否有能力测试界面
                                        //    bit2: 是否有第二个设定温度
                                        //    bit3: 是否有热水设定
                                        //    bit4: 是否有变频参数界面
                                        //    bit5: 是否有变频故障查询
                                        //    bit6: 是否有收氟功能
                                        //    bit7：是否禁用按键音
                                        //    bit8：是否有强开外部热源
                                        //    bit9：是否处于静音模式
                                        //    bit10:是否处于水泵试运行
                                        //    bit11:是否处于制冷试运行
                                        //    bit12:是否处于制热试运行
                                        //    bit13:是否处于制热水试运行
                                        //    bit14:是否处于系统排空调试
                                        //    bit15:是否使用童锁功能
#define DISP_CAN_DO_FG          62      // 62 辅助处理标志(位处理)  是否可以动作
                                        //    bit0: 是否可以强开辅助电热
                                        //    bit1: 是否可以强开水箱电热
#define DISP_TMP_SET_EX         63      // 63 第二个设定温度
#define DISP_TMP_CURT_EX        64      // 64 第二个当前温度
#define DISP_TMP_SET_EX_MIN     65      // 65 第二个设定温度最小值
#define DISP_TMP_SET_EX_MAX     66      // 66 第二个设定温度最大值
#define DISP_TMP_HOT_SET        67      // 67 热水设定温度
#define DISP_TMP_HOT_SET_MIN    68      // 68 热水设定温度最小值
#define DISP_TMP_HOT_SET_MAX    69      // 69 热水设定温度最大值
#define DISP_LMT_TYPE_INFO      70      // 70 系统维护类型信息
#define DISP_LMT_TYPE_VALUE     71      // 71 系统维护类型
#define DISP_INVT_ERR_NUM       72      // 72 变频故障个数
#define DISP_SILENT_LEVEL       73      // 73 静音模式级别
#define DISP_SILENT_LEVEL_ATTR  74      // 74 静音模式级别上下限
#define DISP_ROOM_TEMP_WR_INTVL 75      // 75 下发室内温度的间隔 单位秒，0表示使用前板默认值30秒
#define DISP_BKBD_LANGUAGE      76      // 76 后板语言
#define DISP_DISP_FG_EX2        77      // 77 辅助处理标志(位标志)DISP_DISP_FG_EX不够用了
                                        //    bit0: 是否在运行地暖预热
                                        //    bit1: 是否在运行定时地暖烘干
                                        //    bit2: 是否处于假日离开模式
                                        //    bit3: 是否处于假日在家模式
                                        //    bit4: 是否有4G功能
                                        //    bit5: 是否有远程升级请求
                                        //    bit6: 是否处于地暖烘干功能
                                        //    bit7: 是否处于辅助电热强开
                                        //    bit8: 是否处于水箱电热强开
                                        //    bit9: 是否处于强开热水模式
                                        //    bit10:是否处于强开外部热源
                                        //    bit11:是否处于制热限制
                                        //    bit12:是否模块号从1开始显示
                                        //    bit13:是否用户级以上(包括用户级)权限有初始化功能
                                        /*    bit14:是否显示开关量输入查询界面 */
                                        /*    bit15:是否有机型名称显示 */
#define DISP_DISP_LOGO          78      // 78 显示logo
#define DISP_DISP_FG_EX3        79      // 79 辅助处理标志(位标志)DISP_DISP_FG_EX2不够用了
                                        //    bit0: 是否有商检功能
                                        //    bit1: 是否有wifiOTA功能
                                        //    bit2：是否有3期使用期限
#define DISP_LANGUAGE_FG1       80      // 80 显示语言配置1, bit15~bit0对应彩屏语言定义的0~15
#define DISP_LANGUAGE_FG2       81      // 81 显示语言配置2, bit15~bit0对应彩屏语言定义的16~30
                                        // 注意如果语言个数超过31, 则需要修改3.5寸开机时的处理

#define DISP_INFO_MAX           90      // 最大显示信息字数,根据需要适当修改,减小堆栈压力,超出部分填0
                                        // 建议每次增大10,以便尽早发现堆栈问题
#define SH_MAX_DISP_INFO        100     // 最大显示信息字数

// 2 机组状态
#define SH_IDLE                 0           /* 待机 */
#define SH_READY                1           /* 启动 */
#define SH_RUN                  2           /* 运行 */
#define SH_DELAY                3           /* 停机 */
#define SH_ALARM                4           /* 故障 */
#define SH_DEFREZZ              5           /* 防冻 */
#define SH_DEFROST              6           /* 除霜 */
#define SH_WARM                 7           /* 预热 */
#define SH_CLR_SNOW             8           /* 除雪 */
#define SH_LINK                 9           /* 联锁：二通阀等联锁 */
#define SH_ECONOMY              10          /* 节能 */
#define SH_SILENT               11          /* 静音 */
#define SH_STANDARD             12          /* 标准 */
#define SH_STRONG               13          /* 强劲 */
#define SH_STER                 14          /* 杀菌 */
#define SH_DRYUP                15          /* 烘干 */
#define SH_HOLIDAY              16          /* 假日 */
#define SH_TEST                 17          /* 测试 */
#define SH_REOIL                18          /* 回油 */
#define SH_HEAT_LMT             19          /* 制热限制 */
#define SH_TEST_FAC             20          /* 商检 */
#define SH_MAC_STATUS_MAX       21          /* 最大状态数 */
#define SH_DEFREZZ_WARM         (0xff-1)//防冻预热

// 3、4 温度单位
#define SH_SET_UNIT_C           0           /* 摄氏度 */
#define SH_SET_UNIT_F           1           /* 华氏度 */
#define SH_SET_UNIT_PA          2           /* Pa */
#define SH_SET_UNIT_MAX         3           /* 最大设定单位 */

// 7、8 模式 (新增模式需要在前板增加模式字符串及模式动画图标)
#define SH_MODE_AUTO            0       // 0 自动(设定模式可以为自动，运行模式不能为自动)
#define SH_MODE_FAN             1       // 1 通风
#define SH_MODE_COOL            2       // 2 制冷
#define SH_MODE_HEAT            3       // 3 制热
#define SH_MODE_HOTWATER        4       // 4 热水
#define SH_MODE_ICE             5       // 5 蓄冰
#define SH_MODE_FLOOR           6       // 6 地暖
#define SH_MODE_COOL_HW         7       // 7 制冷+热水
#define SH_MODE_HEAT_HW         8       // 8 制热+热水
#define SH_MODE_MAX             9       //  最大模式数

// 11 参数初始化状态
#define SH_STATE_IDLE           0       // 未开始
#define SH_STATE_ING            1       // 进行中
#define SH_STATE_SUCC           2       // 成功
#define SH_STATE_FAIL           3       // 失败

// 31 控制对象
#define SH_CTRL_OBJ_OUT         0       //出温
#define SH_CTRL_OBJ_IN          1       //回温

// 43 用户查询界面所需的信息
#define SH_CHK_USER_DO_PUMP_BIT 0
#define SH_CHK_USER_DO_FAN_BIT  1
#define SH_CHK_USER_DO_COMP_BIT 2

// 50 辅助处理标志（位处理）
#define SH_HAVE_SINGLE_CM_MNL_DEFROST   (GETBIT(disp_info[DISP_DISP_FG], 0))/* bit0：是否支持单压机手动除霜 */
#define SH_HAVE_HUMI_CTRL               (GETBIT(disp_info[DISP_DISP_FG], 1))/* bit1：是否有湿度控制 */
#define SH_HAVE_START_CODE              (GETBIT(disp_info[DISP_DISP_FG], 2))/* bit2：是否有启动机组密码 */
#define SH_HAVE_RESET_PARA_PASS         (GETBIT(disp_info[DISP_DISP_FG], 3))/* bit3：是否有初始化参数密码 */
#define SH_HAVE_OFF_OILHEAT             (GETBIT(disp_info[DISP_DISP_FG], 4))/* bit4：是否有跳过预热功能 */
#define SH_HAVE_QUICK_OILHEAT           (GETBIT(disp_info[DISP_DISP_FG], 5))/* bit5：是否有快速预热功能 */
#define SH_HAVE_BMS_SET                 (GETBIT(disp_info[DISP_DISP_FG], 6))/* bit6: 是否有BMS设置界面 */
#define SH_IS_ENGLISH_DISP              (GETBIT(disp_info[DISP_DISP_FG], 7))/* bit7: 是否为英文显示 */
#define SH_FORBID_SET_LANGUAGE          (GETBIT(disp_info[DISP_DISP_FG], 8))/* bit8: 是否禁止中英文切换 */
#define SH_IS_PUNP_INVT                 (GETBIT(disp_info[DISP_DISP_FG], 9))/* bit9: 是否为邦普变频器 */
#define SH_HAVE_PASS_CHK                (GETBIT(disp_info[DISP_DISP_FG],10))/* bit10:是否有密码查询界面 */
#define SH_HAVE_FLOOR_DRYUP             (GETBIT(disp_info[DISP_DISP_FG],11))/* bit11:是否有地暖烘干功能*/
#define SH_HAVE_FORCE_IBH               (GETBIT(disp_info[DISP_DISP_FG],12))/* bit12:是否有强开辅助电热*/
#define SH_HAVE_FORCE_TBH               (GETBIT(disp_info[DISP_DISP_FG],13))/* bit13:是否有强开水箱电热*/
#define SH_HAVE_FORCE_HW_MODE           (GETBIT(disp_info[DISP_DISP_FG],14))/* bit14:是否有强制制热水模式*/
#define SH_HAVE_HOLIDAY_MODE            (GETBIT(disp_info[DISP_DISP_FG],15))/* bit15:是否有假日模式设置界面*/

// 51 蜂鸣器报警模式(注意：后板的定义是0不响 1长响 2只响十秒 3周期十秒，因此需要进行一次转换)
#define SH_ALARM_SOUND_SET_ON           0   /* 长响 */
#define SH_ALARM_SOUND_SET_OFF          1   /* 不响 */
#define SH_ALARM_SOUND_SET_10S_ONCE     2   /* 只响十秒 */
#define SH_ALARM_SOUND_SET_10S_CYCLE    3   /* 周期十秒 */

// 52 厂家模式(0单热 1单冷 2热泵 3热水)
#define SH_MODE_HEAT_ONLY       0
#define SH_MODE_COOL_ONLY       1
#define SH_MODE_HEAT_PUMP       2
#define SH_MODE_HOT_WATER       3

// 55、56 湿度模式(新增模式需要在前板增加模式字符串及模式动画图标)
#define SH_HUNI_NULL            0       // 0 自动(设定模式可以为自动，运行模式不能为自动)
#define SH_MODE_DEHUMI          1       // 1 除湿
#define SH_MODE_ADDHUMI         2       // 2 加湿
#define SH_HUMI_MODE_MAX        3       // 3 最大模式数

// 61 辅助处理标志（位处理）
#define SH_HAVE_FCUINFO             (GETBIT(disp_info[DISP_DISP_FG_EX], 0)) /* bit0: 是否有风盘信息界面 */
#define SH_HAVE_ABILITY_TEST        (GETBIT(disp_info[DISP_DISP_FG_EX], 1)) /* bit1: 是否有能力测试界面 */
#define SH_HAVE_SET_TMP_EX          (GETBIT(disp_info[DISP_DISP_FG_EX], 2)) /* bit2: 是否有第二个设定温度 */
#define SH_HAVE_SET_HOT             (GETBIT(disp_info[DISP_DISP_FG_EX], 3)) /* bit3: 是否有热水设定 */
#define SH_HAVE_INVT_PARA           (GETBIT(disp_info[DISP_DISP_FG_EX], 4)) /* bit4: 是否有变频参数界面 */
#define SH_HAVE_INVT_ERR            (GETBIT(disp_info[DISP_DISP_FG_EX], 5)) /* bit5: 是否有变频故障查询 */
#define SH_HAVE_RECYCLE_REFRI       (GETBIT(disp_info[DISP_DISP_FG_EX], 6)) /* bit6: 是否有收氟功能 */
#define SH_HAVE_FORBID_KEY_BUZZER   (GETBIT(disp_info[DISP_DISP_FG_EX], 7)) /* bit7：是否禁用按键音 */
#define SH_HAVE_FORCE_AHS           (GETBIT(disp_info[DISP_DISP_FG_EX], 8)) /* bit8：是否有强开外部热源 */
#define SH_IS_SILENT_MODE_ON        (GETBIT(disp_info[DISP_DISP_FG_EX], 9)) /* bit9：是否处于静音模式 */
#define SH_IS_PUMP_TEST_ON          (GETBIT(disp_info[DISP_DISP_FG_EX],10)) /* bit10:是否处于水泵试运行 */
#define SH_IS_COOL_TEST_ON          (GETBIT(disp_info[DISP_DISP_FG_EX],11)) /* bit11:是否处于制冷试运行 */
#define SH_IS_HEAT_TEST_ON          (GETBIT(disp_info[DISP_DISP_FG_EX],12)) /* bit12:是否处于制热试运行 */
#define SH_IS_HOTWATER_TEST_ON      (GETBIT(disp_info[DISP_DISP_FG_EX],13)) /* bit13:是否处于制热水试运行 */
#define SH_IS_SYSTEM_EMPTY_ON       (GETBIT(disp_info[DISP_DISP_FG_EX],14)) /* bit14:是否处于系统排空调试 */
#define SH_HAVE_CHILD_LOCK          (GETBIT(disp_info[DISP_DISP_FG_EX],15)) /* bit15:是否使用童锁功能 */

// 62 辅助处理标志（位处理）是否可以动作
#define SH_CAN_FORCE_IBH            (GETBIT(disp_info[DISP_CAN_DO_FG], 0))/* bit0: 是否可以强制开辅助电热 */
#define SH_CAN_FORCE_TBH            (GETBIT(disp_info[DISP_CAN_DO_FG], 1))/* bit1: 是否可以强制开水箱电热 */

// 70 系统维护类型信息
#define SH_LMT_TYPE_STRING_ATTR     (HIBYTE(disp_info[DISP_LMT_TYPE_INFO])) /* 系统维护类型选项 */
#define SH_LMT_TYPE_VALUE_MIN       (LOBYTE(disp_info[DISP_LMT_TYPE_INFO]) & 0x0F)
#define SH_LMT_TYPE_VALUE_MAX       ((LOBYTE(disp_info[DISP_LMT_TYPE_INFO]) & 0xF0)>>4)
#define SH_HAVE_LMT_TYPE_SET        (disp_info[DISP_LMT_TYPE_INFO] != 0)

// 71 使用期限类型值
#define SH_LMT_TYPE_STOP_UNIT       0
#define SH_LMT_TYPE_KEEP_RUN        1

// 74 静音级别上下限
#define SH_SILENT_LEVEL_MIN         (LOBYTE(disp_info[DISP_SILENT_LEVEL_ATTR]))
#define SH_SILENT_LEVEL_MAX         (HIBYTE(disp_info[DISP_SILENT_LEVEL_ATTR]))

// 76 语言
#define SH_LANGUAGE_CN      0               /* 中文 */
#define SH_LANGUAGE_EN      1               /* 英文 */
#define SH_LANGUAGE_ES      2               /* 西班牙语 */
#define SH_LANGUAGE_GR      3               /* 希腊 */
#define SH_LANGUAGE_FR      4               /* 法文 */
#define SH_LANGUAGE_IT      5               /* 意大利 */
#define SH_LANGUAGE_PL      6               /* 波兰 */
#define SH_LANGUAGE_CZ      7               /* 捷克 */
#define SH_LANGUAGE_PT      8               /* 葡萄牙 */
#define SH_LANGUAGE_TR      9               /* 土耳其 */
#define SH_LANGUAGE_GE      10              /* 德语 */
#define SH_LANGUAGE_NL      11              /* 荷兰 */
#define SH_LANGUAGE_SE      12              /* 瑞典 */
#define SH_LANGUAGE_DK      13              /* 丹麦 */
#define SH_LANGUAGE_NO      14              /* 挪威 */
#define SH_LANGUAGE_HG      15              /* 匈牙利 */
#define SH_LANGUAGE_BG      16				/* 保加利亚 */
#define SH_LANGUAGE_MAX     17

// 77 辅助处理标志（位处理）
#define SH_IS_FLOOR_PREHEAT_RUN     (GETBIT(disp_info[DISP_DISP_FG_EX2], 0))/* bit0: 是否在运行地暖预热 */
#define SH_IS_FLOOR_TIMER_RUN       (GETBIT(disp_info[DISP_DISP_FG_EX2], 1))/* bit1: 是否在运行定时地暖烘干 */
#define SH_IS_HOLIDAY_AWAY          (GETBIT(disp_info[DISP_DISP_FG_EX2], 2))/* bit2: 是否处于假日离开模式 */
#define SH_IS_HOLIDAY_HOME          (GETBIT(disp_info[DISP_DISP_FG_EX2], 3))/* bit3: 是否处于假日在家模式 */
#define SH_HAVE_4G_FUNCTION         (GETBIT(disp_info[DISP_DISP_FG_EX2], 4))/* bit4: 是否有4G功能 */
#define SH_HAVE_REMOTE_UPGRADE_REQ  (GETBIT(disp_info[DISP_DISP_FG_EX2], 5))/* bit5: 是否有远程升级请求 */
#define SH_IS_FLOOR_DRYUP_ON        (GETBIT(disp_info[DISP_DISP_FG_EX2], 6))/* bit6: 是否处于地暖烘干功能 */
#define SH_IS_FORCE_IBH_ON          (GETBIT(disp_info[DISP_DISP_FG_EX2], 7))/* bit7: 是否处于辅助电热强开 */
#define SH_IS_FORCE_TBH_ON          (GETBIT(disp_info[DISP_DISP_FG_EX2], 8))/* bit8: 是否处于水箱电热强开 */
#define SH_IS_FORCE_HOT_WATER_ON    (GETBIT(disp_info[DISP_DISP_FG_EX2], 9))/* bit9: 是否处于强开热水模式 */
#define SH_IS_FORCE_AHS_ON          (GETBIT(disp_info[DISP_DISP_FG_EX2],10))/* bit10:是否处于强开外部热源 */
#define SH_IS_HEAT_LIMIT            (GETBIT(disp_info[DISP_DISP_FG_EX2],11))/* bit11:是否处于制热限制 */
#define SH_IS_MOD_START_WITH_ONE    (GETBIT(disp_info[DISP_DISP_FG_EX2],12))/* bit12:是否模块号从1开始显示 */
#define SH_IS_USER_PWR_HAVE_CLR     (GETBIT(disp_info[DISP_DISP_FG_EX2],13))/* bit13:是否用户级以上(包括用户级)权限有初始化功能 */
#define SH_IS_DISP_DI               (GETBIT(disp_info[DISP_DISP_FG_EX2],14))/* bit14:是否显示开关量输入查询界面 */
#define SH_HAVE_MAC_VER_DISP        (GETBIT(disp_info[DISP_DISP_FG_EX2],15))/* bit15:是否有机型名称显示 */

// 79 辅助处理标志（位处理）
#define SH_HAVE_TEST_FAC            (GETBIT(disp_info[DISP_DISP_FG_EX3], 0))/* bit0: 是否有商检功能 */
#define SH_HAVE_WIFI_OTA            (GETBIT(disp_info[DISP_DISP_FG_EX3], 1))/* bit1: 是否有wifiOTA功能 */
#define SH_HAVE_3_TERM_LIMIT        (GETBIT(disp_info[DISP_DISP_FG_EX3], 2))/* bit2: 是否有3期使用期限 */
/***********************************************************************************
                                单位表
***********************************************************************************/
#define SH_UNIT_NULL            0       // 0 无
#define SH_UNIT_TMP_C           1       // 1 温度(℃)
#define SH_UNIT_TMP_F           2       // 2 温度(℉)
#define SH_UNIT_A               3       // 3 电流(A)
#define SH_UNIT_mA              4       // 4 电流(mA)
#define SH_UNIT_PRS_B           5       // 5 压力(bar)
#define SH_UNIT_PE              6       // 6 百分比(%)
#define SH_UNIT_HZ              7       // 7 频率(HZ)
#define SH_UNIT_RPS             8       // 8 转速(rps)
#define SH_UNIT_RPM             9       // 9 转速(rpm)
#define SH_UNIT_YEAR            10      // 10 年
#define SH_UNIT_MONTH           11      // 11 月
#define SH_UNIT_DAY             12      // 12 天
#define SH_UNIT_HOUR            13      // 13 时
#define SH_UNIT_MINT            14      // 14 分
#define SH_UNIT_SECOND          15      // 15 秒
#define SH_UNIT_ST              16      // 16 步
#define SH_UNIT_V               17      // 17 电压(V)
#define SH_UNIT_mV              18      // 18 电压(mV)
#define SH_UNIT_kO              19      // 19 电阻(kΩ)
#define SH_UNIT_kW              20      // 20 功率(kW)
#define SH_UNIT_PA              21      // 21 压力(Pa)
#define SH_UNIT_LH              22      // 22 流量(L/H)
#define SH_UNIT_kWh             23      // 23 用电量(kWh)

#define SH_UNIT_MAX             24      //最大单位数

/***********************************************************************************
                                查询信息
***********************************************************************************/
#define NO_ID                   0xff    //0xff不显示序号
typedef struct tagSH_CHK_INFO           //状态信息
{
    U16 name;                           //名称字符串号
    U16 id;                             //序号(高8位为前缀号，低8位为后缀序号，为0xff时不显示序号)
    I16 value;                          //数值(如果是字符型则表示字符串号)
    U16 attrib;                         //属性(0:字符型、1~8:单位、9~11:小数位)
}SH_CHK_INFO;
#define SH_MAX_CHK_INFO         128     //最大可显示查询信息数

#define SH_MAX_CHK_USER_INFO    6
#define SH_MAX_CHK_SYS_INFO     32
#define SH_MAX_CHK_COMP_INFO    24
#define SH_MAX_CHK_DO_INFO      20
#define SH_MAX_CHK_DI_INFO      20

/***********************************************************************************
                                版本信息
***********************************************************************************/
#define MAX_VER_BYTE            32      //版本号字节数

typedef struct tagSH_CHK_VER            //版本信息
{
    U16 pcb_name;                       //名称字符串号
    U16 pcb_id;                         //板号(为0xff时不显示板号，为0xffff时不显示该板版本号)
    U08 fun_code[MAX_VER_BYTE];         //功能码
}SH_CHK_VER;
#define SH_MAX_PCB_NUM          16      //最大板数
#define SH_MAX_COMP_NUM         4       //最大板压机
#define SH_MAX_EXIO_NUM         4       //最大扩展板个数



/***********************************************************************************
                                故障信息
***********************************************************************************/
typedef struct tagSH_ERR_TIME           //故障时间
{
    U16 month;                          //月 (月日同为0则不显示月日)
    U16 day;                            //日
    U16 hour;                           //时 (时分同为0则不显示时分)
    U16 mint;                           //分
}SH_ERR_TIME;

typedef struct tagSH_ERR_HIS            //历史故障信息
{
    U16 name;                           //名称字符串号
    U16 id;                             //序号(高8位为板号，低8位为单元号，为0xff时不显示序号)
    U16 err_id;                         //故障号
    SH_ERR_TIME time;                   //故障时间
}SH_ERR_HIS;
#define SH_MAX_ERR_HIS_NUM      100     //最大历史故障数
#define SH_ERR_WEI_ZHI_GZ       0

typedef struct tagSH_ERR_CURT           //当前故障信息
{
    U16 name;                           //名称字符串号
    U16 id;                             //序号(高8位为板号，低8位为单元号，为0xff时不显示序号)
    U16 err_id;                         //故障号
}SH_ERR_CURT;
#define SH_MAX_ERR_CURT_NUM     100     //最大当前故障数



/***********************************************************************************
                                除霜信息
***********************************************************************************/
typedef struct tagSH_DEFRO_INFO
{
    U16 cell;                           // 模块除霜信息,0~3:模块除霜信息,4~7:模块压机个数
    U16 cms;                            // 压机除霜信息(一个压机占2位)
}SH_DEFRO_INFO;

enum
{
    SH_DEFR_NO_RUN=0,
    SH_DEFR_RUN,
    SH_DEFR_FORCE_DEFORST,
    SH_DEFR_DEFROST
};

/***********************************************************************************
                                使用/禁用信息
***********************************************************************************/
typedef struct tagSH_ENABLE_INFO
{
    U16 pcb_enable;            // 模块禁用信息,每个模块一个位
    U16 pcb_commu_err;         // 模块通信故障信息
    U16 bak[2];                // 保留2个字
    U16 comp_enable[SH_MAX_COMP_NUM]; // 压机禁用信息,每个模块一个位
}SH_ENABLE_INFO;
#if SH_MAX_PCB_NUM > 16
    #error "模块数超16,修改变量类型!"
#endif


/***********************************************************************************
                                参数信息
***********************************************************************************/
//属性掩码
#define SH_UNIT         0x00ff          //bit0~7:   0~255，若值小于32,表示为数值型，值为单位号，最多支持32个单位；若值大于32，表示为字符串型参数，值减去偏差值32之后就是字符组号
#define SH_DECIMAL      0x0300          //bit8~9:   小数位数(0~3)
#define SH_HIDE         0x0400          //bit10:    该位为1表示"隐藏"、为0表示"非隐藏"
#define SH_CONST        0x0800          //bit11:    该位为1表示"不可改"、为0表示"可修改"

#define SH_UNSIGNED     0x0800          //bit11:    复用为无符号数属性

#define SH_SAFE         0x7000          //bit12~14 权限：0：用户， 1：工程， 2：维修， 3：厂家，
#define SH_BIT          0x8000          //bit15:    该位为1表示"位变量"、为0表示"非位变量" 挪至最后一位，若有其他需要时就取消位变量属性
                                        //          当参数为“位变量”时，占用2~3个字符组号。
                                        //          首个字符组为“置位、清零对应字符”，第2字符组为0~7位对应字符串，第3字符组为8~15位对应字符串。
                                        //          首个字符组组号保存在 SH_UNIT 。

#define UNIT_OFFSET     32              // 单位偏移量
#define DECIMAL_BGN_BIT 8               // 小数位数 起始位
#define HIDE_BGN_BIT    10              // 隐藏属性 起始位
#define CONST_BGN_BIT   11              // 不可修改 起始位
#define SAFE_BGN_BIT    12              // 参数权限 起始位
#define BIT_BGN_BIT     15              // 位变量   起始位


typedef struct tagSH_PARA_GRP           //参数组信息
{
    U16 name;                           //名称字符串号
    U16 item_num;                       //组内参数个数
    U16 attrib;                         //属性(1~8:权限、12:隐藏)
}SH_PARA_GRP;
#define SH_MAX_PARA_GRP_NUM     40      //最大参数组数


typedef struct tagSH_PARA_ATTRIB        //参数项属性
{
    U16 name;                           //名称字符串号
    U16 id;                             //序号(高8位为前缀号，低8位为后缀序号，为0xff时不显示序号)
    I16 value;                          //参数值
    I16 min;                            //最小值
    I16 max;                            //最大值
    U16 attrib;                         //属性(0:字符串型、1~8:单位、9~11:小数位、12:隐藏、13:不可修改、14:位变量)
}SH_PARA_ATTRIB;
#define SH_MAX_PARA_ITEM_NUM    50      //最大参数项数

#define SH_MAX_PARA_ITEM_TOTAL  1280

#define SH_MAX_PARA_STRING_GRP  100     //最大字符串组
#define SH_MAX_DICT_STR         400     //最多字典数        /* 涉及到这个的函数中可能有U08的变量，需注意 */
typedef struct tagSH_DICT_ATTR
{
    U16 idx_ofs[SH_MAX_PARA_STRING_GRP];
}SH_DICT_ATTR;

typedef struct tagSH_CHK_INFO_NUM      //查询单板信息个数
{
    U16 cell_pv_num;                   //单元模拟量个数
    U16 comp_pv_num;                   //压机模拟量个数
    U16 do_num;                        //开关量输出个数
    U16 di_num;                        //开关量输入个数
}SH_CHK_INFO_NUM;

#define SH_STR_BEI_YONG         0      // 0 备用
#define SH_STR_WEI_ZHI          1      // 1 未知
#define SH_STR_WU_XIAN_S        2      // 2 无显示
#define SH_MAX_BMS_PARA_NUM     5      //最大BMS通讯参数项数



/***********************************************************************************
                                时钟
***********************************************************************************/
typedef struct tagSH_CLOCK
{
    U16 year;                       //年:2000~2099
    U16 month;                      //月:1~12
    U16 day;                        //日:1~31
    U16 week;                       //星期:0~6
    U16 hour;                       //时:0~23
    U16 min;                        //分:0~59
    U16 sec;                        //秒:0~59
}SH_CLOCK;



/***********************************************************************************
                                定时
***********************************************************************************/
typedef struct tagSH_TIMING
{
    U16 hour;                       //时:0~23
    U16 min;                        //分:0~59
    U16 info;                       //定时信息: bit0~bit6:星期一~星期天；bit7:0关/1开
}SH_TIMING;
#define SH_MAX_TIMING_GRP   12      //最大定时组数



/***********************************************************************************    
                                3期使用期限
***********************************************************************************/
#define SH_MAX_LIMIT_GRP   3 
typedef struct tagSH_LIMIT
{
    U16 limit_sign;                       
    SH_PARA_ATTRIB limit_info[SH_MAX_LIMIT_GRP];                        
}SH_LIMIT;

#define SH_USE_FIRST_LIMIT           0/* bit0: 第一期是否使用 */
#define SH_USE_SECOND_LIMIT          1/* bit1: 第二期是否使用 */
#define SH_USE_THIRD_LIMIT           2/* bit2: 第三期是否使用 */
#define SH_EXPIRE_FIRST_LIMIT        3/* bit3: 第一期是否到期 */
#define SH_EXPIRE_SECOND_LIMIT       4/* bit4: 第二期是否到期 */
#define SH_EXPIRE_THIRD_LIMIT        5/* bit5: 第三期是否到期 */
/***********************************************************************************    
                                密码
***********************************************************************************/
//身份
#define SH_PWR_NONE         0       //未登录
#define SH_PWR_SALE         1       //经销商(使用期限)
#define SH_PWR_USER         2       //用户
#define SH_PWR_PRO          4       //工程调试员
#define SH_PWR_SER          6       //厂家维修员
#define SH_PWR_OMNI         7       //万能解锁密码
#define SH_PWR_FAC          9       //厂家管理员
#define SH_PWR_SPR          10      //超级密码
#define SH_PWR_TERM1        11      //期限1密码
#define SH_PWR_TERM2        12      //期限2密码
#define SH_PWR_TERM3        13      //期限3密码

#define SH_PWR_INIT_PARA    14      //初始化参数密码
#define SH_PWR_START_MAC    15      //启动机组密码

//密码
#define MAX_PASS_BIT        8       //最大密码位数

typedef struct tagSH_PASS
{
    U16 authority;                  //权限级别
    U08 pass_bit[MAX_PASS_BIT];     // 8位密码
}SH_PASS;
#define SH_MAX_PASS_GRP     16      //最大密码组数



/***********************************************************************************
                                参数权限
***********************************************************************************/
#define SH_PARA_USR         0       //用户
#define SH_PARA_PRO         1       //工程
#define SH_PARA_SER         2       //维修
#define SH_PARA_FAC         3       //厂家

/***********************************************************************************
                                命令表
***********************************************************************************/
#define SH_CMD_ON                           0       // 0 开机
#define SH_CMD_OFF                          1       // 1 关机
#define SH_CMD_RST                          2       // 2 故障复位
#define SH_CMD_PASS_RST                     3       // 3 密码故障复位
#define SH_CMD_OFF_OILHEAT                  4       // 4 取消预热
#define SH_CMD_DEFRO                        5       // 5 压机手动除霜
#define SH_CMD_PARA_INIT                    6       // 6 参数初始化
#define SH_CMD_CM_RUNTM_INIT                7       // 7 压机累计运行时间初始化
#define SH_CMD_SYS_RUNTM_INIT               8       // 8 系统累计运行时间初始化
#define SH_CMD_HIS_ERR_INIT                 9       // 9 清空历史故障
#define SH_CMD_INVERTER_INIT                10      // 10 初始化变频器
#define SH_CMD_PUMP                         11      // 11 手动开关水泵
#define SH_CMD_BMS_PARA_SET                 12      // 12 BMS参数设置(接收数据高8位为项号、低8位为值)
#define SH_CMD_QUICK_OILHEAT                13      // 13 快速预热
#define SH_CMD_TUYA_NEW_LINK                14      // 14 涂鸦重新连接标志
#define SH_CMD_INVT_INFO_RD                 15      // 15 变频信息读取(接收数据高8位模块号、低8为压机号)
#define SH_CMD_ENTER_FLOOR_DRYUP            16      // 16 进入地暖烘干功能
#define SH_CMD_FORCE_IBH                    17      // 17 强开辅助电热
#define SH_CMD_FORCE_TBH                    18      // 18 强开水箱电热
#define SH_CMD_FORCE_HW_MODE                19      // 19 强开热水模式
#define SH_CMD_ABILITY_TEST                 20      // 20 能力测试
#define SH_CMD_ON_ZONE_1                    21      // 21 区域一开机
#define SH_CMD_OFF_ZONE_1                   22      // 22 区域一关机
#define SH_CMD_ON_ZONE_2                    23      // 23 区域二开机
#define SH_CMD_OFF_ZONE_2                   24      // 24 区域二关机
#define SH_CMD_ON_HOTW                      25      // 25 热水开机
#define SH_CMD_OFF_HOTW                     26      // 26 热水关机
#define SH_CMD_LIMIT_INIT                   27      // 27 系统维护初始化
#define SH_CMD_RECYCLE_REFRI                28      // 28 收氟功能
#define SH_CMD_TIMER_DHW_ON                 29      // 29 定时开热水泵
#define SH_CMD_FORCE_AHS                    30      // 30 强开外部热源
#define SH_CMD_DISINFECT                    31      // 31 杀菌
#define SH_CMD_SILENT                       32      // 32 静音模式
#define SH_CMD_PUMP_TEST                    33      // 33 水泵试运行
#define SH_CMD_COOL_TEST                    34      // 34 制冷试运行
#define SH_CMD_HEAT_TEST                    35      // 35 制热试运行
#define SH_CMD_HOTWATER_TEST                36      // 36 制热水试运行
#define SH_CMD_SYSTEM_EMPTY                 37      // 37 系统排空调试
#define SH_CMD_FLOOR_PREHEAT                38      // 38 地暖预热命令
#define SH_CMD_FLOOR_TIMER                  39      // 39 定时地暖命令
#define SH_CMD_HOLIDAY_AWAY                 40      // 40 假日离开命令
#define SH_CMD_HOLIDAY_HOME                 41      // 41 假日在家命令
#define SH_CMD_ACCEPT_REMOTE_UPGRADE_REQ    42      // 42 接受远程升级请求
#define SH_CMD_INTO_TEST_FAC                43      // 43 进入商检

#define SH_MAX_CMD                          200     //最大命令数



/***********************************************************************************
                                设置信息
***********************************************************************************/
#define SET_TMP_SET             0       // 0 目标温度(1位小数)
#define SET_MODE_SET            1       // 1 设定模式
#define SET_LIMIT_TIME          2       // 2 使用期限天数(0表示不使用)
#define SET_TIMING_USE          3       // 3 定时使用(TRUE:使用,FALSE:不用)
#define SET_START_CODE          4       // 4 清除启动机组密码功能的使用
#define SET_HUMI_MODE           5       // 5 设定湿度模式
#define SET_LANGUAGE            6       // 6 设定语言
#define SET_TMP_SET_ZONE_1      7       // 7 区域一目标温度
#define SET_TMP_SET_ZONE_2      8       // 8 区域二目标温度
#define SET_TMP_SET_ZONE_HOTW   9       // 9 热水目标温度
#define SET_LIMIT_TYPE          10      // 10设置维护类型
#define SET_SILENT_LEVEL        11      // 11设置静音级别
#define SET_TEMP_UNIT           12      // 12温度单位

#define SH_MAX_SET_INFO         200     // 最大显示信息字数



/***********************************************************************************
                                调试信息
***********************************************************************************/
#define MAX_CHK_TEST_DO_NUM     48
#define MAX_CHK_TEST_EEV_NUM    8
#define CHK_TEST_DO_WORD        _DIVUCARRY(MAX_CHK_TEST_DO_NUM, 16) /* 根据上面的DO_NUM决定 */
#define CHK_TEST_EEV_WORD       _DIVUCARRY(MAX_CHK_TEST_EEV_NUM, 16)/* 根据上面的EEV_NUM决定 */

typedef struct tagSH_CHK_TEST
{
    struct
    {
        U16 str_DO[MAX_CHK_TEST_DO_NUM];        // DO字符串
        U16 str_EEV[MAX_CHK_TEST_EEV_NUM];      // EEV字符串
        U16 DO_num;                             // DO数量
        U16 EEV_num;                            // EEV数量
        U16 EEV_step_min[MAX_CHK_TEST_EEV_NUM]; // EEV手动步数最小值
        U16 EEV_step_max[MAX_CHK_TEST_EEV_NUM]; // EEV手动步数最大值
        U16 EEV_step_pv[MAX_CHK_TEST_EEV_NUM];  // EEV当前步数
        U16 fg_DO_can_mnl_mode;                 // DO是否允许手动模式
        U16 fg_EEV_can_mnl_mode;                // EEV是否允许手动模式
    }ro;

    struct
    {
        U16 EEV_mnl_mode[CHK_TEST_EEV_WORD];    // EEV手动模式
        U16 EEV_step_sv[MAX_CHK_TEST_EEV_NUM];  // EEV手动步数
        U16 DO_mnl_mode;                        // DO手动模式
    }wo;

    struct
    {
        U16 DO_sta[CHK_TEST_DO_WORD];           // DO的值 当控制模式为自动模式时，为后板写；当控制模式为手动时，为前板写
    }wr;
}SH_CHK_TEST;

/***********************************************************************************
                                涂鸦数据
***********************************************************************************/
#ifndef TUYA_F_H                        /* 此部分默认前板已定义 */
#define TUYA_F_H
/* 产品信息数据 */
typedef struct tag_TUYA_INFO
{
    U08 dpid_num;           /* 需要上报的功能点个数 */
    U08 product_id[16];     /* 产品ID */
    U08 bak;                /* 备用，使结构体字节数为偶数 */
}TUYA_INFO;
#define TUYA_INFO_SIZE                  (sizeof(TUYA_INFO)+1) / sizeof(U16)

/* 功能点数据 */
typedef struct tag_TUYA_DATA
{
    U08 dpid;                           /* DPID */
    U08 dp_type;                        /* 功能点类型 */
    U08 data_buf[244];                  /* 传输数据 */
}TUYA_DATA;
#define TUYA_DATA_SIZE                  (sizeof(TUYA_DATA)+1) / sizeof(U16)

/* "功能指令"部分长度(字节数) */
#define RAW_DATA_BYTES                  (244)  /* 透传型 */
#define BOOL_DATA_BYTES                 (1)    /* 布尔型 */
#define NUMBER_DATA_BYTES               (4)    /* 数值型 */
#define STRING_DATA_BYTES               (244)  /* 字符型 */
#define ENUM_DATA_BYTES                 (1)    /* 枚举型 */
#define FAULT_DATA_BYTES                (1)    /* 故障型 */
#endif
#define SH_MAX_TUYA_DATA_NUM    40

/***********************************************************************************
                                变频信息
***********************************************************************************/
#define SH_MAX_INVT_INFO    (2+20)  // 变频信息最大个数
                                    // 1 高8位模块号、低8位压机号 0xffff无效
                                    // 1 高8位是否存在变频器通讯故障、低8位是否变频
                                    // 20 变频信息
typedef struct tagSH_INVTINFO
{
    U16 pcb_used;           /* 模块是否使用 */
    U16 pcb_commu_error;    /* 模块是否通讯故障 */
    I16 invt_info[SH_MAX_INVT_INFO];        /* 变频信息 */
}SH_INVTINFO;

/***********************************************************************************
                                假日模式
***********************************************************************************/
typedef struct tagSH_HOLIDAYINFO
{
    U16 mode_heat_use;
    U16 mode_hotw_use;
    U16 bak[8];
}SH_HOLIDAYINFO;

/***********************************************************************************
                               风盘信息
***********************************************************************************/
#define SH_FCU_MAX          (32)    // 最大风盘个数

typedef struct tagSH_FCUINFO
{
    I16 temp_indoor;
    U16 str_mode;
    U16 str_status;
}SH_FCUINFO;

typedef struct tagSH_FCU
{
    U16 use_num;                    // 使用个数
    SH_FCUINFO fcuinfo[SH_FCU_MAX];
}SH_FCU;


/***********************************************************************************
                               模式信息
***********************************************************************************/
typedef struct
{
    U16 mode_str;
    U16 mode_key;
    U16 mode_cmd_on;
    U16 mode_cmd_off;
    I16 temp_set_min;
    I16 temp_set_max;
    U16 temp_set_cmd;
}SH_MODE_INFO;
#define SH_MODE_INFO_MAX    10
typedef struct
{
    U16 mode_num;
    SH_MODE_INFO mode_info[SH_MODE_INFO_MAX];
}SH_MODE_SET_INFO;

/***********************************************************************************
                               主界面信息
***********************************************************************************/
enum
{
    ZONE_MODE_FCU,
    ZONE_MODE_RAD,
    ZONE_MODE_FLOOR,
    ZONE_MODE_HOTW,
};
enum
{
    ZONE_TEMP_WATER_FLOW,
    ZONE_TEMP_ROOM,
};
enum
{
    ZONE_STA_NULL,
    ZONE_STA_ANTIFREZ,
    ZONE_STA_DEFRO,
    ZONE_STA_HOLIDAY,
    ZONE_STA_SILENT,
    ZONE_STA_ECO,
    ZONE_STA_TEST,
    ZONE_STA_RECYCLE,
};
enum
{
    ZONE_ADDITION_NULL,
    ZONE_ADDITION_HEAT,
    ZONE_ADDITION_ELEC,
    ZONE_ADDITION_SOLAR,
    ZONE_ADDITION_TANK_HEATER,
};
enum
{
    SMART_GRID_NULL,
    SMART_GRID_FREE,
    SMART_GRID_VALLEY,
    SMART_GRID_PEAK,
};
typedef struct
{
    U16 zone_1_mode;
    U16 fg_zone_1_is_run;
    U16 zone_1_temp_type;
    U16 fg_zone_1_can_set_temp;
    U16 fg_zone_1_comp_on;
    U16 zone_1_status;
    U16 zone_1_addition;

    U16 zone_2_mode;
    U16 fg_zone_2_is_run;
    U16 zone_2_temp_type;
    U16 fg_zone_2_can_set_temp;
    U16 fg_zone_2_comp_on;
    U16 zone_2_status;
    U16 zone_2_addition;

    U16 fg_zone_hotw_is_run;
    U16 fg_zone_hotw_disinfect;
    U16 fg_zone_hotw_comp_on;
    U16 zone_hotw_status;
    U16 zone_hotw_addition;

    U16 fg_pumpi_is_on;
    U16 smart_grid_sta;
}SH_MAIN_INFO;

/***********************************************************************************
                               变频参数信息
***********************************************************************************/
typedef struct
{
    I16 addr;
    I16 val;
    U16 str_tips;
}SH_INVT_PARA;

#define SH_MAX_INVT_ERR         50

#define SH_MAX_FLOOR_PREHEAT_INFO   3       /* 地暖预热参数个数 */

#define SH_MAX_FLOOR_TIMER_INFO     4       /* 定时地暖烘干参数个数 */


/***********************************************************************************
                                4G信息
***********************************************************************************/
#define SH_MAX_4G_VER_WORD  16  // !!!该定义需要与控制板一致
#define SH_MAX_4G_SN_WORD   16
typedef struct
{
    struct
    {
        U16 name;
        U16 value;
    }csq;                   /* 信号强度 */
    struct
    {
        U16 name;
        U16 value;
        U16 value_string;
    }sta;                   /* 运行状态 */
    struct
    {
        U16 name;
        U16 info[SH_MAX_4G_VER_WORD];
    }ver;                   /* 版本 */
    struct
    {
        U16 name;
        U16 info[SH_MAX_4G_SN_WORD];
    }sn;                    /* sn */
}SH_4G_INFO;

#define SH_4G_STA_NONE          0   // 无
#define SH_4G_STA_NO_MODULE     1   // 无模组
#define SH_4G_STA_CANCAT        2   // 已连接模组
#define SH_4G_STA_CONNECT_NET   3   // 已连接网络
#define SH_4G_STA_CONNECT_CLOUD 4   // 已连接云
#define SH_4G_STA_ALARM         5   // 故障

typedef enum
{
    SH_REMOTE_REQ_WAIT,
    SH_REMOTE_REQ_OK,
    SH_REMOTE_REQ_CANCEL,
    SH_REMOTE_REQ_NULL,
}SH_REMOTE_REQ;

typedef struct
{
    struct
    {
        U16 fg_TF_is_on;
        U16 wifi_ssid[32];
        U16 wifi_password[32];
    }rd;
    struct
    {
        U16 wifi_sta;
    }wr;
}SH_TEST_FAC_HMI_STRU;
enum
{
    TEST_FAC_WIFI_STA_TESTING,
    TEST_FAC_WIFI_STA_NO_MODULE,
    TEST_FAC_WIFI_STA_SUCC,
    TEST_FAC_WIFI_STA_FAIL,
};

/***********************************************************************************
                                通讯地址
***********************************************************************************/
//计算SIZE时分子加1是为了在stm32和stm8平台上保持一致。

/***********************************************************************************
                                可读写区 通讯地址
***********************************************************************************/
//命令
#define ADDR_SH_CMD_BGN             0
#define SH_CMD_SIZE                 (1)
#define ADDR_SH_CMD_END             (ADDR_SH_CMD_BGN + SH_CMD_SIZE*SH_MAX_CMD)        // 200

//设置
#define ADDR_SH_SET_BGN             200
#define SH_SET_SIZE                 (1)
#define ADDR_SH_SET_END             (ADDR_SH_SET_BGN + SH_SET_SIZE*SH_MAX_SET_INFO)   // 400

//密码
#define ADDR_SH_PASS_BGN            400
#define SH_PASS_SIZE                ((sizeof(SH_PASS)+1) / sizeof(U16))// 5
#define ADDR_SH_PASS_END            (ADDR_SH_PASS_BGN + SH_PASS_SIZE*SH_MAX_PASS_GRP) // 480

//时钟
#define ADDR_SH_CLOCK_BGN           500
#define SH_CLOCK_SIZE               ((sizeof(SH_CLOCK)+1) / sizeof(U16))// 7
#define ADDR_SH_CLOCK_END           (ADDR_SH_CLOCK_BGN + SH_CLOCK_SIZE)               // 507

//定时
#define ADDR_SH_TIMING_BGN          550
#define SH_TIMING_SIZE              ((sizeof(SH_TIMING)+1) / sizeof(U16))// 3
#define ADDR_SH_TIMING_END          (ADDR_SH_TIMING_BGN + SH_TIMING_SIZE*SH_MAX_TIMING_GRP)   // 586

//假日模式
#define ADDR_SH_HOLIDAYINFO_BGN     590
#define SH_HOLIDAYINFO_SIZE         ((sizeof(SH_HOLIDAYINFO)+1) / sizeof(U16))  // 10
#define ADDR_SH_HOLIDAYINFO_END     (ADDR_SH_HOLIDAYINFO_BGN + SH_HOLIDAYINFO_SIZE)   // 600

//室内温度
#define ADDR_ROOM_TEMP_BGN          603
#define SH_ROOM_TEMP_SIZE           (1) // 1
#define ADDR_SH_ROOM_TEMP_END       (ADDR_ROOM_TEMP_BGN + SH_ROOM_TEMP_SIZE)   // 604

//变频参数
#define ADDR_SH_INVTPARA_BGN        610
#define SH_INVTPARA_SIZE            ((sizeof(SH_INVT_PARA)+1) / sizeof(U16))    // 3
#define ADDR_SH_INVTPARA_END        (ADDR_SH_INVTPARA_BGN + SH_INVTPARA_SIZE)   // 613

/* 地暖预热信息 */
#define ADDR_SH_FLOOR_PREHEAT_VAL_BGN   620
#define SH_FLOOR_PREHEAT_VAL_SIZE       (1) /* 1 */
#define ADDR_SH_FLOOR_PREHEAT_VAL_END   (ADDR_SH_FLOOR_PREHEAT_VAL_BGN + SH_FLOOR_PREHEAT_VAL_SIZE * SH_MAX_FLOOR_PREHEAT_INFO) /* 622 */

/* 定时地暖烘干信息 */
#define ADDR_SH_FLOOR_TIMER_VAL_BGN     625
#define SH_FLOOR_TIMER_VAL_SIZE         (1) /* 1 */
#define ADDR_SH_FLOOR_TIMER_VAL_END     (ADDR_SH_FLOOR_TIMER_VAL_BGN + SH_FLOOR_TIMER_VAL_SIZE * SH_MAX_FLOOR_TIMER_INFO)   /* 628 */

//参数值
#define ADDR_SH_PARA_BGN            2000
#define SH_PARA_SIZE                (1)
#define ADDR_SH_PARA_END            (ADDR_SH_PARA_BGN + SH_PARA_SIZE*SH_MAX_PARA_ITEM_TOTAL)   // 3280

//调试模式数据
#define ADDR_SH_CHK_TEST_BGN        3300


#define ADDR_SH_CHK_TEST_WO_BGN     (ADDR_SH_CHK_TEST_RO_BGN+SH_CHK_TEST_RO_SIZE)               // 3384
#define SH_CHK_TEST_WO_SIZE         ((sizeof(((SH_CHK_TEST *)NULL)->wo)+1) / sizeof(U16))
#define ADDR_SH_CHK_TEST_WO_END     (ADDR_SH_CHK_TEST_WO_BGN+SH_CHK_TEST_WO_SIZE)               // 3394

#define ADDR_SH_CHK_TEST_WR_BGN     (ADDR_SH_CHK_TEST_WO_BGN+SH_CHK_TEST_WO_SIZE)               // 3394
#define SH_CHK_TEST_WR_SIZE         ((sizeof(((SH_CHK_TEST *)NULL)->wr)+1) / sizeof(U16))
#define ADDR_SH_CHK_TEST_WR_END     (ADDR_SH_CHK_TEST_WR_BGN+SH_CHK_TEST_WR_SIZE)               // 3397

#define ADDR_SH_PASS_BIG_ENDIAN_BGN (3400)
#define ADDR_SH_PASS_BIG_ENDIAN_END (3401)

/* wifi商检读 */
#define ADDR_SH_TEST_FAC_HMI_RD_BGN (3500)
#define SH_TEST_FAC_HMI_RD_SIZE     ((sizeof(((SH_TEST_FAC_HMI_STRU*)NULL)->rd)+1) / sizeof(U16))   /* 65 */
#define ADDR_SH_TEST_FAC_HMI_RD_END (ADDR_SH_TEST_FAC_HMI_RD_BGN + SH_TEST_FAC_HMI_RD_SIZE)

/* wifi商检写 */
#define ADDR_SH_TEST_FAC_HMI_WR_BGN (ADDR_SH_TEST_FAC_HMI_RD_END)
#define SH_TEST_FAC_HMI_WR_SIZE     ((sizeof(((SH_TEST_FAC_HMI_STRU*)NULL)->wr)+1) / sizeof(U16))   /* 1 */
#define ADDR_SH_TEST_FAC_HMI_WR_END (ADDR_SH_TEST_FAC_HMI_WR_BGN + SH_TEST_FAC_HMI_WR_SIZE)

/* 涂鸦信息 */
#define ADDR_SH_TUYA_INFO_BGN       (3900)                                                      /* 3900 */
#define SH_TUYA_INFO_SIZE           ((sizeof(TUYA_INFO)+1) / sizeof(U16))                       /* 28 */
#define ADDR_SH_TUYA_INFO_END       (ADDR_SH_TUYA_INFO_BGN+SH_TUYA_INFO_SIZE)                   /* 3928 */

/*3期使用期限*/
#define ADDR_SH_LIMIT_BGN           (4000)                                                      /* 4000 */
#define SH_LIMIT_SIZE               ((sizeof(SH_LIMIT)+1) / sizeof(U16))                       
#define ADDR_SH_LIMIT_END           (ADDR_SH_LIMIT_BGN+SH_LIMIT_SIZE)  
/* 涂鸦数据 */
#define ADDR_SH_TUYA_DATA_BGN       (8920)      /* 8920 */
#define SH_TUYA_DATA_SIZE           ((sizeof(TUYA_DATA)+1) / sizeof(U16))   /* 123 */
#define ADDR_SH_TUYA_DATA_END       (ADDR_SH_TUYA_DATA_BGN+SH_TUYA_DATA_SIZE)   /* 9043 */

//机型名称
#define ADDR_SH_MAC_VER_WR_BGN      (9050)
#define SH_MAC_VER_WR_SIZE          (16)
#define ADDR_SH_MAC_VER_WR_END      (ADDR_SH_MAC_VER_WR_BGN + SH_MAC_VER_WR_SIZE)

/***********************************************************************************
                                只读区 通讯地址
***********************************************************************************/

#define SH_ADDR_STRING_START_CODE_BGN   (1700)
#define STRING_START_CODE_SIZE          (128 / sizeof(U16))
#define SH_ADDR_STRING_START_CODE_END   (SH_ADDR_STRING_START_CODE_BGN + STRING_START_CODE_SIZE * 2)/* 中英文 */

//使用禁用信息
#define SH_ADDR_ENABLE_INFO_BGN     (1960)
#define ENABLE_INFO_SIZE            ((sizeof(SH_ENABLE_INFO)+1) / sizeof(U16))
#define SH_ADDR_ENABLE_INFO_END     (SH_ADDR_ENABLE_INFO_BGN + ENABLE_INFO_SIZE)

//除霜信息
#define SH_ADDR_DEFR_INFO_BGN       1968
#define DEFR_INFO_SIZE              ((sizeof(SH_DEFRO_INFO)+1) / sizeof(U16))   // 2
#define SH_ADDR_DEFR_INFO_END       (SH_ADDR_DEFR_INFO_BGN + DEFR_INFO_SIZE*SH_MAX_PCB_NUM)     // 2000

//显示辅助信息
#define ADDR_SH_DISP_BGN            2000
#define SH_DISP_SIZE                (1)
#define ADDR_SH_DISP_END            (ADDR_SH_DISP_BGN + SH_DISP_SIZE*SH_MAX_DISP_INFO)                // 2100

//查询信息个数
#define ADDR_SH_CHK_NUM_BGN         2200
#define SH_CHK_NUM_SIZE             ((sizeof(SH_CHK_INFO_NUM)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_NUM_END         (ADDR_SH_CHK_NUM_BGN+SH_CHK_NUM_SIZE*SH_MAX_PCB_NUM)        //2264

// 调试信息只读类
#define ADDR_SH_CHK_TEST_RO_BGN     (ADDR_SH_CHK_TEST_BGN)                                      // 3300
#define SH_CHK_TEST_RO_SIZE         ((sizeof(((SH_CHK_TEST *)NULL)->ro)+1) / sizeof(U16))
#define ADDR_SH_CHK_TEST_RO_END     (ADDR_SH_CHK_TEST_BGN + SH_CHK_TEST_RO_SIZE)                // 3384

//版本号
#define ADDR_SH_VER_BGN             3400
#define SH_VER_SIZE                 ((sizeof(SH_CHK_VER)+1) / sizeof(U16))  // 18
#define ADDR_SH_VER_END             (ADDR_SH_VER_BGN + SH_VER_SIZE*SH_MAX_PCB_NUM)                    // 3688

//故障 当前
#define ADDR_SH_ERR_CRT_BGN         3700
#define SH_ERR_CRT_SIZE             ((sizeof(SH_ERR_CURT)+1) / sizeof(U16)) // 3
#define ADDR_SH_ERR_CRT_END         (ADDR_SH_ERR_CRT_BGN + SH_ERR_CRT_SIZE*SH_MAX_ERR_CURT_NUM)       // 4000

//故障 历史
#define ADDR_SH_ERR_HIS_BGN         4000
#define SH_ERR_HIS_SIZE             ((sizeof(SH_ERR_HIS)+1) / sizeof(U16))  // 7
#define ADDR_SH_ERR_HIS_END         (ADDR_SH_ERR_HIS_BGN + SH_ERR_HIS_SIZE*SH_MAX_ERR_HIS_NUM)        // 4700

// 风盘信息
#define ADDR_SH_FCUINFO_BGN         4700
#define SH_FCUINFO_SIZE             ((sizeof(SH_FCU)+1) / sizeof(U16))  // 97
#define ADDR_SH_FCUINFO_END         (ADDR_SH_FCUINFO_BGN + SH_FCUINFO_SIZE)        // 4797
/* 后板固件版本号 */
#define ADDR_SH_FIRMWARE_BGN        4800
#define SH_FIRMWARE_SIZE            3
#define ADDR_SH_FIRMWARE_END        (ADDR_SH_FIRMWARE_BGN+SH_FIRMWARE_SIZE)

//参数项属性
#define ADDR_SH_PARA_ATTRIB_BGN     5000
#define SH_PARA_ATTRIB_SIZE         ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))// 6
#define ADDR_SH_PARA_ATTRIB_END     (ADDR_SH_PARA_ATTRIB_BGN + SH_PARA_ATTRIB_SIZE*SH_MAX_PARA_ITEM_TOTAL)  // 12680

//压机个数
#define ADDR_SH_PCB_COMP_NUM_BGN    13000
#define SH_PCB_COMP_NUM_SIZE        (1)  // 16
#define ADDR_SH_PCB_COMP_NUM_END    (ADDR_SH_PCB_COMP_NUM_BGN + SH_PCB_COMP_NUM_SIZE*SH_MAX_PCB_NUM)   // 13016

//BMS通讯设置
#define ADDR_SH_BMS_PARA_BGN        13100
#define SH_BMS_PARA_SIZE            ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))// 6
#define ADDR_SH_BMS_PARA_END        (ADDR_SH_BMS_PARA_BGN + SH_BMS_PARA_SIZE*SH_MAX_BMS_PARA_NUM)       // 13130

/* 模式设置信息 */
#define ADDR_SH_MODE_SET_INFO_BGN   13200
#define SH_MODE_SET_INFO_SIZE       ((sizeof(SH_MODE_SET_INFO)+1) / sizeof(U16))/* 71 */
#define ADDR_SH_MODE_SET_INFO_END   (ADDR_SH_MODE_SET_INFO_BGN + SH_MODE_SET_INFO_SIZE) /* 13271 */

/* 主界面信息 */
#define ADDR_SH_MAIN_INFO_BGN       13300
#define SH_MAIN_INFO_SIZE           ((sizeof(SH_MAIN_INFO)+1) / sizeof(U16))/* 11 */
#define ADDR_SH_MAIN_INFO_END       (ADDR_SH_MAIN_INFO_BGN + SH_MAIN_INFO_SIZE) /* 13311 */

/* 变频故障信息 */
#define ADDR_SH_INVTERR_BGN         13400
#define SH_INVTERR_SIZE             ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_INVTERR_END         (ADDR_SH_INVTERR_BGN + SH_INVTERR_SIZE * SH_MAX_INVT_ERR)   /* 13600 */

/* 地暖预热信息 */
#define ADDR_SH_FLOOR_PREHEAT_BGN   13600
#define SH_FLOOR_PREHEAT_SIZE       ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))  /* 6 */
#define ADDR_SH_FLOOR_PREHEAT_END   (ADDR_SH_FLOOR_PREHEAT_BGN + SH_FLOOR_PREHEAT_SIZE * SH_MAX_FLOOR_PREHEAT_INFO) /* 13617 */

/* 定时地暖烘干信息 */
#define ADDR_SH_FLOOR_TIMER_BGN     13630
#define SH_FLOOR_TIMER_SIZE         ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))  /* 6 */
#define ADDR_SH_FLOOR_TIMER_END     (ADDR_SH_FLOOR_TIMER_BGN + SH_FLOOR_TIMER_SIZE * SH_MAX_FLOOR_TIMER_INFO)   /* 13653 */

//参数组信息
#define ADDR_SH_PARA_GRP_BGN        17000
#define SH_PARA_GRP_SIZE            ((sizeof(SH_PARA_GRP)+1) / sizeof(U16))// 3
#define ADDR_SH_PARA_GRP_END        (ADDR_SH_PARA_GRP_BGN + SH_PARA_GRP_SIZE*SH_MAX_PARA_GRP_NUM)             // 17120

//参数名称字符串
#define ADDR_SH_PARA_STRING_BGN     17200
#define SH_PARA_STRING_SIZE         (1)// 1
#define ADDR_SH_PARA_STRING_END     (ADDR_SH_PARA_STRING_BGN + SH_MAX_DICT_STR)    // 17600

//参数字符串属性
#define ADDR_SH_STR_ATTR_BGN        17600
#define SH_STR_ATTR_PER_SIZE        1
#define ADDR_SH_STR_ATTR_END        (ADDR_SH_STR_ATTR_BGN + SH_STR_ATTR_PER_SIZE*SH_MAX_PARA_STRING_GRP)   //17700

//变频信息
#define ADDR_SH_INVT_INFO_BGN       17900
#define SH_INVT_INFO_SIZE           ((sizeof(SH_INVTINFO)+1) / sizeof(U16)) // 24
#define ADDR_SH_INVT_INFO_END       (ADDR_SH_INVT_INFO_BGN + SH_INVT_INFO_SIZE) // 17924


//查询信息 系统模拟量 预留100个系统模拟量
#define ADDR_SH_CHK_SYS_AV_BGN      18000
#define SH_CHK_SYS_AV_SIZE          ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_SYS_AV_END      (ADDR_SH_CHK_SYS_AV_BGN + SH_CHK_SYS_AV_SIZE*100)                        // 18400

//查询信息 模块单元模拟量 每个模块预留50个，50*16=800个 每个模块单元模拟量地址偏移固定为50*4个字
#define ADDR_SH_CHK_MOD_AV_BGN      18400
#define SH_CHK_MOD_AV_SIZE          ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define SH_CHK_MOD_AV_STEP          (50)
#define ADDR_SH_CHK_MOD_AV_END      (ADDR_SH_CHK_MOD_AV_BGN + SH_CHK_MOD_AV_SIZE*800)   /* 21600 */

//查询信息 压机模拟量
#define ADDR_SH_CHK_COMP_AV_BGN     22000
#define SH_CHK_COMP_AV_SIZE         ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_COMP_AV_END     (ADDR_SH_CHK_COMP_AV_BGN + SH_CHK_COMP_AV_SIZE*2500)                        // 32000

//查询信息 开关量输出
#define ADDR_SH_CHK_DO_BGN          32000
#define SH_CHK_DO_SIZE              ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_DO_END          (ADDR_SH_CHK_DO_BGN + SH_CHK_DO_SIZE*2500)                          // 42000

//查询信息 开关量输入
#define ADDR_SH_CHK_DI_BGN          42000
#define SH_CHK_DI_SIZE              ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_DI_END          (ADDR_SH_CHK_DI_BGN + SH_CHK_DI_SIZE*2500)                          // 52000

/* 用户查询 */
#define ADDR_SH_CHK_USER_BGN        52000
#define SH_CHK_USER_SIZE            ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_USER_END        (ADDR_SH_CHK_USER_BGN + SH_CHK_USER_SIZE * SH_MAX_CHK_USER_INFO)//52020

/* 变频器版本号 */
#define ADDR_SH_VFD_VER_BGN         52100
#define SH_VFD_VER_SIZE             ((sizeof(SH_CHK_VER)+1) / sizeof(U16))// 18
#define ADDR_SH_VFD_VER_END         (ADDR_SH_VFD_VER_BGN + SH_VFD_VER_SIZE*SH_MAX_PCB_NUM)    // 52388

/* 外机板版本号 */
#define ADDR_SH_EXIO_VER_BGN        52400
#define SH_EXIO_VER_SIZE            ((sizeof(SH_CHK_VER)+1) / sizeof(U16))// 18
#define ADDR_SH_EXIO_VER_END        (ADDR_SH_EXIO_VER_BGN + SH_EXIO_VER_SIZE*SH_MAX_EXIO_NUM*SH_MAX_PCB_NUM)    // 53552

/* 4G信息 */
#define ADDR_SH_4G_INFO_BGN         54000
#define SH_4G_INFO_SIZE             ((sizeof(SH_4G_INFO)+1) / sizeof(U16)) // 38
#define ADDR_SH_4G_INFO_END         (ADDR_SH_4G_INFO_BGN + SH_4G_INFO_SIZE) // 54038

/*机器型号*/
#define ADDR_SH_MAC_VER_BGN         54100
#define SH_MAC_VER_SIZE             ((sizeof(SH_CHK_VER)+1) / sizeof(U16)) // 18
#define ADDR_SH_MAC_VER_END         (ADDR_SH_MAC_VER_BGN + SH_MAC_VER_SIZE*SH_MAX_PCB_NUM) // 54388

//调试信息
#define ADDR_SH_CHK_DEBUG_BGN       60000
#define ADDR_SH_CHK_DEBUG_END       65000 // 50000
#define ADDR_SH_SET_DEBUG_BGN       65000
#define ADDR_SH_SET_DEBUG_END       65500 // 50000
#endif



