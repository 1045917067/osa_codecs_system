/** ============================================================================
 *
 *  @File name:	osa_debugger.h
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

#if !defined (__OSA_DEBUGGER_H)
#define __OSA_DEBUGGER_H

/*  --------------------- Include system headers ---------------------------- */

#include <stdio.h>
/*  --------------------- Include user headers   ---------------------------- */

#include "osa_status.h"

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
extern "C" {
#endif  /* defined(__cplusplus) */


/*
 *  --------------------- Macro definition -------------------------------------
 */

#define DBG_DETAILED	1
#define DBG_INFO		2
#define DBG_WARNING		3
#define DBG_ERROR		4
#define DBG_FATAL		5
#define DBG_SILENT		1000

#define DBG(level, tag, arg...) osa_debugger((level), (tag), ##arg);


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

struct __osa_debugger_prm_t;
typedef struct __osa_debugger_prm_t osa_debugger_prm_t;
struct __osa_debugger_prm_t
{
    int             m_debug_level;
    FILE *          m_out;
    unsigned char * m_name;
    unsigned char * m_folder;
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

status_t osa_debugger_init(const osa_debugger_prm_t *prm);

void     osa_debugger(int level, const char *tags, const char *fmt, ...);

status_t osa_debugger_setlevel(int level);

status_t osa_debugger_deinit(void);


/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (OSA_DEBUGGER_H) */
