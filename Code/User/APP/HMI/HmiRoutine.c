/******************************************************************************

          ��Ȩ���� (C), 2001-2021, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : HmiRoutine.c
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2021��4��20��
  ����޸�   :
  ��������   : �ı�/������ʾ�����ȴ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2021��4��20��
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/

/*-----------------����ͷ�ļ� -----------------------------*/
#define HMIROUTINE_GBL
#include "includes.h"


/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/


/* ����Э����,0xFx��ַ�� */
#define PRTC_CODE_NONE      0       // ��
#define PRTC_CODE_NORMAL    1       // ��������
#define PRTC_CODE_TY_WIFI   2       // ͿѻWiFi����

/* �������� */
#define RECV_TYPE_UNKNOW        0   // δ֪
#define RECV_TYPE_NORMAL        1   // �����ַ
#define RECV_TYPE_NEW           2   // 0xFx�µ�ַ
    

/*******************************************************************************
 �� ��: hmi_routine_init()
 �� ��: HMI���ȳ�ʼ��(ȡ��ԭCloud.c��cloud_intial()�еĲ��ֳ�ʼ������)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void hmi_routine_init(void)
{
    ARRSET(hmi_board_type, OLD);
    hmi_trans_type = HMI_DISPLAY;
}

/*****************************************************************************
��  �� : hmi_err_deal()
��  �� : HMIͨ�Ź��ϴ���,���һЩͬ����־
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void hmi_err_deal(void)
{
    if (GET_BIT(hmi_commu_error, hmi_t_num))
    {
        hmi_bk_lose_conn[hmi_t_num] = TRUE;     // ͨ�Ź��Ͼ���Ϊ�Ͽ�����
        hmi_info_sync_flag[hmi_t_num]  = FALSE;
        hmi_info_sup_0xFx[hmi_t_num]   = FALSE;
        hmi_info_dim_dcache[hmi_t_num] = FALSE;
        hmi_info_dim_scache[hmi_t_num] = FALSE;
    }
}

/************************************************
������hmi_deal_request
���ܣ���ʾ����ͨѶ
������
����ֵ����ȷ/����
**************************************************/
U08 hmi_deal_request(UART *pUart)
{
	U08 rece_succ = 0;
	U08 real_rece_num = 0;
	U16 i, j;
	
//	pUart->Bus_Message_Count++;		/* �����ܺ� */

	if (pUart->rxCnt > 4)
	{
		for (i=0; i<1; i++)
//		for (i=0; i<pUart->rece_cnt-4; i++)		/* ��Ч����֡����5���ֽ� */
		{
			if ((pUart->rxBuf[i] & HMI_ADDR_MASK) == pUart->ownAddr)	/* ��ַƥ�� */
			{
    			real_rece_num = pUart->rxBuf[i+1];		/* ����ֵ(֡ͷΪ��֡���ݸ���) */
    			if (real_rece_num == pUart->rxCnt - i - 2)
//    			if (real_rece_num <= pUart->rece_cnt - i)
    			{
    				if (crc_check(&pUart->rxBuf[i], real_rece_num+2))
    				{
    					if (i != 0)
    					{
    						for (j=0; j<real_rece_num; j++)
    						{
    							pUart->rxBuf[j] = pUart->rxBuf[i+j];	/* ��Ч�������¸�ֵ */
    						}
    					}
    					pUart->rxCnt = real_rece_num;
    					
    					rece_succ = 1;	
    					break;
    				}
        		}
    			if (rece_succ != 0)		break;			/* �˳����ѭ�� */
    		}
		}
	}

//	if (rece_succ == 0)		pUart->Bus_CRC_Error_Count++;	/* ����CRC���� */

	pUart->commuErr = COMMU_NO_ERR;
    
	// ������ʾ��ͨѶ��¼
	if (rece_succ == 1)
	{
		pUart->pRecord->frameOk++;/* wjs �������frameOkAhead frameOkBack */
	}
	else
	{
		if (pUart->rxCnt > 4)
		{
			pUart->pRecord->frameDataErr++;
		}
		else
		{
			pUart->pRecord->fLenShortage++;
		}
	}

    return rece_succ;
}

void hmi_rece_tuya(U08* Buf)
{
    TYB_parse_tuya_data(Buf);
}


/************************************************
������hmi_rece
���ܣ���ʾ���ݽ���
������
����ֵ����ȷ/����
**************************************************/
BOOL hmi_rece(UART *pUart)
{
	BOOL fg_rece_succ = FALSE;
    U08 hmi_addr_type;
  	U08 *data_bgn=NULL;			/* ���ݿ�ʼ��ַ */
    U08 data_size;              /* ��Ч���ݸ���,��������ַ,����,CRC,�� */

    
	U08 recv_data_type = RECV_TYPE_UNKNOW;
    U08 prtc_code = PRTC_CODE_NONE;  /* Э���� */
	#define DATA_BGN		2


    
	fg_rece_succ = hmi_deal_request(pUart);
	if (fg_rece_succ)
	{

        hmi_addr_type = pUart->rxBuf[0] & (~HMI_ADDR_MASK);

        switch (hmi_addr_type) 
        {
            case 0x40:
                recv_data_type = RECV_TYPE_NORMAL;
                data_size = pUart->rxBuf[1]-2;
                data_bgn = &pUart->rxBuf[2];
                break;
            case 0xC0:
                recv_data_type = RECV_TYPE_NEW;
                data_size = pUart->rxBuf[1]-3;
                prtc_code = pUart->rxBuf[2];
                data_bgn = &pUart->rxBuf[3];
                break;
        }
	}
    
    if (recv_data_type == RECV_TYPE_NORMAL
        || (recv_data_type == RECV_TYPE_NEW && prtc_code == PRTC_CODE_NORMAL))
    {
        U08 idx = 0;
//        U08 ext_info_cmd = 0;
        U08 have_ext_info = FALSE;
//        U08 hmi_is_text=FALSE;
//        U08 hmi_is_seg=FALSE;
//        U08 hmi_is_yhfy=FALSE;
        U08 hmi_is_ty=FALSE;
        hmi_r_num = (pUart->rxBuf[0] & HMI_ADDR_MASK) - HMI_ADDR;
        if (hmi_r_num >= MAX_HMI)		
        {
            hmi_r_num = 0;		/* ���� */
        }

        hmi_type[hmi_r_num] =  HMI_TYPE_TEXT_NORMAL;
            
        for (idx=0; idx<data_size; idx++)
        {
            U08 d = data_bgn[idx];
            switch (idx)
            {
                case 0: key_value[hmi_r_num]  = (U16)d<<8;     break;
                case 1: key_value[hmi_r_num] |= (U16)d;        break;
/* ������ */
#ifdef _SEGHMI_H_
                case 2: 
					ch_end_f[hmi_r_num]   = (U16)d<<8;  
					front_ver[0] = d;
					break;
                case 3: 
					ch_end_f[hmi_r_num]  |= (U16)d; 
					front_ver[1] = d;
					break;
                case 4: 
					bmp_end_f[hmi_r_num]  = (U16)d<<8;
					front_ver[2] = d;
					break;
                case 5: 
					bmp_end_f[hmi_r_num] |= (U16)d; 
					front_ver[3] = d;
					break;
#else
                case 2: ch_end_f[hmi_r_num]   = (U16)d<<8;     break;
                case 3: ch_end_f[hmi_r_num]  |= (U16)d;        break;
                case 4: bmp_end_f[hmi_r_num]  = (U16)d<<8;     break;
                case 5: bmp_end_f[hmi_r_num] |= (U16)d;        break;
#endif
                case 6: 
                    commu_sucess[hmi_r_num] = d;
                    if (d == 10)
                    {
                        hmi_type[hmi_r_num] =  HMI_TYPE_SEG_NORMAL;
                    }
                    break;
        		case  7: clock[hmi_r_num].sec   = system_time.sec   = d;       break;
        		case  8: clock[hmi_r_num].min   = system_time.min   = d;       break;
        		case  9: clock[hmi_r_num].hour  = system_time.hour  = d;       break;
        		case 10: clock[hmi_r_num].day   = system_time.day   = d;       break;
        		case 11: clock[hmi_r_num].month = system_time.month = d;       break;
        		case 12: clock[hmi_r_num].week  = system_time.week  = d;       break;
        		case 13: clock[hmi_r_num].year  = system_time.year  = d+2000;  break;
                case 14: if(d != 0)     fg_disp_back[hmi_r_num] = TRUE;	       break; 
                case 15: if(d >= 0xA5)  fg_disp_ver_back[hmi_r_num] = TRUE;    break;
                case 16: 
                    switch (d) 
                    {
                        case NORMAL_DATA:
                        case SELECTED_DATA:
                            hmi_board_type[hmi_r_num] = NEW;
                            hmi_type[hmi_r_num] =  HMI_TYPE_TEXT_WIFI_YHFY;
                            break;
                        case 0x81:
                            have_ext_info = TRUE;
                            hmi_type[hmi_r_num] =  HMI_TYPE_TEXT_NORMAL;
                            break;
                        case 0x83:
                            hmi_board_type[hmi_r_num] = OLD;
                            hmi_is_ty = TRUE;
                            have_ext_info = TRUE;
                            hmi_type[hmi_r_num] =  HMI_TYPE_TEXT_WIFI_TY;
                            break;
						case 0x84:
                            have_ext_info = TRUE;
                            hmi_type[hmi_r_num] = HMI_TYPE_SEG_NORMAL;
							break;
                        case 0x86:
                            hmi_is_ty = TRUE;
                            have_ext_info = TRUE;
                            hmi_type[hmi_r_num] = HMI_TYPE_SEG_WIFI_TY;
                            break;
                    }
                    have_ext_info = (d & 0x80)!=0;
                    break;     
                case 17:
                    if (have_ext_info)   // ������չ��Ϣ
                    {
                        hmi_info_sync_flag[hmi_r_num]  = GETBIT(d, 0); 
                        hmi_info_sup_0xFx[hmi_r_num]   = GETBIT(d, 1);   
                        hmi_info_dim_dcache[hmi_r_num] = GETBIT(d, 2);   
                        hmi_info_dim_scache[hmi_r_num] = GETBIT(d, 3);   
                    }
                    break;
                default:    // ���ݾ�����Ϣ�ٺ������
                    break;
            }

            /* Ϳѻ��Ϣ(��) */
            if (hmi_is_ty)
            {
                switch (idx)
                {
                    case 18:    // ����Ϳѻͬ����Ϣ
                        hmi_info_conn_bk_sig[hmi_r_num]     = GETBIT(d, 0);   
                        hmi_info_need_sync_all[hmi_r_num]   = GETBIT(d, 1);   
                        hmi_info_pid_valid[hmi_r_num]       = GETBIT(d, 2);
                        break;
                    case 19:    // Ϳѻ״̬
                        hmi_ty_status = d;   
                        break;
                    case 20:    // Ϳѻ�ź�
                        hmi_ty_signal = d;   
                        break;
                    case 21:    hmi_ty_mac[0] = d;  break;
                    case 22:    hmi_ty_mac[1] = d;  break;
                    case 23:    hmi_ty_mac[2] = d;  break;
                    case 24:    hmi_ty_mac[3] = d;  break;
                    case 25:    hmi_ty_mac[4] = d;  break;
                    case 26:    hmi_ty_mac[5] = d;  break;
                }
            }
            
        }
    }
    else if (recv_data_type == RECV_TYPE_NEW && prtc_code == PRTC_CODE_TY_WIFI)
    {
        key_value[hmi_r_num] = MAKEWORD(data_bgn[0], data_bgn[1]);
        hmi_rece_tuya(&data_bgn[2]);
    }

    switch (hmi_type[hmi_r_num])
    {
        case HMI_TYPE_TEXT_NORMAL:
        case HMI_TYPE_TEXT_WIFI_YHFY:
        case HMI_TYPE_TEXT_WIFI_TY:
            disp_read_key();			/* �ı���ʶ�𰴼� */
            break;
/* ������ */
#ifdef _SEGHMI_H_
        case HMI_TYPE_SEG_NORMAL:
        case HMI_TYPE_SEG_WIFI_YHFY:
        case HMI_TYPE_SEG_WIFI_TY:
            SegHMI_read_key();		/* ����������ʶ�� */
            break;
#endif
    }    
    return fg_rece_succ;
}

BOOL hmi_trans_tuya(U08* Buf)
{
    BOL is_trans_data = FALSE;

    if (hmi_info_need_sync_all[hmi_t_num])
    {
        hmi_info_need_sync_all[hmi_t_num] = FALSE;
        TYB_SET_FG_DPID_UPDATE_ALL;
    }

    if (Main_Key[hmi_t_num] != DM603_KEY_NULL
        || hmi_trans_type == HMI_DISPLAY)
    {
        is_trans_data = FALSE;
    }
    else
    {
        disp_trans_init();

        if (hmi_info_conn_bk_sig[hmi_t_num]
        || !hmi_info_pid_valid[hmi_t_num]
        || !hmi_info_sync_flag[hmi_t_num]
        || hmi_bk_lose_conn[hmi_t_num])
        {
            hmi_info_conn_bk_sig[hmi_t_num] = FALSE;
	        disp_trans_sync(Buf);
	        disp_trans_ty_info(Buf);
            is_trans_data = TRUE;
        }
        else
        {
            U08 dpid_idx = TYB_SLP_select_upload_dpid_idx();
            if (TYB_is_dpid_idx_valid(dpid_idx))
            {
	            disp_trans_ty_data(Buf, dpid_idx);
                is_trans_data = TRUE;
            }
        }
	}

    return is_trans_data;
}
/************************************************
������hmi_trans
���ܣ���ʾ����ͨѶ
����������
����ֵ����
**************************************************/
BOOL hmi_trans(UART *pUart)
{
	W16  crc;
	U08 total = 0;
	static U08 hmi_addr = HMI_ADDR;
	U08 commu_status = 0;
	U08 commu_err_num = 0;
	U08 i;
	
	for (i=0; i<MAX_HMI; i++)
	{
		if (GETBIT(hmi_commu_error, i) != 0)	commu_err_num++;
	}

    #if MAX_HMI==2
	{
        U08	hmi_addr_diff = 0;
    	/* ���Ϻ�ʱ����ͨѶ����ֹֻ��һ���ն�ʱ�ٶȱ��� */
    	for (i=0; i<MAX_HMI; i++)
    	{
    		if (++hmi_addr > HMI_2_ADDR)			hmi_addr = HMI_ADDR;
    		hmi_addr_diff = hmi_addr - HMI_ADDR;
    		
    		if (commu_err_num < MAX_HMI			/* û��ȫ������ */
    			&& GETBIT(hmi_commu_error, hmi_addr_diff) != 0)
    		{
    			if (cnt_hmi_com_wait[hmi_addr_diff] >= HMI_ERR_COM_DLY)
    			{
    				cnt_hmi_com_wait[hmi_addr_diff] = 0;
    				break;
    			}
    		}
    		else												
    		{
    			break;
    		}
    	}
    }
    #else
    {
    	hmi_addr = HMI_ADDR;
	}
    #endif
    
	pUart->ownAddr = hmi_addr;
	hmi_t_num = hmi_addr - HMI_ADDR;
    if (commu_err_num >= MAX_HMI)	
    {
        commu_status = 0x80;		/* bit7��Ϊ�Ƿ�ƥ��ɹ���־(HMI��������2ʱ�����������滻) */
    }
    commu_status |= hmi_commu_error & 0x7f;	/* ͨѶ���Ϻ�ƥ���־��һ���ֽڣ�����HMI�������ɴ���7�� */

    // ͨ�Ź��ϴ���
    hmi_err_deal(); 
    
    switch (hmi_type[hmi_t_num])
    {
        default:
        case HMI_TYPE_TEXT_NORMAL:      
        	total = disp_routine(&pUart->txBuf[3]);
            pUart->txBuf[2] = commu_status;
            total += 1;			
            break;
        case HMI_TYPE_TEXT_WIFI_YHFY:   
                total = disp_routine(&pUart->txBuf[4]);
    			pUart->txBuf[2] = HMI_DISPLAY;
    			pUart->txBuf[3] = commu_status;	
                total += 2;	
            break;
            
        /* ͿѻWi-Fi�ı��� */
        case HMI_TYPE_TEXT_WIFI_TY:
            if (hmi_trans_tuya(&pUart->txBuf[3]))
            {
                total += buf_index;
                hmi_trans_type = HMI_DISPLAY;
            }
            else
            {
                total += disp_routine(&pUart->txBuf[3]);
                hmi_trans_type = HMI_DATA;
            }
            pUart->txBuf[2] = commu_status;
	        total += 1;
            break;

/* ������ */
#ifdef _SEGHMI_H_
        /* ��ͨ������ */
        case HMI_TYPE_SEG_NORMAL:
			total = SegHmi_routine(&pUart->txBuf[4]);
			total += 2;
			pUart->txBuf[2] = hmi_trans_type;
			pUart->txBuf[3] = commu_status;
            break;
        
        /* ���ӷ���Wi-Fi������ */
        case HMI_TYPE_SEG_WIFI_YHFY:
            total = SegHmi_routine(&pUart->txBuf[4]);
            total += 2;
            pUart->txBuf[2] = hmi_trans_type;
            pUart->txBuf[3] = commu_status;
            break;
        
        /* ͿѻWi-Fi������ */
        case HMI_TYPE_SEG_WIFI_TY:
			if (hmi_trans_tuya(&pUart->txBuf[4]))
			{
                total += buf_index;
                hmi_trans_type = HMI_DISPLAY;
			}
            else
            {
                total += SegHmi_routine(&pUart->txBuf[4]);
                hmi_trans_type = HMI_DATA;
            }
            pUart->txBuf[2] = hmi_trans_type;
            pUart->txBuf[3] = commu_status;
            total += 2;
            break;
#endif
    }

    total += 2; // ������ַ�ͳ���
    pUart->txBuf[0] = hmi_addr;
    pUart->txBuf[1] = total;
	crc.word = CRC16(&pUart->txBuf[0], total);
	pUart->txBuf[total++] = crc.byte.low;
	pUart->txBuf[total++] = crc.byte.high;

	pUart->txTotal = total;
	
	return 1;
}

