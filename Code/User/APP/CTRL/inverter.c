/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: inverter.c
 �ļ�����: ��Ƶ��ͨѶЭ�鴦��
 ������  : hzy
 ��������: 2016.12.14
*******************************************************************************/

#define INVERTER_GLOBAL
#include "includes.h"



// ��Ƶ���豸��ַ
//#define ADD_INVERTER_COMMU        1 
#define ADD_INVERTER_COMMU        val_invt_addr 
#define ADD_INVT_FAN_COMMU        110 /* N0004 */


// ��Ƶ�����ݵ�ַ
#define INVT_ADDR_RW_PARA_F00       0x0000  //��������

#define INVT_ADDR_RW_CMD            0x3200  //ͨѶָ��
//#define INVT_ADDR_RW_CMD_SIZE       10      //ͨѶָ���С
//#define INVT_ADDR_RW_CMD_CONTROL        (INVT_ADDR_RW_CMD+0) //������ 
//#define INVT_ADDR_RW_CMD_SET_FREQ       (INVT_ADDR_RW_CMD+1) //����Ƶ��
//#define INVT_ADDR_RW_CMD_EEP_SAVE       (INVT_ADDR_RW_CMD+9) //RAM�в�������EEPROM

#define INVT_ADDR_R_STATES          0x3210 //��Ƶ��״̬


//��Ƶ��ͨѶָ��
#define CMD_CONTROL         0   // ������
#define CMD_SET_FREQ        1   // ����Ƶ��
#define CMD_SET_PFC         4   // ��ԴPFC
#define CMD_SET_FREQ_EC1    7   // EC1����ת��
#define CMD_SET_FREQ_EC2    8   // EC2����ת��
//#define CMD_EEP_SAVE        9   // RAM�в�������EEPROM
#define CMD_INVT_MAX       10   // ͨѶָ���С
static I16 invt_cmd[INVT_ID_MAX][CMD_INVT_MAX];
//static I16 invt_cmd_bak[INVT_ID_MAX][CMD_INVT_MAX];

//����ֵ������
static U08 fg_cmd_changed[INVT_ID_MAX][CMD_INVT_MAX];



//״̬��Ϣ��ֻ����
typedef struct tagINVT_INFO
{
    U16 state;          // ��״̬��
    U16 freq_run;       // ����ת��(0.01rps)
    I16 temperature;    // ģ���¶�(0.1��)
    U16 power_out;      // �������(0.01kW)
    U16 freq_set;       // ����ת��(0.01rps)
    U16 current_out;    // �������(0.1A)
    U16 torque_out;     // ���ת��(0.1%)
    U16 volt_out;       // �����ѹ(0.1V)
    U16 volt_generatrix;// ĸ�ߵ�ѹ(0.1V)
    U16 errid;          // ���ϴ���
    U16 errcode1;       // ������1����Ƶ�������
    U16 errcode2;       // ������2��PFC�¶�
    U16 freq_now;       // ��ǰת��(0.01rps)
    U16 freq_limit;     // ����Ƶ����Ϣ��λ-������Ƶ��ʮλ-������Ƶ����λ-Ƿѹ��Ƶ
    U16 bak1;           // ����
    U16 bak2;           // ����
    U16 bak3;           // ����
    /* ����ΪЭ�����������ݣ�ע�⴦���¾�Э����������� */
    U16 pulse_ec1;       // ec1������
    U16 pulse_ec2;       // ec2������
}INVT_INFO;
GSTATIC  INVT_INFO invt_info[INVT_ID_MAX];               // ��Ƶ����ǰ��Ϣ
#define INVT_INFO_SIZE  (sizeof(INVT_INFO)/sizeof(U16)) // ��Ƶ��״̬��Ϣ��С
#define INVT_INFO_SIZE_OLD  (17) // ��Ƶ��״̬��Ϣ��С-��Э��

//״̬�ָ�λ����
#define INVT_STATE_RUN      2   //������
#define INVT_STATE_ERR      3   //����



//��Ƶ������
enum
{
    INVTCMD_ON,             //����
    INVTCMD_OFF,            //ֹͣ
    INVTCMD_OFF_FREE,       //����ͣ��������ͣ��
    INVTCMD_OFF_QUICK,      //��ͣ(���Ƽ�ʹ��)
    INVTCMD_FREQ_LOCK,      //Ƶ������ 
    INVTCMD_FREQ_UNLOCK,    //Ƶ�ʽ���
    INVTCMD_ACCLRT_LOCK,    //��������  
    INVTCMD_ACCLRT_UNLOCK,  //���ٽ���  
    INVTCMD_ERR_RESET,      //���ϸ�λ
    INVTCMD_PARA_INIT,      //������ʼ��
    INVTCMD_PARA_READ,      //������ȡ
    INVTCMD_PFC_OFF,        //������ԴPFC
    INVTCMD_PFC_ON,         //ʹ����ԴPFC
//    INVTCMD_PARA_SAVE       //��������
};
static U16 invt_cmd_ctrl[INVT_ID_MAX]; //��Ƶ��������



//��Ƶ��ת������
static I16 invt_freq_set[INVT_ID_MAX];
static I16 invt_freq_set_ec[INVT_ID_MAX][FREQSPEEDNUM];



//ģ���ʱ
//100ms��ʱ
enum
{
    INVT_TIMER_100MS_CMD_UPDATE,     // ��������
    INVT_TIMER_100MS_PFC_CMD_UPDATE,     // ����PFC����
    INVT_TIMER_100MS_MAX
};
static U16 invt_timer_100ms[INVT_ID_MAX][INVT_TIMER_100MS_MAX];
//1s��ʱ
enum
{
    INVT_TIMER_1S_WR_FREQ,       //�趨ת��ȷ�ϼ��
    INVT_TIMER_1S_WR_FREQ_FAN,   //�趨����ȷ�ϼ��
    INVT_TIMER_1S_RESET_CHG,     //��Ƶ��λ������
    INVT_TIMER_1S_MAX
};
static U16 invt_timer_1s[INVT_ID_MAX][INVT_TIMER_1S_MAX];

/*******************************************************************************
 �� ��: invt_cmd_on()
 �� ��: ���Ϳ�������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_on(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ON);
}

/*******************************************************************************
 �� ��: invt_cmd_off()
 �� ��: ����ֹͣ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_off(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);
}

/*******************************************************************************
 �� ��: invt_cmd_off_free()
 �� ��: ��������ͣ������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_off_free(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_FREE);
}


/*******************************************************************************
 �� ��: invt_cmd_off_quick()
 �� ��: ���ͼ�ͣ����
 �� ��: ��
 �� ��: ��
 �� ��: "��ͣ"Ϊ�ϵ�ͣ�������Ƽ�ʹ��
*******************************************************************************/
void invt_cmd_off_quick(INVT_ID_ENUM id)        
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK);
}

/*******************************************************************************
 �� ��: invt_cmd_freq_lock()
 �� ��: ����Ƶ����������
 �� ��: ��
 �� ��: ��
 �� ��: ���Ƽ�ʹ��
*******************************************************************************/
void invt_cmd_freq_lock(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_LOCK);
}

/*******************************************************************************
 �� ��: invt_cmd_freq_unlock()
 �� ��: ����Ƶ�ʽ�������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_freq_unlock(INVT_ID_ENUM id)      
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_UNLOCK);
}

/*******************************************************************************
 �� ��: invt_cmd_acclrt_lock()
 �� ��: ���ͱ�����������
 �� ��: ��
 �� ��: ��
 �� ��: ���Ƽ�ʹ��
*******************************************************************************/
void invt_cmd_acclrt_lock(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_LOCK);
}

/*******************************************************************************
 �� ��: invt_cmd_acclrt_unlock()
 �� ��: ���ͱ��ٽ�������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_acclrt_unlock(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_UNLOCK);
}

/*******************************************************************************
 �� ��: invt_cmd_err_reset()
 �� ��: ���͹��ϸ�λ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_err_reset(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ERR_RESET);
}

/*******************************************************************************
 �� ��: invt_cmd_para_init()
 �� ��: ���Ͳ�����ʼ������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_para_init(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_INIT);

    invt_para[id][F00_PARA_INIT] = 11; //������ʼ��ָ�11:ȫ��ʼ����22:����ʼ��ͨѶ������
    fg_para_changed[id][F00_PARA_INIT] = TRUE;
}

/*******************************************************************************
 �� ��: invt_cmd_para_read()
 �� ��: ���Ͳ�����ȡ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_para_read(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_READ);
}

/*******************************************************************************
 �� ��: invt_cmd_pfc_off()
 �� ��: ���ͽ�����ԴPFC����
 �� ��: id--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_pfc_off(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_OFF);
}

/*******************************************************************************
 �� ��: invt_cmd_pfc_on()
 �� ��: ����ʹ����ԴPFC����
 �� ��: id--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_pfc_on(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_ON);
}

#if 0
/*******************************************************************************
 �� ��: invt_cmd_para_save()
 �� ��: ���Ͳ�����������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_cmd_para_save(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_SAVE);
}
#endif


/*******************************************************************************
 �� ��: invt_get_device_addr()
 �� ��: ��ȡ��Ƶ���豸��ַ
 �� ��: �� 
 �� ��: ��Ƶ���豸��ַ
 �� ��: ��
*******************************************************************************/
U08 invt_get_device_addr(INVT_ID_ENUM id)/* N0004 */
{
    U08 addr;
    if (id >= INVT_ID_MAX)
    {
        addr = ADD_INVERTER_COMMU;
    }
    else if (id >= INVT_FAN1)
    {
        addr = id-INVT_FAN1+ADD_INVT_FAN_COMMU;
    }
    else
    {
        addr = id-INVT_ID_1+ADD_INVERTER_COMMU;
    }
    
    return addr;
}

/*******************************************************************************
 �� ��: invt_cmd_get_addr()
 �� ��: ���ʱ�Ƶ��ͨѶָ��ĵ�ַ
 �� ��: �� 
 �� ��: ��Ƶ��ͨѶָ��ĵ�ַ
 �� ��: ��
*******************************************************************************/
U16 invt_cmd_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_CMD;
    return addr;
}

/*******************************************************************************
 �� ��: invt_cmd_get_size()
 �� ��: ���ʱ�Ƶ��ͨѶָ��Ĵ�С���֣�
 �� ��: �� 
 �� ��: ��Ƶ��ͨѶָ��Ĵ�С
 �� ��: ��
*******************************************************************************/
U08 invt_cmd_get_size(void)
{
    U08 size;
    size = CMD_INVT_MAX;
    return size;
}

/*******************************************************************************
 �� ��: invt_paras_get_buf()
 �� ��: ��ȡ��Ƶ�����������Ĵ洢λ��
 �� ��: �� 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
U16* invt_cmd_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_cmd[id][0];
    return buf;
}

/*******************************************************************************
 �� ��: invt_paras_get_addr()
 �� ��: ���ʱ�Ƶ�����������ĵ�ַ
 �� ��: �� 
 �� ��: ��Ƶ�����������ĵ�ַ
 �� ��: ��
*******************************************************************************/
U16 invt_paras_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F00;
    return addr;
}

/*******************************************************************************
 �� ��: invt_paras_get_size()
 �� ��: ���ʱ�Ƶ�����������Ĵ�С���֣�
 �� ��: �� 
 �� ��: ��Ƶ�����������Ĵ�С
 �� ��: ��
*******************************************************************************/
U08 invt_paras_get_size()
{
    U08 size;
    size = F00_MAX;
    return size;
}

/*******************************************************************************
 �� ��: invt_paras_fa_get_addr()
 �� ��: ���ʱ�Ƶ��Fa�����ĵ�ַ
 �� ��: �� 
 �� ��: ��Ƶ��Fa�����ĵ�ַ
 �� ��: ��
*******************************************************************************/
U16 invt_paras_fa_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_FA;
    return addr;
}
U16 invt_paras_fb_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_FB;
    return addr;
}
U16 invt_paras_fp_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_FP;
    return addr;
}
U16 invt_paras_f10_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F10;
    return addr;
}
U16 invt_paras_f9_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F9;
    return addr;
}
U16 invt_paras_f6_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F6;
    return addr;
}

/*******************************************************************************
 �� ��: invt_paras_fa_get_size()
 �� ��: ���ʱ�Ƶ�����������Ĵ�С���֣�
 �� ��: �� 
 �� ��: ��Ƶ�����������Ĵ�С
 �� ��: ��
*******************************************************************************/
U08 invt_paras_fa_get_size()
{
    U08 size;
    size = FA_MAX;
    return size;
}
U08 invt_paras_fb_get_size()
{
    U08 size;
    size = FB_MAX;
    return size;
}
U08 invt_paras_fp_get_size(void)
{
    U08 size;
    size = FP_MAX;
    return size;
}
U08 invt_paras_f10_get_size(void)
{
    U08 size;
    size = F10_MAX;
    return size;
}
U08 invt_paras_f9_get_size(void)
{
    U08 size;
    size = F9_MAX;
    return size;
}
U08 invt_paras_f6_get_size(void)
{
    U08 size;
    size = F6_MAX;
    return size;
}
/*******************************************************************************
 �� ��: invt_paras_fa_get_buf()
 �� ��: ��ȡ��Ƶ�����������Ĵ洢λ��
 �� ��: �� 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
U16* invt_paras_fa_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_fa[id][0];
    return buf;
}
U16* invt_paras_fb_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_fb[id][0];
    return buf;
}
U16* invt_paras_fp_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_fp[id][0];
    return buf;
}
U16* invt_paras_f10_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_f10[id][0];
    return buf;
}
U16* invt_paras_f9_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_f9[id][0];
    return buf;
}
U16* invt_paras_f6_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_f6[id][0];
    return buf;
}

/*******************************************************************************
 �� ��: invt_para_fa_isChanged()
 �� ��: �жϱ�Ƶ�������Ƿ����޸�
 �� ��: INVT_ID_ENUM id--��Ƶ�����; 
 �� ��: FALSE:���޸ģ�����:�޸ĵĲ������+1
 �� ��: ��
*******************************************************************************/
U16 invt_para_fa_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
//    U08 i;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //�����²���
    
//    invt_deal_para(id);  // ��������
    
    //����Ƿ��в�������
    if (fg_para_fa_changed[id][FA_FREQ_START])
	{
	    para_id = FA_FREQ_START;
	    fg_send_para = TRUE;
	}
    else if (fg_para_fa_changed[id][FA_MAX_CUR_LMT])
	{
	    para_id = FA_MAX_CUR_LMT;
	    fg_send_para = TRUE;
	}
    else if (fg_para_fa_changed[id][FA_FREQ_START2])
	{
	    para_id = FA_FREQ_START2;
	    fg_send_para = TRUE;
	} 

    //���Ĳ���
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //������󱣴�
    }
    
    return ChangeIndex;
}
U16 invt_para_fb_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //�����²���
    
    //����Ƿ��в�������
    if (fg_para_fb_changed[id][FB_MAX_LOAD_TIME])
	{
	    para_id = FB_MAX_LOAD_TIME;
	    fg_send_para = TRUE;
	}
    else if (fg_para_fb_changed[id][FB_BLOCKAGE_TIME])
	{
	    para_id = FB_BLOCKAGE_TIME;
	    fg_send_para = TRUE;
	}
    
    //���Ĳ���
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //������󱣴�
    }
    
    return ChangeIndex;
}
U16 invt_para_f10_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //�����²���
    
    //����Ƿ��в�������
    if (fg_para_f10_changed[id][F10_POWER])
	{
	    para_id = F10_POWER;
	    fg_send_para = TRUE;
	}
    
    //���Ĳ���
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //������󱣴�
    }
    
    return ChangeIndex;
}
U16 invt_para_f9_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //�����²���
    
    //����Ƿ��в�������
    if (fg_para_f9_changed[id][F9_EC_FAN_NUM])
	{
	    para_id = F9_EC_FAN_NUM;
	    fg_send_para = TRUE;
	}
    
    //���Ĳ���
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //������󱣴�
    }
    
    return ChangeIndex;
}
U16 invt_para_f6_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //�����²���
    
    //����Ƿ��в�������
    if (fg_para_f6_changed[id][F6_00_EC_PWM_SW_FREQ])
	{
	    para_id = F6_00_EC_PWM_SW_FREQ;
	    fg_send_para = TRUE;
	}
    
    //���Ĳ���
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //������󱣴�
    }
    
    return ChangeIndex;
}

/*******************************************************************************
 �� ��: invt_paras_fa_backup()
 �� ��: ��Ƶ���ڲ������޸ĺ�ȡ���޸ı�־
 �� ��: INVT_ID_ENUM id--��Ƶ�����; U08 index--�������
 �� ��: 0xFF:���޸ģ�����:�޸ĵ��������
 �� ��: ��
*******************************************************************************/
void invt_paras_fa_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<FA_MAX)
    {
        fg_para_fa_changed[id][index]=FALSE;

    }
}
void invt_paras_fb_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<FB_MAX)
    {
        fg_para_fb_changed[id][index]=FALSE;
    }
}
void invt_paras_f10_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F10_MAX)
    {
        fg_para_f10_changed[id][index]=FALSE;
    }
}
void invt_paras_f9_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F9_MAX)
    {
        fg_para_f9_changed[id][index]=FALSE;
    }
}
void invt_paras_f6_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F6_MAX)
    {
        fg_para_f6_changed[id][index]=FALSE;
    }
}
/*******************************************************************************
 �� ��: invt_paras_get_buf()
 �� ��: ��ȡ��Ƶ�����������Ĵ洢λ��
 �� ��: �� 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
U16* invt_paras_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para[id][0];
    return buf;
}

/*******************************************************************************
 �� ��: invt_states_get_addr()
 �� ��: ��ȡ��Ƶ��״̬�ĵ�ַ
 �� ��: �� 
 �� ��: ��Ƶ��״̬�ĵ�ַ
 �� ��: ��
*******************************************************************************/
U16 invt_states_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_R_STATES;
    return addr;
}

/*******************************************************************************
 �� ��: invt_states_get_size()
 �� ��: ��ȡ��Ƶ��״̬�Ĵ�С���֣�
 �� ��: �� 
 �� ��: ��Ƶ��״̬�Ĵ�С
 �� ��: ��
*******************************************************************************/
U08 invt_states_get_size()
{
    U08 size;
    
    if (USE_PWM_FAN_EX
    ||  invt_para_read(INVT_ID_1, INVT_PARA_EC_NUM)>0
    ||  TestProgStaus != TPS_NO_TEST_CMD
        )
    {
        size = INVT_INFO_SIZE;
    }
    else
    {
        size = INVT_INFO_SIZE_OLD;
    }
    
    return size;
}

/*******************************************************************************
 �� ��: invt_states_get_buf()
 �� ��: ��ȡ��Ƶ��״̬�Ĵ洢λ��
 �� ��: �� 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
U16* invt_states_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_info[id];
    return buf;
}



/*******************************************************************************
 �� ��: invt_cmd_deal()
 �� ��: ��ӦӦ�ó�����������
 �� ��: INVT_ID_ENUM id ��Ƶ�����
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void invt_deal_cmd(INVT_ID_ENUM id)
{
    //ͨѶЭ���ж���Ŀ����ָ�λ
    #define INVT_CNTRL_ONOFF1        0   //ON/OFF1�����������У�Ϊ 0 ��ͣ����
    #define INVT_CNTR_OFF2           1   //OFF2��Ϊ 0 ��ϵ�ͣ�����˺���δ���ø�����ʽ
    #define INVT_CNTR_OFF3           2   //OFF3��Ϊ 0 �����ͣ����
    #define INVT_CNTR_LOCK_FREQ      3   //����������Ϊ 0 ������������
    #define INVT_CNTR_LOCK_ACCLRT    4   //б��ʹ�ܣ�Ϊ 0 ��ֹͣ�Ӽ��٣�
    #define INVT_CNTR_ERR_RESET      7   //���ϸ�λ�������ؽ��й��ϸ�λ��
    #define INVT_CNTR_CLOCKWISE      8   //����㶯

    #define CMD_UPDATE_INTRVL        5  // ����������

    static I16 invt_control_bak[INVT_ID_MAX] = {0};    //��Ƶ�������ֱ���
    
    if(invt_timer_100ms[id][INVT_TIMER_100MS_CMD_UPDATE]<CMD_UPDATE_INTRVL)   return;     // ʱ��δ���������� //NOTE-CXW �Ƿ���Ҫʱ������
    
    //����
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ON))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ON);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTRL_ONOFF1);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);
    }
    
    
    
    
    // ͣ��
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_FREE))//����ͣ��
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_FREE);
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK);
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);
        
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTRL_ONOFF1);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);						//�����޸�	
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_ACCLRT);						//�����޸�	

    }
    else if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK))//��ͣ
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK);
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);

        
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
    }
    else if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF))    //����ֹͣ
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTRL_ONOFF1);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
    }

    
    //Ƶ������
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_LOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_LOCK);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);
    }
    //Ƶ�ʽ���
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_UNLOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_UNLOCK);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);
    }
    //��������
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_LOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_LOCK);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_ACCLRT);
    }
    //���ٽ���
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_UNLOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_UNLOCK);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_ACCLRT);
    } 
    //���ϸ�λ
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ERR_RESET))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ERR_RESET);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_ERR_RESET);
        invt_timer_1s[id][INVT_TIMER_1S_RESET_CHG] = 0;
    } 
    else //���ϸ�λ��������Ч����������ʱ����
    {
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_ERR_RESET);
    }

    // ���±�Ƶ�����ֵ�BMSЭ���ַ
    {
        I16 temp = invt_cmd[id][CMD_CONTROL];
        if(invt_timer_1s[id][INVT_TIMER_1S_RESET_CHG] < 3)  // ʹ��λ����֣��Ա������ݱ仯
            SET_BIT(temp, INVT_CNTR_ERR_RESET);
        if (id == INVT_ID_1)
        {
            SET_BMS_DEBUG_VAL(BDV_INVT_CMD_INVT_ID_1, temp);
        }
        else if (id == INVT_FAN1)
        {
            SET_BMS_DEBUG_VAL(BDV_INVT_CMD_INVT_FAN1, temp);
        }
        else if (id == INVT_FAN2)
        {
            SET_BMS_DEBUG_VAL(BDV_INVT_CMD_INVT_FAN2, temp);
        }
    }

    
    //����ı䣬����һ������
    if(invt_control_bak[id] != invt_cmd[id][CMD_CONTROL])
    {
        invt_control_bak[id] = invt_cmd[id][CMD_CONTROL];
        fg_cmd_changed[id][CMD_CONTROL]=TRUE;
    }

    
//    // ��������
//    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_SAVE))
//    {
//        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_SAVE);
//        invt_cmd[id][CMD_EEP_SAVE]=1;
//        fg_cmd_changed[id][CMD_EEP_SAVE]=TRUE;
//    }
    
    invt_timer_100ms[id][INVT_TIMER_100MS_CMD_UPDATE]=0;  
}

/*******************************************************************************
 �� ��: invt_deal_pfc_cmd()
 �� ��: ����ʹ��/������ԴPFC����������Ӧ��ͨ������ֵ: ����(=55)��ʹ��(����ֵ)
 �� ��: id--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void invt_deal_pfc_cmd(INVT_ID_ENUM id)
{
    #define CMD_UPDATE_INTRVL        5  // ����������

    if(invt_timer_100ms[id][INVT_TIMER_100MS_PFC_CMD_UPDATE]<CMD_UPDATE_INTRVL)   return;     // ʱ��δ����������

    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_OFF))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_OFF);
        invt_cmd[id][CMD_SET_PFC] = 55;
        fg_cmd_changed[id][CMD_SET_PFC]=TRUE;
    }

    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_ON))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_ON);
        invt_cmd[id][CMD_SET_PFC] = 0;
        fg_cmd_changed[id][CMD_SET_PFC]=TRUE;
    }

    invt_timer_100ms[id][INVT_TIMER_100MS_PFC_CMD_UPDATE]=0;  
}

/*******************************************************************************
 �� ��: invt_deal_freq()
 �� ��: ��ӦӦ�ó���������Ƶ��
 �� ��: pUart--ͨѶ����; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static void invt_deal_freq(INVT_ID_ENUM id)//(UARTBUF *pUart)
{    
    #define FREQ_CONFRM_TIME      5   //ת��ȷ��ʱ��(��)
    
//    static U16 timer_interval = 0;    //���ͼ������
//    static U16 invt_freq_set_bak = 0; //�趨ת�ٱ���
//    BOOL fg_send_new_freq = FALSE;    //������ת��
    BOOL fg_freq_not_right = FALSE;   //����ת�ٲ����趨ת��
    BOOL fg_freq_changed = FALSE;     //�趨ת�ٸı�

    //����ת�ٲ����趨ת�� �� ������һ��ʱ��
    if(invt_info_freq_arm(id) != invt_freq_set[id]
    && invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ] >= FREQ_CONFRM_TIME)
    {
        fg_freq_not_right = TRUE;
    }
    //�趨ת�ٸı�
    if(invt_cmd[id][CMD_SET_FREQ] != invt_freq_set[id])
    {
        fg_freq_changed = TRUE;
    }
    //����һ��ת��
    if(invt_info_is_running(id) && (fg_freq_not_right || fg_freq_changed))
    {
        invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ] = 0;
//        invt_freq_set_bak = invt_freq_set;
        
        invt_cmd[id][CMD_SET_FREQ]=invt_freq_set[id];
        fg_cmd_changed[id][CMD_SET_FREQ]=TRUE;
        
//        invt_uart_send_frequency(pUart, invt_freq_set);
//        fg_send_new_freq = TRUE;
    }

//    return fg_send_new_freq;
}

/*******************************************************************************
 �� ��: invt_deal_freq_ec()
 �� ��: ��ӦӦ�ó�������ec���ת��
 �� ��: id--��Ƶ����; ec--ec����ţ�
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void invt_deal_freq_ec(INVT_ID_ENUM id, U08 ec)
{    
    I16 value_now = invt_freq_set_ec[id][ec];
    U08 cmd_idx = CMD_SET_FREQ_EC1+ec;		/* ע�⣺����Ų�����Ӧ����Ӧ�޸� */
    
    // ����һ��ת��
    if (invt_cmd[id][cmd_idx] != value_now
    || (value_now==0 
        && (invt_info_pulse_ec1(id)!=0 && ec==FREQ_FAN1
            || invt_info_pulse_ec2(id)!=0 && ec==FREQ_FAN2)	/* �ջ��ط�����˴�����������ͼ�� */
        && invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ_FAN] >= 1)
        )
    {
        invt_cmd[id][cmd_idx]=value_now;
        fg_cmd_changed[id][cmd_idx]=TRUE;
        invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ_FAN] = 0;
    }
}

/*******************************************************************************
 �� ��: invt_cmd_isChanged()
 �� ��: �жϱ�Ƶ�������Ƿ����޸�
 �� ��: INVT_ID_ENUM id--��Ƶ�����; 
 �� ��: FALSE:���޸ģ�����:�޸ĵ��������+1
 �� ��: ��
*******************************************************************************/
U08 invt_cmd_isChanged(INVT_ID_ENUM id)
{
    U08 ChangeIndex=FALSE;
    U08 i;

    invt_deal_cmd(id);              // ��������
    invt_deal_pfc_cmd(id);          // ����PFC����
    invt_deal_freq(id);             // ����Ƶ��
    invt_deal_freq_ec(id, FREQ_FAN1); // ����ec1
    invt_deal_freq_ec(id, FREQ_FAN2); // ����ec2
    
    for(i=0; (i<CMD_INVT_MAX && i<0xFF) ; i++)
    {
        if (fg_cmd_changed[id][i])
        {
            ChangeIndex = i+1;
            
            break;
        }
    }
    return ChangeIndex;
}
/*******************************************************************************
 �� ��: invt_cmd_backup()
 �� ��: ��Ƶ������ʹ�ú󱸷ݣ�ʧЧ��
 �� ��: INVT_ID_ENUM id--��Ƶ�����; U08 index--�������

 �� ��: 0xFF:���޸ģ�����:�޸ĵ��������
 �� ��: ��
*******************************************************************************/
void invt_cmd_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<CMD_INVT_MAX)
    {
//        invt_cmd_bak[id][index]=invt_cmd[id][index];
        fg_cmd_changed[id][index]=FALSE;
//        if(index==CMD_EEP_SAVE)
//        {
//            invt_cmd[id][index]=0;  // ����Ϊ1ʱ���󷢳�����
//        }
    }
}

/*******************************************************************************
 �� ��: invt_para_isChanged()
 �� ��: �жϱ�Ƶ�������Ƿ����޸�
 �� ��: INVT_ID_ENUM id--��Ƶ�����; 
 �� ��: FALSE:���޸ģ�����:�޸ĵĲ������+1
 �� ��: ��
*******************************************************************************/
U16 invt_para_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
//    U08 i;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //�����²���
    
//    invt_deal_para(id);  // ��������
    
    //����Ƿ��в�������
    if(fg_para_changed[id][F00_MOTOR_TYPE])	/* 2023-4-28 �ȷ�����ͺ��ٷ������ޣ���ֹ�����ͺŸ��ĺ��ָֻ������� */
	{
//	    fg_para_changed[id][F00_MOTOR_TYPE] = FALSE;
	    para_id = F00_MOTOR_TYPE;
	    fg_send_para = TRUE;
	}
    else if(fg_para_changed[id][F00_FREQ_MAX])
	{
//	    fg_para_changed[id][F00_FREQ_MAX] = FALSE;
	    para_id = F00_FREQ_MAX;
	    fg_send_para = TRUE;
	}
    else if(fg_para_changed[id][F00_FREQ_MIN])
	{
//	    fg_para_changed[id][F00_FREQ_MIN] = FALSE;
	    para_id = F00_FREQ_MIN;
	    fg_send_para = TRUE;
	}
    

    //���Ĳ���
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //������󱣴�
    }
    else if(fg_para_changed[id][F00_PARA_INIT])
	{
//	    fg_para_changed[id][F00_PARA_INIT] = FALSE;
	    para_id = F00_PARA_INIT;

        ChangeIndex= para_id+1;
        invt_cmd_para_read(id);//��ʼ�����ȡ          //NOTE-CXW ������ʱ��˳��
	}
    
    return ChangeIndex;
}




/*******************************************************************************
 �� ��: invt_paras_backup()
 �� ��: ��Ƶ�������޸ĺ�ȡ���޸ı�־
 �� ��: INVT_ID_ENUM id--��Ƶ�����; U08 index--�������
 �� ��: 0xFF:���޸ģ�����:�޸ĵ��������
 �� ��: ��
*******************************************************************************/
void invt_paras_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F00_MAX)
    {
        fg_para_changed[id][index]=FALSE;

    }
}

/*******************************************************************************
 �� ��: invt_timer()
 �� ��: ģ���ʱ����
 �� ��: BOOL TimeFlag ʱ���־���ò������ݣ�������ֲʱ100ms��־���Ʋ�ͬ��
 �� ��: ��
 �� ��: ��100ms��ʱ��������
*******************************************************************************/
void invt_timer(BOOL TimeFlag)
{
    U08 i,j;
    static U08 cnt_100ms_to_1s = 0; //100���뻻���1��

    if(TimeFlag)
    {
        for(i=0; i<INVT_ID_MAX; i++)
        {
            for(j=0; j<INVT_TIMER_100MS_MAX; j++)
            {
            	if(invt_timer_100ms[i][j] != 0xffff) invt_timer_100ms[i][j]++;
            }
        }     
        if(++cnt_100ms_to_1s >= 10)
        {
            cnt_100ms_to_1s = 0;
            for(i=0; i<INVT_ID_MAX; i++)
            {
                for(j=0; j<INVT_TIMER_1S_MAX; j++)
                {
                	if(invt_timer_1s[i][j] != 0xffff) invt_timer_1s[i][j]++;
                }
            }
        }
    }
}

/*******************************************************************************
 �� ��: invt_init()
 �� ��: ��ʼ����Ƶ��ͨѶģ��
        ��ʼ�����ݣ�ֹͣ��Ƶ�����������Ƶ�ʼ����٣���ȡһ�β�����
 �� ��: INVT_ID_ENUM id ��Ƶ�����
 �� ��: ��
 �� ��: ��Ӧ�ó����ʼ���е���
*******************************************************************************/
void invt_init()
{
    INVT_ID_ENUM i;
    U08 j;

    //������������ʼΪ��Чֵ
    for(i=(INVT_ID_ENUM)0; i<INVT_ID_MAX; i++)
    {
        //�ػ�����������ȡ����
        //invt_cmd_off(i);
        invt_cmd_off_free(i);
        invt_cmd_freq_unlock(i);
        invt_cmd_acclrt_unlock(i);
        invt_cmd_para_read(i);
        
        for(j=0; j<F00_MAX; j++)
        {
        	fg_para_changed[i][j] = FALSE;
        	invt_para[i][j] = INVT_PARA_VALUE_NULL;
        }
        for(j=0; j<CMD_INVT_MAX; j++)
        {
        	fg_cmd_changed[i][j] = FALSE;
        	invt_cmd[i][j] = INVT_PARA_VALUE_NULL;
//            invt_cmd_bak[i][j] = INVT_PARA_VALUE_NULL;
        }

        for(j=0; j<INVT_TIMER_100MS_MAX; j++)
        {
        	invt_timer_100ms[i][j] = 0;
        }
        for(j=0; j<INVT_TIMER_1S_MAX; j++)
        {
        	invt_timer_1s[i][j] = 0;
        }
    }
}



/*******************************************************************************
 �� ��: invt_info_is_running()
 �� ��: ��Ƶ������״̬
 �� ��: ��
 �� ��: TRUE/FALSE--������/������
 �� ��: ��
*******************************************************************************/
BOOL invt_info_is_running(INVT_ID_ENUM id)
{
    return (GET_BIT(invt_info[id].state, INVT_STATE_RUN) == 1);
}

/*******************************************************************************
 �� ��: invt_info_is_alarm()
 �� ��: ��Ƶ������״̬
 �� ��: INVT_ID_ENUM id ��Ƶ�����
 �� ��: TRUE/FALSE--�й���/�޹���
 �� ��: ��
*******************************************************************************/
BOOL invt_info_is_alarm(INVT_ID_ENUM id)
{
    return (GET_BIT(invt_info[id].state, INVT_STATE_ERR) == 1);
}

/*******************************************************************************
 �� ��: invt_info_version()
 �� ��: ��ȡ����汾��
 �� ��: INVT_ID_ENUM id ��Ƶ�����
 �� ��: ��Ƶ������汾��
 �� ��: ��
*******************************************************************************/
U16 invt_info_version(INVT_ID_ENUM id)
{
    return invt_para[id][F00_VERSION];
}

/*******************************************************************************
 �� ��: invt_info_function()
 �� ��: ��ȡ���������
 �� ��: INVT_ID_ENUM id ��Ƶ�����
 �� ��: ��Ƶ�����������
 �� ��: ��
*******************************************************************************/
U16 invt_info_function(INVT_ID_ENUM id)
{
    return invt_para[id][F00_FUNCTION];
}
/*******************************************************************************
 �� ��: invt_info_freq_run()
 �� ��: �鿴��Ƶ������״̬
 �� ��: ��
 �� ��: ��Ƶ������ת��(0.01rps)
 �� ��: ����ת��Ϊ��Ƶ���˿̵�����ת��
*******************************************************************************/
U16 invt_info_freq_state(INVT_ID_ENUM id)
{
    return invt_info[id].state;
}

/*******************************************************************************
 �� ��: invt_info_freq_run()
 �� ��: �鿴����ת��
 �� ��: ��
 �� ��: ��Ƶ������ת��(0.01rps)
 �� ��: ����ת��Ϊ��Ƶ���˿̵�����ת��
*******************************************************************************/
U16 invt_info_freq_run(INVT_ID_ENUM id)
{
    return invt_info[id].freq_run;
}

/*******************************************************************************
 �� ��: invt_info_freq_arm()
 �� ��: �鿴Ŀ��ת��
 �� ��: ��
 �� ��: ��Ƶ���趨ת��(0.01rps)
 �� ��: Ŀ��ת��Ϊ��Ƶ�����յ����趨ת��
*******************************************************************************/
U16 invt_info_freq_arm(INVT_ID_ENUM id)
{
    return invt_info[id].freq_set;
}

/*******************************************************************************
 �� ��: invt_info_freq_now()
 �� ��: �鿴��ǰת��
 �� ��: ��
 �� ��: ��Ƶ����ǰת��(0.01rps)
 �� ��: ��ǰת��Ϊ��Ƶ���˿̵���ʵת��
*******************************************************************************/
U16 invt_info_freq_now(INVT_ID_ENUM id)
{
    return invt_info[id].freq_now;
}

/*******************************************************************************
 �� ��: invt_info_temperature()
 �� ��: �鿴��Ƶģ���¶�
 �� ��: ��
 �� ��: ��ǰ�¶�(0.1��)
 �� ��: ��
*******************************************************************************/
I16 invt_info_temperature(INVT_ID_ENUM id)
{
    return invt_info[id].temperature;
}

/*******************************************************************************
 �� ��: invt_info_powerout()
 �� ��: �鿴��Ƶģ���������
 �� ��: ��
 �� ��: ��ǰ�������(0.01KW)
 �� ��: ��
*******************************************************************************/
U16 invt_info_powerout(INVT_ID_ENUM id)
{
    return invt_info[id].power_out;
}


/*******************************************************************************
 �� ��: invt_info_error_id()
 �� ��: �鿴���Ϻ�
 �� ��: ��
 �� ��: ��Ƶ����ǰ���ϵĹ��Ϻ�
 �� ��: ֻ�ܲ鿴�����緢���Ĺ��ϣ���ͬʱ���ڶ������ʱ��ֻ�ܵȸù��ϸ�λ��
        ���ܲſ������Ĺ��ϡ�
*******************************************************************************/
U16 invt_info_error_id(INVT_ID_ENUM id)
{
    return invt_info[id].errid;
}

/*******************************************************************************
 �� ��: invt_clr_error_id()
 �� ��: ��չ��Ϻ�
 �� ��: ��
 �� ��: 
 �� ��: 
*******************************************************************************/
void invt_clr_error_id(INVT_ID_ENUM id)
{
     invt_info[id].errid = 0;
}

/*******************************************************************************
 �� ��: invt_info_errorcode1_id()
 �� ��: �鿴���Ϻ�1
 �� ��: ��
 �� ��: ��Ƶ����ǰ���ϵĹ��Ϻ�
 �� ��: ֻ�ܲ鿴�����緢���Ĺ��ϣ���ͬʱ���ڶ������ʱ��ֻ�ܵȸù��ϸ�λ��
        ���ܲſ������Ĺ��ϡ�
*******************************************************************************/
U16 invt_info_errorcode1_id(INVT_ID_ENUM id)
{
    return invt_info[id].errcode1;
}
/*******************************************************************************
 �� ��: invt_info_errorcode1_id()
 �� ��: �鿴���Ϻ�1
 �� ��: ��
 �� ��: ��Ƶ����ǰ���ϵĹ��Ϻ�
 �� ��: ֻ�ܲ鿴�����緢���Ĺ��ϣ���ͬʱ���ڶ������ʱ��ֻ�ܵȸù��ϸ�λ��
        ���ܲſ������Ĺ��ϡ�
*******************************************************************************/
U16 invt_info_temp_pfc(INVT_ID_ENUM id)
{
    return invt_info[id].errcode2;
}
/*******************************************************************************
 �� ��: invt_info_freq_limit_id()
 �� ��: �鿴��Ƶ��Ϣ
 �� ��: ��
 �� ��: ��Ƶ����ǰ���ϵĹ��Ϻ�
 �� ��: ֻ�ܲ鿴�����緢���Ĺ��ϣ���ͬʱ���ڶ������ʱ��ֻ�ܵȸù��ϸ�λ��
        ���ܲſ������Ĺ��ϡ�
*******************************************************************************/
U16 invt_info_freq_limit_id(INVT_ID_ENUM id)
{
    return invt_info[id].freq_limit;
}


/*******************************************************************************
 �� ��: invt_info_current_out()
 �� ��: �鿴�������(0.1A)
 �� ��: ��
 �� ��: ��Ƶ����ǰ�������
 �� ��: ��
*******************************************************************************/
U16 invt_info_current_out(INVT_ID_ENUM id)
{
    return invt_info[id].current_out;
}
/*******************************************************************************
 �� ��: invt_info_torque_out()
 �� ��: �鿴���ת��
 �� ��: ��
 �� ��: ��Ƶ����ǰ���ת��
 �� ��: ��
*******************************************************************************/
U16 invt_info_torque_out(INVT_ID_ENUM id)
{
    return invt_info[id].torque_out;
}


/*******************************************************************************
 �� ��: invt_info_voltage_out()
 �� ��: �鿴�����ѹ
 �� ��: ��
 �� ��: ��Ƶ����ǰ�����ѹ
 �� ��: ��
*******************************************************************************/
U16 invt_info_voltage_out(INVT_ID_ENUM id)
{
    return invt_info[id].volt_out;
}

/*******************************************************************************
 �� ��: invt_info_volt_generatrix()
 �� ��: �鿴ĸ�ߵ�ѹ
 �� ��: ��
 �� ��: ��Ƶ����ǰĸ�ߵ�ѹ
 �� ��: ��
*******************************************************************************/
U16 invt_info_volt_generatrix(INVT_ID_ENUM id)
{
    return invt_info[id].volt_generatrix;
}

/*******************************************************************************
 �� ��: invt_info_pulse_ec1()
 �� ��: �鿴ec1������
 �� ��: id--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U16 invt_info_pulse_ec1(INVT_ID_ENUM id)
{
    return invt_info[id].pulse_ec1;
}

/*******************************************************************************
 �� ��: invt_info_pulse_ec2()
 �� ��: �鿴ec2������
 �� ��: id--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U16 invt_info_pulse_ec2(INVT_ID_ENUM id)
{
    return invt_info[id].pulse_ec2;
}


#define PULSE_SAMPLE_MAX  (20)
/*******************************************************************************
 �� ��: invt_info_pulse_ec_filt()
 �� ��: ���������������˲�
 �� ��: invt_id--; 
		fan_id--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U16 invt_info_pulse_ec_filt(INVT_ID_ENUM invt_id, U08 fan_id)
{
    U08 i;
    U16 pulse_sum, pulse_filt, pulse_now;
    
    static U16 pulse_prev[FREQSPEEDNUM] = { 0xffff, 0xffff };
    static U16 pulse_buf[FREQSPEEDNUM][PULSE_SAMPLE_MAX];

    if (fan_id > FREQSPEEDNUM)
    {
        return 0;
    }
    
    /* ѡ�������� */
    switch (fan_id)
    {
        default:
        case FREQ_FAN1:  pulse_now = invt_info_pulse_ec1(invt_id);  break;
        case FREQ_FAN2:  pulse_now = invt_info_pulse_ec2(invt_id);  break;
    }

    /* ���������˲� */
    if (pulse_prev[fan_id] != pulse_now)
    {
        /* ��ʼ�� */
        if (pulse_prev[fan_id] == 0xffff)
        {
            pulse_prev[fan_id] = pulse_now;
            for (i = 0; i < PULSE_SAMPLE_MAX; i++)
            {
                pulse_buf[fan_id][i] = pulse_now;
            }
        }
        else
        {
            for (i = 0; i < (PULSE_SAMPLE_MAX-1); i++)
            {
                pulse_buf[fan_id][i] = pulse_buf[fan_id][i+1];
            }
            pulse_buf[fan_id][i] = pulse_now;
        }
    }

    /* �����ڲ���ֵ���&ȡƽ�� */
    pulse_sum = 0;
    for (i = 0; i < PULSE_SAMPLE_MAX; i++)
    {
        pulse_sum += pulse_buf[fan_id][i];
    }
    pulse_filt = (U32)(pulse_sum*60)/(val_fan_pwm_pulse*PULSE_SAMPLE_MAX);
    
    return pulse_filt;
}

/*******************************************************************************
 �� ��: invt_set_rps_out()
 �� ��: ����Ŀ�����ת��(����ת�����ޡ���������)
 �� ��: freq_new--���ת��(0.01rps); 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_set_rps_out(INVT_ID_ENUM id, U16 freq_new)
{
    I16 max = invt_para[id][F00_FREQ_MAX];
    I16 min = invt_para[id][F00_FREQ_MIN];

    //�趨ת�ٷ�Χ���
    if(max != INVT_PARA_VALUE_NULL && min != INVT_PARA_VALUE_NULL)
    {
        if(freq_new > max)
        {   
            freq_new = max;
        }
        if(freq_new < min)
        {
            freq_new = min;
        }
    }
        
        invt_freq_set[id] = freq_new;
}


/*******************************************************************************
 �� ��: invt_get_rps_out()
 �� ��: ��ȡĿ�����ת��(����ת�����ޡ���������)
 �� ��: id--��Ƶ��ID; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
I16 invt_get_rps_out(INVT_ID_ENUM id)
{
    return invt_freq_set[id] ;
}

/*******************************************************************************
 �� ��: invt_set_rps_ec()
 �� ��: ����Ŀ��ecת��
 �� ��: freq_new--Ŀ��ecת��(0��100%); 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_set_rps_ec(INVT_ID_ENUM id, U08 ec, U16 freq_new)
{
    invt_freq_set_ec[id][ec] = freq_new;
}

/*******************************************************************************
 �� ��: invt_get_rps_ec()
 �� ��: ��ȡĿ��ecת��
 �� ��: id--��Ƶ��ID; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
I16 invt_get_rps_ec(INVT_ID_ENUM id, U08 ec)
{
    return invt_freq_set_ec[id][ec];
}


/*******************************************************************************
 �� ��: invt_para_read()
 �� ��: ��ȡһ��������ֵ
 �� ��: INVT_ID_ENUM id--��Ƶ����� para_id--������; 
 �� ��: ����ֵ
 �� ��: ��
*******************************************************************************/
I16 invt_para_read(INVT_ID_ENUM id,INVT_PARA_ID para_id)
{     
    I16 para_value = INVT_PARA_VALUE_NULL;
    
    switch (para_id)
    {
    	case INVT_PARA_MOTOR_TYPE://����ͺ�(0:����6TD075X��1:����5SS072X��2:����)
    	    para_value = invt_para[id][F00_MOTOR_TYPE];
    		break;
    		
    	case INVT_PARA_FREQ_MAX://ת������(0.01rps)
    	    para_value = invt_para[id][F00_FREQ_MAX];
    		break;
    		
    	case INVT_PARA_FREQ_MIN://ת������(0.01rps)
    	    para_value = invt_para[id][F00_FREQ_MIN];
    		break;

        case INVT_PARA_FREQ_START:// ��������ת��
    	    para_value = invt_para_fa[id][FA_FREQ_START];
    		break;

        case INVT_PARA_POLE:// �������
    	    para_value = invt_para_fa[id][FA_FREQ_POLE];
    		break;
    		
        case INVT_PARA_POWER:// ���ʵȼ�
    	    para_value = invt_para_f10[id][F10_POWER];
    		break;
        case INVT_PARA_EC_NUM:// EC�������
    	    para_value = invt_para_f9[id][F9_EC_FAN_NUM];
    		break;
        case INVT_PARA_EC_PWM_SW_FREQ:// EC���PWM����Ƶ��
            para_value = invt_para_f6[id][F6_00_EC_PWM_SW_FREQ];
            break;
    }
    
    return para_value;
}

/*******************************************************************************
 �� ��: invt_para_set()
 �� ��: ��дһ��������ֵ
 �� ��: INVT_ID_ENUM id--��Ƶ����� 
        para_id--������; 
		para_value--����ֵ; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_para_set(INVT_ID_ENUM id, INVT_PARA_ID para_id, I16 para_value)
{
    //�������ݼ��
    if(para_value == INVT_PARA_VALUE_NULL)
    {
        return;
    }
    
    switch (para_id)
    {
    	case INVT_PARA_MOTOR_TYPE://����ͺ�(0:����6TD075X��1:����5SS072X��2:����)
    	    if(invt_para[id][F00_MOTOR_TYPE] != para_value)
    	    {
        	    fg_para_changed[id][F00_MOTOR_TYPE] = TRUE;
        	    invt_para[id][F00_MOTOR_TYPE] = para_value;
    	    }
    		break;
    		
    	case INVT_PARA_FREQ_MAX://ת������(0.01rps)
    	    if(invt_para[id][F00_FREQ_MAX] != para_value)
    	    {
        	    fg_para_changed[id][F00_FREQ_MAX] = TRUE;
        	    invt_para[id][F00_FREQ_MAX] = para_value;
    	    }
    		break;
    		
    	case INVT_PARA_FREQ_MIN://ת������(0.01rps)
    	    if(invt_para[id][F00_FREQ_MIN] != para_value)
    	    {
        	    fg_para_changed[id][F00_FREQ_MIN] = TRUE;
        	    invt_para[id][F00_FREQ_MIN] = para_value;
    	    }
    		break;
            
     	case INVT_PARA_FREQ_START:// ��������ת��
    	    {
                U16 freq_start = para_value*invt_para_read(id, INVT_PARA_POLE)/2;
    	        if (invt_para_fa[id][FA_FREQ_START] != freq_start
                &&  fun_pv_in(freq_start, 100, 30000)		/* ��Χ���� */
                    )
        	    {
            	    fg_para_fa_changed[id][FA_FREQ_START] = TRUE;
            	    invt_para_fa[id][FA_FREQ_START] = freq_start;
            	    fg_para_fa_changed[id][FA_FREQ_START2] = TRUE;
            	    invt_para_fa[id][FA_FREQ_START2] = freq_start;	
        	    }
    	    }
    		break;
        case INVT_PARA_POWER:// ���ʵȼ�
    	    if(invt_para_f10[id][F10_POWER] != para_value)
    	    {
        	    fg_para_f10_changed[id][F10_POWER] = TRUE;
        	    invt_para_f10[id][F10_POWER] = para_value;
    	    }
    		break;
        case INVT_PARA_EC_NUM:// EC�������
    	    if(invt_para_f9[id][F9_EC_FAN_NUM] != para_value)
    	    {
        	    fg_para_f9_changed[id][F9_EC_FAN_NUM] = TRUE;
        	    invt_para_f9[id][F9_EC_FAN_NUM] = para_value;
    	    }
    		break;
   		case INVT_PARA_EC_PWM_SW_FREQ:// EC���PWM����Ƶ��
   		    if (invt_para_f6[id][F6_00_EC_PWM_SW_FREQ] != para_value)
   		    {
                fg_para_f6_changed[id][F6_00_EC_PWM_SW_FREQ] = TRUE;
        	    invt_para_f6[id][F6_00_EC_PWM_SW_FREQ] = para_value;
            }
            break;
    	default:
    		break;
    }
}

/*******************************************************************************
 �� ��: invt_para_init()
 �� ��: ��ʼ����Ƶ������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_para_init(void)      //NOTE-CXW ��ʼ����Ҫ����
{
    INVT_ID_ENUM i;
    for(i=(INVT_ID_ENUM)0; i<INVT_ID_MAX; i++)
    {
        invt_cmd_para_init(i);
    }
}

/*******************************************************************************
 �� ��: invt_err_reset()
 �� ��: ��Ƶ�����ϸ�λ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void invt_err_reset(void)
{
    INVT_ID_ENUM id;
    for (id=INVT_FAN1; id<INVT_ID_MAX; id++)			/* ��Ƶ��� */
    {
        invt_cmd_err_reset(id);
    }
    
    err_mod_invt_err_reset(INVT_ID_1);			/* ��Ƶѹ�� */
}

