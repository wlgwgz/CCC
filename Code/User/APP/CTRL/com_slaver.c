/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: IO.c
 文件描述:
 创建人  :
 创建日期:
*******************************************************************************/
//#define  COM_GLOBAL
#include "includes.h"

/*******************************************************************************
 函 数: SlaveDealUserDefined()wlg20150715
 功 能: 从机处理ModBus用户自定义功能码:程序下载
 参 数: pduRxBuf--PDU接收缓冲区;
        pduTxBuf--PDU发送缓冲区;
        pPduTxTotal--PDU需发送的字节总数;
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断与自定义的下载协议是否正确，若错误则返回异常码(上层再处理异常响应PDU)
        若正确，处理命令，并置响应PDU到发送缓冲区
        具体应用协议
1、询问应用程序是否能够进入BOOT
01 + 41 + 0000 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERACROSSRS485ENTER"
能够进入BOOT:01 + 41 + 0000 + 0010 + 00 + 01 + CRC
不能进入BOOT:01 + 41 + 0000 + 0010 + 00 + 00 + CRC
2、命令后板进入BOOT
01 + 41 + 0010 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERENTERBOOTFROMAPP"
如果能够进入BOOT:返回，直接进入BOOT
如果不能够进入BOOT:01 + 41 + 0010 + 0010 + 00 + 00 + CRC
*******************************************************************************/
U08 PDU_SlaveDealUserDef(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
{
    U16 subFunctionCode;    /* 子功能码 */
    static CONST I08 s_askMsg[] =      "PUNPPLDOWNLOADERACROSSRS485ENTER";
    static CONST I08 s_downLoadMsg[] = "PUNPPLDOWNLOADERENTERBOOTFROMAPP";
    //U16 *pData;               /* 数据指针 */

    subFunctionCode = MAKEWORD(pduRxBuf[2], pduRxBuf[3]);

    /* 个数判断 */
    if ((MAKEWORD(pduRxBuf[4], pduRxBuf[5]) != 16)
        || (pduRxBuf[6] != 32))
    {
        return MB_ILLEGAL_DATA_VALUE;
    }

    /* 子功能码的判断与处理 */
    switch (subFunctionCode)
    {
        case MB_0x41_SUB_FUNC_ALLOW_BOOT:
            if (STRNCMP(s_askMsg, (I08 *)&pduRxBuf[7], 32) == 0)
            {
                MEMCPY(&pduTxBuf[1], &pduRxBuf[1], 5);
                *pPduTxTotal = 8;

                if (USER_AllowEnterBoot())
                {
                    pduTxBuf[6] = 0;
                    pduTxBuf[7] = 1;
                }
                else
                {
                    pduTxBuf[6] = 0;
                    pduTxBuf[7] = 0;
                }
                return MB_NO_ERR;
            }
            break;
        case MB_0x41_SUB_FUNC_ACCEPT_BOOT:      // 询问是否接受升级
            if (STRNCMP(s_askMsg, (I08 *)&pduRxBuf[7], 32) == 0)
            {
                MEMCPY(&pduTxBuf[1], &pduRxBuf[1], 5);
                *pPduTxTotal = 8;

				switch (fg_accept_remote_upgrade)
				{
					case SH_REMOTE_REQ_WAIT:	/* 未确定是否升级 */
                        pduTxBuf[6] = 0;
		                pduTxBuf[7] = fg_accept_remote_upgrade;
						break;

					case SH_REMOTE_REQ_OK:	/* 确定要升级，需等待机组关闭 */
						if (USER_AllowEnterBoot())
						{
							pduTxBuf[6] = 0;
			                pduTxBuf[7] = fg_accept_remote_upgrade;
		                	fg_request_remote_upgrade = FALSE;
			                fg_accept_remote_upgrade = SH_REMOTE_REQ_WAIT;
						}
                        else
                        {
                            pduTxBuf[6] = 0;
		                    pduTxBuf[7] = SH_REMOTE_REQ_WAIT;
                        }
						break;

					case SH_REMOTE_REQ_CANCEL:	/* 取消升级 */
						pduTxBuf[6] = 0;
		                pduTxBuf[7] = fg_accept_remote_upgrade;
	                	fg_request_remote_upgrade = FALSE;
		                fg_accept_remote_upgrade = SH_REMOTE_REQ_WAIT;
						break;
				}

                return MB_NO_ERR;
            }
            break;

        case MB_0x41_SUB_FUNC_ENTER_BOOT:
            if (STRNCMP(s_downLoadMsg, (I08 *)&pduRxBuf[7], 32) == 0)
            {
                MEMCPY(&pduTxBuf[1], &pduRxBuf[1], 5);
                *pPduTxTotal = 8;

                if (USER_AllowEnterBoot())
                {
                    pduTxBuf[6] = 0;
                    pduTxBuf[7] = 1;
                    USER_EnterBoot();
                }
                else
                {
                    pduTxBuf[6] = 0;
                    pduTxBuf[7] = 0;
                }
                return MB_NO_ERR;
            }
            break;
        case MB_0x41_SUB_FUNC_REQUEST_BOOT:     // 执行请求升级
            if (STRNCMP(s_downLoadMsg, (I08 *)&pduRxBuf[7], 32) == 0)
            {
                MEMCPY(&pduTxBuf[1], &pduRxBuf[1], 5);
                *pPduTxTotal = 8;

                /* 控制板需传给显示屏一个标志 */
                fg_request_remote_upgrade = TRUE;
                if (USER_AllowEnterBoot())
                {
                    pduTxBuf[6] = 0;
                    pduTxBuf[7] = 1;
                }
                else
                {
                    pduTxBuf[6] = 0;
                    pduTxBuf[7] = 0;
                }
                return MB_NO_ERR;
            }
            break;

        default:
            break;
    }

    return MB_ILLEGAL_DATA_ADDR;
}


/****************************************************************
函数：com_slv_HMI_rd_input
功能：主机接收HMI发过来的读数据字请求。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_HMI_rd_input(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0,n = 0,l = 0;
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_U08_num = 0;
    U16 data_addr = 0;
    U08 *src_buf = NULL;
//  U08 mac_addr = 0;
//  U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
    data_U08_num = ((data_num%8) == 0) ? (data_num/8) : ((data_num/8)+1);


    if (data_addr >= ADDR_TEST_BGN )
    {
        addr_offst = (data_addr - ADDR_TEST_BGN)/8;
        src_buf = (U08 *)&ITbl[addr_offst];
    }

    n=(data_addr - ADDR_TEST_BGN)%8;
    m = 0;
    if (src_buf != NULL)        /* 防错设计 */
    {
        U08 *buf = &TxBuf[3];   /* 有效数据在发送缓存中的起始地址 */

        for (i=n; i<(data_num + n); i++)
        {
            buf[m/8] >>= 1;
            if(GETBIT(src_buf[i/8],i%8))
            {
                buf[m/8] |= 0x80;
            }
            m++;

        }
        l = 0;
        if((data_num % 8) != 0)
        {
            l = 8 - (data_num % 8);
            buf[m/8] >>= l;
        }
    }
    else
    {
        com_err = MB_ILLEGAL_DATA_ADDR;
    }

    TxBuf[2] = data_U08_num;    /* 数据个数 */
    *pTxTotal = TxBuf[2] + 3;
    return com_err;
}



/****************************************************************
函数：com_slv_HMI_rd_reg
功能：主机接收HMI发过来的读数据字请求。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_HMI_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0;
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *src_buf = NULL;
//  U08 mac_addr = 0;
    U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */
    BOOL fg_rd_succ = FALSE;    /* 调用外部子程序写入状态标志 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */

    if (RxBuf[1] == MB_RD_INPUT_REGISTERS)      /* 只读整型 */
    {
        if (data_addr >= ADDR_TEST_VERSION_BGN && (data_addr + data_num <= ADDR_TEST_VERSION_END))  /* 主板版本号 */
        {
            addr_offst = data_addr - ADDR_TEST_VERSION_BGN;
            EX_addr = addr_offst / ONE_VER_SIZE;
            addr_offst = addr_offst % ONE_VER_SIZE;
            src_buf = (U16 *)&idu_mac_ver[EX_addr] + addr_offst;
        }
        else if(data_addr >= ADDR_TEST_DI_BGN && (data_addr + data_num <= ADDR_TEST_DI_END))            // 测试开关量
        {
            addr_offst = data_addr - ADDR_TEST_DI_BGN;
            src_buf = (U16 *)&TestTbl_DI[addr_offst];
        }
    }
    else            /* 可读写整型 */
    {
        if(data_addr >= ADDR_TEST_STA_BGN && (data_addr + data_num <= ADDR_TEST_STA_END))                 // 测试状态
        {
            addr_offst = data_addr - ADDR_TEST_STA_BGN;
            src_buf = (U16 *)&TestTbl_STA[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_NTC_BGN && (data_addr + data_num <= ADDR_TEST_NTC_END))          // 测试温度
        {
            addr_offst = data_addr - ADDR_TEST_NTC_BGN;
            src_buf = (U16 *)&TestTbl_NTC[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_CUR_BGN && (data_addr + data_num <= ADDR_TEST_CUR_END))          // 测试电流
        {
            addr_offst = data_addr - ADDR_TEST_CUR_BGN;
            src_buf = (U16 *)&TestTbl_CUR[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_AITYPE_BGN && (data_addr + data_num <= ADDR_TEST_AITYPE_END))        // 测试AI类型
        {
            addr_offst = data_addr - ADDR_TEST_AITYPE_BGN;
            src_buf = (U16 *)&TestTbl_AI_TYPE[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_AI_BGN && (data_addr + data_num <= ADDR_TEST_AI_END))                // 测试AI
        {
            addr_offst = data_addr - ADDR_TEST_AI_BGN;
            src_buf = (U16 *)&TestTbl_AI_DATA[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_STEPNOW_BGN && (data_addr + data_num <= ADDR_TEST_STEPNOW_END))    // 测试步进电机当前值
        {
            addr_offst = data_addr - ADDR_TEST_STEPNOW_BGN;
            src_buf = (U16 *)&TestTbl_STEP_NOW[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_STEPSET_BGN && (data_addr + data_num <= ADDR_TEST_STEPSET_END))  // 测试步进电机设定值
        {
            addr_offst = data_addr - ADDR_TEST_STEPSET_BGN;
            src_buf = (U16 *)&TestTbl_STEP_SET[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_COMMU_BGN && (data_addr + data_num <= ADDR_TEST_COMMU_END))      // 测试通讯
        {
            addr_offst = data_addr - ADDR_TEST_COMMU_BGN;
            src_buf = (U16 *)&TestTbl_COMMU[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_PWM_BGN && (data_addr + data_num <= ADDR_TEST_PWM_END))      // PWM
        {
            addr_offst = data_addr - ADDR_TEST_PWM_BGN;
            src_buf = (U16 *)&TestTbl_PWM[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_INVT_BGN && (data_addr + data_num <= ADDR_TEST_INVT_END))        // INVT
        {
            addr_offst = data_addr - ADDR_TEST_INVT_BGN;
            src_buf = (U16 *)&test_invt_info + addr_offst;
        }
#ifdef COM_4G
        else if(data_addr >= ADDR_4G_TEST_BGN && data_addr + data_num <= ADDR_4G_TEST_END)  //读通讯信息数据
        {
             addr_offst = data_addr - ADDR_4G_TEST_BGN;
             src_buf = (U16 *)&test_4g + addr_offst;
        }
        else if(data_addr >= BMS_4G_RO_BGN && data_addr + data_num <= BMS_4G_RO_END)    //读通讯信息数据
        {
             addr_offst = data_addr - BMS_4G_RO_BGN;
             src_buf = (U16 *)&app_4g_ro + addr_offst;
        }
        else if (data_addr >= BMS_4G_RW_BGN && data_addr + data_num <= BMS_4G_RW_END)
        {
            U16 addr_offst = data_addr - BMS_4G_RW_BGN;
            src_buf = (U16 *)&app_4g_rw + addr_offst;
        }
#endif
#if 0
        else if (data_addr >= ADDR_PASS_BGN && data_addr + data_num <= ADDR_PASS_END)   /* 密码 */
        {
            password_superword();//wlg2015.06.12
            addr_offst = data_addr - ADDR_PASS_BGN;
            //src_buf = (U16 *)&password[0] + addr_offst;
        }
#endif
    }

    if (src_buf != NULL || fg_rd_succ)      /* 防错设计 */
    {
        if (!fg_rd_succ)    /* 没有通过外部子程序读取，在此处读取(写发送缓冲) */
        {
            U08 *buf = &TxBuf[3];   /* 有效数据在发送缓存中的起始地址 */
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

//  TxBuf[2] = (data_num << 1); /* 数据个数为字数，所以字节数需乘以2 */
        TxBuf[2] = (data_num * 2);  /* 数据个数为字数，所以字节数需乘以2 */

    *pTxTotal = TxBuf[2] + 3;

    return com_err;
}
#if 0
/****************************************************************
函数：com_slv_HMI_wr_coil
功能：主机接收HMI发过来数据字。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_HMI_wr_coil(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{

    U16 i, n, m;
    U16 data_num, data_addr;
    U16 *dest_buf = NULL;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */

    if (RxBuf[1] == MB_WR_SINGLE_COIL)      /* 功能码0x05: 写单个位  */
    {
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END) /* 用户命令 */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            n = m >> 4;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* 厂家命令 */
        {
            if (!controller_is_punp)  return  MB_ILLEGAL_DATA_ADDR;

            m = data_addr - ADDR_FAC_CMD_BGN;
            n = m >> 4;
            dest_buf = cmd_system;
        }
        else
        {
            return  MB_ILLEGAL_DATA_ADDR;
        }

        if (dest_buf != NULL)
        {
            if (RxBuf[4] == 0xff)       SETBIT(dest_buf[n], m&0x0f);
            else                        CLRBIT(dest_buf[n], m&0x0f);
        }
    }
    else                            /* 功能码0x0f: 写多个位 */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END)    /* 用户命令 */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* 厂家命令 */
        {
            if (!controller_is_punp)  return  MB_ILLEGAL_DATA_ADDR;

            m = data_addr - ADDR_FAC_CMD_BGN;
            dest_buf = cmd_system;
        }
        else
        {
            return  MB_ILLEGAL_DATA_ADDR;
        }

        if (dest_buf != NULL)
        {
            for (i=0; i<data_num; i++, m++)
            {
                n = m >> 4;

                if (GETBIT(RxBuf[7+i/8], i%8))
                {
                    SETBIT(dest_buf[n], m&0x0f);
                }
                else
                {
                    CLRBIT(dest_buf[n], m&0x0f);
                }
            }
        }
    }

    TxBuf[2] = RxBuf[2];
    TxBuf[3] = RxBuf[3];
    TxBuf[4] = RxBuf[4];
    TxBuf[5] = RxBuf[5];
    *pTxTotal = 6;

    return MB_NO_ERR;
}
#endif
/****************************************************************
函数：com_slv_HMI_wr_reg
功能：主机接收HMI发过来的数据字。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_HMI_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, m;
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *dest_buf = NULL;
    U08 *src_buf = NULL;
    U16 byte_count = 0;
    U08 com_err = MB_NO_ERR;
    BOOL fg_wr_succ = FALSE;    /* 调用外部子程序写入状态标志 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */

    if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* 功能码0x06: 写单个字  */
    {
        data_num = 1;
        src_buf = &RxBuf[4];
    }
    else                            /* 功能码0x10: 写多个字 */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
        src_buf = &RxBuf[7];
    }
    byte_count = data_num << 1; /* 数据个数为字数，所以字节数需乘以2 */
    if(data_addr >= ADDR_TEST_CMD_BGN && (data_addr + data_num <= ADDR_TEST_CMD_END))                   // 测试命令
    {
        addr_offst = data_addr - ADDR_TEST_CMD_BGN;
        dest_buf = (U16 *)&TestTbl_CMD[addr_offst];
    }
    else if(data_addr >= ADDR_TEST_AITYPE_BGN && (data_addr + data_num <= ADDR_TEST_AITYPE_END))        // 测试AI类型
    {
        addr_offst = data_addr - ADDR_TEST_AITYPE_BGN;
        dest_buf = (U16 *)&TestTbl_AI_TYPE[addr_offst];
    }
    else if(data_addr >= ADDR_TEST_STEPSET_BGN && (data_addr + data_num <= ADDR_TEST_STEPSET_END))  // 测试步进电机设定值
    {
        addr_offst = data_addr - ADDR_TEST_STEPSET_BGN;
        dest_buf = (U16 *)&TestTbl_STEP_SET[addr_offst];
    }
    else if(data_addr >= ADDR_TEST_PWM_BGN && (data_addr + data_num <= ADDR_TEST_PWM_END))      // PWM
    {
        addr_offst = data_addr - ADDR_TEST_PWM_BGN;
        dest_buf = (U16 *)&TestTbl_PWM[addr_offst];
    }
    else if(data_addr >= ADDR_TEST_INVT_BGN && (data_addr + data_num <= ADDR_TEST_INVT_END))        // INVT
    {
        addr_offst = data_addr - ADDR_TEST_INVT_BGN;
        dest_buf = (U16 *)&test_invt_info + addr_offst;
    }
#ifdef COM_4G
    else if (data_addr >= BMS_4G_RW_BGN && data_addr + data_num <= BMS_4G_RW_END)
    {
        U16 addr_offst = data_addr - BMS_4G_RW_BGN;
        dest_buf = (U16 *)&app_4g_rw + addr_offst;
    }
#endif

    if (dest_buf != NULL || fg_wr_succ)     /* 防错设计 */
    {
        if (!fg_wr_succ)    /* 没有通过外部子程序写入，在此处写入 */
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


/* 主板 HMI从机应答（8寸屏） */
/****************************************************************
12.3-50 HMI通讯应答(com_slv_HMI)
功能: 主控板应答HMI的读/写请求
参数:   RxBuf--接收缓冲区;
        TxBuf--发送缓冲区;
        pTxTotal--需发送的字节总数;
返回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
其它: /
*****************************************************************/
U08 com_slv_HMI(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = MB_NO_ERR;

    switch (RxBuf[1])
    {
        case MB_DIAGNOSTICS :           /* 0x08 回路诊断 */
//          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_COILS :              /* 0x01 读可读写位 */

            break;

        case MB_RD_DISCRETE_INPUTS :    /* 0x02 读只读位 */
            exceptCode = com_slv_HMI_rd_input(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_HOLDING_REGISTERS :  /* 0x03 读可读写字 */

//          break;          /* 03 04在一个子程序中处理，子程序内部区分功能码 */
        case MB_RD_INPUT_REGISTERS :    /* 0x04 读只读字 */
            exceptCode = com_slv_HMI_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_COIL :        /* 0x05 写单个位 */
            exceptCode = MB_NO_ERR;
//          exceptCode = com_slv_HMI_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_REGISTER :    /* 0x06 写单个字 */
            exceptCode = com_slv_HMI_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_COILS :     /* 0x0f 写多个位 */
            exceptCode = MB_NO_ERR;
//          exceptCode = com_slv_HMI_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_REGISTERS : /* 0x10 写多个字 */
            exceptCode = com_slv_HMI_wr_reg(RxBuf, TxBuf, pTxTotal);        /*  */
            break;

        case MB_USER_DEFINED_DOWNLOAD : /* 0x41 自定义：程序下载 */
            exceptCode = PDU_SlaveDealUserDef(RxBuf, TxBuf, pTxTotal);
            break;

        default :
            /* 一些正确的功能码，由于暂不支持，也作为非法功能码 */
            exceptCode = MB_ILLEGAL_FUNCTION;
            break;
    }

    return exceptCode;
}


/*==========================================
    功能：
    num : 字节数
/==========================================*/

U08 get_information(U08 *buf, U16 addr, U16 num)
{
    U16  m, k;
    U16 *pData;

    if (addr<ADDR_UNIT_MODE_BGN || addr>=ADDR_UNIT_MODE_END)    return 0;

    m = addr - ADDR_UNIT_MODE_BGN;

    for (k = 0; k < num; k += 2)
    {
        #define INFO1_NUM 1                 /* 1 */
        #define INFO2_NUM MAX_PCB           /* 8 */
        #define INFO3_NUM MAX_FREQINFO      /* 16 */
        #define INFO4_NUM MAX_FREQINFO      /* 16 */
        #define INFO5_NUM MAX_FREQINFO      /* 16 */

        #define INFO1_BGN (0)                               /* 0 */
        #define INFO2_BGN (INFO1_BGN+INFO1_NUM)             /* 1 */
        #define INFO3_BGN (INFO2_BGN+INFO2_NUM)             /* 9 */
        #define INFO4_BGN (INFO3_BGN+INFO3_NUM)             /* 25 */
        #define INFO5_BGN (INFO4_BGN+INFO4_NUM)             /* 41 */
        #define INFO6_BGN (INFO5_BGN+INFO5_NUM)             /* 57 */

        #define INFO_END  (INFO6_BGN)

             if(m>=INFO1_BGN && m<INFO2_BGN)    pData = (U16 *)&cell[MAX_PCB].cell_runtime[m-INFO1_BGN+SYS_RUNTIME];
        else if(m>=INFO2_BGN && m<INFO3_BGN)    pData = (U16 *)&cell[m-INFO2_BGN].cell_runtime[0];
        else if(m>=INFO3_BGN && m<INFO4_BGN)    pData = (U16 *)&CELL_INVT_INFO(pcb_addr, INVT_ID_1, m-INFO3_BGN);
        else if(m>=INFO4_BGN && m<INFO5_BGN)    pData = (U16 *)&CELL_INVT_INFO(pcb_addr, INVT_FAN1, m-INFO4_BGN);
        else if(m>=INFO5_BGN && m<INFO6_BGN)    pData = (U16 *)&CELL_INVT_INFO(pcb_addr, INVT_FAN2, m-INFO5_BGN);

        if (m>=INFO1_BGN && m<INFO_END)
        {
            buf[k  ] = HIGH_BYTE(pData[0]);
            buf[k+1] = LOW_BYTE(pData[0]);
        }
        else    // 防止MAX_PV改小后但此处未改而读到意外值
        {
            buf[k  ] = 0;
            buf[k+1] = 0;
        }
        m++;
    }
    return 1;
}

extern BOOL IIC_ReadBytes(U08 *pData, U16 rdaddr, U16 num, U08 chip);



/****************************************************************
函数：com_slv_BMS_rd_input
功能：主机接收BMS发过来的读数据字请求。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_BMS_rd_input(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0,n = 0,l = 0;
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_U08_num = 0;
    U16 data_addr = 0;
    U08 *src_buf = NULL;
    U08 mac_addr = 0;
    //U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */
    BOOL fg_rd_succ = FALSE;    /* 调用外部子程序写入状态标志 */
    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
    data_U08_num = ((data_num%8) == 0) ? (data_num/8) : ((data_num/8)+1);

    switch (RxBuf[1])
    {
        case MB_RD_COILS :              /* 0x01 读可读写位 */

            /* 用户命令,读取为0 */
            if (data_addr >= ADDR_USER_CMD_BGN && data_addr+data_num <= ADDR_USER_CMD_END)
            {
                U08 *buf = &TxBuf[3];

                for (i=0; i<data_U08_num; i++)
                {
                    buf[i] = 0;
                }

                fg_rd_succ = TRUE;
            }

            break;

        case MB_RD_DISCRETE_INPUTS :    /* 0x02 读只读位 */
            if (data_addr >= ADDR_SYS_BIT_BGN && data_addr+data_num <= ADDR_SYS_BIT_END)   /* 系统位信息 */
            {
                addr_offst = (data_addr - ADDR_SYS_BIT_BGN)/8;
                src_buf = (U08 *)&cell[MAX_PCB].cell_relay + addr_offst;
                n=(data_addr - ADDR_SYS_BIT_BGN)%8;
            }
            else if (data_addr >= ADDR_UNIT_BIT_BGN && data_addr+data_num <= ADDR_UNIT_BIT_END) /* 模块位信息 */
            {
        		addr_offst = data_addr - ADDR_UNIT_BIT_BGN;
        		mac_addr = addr_offst / UNIT_STEP_NUM;
        		addr_offst = (addr_offst % UNIT_STEP_NUM)/8;
                /* 只可以访问实际长度 */
                if(data_addr+data_num <= mac_addr*UNIT_STEP_NUM + ADDR_UNIT_BIT_BGN + UNIT_BITS) 
                {
            		src_buf = (U08 *)&cell[mac_addr].cell_relay + addr_offst;
                    n=(data_addr - ADDR_UNIT_BIT_BGN)%8;            
                }
        	}
            break;
    }

    m = 0;

    if (fg_rd_succ)
    {
        ; // nothing 已写入,啥都不干
    }
    else if (src_buf != NULL)       /* 防错设计 */
    {
        U08 *buf = &TxBuf[3];   /* 有效数据在发送缓存中的起始地址 */

        for (i=n; i<(data_num + n); i++)
        {
            buf[m/8] >>= 1;

            if (GETBIT(src_buf[i/8],i%8))
            {
                buf[m/8] |= 0x80;
            }

            m++;
        }

        l = 0;

        if ((data_num % 8) != 0)
        {
            l = 8 - (data_num % 8);
            buf[m/8] >>= l;
        }
    }
    else
    {
        com_err = MB_ILLEGAL_DATA_ADDR;
    }

    TxBuf[2] = data_U08_num;    /* 数据个数 */
    *pTxTotal = TxBuf[2] + 3;
    return com_err;
}

/****************************************************************
函数：com_slv_BMS_rd_reg
功能：主机接收BMS发过来的读数据字请求。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_BMS_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0;
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *src_buf = NULL;
    U08 mac_addr = 0;
    U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */
    BOOL fg_rd_succ = FALSE;    /* 调用外部子程序写入状态标志 */
    U16 cell_offst = 0;     /* 单元起始位置(从第几个结构体单元开始读) */
    U16 cell_num = 0;       /* 单元个数(读取几个结构体单元) */

    static U16* eeprom_addr = (U16*)0;
    static U32* flash_addr = (U32*)0;

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */

    if (data_addr >= ADDR_DEBUG_BGN && data_addr + data_num <= ADDR_DEBUG_END) //读取调试信息
    {
        addr_offst = data_addr - ADDR_DEBUG_BGN;
        src_buf = (U16 *)&gdebugIO.debugstart + addr_offst;
    }
    else if (data_addr >= ADDR_VER_BS_BGN && (data_addr + data_num <= ADDR_VER_BS_END))  /* 主板版本号 */
    {
        addr_offst = data_addr - ADDR_VER_BS_BGN;
        EX_addr = addr_offst / VER_BS_SIZE;
        addr_offst = addr_offst % VER_BS_SIZE;
        src_buf = (U16 *)&idu_mac_ver[EX_addr] + addr_offst;
    }
    else if (data_addr >= ADDR_VER2_BGN && (data_addr + data_num <= ADDR_VER2_END))  /* 主板版本号 */
    {
        addr_offst = data_addr - ADDR_VER2_BGN;
        MEMCPY(&TxBuf[3], &idu_mac_ver[0][0]+addr_offst, data_num<<1);
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= ADDR_CTRL_BGN && data_addr + data_num <= ADDR_CTRL_END)   /*机组信息 公共信息 */
    {
        addr_offst = data_addr - ADDR_CTRL_BGN;
        src_buf = (U16 *)&ctrl_info[0] + addr_offst;
    }
    else if (data_addr >= ADDR_ASSI_BGN && data_addr + data_num <= ADDR_ASSI_END)   /* 辅助信息 */
    {
        addr_offst = data_addr - ADDR_ASSI_BGN;
        src_buf = (U16 *)&assist_info[0] + addr_offst;
    }
    else if (data_addr >= ADDR_TEST_RUN_BGN && data_addr + data_num <= ADDR_TEST_RUN_END)   /* fg_test_run */
    {
        addr_offst = data_addr - ADDR_TEST_RUN_BGN;
        src_buf = (U16 *)&fg_test_run[0] + addr_offst;
    }
    else if (data_addr >= ADDR_BMSHMI_BGN && data_addr + data_num <= ADDR_BMSHMI_END)   /* BMSHMI */
    {
        addr_offst = data_addr - ADDR_BMSHMI_BGN;
        src_buf = (U16 *)&bmsHMI.info_bit[0] + addr_offst;
    }
    else if (data_addr >= ADDR_COMPNUM_BGN && data_addr + data_num <= ADDR_COMPNUM_END)/* 压机个数 */
    {
        addr_offst = data_addr - ADDR_COMPNUM_BGN;
        src_buf = (U16 *)&pcb_comp_num[0] + addr_offst;
    }
    else if (data_addr >= ADDR_UNIT_MODE_BGN && data_addr + data_num <= ADDR_UNIT_MODE_END) /* 系统模块综合信息 */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = get_information(&TxBuf[3], data_addr, data_num<<1);
    }
    else if (data_addr >= ADDR_UNIT_BGN && data_addr + data_num <= ADDR_UNIT_END)   /* 模块信息 */
    {
        addr_offst = data_addr - ADDR_UNIT_BGN;
        mac_addr = addr_offst >> UNIT_BIN_BIT_NUM;
        addr_offst = addr_offst & (UNIT_MODE_SINGLE_NUM - 1);
        src_buf = (U16 *)&cell[mac_addr] + addr_offst;
    }
    else if (data_addr >= ADDR_SYS_BGN && data_addr + data_num <= ADDR_SYS_END) /* 系统信息 */
    {
        addr_offst = data_addr - ADDR_SYS_BGN;
        src_buf = (U16 *)&cell[MAX_PCB] + addr_offst;
    }
    else if (data_addr >= ADDR_VER_ODU_BGN && data_addr + data_num <= ADDR_VER_ODU_END) /* ODU */
    {
        addr_offst = data_addr - ADDR_VER_ODU_BGN;
        MEMCPY(&TxBuf[3], &odu_mac_ver[0][0]+addr_offst, data_num<<1);
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= ADDR_VER_INVT_BGN && data_addr + data_num <= ADDR_VER_INVT_END) /* INVT */
    {
        U08 VerCode[MAX_VER_BYTE] = {0};        /* 用于将变频器版本号转换为U08数组 */
        /* 将变频器版本号转换为U08数组 */
        dm603_get_vfd_ver(VerCode);
        
        addr_offst = data_addr - ADDR_VER_INVT_BGN;
        MEMCPY(&TxBuf[3], &VerCode[0]+addr_offst, data_num<<1);
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= BMS_CLOCK_BGN && data_addr + data_num <= BMS_CLOCK_END)	/* 时钟 */
    {
        /* 暂不支持读时钟 */
    }
    else if (data_addr >= BMS_ERR_HIS_BGN && data_addr + data_num <= BMS_ERR_HIS_END)/* 故障 历史 */
    {
        addr_offst = data_addr - BMS_ERR_HIS_BGN;
        cell_offst = addr_offst / BMS_ERR_HIS_SIZE;
        cell_num = data_num / BMS_ERR_HIS_SIZE;
        fg_rd_succ = SH_get_his_error(&TxBuf[3], cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)   /* 参数值 */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = PARA_GetVal(&TxBuf[3], data_addr, data_num<<1);
    }
    else if (data_addr >= ADDR_PARA_ITEM_BGN && data_addr + data_num <= ADDR_PARA_ITEM_END) /* 参数组 个数 */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = PARA_GetInfo(&TxBuf[3], data_addr, data_num<<1);
    }
    else if (data_addr >= ADDR_PARA_INFO_BGN && data_addr + data_num <= ADDR_PARA_INFO_END) /* 参数组 属性 */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = PARA_GetAttrib(&TxBuf[3], data_addr, data_num<<1);
    }
#ifdef USE_BMS_DEBUG            
    else if(data_addr >= ADDR_BMS_DEBUG_BGN && data_addr + data_num <= ADDR_BMS_DEBUG_END) //调试数据
    {
//        com_slv_BMS_debug_read();
        addr_offst = data_addr - ADDR_BMS_DEBUG_BGN;
        src_buf = (U16 *)&bms_debug + addr_offst;
    }
#endif      
#ifdef USE_COMMU_TEST_READ
    else if(data_addr >= ADDR_TEST_COMMU_BGN && (data_addr + data_num <= ADDR_TEST_COMMU_END))      // 测试通讯
    {
        addr_offst = data_addr - ADDR_TEST_COMMU_BGN;
        src_buf = (U16 *)&TestTbl_COMMU[addr_offst];
    }
#ifdef COM_4G
    else if(data_addr >= ADDR_4G_TEST_BGN && data_addr + data_num <= ADDR_4G_TEST_END)  //读通讯信息数据
    {
         addr_offst = data_addr - ADDR_4G_TEST_BGN;
         src_buf = (U16 *)&test_4g + addr_offst;
    }
#endif    
#endif  
#ifdef COM_4G 
    else if(data_addr >= BMS_4G_RO_BGN && data_addr + data_num <= BMS_4G_RO_END)    //读通讯信息数据
    {
         addr_offst = data_addr - BMS_4G_RO_BGN;
         src_buf = (U16 *)&app_4g_ro + addr_offst;
    }
    else if (data_addr >= BMS_4G_RW_BGN && data_addr + data_num <= BMS_4G_RW_END)
    {
        U16 addr_offst = data_addr - BMS_4G_RW_BGN;
        src_buf = (U16 *)&app_4g_rw + addr_offst;
    }
#endif
    else if (data_addr >= BMS_TEST_FAC_BGN && data_addr + data_num <= BMS_TEST_FAC_END)
    {
        U16 addr_offst = data_addr - BMS_TEST_FAC_BGN;
        src_buf = (U16 *)&test_fac + addr_offst;
    }
#ifdef USE_SPECIAL_READ
    else if (data_addr >= 60672 && data_addr + data_num <= 60673)   /* 特殊功能0xED00: 地址复位，从头读EEPROM */
    {
        src_buf = (U16 *)&TxBuf[3];
        eeprom_addr = (U16*)0;
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= 60928 && data_addr + data_num <= 60993)   /* 特殊功能0xEE00: 读32K EEPROM */
    {
        src_buf = (U16 *)&TxBuf[3];
        if (eeprom_addr < (U16*)0x8000)
        {
            TxBuf[3] = 0;
            TxBuf[4] = 0;
            TxBuf[5] = HIBYTE((U32)eeprom_addr);
            TxBuf[6] = LOBYTE((U32)eeprom_addr);
            DBG_EEP_ReadChip(&TxBuf[7], (U16)(U32)eeprom_addr, 128, EEP_CHIP_NOR);
            eeprom_addr += 64;              /* 存在2字节对齐，此处为128/2 */
            fg_rd_succ = TRUE;
        }
    }
    else if (data_addr >= 65024 && data_addr + data_num <= 65025)   /* 特殊功能0xFE00: 地址清零，从头读FLASH */
    {
        src_buf = (U16 *)&TxBuf[3];
        flash_addr = 0;
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= 65280 && data_addr + data_num <= 65346)   /* 特殊功能0xFF00: 读512K FLASH */
    {
        src_buf = (U16 *)&TxBuf[3];
        if (flash_addr < (U32*)0x00080000)
        {
            TxBuf[3] = DWORDHI((U32)flash_addr);
            TxBuf[4] = DWORDMH((U32)flash_addr);
            TxBuf[5] = DWORDML((U32)flash_addr);
            TxBuf[6] = DWORDLO((U32)flash_addr);
            MEMCPY(&TxBuf[7], (U08*)flash_addr, 128);
            flash_addr += 32;               /* 存在4字节对齐，此处为128/4 */
            fg_rd_succ = TRUE;
        }
    }
#endif

    if (src_buf != NULL || fg_rd_succ)      /* 防错设计 */
    {
        if (!fg_rd_succ)    /* 没有通过外部子程序读取，在此处读取(写发送缓冲) */
        {
            U08 *buf = &TxBuf[3];   /* 有效数据在发送缓存中的起始地址 */
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

    TxBuf[2] = data_num << 1;   /* 数据个数为字数，所以字节数需乘以2 */
    *pTxTotal = TxBuf[2] + 3;

    return com_err;
}

/****************************************************************
函数：com_slv_BMS_wr_coil
功能：主机接收BMS发过来数据字。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_BMS_wr_coil(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, n, m;
    U16 data_num, data_addr;
    U16 *dest_buf = NULL;
//  U08 com_err = MB_NO_ERR;    /* 故障状态 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */

    if (RxBuf[1] == MB_WR_SINGLE_COIL)      /* 功能码0x05: 写单个位  */
    {
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END) /* 用户命令 */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            n = m >> 4;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* 厂家命令 */
        {
            if (!controller_is_punp)  return  MB_ILLEGAL_DATA_ADDR;

            m = data_addr - ADDR_FAC_CMD_BGN;
            n = m >> 4;
            dest_buf = cmd_system;
        }
        else if (data_addr >= ADDR_MUX_CMD_BGN && data_addr < ADDR_MUX_CMD_END) /* cmd_mux */
        {
            m = data_addr - ADDR_MUX_CMD_BGN;
            n = m >> 4;
            dest_buf = cmd_mux;
        }
        else if (data_addr >= ADDR_TEST_DO_BGN && data_addr < ADDR_TEST_DO_END) /* 手动调试 */
        {
            m = data_addr - ADDR_TEST_DO_BGN;
            n = m >> 4;
            dest_buf = test_data.wr.DO_sta;
        }
        else
        {
            return  MB_ILLEGAL_DATA_ADDR;
        }

        if (dest_buf != NULL)
        {
            if (RxBuf[4] == 0xff)       SETBIT(dest_buf[n], m&0x0f);
            else                        CLRBIT(dest_buf[n], m&0x0f);
        }
    }
    else                            /* 功能码0x0f: 写多个位 */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END)    /* 用户命令 */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* 厂家命令 */
        {
            if (!controller_is_punp)  return  MB_ILLEGAL_DATA_ADDR;

            m = data_addr - ADDR_FAC_CMD_BGN;
            dest_buf = cmd_system;
        }
        else if (data_addr >= ADDR_MUX_CMD_BGN && data_addr < ADDR_MUX_CMD_END) /* cmd_mux */
        {
            m = data_addr - ADDR_MUX_CMD_BGN;
            dest_buf = cmd_mux;
        }
        else if (data_addr >= ADDR_TEST_DO_BGN && data_addr < ADDR_TEST_DO_END) /* 手动调试 */
        {
            m = data_addr - ADDR_TEST_DO_BGN;
            dest_buf = test_data.wr.DO_sta;
        }
        else
        {
            return  MB_ILLEGAL_DATA_ADDR;
        }

        if (dest_buf != NULL)
        {
            for (i=0; i<data_num; i++, m++)
            {
                n = m >> 4;

                if (GETBIT(RxBuf[7+i/8], i%8))
                {
                    SETBIT(dest_buf[n], m&0x0f);
                }
                else
                {
                    CLRBIT(dest_buf[n], m&0x0f);
                }
            }
        }
    }

    TxBuf[2] = RxBuf[2];
    TxBuf[3] = RxBuf[3];
    TxBuf[4] = RxBuf[4];
    TxBuf[5] = RxBuf[5];
    *pTxTotal = 6;

    return MB_NO_ERR;
}

/****************************************************************
函数：com_slv_BMS_wr_reg
功能：主机接收BMS发过来的数据字。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_BMS_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, m;
//  U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *dest_buf = NULL;
    U08 *src_buf = NULL;
    U16 byte_count = 0;
    U08 com_err = MB_NO_ERR;
    BOOL fg_wr_succ = FALSE;    /* 调用外部子程序写入状态标志 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */

    if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* 功能码0x06: 写单个字  */
    {
        data_num = 1;
        src_buf = &RxBuf[4];
    }
    else                            /* 功能码0x10: 写多个字 */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
        src_buf = &RxBuf[7];
    }
    byte_count = data_num << 1; /* 数据个数为字数，所以字节数需乘以2 */
    if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)    /* 参数 */
    {
        fg_wr_succ = PARA_Modify(src_buf, data_addr, byte_count);
    }
    else if (data_addr >= BMS_CLOCK_BGN && data_addr + data_num <= BMS_CLOCK_END)	/* 时钟 */
    {
        U16 addr_offst = data_addr - BMS_CLOCK_BGN;
        U16 cell_offst = addr_offst / BMS_CLOCK_NUM;
        U16 cell_num = data_num / BMS_CLOCK_NUM;
        fg_wr_succ = SH_set_clock(src_buf, cell_offst, cell_num);
    }
#ifdef COM_4G
    else if (data_addr >= BMS_4G_RW_BGN && data_addr + data_num <= BMS_4G_RW_END)
    {
        U16 addr_offst = data_addr - BMS_4G_RW_BGN;
        dest_buf = (U16 *)&app_4g_rw + addr_offst;
    }
#endif
    else if (data_addr >= BMS_TEST_FAC_BGN && data_addr + data_num <= BMS_TEST_FAC_END)
    {
        U16 addr_offst = data_addr - BMS_TEST_FAC_BGN;
        dest_buf = (U16 *)&test_fac + addr_offst;
    }
#ifdef USE_BMS_DEBUG            
    else if(data_addr >= ADDR_BMS_DEBUG_BGN && data_addr + data_num <= ADDR_BMS_DEBUG_END) //调试数据
    {
        U16 addr_offst = data_addr - ADDR_BMS_DEBUG_BGN;
        dest_buf = (U16 *)&bms_debug + addr_offst;
    }
#endif 
#ifdef USE_BMS_SOURCE
    else if(data_addr >= ADDR_BMSSRC_BGN && data_addr + data_num <= ADDR_BMSSRC_END)/* BMS资源仿真 */
    {
        U16 addr_offst = data_addr - ADDR_BMSSRC_BGN;
        dest_buf = (U16 *)&bms_source + addr_offst;
    }
#endif
    else if (data_addr >= BMS_ROOM_TEMP_BGN && data_addr + data_num <= BMS_ROOM_TEMP_END)   /* 室内温度 */
    {
        fg_wr_succ = SH_set_room_temp(src_buf, data_num);
    }
    else
    {
        fg_wr_succ = 0;
    }
    if (dest_buf != NULL || fg_wr_succ)     /* 防错设计 */
    {
        if (!fg_wr_succ)    /* 没有通过外部子程序写入，在此处写入 */
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
#if 0


/****************************************************************
函数：com_slv_BMS_rd_input
功能：主机接收BMS发过来的读数据字请求。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_BMS_rd_input(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0,n = 0,l = 0;
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_U08_num = 0;
    U16 data_addr = 0;
    U08 *src_buf = NULL;
    U08 mac_addr = 0;
    U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */
    U08 *buf;
    U16 *pData;
    W16 Data_tmp[8];    // 临时中转变量

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
    if (data_addr >= RD_SYS_IO_ADDR1 && data_addr+data_num <= RD_SYS_IO_ADDR2)
    {
        Data_tmp[0].byte.high = cell[MAX_PCB].cell_relay[0];    // 系统输出
        Data_tmp[0].byte.low  = cell[pcb_addr].cell_relay[MAX_COMP];    // 单元输出

        Data_tmp[1].byte.high = cell[pcb_addr].cell_relay[1];       // 模块输出2
        Data_tmp[1].byte.low  = cell[pcb_addr].cell_relay[0];       // 模块输出1

        Data_tmp[2].byte.high=cell[MAX_PCB].cell_input[0];  // 系统单元输入
        Data_tmp[2].byte.low =cell[pcb_addr].cell_input[0]|cell[pcb_addr].cell_input[MAX_COMP];     // 模块输入

        Data_tmp[3].byte.high=cell[MAX_PCB].cell_error[1];  // 系统故障
        Data_tmp[3].byte.low =cell[MAX_PCB].cell_error[0];
        Data_tmp[4].byte.high=cell[MAX_PCB].cell_error[3];
        Data_tmp[4].byte.low =cell[MAX_PCB].cell_error[2];

        Data_tmp[5].byte.high=cell[pcb_addr].cell_error[1];     // 模块故障
        Data_tmp[5].byte.low =cell[pcb_addr].cell_error[0];
        Data_tmp[6].byte.high=cell[pcb_addr].cell_error[3];
        Data_tmp[6].byte.low =cell[pcb_addr].cell_error[2];
        Data_tmp[7].byte.high=cell[pcb_addr].cell_error[5];
        Data_tmp[7].byte.low =cell[pcb_addr].cell_error[4];

        pData = (U16 *)&Data_tmp[0];

        m = data_addr & 0x01ff;

        buf   = &TxBuf[3];  /* 缓存开始地址 */
        TxBuf[2] = ((data_num - 1) >> 3) + 1;   /* 回送数据字节数 */
        for (i=0; i<TxBuf[2]; i++) buf[i] = 0;


        for (i=0; i<data_num; i++, m++)
        {
            if ((pData[m>>4] >> (m&0x0f)) & 0x0001)  buf[i>>3] |= 0x01 << (i & 0x07);
        }
    }
    else
    {
        TxBuf[2] =0;
        com_err = MB_ILLEGAL_DATA_ADDR;
    }

    //TxBuf[2] = data_U08_num;  /* 数据个数 */
    *pTxTotal = TxBuf[2] + 3;
    return com_err;
}
#endif

/* 主板 BMS从机应答 */
/****************************************************************
12.3-51 BMS通讯应答(com_slv_BMS)
功能：主机应答BMS的读/写请求。
参数:   RxBuf--接收缓冲区;
        TxBuf--发送缓冲区;
        pTxTotal--需发送的字节总数;
返回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
其它: /
*****************************************************************/
U08 com_slv_BMS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = MB_NO_ERR;

    //work_bms_read();//预先处理BMS的数据

    switch (RxBuf[1])
    {
        case MB_DIAGNOSTICS :           /* 0x08 回路诊断 */
//          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_COILS :              /* 0x01 读可读写位 */
        case MB_RD_DISCRETE_INPUTS :    /* 0x02 读只读位 */
            exceptCode = com_slv_BMS_rd_input(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_HOLDING_REGISTERS :  /* 0x03 读可读写字 */
//          break;                  /* 03 04在一个子程序中处理，子程序内部区分功能码 */
        case MB_RD_INPUT_REGISTERS :    /* 0x04 读只读字 */
            exceptCode = com_slv_BMS_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_COIL :        /* 0x05 写单个位 */
            exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_REGISTER :    /* 0x06 写单个字 */
            exceptCode = com_slv_BMS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_COILS :     /* 0x0f 写多个位 */
            exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_REGISTERS : /* 0x10 写多个字 */
            exceptCode = com_slv_BMS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_USER_DEFINED_DOWNLOAD : /* 0x41 自定义：程序下载 */
            exceptCode = PDU_SlaveDealUserDef(RxBuf, TxBuf, pTxTotal);
            break;

        default :
            /* 一些正确的功能码，由于暂不支持，也作为非法功能码 */
            exceptCode = MB_ILLEGAL_FUNCTION;
            break;
    }

    return exceptCode;
}


/* 主板 BMS从机应答 */
/****************************************************************
12.3-51 RLAN通讯应答(com_slv_RLAN)
功能：RLAN从机应答RLAN的读/写请求。
参数:   RxBuf--接收缓冲区;
        TxBuf--发送缓冲区;
        pTxTotal--需发送的字节总数;
返回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
其它: /
*****************************************************************/
//U08 com_slv_RLAN(U08 *RxBuf, U08 *TxBuf, U16 rxcnt, U16 *pTxTotal)
//{
//  U08 exceptCode = MB_NO_ERR;
//  U16 i;
//
//  /* 接收缓冲区后移一个字节(原因：CAN没有设备地址字节，“RxBuf[0]”已经是有效数据) */
//  for (i=rxcnt; i>0; i--)
//  {
//      RxBuf[i] = RxBuf[i-1];
//  }
//
//  /* 按标准MODBUS协议将设备地址、功能码赋给发送缓冲区 */
//  TxBuf[0] = RxBuf[0];
//  TxBuf[1] = RxBuf[1];
//
//    switch (RxBuf[1])     /* 功能码 */
//  {
//      case MB_DIAGNOSTICS :           /* 0x08 回路诊断 */
//  //          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
//          break;
//
//      case MB_RD_HOLDING_REGISTERS :  /* 0x03 读可读写字 */
//  //          break;                  /* 03 04在一个子程序中处理，子程序内部区分功能码 */
//      case MB_RD_INPUT_REGISTERS :    /* 0x04 读只读字 */
//          exceptCode = com_slv_RLAN_rd_reg(RxBuf, TxBuf, pTxTotal);
//
//          break;
//
//      case MB_WR_SINGLE_REGISTER :    /* 0x06 写单个字 */
//          exceptCode = com_slv_RLAN_wr_reg(RxBuf, TxBuf, pTxTotal);
//          break;
//
//      case MB_WR_MULTIPLE_REGISTERS : /* 0x10 写多个字 */
//          exceptCode = com_slv_RLAN_wr_reg(RxBuf, TxBuf, pTxTotal);
//          break;
//
//      default :
//          /* 一些正确的功能码，由于暂不支持，也作为非法功能码 */
//          exceptCode = MB_ILLEGAL_FUNCTION;
//          break;
//  }
//
//  /* 发送缓冲区前移一个字节(CAN没有设备地址字节) */
//  for (i=0; i<*pTxTotal; i++)
//  {
//      TxBuf[i] = TxBuf[i+1];
//  }
//  (*pTxTotal)--;      /* 发送字节比标准MODBUS少一个字节 */
//
//  return exceptCode;
//}

/****************************************************************
函数：com_slv_RLAN_rd_reg
功能：RLAN从机接收RLAN主机发过来的读数据字请求。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
//U08 com_slv_RLAN_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
//{
//  U16 i;
//  U16 m = 0;
//  U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
//  U16 data_num = 0;
//  U16 data_addr = 0;
//  U16 *src_buf = NULL;
//  U08 mac_addr = 0;
//  U08 EX_addr = 0;
//  U08 com_err = MB_NO_ERR;    /* 故障状态 */
//
//  data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
//  data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
//
//  if (RxBuf[1] == MB_RD_INPUT_REGISTERS)      /* 只读整型 */
//  {
//
//      if (data_addr >= ADDR_MAC_BGN && data_addr + data_num <= ADDR_MAC_END)  /* 系统信息 */
//      {
//          addr_offst = data_addr - ADDR_MAC_BGN;
//          mac_addr = addr_offst / MAC_SIZE;
//          addr_offst = addr_offst % MAC_SIZE;
//  //          src_buf = (U16 *)&mac_sys[mac_addr] + addr_offst;
//          src_buf = (U16 *)pmac + addr_offst;
//      }
//  //      else if (data_addr >= ADDR_HIS_BGN && data_addr + data_num < ADDR_HIS_END)  /* 历史故障 */
//  //      {
//  //    //        src_buf = (U16 *)&errormsg[0] + addr_offst;
//  //      }
//  //      else if (data_addr >= ADDR_VER_BGN && data_addr + data_num < ADDR_VER_END)  /* 版本号 */
//  //      {
//  //          addr_offst = data_addr - ADDR_VER_BGN;
//  //          EX_addr = addr_offst / ONE_VER_SIZE;
//  //          addr_offst = addr_offst % ONE_VER_SIZE;
//  //          src_buf = (U16 *)&mac_ver[EX_addr] + addr_offst;
//  //      }
//    }
//  //    else          /* 可读写整型 */
//  //    {
//  //      if (data_addr >= ADDR_PARA_USER_BGN && data_addr + data_num < ADDR_PARA_END)    /* 参数:BMS仅支持用户参数 */
//  //      {
//  //          U08 grp, item;
//  //          src_buf = para_get_para_bgnaddr(data_addr, data_num, &grp, &item);
//  //      }
//  //    }
//
//    if (src_buf != NULL)      /* 防错设计 */
//    {
//      U08 *buf = &TxBuf[3];   /* 有效数据在发送缓存中的起始地址 */
//        m = 0;
//        for (i=0; i<data_num; i++)
//      {
//          buf[m++] = HIBYTE(src_buf[i]);
//          buf[m++] = LOBYTE(src_buf[i]);
//      }
//    }
//    else
//  {
//      com_err = MB_ILLEGAL_DATA_ADDR;
//  }
//
//  TxBuf[2] = data_num << 1;   /* 数据个数为字数，所以字节数需乘以2 */
//  *pTxTotal = TxBuf[2] + 3;
//
//  return com_err;
//}

/****************************************************************
函数：com_slv_RLAN_wr_reg
功能：RLAN从机接收RLAN主机发过来的数据字。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
//U08 com_slv_RLAN_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
//{
//  U16 i, m;
//  U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
//  U16 data_num = 0;
//  U16 data_addr = 0;
//  U16 *dest_buf = NULL;
//  U08 *src_buf = NULL;
//  U16 byte_count = 0;
//  U16 com_err = MB_NO_ERR;
//  BOOL fg_wr_succ = FALSE;    /* 调用外部子程序写入状态标志 */
//
//  data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
//
//  if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* 功能码0x06: 写单个字  */
//  {
//      data_num = 1;
//      src_buf = &RxBuf[4];
//  }
//  else                            /* 功能码0x10: 写多个字 */
//  {
//      data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
//      src_buf = &RxBuf[7];
//  }
//  byte_count = data_num << 1; /* 数据个数为字数，所以字节数需乘以2 */
//
//  if (data_addr >= ADDR_R_CMD_BGN && data_addr <= ADDR_R_CMD_END)         /* 机组间命令 */
//  {
//      dest_buf = (U16 *)&pmac->mac_cmd[CMD_NOR_IDX][0];
//  }
//  else if (data_addr >= ADDR_R_EY_BGN && data_addr <= ADDR_R_EY_END)      /* 机组能量需求 */
//  {
//      dest_buf = (U16 *)&pmac->mac_ey_need[0];
//  }
//
//    if (dest_buf != NULL || fg_wr_succ)       /* 防错设计 */
//    {
//      if (!fg_wr_succ)    /* 没有通过外部子程序写入，在此处写入 */
//      {
//            m = 0;
//            for (i=0; i<byte_count; i+=2)
//          {
//              dest_buf[m++] = MAKEWORD(src_buf[i], src_buf[i+1]);
//          }
//        }
//    }
//    else
//  {
//      com_err = MB_ILLEGAL_DATA_ADDR;
//  }
//
//  TxBuf[2] = RxBuf[2];
//  TxBuf[3] = RxBuf[3];
//  TxBuf[4] = RxBuf[4];
//  TxBuf[5] = RxBuf[5];
//  *pTxTotal = 6;
//
//  return com_err;
//}

/****************************************************************
函数：com_slv_FBUS_rd_reg
功能：主机从FBUS发过来的读数据字请求。
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_FBUS_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0;
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *src_buf = NULL;
//  U08 comp_addr = 0;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */

    if (RxBuf[1] == MB_RD_INPUT_REGISTERS)      /* 只读整型 */
    {
        if (data_addr >= FBUS_VER_BGN && ((data_addr + data_num <= FBUS_VER_END)||(TestProgStaus != TPS_NO_TEST_CMD)))  /* 版本号 */ // 测试时不判断数据个数
        {
            addr_offst = data_addr - FBUS_VER_BGN;
            src_buf = (U16 *)&idu_mac_ver[0] + addr_offst;
        }
        else if (data_addr >= FBUS_ODU_VER_BGN && (data_addr + data_num <= FBUS_ODU_VER_END))
        {
            addr_offst = data_addr - FBUS_ODU_VER_BGN;
            src_buf = (U16 *)&odu_mac_ver[0] + addr_offst;
        }
        else if (data_addr >= FBUS_MAC_BGN && data_addr + data_num <= FBUS_MAC_END) /* 系统信息 */
        {
            addr_offst = data_addr - FBUS_MAC_BGN;
            src_buf = (U16 *)&cell[0] + addr_offst;
        }
        else if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)   /* 参数 */
        {
            addr_offst = data_addr - ADDR_PARA_BGN;
            src_buf = (U16 *)&para_C[0] + addr_offst;
        }
    }
    else            /* 可读写整型 */
    {
#if 0
        if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)    /* 参数 */
        {
            U08 grp, item;
            src_buf = para_get_para_bgnaddr(data_addr, data_num, &grp, &item);
        }
#endif
    }

    if (src_buf != NULL)        /* 防错设计 */
    {
        U08 *buf = &TxBuf[3];   /* 有效数据在发送缓存中的起始地址 */
        m = 0;
        for (i=0; i<data_num; i++)
        {
            buf[m++] = HIBYTE(src_buf[i]);
            buf[m++] = LOBYTE(src_buf[i]);
        }
    }
    else
    {
        com_err = MB_ILLEGAL_DATA_ADDR;
    }

    TxBuf[2] = data_num << 1;
    *pTxTotal = TxBuf[2] + 3;

    return com_err;
}

/**
 * 函数 : system_sta_info
 * 功能 : 系统状态信息处理
 *
 * 参数 : /
 * 返回 : 1/状态已经更新
 *        0/状态没有更新
 *
 * 其它 : --
 */
U08 system_sta_info(U08 *buf, U16 addr, U16 num, U08 opt)
{
    U16  m, k;
    U16 *pData;

    if ((addr < FBUS_SYS_STA_BGN) || (addr >= FBUS_SYS_STA_END)) return 0;

    m = addr - FBUS_SYS_STA_BGN;
    pData = (U16 *)&pmac->cell_pv[0];

    switch (opt)
    {
    case _GET_ :
        break;
    case _SET_ :
        for (k=0; k<num; k += 2)
        {
            pData[m] = MAKEWORD(buf[k], buf[k+1]);
            m++;
        }
        break;
    }
    return 1;
}

/*******************************************************************************
 函 数: rece_bro_special()
 功 能: 从机接收：广播特殊数据
 参 数: buf--;
        addr--;
        num--;
 返 回:
 其 它: 空
*******************************************************************************/
U08 rece_bro_special(U08 *buf, U16 addr, U16 num)
{
    U16  m, k;
    U16 *pData;

    if ((addr < ADDR_SPECIAL_BGN) || (addr >= ADDR_SPECIAL_END)) return 0;

    m = addr - ADDR_SPECIAL_BGN;
    pData = (U16 *)&bro_special[0];

    for (k=0; k<num; k += 2)
    {
        pData[m] = MAKEWORD(buf[k], buf[k+1]);
        m++;
    }

    fg_idle_sta_antifreeze                  = bro_special[0];       /* 待机态防冻 */
    cell[MAX_PCB].cell_runtime[SYS_RUNTIME] = bro_special[1];       /* 机组累计运行时间 */
    limit_value                             = bro_special[2];       /* 系统维护设置 */
    limit_type                              = bro_special[3];       /* 系统维护模式 */
    sv_temp_energy[SUB_SYS_AC_HOTW]         = bro_special[4];
    ctrl_temp_energy[SUB_SYS_AC_HOTW]       = bro_special[5];
    if (power_time > POWER_ON_INTO_WORK+10) /* 防止从机曲轴闪开 */
    {
        fg_warm_complete                    = bro_special[6];
    }
    fg_timed_disinfect_run                  = bro_special[7];
    MacTypeMaster                           = bro_special[8];
    fg_silent_mode                          = bro_special[9];
    fg_ability_test_run                     = bro_special[10];
    ctrl_mode[SUB_SYS_AC_HOTW]              = bro_special[11];
    sv_temp_freq[SUB_SYS_AC_HOTW]           = bro_special[12];
    fg_TBH_run                              = bro_special[13];
    ac_mode                                 = bro_special[14];
    fg_idle_sta_antifreeze_IBH_need         = bro_special[15];
    fg_force_IBH_run                        = bro_special[16];
    fg_AHS_run                              = bro_special[17];
    fg_PUMPf_test_run                       = bro_special[18];
    fg_refri_recy_act                       = bro_special[19];
    heat_runtime[SUB_SYS_AC_HOTW].run       = bro_special[20];
    hotw_runtime.run                        = bro_special[21];
    fg_timed_disinfect_IBH_need             = bro_special[22];
    fg_enter_test_run                       = bro_special[23];
    fg_test_run_heat_IBH_run                = bro_special[24];
    fg_pump_test_run                        = bro_special[25];
    fg_test_run_check_LDIw                  = bro_special[26];
    fg_test_run_PUMPf_off                   = bro_special[27];
    fg_floor_preheat_run                    = bro_special[28];
    fg_floor_dryup_run                      = bro_special[29];
    fg_air_empty_test_run                   = bro_special[30];
    sg_power_mode                           = bro_special[31];
    fg_timed_disinfect_TBH_need             = bro_special[32];
    fg_timed_disinfect_AHS_need             = bro_special[33];
    fg_timed_disinfect_AHS_on               = bro_special[34];
    ctrl_mode[SUB_SYS_AC_ONLY]              = bro_special[35];
    sv_temp_energy[SUB_SYS_AC_ONLY]         = bro_special[36];
    ctrl_temp_energy[SUB_SYS_AC_ONLY]       = bro_special[37];
    sv_temp_freq[SUB_SYS_AC_ONLY]           = bro_special[38];
    status_run                              = bro_special[39];
    fg_run_sta_ac_antifreeze_IBH_need       = bro_special[40];
    fg_run_sta_hotw_antifreeze_IBH_need     = bro_special[41];
    idle_sta_antifreeze_stage               = bro_special[42];
    fg_run_sta_ac_antifreeze                = bro_special[43];
    fg_run_sta_hotw_antifreeze              = bro_special[44];
    fg_zone_A_timed_antifreeze_COMPf_need   = bro_special[45];
    fg_zone_B_timed_antifreeze_COMPf_need   = bro_special[46];
    fg_hotw_timed_antifreeze_COMPf_need     = bro_special[47];
    fg_cool_need                            = bro_special[48];
    fg_heat_need                            = bro_special[49];
    fg_hotw_need                            = bro_special[50];
    silent_mode_level                       = bro_special[51];
    fg_sv_temp_energy_overwrite_ac          = bro_special[52];
    ctrl_temp_type_energy_ac                = bro_special[53];
    heat_runtime[SUB_SYS_AC_ONLY].run       = bro_special[54];
    fg_energy_need[SUB_SYS_AC_HOTW]         = bro_special[55];
    fg_energy_need[SUB_SYS_AC_ONLY]         = bro_special[56];
    fg_hotw_timed_antifreeze_S3V1_OFF_need  = bro_special[57];
    fg_zone_A_run                           = bro_special[58];
    fg_zone_B_run                           = bro_special[59];
    fg_hotw_run                             = bro_special[60];

    return 1;
}

/****************************************************************
函数：com_slv_FBUS_wr_reg
功能：主机向子机卸数据字(FBUS)
参数：数据缓存；写数据地址；写数据个数。
返回：成功/失败
其它：/。
*****************************************************************/
U08 com_slv_FBUS_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, m;
//  U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
//  U16 addr_offst = 0;
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *dest_buf = NULL;
    U08 *src_buf = NULL;
    U16 byte_count = 0;
//  U08 cm_num = 0;
    U08 com_err = MB_NO_ERR;    /* 故障状态 */
    BOOL fg_wr_succ = FALSE;    /* 调用外部子程序写入状态标志 */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* 数据地址 */

    if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* 功能码0x06: 写单个字  */
    {
        data_num = 1;
        src_buf = &RxBuf[4];
    }
    else                            /* 功能码0x10: 写多个字 */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* 数据个数 */
        src_buf = &RxBuf[7];
    }
    byte_count = data_num << 1; /* 数据个数为字数，所以字节数需乘以2 */

    if (data_addr >= FBUS_CMD_BGN && data_addr + data_num <= FBUS_CMD_END)/* 控制命令 */
    {
        dest_buf = (U16 *)&cell[pcb_addr].cell_cmd[0];
    }
    else if (data_addr >= FBUS_SYS_STA_BGN && data_addr + data_num <= FBUS_SYS_STA_END)/* 系统状态 */
    {
        fg_wr_succ = system_sta_info(src_buf, data_addr, byte_count, _SET_);
    }
    else if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)   /* 参数 */
    {
        fg_wr_succ = PARA_Modify(src_buf, data_addr, byte_count);
    }
    else if (data_addr >= ADDR_PASS_BGN && data_addr + data_num <= ADDR_PASS_END)   /* 密码 */
    {
        fg_wr_succ = password_information(src_buf, data_addr, byte_count, _SET_);
    }
    else if (data_addr >= ADDR_SPECIAL_BGN && data_addr + data_num <= ADDR_SPECIAL_END) /* 广播特殊数据 */
    {
        fg_wr_succ = rece_bro_special(src_buf, data_addr, byte_count);
    }

    if (dest_buf != NULL || fg_wr_succ)     /* 防错设计 */
    {
        if (!fg_wr_succ)    /* 没有通过外部子程序写入，在此处写入 */
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

/* S-EX板 FBUS从机应答 */
/****************************************************************
函数：12.3-71 FBUS通讯应答(com_slv_FBUS)
功能：主机应答BMS的读/写请求。
参数:   RxBuf--接收缓冲区;
        TxBuf--发送缓冲区;
        pTxTotal--需发送的字节总数;
返回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
其它: /
*****************************************************************/
U08 com_slv_FBUS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = MB_NO_ERR;

    switch (RxBuf[1])
    {
        case MB_DIAGNOSTICS :           /* 0x08 回路诊断 */
//          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_HOLDING_REGISTERS :  /* 0x03 读可读写字 */
//          break;                  /* 03 04在一个子程序中处理，子程序内部区分功能码 */
        case MB_RD_INPUT_REGISTERS :    /* 0x04 读只读字 */
            exceptCode = com_slv_FBUS_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

//      case MB_WR_SINGLE_COIL :        /* 0x05 写单个位 */
//          exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
//          break;

        case MB_WR_SINGLE_REGISTER :    /* 0x06 写单个字 */
            exceptCode = com_slv_FBUS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

//      case MB_WR_MULTIPLE_COILS :     /* 0x0f 写多个位 */
//          exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
//          break;

        case MB_WR_MULTIPLE_REGISTERS : /* 0x10 写多个字 */
            exceptCode = com_slv_FBUS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        default :
            /* 一些正确的功能码，由于暂不支持，也作为非法功能码 */
            exceptCode = MB_ILLEGAL_FUNCTION;
            break;
    }

    return exceptCode;
}



