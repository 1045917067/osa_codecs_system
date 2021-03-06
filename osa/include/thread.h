/** ============================================================================
 *
 *  @File name:	thread.h
 *
 *  @Description :
 *
 *  @Function List:  //	主要函数及功能
 *	    1.  －－－－－
 *	    2.  －－－－－
 *
 *  @History:	     //	历史修改记录
 *
 *	<author>	    <time>	     <version>	    <desc>
 *  zzx            2013-06-30     v1.0	        write this module.
 *
 *  ============================================================================
 */

#if !defined (__THREAD_H)
#define __THREAD_H

/*  --------------------- Include system headers ---------------------------- */

/*  --------------------- Include user headers   ---------------------------- */
#include "osa.h"

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
extern "C" {
#endif  /* defined(__cplusplus) */


/*
 *  --------------------- Macro definition -------------------------------------
 */

/** ============================================================================
 *  @Macro:         Macro name
 *
 *  @Description:   Description of this macro.
 *  ============================================================================
 */

/*
 *  --------------------- Data type definition ---------------------------------
 */

/** ----------------------------------------------------------------------------
 *  @Name:          Structure name
 *
 *  @Description:   Description of the structure.
 *
 *  @Field:         Field1 member
 *
 *  @Field:         Field2 member
 *  ----------------------------------------------------------------------------
 */

struct __thread_t; typedef struct __thread_t thread_t, *thread_handle;

struct __thread_attrs_t;
typedef struct __thread_attrs_t thread_attrs_t;

struct __thread_attrs_t {
    int     priority;
    int     stacksize;
    int     stackseg;
    char  * name;
    void  * environ;
};

struct __thread_stat_t;
typedef struct __thread_stat_t thread_stat_t;

struct __thread_stat_t {
    int     stacksize;
    int     stackused;
};

extern thread_attrs_t default_thd_attrs;

/*
 *  --------------------- Public function declaration --------------------------
 */

/** =============================================================================
 *
 *  @Function:	    //	函数名称
 *
 *  @Description:   //	函数功能、性能等的描述
 *
 *  @Calls:	        //	被本函数调用的函数清单
 *
 *  @Called By:	    //	调用本函数的函数清单
 *
 *  @Table Accessed://	被访问的表（此项仅对于牵扯到数据库操作的程序）
 *
 *  @Table Updated: //	被修改的表（此项仅对于牵扯到数据库操作的程序）
 *
 *  @Input:	        //	对输入参数的说明
 *
 *  @Output:	    //	对输出参数的说明
 *
 *  @Return:	    //	函数返回值的说明
 *
 *  @Enter          //  Precondition
 *
 *  @Leave          //  Postcondition
 *
 *  @Others:	    //	其它说明
 *
 *  ============================================================================
 */

thread_handle   thread_create(Fxn fxn, thread_attrs_t * attrs, ...);

void            thread_delete(thread_handle hdl);

//int             thread_get_pri(thread_handle hdl);

void *          thread_get_env(thread_handle hdl);

//char *          thread_get_name(thread_handle hdl);

//int             thread_set_pri(thread_handle hdl, int newpri);

//void *          thread_set_env(thread_handle hdl, void *env);

//char *          thread_set_name(thread_handle hdl, char *name);

thread_handle   thread_self(void);

int             thread_cancel(thread_handle hdl);

int             thread_join(thread_handle hdl);

//int             thread_stat(thread_handle hdl);

void            thread_yield(void);


/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__THREAD_H) */
