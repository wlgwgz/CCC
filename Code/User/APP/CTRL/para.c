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

/**使用期限功能**************************************************/

const PARAM para_item_90[] =    /* 系统维护 */
{
    {   0  ,        0  ,        1365 ,      UINT_D,     },   /* 系统维护(天) */
    {   0  ,        0  ,        1 ,         UINT_0,     },   /* 系统维护类型 */
};

PARAMITEM  ParamLimit[] =
{
    { sizeof(para_item_90)/sizeof(PARAM),  (PARAM *)para_item_90,  limit_para_C,  limit_para_F,  0x80,  0,  0,  D_USER          },
};
const EEDATA Limit_DataInfo={MEM_LIMIT_ADDR, (MAX_LMT<<1)};


/*=================================================================================================
                    温度不同单位的换算
/=================================================================================================*/
/*******************************************************************************
 函 数: F_To_C()
 功 能: 由华氏转成摄氏C = (F-32)*5/9
 参 数: F--华氏温度;
		sign--;
 返 回: 摄氏温度(0.1)
 其 它: 空
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
 函 数: C_To_F()
 功 能: 由摄氏转成华氏F = 9*C/5 +32;
 参 数: C--摄氏温度;
		sign--;
 返 回: 华氏温度(0.1)
 其 它: 空
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
//函数：GetSize_Byte
//功能：获取并返回一组数据包含/不包含CRC的空间大小
//参数：DataSize_byte：数据的字节数；opt：是否包含CRC大小
//返回值：地址
//说明：用此函数是为了方便以后更改CRC相关内容
//**********************************************/
#define NO_CRC      0   // 不包含CRC的大小
#define WITH_CRC    1   // 包含CRC

static U16 GetSize_Byte(U16 DataSize_byte, U08 opt)
{
    U16 Size_byte, CRC_size;  // 字节大小


    CRC_size = (WITH_CRC==opt) ? (CRC_SIZE<<1) : 0 ;
    Size_byte = DataSize_byte + CRC_size;

    return Size_byte;
}

///**********************************************
//函数：DataRead_OneParaG
//功能：读取单组参数，并返回执行结果
//参数：ParamGroup_act：要读取的参数组指针； DataInfo：参数组存储信息（起始地址、个数）
//返回值：TRUE/FALSE 成功/失败
//**********************************************/
static U08 DataRead_OneParaG(const PARAMITEM *ParamGroup_act, EEDATA DataInfo)
{
    U08 *ptr;

    ptr = (U08 *)ParamGroup_act->data_f;                /* 参数数据指针 */

    return eprom_rd_near_crc(ptr, DataInfo.Addr, DataInfo.DataNum);
}


///**********************************************
//函数：DataRead_ParaGroup
//功能：读取指定组的参数，并返回执行结果
//参数：GroupBegin：起始组; GroupNum：需访问的组数; GroupAllNum 实际组数; pDataInfo：存储首地址及每组预留数据字节个数
//返回值：FALSE-读取失败，TRUE-读取成功
//**********************************************/
U08 DataRead_ParaGroups(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, U08 GroupAllNum, const EEDATA *pDataInfo)    //NOTE-CXW 待测2017.4.11 修改了地址和字节计算
{
    U08 i, j;
    U08 fg_ReadOK= TRUE;    // 读取成功（FALSE-读取失败，TRUE-读取成功）
    U08 RepeatEnd=0, itemNum=0;

    PARAMITEM *pGroup;      // 参数组
    PARAM *pp2;             // 参数属性
    I16  *pp1_f;            // 华氏
    I16  *pp1_c;            // 摄氏
    EEDATA DataInfoAll;     // 整片数据存储信息
    EEDATA DataInfoEach;    // 单组参数存储信息

    RepeatEnd = GroupNum+GroupBegin;        // 结束

    DataInfoAll.Addr     = pDataInfo->Addr;
    DataInfoAll.DataNum  = GetSize_Byte(pDataInfo->DataNum, WITH_CRC);      // 每组预留字节个数（包含CRC）

    for (i=GroupBegin; i<RepeatEnd; i++)
    {
        if(i>=GroupAllNum)  // 要访问的组超出了实际的组数，返回不处理
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

        DataInfoEach.Addr = DataInfoAll.Addr+i*DataInfoAll.DataNum;     // 单组参数存储地址
        DataInfoEach.DataNum = itemNum<<1;                              // 单组参数实际使用字节个数（不包括CRC）

        if(DataInfoEach.DataNum > pDataInfo->DataNum)  // 要访问的参数项数量超出了预留的参数项（不包括CRC），返回不处理
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
            pp1_c[j] = F_To_C(pp1_f[j], pp2[j].sign);   // 转换数据
        }
    }

    return fg_ReadOK;
}

/*******************************************************************************
 函 数: set_para_modify_flag()
 功 能: 置指定参数组的修改标识，并确认修改范围
 参 数: n--;
		m--;
 返 回:
 其 它: 空
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
函数：DataReset_ParaArray
功能：初始化参数临时变量
参数：ParamGroup-要初始化的参数组地址, opt-初始化后是否保存到EEPROM
返回值：无
*********************************************/
#define NOT_SAVE_TO_EEPROM  0   // 不保存到EEPROM
#define SAVE_TO_EEPROM      1   // 需保存到EEPROM


///**********************************************
//函数: DataReset_ParaGroups
//功能: 按默认值初始化参数内存变量
//参数: PARAMITEM *ParamGroup: 指定参数片指针;     GroupBegin：起始组; GroupNum：需访问的组数;
//      GroupAllNum: 该参数片的实际组数，防止超界; ItemAllByte:该参数片的每组预留参数项字节数量，防止超界
//返回值: --
//**********************************************/
void DataReset_ParaGroups(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, U08 GroupAllNum, U08 ItemAllByte)//, U08 opt)
{
    U08 i, j;
    U08 RepeatEnd=0, itemNum=0;
    U16 idx;
    I16 mac_init;

    PARAMITEM *pGroup;      // 参数组
    PARAM *pp2;             // 参数属性
    I16  *pp1_f;            // 华氏
    I16  *pp1_c;            // 摄氏

    RepeatEnd = GroupNum+GroupBegin;        // 结束

    for (i=GroupBegin; i<RepeatEnd; i++)    // 参数错误，初始化临时变量，但不保存到EEPROM
    {
        if(i>=GroupAllNum)  // 要访问的组超出了实际的组数，返回不处理
        {
            Unknown_Exception[DATA_EXCEPTION] = DATA_BREAK_BOUNDS;
            break;
        }
        pGroup = (PARAMITEM*)&ParamGroup[i];
        pp2 = (PARAM *)pGroup->array;
        itemNum = pGroup->items;
        pp1_c  = (I16 *)pGroup->data_c;
        pp1_f  = (I16 *)pGroup->data_f;

        if((itemNum<<1) > ItemAllByte)  // 要访问的参数项数量超出了预留的参数项（不包括CRC），返回不处理
        {
            Unknown_Exception[DATA_EXCEPTION] = DATA_BREAK_BOUNDS;
            break;
        }

        for (j=0; j<itemNum; j++)
        {
            /* 以para_fac中的参数默认值为准，应对para_fac与ParamPri中的默认值不同的情况 */
            if ((ParamGroup == ParamPri) && PARA_ParaWithinMacType(i, j, &idx))
            {
#ifdef USE_SW_MAC
                mac_init = sw_mac_model;
#else
                /* 这里用机型的默认值而不用当前机型是因为需要初始化，包括机型参数本身也会被初始化 */
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
            pp1_f[j] = C_To_F(pp1_c[j], pp2[j].sign);   /* 参数是温度值 */
        }
    }
}


/********************************************************************************
    函数 :  Judge_InitPara()
    功能 :  处理参数初始化状态

    参数 :  fg_NeedStore 是否有需要存储
    返回 :  无
    备注 :
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
            SET_INIT_PARA(ACTION_FAIL) ;    // 初始化失败
        }
    }

    if (FALSE == fg_NeedStore)                        // 没有需要保存的数据
    {
        if (ACTION_ING == GET_INIT_PARA())
        {
            SET_INIT_PARA(ACTION_SUCC);
        }
        return;
    }
}

/*==========================================
函 数: FlagParaGroup_AllStore
功  能: 标识参数组保存标志，用于直接保存单个参数组
参 数: PARAMITEM *ParamGroup: 指定参数片指针; GroupBegin：起始组; GroupNum：需访问的组数; GroupAllNum 实际组数;
返 回: 无
/==========================================*/
void FlagParaGroup_AllStore(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, U08 GroupAllNum)
{
    U08 i;
    U08 RepeatEnd=0;

    PARAMITEM *pGroup;      // 参数组


    RepeatEnd = GroupNum+GroupBegin;        // 结束

    for (i=GroupBegin; i<RepeatEnd; i++)
    {
        if(i>=GroupAllNum)  // 要访问的组超出了实际的组数，返回不处理
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
函数：DataVerify_ParaArray
功能：判断单组参数超界
参数：
返回值：fg_para_ok: TRUE正常， FALSE超界
**********************************************/
BOOL DataVerify_ParaGroups(PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum)
{
    U08 i, j;
    U08 RepeatEnd=0, Num=0;
//    static U08 tmp_index=0;
    PARAMITEM *pGroup;      // 参数组
    PARAM *pp2;                     // 参数属性

    BOOL fg_para_ok = TRUE;         // 参数正常标志


    RepeatEnd = GroupNum+GroupBegin;        // 结束

    for (i=GroupBegin; i<RepeatEnd; i++)
    {
        pGroup = (PARAMITEM*)&ParamGroup[i];
        pp2 = (PARAM *)pGroup->array;
        Num = pGroup->items;

        for (j=0; j<Num; j++)
        {
            if (para_is_bit(i, j))     continue;       /* 位变量，不判断 */

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
    函数 :  FindParaGroup_NeedStore()
    功能 :  寻找需保存的参数组

    参数 :
    返回 :  0 没有需保存的参数组 / N 需保存的参数组号+1（即第0组返回1）
    备注 :
********************************************************************************/
U08 FindParaGroup_NeedStore(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum)
{
    PARAMITEM *pGroup;      // 参数组
    U08 i;
    U08 Group=0;

    for (i=0; i<GroupNum; i++)
    {
        pGroup = (PARAMITEM*)&ParamGroup[GroupBegin+i];

        if (pGroup->chip & PARA_NEED_STORE)     // 第几组参数需保存
        {
            Group = GroupBegin + i + 1;
            break;
        }
    }

    return (Group);
}

///**********************************************
//函数：DataWrite_OneParaG
//功能：存储指定单组参数，并返回执行结果
//参数：ParamGroup_act：要存储的参数组指针； DataInfo：参数组存储信息（起始地址、个数）
//返回值：0 无需保存的参数组 / N 需保存的参数组号+1（即第0组返回1）
//**********************************************/
static U08 DataWrite_OneParaG(const PARAMITEM *ParamGroup, U08 ParaGroup_one, EEDATA DataInfo)
{
    PARAMITEM *pGroup;      // 参数组
    U08 itemNum = 0;        // 参数个数

    static U08 head = 0;
    static U08 end  = 0;

    U16 StoreAddr;          // 本次开始保存地址
    U16 ParaIndex;          // 本次开始保存参数序号
    U16 num = 0;            // 需保存的参数个数

    U08 *para_buf;
    U08 crc_buf[2];
    W16 crc_value;

    if(ParaGroup_one<1) return 0;

    pGroup = (PARAMITEM*)&ParamGroup[ParaGroup_one-1];
    itemNum = pGroup->items;

    if (pGroup->chip & PARA_STORE_PREP)     // 本组参数还未开始保存，则初始化需保存的起始位置和终止位置
    {
        pGroup->chip &= (~PARA_STORE_PREP);
        head = pGroup->head;
        end  = pGroup->end;
    }

    num = end - head;
    if (num > 0)
    {
        if (num > PARA_WR_MAX)  num = PARA_WR_MAX;          // 每次最多存几个参数

        ParaIndex = head;
        StoreAddr = DataInfo.Addr + (ParaIndex << 1);       // 开始保存地址

        para_buf  = (U08*)(pGroup->data_f + ParaIndex);     // 写法2：para_buf  = (U08*)&pGroup->data_f[ParaIndex];

        if (eprom_wr_no_crc(para_buf, StoreAddr, num<<1))
        {
            head += num;
        }
    }
    else
    {
        para_buf  = (U08*)pGroup->data_f;                   // 写法2：para_buf  = (U08*)&pGroup->data_f[0];
        crc_value.word = CRC16(para_buf, itemNum<<1);       // 算CRC
        crc_buf[0] = crc_value.byte.low;                    //NOTE-CXW 待测2017.4.11 确认大小端
        crc_buf[1] = crc_value.byte.high;

        StoreAddr = DataInfo.Addr + (itemNum<< 1);          // CRC存储地址（紧跟有效数据）

        if (eprom_wr_no_crc(&crc_buf, StoreAddr, 2))        // 写参数CRC
        {
            ParaGroup_one = 0;
            pGroup->chip &= (~PARA_NEED_STORE);
        }
    }
    return ParaGroup_one;
}


/********************************************************************************
    函数 :  DataWrite_ParaGroup()
    功能 :  遍历指定参数组，判断是否存在需要保存的参数组，如果有，则执行保存

    参数 :  GroupBegin：起始组; GroupNum：需访问组数;           pDataInfo：存储首地址及每组预留数据字节个数
    返回 :  TRUE 执行保存并返回真 / FALSE 不需保存直接返回
    备注 :
********************************************************************************/
BOOL DataWrite_ParaGroups(const PARAMITEM *ParamGroup, U08 GroupBegin, U08 GroupNum, const EEDATA *pDataInfo)
{
    static U08 ParaGroup_Store=0;
    EEDATA DataInfoAll;     // 整片数据存储信息
    EEDATA DataInfoEach;    // 单组参数存储信息
    BOOL fg_NeedStore = FALSE;  // 是否需保存

    if (ACTION_START==GET_INIT_PARA())    // 如果开始初始化，则从头开始检测参数保存
    {
        ParaGroup_Store = 0;
        SET_INIT_PARA(ACTION_ING);
    }

    if(ParaGroup_Store==0)
    {
        ParaGroup_Store=FindParaGroup_NeedStore(ParamGroup, GroupBegin, GroupNum);  // 寻找需保存的参数组
    }
    if(ParaGroup_Store>GroupBegin && ParaGroup_Store <= (GroupBegin+GroupNum))
    {
        DataInfoAll.Addr     = pDataInfo->Addr;
        DataInfoAll.DataNum  = GetSize_Byte(pDataInfo->DataNum, WITH_CRC);                  // 每组预留字节个数（包含CRC）

        DataInfoEach.Addr    = DataInfoAll.Addr+(ParaGroup_Store-1)*DataInfoAll.DataNum;    // 单组参数存储地址
        DataInfoEach.DataNum = 0;                                                           // 字节个数进入DataWrite_OneParaG再计算

        ParaGroup_Store = DataWrite_OneParaG(ParamGroup, ParaGroup_Store, DataInfoEach);
        fg_NeedStore = TRUE;
    }

    return fg_NeedStore;
}


/********************************************************************************
    函数 :  DataStore_Parameters()
    功能 :  执行不同参数片的存储
            (参数片:指不同功能的参数组组合，如ParamPri、ParamLimit、ParamModbus都是不同的参数片，
             每个参数片中包含N个相同大小的参数组(N>=1))
    参数 :  无
    返回 :  无
    备注 :
********************************************************************************/
void DataStore_Parameters()
{
    static U08 ParaPiece = 0;   // 当前处理的参数片
    BOOL fg_NeedStore=FALSE;    // 是否需保存
    U08  GroupNum;

    if (ACTION_START==GET_INIT_PARA())    // 如果开始初始化，则从头开始检测参数保存
    {
        ParaPiece = 0;
    }

    switch(ParaPiece)
    {
        case 0:
            GroupNum=(sizeof(ParamPri) / sizeof(PARAMITEM));

            fg_NeedStore = DataWrite_ParaGroups(ParamPri, 0, GroupNum, &ParamPri_DataInfo);
            if (FALSE == fg_NeedStore)    // 没有需保存的参数，换下一参数片
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
            if (FALSE == fg_NeedStore)    // 没有需保存的参数，换下一参数片
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
            if (FALSE == fg_NeedStore)    // 没有需保存的参数，换下一参数片
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
    Judge_InitPara(fg_NeedStore);   // 判断初始化状态

}


//============================================================================================================
/*==========================================
    功  能: 参数值是否有效(特殊注意)
/==========================================*/

U08 para_begin(U08 n, U08 m, I16 v)
{
    U08 para_value_valid = 1;       /* 参数设置值合理(初始化为合理) */

    n = n, m = m, v = v;           /* N0001 */

    return para_value_valid;
}

/*******************************************************************************
 函 数: PARA_MacType()
 功 能: 根据机型设置参数
 参 数: model--机型;
 返 回: 空
 其 它: 1、当处理参数较多时，para_end与para_set的相互调用是否会造成栈溢出？
        2、测试时注意：参数默认值应以para_fac为准，注意是否会因多重制约关系而被再次改动
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
 函 数: PARA_ParaWithinMacType()
 功 能: 判断参数是否为机型相关参数
 参 数: n--组;
        m--项;
 返 回: TRUE/FALSE--是/否
 其 它: 判断成功时，记录该参数在para_fac中的序号
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
    功  能: 参数互相制约(特殊注意)
    [n][m]对应参数的序号
    v  : 为摄氏度
/==========================================*/
void para_end(U08 n, U08 m, I16 v)
{
#ifndef USE_KIT_PARA_FAC
    U08 i;
    
    if (P_isP(n, m, val_air_type)			/* 空调类型 */
    || P_isP(n, m, val_cool_used)			/* 制冷模式 */
    || P_isP(n, m, val_heat_used)			/* 制热模式 */
    )
    {
        if (USE_COOL_ONLY)                  /* 仅制冷 */
        {
            para_set_cent(P_GaI(sv_mode), SET_COOL);	/* 控制模式 */
        }
        else if (USE_HEAT_ONLY)             /* 仅制热 */
        {
            para_set_cent(P_GaI(sv_mode), SET_HEAT);	/* 控制模式 */
        }
    }
    
    if (P_isP(n, m, sv_mode))
    {
        if (USE_DUAL_ZONE_CTRL				/* 双区控制 */
        && (v==SET_COOL)				    /* 设置制冷 */
        )
        {
//            fg_zone_B_run = FALSE;
            SETCMD_N(ZONE_B_OFF);			/* B区关机 */
        }
    }

    /* 变频器使用 */
    if (P_isP(n, m, val_inverter_used))
    {
        if (val_inverter_used)
        {
            COM_InitMaster(COM_INVT, BKBD_INVT_UART2);
        }
        else
        {
            /* 禁用时配置为从机，让上位机能连接变频器 */
            COM_InitSlaver(COM_INVT, BKBD_INVT_UART2, 100);
        }
    }
    /* 设定温度上下限处理 */
    {
        {
            I16 set_cool = para_get_dot(P_GaI(sv_cool)) ? sv_cool : sv_cool/10;
            I16 min=para_get_min_cent(P_GaI(sv_cool));
            I16 max=para_get_max_cent(P_GaI(sv_cool));
            if (set_cool < min)
            {
                para_set_cent(P_GaI(sv_cool), min);	/* 制冷设定温度 */  
            }
            else if (set_cool > max)
            {
                para_set_cent(P_GaI(sv_cool), max);	/* 制冷设定温度 */
            }
        }
        {
            I16 set_cool_B = para_get_dot(P_GaI(sv_cool_zone_B)) ? sv_cool_zone_B : sv_cool_zone_B/10;
            I16 min=para_get_min_cent(P_GaI(sv_cool_zone_B));
            I16 max=para_get_max_cent(P_GaI(sv_cool_zone_B));
            if (set_cool_B < min)
            {
                para_set_cent(P_GaI(sv_cool_zone_B), min);	/* B区制冷设定温度 */ 
            }
            else if (set_cool_B > max)
            {
                para_set_cent(P_GaI(sv_cool_zone_B), max);	/* B区制冷设定温度 */ 
            }
        }
        {
            I16 set_heat = para_get_dot(P_GaI(sv_heat)) ? sv_heat : sv_heat/10;
            I16 min=para_get_min_cent(P_GaI(sv_heat));
            I16 max=para_get_max_cent(P_GaI(sv_heat));
            if (set_heat < min)
            {
                para_set_cent(P_GaI(sv_heat), min);	/* 制热设定温度 */
            }
            else if (set_heat > max)
            {
                para_set_cent(P_GaI(sv_heat), max);	/* 制热设定温度 */
            }
        }
        {
            I16 set_heat_B = para_get_dot(P_GaI(sv_heat_zone_B)) ? sv_heat_zone_B : sv_heat_zone_B  /10;
            I16 min=para_get_min_cent(P_GaI(sv_heat_zone_B));
            I16 max=para_get_max_cent(P_GaI(sv_heat_zone_B));
            if (set_heat_B < min)
            {
                para_set_cent(P_GaI(sv_heat_zone_B), min);	/* B区制热设定温度 */
            }
            else if (set_heat_B > max)
            {
                para_set_cent(P_GaI(sv_heat_zone_B), max);	/* B区制热设定温度 */
            }
        }
        {
            I16 set_hotw = para_get_dot(P_GaI(sv_hotw)) ? sv_hotw : sv_hotw/10;
            I16 min = para_get_min_cent(P_GaI(sv_hotw));
            I16 max = para_get_max_cent(P_GaI(sv_hotw));

            if (set_hotw < min)
            {
                para_set_cent(P_GaI(sv_hotw), min);				/* 热水设定温度 */
            }
            else if (set_hotw > max)
            {
                para_set_cent(P_GaI(sv_hotw), max);				/* 热水设定温度 */
            }
        }
    }

    if (P_isP(n, m, val_bms_format)			/* BMS数据格式 */
    || P_isP(n, m, val_bms_baud_rate)		/* BMS波特率 */
    || P_isP(n, m, val_bms_addr)			/* BMS通讯地址 */
    )
    {
        COM_config_and_init(COM_BMS);
#ifdef COM_4G
        COM_config_and_init(COM_4G);
#endif
    }

    /* 变频压机编码 */
    if (P_isP(n, m, val_invt_motor_type))    invt_para_set(INVT_ID_1,INVT_PARA_MOTOR_TYPE, val_invt_motor_type);

    /* 驱动转速下限 */
    if (P_isP(n, m, val_invt_rps_min))       invt_para_set(INVT_ID_1,INVT_PARA_FREQ_MIN, val_invt_rps_min);

    /* 驱动转速上限 */
    if (P_isP(n, m, val_invt_rps_max))       invt_para_set(INVT_ID_1,INVT_PARA_FREQ_MAX, val_invt_rps_max);

    /* 驱动启动转速 */
    if (P_isP(n, m, val_invt_start_freq))    invt_para_set(INVT_ID_1,INVT_PARA_FREQ_START, val_invt_start_freq);

    /* EC风机编码 */
    if (P_isP(n, m, val_invt_fan_type))
    {
        invt_para_set(INVT_FAN1,INVT_PARA_MOTOR_TYPE, val_invt_fan_type);
        invt_para_set(INVT_FAN2,INVT_PARA_MOTOR_TYPE, val_invt_fan_type);
    }

    /* 主阀使用设置 */
    if (P_isP(n, m, val_motor_use))             SET_EXIO_INIT_HDW(0);

    /* 主阀励磁方式 */
    if (P_isP(n, m, val_motor_fre_type))        eev_set_motor_type(EEV_ID_1, (U16)v);

    /* 主阀励磁频率 */
    if (P_isP(n, m, val_motor_step_fre))        eev_set_step_speed(EEV_ID_1, (U16)v);

    /* 辅阀使用设置 */
    if (P_isP(n, m, val_s_motor_use))           SET_EXIO_INIT_HDW(0);

    /* 辅阀励磁方式 */
    if (P_isP(n, m, val_s_motor_fre_type))      eev_set_motor_type(EEV_ID_2, (U16)v);

    /* 辅阀励磁频率 */
    if (P_isP(n, m, val_s_motor_step_fre))      eev_set_step_speed(EEV_ID_2, (U16)v);

#ifdef USE_ODU_HL081B		/* HL081B */
    /* 温度探头配置 */
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
    /* 温度探头配置 */
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

    /* 工程设置：【变频水泵编码】制约【水泵目标转速】和【水泵目标流量】 */
    if (P_isP(n, m, val_PUMPf_model))
    {
        /* 水泵目标转速处理 */
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
        /* 水泵目标流量处理 */
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

    /* 工况频率表 */
    if (n == GROUP_WF)
    {
        wf_init();
    }

    /* 硬件配置更新，通知IO扩展板 */
    if (P_isP(n, m, val_motor_fre_type)		/* 主阀励磁方式 */
    || P_isP(n, m, val_motor_step_fre)		/* 主阀励磁频率 */
    || P_isP(n, m, val_motor_step_max)		/* 主阀总步数 */
    || P_isP(n, m, val_s_motor_fre_type)	/* 辅阀励磁方式 */
    || P_isP(n, m, val_s_motor_step_fre)	/* 辅阀励磁频率 */
    || P_isP(n, m, val_s_motor_step_max)	/* 辅阀总步数 */
    || P_isP(n, m, val_ntc_sensor_config)   /* 温度探头配置 */
    )
    {
        for (i = 0; i < USE_EXIO; i++)
        {
            SET_EXIO_HDW_CFG_UPDATE(i);
        }
    }

    /* 启用级联时，关联启用缓冲水箱温度探头 */
    if (P_isP(n, m, val_pcb_num))
    {
        if (val_pcb_num > 1)
        {
            para_set_cent(P_GaI(val_TACt1_sensor_used), USED);	/* 缓冲水箱上部温度探头 */
            para_set_cent(P_GaI(val_TACt2_sensor_used), USED);	/* 缓冲水箱下部温度探头 */
        }
    }
#endif

    /* 机器型号 */
    if (P_isP(n, m, val_mac_model))
    {
        PARA_MacType(val_mac_model);
        /* 因主机本身不执行接收广播动作，故主机MacTypeMaster放到这里来更新 */
        if (pcb_type==TYPE_MASTER)  MacTypeMaster = val_mac_model;
    }

#ifdef AT_MAX_NEW
    /* 能力测试选择 */
    if (P_isP(n, m, val_ability_test_set))
    {
        static U08 group_bak = AT_HT_GP_ORIGIN;
        U08 group = para_AT_get_group_ht();

        /* 组号变更才更新预设值：
         * 1. 为了实现同组使用时保留对参数的手动调整
         * 2. 重上电后，非原始组改变工况时，也会更新一次，因group_bak未存到e方
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
    功  能: 取参数的属性
/==========================================*/
U16 para_get_sign(U08 n, U08 m)
{
    U16 temp;
    PARA_SetAttrib(n, m, &temp);
    return temp;
}

/*******************************************************************************
 函 数: GetSettingRange()
 功 能: 获取控制温度的设定值范围
 参 数:
 返 回: 空
 其 它: 返回的是程序中的摄氏度值，与当前温度单位无关
*******************************************************************************/
I16 GetSettingRange(I16 CtrlObject, U08 DataType, U08 Mode)
{
    BOOL isGetMax = (DataType == GET_MAX) ? TRUE : FALSE;	/* 是否获取最大值 */
    BOOL isCtrlTempOut = (CtrlObject == TEMP_OUT) ? TRUE : FALSE;	/* 控制对象是否为出水温度 */
    I16 val = 0;

    switch (Mode)
    {
        /* 制冷 */
        case MODE_COOL:
            val = isGetMax ? val_cool_set_max : val_cool_set_min;
            val -= isCtrlTempOut ? INOUT_DIFF : 0;/* 控制出水时的温度限定 */
            break;

        /* 制热 */
        case MODE_HEAT:
            val = isGetMax ? val_heat_set_max : val_heat_set_min;
            val += isCtrlTempOut ? INOUT_DIFF : 0;/* 控制出水时的温度限定 */
            break;
    }

    return val;
}

/*******************************************************************************
 函 数: get_pumpf_para()
 功 能: 获取变频水泵的相关参数
 参 数:
 返 回: 空
 其 它: 
*******************************************************************************/

/* 获取变频水泵的参数类型命令：转速最大值、转速最小值、流量最大值、流量最小值、...... */
enum {EMPTY, RPM_MAX, RPM_MIN, FLOW_MAX, FLOW_MIN};

I16 get_pumpf_para(U08 type_para)
{
    /* 返回的参数值：转速最大值、转速最小值、流量最大值、流量最小值、...... */
    I16 rpm_max, rpm_min, flow_max, flow_min;
      
    /* 选择变频水泵型号 */
    switch (val_PUMPf_model)
    {
        case PUMPF_00_PARA_9_IPWM1:         /* 威乐 */
            rpm_max = PUMPF_00_RPM_MAX;
            rpm_min = PUMPF_00_RPM_MIN;
            flow_max = PUMPF_00_FLOW_MAX;
            flow_min = PUMPF_00_FLOW_MIN;
            break;
    
        default:
        case PUMPF_01_APM25_9_130_PWM:      /* 新界 */
            rpm_max = PUMPF_01_RPM_MAX;
            rpm_min = PUMPF_01_RPM_MIN;
            flow_max = PUMPF_01_FLOW_MAX;
            flow_min = PUMPF_01_FLOW_MIN;
            break;
    
        case PUMPF_02_GPA25_9HW_130:        /* 新沪 */
            rpm_max = PUMPF_02_RPM_MAX;
            rpm_min = PUMPF_02_RPM_MIN;
            flow_max = PUMPF_02_FLOW_MAX;
            flow_min = PUMPF_02_FLOW_MIN;
            break;
    
        case PUMPF_03_APF25_12_130FPWM1:    /* 新界 APF25-12-130FPWM1 */
            rpm_max = PUMPF_03_RPM_MAX;
            rpm_min = PUMPF_03_RPM_MIN;
            flow_max = PUMPF_03_FLOW_MAX;
            flow_min = PUMPF_03_FLOW_MIN;
            break;
    
        case PUMPF_04_APF25_12_130PWM1:     /* 新界 APF25-12-130PWM1 */
            rpm_max = PUMPF_04_RPM_MAX;
            rpm_min = PUMPF_04_RPM_MIN;
            flow_max = PUMPF_04_POW_MAX;
            flow_min = PUMPF_04_POW_MIN;
            break;
    
        case PUMPF_05_GPA25_11H:            /* 新沪 GPA25_11H1 */
            rpm_max = PUMPF_05_RPM_MAX;
            rpm_min = PUMPF_05_RPM_MIN;
            flow_max = PUMPF_05_FLOW_MAX;
            flow_min = PUMPF_05_FLOW_MIN;
            break;

        case PUMPF_06_HBG_25_125_130P:    /* 奥姆特 HBG-25-125-130P */
            rpm_max = PUMPF_06_RPM_MAX;
            rpm_min = PUMPF_06_RPM_MIN;
            flow_max = PUMPF_06_FLOW_MAX;
            flow_min = PUMPF_06_FLOW_MIN;
            break;

        case PUMPF_07_UPM4XLK_25_90_130:   /* 格兰富 UPM4XLK 25-90 130 */   
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
    功  能: 取参数的最小值
/==========================================*/
I16 para_get_min_real(U08 n, U08 m, U08 temp_unit)
{
	I16 val_min = 0;
#ifndef USE_KIT_PARA_FAC
    I16 s_min;				/* 静态下限 */
    I16 d_min;				/* 动态下限 */
    PARAM *pp2;				/* 参数属性 */

    pp2 = (PARAM *)ParamPri[n].array;
    s_min = pp2[m].min;
    d_min = pp2[m].min;

    /* 控制模式 */
    if (P_isP(n, m, sv_mode))                
    {
        if (USE_COOL_ONLY)	                /* 仅制冷 */
        {
            d_min = SET_COOL;
        }
        else if (USE_HEAT_ONLY)				/* 仅制热 */
        {
            d_min = SET_HEAT;
        }
    }

    /* 制冷设定温度 */
    if (P_isP(n, m, sv_cool))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_COOL);
    }

    /* 制热设定温度 */
    if (P_isP(n, m, sv_heat))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_HEAT);
    }

#ifndef USE_AREA_B_FIX
    /* 制冷设定温度(B) */
    if (P_isP(n, m, sv_cool_zone_B))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_COOL);
    }

    /* 制热设定温度(B) */
    if (P_isP(n, m, sv_heat_zone_B))
    {
        d_min = GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, MODE_HEAT);
    }
#endif    

    /* 热水设定温度 */
    if (P_isP(n, m, sv_hotw))
    {
        d_min = GetSettingRange(TEMP_IN, GET_MIN, MODE_HEAT);
    }

    if (P_isP(n, m, val_lp_value_incl)		/* 制冷低压过低 */
    || P_isP(n, m, val_lp_value_inht)		/* 制热低压过低 */
    )
    {
        d_min = val_J6_scale_min;
    }

    /* 水泵目标转速 */
    if (P_isP(n, m, val_pump_sv))
    {
        d_min = get_pumpf_para(RPM_MIN);
    }

    /* 水泵目标流量 */
    if (P_isP(n, m, val_pump_sv_flow))
    {
        d_min = get_pumpf_para(FLOW_MIN);
    }

    val_min = (d_min < s_min) ? s_min : d_min;	/* 动态下限比静态下限小时，以静态下限为准(防止写入EEPROM后报错) */
#else
    I16 s_min;				/* 静态下限 */
    PARAM *pp2;				/* 参数属性 */

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
 函 数: para_get_min_cent()
 功 能:
 参 数: n--;
		m--;
 返 回:
 其 它: 空
*******************************************************************************/
I16 para_get_min_cent(U08 n, U08 m)
{
	return para_get_min_real(n, m, SH_SET_UNIT_C);
}

/*******************************************************************************
 函 数: para_get_min()
 功 能:
 参 数: n--;
		m--;
 返 回:
 其 它: 空
*******************************************************************************/
I16 para_get_min(U08 n, U08 m)
{
	return para_get_min_real(n, m, sh_temp_unit);
}

/*==========================================
    功  能: 取参数的最大值
/==========================================*/
I16 para_get_max_real(U08 n, U08 m, U08 temp_unit)
{
	I16 val_max = 0;
#ifndef USE_KIT_PARA_FAC
    I16 s_max;				/* 静态上限 */
    I16 d_max;				/* 动态上限 */
    PARAM *pp2;				/* 参数属性 */

    pp2 = (PARAM *)ParamPri[n].array;
    s_max = pp2[m].max;
    d_max = pp2[m].max;
   
    /* 控制模式 */
    if (P_isP(n, m, sv_mode))
    {
        if (USE_COOL_ONLY)                  /* 仅制冷 */
        {
            d_max = SET_COOL;
        }
        else if (USE_HEAT_ONLY)             /* 仅制热 */
        {
            d_max = SET_HEAT;
        }
    }

    /* 制冷设定温度 */
    if (P_isP(n, m, sv_cool))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_COOL);
    }

    /* 制热设定温度 */
    if (P_isP(n, m, sv_heat))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_HEAT);
    }

#ifndef USE_AREA_B_FIX
    /* 制冷设定温度(B) */
    if (P_isP(n, m, sv_cool_zone_B))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_COOL);
    }

    /* 制热设定温度(B) */
    if (P_isP(n, m, sv_heat_zone_B))
    {
        d_max = GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, MODE_HEAT);
    }
#endif

    /* 热水设定温度 */
    if (P_isP(n, m, sv_hotw))
    {
        //默认纽恩泰热水设定温度上限可能按照电热热水上限限制。
        //其它客户热水设定温度上限按照制热回温上相限制。
        if ((val_hotw_max_TBH != 0) && val_TBH_used)
        {
            d_max = val_hotw_max_TBH;
        }
        else
        {
            d_max = GetSettingRange(TEMP_IN, GET_MAX, MODE_HEAT);
        }
    }

    /* 主阀手动步数 */
    if (P_isP(n, m, val_motor_manual_steps))
    {
        d_max = val_motor_step_max;
    }

    /* 辅阀手动步数 */
    if (P_isP(n, m, val_s_motor_manual_steps))
    {
        d_max = val_s_motor_step_max;
    }

    if (P_isP(n, m, val_hp_value_incl)		/* 制冷高压过高 */
    || P_isP(n, m, val_hp_value_inht)		/* 制热高压过高 */
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

    /* 水泵目标转速 */
    if (P_isP(n, m, val_pump_sv))
    {
        d_max = get_pumpf_para(RPM_MAX);
    }

    /* 水泵目标流量 */
    if (P_isP(n, m, val_pump_sv_flow))
    {
        d_max = get_pumpf_para(FLOW_MAX);
    }

    val_max = (d_max > s_max) ? s_max : d_max;	/* 动态上限比静态上限大时，以静态上限为准(防止写入EEPROM后报错) */
#else
    I16 s_max;				/* 静态上限 */
    PARAM *pp2;				/* 参数属性 */

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
 函 数: para_get_max_cent()
 功 能:
 参 数: n--;
		m--;
 返 回:
 其 它: 空
*******************************************************************************/
I16 para_get_max_cent(U08 n, U08 m)
{
	return para_get_max_real(n, m, SH_SET_UNIT_C);
}

/*******************************************************************************
 函 数: para_get_max()
 功 能:
 参 数: n--;
		m--;
 返 回:
 其 它: 空
*******************************************************************************/
I16 para_get_max(U08 n, U08 m)
{
	return para_get_max_real(n, m, sh_temp_unit);
}

/*==========================================
    功  能: 取参数的当前值
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
函数：key_continuous_add
功能：处理连续+键
参数：参数设置项序号、当前值
返回值：参数值
*********************************************/
I16 key_continuous_add(I16 val, I16 max, I16 min, U08 opt)
{
    I16 para_val;

    para_val = val;

    if (key_continuous_time[hmi_t_num] > CTUS_1000)     /* 连续按键超过90秒, 加1000 */
    {
        if (para_val + 1000 >= max)  para_val = max;
        else para_val += 1000;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_100)     /* 连续按键超过30秒, 加100 */
    {
        if (para_val + 100 >= max)  para_val = max;
        else para_val += 100;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_10)  /* 连续按键超过10秒, 加10 */
    {
        if (para_val + 10 >= max)  para_val = max;
        else para_val += 10;
    }
    else
    {
        if(para_val < max)para_val++;
        else
        {
            if(opt==CAL_CYCLE)  para_val = min;     // 循环设置
            if(opt==CAL_ETOE)   para_val = max;     // 不循环设置
        }
    }

    return para_val;
}
/********************************************
函数：key_continuous_sub
功能：处理连续-键
参数：参数设置项序号、当前值
返回值：参数值
*********************************************/
I16 key_continuous_sub(I16 val, I16 max, I16 min, U08 opt)
{
    I16 para_val;

    para_val = val;

    if (key_continuous_time[hmi_t_num] > CTUS_1000)     /* 连续按键超过90秒, 加1000 */
    {
        if (para_val - 1000 <= min)  para_val = min;
        else para_val -= 1000;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_100)     /* 连续按键超过30秒, 加100 */
    {
        if (para_val - 100 <= min)  para_val = min;
        else para_val -= 100;
    }
    else if (key_continuous_time[hmi_t_num] > CTUS_10)  /* 连续按键超过10秒, 加10 */
    {
        if (para_val - 10 <= min)  para_val = min;
        else para_val -= 10;
    }
    else
    {
        if(para_val > min)para_val--;
        else
        {
            if(opt==CAL_CYCLE)  para_val = max;     // 循环设置
            if(opt==CAL_ETOE)   para_val = min;     // 不循环设置
        }
    }

    return para_val;
}

/*==========================================
    功  能: 参数 + 1
/==========================================*/
void para_add(U08 n, U08 m, I16 *v, U08 bit_pos)
{
    U08 i;
    I16 val;
//  U16 tmp;

    val = *v;
    bit_pos = bit_pos;      /* N0001 */
//  if (bit_pos) /* 位变量 */
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
    功  能: 参数 - 1
/==========================================*/
void para_sub(U08 n, U08 m, I16 *v, U08 bit_pos)
{
    U08 i;
    I16 val;
//  U16 tmp;

    val = *v;
    bit_pos = bit_pos;      /* N0001 */
//  if (bit_pos) /* 位变量 */
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


/*PG_GetAddr_xxx说明**********************************************************************

函 数: PG_GetAddr_xxx
功 能: 接口函数：获取xxx参数组的地址（PARAMITEM结构）
使 用: 每增加一个PARAMITEM变量，都增加一个PG_GetAddr_xxx函数，xxx = 相应PARAMITEM变量名
↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/

/********************************************
函数: PG_GetAddr_ParamPri
功能: 获取ParamPri参数组的地址
参数: 参数组序号
返回值: 参数组的地址
*********************************************/
PARAMITEM* PG_GetAddr_ParamPri(U08 n)
{
    PARAMITEM* PgAddr;
    PgAddr = (PARAMITEM*)&ParamPri[n];
    return PgAddr;
}

/********************************************
函数: PG_GetAddr_ParamLimit
功能: 获取ParamLimit参数组的地址
参数: 参数组序号
返回值: 参数组的地址
*********************************************/
PARAMITEM* PG_GetAddr_ParamLimit(U08 n)
{
    PARAMITEM* PgAddr;
    PgAddr = (PARAMITEM*)&ParamLimit[n];
    return PgAddr;
}

/********************************************
函数: PG_GetGroupNum
功能: 获取PARAMITEM参数组的组数
参数: 参数组[]的首地址
返回值: 参数组的组数
使 用: 每增加一个PARAMITEM数组，都在此函数增加相应处理

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
    功  能: 设定参数
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

    /* 不一样时才保存 */
    if (C != pp1->data_c[m] || F != pp1->data_f[m])
    {
        pp1->data_f[m] = F;
        pp1->data_c[m] = C;

        set_para_modify_flag(n, m);
        para_end(n, m, C);	/* para_end中参数关联逻辑全使用摄氏度数据，所有传入数据必须为摄氏度数据 */
    }
}

/*******************************************************************************
 函 数: para_set_cent()
 功 能:
 参 数: n--;
		m--;
		v--;
 返 回: 空
 其 它: 空
*******************************************************************************/
void para_set_cent(U08 n, U08 m, I16 v)
{
	para_set_real(n, m, v, SH_SET_UNIT_C);
}

/*******************************************************************************
 函 数: para_set()
 功 能:
 参 数: n--;
		m--;
		v--;
 返 回: 空
 其 它: 空
*******************************************************************************/
void para_set(U08 n, U08 m, I16 v)
{
	para_set_real(n, m, v, sh_temp_unit);
}

/*==========================================
    功  能: 参数是否隐藏
/==========================================*/
U08 para_is_hide(U08 n, U08 m, U08 grade)
{
    BOOL para_item_hide = FALSE;

    if (grade != 0xff)      /* 不隐藏 */
    {
        if ((para_get_sign(n, m) & (D_HIDE)) != 0)
        {
            para_item_hide = TRUE;
        }
        else
        {
            if (grade == PASS_FAC)              /* 厂家级 */
            {
                if ((para_get_sign(n, m) & D_SAFEMASK) > D_FAC)    para_item_hide = TRUE;
            }
            else if (grade == PASS_SER)         /* 维修级 */
            {
                if ((para_get_sign(n, m) & D_SAFEMASK) > D_SER)    para_item_hide = TRUE;
            }
//          else if (grade == PASS_PRO)         /* 工程级 */
//          {
//              if ((para_get_sign(ParamGroup, m) & D_SAFEMASK) > D_PRO)    para_item_hide = TRUE;
//          }
            else if (grade == PASS_USER)        /* 用户级 */
            {
                if ((para_get_sign(n, m) & D_SAFEMASK) > D_USER)   para_item_hide = TRUE;
            }
        }
    }

    return  para_item_hide;
}
/*==========================================
    功  能: 参数是否为位型
/==========================================*/
U08 para_is_bit(U08 n, U08 m)
{
    return (para_get_sign(n, m) & D_BIT) ? 1 : 0;
}
/*==========================================
    功  能: 参数能否修改
    返回值：0: 可修改; 1: 不可修改
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
函数: PG_Is_Show
功能: 判断参数组是否可以显示
参数: ParamGroup: 参数组地址;         ParaGrade: 当前设置权限级别
返回值: TRUE 允许显示 / FALSE 不允许显示
*********************************************/
BOOL PG_Is_Show(const PARAMITEM *ParamGroup, U16 ParaGrade)
{
    BOOL AllowShow = TRUE;
    const PARAMITEM *pp1;
    U16 GroupSign, ParaSign;
    U08 i, ShowPara=0;

    pp1 = ParamGroup;
    GroupSign = pp1->sign ;     // 参数组属性


    // 判断该参数组本身的属性及权限是否允许显示
    if (((GroupSign & D_SAFEMASK) > ParaGrade)      // 参数组权限>当前设置权限，不允许显示，隐藏
        || (D_HIDE == (GroupSign & D_HIDE)))        // 参数组属性为隐藏
    {
        AllowShow = FALSE;

    }
    else    // 判断该参数组中所有参数项的属性及权限是否允许显示
    {
//        GroupSign &= D_SAFEMASK;

        for (i = 0; i < pp1->items; i++)
        {
            ParaSign = pp1->array[i].sign;

            if (D_HIDE!=(ParaSign & D_HIDE))     // 有参数项需要显示
            {
                ParaSign &= D_SAFEMASK;

                if (ParaGrade >= ParaSign)       // 当前设置权限 >= 参数项权限，允许该项参数显示
                {
                    ShowPara++;
                }
            }
        }
        if ( 0 == ShowPara) AllowShow = FALSE;   // 没有参数项需要显示，则该参数组隐藏

    }

    return AllowShow ;
}

/*==========================================
    功  能: 协议中修改参数（//NOTE-CXW：还未重构）
/==========================================*/
U08 PARA_Modify(U08 *buf, U16 addr, U16 num)
{
    U16  n, m, k;
    I16  tmp;
    U16  n_real, m_real,tmp_real;

    if ((addr < ADDR_PARA_BGN) || (addr >= ADDR_PARA_END)) return 0;

    m = addr - ADDR_PARA_BGN;
    n = m >> PARA_BIN_BIT_NUM;              /* 第几组 */
    m = m & (PARA_SIGLE_NUM-1);             /* 第几号 */

    for (k=0; k<num; m++, k+=2)
    {
        tmp_real = m/PARA_SIGLE_NUM;       //防止一次跨界写入参数
        m_real = m - tmp_real*PARA_SIGLE_NUM;
        n_real = n + tmp_real;



        if (m_real >= ParamPri[n_real].items)  break;       /* 超过参数个数了 */

        tmp = (I16)(((U16)buf[k] << 8) +  buf[k+1]);

        /* 从机【机型参数】不允许广播修改 */
        if (P_isP(n, m, val_mac_model))
        {
            MacTypeMaster = tmp;
            if (pcb_type == TYPE_SLAVER)    continue;
        }

        /* 1、机型不相同时，从机不接受广播机型相关参数
         * 2、现逻辑：若主机修改机型为从机机型，允许从机接收广播机型相关参数
        */
        if (!MACTYPE_IS_SAME && pcb_type==TYPE_SLAVER)
        {
            U16 idx;
            if (PARA_ParaWithinMacType(n, m, &idx))
            {
                continue;
            }
        }

        if (para_is_bit(n_real, m_real));      /* 如果是位变量(没有大小值限制) */
        else
        {
            if (tmp < para_get_min_cent(n_real, m_real))  continue;
            if (tmp > para_get_max_cent(n_real, m_real))  continue;
        }

        if (para_is_nexch(n_real, m_real))  continue;      /*不允许修改 */

//      if (para_is_hide(n, m, 0xff))   continue;       /* 不允许修改 */

        para_set_cent(n_real, m_real, tmp); /* 修改参数 */
    }

    return 1;
}

/*==========================================
    功  能: 协议中读参数值（//NOTE-CXW：还未重构）
/==========================================*/
U08 PARA_GetVal(U08 *buf, U16 addr, U16 num)
{
    U16  n, m, k;
    U16 *pData;

    U16  n_real, m_real,tmp_real;

    if ((addr < ADDR_PARA_BGN) || (addr >= ADDR_PARA_END)) return 0;

    m = addr - ADDR_PARA_BGN;
    n = m >> PARA_BIN_BIT_NUM;      /* 第几组 */
    m = m & (PARA_SIGLE_NUM-1);     /* 第几号 */

    for (k=0; k<num; m++)
    {
        tmp_real = m/PARA_SIGLE_NUM;       //防止一次跨界读取参数
        m_real = m - tmp_real*PARA_SIGLE_NUM;
        n_real = n + tmp_real;

        /* 联网监控暂时只做摄氏度 */
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
函数：PARA_SetAttrib
功能：改变参数属性
参数：参数组号(大项); 参数项目号;参数属性选择;返回值
返回值：无
*******************************************************/
void PARA_SetAttrib(U08 n, U08 m, U16 *result)  //NOTE-CXW 待测2017.4.19
{
    PARAM *pp2;

    pp2 = (PARAM *)ParamPri[n].array;
    *result = *(&(pp2[m].min)+3);

#ifndef USE_KIT_PARA_FAC
    /* 运行中不可设置 */
    if (*result&(D_RNEXCH) && StepIsRun)                            *result |= D_NEXCH;

    /* 00#～07#模块设置 */
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

    /* 控制模式 */
    if (P_isP(n, m, sv_mode))
    {
        if(USE_COOL_ONLY || USE_HEAT_ONLY)                          *result |= D_NEXCH;
    }

    /* EC风机编码 */
    if (P_isP(n, m, val_invt_fan_type))
    {
        if (val_freq_fan_set != FREQ_FAN_SET_EC)                    *result |= D_HIDE;
    }

    /* PWM风机脉冲 */
    if (P_isP(n, m, val_fan_pwm_pulse))
    {
        if (!USE_PWM_FAN)                                           *result |= D_HIDE;
    }

    /* EC风机基址 */
    if (P_isP(n, m, val_ec_fan_base_addr))
    {
        if (!USE_EX_EC_FAN)                                         *result |= D_HIDE;
    }
#endif
}
/*==========================================
    功  能: 协议中读参数属性（//NOTE-CXW：还未重构）
/==========================================*/
U08 PARA_GetAttrib(U08 *buf, U16 addr, U16 num)
{
    U16 n, m, k;
    U08 x;
    U16 temp;

    if ((addr < ADDR_PARA_INFO_BGN) || (addr >= ADDR_PARA_INFO_END))    return 0;

    m = addr - ADDR_PARA_INFO_BGN;
    n = m >> PARA_INFO_BIN_BIT_NUM;         /* 第几组(大项) */
    m = m & (PARA_INFO_SINGLE_NUM - 1);     /* 第几号(起始地址) */

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
    功  能: 协议中读参数信息（//NOTE-CXW：还未重构）
/==========================================*/
U08 PARA_GetInfo(U08 *buf, U16 addr, U16 num)
{
    U16 m, k;
    U16 para_info[GROUP_MAX+1];

    if ((addr < ADDR_PARA_ITEM_BGN) || (addr >= ADDR_PARA_ITEM_END))    return 0;

    /* 初始化参数的信息 */
    para_info[0] = PARA_GROUP_NUM;
    for (k=1; k<= PARA_GROUP_NUM; k++)
    {
        para_info[k] = ParamPri[k-1].items;     /* 低字节为个数 */
        para_info[k] |= ParamPri[k-1].sign & 0xff00;        /* 高字节为属性 */
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
函数：para_broadcast
功能：广播参数到子机。
参数：无。使用标志ParamPri[n].chip的 bit7 判断是否需广播.
返回：无
其它：只要修改1个参数,就广播整组。
*********************************************/
U08 para_broadcast(U08 com)
{
    U08 n;
    U16 addr;
    U16 num;

    if (pcb_addr != 0)      return 0;           /* 主机才广播参数 */

    for (n=0; n<PARA_GROUP_NUM; n++)
    {
        if (ParamPri[n].chip & PARA_NEED_STORE)     continue;   /* 该份参数还未保存 */

        if (ParamPri[n].chip & PARA_NEED_BRO)               /* 需要广播 */
        {
            addr = ADDR_PARA_BGN + n * PARA_SIGLE_NUM;
            num = ParamPri[n].items;

            /* 正常插入广播命令后才能清除标志 */
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
    功  能: 密码广播
    返  回: 0 - 表示没有广播需要
            其它值 - 广播通讯的缓存数据个数
/==========================================*/
U08 m_password_broadcast(U08 com)
{
    U08 x;
    U16 addr, num;

    if (pcb_addr != 0)      return 0;           /* 主机才广播参数 */

    /* 1秒更新一次 */
    if (commu_m_password_broadcast_interval < 10) return 0;
    commu_m_password_broadcast_interval = 0;

    for (x = 0; x <16; x++)
    {
        if (GETBIT(passwd_broadcast, x))
        {
            num  = 4;
            addr = ADDR_PASS_BGN + (x << 2);

            /* 正常插入广播命令后才能清除标志 */
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
    功能：取密码
    num : 字节数
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
        if (m < 4 || m%4 != 0 || num != 8)  /* 第0级密码不允许修改; 不对齐不允许修改; 每次只能修改一处密码 */
        {
            return 0;
        }

        for (k=0; k<num; k++)           /* 非0~~9之间的字符(或空字符)，不允许修改 */
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
                SETBIT(passwd_broadcast, m>>2); /* 每个密码占用了4个字地址 */
            }
            m++;
        }
        break;
    }
    return 1;
}

/*******************************************************************************
 函 数: para_check()
 功 能: 参数检查
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void para_check(void)
{
    // 功能参数读取并校验，失败报警，初始化内存变量
    DataAccess_FacPara(DATA_READ);

    if (fg_seep_crc_err || fg_seep_overflow)
    {
        if (pcb_addr == 0)
        {
            m_set_error(MAX_PCB, M_ERR_EEPROM);  /* 主机EEPROM数据错，停机组 */
        }
        else
        {
            s_set_error(S_ERR_EEPROM);         /* 从机EEPROM数据错，停模块 */
        }
    }
#ifdef USE_SW_MAC
    else					/* 无eeprom错误时才处理 */
    {
        para_set_cent(P_grp(val_mac_model), P_idx(val_mac_model), sw_mac_model);
    }
#endif

    //本系统暂时无系统维护功能 zzl2018.07.31
    // 系统维护参数读取并校验，失败报警，初始化内存变量
    DataAccess_LimitPara(DATA_READ);

    if (fg_limit_crc_err || fg_limit_overflow)
    {
        if (pcb_addr == 0)
        {
            m_set_error(MAX_PCB, M_ERR_EEPROM_LIMIT);  /* 主机系统维护数据错，停机组 */
        }
        else
        {
            fg_limit_crc_err  = FALSE;
            fg_limit_overflow = FALSE;         /* 从机不报“系统维护数据错” */
        }
    }
    // 联网通讯参数读取并校验，失败不报警，直接初始化
//    DataAccess_ModbusPara(DATA_READ);
}

/*******************************************************************************
 函 数: para_check_invt_value()
 功 能: 同步变频参数值
 参 数: 空
 返 回: 空
 其 它: 参数值不一致时，压机型号以控制器为准，转速上下限以变频器为准
*******************************************************************************/
void para_check_invt_value(void)
{
#ifndef USE_KIT_PARA_FAC
    I16 para_value = INVT_PARA_VALUE_NULL;

    if (val_inverter_used)
    {
        INVT_ID_ENUM id = INVT_ID_1;
        // 0 电机型号
        para_value = invt_para_read(id, INVT_PARA_MOTOR_TYPE);
        if(para_value != INVT_PARA_VALUE_NULL && para_value != val_invt_motor_type)
        {
            invt_para_set(id,INVT_PARA_MOTOR_TYPE, val_invt_motor_type);
        }
        // 1 驱动转速下限
        para_value = invt_para_read(id, INVT_PARA_FREQ_MIN);
        if(para_value != INVT_PARA_VALUE_NULL && (para_value/100)*100 != val_invt_rps_min)
        {
            para_set_cent(GROUP_COMP, IDX_INVT_MIN, para_value / 100);
        }
        // 2 驱动转速上限
        para_value = invt_para_read(id, INVT_PARA_FREQ_MAX);
        if(para_value != INVT_PARA_VALUE_NULL && (para_value/100)*100 != val_invt_rps_max)
        {
            para_set_cent(GROUP_COMP, IDX_INVT_MAX, para_value / 100);
        }
        // 驱动开机转速
        {
            I16 para_now = invt_para_read(id, INVT_PARA_FREQ_START);
            para_value = val_invt_start_freq*invt_para_read(id, INVT_PARA_POLE)/2;
            if (para_now != INVT_PARA_VALUE_NULL && para_now != para_value)
            {
                invt_para_set(id, INVT_PARA_FREQ_START, val_invt_start_freq);
            }
        }
#if 0
        // EC风机PWM开关频率
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
        /* EC风机1型号 */
        para_value = invt_para_read(INVT_FAN1, INVT_PARA_MOTOR_TYPE);
        if(para_value != INVT_PARA_VALUE_NULL && para_value != val_invt_fan_type)
        {
            invt_para_set(INVT_FAN1,INVT_PARA_MOTOR_TYPE, val_invt_fan_type);
        }

        /* EC风机2型号 */
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
    函数 :  Task_ID_Init()
    功能 :  往EEPROM中写入识别码

    参数 :  --
    返回 :  --
*******************************************************************************
*/
void Task_ID_Init(void)
{
    U08 i;
    U08 buf_id[MAX_ID];
    const U08 * pIDCode;

    pIDCode = PCB_ID_CODE;  /* 识别码 */
    for (i = 0; i < MAX_ID; i++) buf_id[i] = pIDCode[i];

    //wreeprom(buf_id, MEM_ID_CODE_ADDR, MAX_ID);
    eprom_wr_near_crc(buf_id, MEM_ID_CODE_ADDR, MAX_ID);
}

/*
*******************************************************************************
    函数 :  Task_ID_Read()
    功能 :  由EEPROM中读取识别码，并判断程序是否支持该识别码
    参数 :  --
    返回 :  --
*******************************************************************************
*/
void Task_ID_Read(void)
{
    U08 i;
    U08 buf_id[MAX_ID];
    const U08 * pIDCode;

    //rdeeprom(buf_id, MEM_ID_CODE_ADDR, MAX_ID, 0);
    eprom_rd_near_crc(buf_id, MEM_ID_CODE_ADDR, MAX_ID);

    pIDCode = PCB_ID_CODE;  /* 识别码 */
    for (i = 0; i < MAX_ID; i++)
    {
        if (buf_id[i] != pIDCode[i])
        {
            //s_set_error(ERR_ID_CODE);  /* 识别码错误，程序不能运行 */
            break;
        }
    }
}
/********************************************************************************
    函数 :  DataAccess_EEPROM()
    功能 :  EEPROM数据访问
            读数据：按地址读数据，2次读失败则自动初始化
            初始化：用同一个值初始化所有数据
            写数据：按地址写数据
    参数 :  buf：用于存放数据的变量； addr：读写的EEPROM地址； num：读写的字节个数；
            InitValue：初始化值；     opt：命令
    返回 :  无
    备注 :
********************************************************************************/
void DataAccess_EEPROM(U08 *buf, const EEDATA *pDataInfo, U08 InitValue, U08 opt)
{
    U16 i;
    U16 addr, num;

    addr = pDataInfo->Addr;
    num  = pDataInfo->DataNum;

    switch (opt)
    {
        case DATA_READ :    // 读取

            if (eprom_rd_near_crc(buf, addr, num)) break;
            if (eprom_rd_near_crc(buf, addr, num)) break;
            // 两次读取错误则重新初始化
            // break;

        case DATA_INIT :    // 初始化

            for (i = 0; i < num; i++)
            {
                buf[i] = InitValue;
            }
            // break;

        case DATA_WRITE :   // 保存

            eprom_wr_near_crc(buf, addr, num);
            break;
    }
}

/*****************************************************
    功能：写掉电前时间到EEPROM
******************************************************/
void pwd_time_write_to_eeprom(void)
{
    U16 addr;

    addr = MEM_PWD_TIME_ADDR ;
    eprom_wr_near_crc((U08*)&clock[hmi_t_num], addr, sizeof(LOG_RTC));
}
/*****************************************************
    功能：从EEPROM读掉电前时间
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
    函数 :  DataAccess_SkipPreheatCnt()
    功能 :  数据访问-跳过预热计数
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_SkipPreheatCnt(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_SKIP_PREHEAT_ADDR;

    buf = &skip_preheat_cnt;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}



/********************************************************************************
    函数 :  DataAccess_Language()
    功能 :  数据访问-语言
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_Language(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = RN_LANGUAGE_EN; // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_LANGUAGE_ADDR;

    buf = &sh_language;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    函数 :  DataAccess_Temp_unit()
    功能 :  数据访问-语言
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_Temp_unit(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = RN_TEMP_UNIT_C; // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_TEMP_UNIT_ADDR;

    buf = &sh_temp_unit;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    函数 :  DataAccess_AUTO_RUN()
    功能 :  数据访问-运行状态
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_AUTO_RUN(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;   // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = 1;
    DataInfo.Addr = MEM_AUTO_RUN_ADDR;

    buf = &auto_run_bit;


    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    函数 :  DataAccess_CompRuntime()
    功能 :  数据访问-本模块压机累计运行时间
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_CompRuntime(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_RUNTIME<<1;
    DataInfo.Addr = MEM_RUNTIME_ADDR;

    buf = (U08 *)&pCell->cell_runtime[0];


    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/********************************************************************************
    函数 :  DataAccess_SysRuntime()
    功能 :  数据访问-系统累计运行时间
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_SysRuntime(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_RUNTIME<<1;
    DataInfo.Addr = MEM_SYSTIME_ADDR;

    buf = (U08 *)&cell[MAX_PCB].cell_runtime[0];


    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}


/********************************************************************************
    函数 :  DataAccess_HistoryErr()
    功能 :  数据访问-历史故障
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_HistoryErr(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = F_NO_ERR;   // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_HIS_ERR_TBL;
    DataInfo.Addr = MEM_HIS_ERROR_ADDR;

    buf = his_err_tbl;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}
/********************************************************************************
    函数 :  DataAccess_InvtExceptInfo()
    功能 :  数据访问-变频异常信息
    参数 :  opt:操作码；idx:下标，为最大值时表示操作全部
    返回 :
    备注 :
********************************************************************************/
void DataAccess_InvtExceptInfo(U08 opt, U08 idx)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // 初始化值

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
        /* 地址没有做按4对齐的处理：对于某些eeprom可能存在兼容性问题，比如stm8的片内eeprom */
        DataInfo.DataNum = sizeof(info_invt_except[0]);
        DataInfo.Addr = MEM_INVT_EXP_INFO_ADDR+x*(DataInfo.DataNum+2);  /* 一个信息大小+2字节CRC */
        buf = (U08 *)&info_invt_except[x];
        DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
    }
}

/********************************************************************************
    函数 :  DataAccess_Timer()
    功能 :  数据访问-定时器
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_Timer(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = MAX_TIME_TBL;
    DataInfo.Addr = MEM_DM603_TIME_ADDR;

    buf = dm603_time_tbl;

    DataAccess_EEPROM(buf, &DataInfo, InitValue, opt);
}

/*******************************************************************************
 函 数: DataAccess_holiday()
 功 能: 休假模式数据
 参 数: opt--;
 返 回: 空
 其 它: 空
*******************************************************************************/
void DataAccess_holiday(U08 opt)
{
    U08 *buf = NULL;
    U08 InitValue = 0;  // 初始化值

    EEDATA DataInfo;
    DataInfo.DataNum = HOLIDAY_MAX;
    DataInfo.Addr = MEM_HOLIDAY_ADDR;
    if (opt == DATA_INIT)
    {
        // 数据初始化
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
    函数 :  DataAccess_Password()
    功能 :  数据访问-密码
    参数 :
    返回 :
    备注 :
********************************************************************************/
const U08 InitPassWord[MAX_PASS_GROUP][MAX_PASS_WORD]=
{
    {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},  /* 动态密码 */
    {'1','2','3','4','5','6',0,0},              /* 厂家密码 */
    {'1','2','3','4','5',0,0,0},                /* 维修者密码 */
    {'1','2','3','4',0,0,0,0},                  /* 工程密码 */
    {'1','2','3',0,0,0,0,0},                    /* 用户密码 */
    {'6','6','6','6','6','6','6','6'},          /* 使用期限密码 */
};
void DataAccess_OnePassword(U08 *buf, U08 index, U08 opt)
{
    U16 i;
    U16 SizeWithCRC;
    EEDATA DataInfo;

    DataInfo.DataNum = MAX_PASS_WORD;
    SizeWithCRC = GetSize_Byte(DataInfo.DataNum, WITH_CRC);     // 一组密码占用存储空间（包含CRC）
    DataInfo.Addr = MEM_PASSWORD_ADDR + index * SizeWithCRC;    // 本组密码实际地址

    switch (opt)
    {
        case DATA_READ :    // 读取

            if (eprom_rd_near_crc(buf, DataInfo.Addr, DataInfo.DataNum)) break; // 读取成功，退出
            if (eprom_rd_near_crc(buf, DataInfo.Addr, DataInfo.DataNum)) break;
            // 两次读取错误则重新初始化
            // break;

        case DATA_INIT :    // 初始化

            for (i = 0; i < MAX_PASS_WORD; i++)
            {
                buf[i] = InitPassWord[index][i];
            }
            // break;

        case DATA_WRITE :   // 保存
            SET_BIT(passwd_modify, index);
            SET_BIT(passwd_broadcast, index);
            break;
    }

}

/********************************************************************************
    函数 :  Password_ReadAll()
    功能 :  读取所有密码
    参数 :
    返回 :
    备注 :  主板才调用此函数
********************************************************************************/
void Password_ReadAll()
{
    U08 x = 0;
    for(x=0; x<MAX_PASS_GROUP; x++)
    {
        DataAccess_OnePassword((U08 *)password[x],x, DATA_READ);
    }
    passwd_modify = 0;
    passwd_broadcast = 0xffff;  /* 上电时置广播位 */
}

/********************************************************************************
    函数 :  Password_InitAll()
    功能 :  初始化所有密码
    参数 :
    返回 :
    备注 :  主板才调用此函数
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
    函数 :  Password_super_cal()
    功能 :  根据验证码计算动态密码
    参数 :
    返回 :
    备注 :
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



    for (i=0; i<MAX_PASS_WORD; i++) // 数字限制在7以内并转成字符
    {
        password[PASS_SUPER][i] &= 0x07;
        password[PASS_SUPER][i] += '0';
    }
}


/********************************************************************************
    函数 :  DataStore_EEPROM()
    功能 :  除参数以外的数据按标志分批存储
    参数 :
    返回 :
    备注 :  适用批量且使用修改标志的数据
********************************************************************************/
void DataStore_EEPROM()
{
    U16 x,SizeWithCRC;
    EEDATA DataInfo;


    // 处理密码存储
    if (passwd_modify != 0)
    {
        for (x=0; x<MAX_PASS_GROUP; x++)
        {
            if (GETBIT(passwd_modify, x))
            {
                DataInfo.DataNum = MAX_PASS_WORD;
                SizeWithCRC = GetSize_Byte(DataInfo.DataNum, WITH_CRC);     // 一组密码占用存储空间（包含CRC）
                DataInfo.Addr = MEM_PASSWORD_ADDR + x * SizeWithCRC;        // 本组密码实际地址
                if(eprom_wr_near_crc((U08 *)password[x], DataInfo.Addr, DataInfo.DataNum))
                {
                    if(x==PASS_FAC) ValidCode = 0;  // 如果厂家密码修改成功，清除验证码（）
                }
                CLRBIT(passwd_modify, x);

                break;  // 每次只存储一组密码
            }
        }
        return; // 每次只存储一片数据
    }
}

/********************************************************************************
    函数 :  DataAccess_LimitPara()
    功能 :  数据访问-功能参数
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_FacPara(U08 opt)
{
    U08 GroupNum = 0;

    GroupNum=(sizeof(ParamPri) / sizeof(PARAMITEM));
    PARA_GROUP_NUM = GroupNum;

    switch (opt)
    {
        case DATA_READ :    // 读取
            if(DataRead_ParaGroups(ParamPri, 0, GroupNum, GroupNum, &ParamPri_DataInfo)==TRUE)// 读取成功，进行超界验证
            {
                if(DataVerify_ParaGroups(ParamPri, 0, GroupNum)==FALSE) // 验证失败
                {
                     fg_seep_overflow = TRUE;
                }
            }
            else    // 读取失败
            {
                fg_seep_crc_err = TRUE;
            }

            if (fg_seep_crc_err==FALSE && fg_seep_overflow==FALSE) // 读取成功直接返回,否则住下执行DATA_RESET，复位内存数据
            {
                break;
            }

            #ifdef USE_NO_PARA_RESET
                break;
            #endif

        case DATA_RESET :    // 复位内存数据
            DataReset_ParaGroups(ParamPri, 0, GroupNum, GroupNum, ParamPri_DataInfo.DataNum);
            break;


        case DATA_INIT :    // 初始化
            DataReset_ParaGroups(ParamPri, 0, GroupNum, GroupNum, ParamPri_DataInfo.DataNum);
            if (ACTION_ING!=GET_INIT_PARA())
            {
                SET_INIT_PARA(ACTION_START);   /*  初始化开始 */
            }
            // break;

        case DATA_WRITE :   // 置保存标志
            FlagParaGroup_AllStore(ParamPri, 0, GroupNum, GroupNum);
            break;

    }
}


/********************************************************************************
    函数 :  DataAccess_LimitPara()
    功能 :  数据访问-系统维护参数
    参数 :
    返回 :
    备注 :
********************************************************************************/
void DataAccess_LimitPara(U08 opt)
{
    U08 GroupNum = 0;

    GroupNum=(sizeof(ParamLimit) / sizeof(PARAMITEM));

    switch (opt)
    {
        case DATA_READ :    // 读取
            if(DataRead_ParaGroups(ParamLimit, 0, GroupNum, GroupNum, &Limit_DataInfo)==TRUE)// 读取成功，进行超界验证
            {
                if(DataVerify_ParaGroups(ParamLimit, 0, GroupNum)==FALSE)   // 验证失败
                {
                     fg_limit_overflow = TRUE;
                }
            }
            else    // 读取失败
            {
                fg_limit_crc_err = TRUE;
            }
            if (fg_limit_crc_err==FALSE && fg_limit_overflow==FALSE) // 读取成功直接返回,否则住下执行DATA_RESET，复位内存数据
            {
                break;
            }

        case DATA_RESET :    // 复位内存数据
            DataReset_ParaGroups(ParamLimit, 0, GroupNum, GroupNum, Limit_DataInfo.DataNum);
            break;

        case DATA_INIT :    // 初始化
            DataReset_ParaGroups(ParamLimit, 0, GroupNum, GroupNum, Limit_DataInfo.DataNum);
            if (ACTION_ING!=GET_INIT_PARA())
            {
                SET_INIT_PARA(ACTION_START);   /*  初始化开始 */
            }
            // break;

        case DATA_WRITE :   // 置保存标志
            FlagParaGroup_AllStore(ParamLimit, 0, GroupNum, GroupNum);
            break;
    }
}

/*****************************************************
函数：limit_time_write_to_eeprom
功能：写使用期限到EEPROM
参数：无
返回值：无
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
 函 数: para_get_dot()
 功 能:
 参 数: grp_id--;
        item_id--;
 返 回:
 其 它: 空
*******************************************************************************/
U08 para_get_dot(U08 grp_id, U08 item_id)
{
    return (ParamPri[grp_id].array[item_id].sign & D_DOTMASK);
}


/*******************************************************************************
 函 数: para_AT_use_get()
 功 能: 获取：能力测试选择
 参 数: 空
 返 回: 能力测试选择
 其 它: 空
*******************************************************************************/
I16 para_AT_use_get(void)
{
    #define AT_ORIGIN_IDX_BGN AT_HEAT_A				/* 原始工况边界起始 */
    #define AT_ORIGIN_IDX_END AT_COOL				/* 原始工况边界结束 */
    
    I16 result = val_ability_test_set;

    /* 预设组工况序号转为原始工况序号 */
    if (result > AT_ORIGIN_IDX_END)
    {
        result = (result-AT_MAX)%AT_HT_WC_NUM+AT_ORIGIN_IDX_BGN;
    }

    /* 边界保护 */
    result = fun_pv_limit(result, AT_UNUSED, AT_ORIGIN_IDX_END);

    return result;
}

#ifdef AT_MAX_NEW
/*******************************************************************************
 函 数: para_AT_get_group_ht()
 功 能: 获取能力测试组号-制热
 参 数: 空
 返 回: 能力测试组号-制热
 其 它: 空
*******************************************************************************/
U08 para_AT_get_group_ht(void)
{
    U08 group = AT_HT_GP_ORIGIN;
    
    if (val_ability_test_set >= AT_MAX)
    {
        group = (val_ability_test_set-AT_MAX)/AT_HT_WC_NUM;	        /* 预设组号 */
    }

    return group;
}

/*******************************************************************************
 函 数: para_AT_update_ht()
 功 能: 更新能力测试制热参数
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void para_AT_update_ht(void)
{
    U08 i = 0;
    U08 group = para_AT_get_group_ht();		                /* 能力测试组号-制热 */
    U08 para_grp = P_grp(val_ability_test_set);				/* 预设参数的参数组号 */
    U08 para_idx[AT_HT_PARA_NUM] = { 1,  2,  3,  4,  5,  6,  7,
                                      8,  9, 10, 11, 12, 13, 14,
                                     15, 16, 17, 18, 22, 23, 24,
                                     25, 26, 27, 28, 29, 30, 31,
                                     32, 33, 34, 35, 36, 37, 38,
                                     39, 40, 41, 42, 43, 44, 45,
                                    };  /* 预设参数的参数项号 */

    if (group == AT_HT_GP_ORIGIN)			/* 原始组 */
    {
        U16 idx = 0;

        for (i=0; i<AT_HT_PARA_NUM; i++)
        {
            if (PARA_ParaWithinMacType(para_grp, para_idx[i], &idx))/* 在机型参数表中 */
            {
                para_set_cent(para_grp, para_idx[i], para_fac[idx].para[val_mac_model]);
            }
            else			/* 在通用参数表中 */
            {
                para_set_cent(para_grp, para_idx[i], ParamPri[para_grp].array[para_idx[i]].init);
            }
        }  
    }
    else					                /* 预设组 */
    {
        for (i=0; i<AT_HT_PARA_NUM; i++)
        {
            para_set_cent(para_grp, para_idx[i], AT_presets[group][i][val_mac_model]);
        }  
    }
}
#endif
