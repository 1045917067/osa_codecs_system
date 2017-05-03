/** ============================================================================
 *
 *  @File name:	mutex.h
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

#if !defined (__MUTEX_H)
#define __MUTEX_H

/*  --------------------- Include system headers ---------------------------- */

#include <pthread.h>
/*  --------------------- Include user headers   ---------------------------- */

#include "osa_status.h"

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

#define	MUTEX_INITIALIZER(mutex)	            \
		(mutex.m_mutex = PTHREAD_MUTEX_INITIALIZER;)

#define OSA_DECLARE_AND_INIT_MUTEX(mutex)       \
    static mutex_t (mutex) = {                  \
        .m_mutex = PTHREAD_MUTEX_INITIALIZER    \
    };

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

struct __mutex_t; typedef struct __mutex_t mutex_t;
struct __mutex_t
{
	pthread_mutex_t	m_mutex;
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

/* Creates a mutex */
status_t mutex_create(mutex_t *mutex);
/* Waits infinitely till mutex lock is got. */
status_t mutex_lock  (mutex_t *mutex);
/* Waits for mutex lock for specified amount of time. It returns on getting  */
/* the lock or time out.                                                     */
status_t mutex_timedlock(mutex_t *mutex, unsigned int msec);
/* Releases the lock held on the mutex. */
status_t mutex_unlock(mutex_t *mutex);
/* Closes the mutex. */
status_t mutex_delete(mutex_t *mutex);

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__MUTEX_H) */
