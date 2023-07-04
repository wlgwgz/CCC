/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : com_master.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2016年11月11日星期五
  最近修改   :
  功能描述   : 头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月11日星期五
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/
#ifndef __COM_M_H__
#define __COM_M_H__

#ifdef  COM_M_GLOBAL
    #define COM_M_EXT
#else
    #define COM_M_EXT extern
#endif
/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/
COM_M_EXT U08 com_slv_HMI(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);
COM_M_EXT U08 com_slv_FBUS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);
COM_M_EXT U08 com_slv_BMS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);
        
COM_M_EXT U08 com_invt_wr(U08 com, U08 EX_addr, U16 wr_addr, U08 wr_num,U08 prio);
COM_M_EXT U08 com_invt_rd(U08 com, U08 EX_addr, U16 rd_addr, U08 rd_num,U08 prio);
        
COM_M_EXT U08 com_FBUS_bro(U08 com, U16 wr_addr, U08 wr_num,U08 prio);
COM_M_EXT U08 com_FBUS_wr(U08 com, U08 obj_addr, U16 wr_addr, U08 wr_num, U08 prio);
COM_M_EXT U08 com_FBUS_rd(U08 com, U08 obj_addr, U16 rd_addr, U08 rd_num,U08 prio);
        
COM_M_EXT U08 PDU_SlaveDealUserDef(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal);

/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/

/* 通讯状态返回值 */
#define COM_FAIL					FALSE			/* 插入结点失败 */
#define COM_SUCC					TRUE			/* 插入结点成功 */
#define COM_ERR_DATA_ADDR			2				/* 无效数据地址 */
#define COM_ERR_MAC_ADDR			3				/* 无效设别地址 */


/*-----------------宏定义 ---------------------------------*/






#endif /* __COM_M_H__ */
