/** ============================================================================
 *
 *  @File name:	codecs_main.h
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
 *  zzx            2015-07-29     v1.0	        write this module.
 *
 *  ============================================================================
 */

#if !defined (__CODECS_MAIN_H)
#define __CODECS_MAIN_H

/*  --------------------- Include system headers ---------------------------- */

/*  --------------------- Include user headers   ---------------------------- */

#include "codecs_system.h"
#include "osa_task_mgr.h"

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

struct __codecs_opts_t;
typedef struct __codecs_opts_t codecs_opts_t;
struct __codecs_opts_t
{
    unsigned int            m_log_level;
    unsigned int            m_work_mode;
};

struct __codecs_object_t;
typedef struct __codecs_object_t codecs_object_t;
struct __codecs_object_t
{
    codecs_opts_t           m_codecs_opts;

    codecs_system_prm_t     m_codecs_ed_prm;

    unsigned char           m_codecs_tsk_name[32];

    task_object_t           m_codecs_tsk_obj;

    unsigned int            m_codecs_need_reboot;

    unsigned int            m_codecs_initialized;
};


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

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__CODECS_MAIN_H) */
