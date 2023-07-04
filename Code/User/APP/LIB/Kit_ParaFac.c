/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Kit_ParaFac.c
 文件描述: 多机型参数套件功能(配合参数管理助手PC软件使用)
 创建人  : JH
 创建日期: 2022.05.10
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
    #warning "未关闭多机型参数套件功能!不关闭将影响正常BMS功能!发布时一定要关闭!!!"
#endif

BOOL SH_get_version_filter(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 pcb_id = 0;         /* 板号 */
    SH_CHK_VER ver;         /* 显示信息 */
    U08 idx = 0;            /* 数据返回索引 */
    U08 *pVerCode = NULL;   /* 指向功能码字符串源 */
    U08 data_size = 0;      /* 数据大小 */
    U16 *src_buf = NULL;    /* 指向源数据的指针 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_VER) > (MODBUS_BUF_SIZE-4) || offset+num > 16) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (pcb_id=offset; pcb_id<offset+num; pcb_id++)
    {
        /* *************************更新数据************************* */

        /* 板名、板号、版本号源、版本号长度 */
        ver.pcb_name = msg_idu_version;
        ver.pcb_id = MAKEWORD(pcb_id, NO_ID);
        pVerCode = (U08*)&idu_mac_ver[pcb_id][0];
        data_size = sizeof(APPFUNCODEVER) / sizeof(U08);

        /* ********************************************************** */

        /* 获取版本号字符串 */
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
        /* 赋返回值 */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = 32 / 2;
        for (i=0; i<data_size; i++)
        {
            if (i>0 && HIBYTE(src_buf[i-1]) == 'd' && LOBYTE(src_buf[i]) == 'b' && HIBYTE(src_buf[i]) == 'g')	/* 半字+字 */
            {
                if (idx>=2)
                {
                    dest_buf[idx-2] = 0;	/* 'd'在上一个字的高字节 */
                }
                
                dest_buf[idx++] = 0;
                dest_buf[idx++] = 0;
            }
            else if (i>0 && LOBYTE(src_buf[i-1]) == 'd' && HIBYTE(src_buf[i-1]) == 'b' && LOBYTE(src_buf[i]) == 'g')	/* 字+半字 */
            {
                if (idx>=2)
                {
                    dest_buf[idx-2] = 0;	/* 'd'在上一个字的低字节 */
                    dest_buf[idx-1] = 0;	/* 'b'在上一个字的高字节 */
                }
            
                dest_buf[idx++] = HIBYTE(src_buf[i]);
                dest_buf[idx++] = 0;		/* 'g'在这个字的低字节 */
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
    typedef struct tagFPK_PARA_ATTRIB        //参数项属性
    {
        U16 name;                           //名称字符串号
        U16 id;                             //序号(高8位为前缀号，低8位为后缀序号，为0xff时不显示序号)
        I16 value;                          //参数值
        I16 min;                            //最小值
        I16 max;                            //最大值
        U16 attrib;                         //属性(0:字符串型、1~8:单位、9~11:小数位、12:隐藏、13:不可修改、14:位变量)
    }FPK_PARA_ATTRIB;
    
    U16 i, j;
    U08 grp_idx = 0;        /* 组号 */
    U08 item_idx = 0;       /* 项号 */
    FPK_PARA_ATTRIB para_item;               /* 显示信息 */
    U08 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 数据指针 */

    /* 输入数据检查 */
    if (num*sizeof(FPK_PARA_ATTRIB) > (MODBUS_BUF_SIZE-4) || num+offset > 50*40)/* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* *************************更新数据************************* */

    for (i=offset; i<offset+num; i++)
    {
        /* 计算组号及项号 */
        grp_idx = (U08)(i / GROUP_SIZE);
        item_idx = (U08)(i % GROUP_SIZE);

        /* 默认为隐藏 */
        para_item.attrib = (1 << HIDE_BGN_BIT);

        if (grp_idx < PARA_GROUP_NUM
            && item_idx < ParamPri[grp_idx].items)
        {
            para_item.name = para_small[grp_idx][item_idx].msg;
            para_item.id = 0xFFFF;
            /* 数值 */
            para_item.value = ParamPri[grp_idx].array[item_idx].init;
            /* 最小值 */
            para_item.min = ParamPri[grp_idx].array[item_idx].min;
            /* 最大值 */
            para_item.max = ParamPri[grp_idx].array[item_idx].max;
            /* 属性 */
            para_item.attrib = SH_attr_local2sh(ParamPri[grp_idx].array[item_idx].sign, para_small[grp_idx][item_idx].pmsg);
        }
        else
        {
            return FALSE;
        }

        /* ********************************************************** */

        /* 赋值 */
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
	U16 addr_offst = 0;		/* 数据起始位置(由数据地址决定) */
	U16 data_num = 0;
	U16 data_addr = 0;
	U16 *src_buf = NULL;
	U08 com_err = MB_NO_ERR;	/* 故障状态 */
	BOOL fg_rd_succ = FALSE;	/* 调用外部子程序写入状态标志 */
	
    U16 cell_offst = 0;     /* 单元起始位置(从第几个结构体单元开始读) */
    U16 cell_num = 0;       /* 单元个数(读取几个结构体单元) */

    static BOOL fg_read_default = TRUE;

	data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);	/* 数据地址 */
	data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* 数据个数 */

    switch (RxBuf[1])
    {
        case MB_RD_HOLDING_REGISTERS:
            
            if (data_addr >= ADDR_SH_DISP_BGN && data_addr + data_num <= ADDR_SH_DISP_END) /* 显示辅助信息 */
            {
                addr_offst = data_addr - ADDR_SH_DISP_BGN;
                cell_offst = addr_offst;
                cell_num = data_num;
                fg_rd_succ = SH_get_disp_info(&TxBuf[3], cell_offst, cell_num);
            }
            else if (data_addr >= ADDR_SH_VER_BGN && data_addr + data_num <= ADDR_SH_VER_END)	/* 版本号 */
            {
                addr_offst = data_addr - ADDR_SH_VER_BGN;
                cell_offst = addr_offst / SH_VER_SIZE;
                cell_num = data_num / SH_VER_SIZE;
                fg_rd_succ = SH_get_version_filter(&TxBuf[3], cell_offst, cell_num);
                fg_read_default = TRUE;
            }
            else if (data_addr >= ADDR_SH_PARA_ATTRIB_BGN && data_addr + data_num <= ADDR_SH_PARA_ATTRIB_END)	/* 参数项属性 */
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
            else if(data_addr >= ADDR_SH_PARA_GRP_BGN && data_addr + data_num <= ADDR_SH_PARA_GRP_END)	/* 参数组信息 */
            {
                addr_offst = data_addr - ADDR_SH_PARA_GRP_BGN;
                cell_offst = addr_offst / SH_PARA_GRP_SIZE;
                cell_num = data_num / SH_PARA_GRP_SIZE;
                fg_rd_succ = SH_get_para_grp_info(&TxBuf[3], cell_offst, cell_num);
            }
            else if(data_addr >= ADDR_SH_PARA_STRING_BGN && data_addr + data_num <= ADDR_SH_PARA_STRING_END)/* 参数名称字符串 */
            {
                addr_offst = data_addr - ADDR_SH_PARA_STRING_BGN;
                cell_offst = addr_offst / SH_PARA_STRING_SIZE;
                cell_num = data_num / SH_PARA_STRING_SIZE;
                fg_rd_succ = SH_get_dict_string(&TxBuf[3], cell_offst, cell_num);//SH_get_para_string(&TxBuf[3], cell_offst, cell_num);
            }
            else if (data_addr >= ADDR_SH_STR_ATTR_BGN && data_addr + data_num <= ADDR_SH_STR_ATTR_END)	/* 参数字符串属性 */
            {
                addr_offst = data_addr - ADDR_SH_STR_ATTR_BGN;
                cell_offst = addr_offst / SH_STR_ATTR_PER_SIZE;
                cell_num = data_num / SH_STR_ATTR_PER_SIZE;
                fg_rd_succ = SH_get_dict_attrib(&TxBuf[3], cell_offst, cell_num);
            }
            break;

        case MB_RD_INPUT_REGISTERS:
        	if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)	/* 参数值 */
        	{
        	    src_buf = (U16 *)&TxBuf[3];
        		fg_rd_succ = PARA_GetVal(&TxBuf[3], data_addr, data_num<<1); 				
        	}
            break;
    }


    if (src_buf != NULL || fg_rd_succ)		/* 防错设计 */
    {
    	if (!fg_rd_succ)	/* 没有通过外部子程序读取，在此处读取(写发送缓冲) */
    	{
        	U08 *buf = &TxBuf[3];	/* 有效数据在发送缓存中的起始地址 */
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
    	
	TxBuf[2] = data_num << 1;	/* 数据个数为字数，所以字节数需乘以2 */
	*pTxTotal = TxBuf[2] + 3;
	
	return com_err;
}

U08 com_slv_BMS_aux_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
	U16 i, m;
//	U16 addr_offst = 0;		/* 数据起始位置(由数据地址决定) */
	U16 data_num = 0;
	U16 data_addr = 0;
	U16 *dest_buf = NULL;
	U08 *src_buf = NULL;
	U16 byte_count = 0;
	U08 com_err = MB_NO_ERR;
	BOOL fg_wr_succ = FALSE;	/* 调用外部子程序写入状态标志 */
	
	data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);	/* 数据地址 */
	
	if (RxBuf[1] == MB_WR_SINGLE_REGISTER)		/* 功能码0x06: 写单个字  */
	{
		data_num = 1;
		src_buf = &RxBuf[4];
	}
	else							/* 功能码0x10: 写多个字 */
	{
		data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* 数据个数 */
		src_buf = &RxBuf[7];
	}
	byte_count = data_num << 1;	/* 数据个数为字数，所以字节数需乘以2 */
	if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)	/* 参数 */
	{
		fg_wr_succ = PARA_Modify(src_buf, data_addr, byte_count);
	}  
    else
    {
        fg_wr_succ = 0;
    }
    
    if (dest_buf != NULL || fg_wr_succ)		/* 防错设计 */
    {
    	if (!fg_wr_succ)	/* 没有通过外部子程序写入，在此处写入 */
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

    //work_bms_read();//预先处理BMS的数据

    switch (RxBuf[1])
	{
        case MB_RD_HOLDING_REGISTERS :	/* 0x03 读可读写字 */
//	    	break;					/* 03 04在一个子程序中处理，子程序内部区分功能码 */
	    case MB_RD_INPUT_REGISTERS :	/* 0x04 读只读字 */   
			exceptCode = com_slv_BMS_aux_rd_reg(RxBuf, TxBuf, pTxTotal);
			break;

	   case MB_WR_SINGLE_REGISTER :	/* 0x06 写单个字 */
			exceptCode = com_slv_BMS_aux_wr_reg(RxBuf, TxBuf, pTxTotal);
	    	break;

        case MB_USER_DEFINED_RD_BIGFILE :	/* 0x44 自定义: 读取大数据 */
            exceptCode = SH_MB_rd_data(RxBuf, TxBuf, pTxTotal);
            break;

	    default :
	    	/* 一些正确的功能码，由于暂不支持，也作为非法功能码 */
	    	exceptCode = MB_ILLEGAL_FUNCTION;				
	    	break;
	}

	return exceptCode;
}
#endif

