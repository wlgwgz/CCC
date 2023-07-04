/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : st.h
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2022年3月30日
  最近修改   :
  功能描述   : st.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2022年3月30日
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/



#ifndef __ST_H__
#define __ST_H__



// 用法示例
/*

ST_INIT(st);

STSTA st_example(ST* st)
{
    ST_BEGIN(st);
    do_thing1();
    ST_YIELD(st);
    do_thing2();
    ST_YIELD_REPEAT(st);    // ←--
    do_thing3();            //    ↑  
    ST_EXIT_IF(c);          //    ↑  
    ST_REPEAT(st);          // ---↑
    
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
    unsigned short lc;          // 记录当前执行位置
    unsigned short lc_repeat;   // 记录重复执行位置
}ST;


// 初始化
#define ST_INIT(st)   (st)->lc = (st)->lc_repeat = 0;

// 开始,必须写在开头
#define ST_BEGIN(st)   {STSTA st_sta = ST_WAITING; switch((st)->lc) { case 0:

// 跳出,下次继续
#define ST_YIELD(st)    (st)->lc = __LINE__; st_sta = ST_YIELDED; break; case __LINE__:
// 跳出,并记录重复位置,下次继续
#define ST_YIELD_REPEAT(st)    (st)->lc = (st)->lc_repeat = __LINE__; st_sta = ST_YIELDED;  break; case __LINE__:
// 跳出,当条件不满足时再继续
#define ST_YIELD_IF(st, c) (st)->lc = __LINE__; st_sta = ST_YIELDED;  break; case __LINE__: if (c) {st_sta = ST_YIELDED; break;}

// 当条件满足时等待
#define ST_WAIT_IF(st, c) (st)->lc = __LINE__;  case __LINE__: if (c) break;

// 重复上次记录的位置
#define ST_REPEAT(st)     (st)->lc = (st)->lc_repeat; break;
// 当条件满足时,重复上次记录的位置
#define ST_REPEAT_IF(st, c)    if (c)    {ST_REPEAT(st);}

// 退出
#define ST_EXIT(st)         ST_INIT(st); st_sta = ST_EXITED; break;
// 当条件满足时,退出
#define ST_EXIT_IF(st, c)   if (c)    {ST_EXIT(st);}

// 结束
#define ST_ENDED(st)     ST_INIT(st); st_sta = ST_ENDED; break;   

// 返回,必须写在结尾
#define ST_END(st)      } return st_sta; }


#endif /* __ST_H__ */



