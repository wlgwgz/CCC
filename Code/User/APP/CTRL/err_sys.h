#ifndef ERR_SYS_H
#define ERR_SYS_H    1
#ifdef ERR_SYS_GLOBAL
#define ERR_SYS_EXT
#else 
#define ERR_SYS_EXT extern
#endif

#define ERR_DEALY_TIME				3			/* һ�������ʱ */
#define ERR_OL_TIME				    20			/* ���ع�����ʱ(��λ: 100MS) */

#define ERR_UI35_COMMU_ERR_TIME     (180)   /* ����ͨѶ������ʱ: 180s */

// ������ϲ�������
#define m_set_error(x, y)			SET_BIT(cell[x].cell_error[(y) / ERR_BIT_NUM], (y) % ERR_BIT_NUM)	/* �ù���λ */
#define m_clr_error(x, y)			CLR_BIT(cell[x].cell_error[(y) / ERR_BIT_NUM], (y) % ERR_BIT_NUM)	/* �����λ */
#define m_get_error(x, y)			GET_BIT(cell[x].cell_error[(y) / ERR_BIT_NUM], (y) % ERR_BIT_NUM)	/* ������λ */

/* ѹ������(���䡢����)״̬����(��ȡ����λ����λ) */
#define get_comp_err(x, y)	        GET_BIT(fg_comp_err[x], y)
#define set_comp_err(x, y)	        SET_BIT(fg_comp_err[x], y)
#define clr_comp_err(x, y)	        CLR_BIT(fg_comp_err[x], y)

#define ANYONE_EXIST	0	// ����һ��ģ�����
#define ALL_EXIST		1	// ����ģ�鶼����


ERR_SYS_EXT BOL fg_pump_err;         	    /* ˮ�ù��� */
ERR_SYS_EXT BOL fg_ac_sys_err;          	/* �յ�ϵͳ���� */
ERR_SYS_EXT BOL fg_hotw_sys_err;            /* ��ˮϵͳ���� */
ERR_SYS_EXT U16 fg_comp_err[MAX_COMP];      /* ѹ������(���䡢����) */

ERR_SYS_EXT U08 fg_commu_err;			//����ͨѶ����λ

ERR_SYS_EXT BOL UI35_commu_err_no_stop_flag;    //�߿���ͨѶ���ϲ�ͣ����־

ERR_SYS_EXT void err_sys_init(void);

ERR_SYS_EXT void err_mod_temp_too_low_spec(I16 p_value, I16 exit_value, I16 err_value, U08 *err, U16 *err_time);

ERR_SYS_EXT void menu_error(void);
ERR_SYS_EXT void err_sys_time(void);
ERR_SYS_EXT void err_sys(U08 pcb_num);
ERR_SYS_EXT BOOL all_unit_error_judge(U08 err_num, U08 attrib);
ERR_SYS_EXT BOL err_sys_get_s_commu_err(U08 pcb);
ERR_SYS_EXT I16 get_UI35_ctrl_type(void);
#endif
