/** ============================================================================
 *
 *  @File name:	debug.h
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

#if !defined (__DEBUG_H)
#define __DEBUG_H

/*  --------------------- Include system headers ---------------------------- */

#include <stdio.h>
#include <pthread.h>
#include <time.h>

/*  --------------------- Include user headers   ---------------------------- */


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

#define DBG_DETAILED	1
#define DBG_INFO		2
#define DBG_WARNING		3
#define DBG_ERROR		4
#define DBG_FATAL		5
#define DBG_SILENT		1000

/* Error message */

#define DBG(level, fmt, arg...) if((level) >= g_debug_level && gbl_debug_logger) { \
	time_t tmt; \
	struct tm tm;\
	time(&tmt); \
	localtime_r(&tmt, &tm);\
	pthread_mutex_lock(&gbl_debug_mutex); \
	fprintf(gbl_debug_logger, "[%s %d @ %s %02d, %02d:%02d:%02d] " fmt, __FILE__, __LINE__,  \
			gbl_months[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, \
		##arg) ; \
	fflush(gbl_debug_logger); \
	if(gbl_debug_logger2) { \
		fprintf(gbl_debug_logger2, "[%s %d @ %s %02d, %02d:%02d:%02d] " fmt, __FILE__, __LINE__,  \
				gbl_months[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, \
				##arg) ; \
		fflush(gbl_debug_logger2); \
	} \
	debugger_update(&tm); \
	pthread_mutex_unlock(&gbl_debug_mutex); }

#define ERR(fmt, arg...) DBG(DBG_ERROR, "ERROR: " fmt, ##arg)


/* Function error codes */
#define SUCCESS             0
#define FAILURE             -1

/* Thread error codes */
#define THREAD_SUCCESS      (Void *) 0
#define THREAD_FAILURE      (Void *) -1

#ifdef  TRUE
#undef  TRUE
#endif
#define TRUE    (1)

#ifdef  FALSE
#undef  FALSE
#endif
#define FALSE   (0)

#define SUCCEEDED(r)		((r) >= 0)
#define FAILED(r)			((r) < 0)

// For back compatience

#define DBG1				DBG
#define ERR1				ERR

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

extern int g_debug_level;
/* Global debug mode flag */
extern FILE* gbl_debug_logger;
extern FILE* gbl_debug_logger2;

extern const char* const gbl_months[12];

extern pthread_mutex_t gbl_debug_mutex;

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

void debugger_init(FILE* fp, const char* logger_folder);
void debugger_destroy(void);
void debugger_update(struct tm* tm);

void debugger_setlevel(int level);

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__DEBUG_H) */
