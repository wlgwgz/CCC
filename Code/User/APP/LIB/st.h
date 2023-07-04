/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : st.h
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2022��3��30��
  ����޸�   :
  ��������   : st.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2022��3��30��
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/



#ifndef __ST_H__
#define __ST_H__



// �÷�ʾ��
/*

ST_INIT(st);

STSTA st_example(ST* st)
{
    ST_BEGIN(st);
    do_thing1();
    ST_YIELD(st);
    do_thing2();
    ST_YIELD_REPEAT(st);    // ��--
    do_thing3();            //    ��  
    ST_EXIT_IF(c);          //    ��  
    ST_REPEAT(st);          // ---��
    
    do_thing4();
    
    ST_END(st);

}

*/

typedef enum {
    ST_WAITING=0,
    ST_YIELDED,
    ST_EXITED ,
    ST_ENDED  ,
} STSTA;

typedef struct {
    unsigned short lc;          // ��¼��ǰִ��λ��
    unsigned short lc_repeat;   // ��¼�ظ�ִ��λ��
}ST;


// ��ʼ��
#define ST_INIT(st)   (st)->lc = (st)->lc_repeat = 0;

// ��ʼ,����д�ڿ�ͷ
#define ST_BEGIN(st)   {STSTA st_sta = ST_WAITING; switch((st)->lc) { case 0:

// ����,�´μ���
#define ST_YIELD(st)    (st)->lc = __LINE__; st_sta = ST_YIELDED; break; case __LINE__:
// ����,����¼�ظ�λ��,�´μ���
#define ST_YIELD_REPEAT(st)    (st)->lc = (st)->lc_repeat = __LINE__; st_sta = ST_YIELDED;  break; case __LINE__:
// ����,������������ʱ�ټ���
#define ST_YIELD_IF(st, c) (st)->lc = __LINE__; st_sta = ST_YIELDED;  break; case __LINE__: if (c) {st_sta = ST_YIELDED; break;}

// ����������ʱ�ȴ�
#define ST_WAIT_IF(st, c) (st)->lc = __LINE__;  case __LINE__: if (c) break;

// �ظ��ϴμ�¼��λ��
#define ST_REPEAT(st)     (st)->lc = (st)->lc_repeat; break;
// ����������ʱ,�ظ��ϴμ�¼��λ��
#define ST_REPEAT_IF(st, c)    if (c)    {ST_REPEAT(st);}

// �˳�
#define ST_EXIT(st)         ST_INIT(st); st_sta = ST_EXITED; break;
// ����������ʱ,�˳�
#define ST_EXIT_IF(st, c)   if (c)    {ST_EXIT(st);}

// ����
#define ST_ENDED(st)     ST_INIT(st); st_sta = ST_ENDED; break;   

// ����,����д�ڽ�β
#define ST_END(st)      } return st_sta; }


#endif /* __ST_H__ */



