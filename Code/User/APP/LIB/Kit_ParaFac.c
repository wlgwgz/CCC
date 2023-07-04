/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Kit_ParaFac.c
 �ļ�����: ����Ͳ����׼�����(��ϲ�����������PC���ʹ��)
 ������  : JH
 ��������: 2022.05.10
*******************************************************************************/
#define KIT_PARAFAC_GLOBAL
#include "Includes.h"

#ifdef USE_KIT_PARA_FAC
extern U16 get_dict_num(void);
extern const PARADICT para_dict[];
extern const SMALL *para_small[];
extern const U16 para_large[];
#ifdef SMARTHMI_H
extern BOOL SH_get_disp_info(U08 *dest_buf, U16 offset, U16 num);
extern BOOL SH_get_version(U08 *dest_buf, U16 offset, U16 num);
extern BOOL SH_get_para_item_info(U08 *dest_buf, U16 offset, U16 num);
extern BOOL SH_get_para_grp_info(U08 *dest_buf, U16 offset, U16 num);
extern BOOL SH_get_dict_string(U08 *dest_buf, U16 offset, U16 num);
extern BOOL SH_get_dict_attrib(U08 *dest_buf, U16 offset, U16 num);
extern U16 SH_attr_local2sh(U16 sign_local, const U16 *dic_str);
extern U08 SH_MB_rd_data(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);
#else
#include "SH_Lite/ExStr.c"
#include "SH_Lite/SmartHmi.c"
#endif

#ifdef USE_KIT_PARA_FAC
    #warning "δ�رն���Ͳ����׼�����!���رս�Ӱ������BMS����!����ʱһ��Ҫ�ر�!!!"
#endif

BOOL SH_get_version_filter(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 pcb_id = 0;         /* ��� */
    SH_CHK_VER ver;         /* ��ʾ��Ϣ */
    U08 idx = 0;            /* ���ݷ������� */
    U08 *pVerCode = NULL;   /* ָ�������ַ���Դ */
    U08 data_size = 0;      /* ���ݴ�С */
    U16 *src_buf = NULL;    /* ָ��Դ���ݵ�ָ�� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_VER) > (MODBUS_BUF_SIZE-4) || offset+num > 16) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (pcb_id=offset; pcb_id<offset+num; pcb_id++)
    {
        /* *************************��������************************* */

        /* ��������š��汾��Դ���汾�ų��� */
        ver.pcb_name = msg_idu_version;
        ver.pcb_id = MAKEWORD(pcb_id, NO_ID);
        pVerCode = (U08*)&idu_mac_ver[pcb_id][0];
        data_size = sizeof(APPFUNCODEVER) / sizeof(U08);

        /* ********************************************************** */

        /* ��ȡ�汾���ַ��� */
        for (i=0; i<32; i++)
        {
            if (i < data_size)
            {
                ver.fun_code[i] = pVerCode[i];
            }
            else
            {
                ver.fun_code[i] = 0;
            }
        }
        /* ������ֵ */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = 32 / 2;
        for (i=0; i<data_size; i++)
        {
            if (i>0 && HIBYTE(src_buf[i-1]) == 'd' && LOBYTE(src_buf[i]) == 'b' && HIBYTE(src_buf[i]) == 'g')	/* ����+�� */
            {
                if (idx>=2)
                {
                    dest_buf[idx-2] = 0;	/* 'd'����һ���ֵĸ��ֽ� */
                }
                
                dest_buf[idx++] = 0;
                dest_buf[idx++] = 0;
            }
            else if (i>0 && LOBYTE(src_buf[i-1]) == 'd' && HIBYTE(src_buf[i-1]) == 'b' && LOBYTE(src_buf[i]) == 'g')	/* ��+���� */
            {
                if (idx>=2)
                {
                    dest_buf[idx-2] = 0;	/* 'd'����һ���ֵĵ��ֽ� */
                    dest_buf[idx-1] = 0;	/* 'b'����һ���ֵĸ��ֽ� */
                }
            
                dest_buf[idx++] = HIBYTE(src_buf[i]);
                dest_buf[idx++] = 0;		/* 'g'������ֵĵ��ֽ� */
            }
            else
            {
                dest_buf[idx++] = HIBYTE(src_buf[i]);
                dest_buf[idx++] = LOBYTE(src_buf[i]);
            }            
        }
    }

    return TRUE;
}

BOOL com_get_default_para_item_info(U08 *dest_buf, U16 offset, U16 num)
{
    typedef struct tagFPK_PARA_ATTRIB        //����������
    {
        U16 name;                           //�����ַ�����
        U16 id;                             //���(��8λΪǰ׺�ţ���8λΪ��׺��ţ�Ϊ0xffʱ����ʾ���)
        I16 value;                          //����ֵ
        I16 min;                            //��Сֵ
        I16 max;                            //���ֵ
        U16 attrib;                         //����(0:�ַ����͡�1~8:��λ��9~11:С��λ��12:���ء�13:�����޸ġ�14:λ����)
    }FPK_PARA_ATTRIB;
    
    U16 i, j;
    U08 grp_idx = 0;        /* ��� */
    U08 item_idx = 0;       /* ��� */
    FPK_PARA_ATTRIB para_item;               /* ��ʾ��Ϣ */
    U08 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ����ָ�� */

    /* �������ݼ�� */
    if (num*sizeof(FPK_PARA_ATTRIB) > (MODBUS_BUF_SIZE-4) || num+offset > 50*40)/* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* *************************��������************************* */

    for (i=offset; i<offset+num; i++)
    {
        /* ������ż���� */
        grp_idx = (U08)(i / GROUP_SIZE);
        item_idx = (U08)(i % GROUP_SIZE);

        /* Ĭ��Ϊ���� */
        para_item.attrib = (1 << HIDE_BGN_BIT);

        if (grp_idx < PARA_GROUP_NUM
            && item_idx < ParamPri[grp_idx].items)
        {
            para_item.name = para_small[grp_idx][item_idx].msg;
            para_item.id = 0xFFFF;
            /* ��ֵ */
            para_item.value = ParamPri[grp_idx].array[item_idx].init;
            /* ��Сֵ */
            para_item.min = ParamPri[grp_idx].array[item_idx].min;
            /* ���ֵ */
            para_item.max = ParamPri[grp_idx].array[item_idx].max;
            /* ���� */
            para_item.attrib = SH_attr_local2sh(ParamPri[grp_idx].array[item_idx].sign, para_small[grp_idx][item_idx].pmsg);
        }
        else
        {
            return FALSE;
        }

        /* ********************************************************** */

        /* ��ֵ */
        src_buf = (U16*)&para_item;
        for (j=0; j<((sizeof(FPK_PARA_ATTRIB)+1) / sizeof(U16)); j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

U08 com_slv_BMS_aux_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
	U16 i;
	U16 m = 0;
	U16 addr_offst = 0;		/* ������ʼλ��(�����ݵ�ַ����) */
	U16 data_num = 0;
	U16 data_addr = 0;
	U16 *src_buf = NULL;
	U08 com_err = MB_NO_ERR;	/* ����״̬ */
	BOOL fg_rd_succ = FALSE;	/* �����ⲿ�ӳ���д��״̬��־ */
	
    U16 cell_offst = 0;     /* ��Ԫ��ʼλ��(�ӵڼ����ṹ�嵥Ԫ��ʼ��) */
    U16 cell_num = 0;       /* ��Ԫ����(��ȡ�����ṹ�嵥Ԫ) */

    static BOOL fg_read_default = TRUE;

	data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);	/* ���ݵ�ַ */
	data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* ���ݸ��� */

    switch (RxBuf[1])
    {
        case MB_RD_HOLDING_REGISTERS:
            
            if (data_addr >= ADDR_SH_DISP_BGN && data_addr + data_num <= ADDR_SH_DISP_END) /* ��ʾ������Ϣ */
            {
                addr_offst = data_addr - ADDR_SH_DISP_BGN;
                cell_offst = addr_offst;
                cell_num = data_num;
                fg_rd_succ = SH_get_disp_info(&TxBuf[3], cell_offst, cell_num);
            }
            else if (data_addr >= ADDR_SH_VER_BGN && data_addr + data_num <= ADDR_SH_VER_END)	/* �汾�� */
            {
                addr_offst = data_addr - ADDR_SH_VER_BGN;
                cell_offst = addr_offst / SH_VER_SIZE;
                cell_num = data_num / SH_VER_SIZE;
                fg_rd_succ = SH_get_version_filter(&TxBuf[3], cell_offst, cell_num);
                fg_read_default = TRUE;
            }
            else if (data_addr >= ADDR_SH_PARA_ATTRIB_BGN && data_addr + data_num <= ADDR_SH_PARA_ATTRIB_END)	/* ���������� */
            {
                addr_offst = data_addr - ADDR_SH_PARA_ATTRIB_BGN;
                cell_offst = addr_offst / SH_PARA_ATTRIB_SIZE;
                cell_num = data_num / SH_PARA_ATTRIB_SIZE;
                if (fg_read_default)
                {
                    fg_rd_succ = com_get_default_para_item_info(&TxBuf[3], cell_offst, cell_num);
                    fg_read_default = FALSE;
                }
                else
                {
                    fg_rd_succ = SH_get_para_item_info(&TxBuf[3], cell_offst, cell_num);
                }
            }
            else if(data_addr >= ADDR_SH_PARA_GRP_BGN && data_addr + data_num <= ADDR_SH_PARA_GRP_END)	/* ��������Ϣ */
            {
                addr_offst = data_addr - ADDR_SH_PARA_GRP_BGN;
                cell_offst = addr_offst / SH_PARA_GRP_SIZE;
                cell_num = data_num / SH_PARA_GRP_SIZE;
                fg_rd_succ = SH_get_para_grp_info(&TxBuf[3], cell_offst, cell_num);
            }
            else if(data_addr >= ADDR_SH_PARA_STRING_BGN && data_addr + data_num <= ADDR_SH_PARA_STRING_END)/* ���������ַ��� */
            {
                addr_offst = data_addr - ADDR_SH_PARA_STRING_BGN;
                cell_offst = addr_offst / SH_PARA_STRING_SIZE;
                cell_num = data_num / SH_PARA_STRING_SIZE;
                fg_rd_succ = SH_get_dict_string(&TxBuf[3], cell_offst, cell_num);//SH_get_para_string(&TxBuf[3], cell_offst, cell_num);
            }
            else if (data_addr >= ADDR_SH_STR_ATTR_BGN && data_addr + data_num <= ADDR_SH_STR_ATTR_END)	/* �����ַ������� */
            {
                addr_offst = data_addr - ADDR_SH_STR_ATTR_BGN;
                cell_offst = addr_offst / SH_STR_ATTR_PER_SIZE;
                cell_num = data_num / SH_STR_ATTR_PER_SIZE;
                fg_rd_succ = SH_get_dict_attrib(&TxBuf[3], cell_offst, cell_num);
            }
            break;

        case MB_RD_INPUT_REGISTERS:
        	if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)	/* ����ֵ */
        	{
        	    src_buf = (U16 *)&TxBuf[3];
        		fg_rd_succ = PARA_GetVal(&TxBuf[3], data_addr, data_num<<1); 				
        	}
            break;
    }


    if (src_buf != NULL || fg_rd_succ)		/* ������� */
    {
    	if (!fg_rd_succ)	/* û��ͨ���ⲿ�ӳ����ȡ���ڴ˴���ȡ(д���ͻ���) */
    	{
        	U08 *buf = &TxBuf[3];	/* ��Ч�����ڷ��ͻ����е���ʼ��ַ */
            m = 0;
            for (i=0; i<data_num; i++) 
        	{
        		buf[m++] = HIBYTE(src_buf[i]);
        		buf[m++] = LOBYTE(src_buf[i]);
        	}
        }
    	
    }
    else
	{
		com_err = MB_ILLEGAL_DATA_ADDR;
	}
    	
	TxBuf[2] = data_num << 1;	/* ���ݸ���Ϊ�����������ֽ��������2 */
	*pTxTotal = TxBuf[2] + 3;
	
	return com_err;
}

U08 com_slv_BMS_aux_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
	U16 i, m;
//	U16 addr_offst = 0;		/* ������ʼλ��(�����ݵ�ַ����) */
	U16 data_num = 0;
	U16 data_addr = 0;
	U16 *dest_buf = NULL;
	U08 *src_buf = NULL;
	U16 byte_count = 0;
	U08 com_err = MB_NO_ERR;
	BOOL fg_wr_succ = FALSE;	/* �����ⲿ�ӳ���д��״̬��־ */
	
	data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);	/* ���ݵ�ַ */
	
	if (RxBuf[1] == MB_WR_SINGLE_REGISTER)		/* ������0x06: д������  */
	{
		data_num = 1;
		src_buf = &RxBuf[4];
	}
	else							/* ������0x10: д����� */
	{
		data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* ���ݸ��� */
		src_buf = &RxBuf[7];
	}
	byte_count = data_num << 1;	/* ���ݸ���Ϊ�����������ֽ��������2 */
	if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)	/* ���� */
	{
		fg_wr_succ = PARA_Modify(src_buf, data_addr, byte_count);
	}  
    else
    {
        fg_wr_succ = 0;
    }
    
    if (dest_buf != NULL || fg_wr_succ)		/* ������� */
    {
    	if (!fg_wr_succ)	/* û��ͨ���ⲿ�ӳ���д�룬�ڴ˴�д�� */
    	{
            m = 0;
            for (i=0; i<byte_count; i+=2) 
        	{
        		dest_buf[m++] = MAKEWORD(src_buf[i], src_buf[i+1]);
        	}
        }
    }
    else
	{
		com_err = MB_ILLEGAL_DATA_ADDR;
	}
    	
	TxBuf[2] = RxBuf[2];
	TxBuf[3] = RxBuf[3];
	TxBuf[4] = RxBuf[4];
	TxBuf[5] = RxBuf[5];
	*pTxTotal = 6;
	
	return com_err;
}


U08 com_slv_BMS_aux(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = MB_NO_ERR;

    //work_bms_read();//Ԥ�ȴ���BMS������

    switch (RxBuf[1])
	{
        case MB_RD_HOLDING_REGISTERS :	/* 0x03 ���ɶ�д�� */
//	    	break;					/* 03 04��һ���ӳ����д����ӳ����ڲ����ֹ����� */
	    case MB_RD_INPUT_REGISTERS :	/* 0x04 ��ֻ���� */   
			exceptCode = com_slv_BMS_aux_rd_reg(RxBuf, TxBuf, pTxTotal);
			break;

	   case MB_WR_SINGLE_REGISTER :	/* 0x06 д������ */
			exceptCode = com_slv_BMS_aux_wr_reg(RxBuf, TxBuf, pTxTotal);
	    	break;

        case MB_USER_DEFINED_RD_BIGFILE :	/* 0x44 �Զ���: ��ȡ������ */
            exceptCode = SH_MB_rd_data(RxBuf, TxBuf, pTxTotal);
            break;

	    default :
	    	/* һЩ��ȷ�Ĺ����룬�����ݲ�֧�֣�Ҳ��Ϊ�Ƿ������� */
	    	exceptCode = MB_ILLEGAL_FUNCTION;				
	    	break;
	}

	return exceptCode;
}
#endif

