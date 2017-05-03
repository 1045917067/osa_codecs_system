/** ============================================================================
 *
 *  @File name:	osa_queue.h
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

#if !defined (__OSA_QUEUE_H)
#define __OSA_QUEUE_H

/*  --------------------- Include system headers ---------------------------- */

#include <stdbool.h>
#include <pthread.h>

/*  --------------------- Include user headers   ---------------------------- */

#include "osa.h"
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

enum __queue_state_t; typedef enum __queue_state_t queue_state_t;
enum __queue_state_t
{
    OSA_QUEUE_STATE_INIT = 0,
    OSA_QUEUE_STATE_EXIT = 1,
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

struct __queue_t; typedef struct __queue_t queue_t;
struct __queue_t
{
	unsigned int	m_rd_idx;
	unsigned int	m_wr_idx;
	unsigned int	m_len;
	unsigned int	m_count;
	unsigned int  * m_queue;
	
    pthread_mutex_t m_mutex;
	pthread_cond_t	m_rd_cond;
	pthread_cond_t	m_wr_cond;
    volatile
    unsigned int    m_state;
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

status_t queue_create(queue_t *queue, unsigned int max_len);
status_t queue_put(queue_t *queue, unsigned int value, unsigned int timeout);
status_t queue_get(queue_t *queue, unsigned int *value, unsigned int timeout);
status_t queue_peek(queue_t *queue, unsigned int *value);
status_t queue_count(queue_t *queue, unsigned int *count);
bool     queue_is_empty(queue_t *queue);
status_t queue_exit(queue_t *queue);
status_t queue_set_state(queue_t *queue, queue_state_t state);
status_t queue_reset(queue_t *queue);
status_t queue_delete(queue_t *queue);

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__OSA_QUEUE_H) */
