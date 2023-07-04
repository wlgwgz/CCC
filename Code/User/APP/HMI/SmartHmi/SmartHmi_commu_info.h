/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: SmartHmi_commu_info.h
 �ļ�����: ͨѶ��Ϣ����
 ������  : hzy
 ��������: 2016.07.15
*******************************************************************************/

#ifndef SHCI_H
#define SHCI_H

#ifdef  SHCI_GLOBAL
    #define SHCI_EXT
#else
    #define SHCI_EXT extern
#endif

/*

    ע��:
    �� ���ֻ��ʹ�ô��ļ��������޸Ĵ��ļ������иĶ�����Ҫ��ǰ���Ͻ�����������ͬ�����ļ�����塣
    �� ����ǰ��Ժ��Ϊһ�Զ�Ĺ�ϵ���������޸����ֿ�����ǰ������!

*/


/***********************************************************************************
                                ��ʾ������Ϣ
***********************************************************************************/
#define DISP_PCB_NUM            0       // 0 ģ����
#define DISP_CMP_NUM            1       // 1 ѹ����
#define DISP_MAC_STA            2       // 2 ����״̬
#define DISP_TMP_CURT           3       // 3 ��ǰ�¶�
#define DISP_TMP_SET            4       // 4 �趨�¶�
#define DISP_SET_UNIT           5       // 5 �¶ȵ�λ
#define DISP_MODE_VALID         6       // 6 ����ģʽ
                                        //   ��9~16λΪ0xff���Ӳ�����ȡ����ģʽ��5~8Ϊ������ţ�1~4λΪ������ţ�
                                        //   ��9~16λ��0xff���Ӹø�����Ϣ��ȡ����ģʽ��������ϢΪλ����������15��ģʽ��
#define DISP_MODE_SET           7       // 7 �趨ģʽ
#define DISP_MODE_RUN           8       // 8 ����ģʽ
#define DISP_TIMING_USE         9       // 9 [!����]��ʱʹ��(TRUE:ʹ��,FALSE:����)
#define DISP_TIMING_GRP         10      // 10 [!����]��ʱ����(0��ʾʹ��ǰ��Ķ�ʱ�߼�)
#define DISP_INIT_STATE         11      // 11 ������ʼ��״̬
#define DISP_LMT_ON             12      // 12 ʹ�����޵���(TRUE:����,FALSE:δ��)
#define DISP_LMT_SET_DAY        13      // 13 ʹ�������趨����
#define DISP_LMT_RUN_HOUR       14      // 14 ʹ������������Сʱ��
#define DISP_LMT_LEFT_HOUR      15      // 15 ʹ������ʣ��Сʱ��
#define DISP_ERR_NUM            16      // 16 ��ǰ������
#define DISP_HIS_NUM            17      // 17 ��ʷ������
#define DISP_CHK_AV_NUM         18      // 18 [!����]��ѯ���� ģ��������
#define DISP_CHK_DV_NUM         19      // 19 [!����]��ѯ���� ����������
#define DISP_CHK_EX_NUM         20      // 20 [!����]��ѯ���� ����������
#define DISP_PARA_GRP_NUM       21      // 21 ����������
#define DISP_PASS_GRP_NUM       22      // 22 ��������
#define DISP_DEFRO_USE          23      // 23 [!����]�����ֶ���˪ģ����
#define DISP_WIFI_USE           24      // 24 ���߹���(TRUE:ʹ��,FALSE:����)
#define DISP_PRE_HEAT_TIME_MINT 25      // 25 Ԥ��ʣ�������
#define DISP_TMP_SET_MIN        26      // 26 �趨�¶���Сֵ(�����СֵͬΪ0ʱǰ�岻����Χ�ж�)
#define DISP_TMP_SET_MAX        27      // 27 �趨�¶����ֵ
#define DISP_BOOT_DOWNLOAD      28      // 28 [!����]�Ƿ���������������(TRUE:����,FALSE:����)
#define DISP_TMP_OUT            29      // 29 ��ǰϵͳ����
#define DISP_TMP_IN             30      // 30 ��ǰϵͳ����
#define DISP_CTRL_OBJ           31      // 31 ���ƶ���(0��ʾ�س��£�1��ʾ�ػ���)
#define DISP_CHK_SYS_AV_NUM     32      // 32 ϵͳģ��ģ��������
#define DISP_CHK_CMP_AV_STEP    33      // 33 ѹ��ģ��������
#define DISP_CHK_DO_STEP        34      // 34 DO�������
#define DISP_CHK_DI_STEP        35      // 35 DI���벽��
#define DISP_CHK_SYS_DO_NUM     36      // 36 ϵͳ�������������
#define DISP_SYS_COMP_RUN_NUM   37      // 37 ϵͳ������ѹ����
#define DISP_MODE_SET_INDEX     38      // 38 [!����]�趨ģʽ����(��λ��ţ���λ���)
#define DISP_TEST_EN            39      // 39 ����ģʽʹ�÷�
#define DISP_FORCE_DFRST_INFO_1 40      // 40 ǿ�Ƴ�˪��ʾ��Ϣ1��2021.03.03���ӳ�˪��Ϣ�ṹ��������ã�
#define DISP_FORCE_DFRST_INFO_2 41      // 41 ǿ�Ƴ�˪��ʾ��Ϣ2��2021.03.03���ӳ�˪��Ϣ�ṹ��������ã�
#define DISP_TMP_SET_DOT        42      // 42 �趨�¶�С����
#define DISP_CHK_USER_DO        43      // 43 �û���ѯ��DO���
#define DISP_PARA_GRP_SIZE      44      // 44 �������С
#define DISP_LMT_SET_MAX        45      // 45 ʹ�����������������ֵ
#define DISP_HAVE_MNL_DEFRO     46      // 46 �Ƿ����ֶ���˪����
#define DISP_RUN_ALLOW_SET_MODE 47      // 47 ���й����Ƿ������޸�ģʽ
#define DISP_HAVE_START_CODE    48      // 48 �Ƿ��������������루4.1���������������̶�FALSE��2021.03.03����
#define DISP_START_CODE         49      // 49 �����������루4.1���������������̶�0��2021.03.03����
#define DISP_DISP_FG            50      // 50 ���������־��λ����
                                        //    bit0���Ƿ�֧�ֵ�ѹ���ֶ���˪
                                        //    bit1���Ƿ���ʪ�ȿ���
                                        //    bit2���Ƿ���������������
                                        //    bit3���Ƿ��г�ʼ����������
                                        //    bit4���Ƿ�������Ԥ�ȹ���
                                        //    bit5���Ƿ��п���Ԥ�ȹ���
                                        //    bit6: �Ƿ���BMS���ý���
                                        //    bit7: ��Ӣ��(0:����  1:Ӣ��)
                                        //    bit8: �Ƿ��ֹ��Ӣ���л�
                                        //    bit9: �Ƿ�Ϊ���ձ�Ƶ��
                                        //    bit10:�Ƿ��������ѯ����
                                        //    bit11:�Ƿ��е�ů��ɹ���
                                        //    bit12:�Ƿ���ǿ����������
                                        //    bit13:�Ƿ���ǿ��ˮ�����
                                        //    bit14:�Ƿ���ǿ������ˮģʽ
                                        //    bit15:�Ƿ��м���ģʽ���ý���
#define DISP_ALARM_SOUND_SET    51      // 51 ����������ģʽ(0���� 1���� 2ֻ��ʮ�� 3����ʮ��)
#define DISP_MODE_TYPE          52      // 52 ����ģʽ(0���� 1���� 2�ȱ� 3��ˮ)
#define DISP_TMP_HOT            53      // 53 ��ǰ��ˮ�¶�
#define DISP_HUMI_MODE_VALID    54      // 54 ʪ�ȿ���ģʽ
                                        //    ��9~16λΪ0xff���Ӳ�����ȡ����ģʽ��5~8Ϊ������ţ�1~4λΪ������ţ�
                                        //    ��9~16λ��0xff���Ӹø�����Ϣ��ȡ����ģʽ��������ϢΪλ����������15��ģʽ��
#define DISP_HUMI_MODE_SET      55      // 55 ʪ���趨ģʽ
#define DISP_HUMI_MODE_RUN      56      // 56 ʪ������ģʽ
#define DISP_HUMI_CURT          57      // 57 ��ǰʪ��
#define DISP_HUMI_SET           58      // 58 �趨ʪ��
#define DISP_HUMI_SET_MIN       59      // 59 ʪ���趨��Сֵ
#define DISP_HUMI_SET_MAX       60      // 60 ʪ���趨���ֵ
#define DISP_DISP_FG_EX         61      // 61 ���������־(λ����)DISP_DISP_FG��������
                                        //    bit0: �Ƿ��з�����Ϣ����
                                        //    bit1: �Ƿ����������Խ���
                                        //    bit2: �Ƿ��еڶ����趨�¶�
                                        //    bit3: �Ƿ�����ˮ�趨
                                        //    bit4: �Ƿ��б�Ƶ��������
                                        //    bit5: �Ƿ��б�Ƶ���ϲ�ѯ
                                        //    bit6: �Ƿ����շ�����
                                        //    bit7���Ƿ���ð�����
                                        //    bit8���Ƿ���ǿ���ⲿ��Դ
                                        //    bit9���Ƿ��ھ���ģʽ
                                        //    bit10:�Ƿ���ˮ��������
                                        //    bit11:�Ƿ�������������
                                        //    bit12:�Ƿ�������������
                                        //    bit13:�Ƿ�������ˮ������
                                        //    bit14:�Ƿ���ϵͳ�ſյ���
                                        //    bit15:�Ƿ�ʹ��ͯ������
#define DISP_CAN_DO_FG          62      // 62 ���������־(λ����)  �Ƿ���Զ���
                                        //    bit0: �Ƿ����ǿ����������
                                        //    bit1: �Ƿ����ǿ��ˮ�����
#define DISP_TMP_SET_EX         63      // 63 �ڶ����趨�¶�
#define DISP_TMP_CURT_EX        64      // 64 �ڶ�����ǰ�¶�
#define DISP_TMP_SET_EX_MIN     65      // 65 �ڶ����趨�¶���Сֵ
#define DISP_TMP_SET_EX_MAX     66      // 66 �ڶ����趨�¶����ֵ
#define DISP_TMP_HOT_SET        67      // 67 ��ˮ�趨�¶�
#define DISP_TMP_HOT_SET_MIN    68      // 68 ��ˮ�趨�¶���Сֵ
#define DISP_TMP_HOT_SET_MAX    69      // 69 ��ˮ�趨�¶����ֵ
#define DISP_LMT_TYPE_INFO      70      // 70 ϵͳά��������Ϣ
#define DISP_LMT_TYPE_VALUE     71      // 71 ϵͳά������
#define DISP_INVT_ERR_NUM       72      // 72 ��Ƶ���ϸ���
#define DISP_SILENT_LEVEL       73      // 73 ����ģʽ����
#define DISP_SILENT_LEVEL_ATTR  74      // 74 ����ģʽ����������
#define DISP_ROOM_TEMP_WR_INTVL 75      // 75 �·������¶ȵļ�� ��λ�룬0��ʾʹ��ǰ��Ĭ��ֵ30��
#define DISP_BKBD_LANGUAGE      76      // 76 �������
#define DISP_DISP_FG_EX2        77      // 77 ���������־(λ��־)DISP_DISP_FG_EX��������
                                        //    bit0: �Ƿ������е�ůԤ��
                                        //    bit1: �Ƿ������ж�ʱ��ů���
                                        //    bit2: �Ƿ��ڼ����뿪ģʽ
                                        //    bit3: �Ƿ��ڼ����ڼ�ģʽ
                                        //    bit4: �Ƿ���4G����
                                        //    bit5: �Ƿ���Զ����������
                                        //    bit6: �Ƿ��ڵ�ů��ɹ���
                                        //    bit7: �Ƿ��ڸ�������ǿ��
                                        //    bit8: �Ƿ���ˮ�����ǿ��
                                        //    bit9: �Ƿ���ǿ����ˮģʽ
                                        //    bit10:�Ƿ���ǿ���ⲿ��Դ
                                        //    bit11:�Ƿ�����������
                                        //    bit12:�Ƿ�ģ��Ŵ�1��ʼ��ʾ
                                        //    bit13:�Ƿ��û�������(�����û���)Ȩ���г�ʼ������
                                        /*    bit14:�Ƿ���ʾ�����������ѯ���� */
                                        /*    bit15:�Ƿ��л���������ʾ */
#define DISP_DISP_LOGO          78      // 78 ��ʾlogo
#define DISP_DISP_FG_EX3        79      // 79 ���������־(λ��־)DISP_DISP_FG_EX2��������
                                        //    bit0: �Ƿ����̼칦��
                                        //    bit1: �Ƿ���wifiOTA����
                                        //    bit2���Ƿ���3��ʹ������
#define DISP_LANGUAGE_FG1       80      // 80 ��ʾ��������1, bit15~bit0��Ӧ�������Զ����0~15
#define DISP_LANGUAGE_FG2       81      // 81 ��ʾ��������2, bit15~bit0��Ӧ�������Զ����16~30
                                        // ע��������Ը�������31, ����Ҫ�޸�3.5�翪��ʱ�Ĵ���

#define DISP_INFO_MAX           90      // �����ʾ��Ϣ����,������Ҫ�ʵ��޸�,��С��ջѹ��,����������0
                                        // ����ÿ������10,�Ա㾡�緢�ֶ�ջ����
#define SH_MAX_DISP_INFO        100     // �����ʾ��Ϣ����

// 2 ����״̬
#define SH_IDLE                 0           /* ���� */
#define SH_READY                1           /* ���� */
#define SH_RUN                  2           /* ���� */
#define SH_DELAY                3           /* ͣ�� */
#define SH_ALARM                4           /* ���� */
#define SH_DEFREZZ              5           /* ���� */
#define SH_DEFROST              6           /* ��˪ */
#define SH_WARM                 7           /* Ԥ�� */
#define SH_CLR_SNOW             8           /* ��ѩ */
#define SH_LINK                 9           /* ��������ͨ�������� */
#define SH_ECONOMY              10          /* ���� */
#define SH_SILENT               11          /* ���� */
#define SH_STANDARD             12          /* ��׼ */
#define SH_STRONG               13          /* ǿ�� */
#define SH_STER                 14          /* ɱ�� */
#define SH_DRYUP                15          /* ��� */
#define SH_HOLIDAY              16          /* ���� */
#define SH_TEST                 17          /* ���� */
#define SH_REOIL                18          /* ���� */
#define SH_HEAT_LMT             19          /* �������� */
#define SH_TEST_FAC             20          /* �̼� */
#define SH_MAC_STATUS_MAX       21          /* ���״̬�� */
#define SH_DEFREZZ_WARM         (0xff-1)//����Ԥ��

// 3��4 �¶ȵ�λ
#define SH_SET_UNIT_C           0           /* ���϶� */
#define SH_SET_UNIT_F           1           /* ���϶� */
#define SH_SET_UNIT_PA          2           /* Pa */
#define SH_SET_UNIT_MAX         3           /* ����趨��λ */

// 7��8 ģʽ (����ģʽ��Ҫ��ǰ������ģʽ�ַ�����ģʽ����ͼ��)
#define SH_MODE_AUTO            0       // 0 �Զ�(�趨ģʽ����Ϊ�Զ�������ģʽ����Ϊ�Զ�)
#define SH_MODE_FAN             1       // 1 ͨ��
#define SH_MODE_COOL            2       // 2 ����
#define SH_MODE_HEAT            3       // 3 ����
#define SH_MODE_HOTWATER        4       // 4 ��ˮ
#define SH_MODE_ICE             5       // 5 ���
#define SH_MODE_FLOOR           6       // 6 ��ů
#define SH_MODE_COOL_HW         7       // 7 ����+��ˮ
#define SH_MODE_HEAT_HW         8       // 8 ����+��ˮ
#define SH_MODE_MAX             9       //  ���ģʽ��

// 11 ������ʼ��״̬
#define SH_STATE_IDLE           0       // δ��ʼ
#define SH_STATE_ING            1       // ������
#define SH_STATE_SUCC           2       // �ɹ�
#define SH_STATE_FAIL           3       // ʧ��

// 31 ���ƶ���
#define SH_CTRL_OBJ_OUT         0       //����
#define SH_CTRL_OBJ_IN          1       //����

// 43 �û���ѯ�����������Ϣ
#define SH_CHK_USER_DO_PUMP_BIT 0
#define SH_CHK_USER_DO_FAN_BIT  1
#define SH_CHK_USER_DO_COMP_BIT 2

// 50 ���������־��λ����
#define SH_HAVE_SINGLE_CM_MNL_DEFROST   (GETBIT(disp_info[DISP_DISP_FG], 0))/* bit0���Ƿ�֧�ֵ�ѹ���ֶ���˪ */
#define SH_HAVE_HUMI_CTRL               (GETBIT(disp_info[DISP_DISP_FG], 1))/* bit1���Ƿ���ʪ�ȿ��� */
#define SH_HAVE_START_CODE              (GETBIT(disp_info[DISP_DISP_FG], 2))/* bit2���Ƿ��������������� */
#define SH_HAVE_RESET_PARA_PASS         (GETBIT(disp_info[DISP_DISP_FG], 3))/* bit3���Ƿ��г�ʼ���������� */
#define SH_HAVE_OFF_OILHEAT             (GETBIT(disp_info[DISP_DISP_FG], 4))/* bit4���Ƿ�������Ԥ�ȹ��� */
#define SH_HAVE_QUICK_OILHEAT           (GETBIT(disp_info[DISP_DISP_FG], 5))/* bit5���Ƿ��п���Ԥ�ȹ��� */
#define SH_HAVE_BMS_SET                 (GETBIT(disp_info[DISP_DISP_FG], 6))/* bit6: �Ƿ���BMS���ý��� */
#define SH_IS_ENGLISH_DISP              (GETBIT(disp_info[DISP_DISP_FG], 7))/* bit7: �Ƿ�ΪӢ����ʾ */
#define SH_FORBID_SET_LANGUAGE          (GETBIT(disp_info[DISP_DISP_FG], 8))/* bit8: �Ƿ��ֹ��Ӣ���л� */
#define SH_IS_PUNP_INVT                 (GETBIT(disp_info[DISP_DISP_FG], 9))/* bit9: �Ƿ�Ϊ���ձ�Ƶ�� */
#define SH_HAVE_PASS_CHK                (GETBIT(disp_info[DISP_DISP_FG],10))/* bit10:�Ƿ��������ѯ���� */
#define SH_HAVE_FLOOR_DRYUP             (GETBIT(disp_info[DISP_DISP_FG],11))/* bit11:�Ƿ��е�ů��ɹ���*/
#define SH_HAVE_FORCE_IBH               (GETBIT(disp_info[DISP_DISP_FG],12))/* bit12:�Ƿ���ǿ����������*/
#define SH_HAVE_FORCE_TBH               (GETBIT(disp_info[DISP_DISP_FG],13))/* bit13:�Ƿ���ǿ��ˮ�����*/
#define SH_HAVE_FORCE_HW_MODE           (GETBIT(disp_info[DISP_DISP_FG],14))/* bit14:�Ƿ���ǿ������ˮģʽ*/
#define SH_HAVE_HOLIDAY_MODE            (GETBIT(disp_info[DISP_DISP_FG],15))/* bit15:�Ƿ��м���ģʽ���ý���*/

// 51 ����������ģʽ(ע�⣺���Ķ�����0���� 1���� 2ֻ��ʮ�� 3����ʮ�룬�����Ҫ����һ��ת��)
#define SH_ALARM_SOUND_SET_ON           0   /* ���� */
#define SH_ALARM_SOUND_SET_OFF          1   /* ���� */
#define SH_ALARM_SOUND_SET_10S_ONCE     2   /* ֻ��ʮ�� */
#define SH_ALARM_SOUND_SET_10S_CYCLE    3   /* ����ʮ�� */

// 52 ����ģʽ(0���� 1���� 2�ȱ� 3��ˮ)
#define SH_MODE_HEAT_ONLY       0
#define SH_MODE_COOL_ONLY       1
#define SH_MODE_HEAT_PUMP       2
#define SH_MODE_HOT_WATER       3

// 55��56 ʪ��ģʽ(����ģʽ��Ҫ��ǰ������ģʽ�ַ�����ģʽ����ͼ��)
#define SH_HUNI_NULL            0       // 0 �Զ�(�趨ģʽ����Ϊ�Զ�������ģʽ����Ϊ�Զ�)
#define SH_MODE_DEHUMI          1       // 1 ��ʪ
#define SH_MODE_ADDHUMI         2       // 2 ��ʪ
#define SH_HUMI_MODE_MAX        3       // 3 ���ģʽ��

// 61 ���������־��λ����
#define SH_HAVE_FCUINFO             (GETBIT(disp_info[DISP_DISP_FG_EX], 0)) /* bit0: �Ƿ��з�����Ϣ���� */
#define SH_HAVE_ABILITY_TEST        (GETBIT(disp_info[DISP_DISP_FG_EX], 1)) /* bit1: �Ƿ����������Խ��� */
#define SH_HAVE_SET_TMP_EX          (GETBIT(disp_info[DISP_DISP_FG_EX], 2)) /* bit2: �Ƿ��еڶ����趨�¶� */
#define SH_HAVE_SET_HOT             (GETBIT(disp_info[DISP_DISP_FG_EX], 3)) /* bit3: �Ƿ�����ˮ�趨 */
#define SH_HAVE_INVT_PARA           (GETBIT(disp_info[DISP_DISP_FG_EX], 4)) /* bit4: �Ƿ��б�Ƶ�������� */
#define SH_HAVE_INVT_ERR            (GETBIT(disp_info[DISP_DISP_FG_EX], 5)) /* bit5: �Ƿ��б�Ƶ���ϲ�ѯ */
#define SH_HAVE_RECYCLE_REFRI       (GETBIT(disp_info[DISP_DISP_FG_EX], 6)) /* bit6: �Ƿ����շ����� */
#define SH_HAVE_FORBID_KEY_BUZZER   (GETBIT(disp_info[DISP_DISP_FG_EX], 7)) /* bit7���Ƿ���ð����� */
#define SH_HAVE_FORCE_AHS           (GETBIT(disp_info[DISP_DISP_FG_EX], 8)) /* bit8���Ƿ���ǿ���ⲿ��Դ */
#define SH_IS_SILENT_MODE_ON        (GETBIT(disp_info[DISP_DISP_FG_EX], 9)) /* bit9���Ƿ��ھ���ģʽ */
#define SH_IS_PUMP_TEST_ON          (GETBIT(disp_info[DISP_DISP_FG_EX],10)) /* bit10:�Ƿ���ˮ�������� */
#define SH_IS_COOL_TEST_ON          (GETBIT(disp_info[DISP_DISP_FG_EX],11)) /* bit11:�Ƿ������������� */
#define SH_IS_HEAT_TEST_ON          (GETBIT(disp_info[DISP_DISP_FG_EX],12)) /* bit12:�Ƿ������������� */
#define SH_IS_HOTWATER_TEST_ON      (GETBIT(disp_info[DISP_DISP_FG_EX],13)) /* bit13:�Ƿ�������ˮ������ */
#define SH_IS_SYSTEM_EMPTY_ON       (GETBIT(disp_info[DISP_DISP_FG_EX],14)) /* bit14:�Ƿ���ϵͳ�ſյ��� */
#define SH_HAVE_CHILD_LOCK          (GETBIT(disp_info[DISP_DISP_FG_EX],15)) /* bit15:�Ƿ�ʹ��ͯ������ */

// 62 ���������־��λ�����Ƿ���Զ���
#define SH_CAN_FORCE_IBH            (GETBIT(disp_info[DISP_CAN_DO_FG], 0))/* bit0: �Ƿ����ǿ�ƿ��������� */
#define SH_CAN_FORCE_TBH            (GETBIT(disp_info[DISP_CAN_DO_FG], 1))/* bit1: �Ƿ����ǿ�ƿ�ˮ����� */

// 70 ϵͳά��������Ϣ
#define SH_LMT_TYPE_STRING_ATTR     (HIBYTE(disp_info[DISP_LMT_TYPE_INFO])) /* ϵͳά������ѡ�� */
#define SH_LMT_TYPE_VALUE_MIN       (LOBYTE(disp_info[DISP_LMT_TYPE_INFO]) & 0x0F)
#define SH_LMT_TYPE_VALUE_MAX       ((LOBYTE(disp_info[DISP_LMT_TYPE_INFO]) & 0xF0)>>4)
#define SH_HAVE_LMT_TYPE_SET        (disp_info[DISP_LMT_TYPE_INFO] != 0)

// 71 ʹ����������ֵ
#define SH_LMT_TYPE_STOP_UNIT       0
#define SH_LMT_TYPE_KEEP_RUN        1

// 74 ��������������
#define SH_SILENT_LEVEL_MIN         (LOBYTE(disp_info[DISP_SILENT_LEVEL_ATTR]))
#define SH_SILENT_LEVEL_MAX         (HIBYTE(disp_info[DISP_SILENT_LEVEL_ATTR]))

// 76 ����
#define SH_LANGUAGE_CN      0               /* ���� */
#define SH_LANGUAGE_EN      1               /* Ӣ�� */
#define SH_LANGUAGE_ES      2               /* �������� */
#define SH_LANGUAGE_GR      3               /* ϣ�� */
#define SH_LANGUAGE_FR      4               /* ���� */
#define SH_LANGUAGE_IT      5               /* ����� */
#define SH_LANGUAGE_PL      6               /* ���� */
#define SH_LANGUAGE_CZ      7               /* �ݿ� */
#define SH_LANGUAGE_PT      8               /* ������ */
#define SH_LANGUAGE_TR      9               /* ������ */
#define SH_LANGUAGE_GE      10              /* ���� */
#define SH_LANGUAGE_NL      11              /* ���� */
#define SH_LANGUAGE_SE      12              /* ��� */
#define SH_LANGUAGE_DK      13              /* ���� */
#define SH_LANGUAGE_NO      14              /* Ų�� */
#define SH_LANGUAGE_HG      15              /* ������ */
#define SH_LANGUAGE_BG      16				/* �������� */
#define SH_LANGUAGE_MAX     17

// 77 ���������־��λ����
#define SH_IS_FLOOR_PREHEAT_RUN     (GETBIT(disp_info[DISP_DISP_FG_EX2], 0))/* bit0: �Ƿ������е�ůԤ�� */
#define SH_IS_FLOOR_TIMER_RUN       (GETBIT(disp_info[DISP_DISP_FG_EX2], 1))/* bit1: �Ƿ������ж�ʱ��ů��� */
#define SH_IS_HOLIDAY_AWAY          (GETBIT(disp_info[DISP_DISP_FG_EX2], 2))/* bit2: �Ƿ��ڼ����뿪ģʽ */
#define SH_IS_HOLIDAY_HOME          (GETBIT(disp_info[DISP_DISP_FG_EX2], 3))/* bit3: �Ƿ��ڼ����ڼ�ģʽ */
#define SH_HAVE_4G_FUNCTION         (GETBIT(disp_info[DISP_DISP_FG_EX2], 4))/* bit4: �Ƿ���4G���� */
#define SH_HAVE_REMOTE_UPGRADE_REQ  (GETBIT(disp_info[DISP_DISP_FG_EX2], 5))/* bit5: �Ƿ���Զ���������� */
#define SH_IS_FLOOR_DRYUP_ON        (GETBIT(disp_info[DISP_DISP_FG_EX2], 6))/* bit6: �Ƿ��ڵ�ů��ɹ��� */
#define SH_IS_FORCE_IBH_ON          (GETBIT(disp_info[DISP_DISP_FG_EX2], 7))/* bit7: �Ƿ��ڸ�������ǿ�� */
#define SH_IS_FORCE_TBH_ON          (GETBIT(disp_info[DISP_DISP_FG_EX2], 8))/* bit8: �Ƿ���ˮ�����ǿ�� */
#define SH_IS_FORCE_HOT_WATER_ON    (GETBIT(disp_info[DISP_DISP_FG_EX2], 9))/* bit9: �Ƿ���ǿ����ˮģʽ */
#define SH_IS_FORCE_AHS_ON          (GETBIT(disp_info[DISP_DISP_FG_EX2],10))/* bit10:�Ƿ���ǿ���ⲿ��Դ */
#define SH_IS_HEAT_LIMIT            (GETBIT(disp_info[DISP_DISP_FG_EX2],11))/* bit11:�Ƿ����������� */
#define SH_IS_MOD_START_WITH_ONE    (GETBIT(disp_info[DISP_DISP_FG_EX2],12))/* bit12:�Ƿ�ģ��Ŵ�1��ʼ��ʾ */
#define SH_IS_USER_PWR_HAVE_CLR     (GETBIT(disp_info[DISP_DISP_FG_EX2],13))/* bit13:�Ƿ��û�������(�����û���)Ȩ���г�ʼ������ */
#define SH_IS_DISP_DI               (GETBIT(disp_info[DISP_DISP_FG_EX2],14))/* bit14:�Ƿ���ʾ�����������ѯ���� */
#define SH_HAVE_MAC_VER_DISP        (GETBIT(disp_info[DISP_DISP_FG_EX2],15))/* bit15:�Ƿ��л���������ʾ */

// 79 ���������־��λ����
#define SH_HAVE_TEST_FAC            (GETBIT(disp_info[DISP_DISP_FG_EX3], 0))/* bit0: �Ƿ����̼칦�� */
#define SH_HAVE_WIFI_OTA            (GETBIT(disp_info[DISP_DISP_FG_EX3], 1))/* bit1: �Ƿ���wifiOTA���� */
#define SH_HAVE_3_TERM_LIMIT        (GETBIT(disp_info[DISP_DISP_FG_EX3], 2))/* bit2: �Ƿ���3��ʹ������ */
/***********************************************************************************
                                ��λ��
***********************************************************************************/
#define SH_UNIT_NULL            0       // 0 ��
#define SH_UNIT_TMP_C           1       // 1 �¶�(��)
#define SH_UNIT_TMP_F           2       // 2 �¶�(�H)
#define SH_UNIT_A               3       // 3 ����(A)
#define SH_UNIT_mA              4       // 4 ����(mA)
#define SH_UNIT_PRS_B           5       // 5 ѹ��(bar)
#define SH_UNIT_PE              6       // 6 �ٷֱ�(%)
#define SH_UNIT_HZ              7       // 7 Ƶ��(HZ)
#define SH_UNIT_RPS             8       // 8 ת��(rps)
#define SH_UNIT_RPM             9       // 9 ת��(rpm)
#define SH_UNIT_YEAR            10      // 10 ��
#define SH_UNIT_MONTH           11      // 11 ��
#define SH_UNIT_DAY             12      // 12 ��
#define SH_UNIT_HOUR            13      // 13 ʱ
#define SH_UNIT_MINT            14      // 14 ��
#define SH_UNIT_SECOND          15      // 15 ��
#define SH_UNIT_ST              16      // 16 ��
#define SH_UNIT_V               17      // 17 ��ѹ(V)
#define SH_UNIT_mV              18      // 18 ��ѹ(mV)
#define SH_UNIT_kO              19      // 19 ����(k��)
#define SH_UNIT_kW              20      // 20 ����(kW)
#define SH_UNIT_PA              21      // 21 ѹ��(Pa)
#define SH_UNIT_LH              22      // 22 ����(L/H)
#define SH_UNIT_kWh             23      // 23 �õ���(kWh)

#define SH_UNIT_MAX             24      //���λ��

/***********************************************************************************
                                ��ѯ��Ϣ
***********************************************************************************/
#define NO_ID                   0xff    //0xff����ʾ���
typedef struct tagSH_CHK_INFO           //״̬��Ϣ
{
    U16 name;                           //�����ַ�����
    U16 id;                             //���(��8λΪǰ׺�ţ���8λΪ��׺��ţ�Ϊ0xffʱ����ʾ���)
    I16 value;                          //��ֵ(������ַ������ʾ�ַ�����)
    U16 attrib;                         //����(0:�ַ��͡�1~8:��λ��9~11:С��λ)
}SH_CHK_INFO;
#define SH_MAX_CHK_INFO         128     //������ʾ��ѯ��Ϣ��

#define SH_MAX_CHK_USER_INFO    6
#define SH_MAX_CHK_SYS_INFO     32
#define SH_MAX_CHK_COMP_INFO    24
#define SH_MAX_CHK_DO_INFO      20
#define SH_MAX_CHK_DI_INFO      20

/***********************************************************************************
                                �汾��Ϣ
***********************************************************************************/
#define MAX_VER_BYTE            32      //�汾���ֽ���

typedef struct tagSH_CHK_VER            //�汾��Ϣ
{
    U16 pcb_name;                       //�����ַ�����
    U16 pcb_id;                         //���(Ϊ0xffʱ����ʾ��ţ�Ϊ0xffffʱ����ʾ�ð�汾��)
    U08 fun_code[MAX_VER_BYTE];         //������
}SH_CHK_VER;
#define SH_MAX_PCB_NUM          16      //������
#define SH_MAX_COMP_NUM         4       //����ѹ��
#define SH_MAX_EXIO_NUM         4       //�����չ�����



/***********************************************************************************
                                ������Ϣ
***********************************************************************************/
typedef struct tagSH_ERR_TIME           //����ʱ��
{
    U16 month;                          //�� (����ͬΪ0����ʾ����)
    U16 day;                            //��
    U16 hour;                           //ʱ (ʱ��ͬΪ0����ʾʱ��)
    U16 mint;                           //��
}SH_ERR_TIME;

typedef struct tagSH_ERR_HIS            //��ʷ������Ϣ
{
    U16 name;                           //�����ַ�����
    U16 id;                             //���(��8λΪ��ţ���8λΪ��Ԫ�ţ�Ϊ0xffʱ����ʾ���)
    U16 err_id;                         //���Ϻ�
    SH_ERR_TIME time;                   //����ʱ��
}SH_ERR_HIS;
#define SH_MAX_ERR_HIS_NUM      100     //�����ʷ������
#define SH_ERR_WEI_ZHI_GZ       0

typedef struct tagSH_ERR_CURT           //��ǰ������Ϣ
{
    U16 name;                           //�����ַ�����
    U16 id;                             //���(��8λΪ��ţ���8λΪ��Ԫ�ţ�Ϊ0xffʱ����ʾ���)
    U16 err_id;                         //���Ϻ�
}SH_ERR_CURT;
#define SH_MAX_ERR_CURT_NUM     100     //���ǰ������



/***********************************************************************************
                                ��˪��Ϣ
***********************************************************************************/
typedef struct tagSH_DEFRO_INFO
{
    U16 cell;                           // ģ���˪��Ϣ,0~3:ģ���˪��Ϣ,4~7:ģ��ѹ������
    U16 cms;                            // ѹ����˪��Ϣ(һ��ѹ��ռ2λ)
}SH_DEFRO_INFO;

enum
{
    SH_DEFR_NO_RUN=0,
    SH_DEFR_RUN,
    SH_DEFR_FORCE_DEFORST,
    SH_DEFR_DEFROST
};

/***********************************************************************************
                                ʹ��/������Ϣ
***********************************************************************************/
typedef struct tagSH_ENABLE_INFO
{
    U16 pcb_enable;            // ģ�������Ϣ,ÿ��ģ��һ��λ
    U16 pcb_commu_err;         // ģ��ͨ�Ź�����Ϣ
    U16 bak[2];                // ����2����
    U16 comp_enable[SH_MAX_COMP_NUM]; // ѹ��������Ϣ,ÿ��ģ��һ��λ
}SH_ENABLE_INFO;
#if SH_MAX_PCB_NUM > 16
    #error "ģ������16,�޸ı�������!"
#endif


/***********************************************************************************
                                ������Ϣ
***********************************************************************************/
//��������
#define SH_UNIT         0x00ff          //bit0~7:   0~255����ֵС��32,��ʾΪ��ֵ�ͣ�ֵΪ��λ�ţ����֧��32����λ����ֵ����32����ʾΪ�ַ����Ͳ�����ֵ��ȥƫ��ֵ32֮������ַ����
#define SH_DECIMAL      0x0300          //bit8~9:   С��λ��(0~3)
#define SH_HIDE         0x0400          //bit10:    ��λΪ1��ʾ"����"��Ϊ0��ʾ"������"
#define SH_CONST        0x0800          //bit11:    ��λΪ1��ʾ"���ɸ�"��Ϊ0��ʾ"���޸�"

#define SH_UNSIGNED     0x0800          //bit11:    ����Ϊ�޷���������

#define SH_SAFE         0x7000          //bit12~14 Ȩ�ޣ�0���û��� 1�����̣� 2��ά�ޣ� 3�����ң�
#define SH_BIT          0x8000          //bit15:    ��λΪ1��ʾ"λ����"��Ϊ0��ʾ"��λ����" Ų�����һλ������������Ҫʱ��ȡ��λ��������
                                        //          ������Ϊ��λ������ʱ��ռ��2~3���ַ���š�
                                        //          �׸��ַ���Ϊ����λ�������Ӧ�ַ�������2�ַ���Ϊ0~7λ��Ӧ�ַ�������3�ַ���Ϊ8~15λ��Ӧ�ַ�����
                                        //          �׸��ַ�����ű����� SH_UNIT ��

#define UNIT_OFFSET     32              // ��λƫ����
#define DECIMAL_BGN_BIT 8               // С��λ�� ��ʼλ
#define HIDE_BGN_BIT    10              // �������� ��ʼλ
#define CONST_BGN_BIT   11              // �����޸� ��ʼλ
#define SAFE_BGN_BIT    12              // ����Ȩ�� ��ʼλ
#define BIT_BGN_BIT     15              // λ����   ��ʼλ


typedef struct tagSH_PARA_GRP           //��������Ϣ
{
    U16 name;                           //�����ַ�����
    U16 item_num;                       //���ڲ�������
    U16 attrib;                         //����(1~8:Ȩ�ޡ�12:����)
}SH_PARA_GRP;
#define SH_MAX_PARA_GRP_NUM     40      //����������


typedef struct tagSH_PARA_ATTRIB        //����������
{
    U16 name;                           //�����ַ�����
    U16 id;                             //���(��8λΪǰ׺�ţ���8λΪ��׺��ţ�Ϊ0xffʱ����ʾ���)
    I16 value;                          //����ֵ
    I16 min;                            //��Сֵ
    I16 max;                            //���ֵ
    U16 attrib;                         //����(0:�ַ����͡�1~8:��λ��9~11:С��λ��12:���ء�13:�����޸ġ�14:λ����)
}SH_PARA_ATTRIB;
#define SH_MAX_PARA_ITEM_NUM    50      //����������

#define SH_MAX_PARA_ITEM_TOTAL  1280

#define SH_MAX_PARA_STRING_GRP  100     //����ַ�����
#define SH_MAX_DICT_STR         400     //����ֵ���        /* �漰������ĺ����п�����U08�ı�������ע�� */
typedef struct tagSH_DICT_ATTR
{
    U16 idx_ofs[SH_MAX_PARA_STRING_GRP];
}SH_DICT_ATTR;

typedef struct tagSH_CHK_INFO_NUM      //��ѯ������Ϣ����
{
    U16 cell_pv_num;                   //��Ԫģ��������
    U16 comp_pv_num;                   //ѹ��ģ��������
    U16 do_num;                        //�������������
    U16 di_num;                        //�������������
}SH_CHK_INFO_NUM;

#define SH_STR_BEI_YONG         0      // 0 ����
#define SH_STR_WEI_ZHI          1      // 1 δ֪
#define SH_STR_WU_XIAN_S        2      // 2 ����ʾ
#define SH_MAX_BMS_PARA_NUM     5      //���BMSͨѶ��������



/***********************************************************************************
                                ʱ��
***********************************************************************************/
typedef struct tagSH_CLOCK
{
    U16 year;                       //��:2000~2099
    U16 month;                      //��:1~12
    U16 day;                        //��:1~31
    U16 week;                       //����:0~6
    U16 hour;                       //ʱ:0~23
    U16 min;                        //��:0~59
    U16 sec;                        //��:0~59
}SH_CLOCK;



/***********************************************************************************
                                ��ʱ
***********************************************************************************/
typedef struct tagSH_TIMING
{
    U16 hour;                       //ʱ:0~23
    U16 min;                        //��:0~59
    U16 info;                       //��ʱ��Ϣ: bit0~bit6:����һ~�����죻bit7:0��/1��
}SH_TIMING;
#define SH_MAX_TIMING_GRP   12      //���ʱ����



/***********************************************************************************    
                                3��ʹ������
***********************************************************************************/
#define SH_MAX_LIMIT_GRP   3 
typedef struct tagSH_LIMIT
{
    U16 limit_sign;                       
    SH_PARA_ATTRIB limit_info[SH_MAX_LIMIT_GRP];                        
}SH_LIMIT;

#define SH_USE_FIRST_LIMIT           0/* bit0: ��һ���Ƿ�ʹ�� */
#define SH_USE_SECOND_LIMIT          1/* bit1: �ڶ����Ƿ�ʹ�� */
#define SH_USE_THIRD_LIMIT           2/* bit2: �������Ƿ�ʹ�� */
#define SH_EXPIRE_FIRST_LIMIT        3/* bit3: ��һ���Ƿ��� */
#define SH_EXPIRE_SECOND_LIMIT       4/* bit4: �ڶ����Ƿ��� */
#define SH_EXPIRE_THIRD_LIMIT        5/* bit5: �������Ƿ��� */
/***********************************************************************************    
                                ����
***********************************************************************************/
//���
#define SH_PWR_NONE         0       //δ��¼
#define SH_PWR_SALE         1       //������(ʹ������)
#define SH_PWR_USER         2       //�û�
#define SH_PWR_PRO          4       //���̵���Ա
#define SH_PWR_SER          6       //����ά��Ա
#define SH_PWR_OMNI         7       //���ܽ�������
#define SH_PWR_FAC          9       //���ҹ���Ա
#define SH_PWR_SPR          10      //��������
#define SH_PWR_TERM1        11      //����1����
#define SH_PWR_TERM2        12      //����2����
#define SH_PWR_TERM3        13      //����3����

#define SH_PWR_INIT_PARA    14      //��ʼ����������
#define SH_PWR_START_MAC    15      //������������

//����
#define MAX_PASS_BIT        8       //�������λ��

typedef struct tagSH_PASS
{
    U16 authority;                  //Ȩ�޼���
    U08 pass_bit[MAX_PASS_BIT];     // 8λ����
}SH_PASS;
#define SH_MAX_PASS_GRP     16      //�����������



/***********************************************************************************
                                ����Ȩ��
***********************************************************************************/
#define SH_PARA_USR         0       //�û�
#define SH_PARA_PRO         1       //����
#define SH_PARA_SER         2       //ά��
#define SH_PARA_FAC         3       //����

/***********************************************************************************
                                �����
***********************************************************************************/
#define SH_CMD_ON                           0       // 0 ����
#define SH_CMD_OFF                          1       // 1 �ػ�
#define SH_CMD_RST                          2       // 2 ���ϸ�λ
#define SH_CMD_PASS_RST                     3       // 3 ������ϸ�λ
#define SH_CMD_OFF_OILHEAT                  4       // 4 ȡ��Ԥ��
#define SH_CMD_DEFRO                        5       // 5 ѹ���ֶ���˪
#define SH_CMD_PARA_INIT                    6       // 6 ������ʼ��
#define SH_CMD_CM_RUNTM_INIT                7       // 7 ѹ���ۼ�����ʱ���ʼ��
#define SH_CMD_SYS_RUNTM_INIT               8       // 8 ϵͳ�ۼ�����ʱ���ʼ��
#define SH_CMD_HIS_ERR_INIT                 9       // 9 �����ʷ����
#define SH_CMD_INVERTER_INIT                10      // 10 ��ʼ����Ƶ��
#define SH_CMD_PUMP                         11      // 11 �ֶ�����ˮ��
#define SH_CMD_BMS_PARA_SET                 12      // 12 BMS��������(�������ݸ�8λΪ��š���8λΪֵ)
#define SH_CMD_QUICK_OILHEAT                13      // 13 ����Ԥ��
#define SH_CMD_TUYA_NEW_LINK                14      // 14 Ϳѻ�������ӱ�־
#define SH_CMD_INVT_INFO_RD                 15      // 15 ��Ƶ��Ϣ��ȡ(�������ݸ�8λģ��š���8Ϊѹ����)
#define SH_CMD_ENTER_FLOOR_DRYUP            16      // 16 �����ů��ɹ���
#define SH_CMD_FORCE_IBH                    17      // 17 ǿ����������
#define SH_CMD_FORCE_TBH                    18      // 18 ǿ��ˮ�����
#define SH_CMD_FORCE_HW_MODE                19      // 19 ǿ����ˮģʽ
#define SH_CMD_ABILITY_TEST                 20      // 20 ��������
#define SH_CMD_ON_ZONE_1                    21      // 21 ����һ����
#define SH_CMD_OFF_ZONE_1                   22      // 22 ����һ�ػ�
#define SH_CMD_ON_ZONE_2                    23      // 23 ���������
#define SH_CMD_OFF_ZONE_2                   24      // 24 ������ػ�
#define SH_CMD_ON_HOTW                      25      // 25 ��ˮ����
#define SH_CMD_OFF_HOTW                     26      // 26 ��ˮ�ػ�
#define SH_CMD_LIMIT_INIT                   27      // 27 ϵͳά����ʼ��
#define SH_CMD_RECYCLE_REFRI                28      // 28 �շ�����
#define SH_CMD_TIMER_DHW_ON                 29      // 29 ��ʱ����ˮ��
#define SH_CMD_FORCE_AHS                    30      // 30 ǿ���ⲿ��Դ
#define SH_CMD_DISINFECT                    31      // 31 ɱ��
#define SH_CMD_SILENT                       32      // 32 ����ģʽ
#define SH_CMD_PUMP_TEST                    33      // 33 ˮ��������
#define SH_CMD_COOL_TEST                    34      // 34 ����������
#define SH_CMD_HEAT_TEST                    35      // 35 ����������
#define SH_CMD_HOTWATER_TEST                36      // 36 ����ˮ������
#define SH_CMD_SYSTEM_EMPTY                 37      // 37 ϵͳ�ſյ���
#define SH_CMD_FLOOR_PREHEAT                38      // 38 ��ůԤ������
#define SH_CMD_FLOOR_TIMER                  39      // 39 ��ʱ��ů����
#define SH_CMD_HOLIDAY_AWAY                 40      // 40 �����뿪����
#define SH_CMD_HOLIDAY_HOME                 41      // 41 �����ڼ�����
#define SH_CMD_ACCEPT_REMOTE_UPGRADE_REQ    42      // 42 ����Զ����������
#define SH_CMD_INTO_TEST_FAC                43      // 43 �����̼�

#define SH_MAX_CMD                          200     //���������



/***********************************************************************************
                                ������Ϣ
***********************************************************************************/
#define SET_TMP_SET             0       // 0 Ŀ���¶�(1λС��)
#define SET_MODE_SET            1       // 1 �趨ģʽ
#define SET_LIMIT_TIME          2       // 2 ʹ����������(0��ʾ��ʹ��)
#define SET_TIMING_USE          3       // 3 ��ʱʹ��(TRUE:ʹ��,FALSE:����)
#define SET_START_CODE          4       // 4 ��������������빦�ܵ�ʹ��
#define SET_HUMI_MODE           5       // 5 �趨ʪ��ģʽ
#define SET_LANGUAGE            6       // 6 �趨����
#define SET_TMP_SET_ZONE_1      7       // 7 ����һĿ���¶�
#define SET_TMP_SET_ZONE_2      8       // 8 �����Ŀ���¶�
#define SET_TMP_SET_ZONE_HOTW   9       // 9 ��ˮĿ���¶�
#define SET_LIMIT_TYPE          10      // 10����ά������
#define SET_SILENT_LEVEL        11      // 11���þ�������
#define SET_TEMP_UNIT           12      // 12�¶ȵ�λ

#define SH_MAX_SET_INFO         200     // �����ʾ��Ϣ����



/***********************************************************************************
                                ������Ϣ
***********************************************************************************/
#define MAX_CHK_TEST_DO_NUM     48
#define MAX_CHK_TEST_EEV_NUM    8
#define CHK_TEST_DO_WORD        _DIVUCARRY(MAX_CHK_TEST_DO_NUM, 16) /* ���������DO_NUM���� */
#define CHK_TEST_EEV_WORD       _DIVUCARRY(MAX_CHK_TEST_EEV_NUM, 16)/* ���������EEV_NUM���� */

typedef struct tagSH_CHK_TEST
{
    struct
    {
        U16 str_DO[MAX_CHK_TEST_DO_NUM];        // DO�ַ���
        U16 str_EEV[MAX_CHK_TEST_EEV_NUM];      // EEV�ַ���
        U16 DO_num;                             // DO����
        U16 EEV_num;                            // EEV����
        U16 EEV_step_min[MAX_CHK_TEST_EEV_NUM]; // EEV�ֶ�������Сֵ
        U16 EEV_step_max[MAX_CHK_TEST_EEV_NUM]; // EEV�ֶ��������ֵ
        U16 EEV_step_pv[MAX_CHK_TEST_EEV_NUM];  // EEV��ǰ����
        U16 fg_DO_can_mnl_mode;                 // DO�Ƿ������ֶ�ģʽ
        U16 fg_EEV_can_mnl_mode;                // EEV�Ƿ������ֶ�ģʽ
    }ro;

    struct
    {
        U16 EEV_mnl_mode[CHK_TEST_EEV_WORD];    // EEV�ֶ�ģʽ
        U16 EEV_step_sv[MAX_CHK_TEST_EEV_NUM];  // EEV�ֶ�����
        U16 DO_mnl_mode;                        // DO�ֶ�ģʽ
    }wo;

    struct
    {
        U16 DO_sta[CHK_TEST_DO_WORD];           // DO��ֵ ������ģʽΪ�Զ�ģʽʱ��Ϊ���д��������ģʽΪ�ֶ�ʱ��Ϊǰ��д
    }wr;
}SH_CHK_TEST;

/***********************************************************************************
                                Ϳѻ����
***********************************************************************************/
#ifndef TUYA_F_H                        /* �˲���Ĭ��ǰ���Ѷ��� */
#define TUYA_F_H
/* ��Ʒ��Ϣ���� */
typedef struct tag_TUYA_INFO
{
    U08 dpid_num;           /* ��Ҫ�ϱ��Ĺ��ܵ���� */
    U08 product_id[16];     /* ��ƷID */
    U08 bak;                /* ���ã�ʹ�ṹ���ֽ���Ϊż�� */
}TUYA_INFO;
#define TUYA_INFO_SIZE                  (sizeof(TUYA_INFO)+1) / sizeof(U16)

/* ���ܵ����� */
typedef struct tag_TUYA_DATA
{
    U08 dpid;                           /* DPID */
    U08 dp_type;                        /* ���ܵ����� */
    U08 data_buf[244];                  /* �������� */
}TUYA_DATA;
#define TUYA_DATA_SIZE                  (sizeof(TUYA_DATA)+1) / sizeof(U16)

/* "����ָ��"���ֳ���(�ֽ���) */
#define RAW_DATA_BYTES                  (244)  /* ͸���� */
#define BOOL_DATA_BYTES                 (1)    /* ������ */
#define NUMBER_DATA_BYTES               (4)    /* ��ֵ�� */
#define STRING_DATA_BYTES               (244)  /* �ַ��� */
#define ENUM_DATA_BYTES                 (1)    /* ö���� */
#define FAULT_DATA_BYTES                (1)    /* ������ */
#endif
#define SH_MAX_TUYA_DATA_NUM    40

/***********************************************************************************
                                ��Ƶ��Ϣ
***********************************************************************************/
#define SH_MAX_INVT_INFO    (2+20)  // ��Ƶ��Ϣ������
                                    // 1 ��8λģ��š���8λѹ���� 0xffff��Ч
                                    // 1 ��8λ�Ƿ���ڱ�Ƶ��ͨѶ���ϡ���8λ�Ƿ��Ƶ
                                    // 20 ��Ƶ��Ϣ
typedef struct tagSH_INVTINFO
{
    U16 pcb_used;           /* ģ���Ƿ�ʹ�� */
    U16 pcb_commu_error;    /* ģ���Ƿ�ͨѶ���� */
    I16 invt_info[SH_MAX_INVT_INFO];        /* ��Ƶ��Ϣ */
}SH_INVTINFO;

/***********************************************************************************
                                ����ģʽ
***********************************************************************************/
typedef struct tagSH_HOLIDAYINFO
{
    U16 mode_heat_use;
    U16 mode_hotw_use;
    U16 bak[8];
}SH_HOLIDAYINFO;

/***********************************************************************************
                               ������Ϣ
***********************************************************************************/
#define SH_FCU_MAX          (32)    // �����̸���

typedef struct tagSH_FCUINFO
{
    I16 temp_indoor;
    U16 str_mode;
    U16 str_status;
}SH_FCUINFO;

typedef struct tagSH_FCU
{
    U16 use_num;                    // ʹ�ø���
    SH_FCUINFO fcuinfo[SH_FCU_MAX];
}SH_FCU;


/***********************************************************************************
                               ģʽ��Ϣ
***********************************************************************************/
typedef struct
{
    U16 mode_str;
    U16 mode_key;
    U16 mode_cmd_on;
    U16 mode_cmd_off;
    I16 temp_set_min;
    I16 temp_set_max;
    U16 temp_set_cmd;
}SH_MODE_INFO;
#define SH_MODE_INFO_MAX    10
typedef struct
{
    U16 mode_num;
    SH_MODE_INFO mode_info[SH_MODE_INFO_MAX];
}SH_MODE_SET_INFO;

/***********************************************************************************
                               ��������Ϣ
***********************************************************************************/
enum
{
    ZONE_MODE_FCU,
    ZONE_MODE_RAD,
    ZONE_MODE_FLOOR,
    ZONE_MODE_HOTW,
};
enum
{
    ZONE_TEMP_WATER_FLOW,
    ZONE_TEMP_ROOM,
};
enum
{
    ZONE_STA_NULL,
    ZONE_STA_ANTIFREZ,
    ZONE_STA_DEFRO,
    ZONE_STA_HOLIDAY,
    ZONE_STA_SILENT,
    ZONE_STA_ECO,
    ZONE_STA_TEST,
    ZONE_STA_RECYCLE,
};
enum
{
    ZONE_ADDITION_NULL,
    ZONE_ADDITION_HEAT,
    ZONE_ADDITION_ELEC,
    ZONE_ADDITION_SOLAR,
    ZONE_ADDITION_TANK_HEATER,
};
enum
{
    SMART_GRID_NULL,
    SMART_GRID_FREE,
    SMART_GRID_VALLEY,
    SMART_GRID_PEAK,
};
typedef struct
{
    U16 zone_1_mode;
    U16 fg_zone_1_is_run;
    U16 zone_1_temp_type;
    U16 fg_zone_1_can_set_temp;
    U16 fg_zone_1_comp_on;
    U16 zone_1_status;
    U16 zone_1_addition;

    U16 zone_2_mode;
    U16 fg_zone_2_is_run;
    U16 zone_2_temp_type;
    U16 fg_zone_2_can_set_temp;
    U16 fg_zone_2_comp_on;
    U16 zone_2_status;
    U16 zone_2_addition;

    U16 fg_zone_hotw_is_run;
    U16 fg_zone_hotw_disinfect;
    U16 fg_zone_hotw_comp_on;
    U16 zone_hotw_status;
    U16 zone_hotw_addition;

    U16 fg_pumpi_is_on;
    U16 smart_grid_sta;
}SH_MAIN_INFO;

/***********************************************************************************
                               ��Ƶ������Ϣ
***********************************************************************************/
typedef struct
{
    I16 addr;
    I16 val;
    U16 str_tips;
}SH_INVT_PARA;

#define SH_MAX_INVT_ERR         50

#define SH_MAX_FLOOR_PREHEAT_INFO   3       /* ��ůԤ�Ȳ������� */

#define SH_MAX_FLOOR_TIMER_INFO     4       /* ��ʱ��ů��ɲ������� */


/***********************************************************************************
                                4G��Ϣ
***********************************************************************************/
#define SH_MAX_4G_VER_WORD  16  // !!!�ö�����Ҫ����ư�һ��
#define SH_MAX_4G_SN_WORD   16
typedef struct
{
    struct
    {
        U16 name;
        U16 value;
    }csq;                   /* �ź�ǿ�� */
    struct
    {
        U16 name;
        U16 value;
        U16 value_string;
    }sta;                   /* ����״̬ */
    struct
    {
        U16 name;
        U16 info[SH_MAX_4G_VER_WORD];
    }ver;                   /* �汾 */
    struct
    {
        U16 name;
        U16 info[SH_MAX_4G_SN_WORD];
    }sn;                    /* sn */
}SH_4G_INFO;

#define SH_4G_STA_NONE          0   // ��
#define SH_4G_STA_NO_MODULE     1   // ��ģ��
#define SH_4G_STA_CANCAT        2   // ������ģ��
#define SH_4G_STA_CONNECT_NET   3   // ����������
#define SH_4G_STA_CONNECT_CLOUD 4   // ��������
#define SH_4G_STA_ALARM         5   // ����

typedef enum
{
    SH_REMOTE_REQ_WAIT,
    SH_REMOTE_REQ_OK,
    SH_REMOTE_REQ_CANCEL,
    SH_REMOTE_REQ_NULL,
}SH_REMOTE_REQ;

typedef struct
{
    struct
    {
        U16 fg_TF_is_on;
        U16 wifi_ssid[32];
        U16 wifi_password[32];
    }rd;
    struct
    {
        U16 wifi_sta;
    }wr;
}SH_TEST_FAC_HMI_STRU;
enum
{
    TEST_FAC_WIFI_STA_TESTING,
    TEST_FAC_WIFI_STA_NO_MODULE,
    TEST_FAC_WIFI_STA_SUCC,
    TEST_FAC_WIFI_STA_FAIL,
};

/***********************************************************************************
                                ͨѶ��ַ
***********************************************************************************/
//����SIZEʱ���Ӽ�1��Ϊ����stm32��stm8ƽ̨�ϱ���һ�¡�

/***********************************************************************************
                                �ɶ�д�� ͨѶ��ַ
***********************************************************************************/
//����
#define ADDR_SH_CMD_BGN             0
#define SH_CMD_SIZE                 (1)
#define ADDR_SH_CMD_END             (ADDR_SH_CMD_BGN + SH_CMD_SIZE*SH_MAX_CMD)        // 200

//����
#define ADDR_SH_SET_BGN             200
#define SH_SET_SIZE                 (1)
#define ADDR_SH_SET_END             (ADDR_SH_SET_BGN + SH_SET_SIZE*SH_MAX_SET_INFO)   // 400

//����
#define ADDR_SH_PASS_BGN            400
#define SH_PASS_SIZE                ((sizeof(SH_PASS)+1) / sizeof(U16))// 5
#define ADDR_SH_PASS_END            (ADDR_SH_PASS_BGN + SH_PASS_SIZE*SH_MAX_PASS_GRP) // 480

//ʱ��
#define ADDR_SH_CLOCK_BGN           500
#define SH_CLOCK_SIZE               ((sizeof(SH_CLOCK)+1) / sizeof(U16))// 7
#define ADDR_SH_CLOCK_END           (ADDR_SH_CLOCK_BGN + SH_CLOCK_SIZE)               // 507

//��ʱ
#define ADDR_SH_TIMING_BGN          550
#define SH_TIMING_SIZE              ((sizeof(SH_TIMING)+1) / sizeof(U16))// 3
#define ADDR_SH_TIMING_END          (ADDR_SH_TIMING_BGN + SH_TIMING_SIZE*SH_MAX_TIMING_GRP)   // 586

//����ģʽ
#define ADDR_SH_HOLIDAYINFO_BGN     590
#define SH_HOLIDAYINFO_SIZE         ((sizeof(SH_HOLIDAYINFO)+1) / sizeof(U16))  // 10
#define ADDR_SH_HOLIDAYINFO_END     (ADDR_SH_HOLIDAYINFO_BGN + SH_HOLIDAYINFO_SIZE)   // 600

//�����¶�
#define ADDR_ROOM_TEMP_BGN          603
#define SH_ROOM_TEMP_SIZE           (1) // 1
#define ADDR_SH_ROOM_TEMP_END       (ADDR_ROOM_TEMP_BGN + SH_ROOM_TEMP_SIZE)   // 604

//��Ƶ����
#define ADDR_SH_INVTPARA_BGN        610
#define SH_INVTPARA_SIZE            ((sizeof(SH_INVT_PARA)+1) / sizeof(U16))    // 3
#define ADDR_SH_INVTPARA_END        (ADDR_SH_INVTPARA_BGN + SH_INVTPARA_SIZE)   // 613

/* ��ůԤ����Ϣ */
#define ADDR_SH_FLOOR_PREHEAT_VAL_BGN   620
#define SH_FLOOR_PREHEAT_VAL_SIZE       (1) /* 1 */
#define ADDR_SH_FLOOR_PREHEAT_VAL_END   (ADDR_SH_FLOOR_PREHEAT_VAL_BGN + SH_FLOOR_PREHEAT_VAL_SIZE * SH_MAX_FLOOR_PREHEAT_INFO) /* 622 */

/* ��ʱ��ů�����Ϣ */
#define ADDR_SH_FLOOR_TIMER_VAL_BGN     625
#define SH_FLOOR_TIMER_VAL_SIZE         (1) /* 1 */
#define ADDR_SH_FLOOR_TIMER_VAL_END     (ADDR_SH_FLOOR_TIMER_VAL_BGN + SH_FLOOR_TIMER_VAL_SIZE * SH_MAX_FLOOR_TIMER_INFO)   /* 628 */

//����ֵ
#define ADDR_SH_PARA_BGN            2000
#define SH_PARA_SIZE                (1)
#define ADDR_SH_PARA_END            (ADDR_SH_PARA_BGN + SH_PARA_SIZE*SH_MAX_PARA_ITEM_TOTAL)   // 3280

//����ģʽ����
#define ADDR_SH_CHK_TEST_BGN        3300


#define ADDR_SH_CHK_TEST_WO_BGN     (ADDR_SH_CHK_TEST_RO_BGN+SH_CHK_TEST_RO_SIZE)               // 3384
#define SH_CHK_TEST_WO_SIZE         ((sizeof(((SH_CHK_TEST *)NULL)->wo)+1) / sizeof(U16))
#define ADDR_SH_CHK_TEST_WO_END     (ADDR_SH_CHK_TEST_WO_BGN+SH_CHK_TEST_WO_SIZE)               // 3394

#define ADDR_SH_CHK_TEST_WR_BGN     (ADDR_SH_CHK_TEST_WO_BGN+SH_CHK_TEST_WO_SIZE)               // 3394
#define SH_CHK_TEST_WR_SIZE         ((sizeof(((SH_CHK_TEST *)NULL)->wr)+1) / sizeof(U16))
#define ADDR_SH_CHK_TEST_WR_END     (ADDR_SH_CHK_TEST_WR_BGN+SH_CHK_TEST_WR_SIZE)               // 3397

#define ADDR_SH_PASS_BIG_ENDIAN_BGN (3400)
#define ADDR_SH_PASS_BIG_ENDIAN_END (3401)

/* wifi�̼�� */
#define ADDR_SH_TEST_FAC_HMI_RD_BGN (3500)
#define SH_TEST_FAC_HMI_RD_SIZE     ((sizeof(((SH_TEST_FAC_HMI_STRU*)NULL)->rd)+1) / sizeof(U16))   /* 65 */
#define ADDR_SH_TEST_FAC_HMI_RD_END (ADDR_SH_TEST_FAC_HMI_RD_BGN + SH_TEST_FAC_HMI_RD_SIZE)

/* wifi�̼�д */
#define ADDR_SH_TEST_FAC_HMI_WR_BGN (ADDR_SH_TEST_FAC_HMI_RD_END)
#define SH_TEST_FAC_HMI_WR_SIZE     ((sizeof(((SH_TEST_FAC_HMI_STRU*)NULL)->wr)+1) / sizeof(U16))   /* 1 */
#define ADDR_SH_TEST_FAC_HMI_WR_END (ADDR_SH_TEST_FAC_HMI_WR_BGN + SH_TEST_FAC_HMI_WR_SIZE)

/* Ϳѻ��Ϣ */
#define ADDR_SH_TUYA_INFO_BGN       (3900)                                                      /* 3900 */
#define SH_TUYA_INFO_SIZE           ((sizeof(TUYA_INFO)+1) / sizeof(U16))                       /* 28 */
#define ADDR_SH_TUYA_INFO_END       (ADDR_SH_TUYA_INFO_BGN+SH_TUYA_INFO_SIZE)                   /* 3928 */

/*3��ʹ������*/
#define ADDR_SH_LIMIT_BGN           (4000)                                                      /* 4000 */
#define SH_LIMIT_SIZE               ((sizeof(SH_LIMIT)+1) / sizeof(U16))                       
#define ADDR_SH_LIMIT_END           (ADDR_SH_LIMIT_BGN+SH_LIMIT_SIZE)  
/* Ϳѻ���� */
#define ADDR_SH_TUYA_DATA_BGN       (8920)      /* 8920 */
#define SH_TUYA_DATA_SIZE           ((sizeof(TUYA_DATA)+1) / sizeof(U16))   /* 123 */
#define ADDR_SH_TUYA_DATA_END       (ADDR_SH_TUYA_DATA_BGN+SH_TUYA_DATA_SIZE)   /* 9043 */

//��������
#define ADDR_SH_MAC_VER_WR_BGN      (9050)
#define SH_MAC_VER_WR_SIZE          (16)
#define ADDR_SH_MAC_VER_WR_END      (ADDR_SH_MAC_VER_WR_BGN + SH_MAC_VER_WR_SIZE)

/***********************************************************************************
                                ֻ���� ͨѶ��ַ
***********************************************************************************/

#define SH_ADDR_STRING_START_CODE_BGN   (1700)
#define STRING_START_CODE_SIZE          (128 / sizeof(U16))
#define SH_ADDR_STRING_START_CODE_END   (SH_ADDR_STRING_START_CODE_BGN + STRING_START_CODE_SIZE * 2)/* ��Ӣ�� */

//ʹ�ý�����Ϣ
#define SH_ADDR_ENABLE_INFO_BGN     (1960)
#define ENABLE_INFO_SIZE            ((sizeof(SH_ENABLE_INFO)+1) / sizeof(U16))
#define SH_ADDR_ENABLE_INFO_END     (SH_ADDR_ENABLE_INFO_BGN + ENABLE_INFO_SIZE)

//��˪��Ϣ
#define SH_ADDR_DEFR_INFO_BGN       1968
#define DEFR_INFO_SIZE              ((sizeof(SH_DEFRO_INFO)+1) / sizeof(U16))   // 2
#define SH_ADDR_DEFR_INFO_END       (SH_ADDR_DEFR_INFO_BGN + DEFR_INFO_SIZE*SH_MAX_PCB_NUM)     // 2000

//��ʾ������Ϣ
#define ADDR_SH_DISP_BGN            2000
#define SH_DISP_SIZE                (1)
#define ADDR_SH_DISP_END            (ADDR_SH_DISP_BGN + SH_DISP_SIZE*SH_MAX_DISP_INFO)                // 2100

//��ѯ��Ϣ����
#define ADDR_SH_CHK_NUM_BGN         2200
#define SH_CHK_NUM_SIZE             ((sizeof(SH_CHK_INFO_NUM)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_NUM_END         (ADDR_SH_CHK_NUM_BGN+SH_CHK_NUM_SIZE*SH_MAX_PCB_NUM)        //2264

// ������Ϣֻ����
#define ADDR_SH_CHK_TEST_RO_BGN     (ADDR_SH_CHK_TEST_BGN)                                      // 3300
#define SH_CHK_TEST_RO_SIZE         ((sizeof(((SH_CHK_TEST *)NULL)->ro)+1) / sizeof(U16))
#define ADDR_SH_CHK_TEST_RO_END     (ADDR_SH_CHK_TEST_BGN + SH_CHK_TEST_RO_SIZE)                // 3384

//�汾��
#define ADDR_SH_VER_BGN             3400
#define SH_VER_SIZE                 ((sizeof(SH_CHK_VER)+1) / sizeof(U16))  // 18
#define ADDR_SH_VER_END             (ADDR_SH_VER_BGN + SH_VER_SIZE*SH_MAX_PCB_NUM)                    // 3688

//���� ��ǰ
#define ADDR_SH_ERR_CRT_BGN         3700
#define SH_ERR_CRT_SIZE             ((sizeof(SH_ERR_CURT)+1) / sizeof(U16)) // 3
#define ADDR_SH_ERR_CRT_END         (ADDR_SH_ERR_CRT_BGN + SH_ERR_CRT_SIZE*SH_MAX_ERR_CURT_NUM)       // 4000

//���� ��ʷ
#define ADDR_SH_ERR_HIS_BGN         4000
#define SH_ERR_HIS_SIZE             ((sizeof(SH_ERR_HIS)+1) / sizeof(U16))  // 7
#define ADDR_SH_ERR_HIS_END         (ADDR_SH_ERR_HIS_BGN + SH_ERR_HIS_SIZE*SH_MAX_ERR_HIS_NUM)        // 4700

// ������Ϣ
#define ADDR_SH_FCUINFO_BGN         4700
#define SH_FCUINFO_SIZE             ((sizeof(SH_FCU)+1) / sizeof(U16))  // 97
#define ADDR_SH_FCUINFO_END         (ADDR_SH_FCUINFO_BGN + SH_FCUINFO_SIZE)        // 4797
/* ���̼��汾�� */
#define ADDR_SH_FIRMWARE_BGN        4800
#define SH_FIRMWARE_SIZE            3
#define ADDR_SH_FIRMWARE_END        (ADDR_SH_FIRMWARE_BGN+SH_FIRMWARE_SIZE)

//����������
#define ADDR_SH_PARA_ATTRIB_BGN     5000
#define SH_PARA_ATTRIB_SIZE         ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))// 6
#define ADDR_SH_PARA_ATTRIB_END     (ADDR_SH_PARA_ATTRIB_BGN + SH_PARA_ATTRIB_SIZE*SH_MAX_PARA_ITEM_TOTAL)  // 12680

//ѹ������
#define ADDR_SH_PCB_COMP_NUM_BGN    13000
#define SH_PCB_COMP_NUM_SIZE        (1)  // 16
#define ADDR_SH_PCB_COMP_NUM_END    (ADDR_SH_PCB_COMP_NUM_BGN + SH_PCB_COMP_NUM_SIZE*SH_MAX_PCB_NUM)   // 13016

//BMSͨѶ����
#define ADDR_SH_BMS_PARA_BGN        13100
#define SH_BMS_PARA_SIZE            ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))// 6
#define ADDR_SH_BMS_PARA_END        (ADDR_SH_BMS_PARA_BGN + SH_BMS_PARA_SIZE*SH_MAX_BMS_PARA_NUM)       // 13130

/* ģʽ������Ϣ */
#define ADDR_SH_MODE_SET_INFO_BGN   13200
#define SH_MODE_SET_INFO_SIZE       ((sizeof(SH_MODE_SET_INFO)+1) / sizeof(U16))/* 71 */
#define ADDR_SH_MODE_SET_INFO_END   (ADDR_SH_MODE_SET_INFO_BGN + SH_MODE_SET_INFO_SIZE) /* 13271 */

/* ��������Ϣ */
#define ADDR_SH_MAIN_INFO_BGN       13300
#define SH_MAIN_INFO_SIZE           ((sizeof(SH_MAIN_INFO)+1) / sizeof(U16))/* 11 */
#define ADDR_SH_MAIN_INFO_END       (ADDR_SH_MAIN_INFO_BGN + SH_MAIN_INFO_SIZE) /* 13311 */

/* ��Ƶ������Ϣ */
#define ADDR_SH_INVTERR_BGN         13400
#define SH_INVTERR_SIZE             ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_INVTERR_END         (ADDR_SH_INVTERR_BGN + SH_INVTERR_SIZE * SH_MAX_INVT_ERR)   /* 13600 */

/* ��ůԤ����Ϣ */
#define ADDR_SH_FLOOR_PREHEAT_BGN   13600
#define SH_FLOOR_PREHEAT_SIZE       ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))  /* 6 */
#define ADDR_SH_FLOOR_PREHEAT_END   (ADDR_SH_FLOOR_PREHEAT_BGN + SH_FLOOR_PREHEAT_SIZE * SH_MAX_FLOOR_PREHEAT_INFO) /* 13617 */

/* ��ʱ��ů�����Ϣ */
#define ADDR_SH_FLOOR_TIMER_BGN     13630
#define SH_FLOOR_TIMER_SIZE         ((sizeof(SH_PARA_ATTRIB)+1) / sizeof(U16))  /* 6 */
#define ADDR_SH_FLOOR_TIMER_END     (ADDR_SH_FLOOR_TIMER_BGN + SH_FLOOR_TIMER_SIZE * SH_MAX_FLOOR_TIMER_INFO)   /* 13653 */

//��������Ϣ
#define ADDR_SH_PARA_GRP_BGN        17000
#define SH_PARA_GRP_SIZE            ((sizeof(SH_PARA_GRP)+1) / sizeof(U16))// 3
#define ADDR_SH_PARA_GRP_END        (ADDR_SH_PARA_GRP_BGN + SH_PARA_GRP_SIZE*SH_MAX_PARA_GRP_NUM)             // 17120

//���������ַ���
#define ADDR_SH_PARA_STRING_BGN     17200
#define SH_PARA_STRING_SIZE         (1)// 1
#define ADDR_SH_PARA_STRING_END     (ADDR_SH_PARA_STRING_BGN + SH_MAX_DICT_STR)    // 17600

//�����ַ�������
#define ADDR_SH_STR_ATTR_BGN        17600
#define SH_STR_ATTR_PER_SIZE        1
#define ADDR_SH_STR_ATTR_END        (ADDR_SH_STR_ATTR_BGN + SH_STR_ATTR_PER_SIZE*SH_MAX_PARA_STRING_GRP)   //17700

//��Ƶ��Ϣ
#define ADDR_SH_INVT_INFO_BGN       17900
#define SH_INVT_INFO_SIZE           ((sizeof(SH_INVTINFO)+1) / sizeof(U16)) // 24
#define ADDR_SH_INVT_INFO_END       (ADDR_SH_INVT_INFO_BGN + SH_INVT_INFO_SIZE) // 17924


//��ѯ��Ϣ ϵͳģ���� Ԥ��100��ϵͳģ����
#define ADDR_SH_CHK_SYS_AV_BGN      18000
#define SH_CHK_SYS_AV_SIZE          ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_SYS_AV_END      (ADDR_SH_CHK_SYS_AV_BGN + SH_CHK_SYS_AV_SIZE*100)                        // 18400

//��ѯ��Ϣ ģ�鵥Ԫģ���� ÿ��ģ��Ԥ��50����50*16=800�� ÿ��ģ�鵥Ԫģ������ַƫ�ƹ̶�Ϊ50*4����
#define ADDR_SH_CHK_MOD_AV_BGN      18400
#define SH_CHK_MOD_AV_SIZE          ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define SH_CHK_MOD_AV_STEP          (50)
#define ADDR_SH_CHK_MOD_AV_END      (ADDR_SH_CHK_MOD_AV_BGN + SH_CHK_MOD_AV_SIZE*800)   /* 21600 */

//��ѯ��Ϣ ѹ��ģ����
#define ADDR_SH_CHK_COMP_AV_BGN     22000
#define SH_CHK_COMP_AV_SIZE         ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_COMP_AV_END     (ADDR_SH_CHK_COMP_AV_BGN + SH_CHK_COMP_AV_SIZE*2500)                        // 32000

//��ѯ��Ϣ ���������
#define ADDR_SH_CHK_DO_BGN          32000
#define SH_CHK_DO_SIZE              ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_DO_END          (ADDR_SH_CHK_DO_BGN + SH_CHK_DO_SIZE*2500)                          // 42000

//��ѯ��Ϣ ����������
#define ADDR_SH_CHK_DI_BGN          42000
#define SH_CHK_DI_SIZE              ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_DI_END          (ADDR_SH_CHK_DI_BGN + SH_CHK_DI_SIZE*2500)                          // 52000

/* �û���ѯ */
#define ADDR_SH_CHK_USER_BGN        52000
#define SH_CHK_USER_SIZE            ((sizeof(SH_CHK_INFO)+1) / sizeof(U16)) // 4
#define ADDR_SH_CHK_USER_END        (ADDR_SH_CHK_USER_BGN + SH_CHK_USER_SIZE * SH_MAX_CHK_USER_INFO)//52020

/* ��Ƶ���汾�� */
#define ADDR_SH_VFD_VER_BGN         52100
#define SH_VFD_VER_SIZE             ((sizeof(SH_CHK_VER)+1) / sizeof(U16))// 18
#define ADDR_SH_VFD_VER_END         (ADDR_SH_VFD_VER_BGN + SH_VFD_VER_SIZE*SH_MAX_PCB_NUM)    // 52388

/* �����汾�� */
#define ADDR_SH_EXIO_VER_BGN        52400
#define SH_EXIO_VER_SIZE            ((sizeof(SH_CHK_VER)+1) / sizeof(U16))// 18
#define ADDR_SH_EXIO_VER_END        (ADDR_SH_EXIO_VER_BGN + SH_EXIO_VER_SIZE*SH_MAX_EXIO_NUM*SH_MAX_PCB_NUM)    // 53552

/* 4G��Ϣ */
#define ADDR_SH_4G_INFO_BGN         54000
#define SH_4G_INFO_SIZE             ((sizeof(SH_4G_INFO)+1) / sizeof(U16)) // 38
#define ADDR_SH_4G_INFO_END         (ADDR_SH_4G_INFO_BGN + SH_4G_INFO_SIZE) // 54038

/*�����ͺ�*/
#define ADDR_SH_MAC_VER_BGN         54100
#define SH_MAC_VER_SIZE             ((sizeof(SH_CHK_VER)+1) / sizeof(U16)) // 18
#define ADDR_SH_MAC_VER_END         (ADDR_SH_MAC_VER_BGN + SH_MAC_VER_SIZE*SH_MAX_PCB_NUM) // 54388

//������Ϣ
#define ADDR_SH_CHK_DEBUG_BGN       60000
#define ADDR_SH_CHK_DEBUG_END       65000 // 50000
#define ADDR_SH_SET_DEBUG_BGN       65000
#define ADDR_SH_SET_DEBUG_END       65500 // 50000
#endif



