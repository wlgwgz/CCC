#ifndef ERR_SYS_H
#define ERR_SYS_H    1
#ifdef ERR_SYS_GLOBAL
#define ERR_SYS_EXT
#else 
#define ERR_SYS_EXT extern
#endif

#define ERR_DEALY_TIME				3			/* 一般故障延时 */
#define ERR_OL_TIME				    20			/* 过载故障延时(单位: 100MS) */

#define ERR_UI35_COMMU_ERR_TIME     (180)   /* 彩屏通讯故障延时: 180s */

// 主板故障操作参数
#define m_set_error(x, y)			SET_BIT(cell[x].cell_error[(y) / ERR_BIT_NUM], (y) % ERR_BIT_NUM)	/* 置故障位 */
#define m_clr_error(x, y)			CLR_BIT(cell[x].cell_error[(y) / ERR_BIT_NUM], (y) % ERR_BIT_NUM)	/* 清故障位 */
#define m_get_error(x, y)			GET_BIT(cell[x].cell_error[(y) / ERR_BIT_NUM], (y) % ERR_BIT_NUM)	/* 读故障位 */

/* 压机故障(制冷、制热)状态操作(读取、置位、复位) */
#define get_comp_err(x, y)	        GET_BIT(fg_comp_err[x], y)
#define set_comp_err(x, y)	        SET_BIT(fg_comp_err[x], y)
#define clr_comp_err(x, y)	        CLR_BIT(fg_comp_err[x], y)

#define ANYONE_EXIST	0	// 任意一个模块存在
#define ALL_EXIST		1	// 所有模块都存在


ERR_SYS_EXT BOL fg_pump_err;         	    /* 水泵故障 */
ERR_SYS_EXT BOL fg_ac_sys_err;          	/* 空调系统故障 */
ERR_SYS_EXT BOL fg_hotw_sys_err;            /* 热水系统故障 */
ERR_SYS_EXT U16 fg_comp_err[MAX_COMP];      /* 压机故障(制冷、制热) */

ERR_SYS_EXT U08 fg_commu_err;			//级联通讯故障位

ERR_SYS_EXT BOL UI35_commu_err_no_stop_flag;    //线控器通讯故障不停机标志

ERR_SYS_EXT void err_sys_init(void);

ERR_SYS_EXT void err_mod_temp_too_low_spec(I16 p_value, I16 exit_value, I16 err_value, U08 *err, U16 *err_time);

ERR_SYS_EXT void menu_error(void);
ERR_SYS_EXT void err_sys_time(void);
ERR_SYS_EXT void err_sys(U08 pcb_num);
ERR_SYS_EXT BOOL all_unit_error_judge(U08 err_num, U08 attrib);
ERR_SYS_EXT BOL err_sys_get_s_commu_err(U08 pcb);
ERR_SYS_EXT I16 get_UI35_ctrl_type(void);
#endif
