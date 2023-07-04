/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: IO.c
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#define  COM_M_GLOBAL
#include "includes.h"

/****************************************************************
函数：12.3-2 发送命令到子机(com_invt_wr)
功能：主机发送控制命令到子机。
参数：串口号；设备；写数据地址；写个数。
返回：成功/失败
其它：地址无效时不插入通讯结点。
*****************************************************************/
U08 com_invt_wr(U08 com, U08 EX_addr, U16 wr_addr, U08 wr_num, U08 prio)
{
	U08 wr_succ = COM_SUCC;
	U16 *buf = NULL;
	U16 addr_offst = 0;
    U16 addr_invt_para, addr_invt_para_fa,addr_invt_para_fb,addr_invt_para_f10,addr_invt_para_f9,addr_invt_cmd, addr_invt_para_f6;
    U08 size_invt_para, size_invt_para_fa,size_invt_para_fb,size_invt_para_f10,size_invt_para_f9,size_invt_cmd, size_invt_para_f6;
    INVT_ID_ENUM id = (INVT_ID_ENUM)EX_addr;

        if (id < INVT_ID_MAX)   // 变频器
        {
            addr_invt_para = invt_paras_get_addr();     // 变频器参数地址
            size_invt_para = invt_paras_get_size();     // 变频器参数大小
            addr_invt_cmd  = invt_cmd_get_addr();       // 变频器通讯指令地址
            size_invt_cmd  = invt_cmd_get_size();       // 变频器通讯指令大小

            addr_invt_para_fa = invt_paras_fa_get_addr();     // 变频器FA参数地址
            size_invt_para_fa = invt_paras_fa_get_size();     // 变频器FA参数大小

            addr_invt_para_fb = invt_paras_fb_get_addr();     // 变频器FB参数地址
            size_invt_para_fb = invt_paras_fb_get_size();     // 变频器FB参数大小

            addr_invt_para_f10 = invt_paras_f10_get_addr();     // 变频器F10参数地址
            size_invt_para_f10 = invt_paras_f10_get_size();     // 变频器F10参数大小

            addr_invt_para_f9 = invt_paras_f9_get_addr();     // 变频器F9参数地址
            size_invt_para_f9 = invt_paras_f9_get_size();     // 变频器F9参数大小

            addr_invt_para_f6 = invt_paras_f6_get_addr();     // 变频器F6参数地址
            size_invt_para_f6 = invt_paras_f6_get_size();     // 变频器F6参数大小
            
            if (wr_addr >= info_invt_para[id].addr              /* 变频参数界面 */
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
                wr_succ = COM_ERR_DATA_ADDR;		/* 地址超界，不发送数据 */
            }
        	
        	if (wr_succ == COM_SUCC)
        	{
        		wr_succ = COM_WriteWords(com, invt_get_device_addr(id), wr_addr, wr_num, buf,prio);
        	}
        }
        else
        {
        	wr_succ = COM_ERR_MAC_ADDR;		/* 设备地址错 */
        }
	
	return wr_succ;
}


/****************************************************************
12.3-3 读子机数据(com_invt_rd)
功能：主机读取子机数据。
参数：串口号；设备；读数据地址；读个数。
返回：成功/失败/错误
其它：/。
*****************************************************************/
U08 com_invt_rd(U08 com, U08 EX_addr, U16 rd_addr, U08 rd_num,U08 prio)
{
	U08 rd_succ = COM_SUCC;
	U16 *buf = NULL;
    U16 addr_offst = 0;

    U16 addr_invt_state, addr_invt_para, addr_invt_para_fa, addr_invt_para_fb, addr_invt_para_fp, addr_invt_para_f10, addr_invt_para_f9, addr_invt_para_f6;
    U08 size_invt_state, size_invt_para, size_invt_para_fa, size_invt_para_fb, size_invt_para_fp, size_invt_para_f10, size_invt_para_f9, size_invt_para_f6;
    INVT_ID_ENUM id = (INVT_ID_ENUM)EX_addr;
	
	if (id < INVT_ID_MAX)   // 变频器
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
        
        if (rd_addr >= info_invt_para[id].addr              /* 变频参数界面 */
        && (rd_addr+rd_num) <= (info_invt_para[id].addr+1)
        &&  info_invt_para[id].fg_rd
        )  
        {
            addr_offst = rd_addr - info_invt_para[id].addr;
            buf = (U16 *)&info_invt_para[id].val+addr_offst;
            info_invt_para[id].fg_rd = FALSE;
        }
        else if (rd_addr >= addr_invt_state && (rd_addr+rd_num) <= (addr_invt_state+size_invt_state))  /* 变频器状态 */
        {
            addr_offst = rd_addr - addr_invt_state;
            buf = (U16 *)invt_states_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para && (rd_addr+rd_num) <= (addr_invt_para+size_invt_para))  /* 变频器基础参数 */
        {
            addr_offst = rd_addr - addr_invt_para;
            buf = (U16 *)invt_paras_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_fa && (rd_addr+rd_num) <= (addr_invt_para_fa+size_invt_para_fa))  /* 变频器基础参数 */
        {
            addr_offst = rd_addr - addr_invt_para_fa;
            buf = (U16 *)invt_paras_fa_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_fb && (rd_addr+rd_num) <= (addr_invt_para_fb+size_invt_para_fb))  /* 变频器基础参数 */
        {
            addr_offst = rd_addr - addr_invt_para_fb;
            buf = (U16 *)invt_paras_fb_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_fp && (rd_addr+rd_num) <= (addr_invt_para_fp+size_invt_para_fp))  /* 变频器基础参数 */
        {
            addr_offst = rd_addr - addr_invt_para_fp;
            buf = (U16 *)invt_paras_fp_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_f10 && (rd_addr+rd_num) <= (addr_invt_para_f10+size_invt_para_f10))  /* 变频器基础参数 */
        {
            addr_offst = rd_addr - addr_invt_para_f10;
            buf = (U16 *)invt_paras_f10_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_f9 && (rd_addr+rd_num) <= (addr_invt_para_f9+size_invt_para_f9))  /* 变频器基础参数 */
        {
            addr_offst = rd_addr - addr_invt_para_f9;
            buf = (U16 *)invt_paras_f9_get_buf(id)+addr_offst;
        }
        else if (rd_addr >= addr_invt_para_f6 && (rd_addr+rd_num) <= (addr_invt_para_f6+size_invt_para_f6))/* 变频器F6参数 */
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
            rd_succ = COM_ERR_DATA_ADDR;        /* 地址超界，不发送数据 */
        }
        
        if (rd_succ == COM_SUCC)
        {
            rd_succ = COM_ReadRWwords(com, invt_get_device_addr(id), rd_addr, rd_num, buf, prio);
        }
    }
    else
    {
    	rd_succ = COM_ERR_MAC_ADDR;		/* 设备地址错 */
    }
	
	return rd_succ;
}

/****************************************************************
12.3-3 读子机数据(com_FBUS_rd)
功能：主机读取子机数据。
参数：串口号；设备地址；读数据地址；读个数。
返回：成功/失败/错误
其它：/。
*****************************************************************/
U08 com_FBUS_rd(U08 com, U08 obj_addr, U16 rd_addr, U08 rd_num,U08 prio)
{
	U08 rd_succ = COM_SUCC;
	U16 *buf = NULL;
	//U16 addr_offst = 0;
	//U08 cm_num = 0;
	
	if (obj_addr > 0 && obj_addr < val_pcb_num)
	{
    	if (rd_addr >= FBUS_VER_BGN && rd_addr < FBUS_VER_END)	/* 版本号 */
    	{
    		buf = (U16 *)&idu_mac_ver[obj_addr];
    	}
    	else if (rd_addr >= FBUS_ODU_VER_BGN && rd_addr < FBUS_ODU_VER_END)
    	{
            buf = (U16 *)&odu_mac_ver[obj_addr];
    	}
		else if (rd_addr >= FBUS_MAC_BGN && rd_addr <= FBUS_MAC_END)	/* 系统信息 */
		{
    		buf = (U16 *)&cell[obj_addr] + (rd_addr-FBUS_MAC_BGN)%CELL_SIZE;
		}
		else if (rd_addr >= ADDR_PARA_BGN && rd_addr <= ADDR_PARA_END)   /* 参数 */
		{
            buf = (U16 *)&s_para_C[0] + (rd_addr - ADDR_PARA_BGN);
		}
    	else
    	{
    		rd_succ = COM_ERR_DATA_ADDR;		/* 地址超界，不发送数据 */
    	}
    	
    	if (rd_succ == COM_SUCC)
    	{
    		rd_succ = COM_ReadROwords(com, obj_addr, rd_addr, rd_num, buf, prio);
    	}
    }
    else
    {
    	rd_succ = COM_ERR_MAC_ADDR;		/* 设备地址错 */
    }

	return rd_succ;
}

/****************************************************************
函数：12.3-2 发送命令到子机(com_FBUS_wr)
功能：主机发送控制命令到子机。
参数：串口号；设备地址；写数据地址；写个数。
返回：成功/失败
其它：地址无效时不插入通讯结点。
*****************************************************************/
U08 com_FBUS_wr(U08 com, U08 obj_addr, U16 wr_addr, U08 wr_num, U08 prio)
{
	U08 wr_succ = COM_SUCC;
	U16 *buf = NULL;
	//U16 addr_offst = 0;
	//U08 cm_num = 0;
	
	if (obj_addr > 0 && obj_addr < val_pcb_num)
	{
    	if (wr_addr >= FBUS_CMD_BGN && wr_addr < FBUS_CMD_END)			/* 机组内命令 */
    	{
    		buf = (U16 *)&nor_cell_cmd[obj_addr][0];
    	}
    	else
    	{
    		wr_succ = COM_ERR_DATA_ADDR;		/* 地址超界，不发送数据 */
    	}
    	
    	if (wr_succ == COM_SUCC)
    	{
    		wr_succ = COM_WriteWords(com, obj_addr, wr_addr, wr_num, buf,prio);
    	}
    }
    else
    {
    	wr_succ = COM_ERR_MAC_ADDR;		/* 设备地址错 */
    }
	
	return wr_succ;
}

/****************************************************************
函数：12.3-1广播数据到子机(com_FBUS_bro)
功能：主机广播到子机的所有数据，包括：命令、状态、参数等，不同类型用地址区分。
参数：串口号；写数据地址；写个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_FBUS_bro(U08 com, U16 wr_addr, U08 wr_num, U08 prio)
{
	extern PARAMITEM  ParamPri[];
	U08 wr_succ = COM_SUCC;
	U08 grp, item;
	PARAMITEM *pp1;	
		
	U16 *buf = NULL;
	
	if (wr_addr >= FBUS_CMD_BGN && ((wr_addr + wr_num) <= FBUS_CMD_END))			/* 机组内命令 */
	{
		buf = (U16 *)&bro_cell_cmd[0];
	}
	else if (wr_addr >= FBUS_SYS_STA_BGN && ((wr_addr + wr_num) <= FBUS_SYS_STA_END))/* 系统状态 */
	{
		buf = (U16 *)&pmac->cell_pv[0];
	}
	else if (wr_addr >= ADDR_PARA_BGN && ((wr_addr + wr_num) <= ADDR_PARA_END))	/* 参数 */
	{
		grp = (wr_addr - ADDR_PARA_BGN) >> PARA_BIN_BIT_NUM;	/* 组 */
		item = (wr_addr - ADDR_PARA_BGN) & (PARA_SIGLE_NUM-1);	/* 号 */
		pp1 = (PARAMITEM *)&ParamPri[grp];
		buf = (U16 *)(&pp1->data_c[item]);
	}
	else if (wr_addr >= ADDR_PASS_BGN && ((wr_addr + wr_num) <= ADDR_PASS_END))	/* 密码 */
	{
		grp = (wr_addr - ADDR_PASS_BGN) >> 2;	/* 组 */
		buf = (U16 *)(&password[grp][0]);
	}
    else if (wr_addr >= ADDR_SPECIAL_BGN && ((wr_addr + wr_num) <= ADDR_SPECIAL_END))	/* 广播特殊数据 */
	{
		buf = (U16 *)&bro_special[0];
	}
	
	if (buf == NULL)	wr_succ = COM_ERR_DATA_ADDR;		/* 地址超界，不发送数据 */
	
	if (wr_succ == COM_SUCC)
	{
		
		wr_succ = COM_WriteWords(com, ADDR_BRO_FBUS, wr_addr, wr_num, buf, prio);	/* 设备地址0代表广播 */
	}

	return wr_succ;
}


