/** ============================================================================
 *
 *  @File name:	debug.c
 *
 *  @Description:   The description of this file.
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

/*  --------------------- Include system headers ---------------------------- */

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*  --------------------- Include user headers   ---------------------------- */

#include "debug.h"

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
 *  --------------------- Structure definition ---------------------------------
 */

/** ----------------------------------------------------------------------------
 *  @Name:          Structure name
 *
 *  @Description:   Description of the structure.
 *
 *  @Field:         Field1 member
 *
 *  @Field          Field2 member
 *  ----------------------------------------------------------------------------
 */

/*
 *  --------------------- Global variable definition ---------------------------
 */

/** ----------------------------------------------------------------------------
 *  @Name:          Variable name
 *
 *  @Description:   Description of the variable.
 * -----------------------------------------------------------------------------
 */

/* Global debugger loggers */
FILE* gbl_debug_logger;
FILE* gbl_debug_logger2 = NULL;

pthread_mutex_t gbl_debug_mutex;

static const char* dbg_logger_folder;
static struct tm old_tm;

static int  cur_init = 0;

int g_debug_level;

const char* const gbl_months[12] =
{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/*
 *  --------------------- Local function forward declaration -------------------
 */

/** ============================================================================
 *
 *  @Function:	    Local function forward declaration.
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

/*
 *  --------------------- Public function definition ---------------------------
 */

/** ============================================================================
 *
 *  @Function:	    Public function definition.
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

void debugger_init(FILE* fp, const char* logger_folder)
{
    if (cur_init++ == 0) {
        g_debug_level = DBG_INFO;
        gbl_debug_logger = fp;
        dbg_logger_folder = logger_folder;
        pthread_mutex_init(&gbl_debug_mutex, NULL);
    }
}

void debugger_destroy(void)
{
	time_t tmt;
	time(&tmt);
	struct tm tm;

    if (--cur_init == 0) {
        localtime_r(&tmt, &tm);

        if(gbl_debug_logger2) {
            fprintf(gbl_debug_logger2, "\n\nSystem logger stopped at %s %02d, %02d:%02d:%02d.\n",
                    gbl_months[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            fclose(gbl_debug_logger2);
        }
        pthread_mutex_destroy(&gbl_debug_mutex);
    }
}

void debugger_update(struct tm* tm)
{
	if(dbg_logger_folder == NULL) {
		return;
	}

	if(old_tm.tm_year != tm->tm_year || old_tm.tm_mon != tm->tm_mon || old_tm.tm_mday != tm->tm_mday) {
		char filename[128];
		old_tm = *tm;
		sprintf(filename, "%s/osa_%04d%02d%02d.log", dbg_logger_folder,
				tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

		if(gbl_debug_logger2) {
			fprintf(gbl_debug_logger2, "\n\nSystem logger stopped at %s %02d, %02d:%02d:%02d.\n",
				gbl_months[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
			fclose(gbl_debug_logger2);
		}

		gbl_debug_logger2 = fopen(filename, "a");

		if(gbl_debug_logger2) {
			fprintf(gbl_debug_logger2, "\n\nSystem logger started at %s %02d, %02d:%02d:%02d.\n",
					gbl_months[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		}
	}
}

void debugger_setlevel(int level)
{
	g_debug_level = level;
}
/*
 *  --------------------- Local function definition ----------------------------
 */

/** ============================================================================
 *
 *  @Function:	    Local function definition.
 *
 *  @Description:   //	函数功能、性能等的描述
 *
 *  ============================================================================
 */

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */
