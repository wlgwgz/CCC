/******************************************************************************

          版权所有 (C), 2001-2021, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : HmiRoutine.c
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2021年4月20日
  最近修改   :
  功能描述   : 文本/断码显示屏调度处理
  函数列表   :
  修改历史   :
  1.日    期   : 2021年4月20日
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/

/*-----------------包含头文件 -----------------------------*/
#define HMIROUTINE_GBL
#include "includes.h"


/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/


/* 回送协议码,0xFx地址有 */
#define PRTC_CODE_NONE      0       // 无
#define PRTC_CODE_NORMAL    1       // 常规数据
#define PRTC_CODE_TY_WIFI   2       // 涂鸦WiFi数据

/* 接收类型 */
#define RECV_TYPE_UNKNOW        0   // 未知
#define RECV_TYPE_NORMAL        1   // 常规地址
#define RECV_TYPE_NEW           2   // 0xFx新地址
    

/*******************************************************************************
 函 数: hmi_routine_init()
 功 能: HMI调度初始化(取代原Cloud.c的cloud_intial()中的部分初始化操作)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void hmi_routine_init(void)
{
    ARRSET(hmi_board_type, OLD);
    hmi_trans_type = HMI_DISPLAY;
}

/*****************************************************************************
函  数 : hmi_err_deal()
功  能 : HMI通信故障处理,清除一些同步标志
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void hmi_err_deal(void)
{
    if (GET_BIT(hmi_commu_error, hmi_t_num))
    {
        hmi_bk_lose_conn[hmi_t_num] = TRUE;     // 通信故障就认为断开连接
        hmi_info_sync_flag[hmi_t_num]  = FALSE;
        hmi_info_sup_0xFx[hmi_t_num]   = FALSE;
        hmi_info_dim_dcache[hmi_t_num] = FALSE;
        hmi_info_dim_scache[hmi_t_num] = FALSE;
    }
}

/************************************************
函数：hmi_deal_request
功能：显示数据通讯
参数：
返回值：正确/错误
**************************************************/
U08 hmi_deal_request(UART *pUart)
{
	U08 rece_succ = 0;
	U08 real_rece_num = 0;
	U16 i, j;
	
//	pUart->Bus_Message_Count++;		/* 报文总和 */

	if (pUart->rxCnt > 4)
	{
		for (i=0; i<1; i++)
//		for (i=0; i<pUart->rece_cnt-4; i++)		/* 有效数据帧最少5个字节 */
		{
			if ((pUart->rxBuf[i] & HMI_ADDR_MASK) == pUart->ownAddr)	/* 地址匹配 */
			{
    			real_rece_num = pUart->rxBuf[i+1];		/* 赋初值(帧头为本帧数据个数) */
    			if (real_rece_num == pUart->rxCnt - i - 2)
//    			if (real_rece_num <= pUart->rece_cnt - i)
    			{
    				if (crc_check(&pUart->rxBuf[i], real_rece_num+2))
    				{
    					if (i != 0)
    					{
    						for (j=0; j<real_rece_num; j++)
    						{
    							pUart->rxBuf[j] = pUart->rxBuf[i+j];	/* 有效数据重新赋值 */
    						}
    					}
    					pUart->rxCnt = real_rece_num;
    					
    					rece_succ = 1;	
    					break;
    				}
        		}
    			if (rece_succ != 0)		break;			/* 退出外层循环 */
    		}
		}
	}

//	if (rece_succ == 0)		pUart->Bus_CRC_Error_Count++;	/* 报文CRC错误 */

	pUart->commuErr = COMMU_NO_ERR;
    
	// 增加显示屏通讯记录
	if (rece_succ == 1)
	{
		pUart->pRecord->frameOk++;/* wjs 如何区分frameOkAhead frameOkBack */
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
函数：hmi_rece
功能：显示数据接收
参数：
返回值：正确/错误
**************************************************/
BOOL hmi_rece(UART *pUart)
{
	BOOL fg_rece_succ = FALSE;
    U08 hmi_addr_type;
  	U08 *data_bgn=NULL;			/* 数据开始地址 */
    U08 data_size;              /* 有效数据个数,不包括地址,长度,CRC,等 */

    
	U08 recv_data_type = RECV_TYPE_UNKNOW;
    U08 prtc_code = PRTC_CODE_NONE;  /* 协议码 */
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
            hmi_r_num = 0;		/* 防错 */
        }

        hmi_type[hmi_r_num] =  HMI_TYPE_TEXT_NORMAL;
            
        for (idx=0; idx<data_size; idx++)
        {
            U08 d = data_bgn[idx];
            switch (idx)
            {
                case 0: key_value[hmi_r_num]  = (U16)d<<8;     break;
                case 1: key_value[hmi_r_num] |= (U16)d;        break;
/* 段码屏 */
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
                    if (have_ext_info)   // 解析扩展信息
                    {
                        hmi_info_sync_flag[hmi_r_num]  = GETBIT(d, 0); 
                        hmi_info_sup_0xFx[hmi_r_num]   = GETBIT(d, 1);   
                        hmi_info_dim_dcache[hmi_r_num] = GETBIT(d, 2);   
                        hmi_info_dim_scache[hmi_r_num] = GETBIT(d, 3);   
                    }
                    break;
                default:    // 根据具体信息再后面解析
                    break;
            }

            /* 涂鸦信息(续) */
            if (hmi_is_ty)
            {
                switch (idx)
                {
                    case 18:    // 解析涂鸦同步信息
                        hmi_info_conn_bk_sig[hmi_r_num]     = GETBIT(d, 0);   
                        hmi_info_need_sync_all[hmi_r_num]   = GETBIT(d, 1);   
                        hmi_info_pid_valid[hmi_r_num]       = GETBIT(d, 2);
                        break;
                    case 19:    // 涂鸦状态
                        hmi_ty_status = d;   
                        break;
                    case 20:    // 涂鸦信号
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
            disp_read_key();			/* 文本屏识别按键 */
            break;
/* 段码屏 */
#ifdef _SEGHMI_H_
        case HMI_TYPE_SEG_NORMAL:
        case HMI_TYPE_SEG_WIFI_YHFY:
        case HMI_TYPE_SEG_WIFI_TY:
            SegHMI_read_key();		/* 断码屏按键识别 */
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
函数：hmi_trans
功能：显示数据通讯
参数：串口
返回值：无
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
    	/* 故障后定时尝试通讯，防止只接一个终端时速度变慢 */
    	for (i=0; i<MAX_HMI; i++)
    	{
    		if (++hmi_addr > HMI_2_ADDR)			hmi_addr = HMI_ADDR;
    		hmi_addr_diff = hmi_addr - HMI_ADDR;
    		
    		if (commu_err_num < MAX_HMI			/* 没有全部故障 */
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
        commu_status = 0x80;		/* bit7作为是否匹配成功标志(HMI数量大于2时，该条件可替换) */
    }
    commu_status |= hmi_commu_error & 0x7f;	/* 通讯故障和匹配标志用一个字节，所以HMI个数不可大于7个 */

    // 通信故障处理
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
            
        /* 涂鸦Wi-Fi文本屏 */
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

/* 段码屏 */
#ifdef _SEGHMI_H_
        /* 普通段码屏 */
        case HMI_TYPE_SEG_NORMAL:
			total = SegHmi_routine(&pUart->txBuf[4]);
			total += 2;
			pUart->txBuf[2] = hmi_trans_type;
			pUart->txBuf[3] = commu_status;
            break;
        
        /* 银河风云Wi-Fi段码屏 */
        case HMI_TYPE_SEG_WIFI_YHFY:
            total = SegHmi_routine(&pUart->txBuf[4]);
            total += 2;
            pUart->txBuf[2] = hmi_trans_type;
            pUart->txBuf[3] = commu_status;
            break;
        
        /* 涂鸦Wi-Fi段码屏 */
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

    total += 2; // 包括地址和长度
    pUart->txBuf[0] = hmi_addr;
    pUart->txBuf[1] = total;
	crc.word = CRC16(&pUart->txBuf[0], total);
	pUart->txBuf[total++] = crc.byte.low;
	pUart->txBuf[total++] = crc.byte.high;

	pUart->txTotal = total;
	
	return 1;
}

