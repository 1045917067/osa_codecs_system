/** ============================================================================
 *
 *  @File name:	osa_debugger.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
/*  --------------------- Include user headers   ---------------------------- */

#include "osa.h"
#include "osa_debugger.h"
#include "mutex.h"

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

#define OSA_DEBUGGER_CRITICAL_ENTER()       \
    do {                                    \
        mutex_lock(&glb_debug_mutex);       \
    } while (0)

#define OSA_DEBUGGER_CRITICAL_LEAVE()       \
    do {                                    \
        mutex_unlock(&glb_debug_mutex);     \
    } while (0)

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

struct __osa_debugger_object_t;
typedef struct __osa_debugger_object_t osa_debugger_object_t;
struct __osa_debugger_object_t
{
    osa_debugger_prm_t
                    m_debug_prm;

    FILE *          m_out;
    FILE *          m_out2;
    unsigned int    m_debug_level;
    mutex_t         m_mutex;
    struct tm       m_tm;
};

/*
 *  --------------------- Global variable definition ---------------------------
 */

/** ----------------------------------------------------------------------------
 *  @Name:          Variable name
 *
 *  @Description:   Description of the variable.
 * -----------------------------------------------------------------------------
 */

static unsigned int glb_cur_init = 0;

static osa_debugger_object_t glb_debug_obj;

OSA_DECLARE_AND_INIT_MUTEX(glb_debug_mutex);


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

static status_t
__osa_debugger_init(osa_debugger_object_t *pobj, const osa_debugger_prm_t *prm);

static status_t __osa_debugger_deinit(osa_debugger_object_t *pobj);

static void     __osa_debugger_update(struct tm *ptm);


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

status_t osa_debugger_init(const osa_debugger_prm_t *prm)
{
    status_t status = OSA_SOK;
    osa_debugger_object_t *pobj = &glb_debug_obj;

    OSA_DEBUGGER_CRITICAL_ENTER();

    if (glb_cur_init++ == 0) {
        status = __osa_debugger_init(pobj, prm);
    }

    OSA_DEBUGGER_CRITICAL_LEAVE();

    return status;
}

void     osa_debugger(int level, const char *tags, const char *fmt, ...)
{
    osa_debugger_object_t *pobj = &glb_debug_obj;

    if (level >= pobj->m_debug_level && pobj->m_out != NULL) {
        struct tm tim;
        struct timeval tv;
        va_list ap;

        gettimeofday(&tv, NULL);
        localtime_r(&tv.tv_sec, &tim);

        mutex_lock(&pobj->m_mutex);

        fprintf(pobj->m_out, "%d-%02d-%02dT%02d:%02d:%02d.%03d| %s| ",
                tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec,
                tv.tv_usec / 1000, tags);
        fflush(pobj->m_out);

        if (pobj->m_out2 != NULL) {
            fprintf(pobj->m_out2, "%d-%02d-%02dT%02d:%02d:%02d.%03d| %s| ",
                    tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec,
                    tv.tv_usec / 1000, tags);
            fflush(pobj->m_out2);
        }

        va_start(ap, fmt);

        vfprintf(pobj->m_out, fmt, ap);

        if (pobj->m_out2 != NULL) {
            vfprintf(pobj->m_out2, fmt, ap);
        }

        va_end(ap);
        
        __osa_debugger_update(&tim);

        mutex_unlock(&pobj->m_mutex);
    }
}

status_t osa_debugger_setlevel(int level)
{
    osa_debugger_object_t *pobj = &glb_debug_obj;

    if (level < DBG_DETAILED || level > DBG_FATAL) {
        return OSA_EINVAL;
    }

    mutex_lock(&pobj->m_mutex);

    pobj->m_debug_level = level;

    mutex_unlock(&pobj->m_mutex);

	osa_debugger(DBG_ERROR, "osa debugger", "Logger system set debug level=%d.\n", pobj->m_debug_level);

    return OSA_SOK;
}

status_t osa_debugger_deinit(void)
{
    status_t status = OSA_SOK;
    osa_debugger_object_t *pobj = &glb_debug_obj;

    OSA_DEBUGGER_CRITICAL_ENTER();

    if (--glb_cur_init == 0) {
        status = __osa_debugger_deinit(pobj);
    }

    OSA_DEBUGGER_CRITICAL_LEAVE();

    return status;
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

static status_t
__osa_debugger_init(osa_debugger_object_t *pobj, const osa_debugger_prm_t *prm)
{
    status_t status = OSA_SOK;

    memset(pobj, 0, sizeof(*pobj));

    memcpy(&pobj->m_debug_prm, prm, sizeof(*prm));

    pobj->m_out = prm->m_out;
    pobj->m_debug_level = prm->m_debug_level;
    status |=mutex_create(&pobj->m_mutex);

    OSA_assert(OSA_SOK == status);

    if (pobj->m_debug_prm.m_folder != NULL) {
        unsigned char filename[256];
        time_t now_time;
        struct tm tim;

        time(&now_time);
        localtime_r(&now_time, &tim);

        snprintf(filename, sizeof(filename) - 1, "%s/%s_%04d%02d%02d.log",
				pobj->m_debug_prm.m_folder,
                pobj->m_debug_prm.m_name,
				tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday);

        pobj->m_out2 = fopen(filename, "ab");

        if (pobj->m_out2 != NULL) {
            osa_debugger(DBG_ERROR, "osa debugger", "Logger system started.\n");
        }
    }

    return status;
}

static status_t
__osa_debugger_deinit(osa_debugger_object_t *pobj)
{
    status_t status = OSA_SOK;

    if (pobj->m_out2 != NULL) {
        DBG(DBG_ERROR, "osa debugger", "Logger system stopped.\n\n");
    }

    if (pobj->m_out2 != NULL) {
        fclose(pobj->m_out2);
    }

    status |= mutex_delete(&pobj->m_mutex);

    return status;
}

static void __osa_debugger_update(struct tm *ptm)
{
    osa_debugger_object_t *pobj = &glb_debug_obj;

	if(pobj->m_out2 == NULL) {
		return;
	}

	if (pobj->m_tm.tm_year != ptm->tm_year 
            || pobj->m_tm.tm_mon != ptm->tm_mon
            || pobj->m_tm.tm_mday != ptm->tm_mday) {
		char filename[256];
		pobj->m_tm = *ptm;

        if (pobj->m_out2 != NULL) {
            fclose(pobj->m_out2);
        }
        
        snprintf(filename, sizeof(filename) - 1, "%s/%s_%04d%02d%02d.log",
				pobj->m_debug_prm.m_folder,
				pobj->m_debug_prm.m_name,
				pobj->m_tm.tm_year + 1900, pobj->m_tm.tm_mon + 1, pobj->m_tm.tm_mday);

        pobj->m_out2 = fopen(filename, "ab");
	}
}

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */
