/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: IO.c
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#define  COM_M_GLOBAL
#include "includes.h"

/****************************************************************
������12.3-2 ��������ӻ�(com_invt_wr)
���ܣ��������Ϳ�������ӻ���
���������ںţ��豸��д���ݵ�ַ��д������
���أ��ɹ�/ʧ��
��������ַ��Чʱ������ͨѶ��㡣
*****************************************************************/
U08 com_invt_wr(U08 com, U08 EX_addr, U16 wr_addr, U08 wr_num, U08 prio)
{
	U08 wr_succ = COM_SUCC;
	U16 *buf = NULL;
	U16 addr_offst = 0;
    U16 addr_invt_para, addr_invt_para_fa,addr_invt_para_fb,addr_invt_para_f10,addr_invt_para_f9,addr_invt_cmd, addr_invt_para_f6;
    U08 size_invt_para, size_invt_para_fa,size_invt_para_fb,size_invt_para_f10,size_invt_para_f9,size_invt_cmd, size_invt_para_f6;
    INVT_ID_ENUM id = (INVT_ID_ENUM)EX_addr;

        if (id < INVT_ID_MAX)   // ��Ƶ��
        {
            addr_invt_para = invt_paras_get_addr();     // ��Ƶ��������ַ
            size_invt_para = invt_paras_get_size();     // ��Ƶ��������С
            addr_invt_cmd  = invt_cmd_get_addr();       // ��Ƶ��ͨѶָ���ַ
            size_invt_cmd  = invt_cmd_get_size();       // ��Ƶ��ͨѶָ���С

            addr_invt_para_fa = invt_paras_fa_get_addr();     // ��Ƶ��FA������ַ
            size_invt_para_fa = invt_paras_fa_get_size();     // ��Ƶ��FA������С

            addr_invt_para_fb = invt_paras_fb_get_addr();     // ��Ƶ��FB������ַ
            size_invt_para_fb = invt_paras_fb_get_size();     // ��Ƶ��FB������С

            addr_invt_para_f10 = invt_paras_f10_get_addr();     // ��Ƶ��F10������ַ
            size_invt_para_f10 = invt_paras_f10_get_size();     // ��Ƶ��F10������С

            addr_invt_para_f9 = invt_paras_f9_get_addr();     // ��Ƶ��F9������ַ
            size_invt_para_f9 = invt_paras_f9_get_size();     // ��Ƶ��F9������С

            addr_invt_para_f6 = invt_paras_f6_get_addr();     // ��Ƶ��F6������ַ
            size_invt_para_f6 = invt_paras_f6_get_size();     // ��Ƶ��F6������С
            
            if (wr_addr >= info_invt_para[id].addr              /* ��Ƶ�������� */
            && (wr_addr+wr_num) <= (info_invt_para[id].addr+1)
            &&  info_invt_para[id].fg_wr
            )  
            {
                addr_offst = wr_addr - info_invt_para[id].addr;
                buf = (U16 *)&info_invt_para[id].val+addr_offst;
                info_invt_para[id].fg_wr = FALSE;
            }
            else if (wr_addr >= addr_invt_para && (wr_addr+wr_num) <= (addr_invt_para+size_invt_para))			
        	{
        		addr_offst = wr_addr - addr_invt_para;
                buf = (U16 *)invt_paras_get_buf(id)+addr_offst;
        	}
        	else if(wr_addr >= addr_invt_para_fa && (wr_addr+wr_num) <= (addr_invt_para_fa+size_invt_para_fa))
        	{
        		addr_offst = wr_addr - addr_invt_para_fa;
                buf = (U16 *)invt_paras_fa_get_buf(id)+addr_offst;
        	}
            else if(wr_addr >= addr_invt_para_fb && (wr_addr+wr_num) <= (addr_invt_para_fb+size_invt_para_fb))
        	{
        		addr_offst = wr_addr - addr_invt_para_fb;
                buf = (U16 *)invt_paras_fb_get_buf(id)+addr_offst;
            }
            else if(wr_addr >= addr_invt_para_f10 && (wr_addr+wr_num) <= (addr_invt_para_f10+size_invt_para_f10))
        	{
        		addr_offst = wr_addr - addr_invt_para_f10;
                buf = (U16 *)invt_paras_f10_get_buf(id)+addr_offst;
        	}
            else if(wr_addr >= addr_invt_para_f9 && (wr_addr+wr_num) <= (addr_invt_para_f9+size_invt_para_f9))
        	{
        		addr_offst = wr_addr - addr_invt_para_f9;
                buf = (U16 *)invt_paras_f9_get_buf(id)+addr_offst;
            }
            else if(wr_addr >= addr_invt_para_f6 && (wr_addr+wr_num) <= (addr_invt_para_f6+size_invt_para_f6))
            {
                addr_offst = wr_addr - addr_invt_para_f6;
                buf = (U16 *)invt_paras_f6_get_buf(id)+addr_offst;
            }
        	else if (wr_addr >= addr_invt_cmd && (wr_addr+wr_num) <= (addr_invt_cmd+size_invt_cmd))			
        	{
        		addr_offst = wr_addr - addr_invt_cmd;
                buf = (U16 *)invt_cmd_get_buf(id)+addr_offst;
        	}
        	else
            {
                wr_succ = COM_ERR_DATA_ADDR;		/* ��ַ���磬���������� */
            }
        	
        	if (wr_succ == COM_SUCC)
        	{
        		wr_succ = COM_WriteWords(com, invt_get_device_addr(id), wr_addr, wr_num, buf,prio);
        	}
        }
        else
        {
        	wr_succ = COM_ERR_MAC_ADDR;		/* �豸��ַ�� */
        }
	
	return wr_succ;
}


/****************************************************************
12.3-3 ���ӻ�����(com_invt_rd)
���ܣ�������ȡ�ӻ����ݡ�
���������ںţ��豸�������ݵ�ַ����������
���أ��ɹ�/ʧ��/����
������/��
*****************************************************************/
U08 com_invt_rd(U08 com, U08 EX_addr, U16 rd_addr, U08 rd_num,U08 prio)
{
	U08 rd_succ = COM_SUCC;
	U16 *buf = NULL;
    U16 addr_offst = 0;

    U16 addr_invt_state, addr_invt_para, addr_invt_para_fa, addr_invt_para_fb, addr_invt_para_fp, addr_invt_para_f10, addr_invt_para_f9, addr_invt_para_f6;
    U08 size_invt_state, size_invt_para, size_invt_para_fa, size_invt_para_fb, size_invt_para_fp, size_invt_para_f10, size_invt_para_f9, size_invt_para_f6;
    INVT_ID_ENUM id = (INVT_ID_ENUM)EX_addr;
	
	if (id < INVT_ID_MAX)   // ��Ƶ��
    {
        addr_invt_state = invt_states_get_addr();
        size_invt_state = invt_states_get_size();
        addr_invt_para = invt_paras_get_addr();
        size_invt_para = invt_paras_get_size();
        addr_invt_para_fa = invt_paras_fa_get_addr();
        size_invt_para_fa = invt_paras_fa_get_size();
        addr_invt_para_fb = invt_paras_fb_get_addr();
        size_invt_para_fb = invt_paras_fb_get_size();
        addr_invt_para_fp = invt_paras_fp_get_addr();
        size_invt_para_fp = invt_paras_fp_get_size();
        addr_invt_para_f10 = invt_paras_f10_get_addr();
        size_invt_para_f10 = invt_paras_f10_get_size();
        addr_invt_para_f9 = invt_paras_f9_get_addr();
        size_invt_para_f9 = invt_paras_f9_get_size();
        addr_invt_para_f6 = invt_paras_f6_get_addr();
        size_invt_para_f6 = invt_paras_f6_get_size();
        
        if (rd_addr >= info_invt_para[id].addr              /* ��Ƶ�������� */
        && (rd_addr+rd_num) <= (info_invt_para[id].addr+1)
        &&  info_invt_para[id].fg_rd
        )  
        {
            addr_offst = rd_addr - info_invt_para[id].addr;
            buf = (U16 *)&info_invt_para[id].val+addr_offst;
            info_invt_para[id].fg_rd = FALSE;
        }
        else if (rd_addr >= addr_invt_state && (rd_addr+rd_num) <= (addr_invt_state+size_invt_state))  /* ��Ƶ��״̬ */
        {
            addr_offst = rd_addr - addr_invt_state;
            buf = (U16 *)invt_states_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para && (rd_addr+rd_num) <= (addr_invt_para+size_invt_para))  /* ��Ƶ���������� */
        {
            addr_offst = rd_addr - addr_invt_para;
            buf = (U16 *)invt_paras_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_fa && (rd_addr+rd_num) <= (addr_invt_para_fa+size_invt_para_fa))  /* ��Ƶ���������� */
        {
            addr_offst = rd_addr - addr_invt_para_fa;
            buf = (U16 *)invt_paras_fa_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_fb && (rd_addr+rd_num) <= (addr_invt_para_fb+size_invt_para_fb))  /* ��Ƶ���������� */
        {
            addr_offst = rd_addr - addr_invt_para_fb;
            buf = (U16 *)invt_paras_fb_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_fp && (rd_addr+rd_num) <= (addr_invt_para_fp+size_invt_para_fp))  /* ��Ƶ���������� */
        {
            addr_offst = rd_addr - addr_invt_para_fp;
            buf = (U16 *)invt_paras_fp_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_f10 && (rd_addr+rd_num) <= (addr_invt_para_f10+size_invt_para_f10))  /* ��Ƶ���������� */
        {
            addr_offst = rd_addr - addr_invt_para_f10;
            buf = (U16 *)invt_paras_f10_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_f9 && (rd_addr+rd_num) <= (addr_invt_para_f9+size_invt_para_f9))  /* ��Ƶ���������� */
        {
            addr_offst = rd_addr - addr_invt_para_f9;
            buf = (U16 *)invt_paras_f9_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_f6 && (rd_addr+rd_num) <= (addr_invt_para_f6+size_invt_para_f6))/* ��Ƶ��F6���� */
        {
            addr_offst = rd_addr - addr_invt_para_f6;
            buf = (U16 *)invt_paras_f6_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= INVT_ADDR_AD && (rd_addr+rd_num) <= (INVT_ADDR_AD+INVT_AD_SIZE))
        {
            addr_offst = rd_addr - INVT_ADDR_AD;
            buf = (U16 *)&invt_ad[id]+addr_offst;
        }
        else if (rd_addr >= INVT_ADDR_CUR_WAVE && (rd_addr+rd_num) <= (INVT_ADDR_CUR_WAVE+INVT_CUR_WAVE_SIZE))
        {
            addr_offst = rd_addr - INVT_ADDR_CUR_WAVE;
            buf = (U16 *)&invt_curr_wave[id]+addr_offst;
        }
        else if (rd_addr >= INVT_ADDR_EC_FAN_FG && (rd_addr+rd_num) <= (INVT_ADDR_EC_FAN_FG+INVT_EC_FAN_FG_SIZE))
        {
            addr_offst = rd_addr - INVT_ADDR_EC_FAN_FG;
            buf = (U16 *)&invt_ec_fan_fg[id]+addr_offst;
        }
        else
        {
            rd_succ = COM_ERR_DATA_ADDR;        /* ��ַ���磬���������� */
        }
        
        if (rd_succ == COM_SUCC)
        {
            rd_succ = COM_ReadRWwords(com, invt_get_device_addr(id), rd_addr, rd_num, buf, prio);
        }
    }
    else
    {
    	rd_succ = COM_ERR_MAC_ADDR;		/* �豸��ַ�� */
    }
	
	return rd_succ;
}

/****************************************************************
12.3-3 ���ӻ�����(com_FBUS_rd)
���ܣ�������ȡ�ӻ����ݡ�
���������ںţ��豸��ַ�������ݵ�ַ����������
���أ��ɹ�/ʧ��/����
������/��
*****************************************************************/
U08 com_FBUS_rd(U08 com, U08 obj_addr, U16 rd_addr, U08 rd_num,U08 prio)
{
	U08 rd_succ = COM_SUCC;
	U16 *buf = NULL;
	//U16 addr_offst = 0;
	//U08 cm_num = 0;
	
	if (obj_addr > 0 && obj_addr < val_pcb_num)
	{
    	if (rd_addr >= FBUS_VER_BGN && rd_addr < FBUS_VER_END)	/* �汾�� */
    	{
    		buf = (U16 *)&idu_mac_ver[obj_addr];
    	}
    	else if (rd_addr >= FBUS_ODU_VER_BGN && rd_addr < FBUS_ODU_VER_END)
    	{
            buf = (U16 *)&odu_mac_ver[obj_addr];
    	}
		else if (rd_addr >= FBUS_MAC_BGN && rd_addr <= FBUS_MAC_END)	/* ϵͳ��Ϣ */
		{
    		buf = (U16 *)&cell[obj_addr] + (rd_addr-FBUS_MAC_BGN)%CELL_SIZE;
		}
		else if (rd_addr >= ADDR_PARA_BGN && rd_addr <= ADDR_PARA_END)   /* ���� */
		{
            buf = (U16 *)&s_para_C[0] + (rd_addr - ADDR_PARA_BGN);
		}
    	else
    	{
    		rd_succ = COM_ERR_DATA_ADDR;		/* ��ַ���磬���������� */
    	}
    	
    	if (rd_succ == COM_SUCC)
    	{
    		rd_succ = COM_ReadROwords(com, obj_addr, rd_addr, rd_num, buf, prio);
    	}
    }
    else
    {
    	rd_succ = COM_ERR_MAC_ADDR;		/* �豸��ַ�� */
    }

	return rd_succ;
}

/****************************************************************
������12.3-2 ��������ӻ�(com_FBUS_wr)
���ܣ��������Ϳ�������ӻ���
���������ںţ��豸��ַ��д���ݵ�ַ��д������
���أ��ɹ�/ʧ��
��������ַ��Чʱ������ͨѶ��㡣
*****************************************************************/
U08 com_FBUS_wr(U08 com, U08 obj_addr, U16 wr_addr, U08 wr_num, U08 prio)
{
	U08 wr_succ = COM_SUCC;
	U16 *buf = NULL;
	//U16 addr_offst = 0;
	//U08 cm_num = 0;
	
	if (obj_addr > 0 && obj_addr < val_pcb_num)
	{
    	if (wr_addr >= FBUS_CMD_BGN && wr_addr < FBUS_CMD_END)			/* ���������� */
    	{
    		buf = (U16 *)&nor_cell_cmd[obj_addr][0];
    	}
    	else
    	{
    		wr_succ = COM_ERR_DATA_ADDR;		/* ��ַ���磬���������� */
    	}
    	
    	if (wr_succ == COM_SUCC)
    	{
    		wr_succ = COM_WriteWords(com, obj_addr, wr_addr, wr_num, buf,prio);
    	}
    }
    else
    {
    	wr_succ = COM_ERR_MAC_ADDR;		/* �豸��ַ�� */
    }
	
	return wr_succ;
}

/****************************************************************
������12.3-1�㲥���ݵ��ӻ�(com_FBUS_bro)
���ܣ������㲥���ӻ����������ݣ����������״̬�������ȣ���ͬ�����õ�ַ���֡�
���������ںţ�д���ݵ�ַ��д������
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_FBUS_bro(U08 com, U16 wr_addr, U08 wr_num, U08 prio)
{
	extern PARAMITEM  ParamPri[];
	U08 wr_succ = COM_SUCC;
	U08 grp, item;
	PARAMITEM *pp1;	
		
	U16 *buf = NULL;
	
	if (wr_addr >= FBUS_CMD_BGN && ((wr_addr + wr_num) <= FBUS_CMD_END))			/* ���������� */
	{
		buf = (U16 *)&bro_cell_cmd[0];
	}
	else if (wr_addr >= FBUS_SYS_STA_BGN && ((wr_addr + wr_num) <= FBUS_SYS_STA_END))/* ϵͳ״̬ */
	{
		buf = (U16 *)&pmac->cell_pv[0];
	}
	else if (wr_addr >= ADDR_PARA_BGN && ((wr_addr + wr_num) <= ADDR_PARA_END))	/* ���� */
	{
		grp = (wr_addr - ADDR_PARA_BGN) >> PARA_BIN_BIT_NUM;	/* �� */
		item = (wr_addr - ADDR_PARA_BGN) & (PARA_SIGLE_NUM-1);	/* �� */
		pp1 = (PARAMITEM *)&ParamPri[grp];
		buf = (U16 *)(&pp1->data_c[item]);
	}
	else if (wr_addr >= ADDR_PASS_BGN && ((wr_addr + wr_num) <= ADDR_PASS_END))	/* ���� */
	{
		grp = (wr_addr - ADDR_PASS_BGN) >> 2;	/* �� */
		buf = (U16 *)(&password[grp][0]);
	}
    else if (wr_addr >= ADDR_SPECIAL_BGN && ((wr_addr + wr_num) <= ADDR_SPECIAL_END))	/* �㲥�������� */
	{
		buf = (U16 *)&bro_special[0];
	}
	
	if (buf == NULL)	wr_succ = COM_ERR_DATA_ADDR;		/* ��ַ���磬���������� */
	
	if (wr_succ == COM_SUCC)
	{
		
		wr_succ = COM_WriteWords(com, ADDR_BRO_FBUS, wr_addr, wr_num, buf, prio);	/* �豸��ַ0����㲥 */
	}

	return wr_succ;
}


