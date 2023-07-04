/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : com_master.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2016��11��11��������
  ����޸�   :
  ��������   : ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��11��������
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __COM_M_H__
#define __COM_M_H__

#ifdef  COM_M_GLOBAL
    #define COM_M_EXT
#else
    #define COM_M_EXT extern
#endif
/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/
COM_M_EXT U08 com_slv_HMI(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);
COM_M_EXT U08 com_slv_FBUS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);
COM_M_EXT U08 com_slv_BMS(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);
        
COM_M_EXT U08 com_invt_wr(U08 com, U08 EX_addr, U16 wr_addr, U08 wr_num,U08 prio);
COM_M_EXT U08 com_invt_rd(U08 com, U08 EX_addr, U16 rd_addr, U08 rd_num,U08 prio);
        
COM_M_EXT U08 com_FBUS_bro(U08 com, U16 wr_addr, U08 wr_num,U08 prio);
COM_M_EXT U08 com_FBUS_wr(U08 com, U08 obj_addr, U16 wr_addr, U08 wr_num, U08 prio);
COM_M_EXT U08 com_FBUS_rd(U08 com, U08 obj_addr, U16 rd_addr, U08 rd_num,U08 prio);
        
COM_M_EXT U08 PDU_SlaveDealUserDef(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal);

/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/

/* ͨѶ״̬����ֵ */
#define COM_FAIL					FALSE			/* ������ʧ�� */
#define COM_SUCC					TRUE			/* ������ɹ� */
#define COM_ERR_DATA_ADDR			2				/* ��Ч���ݵ�ַ */
#define COM_ERR_MAC_ADDR			3				/* ��Ч����ַ */


/*-----------------�궨�� ---------------------------------*/






#endif /* __COM_M_H__ */
