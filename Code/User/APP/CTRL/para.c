/***********************************************************************************
         design by punp co.ltd copyright limited
         desinger
         date :
         name : para.c
***********************************************************************************/
#define PARA_GLOBALS
#include "includes.h"

#if defined(M_DEBUG)
    #include "para_debug.c"
#elif defined(M_TCL)
    #include "para_tcl.c"
#elif defined(M_CHANGHONG)
    #include "para_changhong.c"
#elif defined(M_TONGYONG)
    #include "para_tongyong.c"
#elif defined(M_DISEN)
    #include "para_disen.c"
#elif defined(M_PAIWO)
    #include "para_paiwo.c"
#elif defined(M_NIUENTAI)
    #include "para_niuentai.c"
#elif defined(M_SHENGNENG)
    #include "para_shengneng.c"
#elif defined(M_HONGHE)
    #include "para_honghe.c"
#elif defined(M_WANHE)
    #include "para_wanhe.c"
#elif defined(M_CHANGFAN)
    #include "para_changfan.c"
#endif

/**ʹ�����޹���**************************************************/

const PARAM para_item_90[] =    /* ϵͳά�� */
{
    {   0  ,        0  ,        1365 ,      UINT_D,     },   /* ϵͳά��(��) */
    {   0  ,        0  ,        1 ,         UINT_0,     },   /* ϵͳά������ */
};

PARAMITEM  ParamLimit[] =
{
    { sizeof(para_item_90)/sizeof(PARAM),  (PARAM *)para_item_90,  limit_para_C,  limit_para_F,  0x80,  0,  0,  D_USER          },
};
const EEDATA Limit_DataInfo={MEM_LIMIT_ADDR, (MAX_LMT<<1)};


/*=================================================================================================
                    �¶Ȳ�ͬ��λ�Ļ���
/=================================================================================================*/
/*******************************************************************************
 �� ��: F_To_C()
 �� ��: �ɻ���ת������C = (F-32)*5/9
 �� ��: F--�����¶�;
		sign--;
 �� ��: �����¶�(0.1)
 �� ��: ��
*******************************************************************************/
I16 F_To_C(I16 F, U16 sign)
{
    U16  shift;
    U16  opt;
    long  C;

    if ((sign & UINT_MASK) != UINT_C
    	|| SensorCannotUse(F)
    	|| F == -999)
	{
		return  F;
	}

    shift = sign & D_DOTMASK;
    opt   = sign & D_DE;

    if (opt == 0) F -= 32*pow(10, shift);

    C = fun_div_round((I32)F*5, 9, DIV_ROUND);

    return (int)C;
}

/*******************************************************************************
 �� ��: C_To_F()
 �� ��: ������ת�ɻ���F = 9*C/5 +32;
 �� ��: C--�����¶�;
		sign--;
 �� ��: �����¶�(0.1)
 �� ��: ��
*******************************************************************************/
I16 C_To_F(I16 C, U16 sign)
{
    U16  shift;
    U16  opt;
    long  F;

    if ((sign & UINT_MASK) != UINT_C
    	|| SensorCannotUse(C)
    	|| C == -999)
	{
		return  C;
	}

    shift = sign & D_DOTMASK;
    opt   = sign & D_DE;

    F = fun_div_round((I32)C*9, 5, DIV_ROUND);

    if (opt == 0) F += 32*pow(10, shift);

    return (int)F;
}

///**********************************************
//������GetSize_Byte
//���ܣ���ȡ������һ�����ݰ���/������CRC�Ŀռ��С
//������DataSize_byte�����ݵ��ֽ�����opt���Ƿ����CRC��С
//����ֵ����ַ
//˵�����ô˺�����Ϊ�˷����Ժ����CRC�������
//**********************************************/
#define NO_CRC      0   // ������CRC�Ĵ�С
#define WITH_CRC    1   // ����CRC

static U16 GetSize_Byte(U16 DataSize_byte, U08 opt)
{
    U16 Size_byte, CRC_size;  // �ֽڴ�С


    CRC_size = (WITH_CRC==opt) ? (CRC_SIZE<<1) : 0 ;
    Size_byte = DataSize_byte + CRC_size;

    return Size_byte;
}

///**********************************************
//������DataRead_OneParaG
//���ܣ���ȡ���������������ִ�н��
//������ParamGroup_act��Ҫ��ȡ�Ĳ�����ָ�룻 DataInfo��������洢��Ϣ����ʼ��ַ��������
//����ֵ��TRUE/FALSE �ɹ�/ʧ��
//**********************************************/
static U08 DataRead_OneParaG(const PARAMITEM *ParamGroup_act, EEDATA DataInfo)
{
    U08 *ptr;

    ptr = (U08 *)ParamGroup_act->data_f;                /* ��������ָ�� */

    return eprom_rd_near_crc(ptr, DataInfo.Addr, DataInfo.DataNum);
}


///**********************************************
//������DataRead_ParaGroup
//���ܣ���ȡָ����Ĳ�����������ִ�н��
//������GroupBegin����ʼ��; GroupNum������ʵ�����; GroupAllNum ʵ������; pDataInfo���洢�׵�ַ��ÿ��Ԥ�������ֽڸ���
//����ֵ��FALSE-��ȡʧ�ܣ�TRUE-��ȡ�ɹ�
//**********************************************/
U08 DataRead_ParaGroups(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, U08 GroupAllNum, const EEDATA *pDataInfo)    //NOTE-CXW ����2017.4.11 �޸��˵�ַ���ֽڼ���
{
    U08 i, j;
    U08 fg_ReadOK= TRUE;    // ��ȡ�ɹ���FALSE-��ȡʧ�ܣ�TRUE-��ȡ�ɹ���
    U08 RepeatEnd=0, itemNum=0;

    PARAMITEM *pGroup;      // ������
    PARAM *pp2;             // ��������
    I16  *pp1_f;            // ����
    I16  *pp1_c;            // ����
    EEDATA DataInfoAll;     // ��Ƭ���ݴ洢��Ϣ
    EEDATA DataInfoEach;    // ��������洢��Ϣ

    RepeatEnd = GroupNum+GroupBegin;        // ����

    DataInfoAll.Addr     = pDataInfo->Addr;
    DataInfoAll.DataNum  = GetSize_Byte(pDataInfo->DataNum, WITH_CRC);      // ÿ��Ԥ���ֽڸ���������CRC��

    for (i=GroupBegin; i<RepeatEnd; i++)
    {
        if(i>=GroupAllNum)  // Ҫ���ʵ��鳬����ʵ�ʵ����������ز�����
        {
            Unknown_Exception[DATA_EXCEPTION] = DATA_BREAK_BOUNDS;
            break;
        }
        wdg_clear();
        pGroup = (PARAMITEM*)&ParamGroup[i];
        pp2 = (PARAM *)pGroup->array;
        itemNum = pGroup->items;
        pp1_c  = (I16 *)pGroup->data_c;
        pp1_f  = (I16 *)pGroup->data_f;

        DataInfoEach.Addr = DataInfoAll.Addr+i*DataInfoAll.DataNum;     // ��������洢��ַ
        DataInfoEach.DataNum = itemNum<<1;                              // �������ʵ��ʹ���ֽڸ�����������CRC��

        if(DataInfoEach.DataNum > pDataInfo->DataNum)  // Ҫ���ʵĲ���������������Ԥ���Ĳ����������CRC�������ز�����
        {
            Unknown_Exception[DATA_EXCEPTION] = DATA_BREAK_BOUNDS;
            break;
        }
        if (DataRead_OneParaG(pGroup,DataInfoEach) == FALSE)
        {
//            fg_seep_crc_iderr |= 0x0001<<i;
            fg_seep_crc_iderr = i+1;
            fg_ReadOK = FALSE;

        }

        for (j=0; j<itemNum; j++)
        {
            pp1_c[j] = F_To_C(pp1_f[j], pp2[j].sign);   // ת������
        }
    }

    return fg_ReadOK;
}

/*******************************************************************************
 �� ��: set_para_modify_flag()
 �� ��: ��ָ����������޸ı�ʶ����ȷ���޸ķ�Χ
 �� ��: n--;
		m--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void set_para_modify_flag(U08 n, U08 m)
{
    if (ParamPri[n].chip & PARA_NEED_STORE) //
    {
        if (m < ParamPri[n].head) ParamPri[n].head = m;
        if (m >= ParamPri[n].end) ParamPri[n].end  = m + 1;
    }
    else
    {
        ParamPri[n].head = m;
        ParamPri[n].end  = m+1;
    }
    ParamPri[n].chip = 0xff;
}

/*********************************************
������DataReset_ParaArray
���ܣ���ʼ��������ʱ����
������ParamGroup-Ҫ��ʼ���Ĳ������ַ, opt-��ʼ�����Ƿ񱣴浽EEPROM
����ֵ����
*********************************************/
#define NOT_SAVE_TO_EEPROM  0   // �����浽EEPROM
#define SAVE_TO_EEPROM      1   // �豣�浽EEPROM


///**********************************************
//����: DataReset_ParaGroups
//����: ��Ĭ��ֵ��ʼ�������ڴ����
//����: PARAMITEM *ParamGroup: ָ������Ƭָ��;     GroupBegin����ʼ��; GroupNum������ʵ�����;
//      GroupAllNum: �ò���Ƭ��ʵ����������ֹ����; ItemAllByte:�ò���Ƭ��ÿ��Ԥ���������ֽ���������ֹ����
//����ֵ: --
//**********************************************/
void DataReset_ParaGroups(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, U08 GroupAllNum, U08 ItemAllByte)//, U08 opt)
{
    U08 i, j;
    U08 RepeatEnd=0, itemNum=0;
    U16 idx;
    I16 mac_init;

    PARAMITEM *pGroup;      // ������
    PARAM *pp2;             // ��������
    I16  *pp1_f;            // ����
    I16  *pp1_c;            // ����

    RepeatEnd = GroupNum+GroupBegin;        // ����

    for (i=GroupBegin; i<RepeatEnd; i++)    // �������󣬳�ʼ����ʱ�������������浽EEPROM
    {
        if(i>=GroupAllNum)  // Ҫ���ʵ��鳬����ʵ�ʵ����������ز�����
        {
            Unknown_Exception[DATA_EXCEPTION] = DATA_BREAK_BOUNDS;
            break;
        }
        pGroup = (PARAMITEM*)&ParamGroup[i];
        pp2 = (PARAM *)pGroup->array;
        itemNum = pGroup->items;
        pp1_c  = (I16 *)pGroup->data_c;
        pp1_f  = (I16 *)pGroup->data_f;

        if((itemNum<<1) > ItemAllByte)  // Ҫ���ʵĲ���������������Ԥ���Ĳ����������CRC�������ز�����
        {
            Unknown_Exception[DATA_EXCEPTION] = DATA_BREAK_BOUNDS;
            break;
        }

        for (j=0; j<itemNum; j++)
        {
            /* ��para_fac�еĲ���Ĭ��ֵΪ׼��Ӧ��para_fac��ParamPri�е�Ĭ��ֵ��ͬ����� */
            if ((ParamGroup == ParamPri) && PARA_ParaWithinMacType(i, j, &idx))
            {
#ifdef USE_SW_MAC
                mac_init = sw_mac_model;
#else
                /* �����û��͵�Ĭ��ֵ�����õ�ǰ��������Ϊ��Ҫ��ʼ�����������Ͳ�������Ҳ�ᱻ��ʼ�� */
                mac_init = ParamPri[P_grp(val_mac_model)].array[P_idx(val_mac_model)].init;
#endif
                pp1_c[j] = para_fac[idx].para[mac_init];
            }
#ifdef USE_SW_MAC
            else if ((ParamGroup == ParamPri) && (i==P_grp(val_mac_model) && j==P_idx(val_mac_model)))
            {
                pp1_c[j] = sw_mac_model;
            }
#endif
            else
            {
                pp1_c[j] = pp2[j].init;
            }
            pp1_f[j] = C_To_F(pp1_c[j], pp2[j].sign);   /* �������¶�ֵ */
        }
    }
}


/********************************************************************************
    ���� :  Judge_InitPara()
    ���� :  ���������ʼ��״̬

    ���� :  fg_NeedStore �Ƿ�����Ҫ�洢
    ���� :  ��
    ��ע :
********************************************************************************/
void Judge_InitPara(BOOL fg_NeedStore)
{
    if (ACTION_ING != GET_INIT_PARA())
    {
        para_init_dly = 0;
        return;
    }
    else
    {
        if (para_init_dly > 10)
        {
            SET_INIT_PARA(ACTION_FAIL) ;    // ��ʼ��ʧ��
        }
    }

    if (FALSE == fg_NeedStore)                        // û����Ҫ���������
    {
        if (ACTION_ING == GET_INIT_PARA())
        {
            SET_INIT_PARA(ACTION_SUCC);
        }
        return;
    }
}

/*==========================================
�� ��: FlagParaGroup_AllStore
��  ��: ��ʶ�����鱣���־������ֱ�ӱ��浥��������
�� ��: PARAMITEM *ParamGroup: ָ������Ƭָ��; GroupBegin����ʼ��; GroupNum������ʵ�����; GroupAllNum ʵ������;
�� ��: ��
/==========================================*/
void FlagParaGroup_AllStore(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, U08 GroupAllNum)
{
    U08 i;
    U08 RepeatEnd=0;

    PARAMITEM *pGroup;      // ������


    RepeatEnd = GroupNum+GroupBegin;        // ����

    for (i=GroupBegin; i<RepeatEnd; i++)
    {
        if(i>=GroupAllNum)  // Ҫ���ʵ��鳬����ʵ�ʵ����������ز�����
        {
            Unknown_Exception[DATA_EXCEPTION] = DATA_BREAK_BOUNDS;
            break;
        }
        pGroup = (PARAMITEM*)&ParamGroup[i];

        pGroup->head = 0;
        pGroup->end  = pGroup->items;

        pGroup->chip = 0xff;

    }

}

/**********************************************
������DataVerify_ParaArray
���ܣ��жϵ����������
������
����ֵ��fg_para_ok: TRUE������ FALSE����
**********************************************/
BOOL DataVerify_ParaGroups(PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum)
{
    U08 i, j;
    U08 RepeatEnd=0, Num=0;
//    static U08 tmp_index=0;
    PARAMITEM *pGroup;      // ������
    PARAM *pp2;                     // ��������

    BOOL fg_para_ok = TRUE;         // ����������־


    RepeatEnd = GroupNum+GroupBegin;        // ����

    for (i=GroupBegin; i<RepeatEnd; i++)
    {
        pGroup = (PARAMITEM*)&ParamGroup[i];
        pp2 = (PARAM *)pGroup->array;
        Num = pGroup->items;

        for (j=0; j<Num; j++)
        {
            if (para_is_bit(i, j))     continue;       /* λ���������ж� */

            if (pGroup->data_c[j] > pp2[j].max || pGroup->data_c[j] < pp2[j].min)
            {
                fg_para_ok = FALSE;
//                fg_seep_overflow_iderr |= 0x0001<<i;
                fg_seep_overflow_iderr = i+1;
                fg_seep_overflow_item  = j+1;
                i = RepeatEnd;
                break;
            }
        }
    }

    return fg_para_ok;
}

/********************************************************************************
    ���� :  FindParaGroup_NeedStore()
    ���� :  Ѱ���豣��Ĳ�����

    ���� :
    ���� :  0 û���豣��Ĳ����� / N �豣��Ĳ������+1������0�鷵��1��
    ��ע :
********************************************************************************/
U08 FindParaGroup_NeedStore(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum)
{
    PARAMITEM *pGroup;      // ������
    U08 i;
    U08 Group=0;

    for (i=0; i<GroupNum; i++)
    {
        pGroup = (PARAMITEM*)&ParamGroup[GroupBegin+i];

        if (pGroup->chip & PARA_NEED_STORE)     // �ڼ�������豣��
        {
            Group = GroupBegin + i + 1;
            break;
        }
    }

    return (Group);
}

///**********************************************
//������DataWrite_OneParaG
//���ܣ��洢ָ�����������������ִ�н��
//������ParamGroup_act��Ҫ�洢�Ĳ�����ָ�룻 DataInfo��������洢��Ϣ����ʼ��ַ��������
//����ֵ��0 ���豣��Ĳ����� / N �豣��Ĳ������+1������0�鷵��1��
//**********************************************/
static U08 DataWrite_OneParaG(const PARAMITEM *ParamGroup, U08 ParaGroup_one, EEDATA DataInfo)
{
    PARAMITEM *pGroup;      // ������
    U08 itemNum = 0;        // ��������

    static U08 head = 0;
    static U08 end  = 0;

    U16 StoreAddr;          // ���ο�ʼ�����ַ
    U16 ParaIndex;          // ���ο�ʼ����������
    U16 num = 0;            // �豣��Ĳ�������

    U08 *para_buf;
    U08 crc_buf[2];
    W16 crc_value;

    if(ParaGroup_one<1) return 0;

    pGroup = (PARAMITEM*)&ParamGroup[ParaGroup_one-1];
    itemNum = pGroup->items;

    if (pGroup->chip & PARA_STORE_PREP)     // ���������δ��ʼ���棬���ʼ���豣�����ʼλ�ú���ֹλ��
    {
        pGroup->chip &= (~PARA_STORE_PREP);
        head = pGroup->head;
        end  = pGroup->end;
    }

    num = end - head;
    if (num > 0)
    {
        if (num > PARA_WR_MAX)  num = PARA_WR_MAX;          // ÿ�����漸������

        ParaIndex = head;
        StoreAddr = DataInfo.Addr + (ParaIndex << 1);       // ��ʼ�����ַ

        para_buf  = (U08*)(pGroup->data_f + ParaIndex);     // д��2��para_buf  = (U08*)&pGroup->data_f[ParaIndex];

        if (eprom_wr_no_crc(para_buf, StoreAddr, num<<1))
        {
            head += num;
        }
    }
    else
    {
        para_buf  = (U08*)pGroup->data_f;                   // д��2��para_buf  = (U08*)&pGroup->data_f[0];
        crc_value.word = CRC16(para_buf, itemNum<<1);       // ��CRC
        crc_buf[0] = crc_value.byte.low;                    //NOTE-CXW ����2017.4.11 ȷ�ϴ�С��
        crc_buf[1] = crc_value.byte.high;

        StoreAddr = DataInfo.Addr + (itemNum<< 1);          // CRC�洢��ַ��������Ч���ݣ�

        if (eprom_wr_no_crc(&crc_buf, StoreAddr, 2))        // д����CRC
        {
            ParaGroup_one = 0;
            pGroup->chip &= (~PARA_NEED_STORE);
        }
    }
    return ParaGroup_one;
}


/********************************************************************************
    ���� :  DataWrite_ParaGroup()
    ���� :  ����ָ�������飬�ж��Ƿ������Ҫ����Ĳ����飬����У���ִ�б���

    ���� :  GroupBegin����ʼ��; GroupNum�����������;           pDataInfo���洢�׵�ַ��ÿ��Ԥ�������ֽڸ���
    ���� :  TRUE ִ�б��沢������ / FALSE ���豣��ֱ�ӷ���
    ��ע :
********************************************************************************/
BOOL DataWrite_ParaGroups(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, const EEDATA *pDataInfo)
{
    static U08 ParaGroup_Store=0;
    EEDATA DataInfoAll;     // ��Ƭ���ݴ洢��Ϣ
    EEDATA DataInfoEach;    // ��������洢��Ϣ
    BOOL fg_NeedStore = FALSE;  // �Ƿ��豣��

    if (ACTION_START==GET_INIT_PARA())    // �����ʼ��ʼ�������ͷ��ʼ����������
    {
        ParaGroup_Store = 0;
        SET_INIT_PARA(ACTION_ING);
    }

    if(ParaGroup_Store==0)
    {
        ParaGroup_Store=FindParaGroup_NeedStore(ParamGroup, GroupBegin, GroupNum);  // Ѱ���豣��Ĳ�����
    }
    if(ParaGroup_Store>GroupBegin && ParaGroup_Store <= (GroupBegin+GroupNum))
    {
        DataInfoAll.Addr     = pDataInfo->Addr;
        DataInfoAll.DataNum  = GetSize_Byte(pDataInfo->DataNum, WITH_CRC);                  // ÿ��Ԥ���ֽڸ���������CRC��

        DataInfoEach.Addr    = DataInfoAll.Addr+(ParaGroup_Store-1)*DataInfoAll.DataNum;    // ��������洢��ַ
        DataInfoEach.DataNum = 0;                                                           // �ֽڸ�������DataWrite_OneParaG�ټ���

        ParaGroup_Store = DataWrite_OneParaG(ParamGroup, ParaGroup_Store, DataInfoEach);
        fg_NeedStore = TRUE;
    }

    return fg_NeedStore;
}


/********************************************************************************
    ���� :  DataStore_Parameters()
    ���� :  ִ�в�ͬ����Ƭ�Ĵ洢
            (����Ƭ:ָ��ͬ���ܵĲ�������ϣ���ParamPri��ParamLimit��ParamModbus���ǲ�ͬ�Ĳ���Ƭ��
             ÿ������Ƭ�а���N����ͬ��С�Ĳ�����(N>=1))
    ���� :  ��
    ���� :  ��
    ��ע :
********************************************************************************/
void DataStore_Parameters()
{
    static U08 ParaPiece = 0;   // ��ǰ����Ĳ���Ƭ
    BOOL fg_NeedStore=FALSE;    // �Ƿ��豣��
    U08  GroupNum;

    if (ACTION_START==GET_INIT_PARA())    // �����ʼ��ʼ�������ͷ��ʼ����������
    {
        ParaPiece = 0;
    }

    switch(ParaPiece)
    {
        case 0:
            GroupNum=(sizeof(ParamPri) / sizeof(PARAMITEM));

            fg_NeedStore = DataWrite_ParaGroups(ParamPri, 0, GroupNum, &ParamPri_DataInfo);
            if (FALSE == fg_NeedStore)    // û���豣��Ĳ���������һ����Ƭ
            {
                ParaPiece++;
            }
            else
            {
                break;
            }

           // break;

        case 1:

//            GroupNum=(sizeof(ParamModbus) / sizeof(PARAMITEM));
//
//            fg_NeedStore = DataWrite_ParaGroups(ParamModbus, 0, GroupNum, &Modbus_DataInfo);
            if (FALSE == fg_NeedStore)    // û���豣��Ĳ���������һ����Ƭ
            {
                ParaPiece++;
            }
            else
            {
                break;
            }
            // break;

        case 2:

            GroupNum=(sizeof(ParamLimit) / sizeof(PARAMITEM));

            fg_NeedStore = DataWrite_ParaGroups(ParamLimit, 0, GroupNum, &Limit_DataInfo);
            if (FALSE == fg_NeedStore)    // û���豣��Ĳ���������һ����Ƭ
            {
                ParaPiece=0;
            }
            else
            {
                break;
            }
            // break;

        default:

            break;
    }
    Judge_InitPara(fg_NeedStore);   // �жϳ�ʼ��״̬

}


//============================================================================================================
/*==========================================
    ��  ��: ����ֵ�Ƿ���Ч(����ע��)
/==========================================*/

U08 para_begin(U08 n, U08 m, I16 v)
{
    U08 para_value_valid = 1;       /* ��������ֵ����(��ʼ��Ϊ����) */

    n = n, m = m, v = v;           /* N0001 */

    return para_value_valid;
}

/*******************************************************************************
 �� ��: PARA_MacType()
 �� ��: ���ݻ������ò���
 �� ��: model--����;
 �� ��: ��
 �� ��: 1������������϶�ʱ��para_end��para_set���໥�����Ƿ�����ջ�����
        2������ʱע�⣺����Ĭ��ֵӦ��para_facΪ׼��ע���Ƿ���������Լ��ϵ�����ٴθĶ�
*******************************************************************************/
void PARA_MacType(U08 model)
{
    U16 i;
    if (model >= MAC_MAX)   return;
    for (i=0; i<ARRELE(para_fac); i++)
    {
        para_set_cent(para_fac[i].grp, para_fac[i].idx, para_fac[i].para[model]);
    }
}
/*******************************************************************************
 �� ��: PARA_ParaWithinMacType()
 �� ��: �жϲ����Ƿ�Ϊ������ز���
 �� ��: n--��;
        m--��;
 �� ��: TRUE/FALSE--��/��
 �� ��: �жϳɹ�ʱ����¼�ò�����para_fac�е����
*******************************************************************************/
BOOL PARA_ParaWithinMacType(U08 n, U08 m, U16 *idx)
{
    BOOL fg_result = FALSE;
    U16 i;
    for (i=0; i<ARRELE(para_fac); i++)
    {
        if (n==para_fac[i].grp && m==para_fac[i].idx)
        {
            fg_result = TRUE;
            *idx = i;
            break;
        }
    }
    return fg_result;
}

/*==========================================
    ��  ��: ����������Լ(����ע��)
    [n][m]��Ӧ���������
    v  : Ϊ���϶�
/==========================================*/
void para_end(U08 n, U08 m, I16 v)
{
#ifndef USE_KIT_PARA_FAC
    U08 i;
    
    if (P_isP(n, m, val_air_type)			/* �յ����� */
    || P_isP(n, m, val_cool_used)			/* ����ģʽ */
    || P_isP(n, m, val_heat_used)			/* ����ģʽ */
    )
    {
        if (USE_COOL_ONLY)                  /* ������ */
        {
            para_set_cent(P_GaI(sv_mode), SET_COOL);	/* ����ģʽ */
        }
        else if (USE_HEAT_ONLY)             /* ������ */
        {
            para_set_cent(P_GaI(sv_mode), SET_HEAT);	/* ����ģʽ */
        }
    }
    
    if (P_isP(n, m, sv_mode))
    {
        if (USE_DUAL_ZONE_CTRL				/* ˫������ */
        && (v==SET_COOL)				    /* �������� */
        )
        {
//            fg_zone_B_run = FALSE;
            SETCMD_N(ZONE_B_OFF);			/* B���ػ� */
        }
    }

    /* ��Ƶ��ʹ�� */
    if (P_isP(n, m, val_inverter_used))
    {
        if (val_inverter_used)
        {
            COM_InitMaster(COM_INVT, BKBD_INVT_UART2);
        }
        else
        {
            /* ����ʱ����Ϊ�ӻ�������λ�������ӱ�Ƶ�� */
            COM_InitSlaver(COM_INVT, BKBD_INVT_UART2, 100);
        }
    }
    /* �趨�¶������޴��� */
    {
        {
            I16 set_cool = para_get_dot(P_GaI(sv_cool)) ? sv_cool : sv_cool/10;
            I16 min=para_get_min_cent(P_GaI(sv_cool));
            I16 max=para_get_max_cent(P_GaI(sv_cool));
            if (set_cool < min)
            {
                para_set_cent(P_GaI(sv_cool), min);	/* �����趨�¶� */  
            }
            else if (set_cool > max)
            {
                para_set_cent(P_GaI(sv_cool), max);	/* �����趨�¶� */
            }
        }
        {
            I16 set_cool_B = para_get_dot(P_GaI(sv_cool_zone_B)) ? sv_cool_zone_B : sv_cool_zone_B/10;
            I16 min=para_get_min_cent(P_GaI(sv_cool_zone_B));
            I16 max=para_get_max_cent(P_GaI(sv_cool_zone_B));
            if (set_cool_B < min)
            {
                para_set_cent(P_GaI(sv_cool_zone_B), min);	/* B�������趨�¶� */ 
            }
            else if (set_cool_B > max)
            {
                para_set_cent(P_GaI(sv_cool_zone_B), max);	/* B�������趨�¶� */ 
            }
        }
        {
            I16 set_heat = para_get_dot(P_GaI(sv_heat)) ? sv_heat : sv_heat/10;
            I16 min=para_get_min_cent(P_GaI(sv_heat));
            I16 max=para_get_max_cent(P_GaI(sv_heat));
            if (set_heat < min)
            {
                para_set_cent(P_GaI(sv_heat), min);	/* �����趨�¶� */
            }
            else if (set_heat > max)
            {
                para_set_cent(P_GaI(sv_heat), max);	/* �����趨�¶� */
            }
        }
        {
            I16 set_heat_B = para_get_dot(P_GaI(sv_heat_zone_B)) ? sv_heat_zone_B : sv_heat_zone_B  /10;
            I16 min=para_get_min_cent(P_GaI(sv_heat_zone_B));
            I16 max=para_get_max_cent(P_GaI(sv_heat_zone_B));
            if (set_heat_B < min)
            {
                para_set_cent(P_GaI(sv_heat_zone_B), min);	/* B�������趨�¶� */
            }
            else if (set_heat_B > max)
            {
                para_set_cent(P_GaI(sv_heat_zone_B), max);	/* B�������趨�¶� */
            }
        }
        {
            I16 set_hotw = para_get_dot(P_GaI(sv_hotw)) ? sv_hotw : sv_hotw/10;
            I16 min = para_get_min_cent(P_GaI(sv_hotw));
            I16 max = para_get_max_cent(P_GaI(sv_hotw));

            if (set_hotw < min)
            {
                para_set_cent(P_GaI(sv_hotw), min);				/* ��ˮ�趨�¶� */
            }
            else if (set_hotw > max)
            {
                para_set_cent(P_GaI(sv_hotw), max);				/* ��ˮ�趨�¶� */
            }
        }
    }

    if (P_isP(n, m, val_bms_format)			/* BMS���ݸ�ʽ */
    || P_isP(n, m, val_bms_baud_rate)		/* BMS������ */
    || P_isP(n, m, val_bms_addr)			/* BMSͨѶ��ַ */
    )
    {
        COM_config_and_init(COM_BMS);
#ifdef COM_4G
        COM_config_and_init(COM_4G);
#endif
    }

    /* ��Ƶѹ������ */
    if (P_isP(n, m, val_invt_motor_type))    invt_para_set(INVT_ID_1,INVT_PARA_MOTOR_TYPE, val_invt_motor_type);

    /* ����ת������ */
    if (P_isP(n, m, val_invt_rps_min))       invt_para_set(INVT_ID_1,INVT_PARA_FREQ_MIN, val_invt_rps_min);

    /* ����ת������ */
    if (P_isP(n, m, val_invt_rps_max))       invt_para_set(INVT_ID_1,INVT_PARA_FREQ_MAX, val_invt_rps_max);

    /* ��������ת�� */
    if (P_isP(n, m, val_invt_start_freq))    invt_para_set(INVT_ID_1,INVT_PARA_FREQ_START, val_invt_start_freq);

    /* EC������� */
    if (P_isP(n, m, val_invt_fan_type))
    {
        invt_para_set(INVT_FAN1,INVT_PARA_MOTOR_TYPE, val_invt_fan_type);
        invt_para_set(INVT_FAN2,INVT_PARA_MOTOR_TYPE, val_invt_fan_type);
    }

    /* ����ʹ������ */
    if (P_isP(n, m, val_motor_use))             SET_EXIO_INIT_HDW(0);

    /* �������ŷ�ʽ */
    if (P_isP(n, m, val_motor_fre_type))        eev_set_motor_type(EEV_ID_1, (U16)v);

    /* ��������Ƶ�� */
    if (P_isP(n, m, val_motor_step_fre))        eev_set_step_speed(EEV_ID_1, (U16)v);

    /* ����ʹ������ */
    if (P_isP(n, m, val_s_motor_use))           SET_EXIO_INIT_HDW(0);

    /* �������ŷ�ʽ */
    if (P_isP(n, m, val_s_motor_fre_type))      eev_set_motor_type(EEV_ID_2, (U16)v);

    /* ��������Ƶ�� */
    if (P_isP(n, m, val_s_motor_step_fre))      eev_set_step_speed(EEV_ID_2, (U16)v);

#ifdef USE_ODU_HL081B		/* HL081B */
    /* �¶�̽ͷ���� */
    if (P_isP(n, m, val_ntc_sensor_config))
    {
        if (USE_PUNP_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
        }
        else if (USE_TCL_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_TCL_NORMAL_10K);
        }
        else if (USE_CHANGHONG_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
        }
        else if (USE_PAIWO_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_PAIWO_NORMAL_10K);
        }
        else if (USE_NIUENTAI_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_NIUENTAI_NORMAL_5K);
        }
    }
#else						/* HL081A */
    /* �¶�̽ͷ���� */
    if (P_isP(n, m, val_ntc_sensor_config))
    {
        if (USE_PUNP_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
        }
        else if (USE_TCL_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_TCL_NORMAL_10K);
        }
        else if (USE_CHANGHONG_NTC)
        {
            ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
        }
    }
#endif

    /* �������ã�����Ƶˮ�ñ��롿��Լ��ˮ��Ŀ��ת�١��͡�ˮ��Ŀ�������� */
    if (P_isP(n, m, val_PUMPf_model))
    {
        /* ˮ��Ŀ��ת�ٴ��� */
        {
            U16 pump_sv = para_get_val(P_GaI(val_pump_sv));
            U16 min = para_get_min_cent(P_GaI(val_pump_sv));
            U16 max = para_get_max_cent(P_GaI(val_pump_sv));
            if (pump_sv < min)
            {
                para_set_cent(P_GaI(val_pump_sv), min);
            }
            else if (pump_sv > max)
            {
                para_set_cent(P_GaI(val_pump_sv), max);
            }
        }
        /* ˮ��Ŀ���������� */
        {
            U16 pump_sv_flow = para_get_val(P_GaI(val_pump_sv_flow));
            U16 min = para_get_min_cent(P_GaI(val_pump_sv_flow));
            U16 max = para_get_max_cent(P_GaI(val_pump_sv_flow));
            if (pump_sv_flow < min)
            {
                para_set_cent(P_GaI(val_pump_sv_flow), min);
            }
            else if (pump_sv_flow > max)
            {
                para_set_cent(P_GaI(val_pump_sv_flow), max);
            }
        }
    }

    /* ����Ƶ�ʱ� */
    if (n == GROUP_WF)
    {
        wf_init();
    }

    /* Ӳ�����ø��£�֪ͨIO��չ�� */
    if (P_isP(n, m, val_motor_fre_type)		/* �������ŷ�ʽ */
    || P_isP(n, m, val_motor_step_fre)		/* ��������Ƶ�� */
    || P_isP(n, m, val_motor_step_max)		/* �����ܲ��� */
    || P_isP(n, m, val_s_motor_fre_type)	/* �������ŷ�ʽ */
    || P_isP(n, m, val_s_motor_step_fre)	/* ��������Ƶ�� */
    || P_isP(n, m, val_s_motor_step_max)	/* �����ܲ��� */
    || P_isP(n, m, val_ntc_sensor_config)   /* �¶�̽ͷ���� */
    )
    {
        for (i = 0; i < USE_EXIO; i++)
        {
            SET_EXIO_HDW_CFG_UPDATE(i);
        }
    }

    /* ���ü���ʱ���������û���ˮ���¶�̽ͷ */
    if (P_isP(n, m, val_pcb_num))
    {
        if (val_pcb_num > 1)
        {
            para_set_cent(P_GaI(val_TACt1_sensor_used), USED);	/* ����ˮ���ϲ��¶�̽ͷ */
            para_set_cent(P_GaI(val_TACt2_sensor_used), USED);	/* ����ˮ���²��¶�̽ͷ */
        }
    }
#endif

    /* �����ͺ� */
    if (P_isP(n, m, val_mac_model))
    {
        PARA_MacType(val_mac_model);
        /* ����������ִ�н��չ㲥������������MacTypeMaster�ŵ����������� */
        if (pcb_type==TYPE_MASTER)  MacTypeMaster = val_mac_model;
    }

#ifdef AT_MAX_NEW
    /* ��������ѡ�� */
    if (P_isP(n, m, val_ability_test_set))
    {
        static U08 group_bak = AT_HT_GP_ORIGIN;
        U08 group = para_AT_get_group_ht();

        /* ��ű���Ÿ���Ԥ��ֵ��
         * 1. Ϊ��ʵ��ͬ��ʹ��ʱ�����Բ������ֶ�����
         * 2. ���ϵ�󣬷�ԭʼ��ı乤��ʱ��Ҳ�����һ�Σ���group_bakδ�浽e��
         */
        if (group_bak != group)
        {
            group_bak = group;
            
            para_AT_update_ht();
        }
    }
#endif    

}
/*==========================================
    ��  ��: ȡ����������
/==========================================*/
U16 para_get_sign(U08 n, U08 m)
{
    U16 temp;
    PARA_SetAttrib(n, m, &temp);
    return temp;
}

/*******************************************************************************
 �� ��: GetSettingRange()
 �� ��: ��ȡ�����¶ȵ��趨ֵ��Χ
 �� ��:
 �� ��: ��
 �� ��: ���ص��ǳ����е����϶�ֵ���뵱ǰ�¶ȵ�λ�޹�
*******************************************************************************/
I16 GetSettingRange(I16 CtrlObject, U08 DataType, U08 Mode)
{
    BOOL isGetMax = (DataType == GET_MAX) ? TRUE : FALSE;	/* �Ƿ��ȡ���ֵ */
    BOOL isCtrlTempOut = (CtrlObject == TEMP_OUT) ? TRUE : FALSE;	/* ���ƶ����Ƿ�Ϊ��ˮ�¶� */
    I16 val = 0;

    switch (Mode)
    {
        /* ���� */
        case MODE_COOL:
            val = isGetMax ? val_cool_set_max : val_cool_set_min;
            val -= isCtrlTempOut ? INOUT_DIFF : 0;/* ���Ƴ�ˮʱ���¶��޶� */
            break;

        /* ���� */
        case MODE_HEAT:
            val = isGetMax ? val_heat_set_max : val_heat_set_min;
            val += isCtrlTempOut ? INOUT_DIFF : 0;/* ���Ƴ�ˮʱ���¶��޶� */
            break;
    }

    return val;
}

/*******************************************************************************
 �� ��: get_pumpf_para()
 �� ��: ��ȡ��Ƶˮ�õ���ز���
 �� ��:
 �� ��: ��
 �� ��: 
*******************************************************************************/

/* ��ȡ��Ƶˮ�õĲ����������ת�����ֵ��ת����Сֵ���������ֵ��������Сֵ��...... */
enum {EMPTY, RPM_MAX, RPM_MIN, FLOW_MAX, FLOW_MIN};

I16 get_pumpf_para(U08 type_para)
{
    /* ���صĲ���ֵ��ת�����ֵ��ת����Сֵ���������ֵ��������Сֵ��...... */
    I16 rpm_max, rpm_min, flow_max, flow_min;
      
    /* ѡ���Ƶˮ���ͺ� */
    switch (val_PUMPf_model)
    {
        case PUMPF_00_PARA_9_IPWM1:         /* ���� */
            rpm_max = PUMPF_00_RPM_MAX;
            rpm_min = PUMPF_00_RPM_MIN;
            flow_max = PUMPF_00_FLOW_MAX;
            flow_min = PUMPF_00_FLOW_MIN;
            break;
    
        default:
        case PUMPF_01_APM25_9_130_PWM:      /* �½� */
            rpm_max = PUMPF_01_RPM_MAX;
            rpm_min = PUMPF_01_RPM_MIN;
            flow_max = PUMPF_01_FLOW_MAX;
            flow_min = PUMPF_01_FLOW_MIN;
            break;
    
        case PUMPF_02_GPA25_9HW_130:        /* �»� */
            rpm_max = PUMPF_02_RPM_MAX;
            rpm_min = PUMPF_02_RPM_MIN;
            flow_max = PUMPF_02_FLOW_MAX;
            flow_min = PUMPF_02_FLOW_MIN;
            break;
    
        case PUMPF_03_APF25_12_130FPWM1:    /* �½� APF25-12-130FPWM1 */
            rpm_max = PUMPF_03_RPM_MAX;
            rpm_min = PUMPF_03_RPM_MIN;
            flow_max = PUMPF_03_FLOW_MAX;
            flow_min = PUMPF_03_FLOW_MIN;
            break;
    
        case PUMPF_04_APF25_12_130PWM1:     /* �½� APF25-12-130PWM1 */
            rpm_max = PUMPF_04_RPM_MAX;
            rpm_min = PUMPF_04_RPM_MIN;
            flow_max = PUMPF_04_POW_MAX;
            flow_min = PUMPF_04_POW_MIN;
            break;
    
        case PUMPF_05_GPA25_11H:            /* �»� GPA25_11H1 */
            rpm_max = PUMPF_05_RPM_MAX;
            rpm_min = PUMPF_05_RPM_MIN;
            flow_max = PUMPF_05_FLOW_MAX;
            flow_min = PUMPF_05_FLOW_MIN;
            break;

        case PUMPF_06_HBG_25_125_130P:    /* ��ķ�� HBG-25-125-130P */
            rpm_max = PUMPF_06_RPM_MAX;
            rpm_min = PUMPF_06_RPM_MIN;
            flow_max = PUMPF_06_FLOW_MAX;
            flow_min = PUMPF_06_FLOW_MIN;
            break;

        case PUMPF_07_UPM4XLK_25_90_130:   /* ������ UPM4XLK 25-90 130 */   
            rpm_max = PUMPF_07_RPM_MAX;
            rpm_min = PUMPF_07_RPM_MIN;
            flow_max = PUMPF_07_FLOW_MAX;
            flow_min = PUMPF_07_FLOW_MIN;
            break;
    }
    
    switch (type_para)
    {
        case EMPTY:
            return 0;
        case RPM_MAX:
            return rpm_max;
        case RPM_MIN:
            return rpm_min;
        case FLOW_MAX:
            return flow_max;
        case FLOW_MIN:
            return flow_min;
        default:
            break;
    }
    
    return 0;
}

/*==========================================
    ��  ��: ȡ��������Сֵ
/==========================================*/
I16 para_get_min_real(U08 n, U08 m, U08 temp_unit)
{
	I16 val_min = 0;
#ifndef USE_KIT_PARA_FAC
    I16 s_min;				/* ��̬���� */
    I16 d_min;				/* ��̬���� */
    PARAM *pp2;				/* �������� */

    pp2 = (PARAM *)ParamPri[n].array;
    s_min = pp2[m].min;
    d_min = pp2[m].min;

    /* ����ģʽ */
    if (P_isP(n, m, sv_mode))                
    {
        if (USE_COOL_ONLY)	                /* ������ */
        {
            d_min = SET_COOL;
        }
        else if (USE_HEAT_ONLY)				/* ������ */
        {
            d_min = SET_HEAT;
        }
    }

    /* �����趨�¶� */
    if (P_isP(n, m, sv_cool))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_COOL);
    }

    /* �����趨�¶� */
    if (P_isP(n, m, sv_heat))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_HEAT);
    }

#ifndef USE_AREA_B_FIX
    /* �����趨�¶�(B) */
    if (P_isP(n, m, sv_cool_zone_B))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_COOL);
    }

    /* �����趨�¶�(B) */
    if (P_isP(n, m, sv_heat_zone_B))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_HEAT);
    }
#endif    

    /* ��ˮ�趨�¶� */
    if (P_isP(n, m, sv_hotw))
    {
        d_min = GetSettingRange(TEMP_IN, GET_MIN, MODE_HEAT);
    }

    if (P_isP(n, m, val_lp_value_incl)		/* �����ѹ���� */
    || P_isP(n, m, val_lp_value_inht)		/* ���ȵ�ѹ���� */
    )
    {
        d_min = val_J6_scale_min;
    }

    /* ˮ��Ŀ��ת�� */
    if (P_isP(n, m, val_pump_sv))
    {
        d_min = get_pumpf_para(RPM_MIN);
    }

    /* ˮ��Ŀ������ */
    if (P_isP(n, m, val_pump_sv_flow))
    {
        d_min = get_pumpf_para(FLOW_MIN);
    }

    val_min = (d_min < s_min) ? s_min : d_min;	/* ��̬���ޱȾ�̬����Сʱ���Ծ�̬����Ϊ׼(��ֹд��EEPROM�󱨴�) */
#else
    I16 s_min;				/* ��̬���� */
    PARAM *pp2;				/* �������� */

    pp2 = (PARAM *)ParamPri[n].array;
    s_min = pp2[m].min;

    val_min = s_min;
#endif

	if (temp_unit == SH_SET_UNIT_F)
	{
		val_min = C_To_F(val_min, para_get_sign(n, m));
	}

	return val_min;
}

/*******************************************************************************
 �� ��: para_get_min_cent()
 �� ��:
 �� ��: n--;
		m--;
 �� ��:
 �� ��: ��
*******************************************************************************/
I16 para_get_min_cent(U08 n, U08 m)
{
	return para_get_min_real(n, m, SH_SET_UNIT_C);
}

/*******************************************************************************
 �� ��: para_get_min()
 �� ��:
 �� ��: n--;
		m--;
 �� ��:
 �� ��: ��
*******************************************************************************/
I16 para_get_min(U08 n, U08 m)
{
	return para_get_min_real(n, m, sh_temp_unit);
}

/*==========================================
    ��  ��: ȡ���������ֵ
/==========================================*/
I16 para_get_max_real(U08 n, U08 m, U08 temp_unit)
{
	I16 val_max = 0;
#ifndef USE_KIT_PARA_FAC
    I16 s_max;				/* ��̬���� */
    I16 d_max;				/* ��̬���� */
    PARAM *pp2;				/* �������� */

    pp2 = (PARAM *)ParamPri[n].array;
    s_max = pp2[m].max;
    d_max = pp2[m].max;
   
    /* ����ģʽ */
    if (P_isP(n, m, sv_mode))
    {
        if (USE_COOL_ONLY)                  /* ������ */
        {
            d_max = SET_COOL;
        }
        else if (USE_HEAT_ONLY)             /* ������ */
        {
            d_max = SET_HEAT;
        }
    }

    /* �����趨�¶� */
    if (P_isP(n, m, sv_cool))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_COOL);
    }

    /* �����趨�¶� */
    if (P_isP(n, m, sv_heat))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_HEAT);
    }

#ifndef USE_AREA_B_FIX
    /* �����趨�¶�(B) */
    if (P_isP(n, m, sv_cool_zone_B))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_COOL);
    }

    /* �����趨�¶�(B) */
    if (P_isP(n, m, sv_heat_zone_B))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_HEAT);
    }
#endif

    /* ��ˮ�趨�¶� */
    if (P_isP(n, m, sv_hotw))
    {
        //Ĭ��Ŧ��̩��ˮ�趨�¶����޿��ܰ��յ�����ˮ�������ơ�
        //�����ͻ���ˮ�趨�¶����ް������Ȼ����������ơ�
        if ((val_hotw_max_TBH != 0) && val_TBH_used)
        {
            d_max = val_hotw_max_TBH;
        }
        else
        {
            d_max = GetSettingRange(TEMP_IN, GET_MAX, MODE_HEAT);
        }
    }

    /* �����ֶ����� */
    if (P_isP(n, m, val_motor_manual_steps))
    {
        d_max = val_motor_step_max;
    }

    /* �����ֶ����� */
    if (P_isP(n, m, val_s_motor_manual_steps))
    {
        d_max = val_s_motor_step_max;
    }

    if (P_isP(n, m, val_hp_value_incl)		/* �����ѹ���� */
    || P_isP(n, m, val_hp_value_inht)		/* ���ȸ�ѹ���� */
    )
    {
        if (USE_REGULAR_PRS)
        {
            d_max = val_J6_scale_max;
        }
        else
        {
            d_max = val_J5_scale_max;
        }
    }

    /* ˮ��Ŀ��ת�� */
    if (P_isP(n, m, val_pump_sv))
    {
        d_max = get_pumpf_para(RPM_MAX);
    }

    /* ˮ��Ŀ������ */
    if (P_isP(n, m, val_pump_sv_flow))
    {
        d_max = get_pumpf_para(FLOW_MAX);
    }

    val_max = (d_max > s_max) ? s_max : d_max;	/* ��̬���ޱȾ�̬���޴�ʱ���Ծ�̬����Ϊ׼(��ֹд��EEPROM�󱨴�) */
#else
    I16 s_max;				/* ��̬���� */
    PARAM *pp2;				/* �������� */

    pp2 = (PARAM *)ParamPri[n].array;
    s_max = pp2[m].max;

    val_max = s_max;
#endif

	if (temp_unit == SH_SET_UNIT_F)
	{
		val_max = C_To_F(val_max, para_get_sign(n, m));
	}

	return val_max;
}

/*******************************************************************************
 �� ��: para_get_max_cent()
 �� ��:
 �� ��: n--;
		m--;
 �� ��:
 �� ��: ��
*******************************************************************************/
I16 para_get_max_cent(U08 n, U08 m)
{
	return para_get_max_real(n, m, SH_SET_UNIT_C);
}

/*******************************************************************************
 �� ��: para_get_max()
 �� ��:
 �� ��: n--;
		m--;
 �� ��:
 �� ��: ��
*******************************************************************************/
I16 para_get_max(U08 n, U08 m)
{
	return para_get_max_real(n, m, sh_temp_unit);
}

/*==========================================
    ��  ��: ȡ�����ĵ�ǰֵ
/==========================================*/
I16 para_get_val(U08 n, U08 m)
{
    PARAMITEM *pp1;

    pp1 = (PARAMITEM *)&ParamPri[n];

    if (TEMP_IS_FARA)
    {
    	return pp1->data_f[m];
    }
    else
    {
    	return pp1->data_c[m];
    }

}
/********************************************
������key_continuous_add
���ܣ���������+��
������������������š���ǰֵ
����ֵ������ֵ
*********************************************/
I16 key_continuous_add(I16 val, I16 max, I16 min, U08 opt)
{
    I16 para_val;

    para_val = val;

    if (key_continuous_time[hmi_t_num] > CTUS_1000)     /* ������������90��, ��1000 */
    {
        if (para_val + 1000 >= max)  para_val = max;
        else para_val += 1000;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_100)     /* ������������30��, ��100 */
    {
        if (para_val + 100 >= max)  para_val = max;
        else para_val += 100;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_10)  /* ������������10��, ��10 */
    {
        if (para_val + 10 >= max)  para_val = max;
        else para_val += 10;
    }
    else
    {
        if(para_val < max)para_val++;
        else
        {
            if(opt==CAL_CYCLE)  para_val = min;     // ѭ������
            if(opt==CAL_ETOE)   para_val = max;     // ��ѭ������
        }
    }

    return para_val;
}
/********************************************
������key_continuous_sub
���ܣ���������-��
������������������š���ǰֵ
����ֵ������ֵ
*********************************************/
I16 key_continuous_sub(I16 val, I16 max, I16 min, U08 opt)
{
    I16 para_val;

    para_val = val;

    if (key_continuous_time[hmi_t_num] > CTUS_1000)     /* ������������90��, ��1000 */
    {
        if (para_val - 1000 <= min)  para_val = min;
        else para_val -= 1000;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_100)     /* ������������30��, ��100 */
    {
        if (para_val - 100 <= min)  para_val = min;
        else para_val -= 100;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_10)  /* ������������10��, ��10 */
    {
        if (para_val - 10 <= min)  para_val = min;
        else para_val -= 10;
    }
    else
    {
        if(para_val > min)para_val--;
        else
        {
            if(opt==CAL_CYCLE)  para_val = max;     // ѭ������
            if(opt==CAL_ETOE)   para_val = min;     // ��ѭ������
        }
    }

    return para_val;
}

/*==========================================
    ��  ��: ���� + 1
/==========================================*/
void para_add(U08 n, U08 m, I16 *v, U08 bit_pos)
{
    U08 i;
    I16 val;
//  U16 tmp;

    val = *v;
    bit_pos = bit_pos;      /* N0001 */
//  if (bit_pos) /* λ���� */
//  {
//      tmp = (U16)val;
//      tmp |= (0x0001 << (bit_pos - 1));
//      val = (I16)tmp;
//  }
//  else
//  {
        for (i=0; i<16; i++)
        {
            if (P_isP(n, m, sv_cool)
             || P_isP(n, m, sv_heat)
            )
            {
                val = key_continuous_add(val, para_get_max(n, m), para_get_min(n, m),CAL_ETOE);
            }
            else
            {
                val = key_continuous_add(val, para_get_max(n, m), para_get_min(n, m),CAL_CYCLE);
            }
            if (para_begin(n, m, val))  break;
        }
        if (i >= 16)  val = *v;
//  }
    *v = val;
}


/*==========================================
    ��  ��: ���� - 1
/==========================================*/
void para_sub(U08 n, U08 m, I16 *v, U08 bit_pos)
{
    U08 i;
    I16 val;
//  U16 tmp;

    val = *v;
    bit_pos = bit_pos;      /* N0001 */
//  if (bit_pos) /* λ���� */
//  {
//      tmp = (U16)val;
//      tmp &= ~(0x0001 << (bit_pos - 1));
//      val = (I16)tmp;
//  }
//  else
//  {
        for (i=0; i<16; i++)
        {
            if (P_isP(n, m, sv_cool)
             || P_isP(n, m, sv_heat)
            )
            {
                val = key_continuous_sub(val, para_get_max(n, m), para_get_min(n, m),CAL_ETOE);
            }
            else
            {
                val = key_continuous_sub(val, para_get_max(n, m), para_get_min(n, m),CAL_CYCLE);
            }
            if (para_begin(n, m, val))  break;
        }
        if (i >= 16)  val = *v;
//  }
    *v = val;
}


/*PG_GetAddr_xxx˵��**********************************************************************

�� ��: PG_GetAddr_xxx
�� ��: �ӿں�������ȡxxx������ĵ�ַ��PARAMITEM�ṹ��
ʹ ��: ÿ����һ��PARAMITEM������������һ��PG_GetAddr_xxx������xxx = ��ӦPARAMITEM������
����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

/********************************************
����: PG_GetAddr_ParamPri
����: ��ȡParamPri������ĵ�ַ
����: ���������
����ֵ: ������ĵ�ַ
*********************************************/
PARAMITEM* PG_GetAddr_ParamPri(U08 n)
{
    PARAMITEM* PgAddr;
    PgAddr = (PARAMITEM*)&ParamPri[n];
    return PgAddr;
}

/********************************************
����: PG_GetAddr_ParamLimit
����: ��ȡParamLimit������ĵ�ַ
����: ���������
����ֵ: ������ĵ�ַ
*********************************************/
PARAMITEM* PG_GetAddr_ParamLimit(U08 n)
{
    PARAMITEM* PgAddr;
    PgAddr = (PARAMITEM*)&ParamLimit[n];
    return PgAddr;
}

/********************************************
����: PG_GetGroupNum
����: ��ȡPARAMITEM�����������
����: ������[]���׵�ַ
����ֵ: �����������
ʹ ��: ÿ����һ��PARAMITEM���飬���ڴ˺���������Ӧ����

*********************************************/
U08 PG_GetGroupNum(const PARAMITEM *ParamGroup)
{
    U08 GroupNum = 0;

    if(ParamGroup==(PARAMITEM *)&ParamPri[0])
    {
        GroupNum = (sizeof(ParamPri) / sizeof(PARAMITEM));
    }
//    else if(ParamGroup==(PARAMITEM *)&ParamModbus[0])
//    {
//      GroupNum = (sizeof(ParamModbus) / sizeof(PARAMITEM));
//    }
    else if(ParamGroup==(PARAMITEM *)&ParamLimit[0])
    {
        GroupNum = (sizeof(ParamLimit) / sizeof(PARAMITEM));
    }

    return GroupNum;
}


/*==========================================
    ��  ��: �趨����
/==========================================*/
void para_set_real(U08 n, U08 m, I16 v, U08 temp_unit)
{
    PARAMITEM *pp1;
    I16 C = (temp_unit==SH_SET_UNIT_C) ? v : F_To_C(v, para_get_sign(n, m));
    I16 F = (temp_unit==SH_SET_UNIT_F) ? v : C_To_F(v, para_get_sign(n, m));
    if (!para_begin(n, m, v)
	    ||  v<para_get_min_real(n, m, temp_unit)
	    ||  v>para_get_max_real(n, m, temp_unit))
	{
		return;
	}

    pp1 = (PARAMITEM *)&ParamPri[n];

    /* ��һ��ʱ�ű��� */
    if (C != pp1->data_c[m] || F != pp1->data_f[m])
    {
        pp1->data_f[m] = F;
        pp1->data_c[m] = C;

        set_para_modify_flag(n, m);
        para_end(n, m, C);	/* para_end�в��������߼�ȫʹ�����϶����ݣ����д������ݱ���Ϊ���϶����� */
    }
}

/*******************************************************************************
 �� ��: para_set_cent()
 �� ��:
 �� ��: n--;
		m--;
		v--;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void para_set_cent(U08 n, U08 m, I16 v)
{
	para_set_real(n, m, v, SH_SET_UNIT_C);
}

/*******************************************************************************
 �� ��: para_set()
 �� ��:
 �� ��: n--;
		m--;
		v--;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void para_set(U08 n, U08 m, I16 v)
{
	para_set_real(n, m, v, sh_temp_unit);
}

/*==========================================
    ��  ��: �����Ƿ�����
/==========================================*/
U08 para_is_hide(U08 n, U08 m, U08 grade)
{
    BOOL para_item_hide = FALSE;

    if (grade != 0xff)      /* ������ */
    {
        if ((para_get_sign(n, m) & (D_HIDE)) != 0)
        {
            para_item_hide = TRUE;
        }
        else
        {
            if (grade == PASS_FAC)              /* ���Ҽ� */
            {
                if ((para_get_sign(n, m) & D_SAFEMASK) > D_FAC)    para_item_hide = TRUE;
            }
            else if (grade == PASS_SER)         /* ά�޼� */
            {
                if ((para_get_sign(n, m) & D_SAFEMASK) > D_SER)    para_item_hide = TRUE;
            }
//          else if (grade == PASS_PRO)         /* ���̼� */
//          {
//              if ((para_get_sign(ParamGroup, m) & D_SAFEMASK) > D_PRO)    para_item_hide = TRUE;
//          }
            else if (grade == PASS_USER)        /* �û��� */
            {
                if ((para_get_sign(n, m) & D_SAFEMASK) > D_USER)   para_item_hide = TRUE;
            }
        }
    }

    return  para_item_hide;
}
/*==========================================
    ��  ��: �����Ƿ�Ϊλ��
/==========================================*/
U08 para_is_bit(U08 n, U08 m)
{
    return (para_get_sign(n, m) & D_BIT) ? 1 : 0;
}
/*==========================================
    ��  ��: �����ܷ��޸�
    ����ֵ��0: ���޸�; 1: �����޸�
/==========================================*/
U08 para_is_nexch(U08 n, U08 m)
{
    U16 sign;

    sign = para_get_sign(n, m);

    if (sign & (D_NEXCH))
    {
        return 1;
    }

    return 0;
}

/********************************************
����: PG_Is_Show
����: �жϲ������Ƿ������ʾ
����: ParamGroup: �������ַ;         ParaGrade: ��ǰ����Ȩ�޼���
����ֵ: TRUE ������ʾ / FALSE ��������ʾ
*********************************************/
BOOL PG_Is_Show(const PARAMITEM *ParamGroup, U16 ParaGrade)
{
    BOOL AllowShow = TRUE;
    const PARAMITEM *pp1;
    U16 GroupSign, ParaSign;
    U08 i, ShowPara=0;

    pp1 = ParamGroup;
    GroupSign = pp1->sign ;     // ����������


    // �жϸò����鱾������Լ�Ȩ���Ƿ�������ʾ
    if (((GroupSign & D_SAFEMASK) > ParaGrade)      // ������Ȩ��>��ǰ����Ȩ�ޣ���������ʾ������
        || (D_HIDE == (GroupSign & D_HIDE)))        // ����������Ϊ����
    {
        AllowShow = FALSE;

    }
    else    // �жϸò����������в���������Լ�Ȩ���Ƿ�������ʾ
    {
//        GroupSign &= D_SAFEMASK;

        for (i = 0; i < pp1->items; i++)
        {
            ParaSign = pp1->array[i].sign;

            if (D_HIDE!=(ParaSign & D_HIDE))     // �в�������Ҫ��ʾ
            {
                ParaSign &= D_SAFEMASK;

                if (ParaGrade >= ParaSign)       // ��ǰ����Ȩ�� >= ������Ȩ�ޣ�������������ʾ
                {
                    ShowPara++;
                }
            }
        }
        if ( 0 == ShowPara) AllowShow = FALSE;   // û�в�������Ҫ��ʾ����ò���������

    }

    return AllowShow ;
}

/*==========================================
    ��  ��: Э�����޸Ĳ�����//NOTE-CXW����δ�ع���
/==========================================*/
U08 PARA_Modify(U08 *buf, U16 addr, U16 num)
{
    U16  n, m, k;
    I16  tmp;
    U16  n_real, m_real,tmp_real;

    if ((addr < ADDR_PARA_BGN) || (addr >= ADDR_PARA_END)) return 0;

    m = addr - ADDR_PARA_BGN;
    n = m >> PARA_BIN_BIT_NUM;              /* �ڼ��� */
    m = m & (PARA_SIGLE_NUM-1);             /* �ڼ��� */

    for (k=0; k<num; m++, k+=2)
    {
        tmp_real = m/PARA_SIGLE_NUM;       //��ֹһ�ο��д�����
        m_real = m - tmp_real*PARA_SIGLE_NUM;
        n_real = n + tmp_real;



        if (m_real >= ParamPri[n_real].items)  break;       /* �������������� */

        tmp = (I16)(((U16)buf[k] << 8) +  buf[k+1]);

        /* �ӻ������Ͳ�����������㲥�޸� */
        if (P_isP(n, m, val_mac_model))
        {
            MacTypeMaster = tmp;
            if (pcb_type == TYPE_SLAVER)    continue;
        }

        /* 1�����Ͳ���ͬʱ���ӻ������ܹ㲥������ز���
         * 2�����߼����������޸Ļ���Ϊ�ӻ����ͣ�����ӻ����չ㲥������ز���
        */
        if (!MACTYPE_IS_SAME && pcb_type==TYPE_SLAVER)
        {
            U16 idx;
            if (PARA_ParaWithinMacType(n, m, &idx))
            {
                continue;
            }
        }

        if (para_is_bit(n_real, m_real));      /* �����λ����(û�д�Сֵ����) */
        else
        {
            if (tmp < para_get_min_cent(n_real, m_real))  continue;
            if (tmp > para_get_max_cent(n_real, m_real))  continue;
        }

        if (para_is_nexch(n_real, m_real))  continue;      /*�������޸� */

//      if (para_is_hide(n, m, 0xff))   continue;       /* �������޸� */

        para_set_cent(n_real, m_real, tmp); /* �޸Ĳ��� */
    }

    return 1;
}

/*==========================================
    ��  ��: Э���ж�����ֵ��//NOTE-CXW����δ�ع���
/==========================================*/
U08 PARA_GetVal(U08 *buf, U16 addr, U16 num)
{
    U16  n, m, k;
    U16 *pData;

    U16  n_real, m_real,tmp_real;

    if ((addr < ADDR_PARA_BGN) || (addr >= ADDR_PARA_END)) return 0;

    m = addr - ADDR_PARA_BGN;
    n = m >> PARA_BIN_BIT_NUM;      /* �ڼ��� */
    m = m & (PARA_SIGLE_NUM-1);     /* �ڼ��� */

    for (k=0; k<num; m++)
    {
        tmp_real = m/PARA_SIGLE_NUM;       //��ֹһ�ο���ȡ����
        m_real = m - tmp_real*PARA_SIGLE_NUM;
        n_real = n + tmp_real;

        /* ���������ʱֻ�����϶� */
#ifdef  USE_TRAS_F_TEMPERATURE
        pData = (U16 *)ParamPri[n_real].data_f;
#else
        pData = (U16 *)ParamPri[n_real].data_c;
#endif

        buf[k++] = HIGH_BYTE(pData[m_real]);
        buf[k++] = LOW_BYTE(pData[m_real]);
    }
    return 1;
}

/******************************************************
������PARA_SetAttrib
���ܣ��ı��������
�������������(����); ������Ŀ��;��������ѡ��;����ֵ
����ֵ����
*******************************************************/
void PARA_SetAttrib(U08 n, U08 m, U16 *result)  //NOTE-CXW ����2017.4.19
{
    PARAM *pp2;

    pp2 = (PARAM *)ParamPri[n].array;
    *result = *(&(pp2[m].min)+3);

#ifndef USE_KIT_PARA_FAC
    /* �����в������� */
    if (*result&(D_RNEXCH) && StepIsRun)                            *result |= D_NEXCH;

    /* 00#��07#ģ������ */
    if (P_isP(n, m, val_cell_enable_00)
    || P_isP(n, m, val_cell_enable_01)
    || P_isP(n, m, val_cell_enable_02)
    || P_isP(n, m, val_cell_enable_03)
    || P_isP(n, m, val_cell_enable_04)
    || P_isP(n, m, val_cell_enable_05)
    || P_isP(n, m, val_cell_enable_06)
    || P_isP(n, m, val_cell_enable_07)
    )
    {
        if (m > P_idx(val_cell_enable_00)+val_pcb_num-1)            *result |= D_HIDE;
    }

    /* ����ģʽ */
    if (P_isP(n, m, sv_mode))
    {
        if(USE_COOL_ONLY || USE_HEAT_ONLY)                          *result |= D_NEXCH;
    }

    /* EC������� */
    if (P_isP(n, m, val_invt_fan_type))
    {
        if (val_freq_fan_set != FREQ_FAN_SET_EC)                    *result |= D_HIDE;
    }

    /* PWM������� */
    if (P_isP(n, m, val_fan_pwm_pulse))
    {
        if (!USE_PWM_FAN)                                           *result |= D_HIDE;
    }

    /* EC�����ַ */
    if (P_isP(n, m, val_ec_fan_base_addr))
    {
        if (!USE_EX_EC_FAN)                                         *result |= D_HIDE;
    }
#endif
}
/*==========================================
    ��  ��: Э���ж��������ԣ�//NOTE-CXW����δ�ع���
/==========================================*/
U08 PARA_GetAttrib(U08 *buf, U16 addr, U16 num)
{
    U16 n, m, k;
    U08 x;
    U16 temp;

    if ((addr < ADDR_PARA_INFO_BGN) || (addr >= ADDR_PARA_INFO_END))    return 0;

    m = addr - ADDR_PARA_INFO_BGN;
    n = m >> PARA_INFO_BIN_BIT_NUM;         /* �ڼ���(����) */
    m = m & (PARA_INFO_SINGLE_NUM - 1);     /* �ڼ���(��ʼ��ַ) */

    for (k=0; k<num; m++)
    {
        x = m/4;
        temp = 0;
        PARA_SetAttrib(n, x, &temp);

        buf[k++] = HIBYTE(temp);
        buf[k++] = LOBYTE(temp);
    }

    return 1;
}
/*==========================================
    ��  ��: Э���ж�������Ϣ��//NOTE-CXW����δ�ع���
/==========================================*/
U08 PARA_GetInfo(U08 *buf, U16 addr, U16 num)
{
    U16 m, k;
    U16 para_info[GROUP_MAX+1];

    if ((addr < ADDR_PARA_ITEM_BGN) || (addr >= ADDR_PARA_ITEM_END))    return 0;

    /* ��ʼ����������Ϣ */
    para_info[0] = PARA_GROUP_NUM;
    for (k=1; k<= PARA_GROUP_NUM; k++)
    {
        para_info[k] = ParamPri[k-1].items;     /* ���ֽ�Ϊ���� */
        para_info[k] |= ParamPri[k-1].sign & 0xff00;        /* ���ֽ�Ϊ���� */
    }

    m = addr - ADDR_PARA_ITEM_BGN;

    for (k=0; k<num; m++)
    {
        buf[k++] = HIGH_BYTE(para_info[m]);
        buf[k++] = LOW_BYTE(para_info[m]);
    }
    return 1;

}

/*********************************************
������para_broadcast
���ܣ��㲥�������ӻ���
�������ޡ�ʹ�ñ�־ParamPri[n].chip�� bit7 �ж��Ƿ���㲥.
���أ���
������ֻҪ�޸�1������,�͹㲥���顣
*********************************************/
U08 para_broadcast(U08 com)
{
    U08 n;
    U16 addr;
    U16 num;

    if (pcb_addr != 0)      return 0;           /* �����Ź㲥���� */

    for (n=0; n<PARA_GROUP_NUM; n++)
    {
        if (ParamPri[n].chip & PARA_NEED_STORE)     continue;   /* �÷ݲ�����δ���� */

        if (ParamPri[n].chip & PARA_NEED_BRO)               /* ��Ҫ�㲥 */
        {
            addr = ADDR_PARA_BGN + n * PARA_SIGLE_NUM;
            num = ParamPri[n].items;

            /* ��������㲥�������������־ */
            if (com_FBUS_bro(com, addr, num, Q_PRIO_HIGH))
            {
                ParamPri[n].chip &= (~PARA_NEED_BRO);
                return 1;
            }
        }
    }

    return 0;
}

/*==========================================
    ��  ��: ����㲥
    ��  ��: 0 - ��ʾû�й㲥��Ҫ
            ����ֵ - �㲥ͨѶ�Ļ������ݸ���
/==========================================*/
U08 m_password_broadcast(U08 com)
{
    U08 x;
    U16 addr, num;

    if (pcb_addr != 0)      return 0;           /* �����Ź㲥���� */

    /* 1�����һ�� */
    if (commu_m_password_broadcast_interval < 10) return 0;
    commu_m_password_broadcast_interval = 0;

    for (x = 0; x <16; x++)
    {
        if (GETBIT(passwd_broadcast, x))
        {
            num  = 4;
            addr = ADDR_PASS_BGN + (x << 2);

            /* ��������㲥�������������־ */
            if (com_FBUS_bro(com, addr, num, Q_PRIO_HIGH))
            {
                CLRBIT(passwd_broadcast, x);
                return 1;
            }
        }
    }

    return 0;
}

/*==========================================
    ���ܣ�ȡ����
    num : �ֽ���
/==========================================*/
U08 password_information(U08 *buf, U16 addr, U16 num, U08 opt)
{
    U16  m, k;
    U16 *pData;
    U16 tmp;

    if ((addr < ADDR_PASS_BGN) || (addr >= ADDR_PASS_END)) return 0;

    m = addr - ADDR_PASS_BGN;
    pData = (U16 *)&password;

    switch (opt)
    {
    case _GET_ :
        for (k=0; k<num; k += 2)
        {
            buf[k  ] = HIBYTE(pData[m]);
            buf[k+1] = LOBYTE(pData[m++]);
        }
        break;
    case _SET_ :
        if (m < 4 || m%4 != 0 || num != 8)  /* ��0�����벻�����޸�; �����벻�����޸�; ÿ��ֻ���޸�һ������ */
        {
            return 0;
        }

        for (k=0; k<num; k++)           /* ��0~~9֮����ַ�(����ַ�)���������޸� */
        {
            if ((buf[k] < '0' || buf[k] > '9') && buf[k] != 0)
            {

                return 0;
            }
        }

        for (k=0; k<num; k += 2)
        {

            tmp = MAKEWORD(buf[k], buf[k+1]);
            if (tmp != pData[m])
            {
                pData[m] = tmp;

                SETBIT(passwd_modify, m>>2);
                SETBIT(passwd_broadcast, m>>2); /* ÿ������ռ����4���ֵ�ַ */
            }
            m++;
        }
        break;
    }
    return 1;
}

/*******************************************************************************
 �� ��: para_check()
 �� ��: �������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void para_check(void)
{
    // ���ܲ�����ȡ��У�飬ʧ�ܱ�������ʼ���ڴ����
    DataAccess_FacPara(DATA_READ);

    if (fg_seep_crc_err || fg_seep_overflow)
    {
        if (pcb_addr == 0)
        {
            m_set_error(MAX_PCB, M_ERR_EEPROM);  /* ����EEPROM���ݴ�ͣ���� */
        }
        else
        {
            s_set_error(S_ERR_EEPROM);         /* �ӻ�EEPROM���ݴ�ͣģ�� */
        }
    }
#ifdef USE_SW_MAC
    else					/* ��eeprom����ʱ�Ŵ��� */
    {
        para_set_cent(P_grp(val_mac_model), P_idx(val_mac_model), sw_mac_model);
    }
#endif

    //��ϵͳ��ʱ��ϵͳά������ zzl2018.07.31
    // ϵͳά��������ȡ��У�飬ʧ�ܱ�������ʼ���ڴ����
    DataAccess_LimitPara(DATA_READ);

    if (fg_limit_crc_err || fg_limit_overflow)
    {
        if (pcb_addr == 0)
        {
            m_set_error(MAX_PCB, M_ERR_EEPROM_LIMIT);  /* ����ϵͳά�����ݴ�ͣ���� */
        }
        else
        {
            fg_limit_crc_err  = FALSE;
            fg_limit_overflow = FALSE;         /* �ӻ�������ϵͳά�����ݴ� */
        }
    }
    // ����ͨѶ������ȡ��У�飬ʧ�ܲ�������ֱ�ӳ�ʼ��
//    DataAccess_ModbusPara(DATA_READ);
}

/*******************************************************************************
 �� ��: para_check_invt_value()
 �� ��: ͬ����Ƶ����ֵ
 �� ��: ��
 �� ��: ��
 �� ��: ����ֵ��һ��ʱ��ѹ���ͺ��Կ�����Ϊ׼��ת���������Ա�Ƶ��Ϊ׼
*******************************************************************************/
void para_check_invt_value(void)
{
#ifndef USE_KIT_PARA_FAC
    I16 para_value = INVT_PARA_VALUE_NULL;

    if (val_inverter_used)
    {
        INVT_ID_ENUM id = INVT_ID_1;
        // 0 ����ͺ�
        para_value = invt_para_read(id, INVT_PARA_MOTOR_TYPE);
        if(para_value != INVT_PARA_VALUE_NULL && para_value != val_invt_motor_type)
        {
            invt_para_set(id,INVT_PARA_MOTOR_TYPE, val_invt_motor_type);
        }
        // 1 ����ת������
        para_value = invt_para_read(id, INVT_PARA_FREQ_MIN);
        if(para_value != INVT_PARA_VALUE_NULL && (para_value/100)*100 != val_invt_rps_min)
        {
            para_set_cent(GROUP_COMP, IDX_INVT_MIN, para_value / 100);
        }
        // 2 ����ת������
        para_value = invt_para_read(id, INVT_PARA_FREQ_MAX);
        if(para_value != INVT_PARA_VALUE_NULL && (para_value/100)*100 != val_invt_rps_max)
        {
            para_set_cent(GROUP_COMP, IDX_INVT_MAX, para_value / 100);
        }
        // ��������ת��
        {
            I16 para_now = invt_para_read(id, INVT_PARA_FREQ_START);
            para_value = val_invt_start_freq*invt_para_read(id, INVT_PARA_POLE)/2;
            if (para_now != INVT_PARA_VALUE_NULL && para_now != para_value)
            {
                invt_para_set(id, INVT_PARA_FREQ_START, val_invt_start_freq);
            }
        }
#if 0
        // EC���PWM����Ƶ��
        para_value = invt_para_read(id, INVT_PARA_EC_PWM_SW_FREQ);
        if(para_value != INVT_PARA_VALUE_NULL && para_value != val_F6_00_set)
        {
            invt_para_set(id, INVT_PARA_EC_PWM_SW_FREQ, val_F6_00_set);
        }
#endif
    }
    /* N0004 */
    if (val_freq_fan_set == FREQ_FAN_SET_EC)
    {
        /* EC���1�ͺ� */
        para_value = invt_para_read(INVT_FAN1, INVT_PARA_MOTOR_TYPE);
        if(para_value != INVT_PARA_VALUE_NULL && para_value != val_invt_fan_type)
        {
            invt_para_set(INVT_FAN1,INVT_PARA_MOTOR_TYPE, val_invt_fan_type);
        }

        /* EC���2�ͺ� */
        para_value = invt_para_read(INVT_FAN2, INVT_PARA_MOTOR_TYPE);
        if(para_value != INVT_PARA_VALUE_NULL && para_value != val_invt_fan_type)
        {
            invt_para_set(INVT_FAN2,INVT_PARA_MOTOR_TYPE, val_invt_fan_type);
        }
    }
#endif
}



/*
*******************************************************************************
    ���� :  Task_ID_Init()
    ���� :  ��EEPROM��д��ʶ����

    ���� :  --
    ���� :  --
*******************************************************************************
*/
void Task_ID_Init(void)
{
    U08 i;
    U08 buf_id[MAX_ID];
    const U08 * pIDCode;

    pIDCode = PCB_ID_CODE;  /* ʶ���� */
    for (i = 0; i < MAX_ID; i++) buf_id[i] = pIDCode[i];

    //wreeprom(buf_id, MEM_ID_CODE_ADDR, MAX_ID);
    eprom_wr_near_crc(buf_id, MEM_ID_CODE_ADDR, MAX_ID);
}

/*
*******************************************************************************
    ���� :  Task_ID_Read()
    ���� :  ��EEPROM�ж�ȡʶ���룬���жϳ����Ƿ�֧�ָ�ʶ����
    ���� :  --
    ���� :  --
*******************************************************************************
*/
void Task_ID_Read(void)
{
    U08 i;
    U08 buf_id[MAX_ID];
    const U08 * pIDCode;

    //rdeeprom(buf_id, MEM_ID_CODE_ADDR, MAX_ID, 0);
    eprom_rd_near_crc(buf_id, MEM_ID_CODE_ADDR, MAX_ID);

    pIDCode = PCB_ID_CODE;  /* ʶ���� */
    for (i = 0; i < MAX_ID; i++)
    {
        if (buf_id[i] != pIDCode[i])
        {
            //s_set_error(ERR_ID_CODE);  /* ʶ������󣬳��������� */
            break;
        }
    }
}
/********************************************************************************
    ���� :  DataAccess_EEPROM()
    ���� :  EEPROM���ݷ���
            �����ݣ�����ַ�����ݣ�2�ζ�ʧ�����Զ���ʼ��
            ��ʼ������ͬһ��ֵ��ʼ����������
            д���ݣ�����ַд����
    ���� :  buf�����ڴ�����ݵı����� addr����д��EEPROM��ַ�� num����д���ֽڸ�����
            InitValue����ʼ��ֵ��     opt������
    ���� :  ��
    ��ע :
********************************************************************************/
void DataAccess_EEPROM(U08 *buf, const EEDATA *pDataInfo, U08 InitValue, U08 opt)
{
    U16 i;
    U16 addr, num;

    addr = pDataInfo->Addr;
    num  = pDataInfo->DataNum;

    switch (opt)
    {
        case DATA_READ :    // ��ȡ

            if (eprom_rd_near_crc(buf, addr, num)) break;
            if (eprom_rd_near_crc(buf, addr, num)) break;
            // ���ζ�ȡ���������³�ʼ��
            // break;

        case DATA_INIT :    // ��ʼ��

            for (i = 0; i < num; i++)
            {
                buf[i] = InitValue;
            }
            // break;

        case DATA_WRITE :   // ����

            eprom_wr_near_crc(buf, addr, num);
            break;
    }
}

/*****************************************************
    ���ܣ�д����ǰʱ�䵽EEPROM
******************************************************/
void pwd_time_write_to_eeprom(void)
{
    U16 addr;

    addr = MEM_PWD_TIME_ADDR ;
    eprom_wr_near_crc((U08*)&clock[hmi_t_num], addr, sizeof(LOG_RTC));
}
/*****************************************************
    ���ܣ���EEPROM������ǰʱ��
******************************************************/
void pwd_time_read_from_eeprom(void)
{
    U16 addr;

    addr = MEM_PWD_TIME_ADDR;

    if (eprom_rd_near_crc((U08*)&power_down_time_last, addr, sizeof(LOG_RTC)) == FALSE
    ||  !fun_rtc_check(power_down_time_last)
        )
    {
        VALCLR(power_down_time_last);
    }
}


void DataAccess_PowerUsageWeek(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;

    EEDATA DataInfo;
    DataInfo.DataNum = POWER_USAGE_WEEK_BYTES;
    DataInfo.Addr = MEM_POWER_USAGE_WEEK_ADDR;

    buf = (U08 *)&pwru_week_cache;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

void DataAccess_PowerUsageYear(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;

    EEDATA DataInfo;
    DataInfo.DataNum = POWER_USAGE_YEAR_BYTES;
    DataInfo.Addr = MEM_POWER_USAGE_YEAR_ADDR;

    buf = (U08 *)&pwru_year_cache;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    ���� :  DataAccess_SkipPreheatCnt()
    ���� :  ���ݷ���-����Ԥ�ȼ���
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_SkipPreheatCnt(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_SKIP_PREHEAT_ADDR;

    buf = &skip_preheat_cnt;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}



/********************************************************************************
    ���� :  DataAccess_Language()
    ���� :  ���ݷ���-����
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_Language(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = RN_LANGUAGE_EN; // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_LANGUAGE_ADDR;

    buf = &sh_language;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    ���� :  DataAccess_Temp_unit()
    ���� :  ���ݷ���-����
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_Temp_unit(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = RN_TEMP_UNIT_C; // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_TEMP_UNIT_ADDR;

    buf = &sh_temp_unit;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    ���� :  DataAccess_AUTO_RUN()
    ���� :  ���ݷ���-����״̬
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_AUTO_RUN(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;   // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_AUTO_RUN_ADDR;

    buf = &auto_run_bit;


    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    ���� :  DataAccess_CompRuntime()
    ���� :  ���ݷ���-��ģ��ѹ���ۼ�����ʱ��
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_CompRuntime(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_RUNTIME<<1;
    DataInfo.Addr = MEM_RUNTIME_ADDR;

    buf = (U08 *)&pCell->cell_runtime[0];


    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    ���� :  DataAccess_SysRuntime()
    ���� :  ���ݷ���-ϵͳ�ۼ�����ʱ��
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_SysRuntime(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_RUNTIME<<1;
    DataInfo.Addr = MEM_SYSTIME_ADDR;

    buf = (U08 *)&cell[MAX_PCB].cell_runtime[0];


    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}


/********************************************************************************
    ���� :  DataAccess_HistoryErr()
    ���� :  ���ݷ���-��ʷ����
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_HistoryErr(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = F_NO_ERR;   // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_HIS_ERR_TBL;
    DataInfo.Addr = MEM_HIS_ERROR_ADDR;

    buf = his_err_tbl;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}
/********************************************************************************
    ���� :  DataAccess_InvtExceptInfo()
    ���� :  ���ݷ���-��Ƶ�쳣��Ϣ
    ���� :  opt:�����룻idx:�±꣬Ϊ���ֵʱ��ʾ����ȫ��
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_InvtExceptInfo(U08 opt, U08 idx)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // ��ʼ��ֵ

    EEDATA DataInfo;
    U08 idx_bgn=0, idx_end=0;
    U08 x = 0;

    if (idx == _MAX_(idx))
    {
        idx_bgn = 0;
        idx_end = INFO_MAX;
    }
    else
    {
        idx_bgn = idx;
        idx_end = idx+1;
    }

    for (x=idx_bgn; x<idx_end; x++)
    {
        /* ��ַû������4����Ĵ�������ĳЩeeprom���ܴ��ڼ��������⣬����stm8��Ƭ��eeprom */
        DataInfo.DataNum = sizeof(info_invt_except[0]);
        DataInfo.Addr = MEM_INVT_EXP_INFO_ADDR+x*(DataInfo.DataNum+2);  /* һ����Ϣ��С+2�ֽ�CRC */
        buf = (U08 *)&info_invt_except[x];
        DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
    }
}

/********************************************************************************
    ���� :  DataAccess_Timer()
    ���� :  ���ݷ���-��ʱ��
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_Timer(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_TIME_TBL;
    DataInfo.Addr = MEM_DM603_TIME_ADDR;

    buf = dm603_time_tbl;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/*******************************************************************************
 �� ��: DataAccess_holiday()
 �� ��: �ݼ�ģʽ����
 �� ��: opt--;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void DataAccess_holiday(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // ��ʼ��ֵ

    EEDATA DataInfo;
    DataInfo.DataNum = HOLIDAY_MAX;
    DataInfo.Addr = MEM_HOLIDAY_ADDR;
    if (opt == DATA_INIT)
    {
        // ���ݳ�ʼ��
        dm603_holiday_tbl[HOLIDAY_USED] = 0;
        dm603_holiday_tbl[HOLIDAY_HEAT_SET] = 25;
        dm603_holiday_tbl[HOLIDAY_HW_SET] = 25;
        dm603_holiday_tbl[HOLIDAY_START_YEAR] = 0;
        dm603_holiday_tbl[HOLIDAY_START_MONTH] = 1;
        dm603_holiday_tbl[HOLIDAY_START_DAY] = 1;
        dm603_holiday_tbl[HOLIDAY_END_YEAR] = 0;
        dm603_holiday_tbl[HOLIDAY_END_MONTH] = 1;
        dm603_holiday_tbl[HOLIDAY_END_DAY] = 2;
        opt = DATA_WRITE;
    }
    buf = dm603_holiday_tbl;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    ���� :  DataAccess_Password()
    ���� :  ���ݷ���-����
    ���� :
    ���� :
    ��ע :
********************************************************************************/
const U08 InitPassWord[MAX_PASS_GROUP][MAX_PASS_WORD]=
{
    {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},  /* ��̬���� */
    {'1','2','3','4','5','6',0,0},              /* �������� */
    {'1','2','3','4','5',0,0,0},                /* ά�������� */
    {'1','2','3','4',0,0,0,0},                  /* �������� */
    {'1','2','3',0,0,0,0,0},                    /* �û����� */
    {'6','6','6','6','6','6','6','6'},          /* ʹ���������� */
};
void DataAccess_OnePassword(U08 *buf, U08 index, U08 opt)
{
    U16 i;
    U16 SizeWithCRC;
    EEDATA DataInfo;

    DataInfo.DataNum = MAX_PASS_WORD;
    SizeWithCRC = GetSize_Byte(DataInfo.DataNum, WITH_CRC);     // һ������ռ�ô洢�ռ䣨����CRC��
    DataInfo.Addr = MEM_PASSWORD_ADDR + index * SizeWithCRC;    // ��������ʵ�ʵ�ַ

    switch (opt)
    {
        case DATA_READ :    // ��ȡ

            if (eprom_rd_near_crc(buf, DataInfo.Addr, DataInfo.DataNum)) break; // ��ȡ�ɹ����˳�
            if (eprom_rd_near_crc(buf, DataInfo.Addr, DataInfo.DataNum)) break;
            // ���ζ�ȡ���������³�ʼ��
            // break;

        case DATA_INIT :    // ��ʼ��

            for (i = 0; i < MAX_PASS_WORD; i++)
            {
                buf[i] = InitPassWord[index][i];
            }
            // break;

        case DATA_WRITE :   // ����
            SET_BIT(passwd_modify, index);
            SET_BIT(passwd_broadcast, index);
            break;
    }

}

/********************************************************************************
    ���� :  Password_ReadAll()
    ���� :  ��ȡ��������
    ���� :
    ���� :
    ��ע :  ����ŵ��ô˺���
********************************************************************************/
void Password_ReadAll()
{
    U08 x = 0;
    for(x=0; x<MAX_PASS_GROUP; x++)
    {
        DataAccess_OnePassword((U08 *)password[x],x, DATA_READ);
    }
    passwd_modify = 0;
    passwd_broadcast = 0xffff;  /* �ϵ�ʱ�ù㲥λ */
}

/********************************************************************************
    ���� :  Password_InitAll()
    ���� :  ��ʼ����������
    ���� :
    ���� :
    ��ע :  ����ŵ��ô˺���
********************************************************************************/
void Password_InitAll()
{
    U08 x = 0;
    for(x=0; x<MAX_PASS_GROUP; x++)
    {
        DataAccess_OnePassword((U08 *)password[x],x, DATA_INIT);
    }
}
/********************************************************************************
    ���� :  Password_super_cal()
    ���� :  ������֤����㶯̬����
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void Password_super_cal(U16 Passcode)
{
    U08 i;
    U08 buf[2];
    W16 crc1, crc2;

    crc1.word = Passcode / 24;

    buf[0] = crc1.byte.high;    buf[1] = crc1.byte.low;
    crc1.word = CRC16(buf, 2);

    buf[0] = crc1.byte.high;    buf[1] = crc1.byte.low;
    crc2.word = CRC16(buf, 2);

    password[PASS_SUPER][0] = HIGH4_BIT(crc1.byte.high);
    password[PASS_SUPER][1] = LOW4_BIT (crc1.byte.high);
    password[PASS_SUPER][2] = HIGH4_BIT(crc1.byte.low);
    password[PASS_SUPER][3] = LOW4_BIT (crc1.byte.low);
    password[PASS_SUPER][4] = HIGH4_BIT(crc2.byte.high);
    password[PASS_SUPER][5] = LOW4_BIT (crc2.byte.high);
    password[PASS_SUPER][6] = HIGH4_BIT(crc2.byte.low);
    password[PASS_SUPER][7] = LOW4_BIT (crc2.byte.low);



    for (i=0; i<MAX_PASS_WORD; i++) // ����������7���ڲ�ת���ַ�
    {
        password[PASS_SUPER][i] &= 0x07;
        password[PASS_SUPER][i] += '0';
    }
}


/********************************************************************************
    ���� :  DataStore_EEPROM()
    ���� :  ��������������ݰ���־�����洢
    ���� :
    ���� :
    ��ע :  ����������ʹ���޸ı�־������
********************************************************************************/
void DataStore_EEPROM()
{
    U16 x,SizeWithCRC;
    EEDATA DataInfo;


    // ��������洢
    if (passwd_modify != 0)
    {
        for (x=0; x<MAX_PASS_GROUP; x++)
        {
            if (GETBIT(passwd_modify, x))
            {
                DataInfo.DataNum = MAX_PASS_WORD;
                SizeWithCRC = GetSize_Byte(DataInfo.DataNum, WITH_CRC);     // һ������ռ�ô洢�ռ䣨����CRC��
                DataInfo.Addr = MEM_PASSWORD_ADDR + x * SizeWithCRC;        // ��������ʵ�ʵ�ַ
                if(eprom_wr_near_crc((U08 *)password[x], DataInfo.Addr, DataInfo.DataNum))
                {
                    if(x==PASS_FAC) ValidCode = 0;  // ������������޸ĳɹ��������֤�루��
                }
                CLRBIT(passwd_modify, x);

                break;  // ÿ��ֻ�洢һ������
            }
        }
        return; // ÿ��ֻ�洢һƬ����
    }
}

/********************************************************************************
    ���� :  DataAccess_LimitPara()
    ���� :  ���ݷ���-���ܲ���
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_FacPara(U08 opt)
{
    U08 GroupNum = 0;

    GroupNum=(sizeof(ParamPri) / sizeof(PARAMITEM));
    PARA_GROUP_NUM = GroupNum;

    switch (opt)
    {
        case DATA_READ :    // ��ȡ
            if(DataRead_ParaGroups(ParamPri, 0, GroupNum, GroupNum, &ParamPri_DataInfo)==TRUE)// ��ȡ�ɹ������г�����֤
            {
                if(DataVerify_ParaGroups(ParamPri, 0, GroupNum)==FALSE) // ��֤ʧ��
                {
                     fg_seep_overflow = TRUE;
                }
            }
            else    // ��ȡʧ��
            {
                fg_seep_crc_err = TRUE;
            }

            if (fg_seep_crc_err==FALSE && fg_seep_overflow==FALSE) // ��ȡ�ɹ�ֱ�ӷ���,����ס��ִ��DATA_RESET����λ�ڴ�����
            {
                break;
            }

            #ifdef USE_NO_PARA_RESET
                break;
            #endif

        case DATA_RESET :    // ��λ�ڴ�����
            DataReset_ParaGroups(ParamPri, 0, GroupNum, GroupNum, ParamPri_DataInfo.DataNum);
            break;


        case DATA_INIT :    // ��ʼ��
            DataReset_ParaGroups(ParamPri, 0, GroupNum, GroupNum, ParamPri_DataInfo.DataNum);
            if (ACTION_ING!=GET_INIT_PARA())
            {
                SET_INIT_PARA(ACTION_START);   /*  ��ʼ����ʼ */
            }
            // break;

        case DATA_WRITE :   // �ñ����־
            FlagParaGroup_AllStore(ParamPri, 0, GroupNum, GroupNum);
            break;

    }
}


/********************************************************************************
    ���� :  DataAccess_LimitPara()
    ���� :  ���ݷ���-ϵͳά������
    ���� :
    ���� :
    ��ע :
********************************************************************************/
void DataAccess_LimitPara(U08 opt)
{
    U08 GroupNum = 0;

    GroupNum=(sizeof(ParamLimit) / sizeof(PARAMITEM));

    switch (opt)
    {
        case DATA_READ :    // ��ȡ
            if(DataRead_ParaGroups(ParamLimit, 0, GroupNum, GroupNum, &Limit_DataInfo)==TRUE)// ��ȡ�ɹ������г�����֤
            {
                if(DataVerify_ParaGroups(ParamLimit, 0, GroupNum)==FALSE)   // ��֤ʧ��
                {
                     fg_limit_overflow = TRUE;
                }
            }
            else    // ��ȡʧ��
            {
                fg_limit_crc_err = TRUE;
            }
            if (fg_limit_crc_err==FALSE && fg_limit_overflow==FALSE) // ��ȡ�ɹ�ֱ�ӷ���,����ס��ִ��DATA_RESET����λ�ڴ�����
            {
                break;
            }

        case DATA_RESET :    // ��λ�ڴ�����
            DataReset_ParaGroups(ParamLimit, 0, GroupNum, GroupNum, Limit_DataInfo.DataNum);
            break;

        case DATA_INIT :    // ��ʼ��
            DataReset_ParaGroups(ParamLimit, 0, GroupNum, GroupNum, Limit_DataInfo.DataNum);
            if (ACTION_ING!=GET_INIT_PARA())
            {
                SET_INIT_PARA(ACTION_START);   /*  ��ʼ����ʼ */
            }
            // break;

        case DATA_WRITE :   // �ñ����־
            FlagParaGroup_AllStore(ParamLimit, 0, GroupNum, GroupNum);
            break;
    }
}

/*****************************************************
������limit_time_write_to_eeprom
���ܣ�дʹ�����޵�EEPROM
��������
����ֵ����
******************************************************/
void eeprom_limit_time_write(void)
{
    U16 addr;

    if (limit_val_bak != limit_value)
    {
        limit_val_bak = limit_value;

        addr = MEM_LIMIT_ADDR ;

        eprom_wr_near_crc((U08 *)limit_para_C, addr, MAX_LMT<<1);
    }
}

/*******************************************************************************
 �� ��: para_get_dot()
 �� ��:
 �� ��: grp_id--;
        item_id--;
 �� ��:
 �� ��: ��
*******************************************************************************/
U08 para_get_dot(U08 grp_id, U08 item_id)
{
    return (ParamPri[grp_id].array[item_id].sign & D_DOTMASK);
}


/*******************************************************************************
 �� ��: para_AT_use_get()
 �� ��: ��ȡ����������ѡ��
 �� ��: ��
 �� ��: ��������ѡ��
 �� ��: ��
*******************************************************************************/
I16 para_AT_use_get(void)
{
    #define AT_ORIGIN_IDX_BGN AT_HEAT_A				/* ԭʼ�����߽���ʼ */
    #define AT_ORIGIN_IDX_END AT_COOL				/* ԭʼ�����߽���� */
    
    I16 result = val_ability_test_set;

    /* Ԥ���鹤�����תΪԭʼ������� */
    if (result > AT_ORIGIN_IDX_END)
    {
        result = (result-AT_MAX)%AT_HT_WC_NUM+AT_ORIGIN_IDX_BGN;
    }

    /* �߽籣�� */
    result = fun_pv_limit(result, AT_UNUSED, AT_ORIGIN_IDX_END);

    return result;
}

#ifdef AT_MAX_NEW
/*******************************************************************************
 �� ��: para_AT_get_group_ht()
 �� ��: ��ȡ�����������-����
 �� ��: ��
 �� ��: �����������-����
 �� ��: ��
*******************************************************************************/
U08 para_AT_get_group_ht(void)
{
    U08 group = AT_HT_GP_ORIGIN;
    
    if (val_ability_test_set >= AT_MAX)
    {
        group = (val_ability_test_set-AT_MAX)/AT_HT_WC_NUM;	        /* Ԥ����� */
    }

    return group;
}

/*******************************************************************************
 �� ��: para_AT_update_ht()
 �� ��: ���������������Ȳ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void para_AT_update_ht(void)
{
    U08 i = 0;
    U08 group = para_AT_get_group_ht();		                /* �����������-���� */
    U08 para_grp = P_grp(val_ability_test_set);				/* Ԥ������Ĳ������ */
    U08 para_idx[AT_HT_PARA_NUM] = { 1,  2,  3,  4,  5,  6,  7,
                                      8,  9, 10, 11, 12, 13, 14,
                                     15, 16, 17, 18, 22, 23, 24,
                                     25, 26, 27, 28, 29, 30, 31,
                                     32, 33, 34, 35, 36, 37, 38,
                                     39, 40, 41, 42, 43, 44, 45,
                                    };  /* Ԥ������Ĳ������ */

    if (group == AT_HT_GP_ORIGIN)			/* ԭʼ�� */
    {
        U16 idx = 0;

        for (i=0; i<AT_HT_PARA_NUM; i++)
        {
            if (PARA_ParaWithinMacType(para_grp, para_idx[i], &idx))/* �ڻ��Ͳ������� */
            {
                para_set_cent(para_grp, para_idx[i], para_fac[idx].para[val_mac_model]);
            }
            else			/* ��ͨ�ò������� */
            {
                para_set_cent(para_grp, para_idx[i], ParamPri[para_grp].array[para_idx[i]].init);
            }
        }  
    }
    else					                /* Ԥ���� */
    {
        for (i=0; i<AT_HT_PARA_NUM; i++)
        {
            para_set_cent(para_grp, para_idx[i], AT_presets[group][i][val_mac_model]);
        }  
    }
}
#endif
