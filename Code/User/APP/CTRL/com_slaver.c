/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: IO.c
 �ļ�����:
 ������  :
 ��������:
*******************************************************************************/
//#define  COM_GLOBAL
#include "includes.h"

/*******************************************************************************
 �� ��: SlaveDealUserDefined()wlg20150715
 �� ��: �ӻ�����ModBus�û��Զ��幦����:��������
 �� ��: pduRxBuf--PDU���ջ�����;
        pduTxBuf--PDU���ͻ�����;
        pPduTxTotal--PDU�跢�͵��ֽ�����;
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �ж����Զ��������Э���Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
        ����ȷ���������������ӦPDU�����ͻ�����
        ����Ӧ��Э��
1��ѯ��Ӧ�ó����Ƿ��ܹ�����BOOT
01 + 41 + 0000 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERACROSSRS485ENTER"
�ܹ�����BOOT:01 + 41 + 0000 + 0010 + 00 + 01 + CRC
���ܽ���BOOT:01 + 41 + 0000 + 0010 + 00 + 00 + CRC
2�����������BOOT
01 + 41 + 0010 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERENTERBOOTFROMAPP"
����ܹ�����BOOT:���أ�ֱ�ӽ���BOOT
������ܹ�����BOOT:01 + 41 + 0010 + 0010 + 00 + 00 + CRC
*******************************************************************************/
U08 PDU_SlaveDealUserDef(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
{
    U16 subFunctionCode;    /* �ӹ����� */
    static CONST I08 s_askMsg[] =      "PUNPPLDOWNLOADERACROSSRS485ENTER";
    static CONST I08 s_downLoadMsg[] = "PUNPPLDOWNLOADERENTERBOOTFROMAPP";
    //U16 *pData;               /* ����ָ�� */

    subFunctionCode = MAKEWORD(pduRxBuf[2], pduRxBuf[3]);

    /* �����ж� */
    if ((MAKEWORD(pduRxBuf[4], pduRxBuf[5]) != 16)
        || (pduRxBuf[6] != 32))
    {
        return MB_ILLEGAL_DATA_VALUE;
    }

    /* �ӹ�������ж��봦�� */
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
        case MB_0x41_SUB_FUNC_ACCEPT_BOOT:      // ѯ���Ƿ��������
            if (STRNCMP(s_askMsg, (I08 *)&pduRxBuf[7], 32) == 0)
            {
                MEMCPY(&pduTxBuf[1], &pduRxBuf[1], 5);
                *pPduTxTotal = 8;

				switch (fg_accept_remote_upgrade)
				{
					case SH_REMOTE_REQ_WAIT:	/* δȷ���Ƿ����� */
                        pduTxBuf[6] = 0;
		                pduTxBuf[7] = fg_accept_remote_upgrade;
						break;

					case SH_REMOTE_REQ_OK:	/* ȷ��Ҫ��������ȴ�����ر� */
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

					case SH_REMOTE_REQ_CANCEL:	/* ȡ������ */
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
        case MB_0x41_SUB_FUNC_REQUEST_BOOT:     // ִ����������
            if (STRNCMP(s_downLoadMsg, (I08 *)&pduRxBuf[7], 32) == 0)
            {
                MEMCPY(&pduTxBuf[1], &pduRxBuf[1], 5);
                *pPduTxTotal = 8;

                /* ���ư��贫����ʾ��һ����־ */
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
������com_slv_HMI_rd_input
���ܣ���������HMI�������Ķ�����������
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_HMI_rd_input(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0,n = 0,l = 0;
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_U08_num = 0;
    U16 data_addr = 0;
    U08 *src_buf = NULL;
//  U08 mac_addr = 0;
//  U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
    data_U08_num = ((data_num%8) == 0) ? (data_num/8) : ((data_num/8)+1);


    if (data_addr >= ADDR_TEST_BGN )
    {
        addr_offst = (data_addr - ADDR_TEST_BGN)/8;
        src_buf = (U08 *)&ITbl[addr_offst];
    }

    n=(data_addr - ADDR_TEST_BGN)%8;
    m = 0;
    if (src_buf != NULL)        /* ������� */
    {
        U08 *buf = &TxBuf[3];   /* ��Ч�����ڷ��ͻ����е���ʼ��ַ */

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

    TxBuf[2] = data_U08_num;    /* ���ݸ��� */
    *pTxTotal = TxBuf[2] + 3;
    return com_err;
}



/****************************************************************
������com_slv_HMI_rd_reg
���ܣ���������HMI�������Ķ�����������
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_HMI_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0;
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *src_buf = NULL;
//  U08 mac_addr = 0;
    U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */
    BOOL fg_rd_succ = FALSE;    /* �����ⲿ�ӳ���д��״̬��־ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */

    if (RxBuf[1] == MB_RD_INPUT_REGISTERS)      /* ֻ������ */
    {
        if (data_addr >= ADDR_TEST_VERSION_BGN && (data_addr + data_num <= ADDR_TEST_VERSION_END))  /* ����汾�� */
        {
            addr_offst = data_addr - ADDR_TEST_VERSION_BGN;
            EX_addr = addr_offst / ONE_VER_SIZE;
            addr_offst = addr_offst % ONE_VER_SIZE;
            src_buf = (U16 *)&idu_mac_ver[EX_addr] + addr_offst;
        }
        else if(data_addr >= ADDR_TEST_DI_BGN && (data_addr + data_num <= ADDR_TEST_DI_END))            // ���Կ�����
        {
            addr_offst = data_addr - ADDR_TEST_DI_BGN;
            src_buf = (U16 *)&TestTbl_DI[addr_offst];
        }
    }
    else            /* �ɶ�д���� */
    {
        if(data_addr >= ADDR_TEST_STA_BGN && (data_addr + data_num <= ADDR_TEST_STA_END))                 // ����״̬
        {
            addr_offst = data_addr - ADDR_TEST_STA_BGN;
            src_buf = (U16 *)&TestTbl_STA[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_NTC_BGN && (data_addr + data_num <= ADDR_TEST_NTC_END))          // �����¶�
        {
            addr_offst = data_addr - ADDR_TEST_NTC_BGN;
            src_buf = (U16 *)&TestTbl_NTC[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_CUR_BGN && (data_addr + data_num <= ADDR_TEST_CUR_END))          // ���Ե���
        {
            addr_offst = data_addr - ADDR_TEST_CUR_BGN;
            src_buf = (U16 *)&TestTbl_CUR[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_AITYPE_BGN && (data_addr + data_num <= ADDR_TEST_AITYPE_END))        // ����AI����
        {
            addr_offst = data_addr - ADDR_TEST_AITYPE_BGN;
            src_buf = (U16 *)&TestTbl_AI_TYPE[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_AI_BGN && (data_addr + data_num <= ADDR_TEST_AI_END))                // ����AI
        {
            addr_offst = data_addr - ADDR_TEST_AI_BGN;
            src_buf = (U16 *)&TestTbl_AI_DATA[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_STEPNOW_BGN && (data_addr + data_num <= ADDR_TEST_STEPNOW_END))    // ���Բ��������ǰֵ
        {
            addr_offst = data_addr - ADDR_TEST_STEPNOW_BGN;
            src_buf = (U16 *)&TestTbl_STEP_NOW[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_STEPSET_BGN && (data_addr + data_num <= ADDR_TEST_STEPSET_END))  // ���Բ�������趨ֵ
        {
            addr_offst = data_addr - ADDR_TEST_STEPSET_BGN;
            src_buf = (U16 *)&TestTbl_STEP_SET[addr_offst];
        }
        else if(data_addr >= ADDR_TEST_COMMU_BGN && (data_addr + data_num <= ADDR_TEST_COMMU_END))      // ����ͨѶ
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
        else if(data_addr >= ADDR_4G_TEST_BGN && data_addr + data_num <= ADDR_4G_TEST_END)  //��ͨѶ��Ϣ����
        {
             addr_offst = data_addr - ADDR_4G_TEST_BGN;
             src_buf = (U16 *)&test_4g + addr_offst;
        }
        else if(data_addr >= BMS_4G_RO_BGN && data_addr + data_num <= BMS_4G_RO_END)    //��ͨѶ��Ϣ����
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
        else if (data_addr >= ADDR_PASS_BGN && data_addr + data_num <= ADDR_PASS_END)   /* ���� */
        {
            password_superword();//wlg2015.06.12
            addr_offst = data_addr - ADDR_PASS_BGN;
            //src_buf = (U16 *)&password[0] + addr_offst;
        }
#endif
    }

    if (src_buf != NULL || fg_rd_succ)      /* ������� */
    {
        if (!fg_rd_succ)    /* û��ͨ���ⲿ�ӳ����ȡ���ڴ˴���ȡ(д���ͻ���) */
        {
            U08 *buf = &TxBuf[3];   /* ��Ч�����ڷ��ͻ����е���ʼ��ַ */
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

//  TxBuf[2] = (data_num << 1); /* ���ݸ���Ϊ�����������ֽ��������2 */
        TxBuf[2] = (data_num * 2);  /* ���ݸ���Ϊ�����������ֽ��������2 */

    *pTxTotal = TxBuf[2] + 3;

    return com_err;
}
#if 0
/****************************************************************
������com_slv_HMI_wr_coil
���ܣ���������HMI�����������֡�
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_HMI_wr_coil(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{

    U16 i, n, m;
    U16 data_num, data_addr;
    U16 *dest_buf = NULL;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */

    if (RxBuf[1] == MB_WR_SINGLE_COIL)      /* ������0x05: д����λ  */
    {
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END) /* �û����� */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            n = m >> 4;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* �������� */
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
    else                            /* ������0x0f: д���λ */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END)    /* �û����� */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* �������� */
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
������com_slv_HMI_wr_reg
���ܣ���������HMI�������������֡�
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_HMI_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, m;
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *dest_buf = NULL;
    U08 *src_buf = NULL;
    U16 byte_count = 0;
    U08 com_err = MB_NO_ERR;
    BOOL fg_wr_succ = FALSE;    /* �����ⲿ�ӳ���д��״̬��־ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */

    if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* ������0x06: д������  */
    {
        data_num = 1;
        src_buf = &RxBuf[4];
    }
    else                            /* ������0x10: д����� */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
        src_buf = &RxBuf[7];
    }
    byte_count = data_num << 1; /* ���ݸ���Ϊ�����������ֽ��������2 */
    if(data_addr >= ADDR_TEST_CMD_BGN && (data_addr + data_num <= ADDR_TEST_CMD_END))                   // ��������
    {
        addr_offst = data_addr - ADDR_TEST_CMD_BGN;
        dest_buf = (U16 *)&TestTbl_CMD[addr_offst];
    }
    else if(data_addr >= ADDR_TEST_AITYPE_BGN && (data_addr + data_num <= ADDR_TEST_AITYPE_END))        // ����AI����
    {
        addr_offst = data_addr - ADDR_TEST_AITYPE_BGN;
        dest_buf = (U16 *)&TestTbl_AI_TYPE[addr_offst];
    }
    else if(data_addr >= ADDR_TEST_STEPSET_BGN && (data_addr + data_num <= ADDR_TEST_STEPSET_END))  // ���Բ�������趨ֵ
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

    if (dest_buf != NULL || fg_wr_succ)     /* ������� */
    {
        if (!fg_wr_succ)    /* û��ͨ���ⲿ�ӳ���д�룬�ڴ˴�д�� */
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


/* ���� HMI�ӻ�Ӧ��8������ */
/****************************************************************
12.3-50 HMIͨѶӦ��(com_slv_HMI)
����: ���ذ�Ӧ��HMI�Ķ�/д����
����:   RxBuf--���ջ�����;
        TxBuf--���ͻ�����;
        pTxTotal--�跢�͵��ֽ�����;
����: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
����: /
*****************************************************************/
U08 com_slv_HMI(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = MB_NO_ERR;

    switch (RxBuf[1])
    {
        case MB_DIAGNOSTICS :           /* 0x08 ��·��� */
//          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_COILS :              /* 0x01 ���ɶ�дλ */

            break;

        case MB_RD_DISCRETE_INPUTS :    /* 0x02 ��ֻ��λ */
            exceptCode = com_slv_HMI_rd_input(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_HOLDING_REGISTERS :  /* 0x03 ���ɶ�д�� */

//          break;          /* 03 04��һ���ӳ����д����ӳ����ڲ����ֹ����� */
        case MB_RD_INPUT_REGISTERS :    /* 0x04 ��ֻ���� */
            exceptCode = com_slv_HMI_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_COIL :        /* 0x05 д����λ */
            exceptCode = MB_NO_ERR;
//          exceptCode = com_slv_HMI_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_REGISTER :    /* 0x06 д������ */
            exceptCode = com_slv_HMI_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_COILS :     /* 0x0f д���λ */
            exceptCode = MB_NO_ERR;
//          exceptCode = com_slv_HMI_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_REGISTERS : /* 0x10 д����� */
            exceptCode = com_slv_HMI_wr_reg(RxBuf, TxBuf, pTxTotal);        /*  */
            break;

        case MB_USER_DEFINED_DOWNLOAD : /* 0x41 �Զ��壺�������� */
            exceptCode = PDU_SlaveDealUserDef(RxBuf, TxBuf, pTxTotal);
            break;

        default :
            /* һЩ��ȷ�Ĺ����룬�����ݲ�֧�֣�Ҳ��Ϊ�Ƿ������� */
            exceptCode = MB_ILLEGAL_FUNCTION;
            break;
    }

    return exceptCode;
}


/*==========================================
    ���ܣ�
    num : �ֽ���
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
        else    // ��ֹMAX_PV��С�󵫴˴�δ�Ķ���������ֵ
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
������com_slv_BMS_rd_input
���ܣ���������BMS�������Ķ�����������
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_BMS_rd_input(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0,n = 0,l = 0;
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_U08_num = 0;
    U16 data_addr = 0;
    U08 *src_buf = NULL;
    U08 mac_addr = 0;
    //U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */
    BOOL fg_rd_succ = FALSE;    /* �����ⲿ�ӳ���д��״̬��־ */
    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
    data_U08_num = ((data_num%8) == 0) ? (data_num/8) : ((data_num/8)+1);

    switch (RxBuf[1])
    {
        case MB_RD_COILS :              /* 0x01 ���ɶ�дλ */

            /* �û�����,��ȡΪ0 */
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

        case MB_RD_DISCRETE_INPUTS :    /* 0x02 ��ֻ��λ */
            if (data_addr >= ADDR_SYS_BIT_BGN && data_addr+data_num <= ADDR_SYS_BIT_END)   /* ϵͳλ��Ϣ */
            {
                addr_offst = (data_addr - ADDR_SYS_BIT_BGN)/8;
                src_buf = (U08 *)&cell[MAX_PCB].cell_relay + addr_offst;
                n=(data_addr - ADDR_SYS_BIT_BGN)%8;
            }
            else if (data_addr >= ADDR_UNIT_BIT_BGN && data_addr+data_num <= ADDR_UNIT_BIT_END) /* ģ��λ��Ϣ */
            {
        		addr_offst = data_addr - ADDR_UNIT_BIT_BGN;
        		mac_addr = addr_offst / UNIT_STEP_NUM;
        		addr_offst = (addr_offst % UNIT_STEP_NUM)/8;
                /* ֻ���Է���ʵ�ʳ��� */
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
        ; // nothing ��д��,ɶ������
    }
    else if (src_buf != NULL)       /* ������� */
    {
        U08 *buf = &TxBuf[3];   /* ��Ч�����ڷ��ͻ����е���ʼ��ַ */

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

    TxBuf[2] = data_U08_num;    /* ���ݸ��� */
    *pTxTotal = TxBuf[2] + 3;
    return com_err;
}

/****************************************************************
������com_slv_BMS_rd_reg
���ܣ���������BMS�������Ķ�����������
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_BMS_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0;
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *src_buf = NULL;
    U08 mac_addr = 0;
    U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */
    BOOL fg_rd_succ = FALSE;    /* �����ⲿ�ӳ���д��״̬��־ */
    U16 cell_offst = 0;     /* ��Ԫ��ʼλ��(�ӵڼ����ṹ�嵥Ԫ��ʼ��) */
    U16 cell_num = 0;       /* ��Ԫ����(��ȡ�����ṹ�嵥Ԫ) */

    static U16* eeprom_addr = (U16*)0;
    static U32* flash_addr = (U32*)0;

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */

    if (data_addr >= ADDR_DEBUG_BGN && data_addr + data_num <= ADDR_DEBUG_END) //��ȡ������Ϣ
    {
        addr_offst = data_addr - ADDR_DEBUG_BGN;
        src_buf = (U16 *)&gdebugIO.debugstart + addr_offst;
    }
    else if (data_addr >= ADDR_VER_BS_BGN && (data_addr + data_num <= ADDR_VER_BS_END))  /* ����汾�� */
    {
        addr_offst = data_addr - ADDR_VER_BS_BGN;
        EX_addr = addr_offst / VER_BS_SIZE;
        addr_offst = addr_offst % VER_BS_SIZE;
        src_buf = (U16 *)&idu_mac_ver[EX_addr] + addr_offst;
    }
    else if (data_addr >= ADDR_VER2_BGN && (data_addr + data_num <= ADDR_VER2_END))  /* ����汾�� */
    {
        addr_offst = data_addr - ADDR_VER2_BGN;
        MEMCPY(&TxBuf[3], &idu_mac_ver[0][0]+addr_offst, data_num<<1);
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= ADDR_CTRL_BGN && data_addr + data_num <= ADDR_CTRL_END)   /*������Ϣ ������Ϣ */
    {
        addr_offst = data_addr - ADDR_CTRL_BGN;
        src_buf = (U16 *)&ctrl_info[0] + addr_offst;
    }
    else if (data_addr >= ADDR_ASSI_BGN && data_addr + data_num <= ADDR_ASSI_END)   /* ������Ϣ */
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
    else if (data_addr >= ADDR_COMPNUM_BGN && data_addr + data_num <= ADDR_COMPNUM_END)/* ѹ������ */
    {
        addr_offst = data_addr - ADDR_COMPNUM_BGN;
        src_buf = (U16 *)&pcb_comp_num[0] + addr_offst;
    }
    else if (data_addr >= ADDR_UNIT_MODE_BGN && data_addr + data_num <= ADDR_UNIT_MODE_END) /* ϵͳģ���ۺ���Ϣ */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = get_information(&TxBuf[3], data_addr, data_num<<1);
    }
    else if (data_addr >= ADDR_UNIT_BGN && data_addr + data_num <= ADDR_UNIT_END)   /* ģ����Ϣ */
    {
        addr_offst = data_addr - ADDR_UNIT_BGN;
        mac_addr = addr_offst >> UNIT_BIN_BIT_NUM;
        addr_offst = addr_offst & (UNIT_MODE_SINGLE_NUM - 1);
        src_buf = (U16 *)&cell[mac_addr] + addr_offst;
    }
    else if (data_addr >= ADDR_SYS_BGN && data_addr + data_num <= ADDR_SYS_END) /* ϵͳ��Ϣ */
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
        U08 VerCode[MAX_VER_BYTE] = {0};        /* ���ڽ���Ƶ���汾��ת��ΪU08���� */
        /* ����Ƶ���汾��ת��ΪU08���� */
        dm603_get_vfd_ver(VerCode);
        
        addr_offst = data_addr - ADDR_VER_INVT_BGN;
        MEMCPY(&TxBuf[3], &VerCode[0]+addr_offst, data_num<<1);
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= BMS_CLOCK_BGN && data_addr + data_num <= BMS_CLOCK_END)	/* ʱ�� */
    {
        /* �ݲ�֧�ֶ�ʱ�� */
    }
    else if (data_addr >= BMS_ERR_HIS_BGN && data_addr + data_num <= BMS_ERR_HIS_END)/* ���� ��ʷ */
    {
        addr_offst = data_addr - BMS_ERR_HIS_BGN;
        cell_offst = addr_offst / BMS_ERR_HIS_SIZE;
        cell_num = data_num / BMS_ERR_HIS_SIZE;
        fg_rd_succ = SH_get_his_error(&TxBuf[3], cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)   /* ����ֵ */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = PARA_GetVal(&TxBuf[3], data_addr, data_num<<1);
    }
    else if (data_addr >= ADDR_PARA_ITEM_BGN && data_addr + data_num <= ADDR_PARA_ITEM_END) /* ������ ���� */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = PARA_GetInfo(&TxBuf[3], data_addr, data_num<<1);
    }
    else if (data_addr >= ADDR_PARA_INFO_BGN && data_addr + data_num <= ADDR_PARA_INFO_END) /* ������ ���� */
    {
        src_buf = (U16 *)&TxBuf[3];
        fg_rd_succ = PARA_GetAttrib(&TxBuf[3], data_addr, data_num<<1);
    }
#ifdef USE_BMS_DEBUG            
    else if(data_addr >= ADDR_BMS_DEBUG_BGN && data_addr + data_num <= ADDR_BMS_DEBUG_END) //��������
    {
//        com_slv_BMS_debug_read();
        addr_offst = data_addr - ADDR_BMS_DEBUG_BGN;
        src_buf = (U16 *)&bms_debug + addr_offst;
    }
#endif      
#ifdef USE_COMMU_TEST_READ
    else if(data_addr >= ADDR_TEST_COMMU_BGN && (data_addr + data_num <= ADDR_TEST_COMMU_END))      // ����ͨѶ
    {
        addr_offst = data_addr - ADDR_TEST_COMMU_BGN;
        src_buf = (U16 *)&TestTbl_COMMU[addr_offst];
    }
#ifdef COM_4G
    else if(data_addr >= ADDR_4G_TEST_BGN && data_addr + data_num <= ADDR_4G_TEST_END)  //��ͨѶ��Ϣ����
    {
         addr_offst = data_addr - ADDR_4G_TEST_BGN;
         src_buf = (U16 *)&test_4g + addr_offst;
    }
#endif    
#endif  
#ifdef COM_4G 
    else if(data_addr >= BMS_4G_RO_BGN && data_addr + data_num <= BMS_4G_RO_END)    //��ͨѶ��Ϣ����
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
    else if (data_addr >= 60672 && data_addr + data_num <= 60673)   /* ���⹦��0xED00: ��ַ��λ����ͷ��EEPROM */
    {
        src_buf = (U16 *)&TxBuf[3];
        eeprom_addr = (U16*)0;
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= 60928 && data_addr + data_num <= 60993)   /* ���⹦��0xEE00: ��32K EEPROM */
    {
        src_buf = (U16 *)&TxBuf[3];
        if (eeprom_addr < (U16*)0x8000)
        {
            TxBuf[3] = 0;
            TxBuf[4] = 0;
            TxBuf[5] = HIBYTE((U32)eeprom_addr);
            TxBuf[6] = LOBYTE((U32)eeprom_addr);
            DBG_EEP_ReadChip(&TxBuf[7], (U16)(U32)eeprom_addr, 128, EEP_CHIP_NOR);
            eeprom_addr += 64;              /* ����2�ֽڶ��룬�˴�Ϊ128/2 */
            fg_rd_succ = TRUE;
        }
    }
    else if (data_addr >= 65024 && data_addr + data_num <= 65025)   /* ���⹦��0xFE00: ��ַ���㣬��ͷ��FLASH */
    {
        src_buf = (U16 *)&TxBuf[3];
        flash_addr = 0;
        fg_rd_succ = TRUE;
    }
    else if (data_addr >= 65280 && data_addr + data_num <= 65346)   /* ���⹦��0xFF00: ��512K FLASH */
    {
        src_buf = (U16 *)&TxBuf[3];
        if (flash_addr < (U32*)0x00080000)
        {
            TxBuf[3] = DWORDHI((U32)flash_addr);
            TxBuf[4] = DWORDMH((U32)flash_addr);
            TxBuf[5] = DWORDML((U32)flash_addr);
            TxBuf[6] = DWORDLO((U32)flash_addr);
            MEMCPY(&TxBuf[7], (U08*)flash_addr, 128);
            flash_addr += 32;               /* ����4�ֽڶ��룬�˴�Ϊ128/4 */
            fg_rd_succ = TRUE;
        }
    }
#endif

    if (src_buf != NULL || fg_rd_succ)      /* ������� */
    {
        if (!fg_rd_succ)    /* û��ͨ���ⲿ�ӳ����ȡ���ڴ˴���ȡ(д���ͻ���) */
        {
            U08 *buf = &TxBuf[3];   /* ��Ч�����ڷ��ͻ����е���ʼ��ַ */
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

    TxBuf[2] = data_num << 1;   /* ���ݸ���Ϊ�����������ֽ��������2 */
    *pTxTotal = TxBuf[2] + 3;

    return com_err;
}

/****************************************************************
������com_slv_BMS_wr_coil
���ܣ���������BMS�����������֡�
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_BMS_wr_coil(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, n, m;
    U16 data_num, data_addr;
    U16 *dest_buf = NULL;
//  U08 com_err = MB_NO_ERR;    /* ����״̬ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */

    if (RxBuf[1] == MB_WR_SINGLE_COIL)      /* ������0x05: д����λ  */
    {
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END) /* �û����� */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            n = m >> 4;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* �������� */
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
        else if (data_addr >= ADDR_TEST_DO_BGN && data_addr < ADDR_TEST_DO_END) /* �ֶ����� */
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
    else                            /* ������0x0f: д���λ */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
        if (data_addr >= ADDR_USER_CMD_BGN && data_addr < ADDR_USER_CMD_END)    /* �û����� */
        {
            m = data_addr - ADDR_USER_CMD_BGN;
            dest_buf = cmd_normal;
        }
        else if (data_addr >= ADDR_FAC_CMD_BGN && data_addr < ADDR_FAC_CMD_END) /* �������� */
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
        else if (data_addr >= ADDR_TEST_DO_BGN && data_addr < ADDR_TEST_DO_END) /* �ֶ����� */
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
������com_slv_BMS_wr_reg
���ܣ���������BMS�������������֡�
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_BMS_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, m;
//  U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *dest_buf = NULL;
    U08 *src_buf = NULL;
    U16 byte_count = 0;
    U08 com_err = MB_NO_ERR;
    BOOL fg_wr_succ = FALSE;    /* �����ⲿ�ӳ���д��״̬��־ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */

    if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* ������0x06: д������  */
    {
        data_num = 1;
        src_buf = &RxBuf[4];
    }
    else                            /* ������0x10: д����� */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
        src_buf = &RxBuf[7];
    }
    byte_count = data_num << 1; /* ���ݸ���Ϊ�����������ֽ��������2 */
    if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)    /* ���� */
    {
        fg_wr_succ = PARA_Modify(src_buf, data_addr, byte_count);
    }
    else if (data_addr >= BMS_CLOCK_BGN && data_addr + data_num <= BMS_CLOCK_END)	/* ʱ�� */
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
    else if(data_addr >= ADDR_BMS_DEBUG_BGN && data_addr + data_num <= ADDR_BMS_DEBUG_END) //��������
    {
        U16 addr_offst = data_addr - ADDR_BMS_DEBUG_BGN;
        dest_buf = (U16 *)&bms_debug + addr_offst;
    }
#endif 
#ifdef USE_BMS_SOURCE
    else if(data_addr >= ADDR_BMSSRC_BGN && data_addr + data_num <= ADDR_BMSSRC_END)/* BMS��Դ���� */
    {
        U16 addr_offst = data_addr - ADDR_BMSSRC_BGN;
        dest_buf = (U16 *)&bms_source + addr_offst;
    }
#endif
    else if (data_addr >= BMS_ROOM_TEMP_BGN && data_addr + data_num <= BMS_ROOM_TEMP_END)   /* �����¶� */
    {
        fg_wr_succ = SH_set_room_temp(src_buf, data_num);
    }
    else
    {
        fg_wr_succ = 0;
    }
    if (dest_buf != NULL || fg_wr_succ)     /* ������� */
    {
        if (!fg_wr_succ)    /* û��ͨ���ⲿ�ӳ���д�룬�ڴ˴�д�� */
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
������com_slv_BMS_rd_input
���ܣ���������BMS�������Ķ�����������
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_BMS_rd_input(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0,n = 0,l = 0;
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_U08_num = 0;
    U16 data_addr = 0;
    U08 *src_buf = NULL;
    U08 mac_addr = 0;
    U08 EX_addr = 0;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */
    U08 *buf;
    U16 *pData;
    W16 Data_tmp[8];    // ��ʱ��ת����

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
    if (data_addr >= RD_SYS_IO_ADDR1 && data_addr+data_num <= RD_SYS_IO_ADDR2)
    {
        Data_tmp[0].byte.high = cell[MAX_PCB].cell_relay[0];    // ϵͳ���
        Data_tmp[0].byte.low  = cell[pcb_addr].cell_relay[MAX_COMP];    // ��Ԫ���

        Data_tmp[1].byte.high = cell[pcb_addr].cell_relay[1];       // ģ�����2
        Data_tmp[1].byte.low  = cell[pcb_addr].cell_relay[0];       // ģ�����1

        Data_tmp[2].byte.high=cell[MAX_PCB].cell_input[0];  // ϵͳ��Ԫ����
        Data_tmp[2].byte.low =cell[pcb_addr].cell_input[0]|cell[pcb_addr].cell_input[MAX_COMP];     // ģ������

        Data_tmp[3].byte.high=cell[MAX_PCB].cell_error[1];  // ϵͳ����
        Data_tmp[3].byte.low =cell[MAX_PCB].cell_error[0];
        Data_tmp[4].byte.high=cell[MAX_PCB].cell_error[3];
        Data_tmp[4].byte.low =cell[MAX_PCB].cell_error[2];

        Data_tmp[5].byte.high=cell[pcb_addr].cell_error[1];     // ģ�����
        Data_tmp[5].byte.low =cell[pcb_addr].cell_error[0];
        Data_tmp[6].byte.high=cell[pcb_addr].cell_error[3];
        Data_tmp[6].byte.low =cell[pcb_addr].cell_error[2];
        Data_tmp[7].byte.high=cell[pcb_addr].cell_error[5];
        Data_tmp[7].byte.low =cell[pcb_addr].cell_error[4];

        pData = (U16 *)&Data_tmp[0];

        m = data_addr & 0x01ff;

        buf   = &TxBuf[3];  /* ���濪ʼ��ַ */
        TxBuf[2] = ((data_num - 1) >> 3) + 1;   /* ���������ֽ��� */
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

    //TxBuf[2] = data_U08_num;  /* ���ݸ��� */
    *pTxTotal = TxBuf[2] + 3;
    return com_err;
}
#endif

/* ���� BMS�ӻ�Ӧ�� */
/****************************************************************
12.3-51 BMSͨѶӦ��(com_slv_BMS)
���ܣ�����Ӧ��BMS�Ķ�/д����
����:   RxBuf--���ջ�����;
        TxBuf--���ͻ�����;
        pTxTotal--�跢�͵��ֽ�����;
����: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
����: /
*****************************************************************/
U08 com_slv_BMS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = MB_NO_ERR;

    //work_bms_read();//Ԥ�ȴ���BMS������

    switch (RxBuf[1])
    {
        case MB_DIAGNOSTICS :           /* 0x08 ��·��� */
//          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_COILS :              /* 0x01 ���ɶ�дλ */
        case MB_RD_DISCRETE_INPUTS :    /* 0x02 ��ֻ��λ */
            exceptCode = com_slv_BMS_rd_input(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_HOLDING_REGISTERS :  /* 0x03 ���ɶ�д�� */
//          break;                  /* 03 04��һ���ӳ����д����ӳ����ڲ����ֹ����� */
        case MB_RD_INPUT_REGISTERS :    /* 0x04 ��ֻ���� */
            exceptCode = com_slv_BMS_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_COIL :        /* 0x05 д����λ */
            exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_REGISTER :    /* 0x06 д������ */
            exceptCode = com_slv_BMS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_COILS :     /* 0x0f д���λ */
            exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_MULTIPLE_REGISTERS : /* 0x10 д����� */
            exceptCode = com_slv_BMS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_USER_DEFINED_DOWNLOAD : /* 0x41 �Զ��壺�������� */
            exceptCode = PDU_SlaveDealUserDef(RxBuf, TxBuf, pTxTotal);
            break;

        default :
            /* һЩ��ȷ�Ĺ����룬�����ݲ�֧�֣�Ҳ��Ϊ�Ƿ������� */
            exceptCode = MB_ILLEGAL_FUNCTION;
            break;
    }

    return exceptCode;
}


/* ���� BMS�ӻ�Ӧ�� */
/****************************************************************
12.3-51 RLANͨѶӦ��(com_slv_RLAN)
���ܣ�RLAN�ӻ�Ӧ��RLAN�Ķ�/д����
����:   RxBuf--���ջ�����;
        TxBuf--���ͻ�����;
        pTxTotal--�跢�͵��ֽ�����;
����: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
����: /
*****************************************************************/
//U08 com_slv_RLAN(U08 *RxBuf, U08 *TxBuf, U16 rxcnt, U16 *pTxTotal)
//{
//  U08 exceptCode = MB_NO_ERR;
//  U16 i;
//
//  /* ���ջ���������һ���ֽ�(ԭ��CANû���豸��ַ�ֽڣ���RxBuf[0]���Ѿ�����Ч����) */
//  for (i=rxcnt; i>0; i--)
//  {
//      RxBuf[i] = RxBuf[i-1];
//  }
//
//  /* ����׼MODBUSЭ�齫�豸��ַ�������븳�����ͻ����� */
//  TxBuf[0] = RxBuf[0];
//  TxBuf[1] = RxBuf[1];
//
//    switch (RxBuf[1])     /* ������ */
//  {
//      case MB_DIAGNOSTICS :           /* 0x08 ��·��� */
//  //          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
//          break;
//
//      case MB_RD_HOLDING_REGISTERS :  /* 0x03 ���ɶ�д�� */
//  //          break;                  /* 03 04��һ���ӳ����д����ӳ����ڲ����ֹ����� */
//      case MB_RD_INPUT_REGISTERS :    /* 0x04 ��ֻ���� */
//          exceptCode = com_slv_RLAN_rd_reg(RxBuf, TxBuf, pTxTotal);
//
//          break;
//
//      case MB_WR_SINGLE_REGISTER :    /* 0x06 д������ */
//          exceptCode = com_slv_RLAN_wr_reg(RxBuf, TxBuf, pTxTotal);
//          break;
//
//      case MB_WR_MULTIPLE_REGISTERS : /* 0x10 д����� */
//          exceptCode = com_slv_RLAN_wr_reg(RxBuf, TxBuf, pTxTotal);
//          break;
//
//      default :
//          /* һЩ��ȷ�Ĺ����룬�����ݲ�֧�֣�Ҳ��Ϊ�Ƿ������� */
//          exceptCode = MB_ILLEGAL_FUNCTION;
//          break;
//  }
//
//  /* ���ͻ�����ǰ��һ���ֽ�(CANû���豸��ַ�ֽ�) */
//  for (i=0; i<*pTxTotal; i++)
//  {
//      TxBuf[i] = TxBuf[i+1];
//  }
//  (*pTxTotal)--;      /* �����ֽڱȱ�׼MODBUS��һ���ֽ� */
//
//  return exceptCode;
//}

/****************************************************************
������com_slv_RLAN_rd_reg
���ܣ�RLAN�ӻ�����RLAN�����������Ķ�����������
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
//U08 com_slv_RLAN_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
//{
//  U16 i;
//  U16 m = 0;
//  U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
//  U16 data_num = 0;
//  U16 data_addr = 0;
//  U16 *src_buf = NULL;
//  U08 mac_addr = 0;
//  U08 EX_addr = 0;
//  U08 com_err = MB_NO_ERR;    /* ����״̬ */
//
//  data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
//  data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
//
//  if (RxBuf[1] == MB_RD_INPUT_REGISTERS)      /* ֻ������ */
//  {
//
//      if (data_addr >= ADDR_MAC_BGN && data_addr + data_num <= ADDR_MAC_END)  /* ϵͳ��Ϣ */
//      {
//          addr_offst = data_addr - ADDR_MAC_BGN;
//          mac_addr = addr_offst / MAC_SIZE;
//          addr_offst = addr_offst % MAC_SIZE;
//  //          src_buf = (U16 *)&mac_sys[mac_addr] + addr_offst;
//          src_buf = (U16 *)pmac + addr_offst;
//      }
//  //      else if (data_addr >= ADDR_HIS_BGN && data_addr + data_num < ADDR_HIS_END)  /* ��ʷ���� */
//  //      {
//  //    //        src_buf = (U16 *)&errormsg[0] + addr_offst;
//  //      }
//  //      else if (data_addr >= ADDR_VER_BGN && data_addr + data_num < ADDR_VER_END)  /* �汾�� */
//  //      {
//  //          addr_offst = data_addr - ADDR_VER_BGN;
//  //          EX_addr = addr_offst / ONE_VER_SIZE;
//  //          addr_offst = addr_offst % ONE_VER_SIZE;
//  //          src_buf = (U16 *)&mac_ver[EX_addr] + addr_offst;
//  //      }
//    }
//  //    else          /* �ɶ�д���� */
//  //    {
//  //      if (data_addr >= ADDR_PARA_USER_BGN && data_addr + data_num < ADDR_PARA_END)    /* ����:BMS��֧���û����� */
//  //      {
//  //          U08 grp, item;
//  //          src_buf = para_get_para_bgnaddr(data_addr, data_num, &grp, &item);
//  //      }
//  //    }
//
//    if (src_buf != NULL)      /* ������� */
//    {
//      U08 *buf = &TxBuf[3];   /* ��Ч�����ڷ��ͻ����е���ʼ��ַ */
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
//  TxBuf[2] = data_num << 1;   /* ���ݸ���Ϊ�����������ֽ��������2 */
//  *pTxTotal = TxBuf[2] + 3;
//
//  return com_err;
//}

/****************************************************************
������com_slv_RLAN_wr_reg
���ܣ�RLAN�ӻ�����RLAN�����������������֡�
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
//U08 com_slv_RLAN_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
//{
//  U16 i, m;
//  U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
//  U16 data_num = 0;
//  U16 data_addr = 0;
//  U16 *dest_buf = NULL;
//  U08 *src_buf = NULL;
//  U16 byte_count = 0;
//  U16 com_err = MB_NO_ERR;
//  BOOL fg_wr_succ = FALSE;    /* �����ⲿ�ӳ���д��״̬��־ */
//
//  data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
//
//  if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* ������0x06: д������  */
//  {
//      data_num = 1;
//      src_buf = &RxBuf[4];
//  }
//  else                            /* ������0x10: д����� */
//  {
//      data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
//      src_buf = &RxBuf[7];
//  }
//  byte_count = data_num << 1; /* ���ݸ���Ϊ�����������ֽ��������2 */
//
//  if (data_addr >= ADDR_R_CMD_BGN && data_addr <= ADDR_R_CMD_END)         /* ��������� */
//  {
//      dest_buf = (U16 *)&pmac->mac_cmd[CMD_NOR_IDX][0];
//  }
//  else if (data_addr >= ADDR_R_EY_BGN && data_addr <= ADDR_R_EY_END)      /* ������������ */
//  {
//      dest_buf = (U16 *)&pmac->mac_ey_need[0];
//  }
//
//    if (dest_buf != NULL || fg_wr_succ)       /* ������� */
//    {
//      if (!fg_wr_succ)    /* û��ͨ���ⲿ�ӳ���д�룬�ڴ˴�д�� */
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
������com_slv_FBUS_rd_reg
���ܣ�������FBUS�������Ķ�����������
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_FBUS_rd_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i;
    U16 m = 0;
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *src_buf = NULL;
//  U08 comp_addr = 0;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */

    if (RxBuf[1] == MB_RD_INPUT_REGISTERS)      /* ֻ������ */
    {
        if (data_addr >= FBUS_VER_BGN && ((data_addr + data_num <= FBUS_VER_END)||(TestProgStaus != TPS_NO_TEST_CMD)))  /* �汾�� */ // ����ʱ���ж����ݸ���
        {
            addr_offst = data_addr - FBUS_VER_BGN;
            src_buf = (U16 *)&idu_mac_ver[0] + addr_offst;
        }
        else if (data_addr >= FBUS_ODU_VER_BGN && (data_addr + data_num <= FBUS_ODU_VER_END))
        {
            addr_offst = data_addr - FBUS_ODU_VER_BGN;
            src_buf = (U16 *)&odu_mac_ver[0] + addr_offst;
        }
        else if (data_addr >= FBUS_MAC_BGN && data_addr + data_num <= FBUS_MAC_END) /* ϵͳ��Ϣ */
        {
            addr_offst = data_addr - FBUS_MAC_BGN;
            src_buf = (U16 *)&cell[0] + addr_offst;
        }
        else if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)   /* ���� */
        {
            addr_offst = data_addr - ADDR_PARA_BGN;
            src_buf = (U16 *)&para_C[0] + addr_offst;
        }
    }
    else            /* �ɶ�д���� */
    {
#if 0
        if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)    /* ���� */
        {
            U08 grp, item;
            src_buf = para_get_para_bgnaddr(data_addr, data_num, &grp, &item);
        }
#endif
    }

    if (src_buf != NULL)        /* ������� */
    {
        U08 *buf = &TxBuf[3];   /* ��Ч�����ڷ��ͻ����е���ʼ��ַ */
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
 * ���� : system_sta_info
 * ���� : ϵͳ״̬��Ϣ����
 *
 * ���� : /
 * ���� : 1/״̬�Ѿ�����
 *        0/״̬û�и���
 *
 * ���� : --
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
 �� ��: rece_bro_special()
 �� ��: �ӻ����գ��㲥��������
 �� ��: buf--;
        addr--;
        num--;
 �� ��:
 �� ��: ��
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

    fg_idle_sta_antifreeze                  = bro_special[0];       /* ����̬���� */
    cell[MAX_PCB].cell_runtime[SYS_RUNTIME] = bro_special[1];       /* �����ۼ�����ʱ�� */
    limit_value                             = bro_special[2];       /* ϵͳά������ */
    limit_type                              = bro_special[3];       /* ϵͳά��ģʽ */
    sv_temp_energy[SUB_SYS_AC_HOTW]         = bro_special[4];
    ctrl_temp_energy[SUB_SYS_AC_HOTW]       = bro_special[5];
    if (power_time > POWER_ON_INTO_WORK+10) /* ��ֹ�ӻ��������� */
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
������com_slv_FBUS_wr_reg
���ܣ��������ӻ�ж������(FBUS)
���������ݻ��棻д���ݵ�ַ��д���ݸ�����
���أ��ɹ�/ʧ��
������/��
*****************************************************************/
U08 com_slv_FBUS_wr_reg(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 i, m;
//  U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
//  U16 addr_offst = 0;
    U16 data_num = 0;
    U16 data_addr = 0;
    U16 *dest_buf = NULL;
    U08 *src_buf = NULL;
    U16 byte_count = 0;
//  U08 cm_num = 0;
    U08 com_err = MB_NO_ERR;    /* ����״̬ */
    BOOL fg_wr_succ = FALSE;    /* �����ⲿ�ӳ���д��״̬��־ */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);   /* ���ݵ�ַ */

    if (RxBuf[1] == MB_WR_SINGLE_REGISTER)      /* ������0x06: д������  */
    {
        data_num = 1;
        src_buf = &RxBuf[4];
    }
    else                            /* ������0x10: д����� */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);    /* ���ݸ��� */
        src_buf = &RxBuf[7];
    }
    byte_count = data_num << 1; /* ���ݸ���Ϊ�����������ֽ��������2 */

    if (data_addr >= FBUS_CMD_BGN && data_addr + data_num <= FBUS_CMD_END)/* �������� */
    {
        dest_buf = (U16 *)&cell[pcb_addr].cell_cmd[0];
    }
    else if (data_addr >= FBUS_SYS_STA_BGN && data_addr + data_num <= FBUS_SYS_STA_END)/* ϵͳ״̬ */
    {
        fg_wr_succ = system_sta_info(src_buf, data_addr, byte_count, _SET_);
    }
    else if (data_addr >= ADDR_PARA_BGN && data_addr + data_num <= ADDR_PARA_END)   /* ���� */
    {
        fg_wr_succ = PARA_Modify(src_buf, data_addr, byte_count);
    }
    else if (data_addr >= ADDR_PASS_BGN && data_addr + data_num <= ADDR_PASS_END)   /* ���� */
    {
        fg_wr_succ = password_information(src_buf, data_addr, byte_count, _SET_);
    }
    else if (data_addr >= ADDR_SPECIAL_BGN && data_addr + data_num <= ADDR_SPECIAL_END) /* �㲥�������� */
    {
        fg_wr_succ = rece_bro_special(src_buf, data_addr, byte_count);
    }

    if (dest_buf != NULL || fg_wr_succ)     /* ������� */
    {
        if (!fg_wr_succ)    /* û��ͨ���ⲿ�ӳ���д�룬�ڴ˴�д�� */
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

/* S-EX�� FBUS�ӻ�Ӧ�� */
/****************************************************************
������12.3-71 FBUSͨѶӦ��(com_slv_FBUS)
���ܣ�����Ӧ��BMS�Ķ�/д����
����:   RxBuf--���ջ�����;
        TxBuf--���ͻ�����;
        pTxTotal--�跢�͵��ֽ�����;
����: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
����: /
*****************************************************************/
U08 com_slv_FBUS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = MB_NO_ERR;

    switch (RxBuf[1])
    {
        case MB_DIAGNOSTICS :           /* 0x08 ��·��� */
//          exceptCode = PDU_SlaveDealDiag(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_RD_HOLDING_REGISTERS :  /* 0x03 ���ɶ�д�� */
//          break;                  /* 03 04��һ���ӳ����д����ӳ����ڲ����ֹ����� */
        case MB_RD_INPUT_REGISTERS :    /* 0x04 ��ֻ���� */
            exceptCode = com_slv_FBUS_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

//      case MB_WR_SINGLE_COIL :        /* 0x05 д����λ */
//          exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
//          break;

        case MB_WR_SINGLE_REGISTER :    /* 0x06 д������ */
            exceptCode = com_slv_FBUS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

//      case MB_WR_MULTIPLE_COILS :     /* 0x0f д���λ */
//          exceptCode = com_slv_BMS_wr_coil(RxBuf, TxBuf, pTxTotal);
//          break;

        case MB_WR_MULTIPLE_REGISTERS : /* 0x10 д����� */
            exceptCode = com_slv_FBUS_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        default :
            /* һЩ��ȷ�Ĺ����룬�����ݲ�֧�֣�Ҳ��Ϊ�Ƿ������� */
            exceptCode = MB_ILLEGAL_FUNCTION;
            break;
    }

    return exceptCode;
}



