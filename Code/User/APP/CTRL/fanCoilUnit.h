/******************************************************************************

                  ��Ȩ���� (C), 2001-2017, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : fanCoilUnit.h
  �� �� ��   : ����
  ��    ��   : kjh
  ��������   : 2021��4��7��
  ����޸�   :
  ��������   : fanCoilUnit.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2021��4��7��
    ��    ��   : kjh
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __FAN_COIL_UNIT_H__
#define __FAN_COIL_UNIT_H__


#ifdef GBL_FCU
#define FCU_EXT
#else
#define FCU_EXT extern
#endif

/*-----------------�ӿ�------------------------*/
/* ʹ�÷���̹ܸ���:���ΪMAX_FCU */
#define FCU_NUM_USE         val_fcu_num

#define FCU_COM             COM_FCU_FBUS
#define FCU_INIT_TIME       POWER_ON_INTO_WORK

/* �������� */
#define FCU_CMD_RUN             (key_run = TRUE)

/* �����趨ģʽ */
#define FCU_MASTER_MODE         sv_mode
#define FCU_SET_MODE_COOL       para_set_cent(P_GaI(sv_mode), SET_COOL)
#define FCU_SET_MODE_HEAT       para_set_cent(P_GaI(sv_mode), SET_HEAT)


#define fcu_set_error(err_num)  (m_set_error(MAX_PCB, err_num))
#define fcu_clr_error(err_num)  (m_clr_error(MAX_PCB, err_num))
#define fcu_get_error(err_num)  (m_get_error(MAX_PCB, err_num))

// ����̹�����
#define FCU_PUNP                0         // ����
#define fcu_type                FCU_PUNP

/* ����̹��ź� */
FCU_EXT U16 fcu_semaphore;
#define FCU_SEM_NULL        0
#define FCU_SEM_M_START_S   1
#define FCU_SEM_M_CLOSE_S   2
//#define FCU_SEM_M_SET_MODE  3

/*-----------------�ڲ�------------------------*/

#define FCU_NUM_MAX             32		/* ���ĺ�ע������ر�־λ�Ƿ� */
#define FCU_ARRAY_SIZE_MAX		_DIVUCARRY(FCU_NUM_MAX, 16) /* ���������� */
#define FCU_ADDR_MAX			(247)	 	            /* FCUͨѶ��ַ���ֵ */
#define FCU_ADDR_BASE			(val_fcu_base_addr)	 	/* FCUͨѶ��ַ��ַ */

FCU_EXT I16 fcu_pv[FCU_NUM_MAX][10];
#define fcu_temp_indoor(ec)     (fcu_pv[ec][0])				            /* ���������¶� */
#define fcu_egy_need(ec)        (fcu_pv[ec][1])				            /* ������������ */
#define fcu_err_code(ec)	    (fcu_pv[ec][2])				            /* ���̹��ϴ��� */
#define fcu_sta_onoff(ec)	    (fcu_ctrl_rd(ec, FCU_ADDR_WR_ONOFF))	/* ���̿���״̬ */
#define fcu_sta_mode(ec)	    (fcu_ctrl_rd(ec, FCU_ADDR_WR_SET_MODE))	/* �����趨ģʽ */


//4 ͨ������(Ҫ��֤���ȹ���ͨ������)
FCU_EXT I16 fcu_rd_array[FCU_NUM_MAX][32];/* N0004 */
FCU_EXT I16 fcu_rw_array_wr[FCU_NUM_MAX][15];
FCU_EXT I16 fcu_rw_array_rd[FCU_NUM_MAX][15];
FCU_EXT I16 fcu_rw_array_w_fg[FCU_NUM_MAX][15];

#define fcu_rd_sta(ec, addr)        fcu_rd_array[ec][addr-FCU_ADDR_RD_BASE]
#define fcu_ctrl_wr(ec, addr)       fcu_rw_array_wr[ec][addr-FCU_ADDR_WR_BASE]
#define fcu_ctrl_rd(ec, addr)       fcu_rw_array_rd[ec][addr-FCU_ADDR_WR_BASE]
#define fcu_ctrl_fg_wr(ec, addr)    fcu_rw_array_w_fg[ec][addr-FCU_ADDR_WR_BASE]

/* �������䲻ͬ����Э�� */
//4 ���շ���̹�
#define FCU_ADDR_RD_BASE        0			/* ֻ��������ʼ��ַ */
#define FCU_ADDR_RD_NUM         18			/* ֻ����������:������ݵ�ַ-��ʼ���ݵ�ַ+1 */
#define FCU_ADDR_RD_TEMP        0			/* �����¶� */
#define FCU_ADDR_RD_EGY         16			/* �������� */
#define FCU_ADDR_RD_ERROR       17			/* ������ */

#define FCU_ADDR_WR_BASE        2			/* ��д������ʼ��ַ */
#define FCU_ADDR_WR_NUM         2			/* ��д��������:������ݵ�ַ-��ʼ���ݵ�ַ+1 */
#define FCU_ADDR_WR_ONOFF       2			/* ���ػ� */
#define FCU_ADDR_WR_SET_MODE    3			/* �趨ģʽ */
//#define FCU_ADDR_WR_RST       1019

#define FCU_MODE_COOL       	1			/* ���� */
#define FCU_MODE_HEAT       	2			/* ���� */
#define FCU_MODE_VENT       	3			/* ͨ�� */
#define FCU_MODE_FLOOR      	4			/* ��ů */

typedef struct tagFCU
{
    I16 temp_indoor;
    U16 err_word[2];
    U16 warn_word[2];
} S_FCU;
FCU_EXT S_FCU fcu[FCU_NUM_MAX];

/* ��ĩģʽ���� */
#define FCU_MODE_INDEPEND   0	/* �໥���� */
#define FCU_MODE_M_MAIN     1	/* ����Ϊ�� */
#define FCU_MODE_S_MAIN     2	/* ĩ��Ϊ�� */
#define FCU_MODE_LINK       3	/* �໥���� */
#define FCU_MODE_MAX        4
#define MODE_IS_INDEPEND    (val_fcu_mode_set == FCU_MODE_INDEPEND)
#define MODE_IS_M_MAIN      (val_fcu_mode_set == FCU_MODE_M_MAIN)
#define MODE_IS_S_MAIN      (val_fcu_mode_set == FCU_MODE_S_MAIN)
#define MODE_IS_LINK        (val_fcu_mode_set == FCU_MODE_LINK)

/* �ܵ��������� */
#define CPST_PV         0			/* ������ǰ�¶� */
#define CPST_SV         1			/* ����Ŀ���¶� */
#define CPST_SET_MAX    2

/* ĩ�˿������� */
#define S_START_M_NONE          0			/* ������ */
#define S_START_M_ONLY          1			/* �������� */
#define S_START_M_ALL           2			/* ��ĩȫ�� */
#define S_START_M_MAX           3

/* fcuλ���� */
#define GET_FCU_BIT(name, ec)	GETBIT(name[ec/16], ec%16)
#define SET_FCU_BIT(name, ec)	SETBIT(name[ec/16], ec%16)
#define CLR_FCU_BIT(name, ec)	CLRBIT(name[ec/16], ec%16)
/*-----------------�ⲿ------------------------*/

FCU_EXT U16 fcu_commu_err[FCU_ARRAY_SIZE_MAX];

FCU_EXT void fcu_deal(void);
FCU_EXT void fcu_init(void);
FCU_EXT void fcu_rd_update(void);
FCU_EXT BOL  fcu_commu(void);
FCU_EXT void fcu_err_reset(void);
FCU_EXT void fcu_time(void);
FCU_EXT BOL  fcu_has_egy_need(void);
FCU_EXT I16  fcu_energy_cpst(void);
FCU_EXT U08 fcu_state_get(U08 i);

#endif /* __FAN_COIL_UNIT_H__ */

