/** ============================================================================
 *
 *  @File name:	sem.c
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

/*  --------------------- Include user headers   ---------------------------- */

#include "osa.h"
#include "osa_sem.h"

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

#define osa_sem_is_exit(sem)   ((sem)->m_state == OSA_SEM_STATE_EXIT)
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

enum __osa_sem_state_t; typedef enum __osa_sem_state_t osa_sem_state_t;
enum __osa_sem_state_t
{
    OSA_SEM_STATE_INIT = 0,
    OSA_SEM_STATE_EXIT = 1,
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

status_t osa_sem_create(osa_sem_t *sem, unsigned int max_cnt, unsigned int init_value)
{
    status_t status = OSA_SOK;
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    status |= pthread_mutexattr_init(&mutex_attr);
    status |= pthread_condattr_init(&cond_attr);

    status |= pthread_mutex_init(&sem->m_mutex, &mutex_attr);
    status |= pthread_cond_init(&sem->m_cond, &cond_attr);
    
    sem->m_count     = init_value;
    sem->m_max_count = max_cnt;

    if (sem->m_max_count == 0) {
        sem->m_max_count = 1;
    }

    if (sem->m_count > sem->m_max_count) {
        sem->m_count = sem->m_max_count;
    }

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_condattr_destroy(&cond_attr);

    sem->m_state = OSA_SEM_STATE_INIT;

    return status;
}

status_t osa_sem_wait  (osa_sem_t *sem, unsigned int timeout)
{
    status_t status = OSA_EFAIL;

    pthread_mutex_lock(&sem->m_mutex);

    while (!osa_sem_is_exit(sem)) {
        if (sem->m_count > 0) {
            sem->m_count--;
            status = OSA_SOK;
            break;
        } else {
            if (timeout == OSA_TIMEOUT_NONE) {
                break;
            }

            status |= pthread_cond_wait(&sem->m_cond, &sem->m_mutex);
        }
    }

    pthread_mutex_unlock(&sem->m_mutex);

    return status;
}

status_t osa_sem_signal(osa_sem_t *sem)
{
    status_t status = OSA_SOK;

    pthread_mutex_lock(&sem->m_mutex);

    if (sem->m_count < sem->m_max_count) {
        sem->m_count++;
        status |= pthread_cond_signal(&sem->m_cond);
    }

    pthread_mutex_unlock(&sem->m_mutex);

    return status;
}

status_t osa_sem_exit  (osa_sem_t *sem)
{
    status_t status = OSA_SOK;

    pthread_mutex_lock(&sem->m_mutex);

    sem->m_state = OSA_SEM_STATE_EXIT;

    pthread_cond_broadcast(&sem->m_cond);

    pthread_mutex_unlock(&sem->m_mutex);

    return status;
}

status_t osa_sem_delete(osa_sem_t *sem)
{
    status_t status = OSA_SOK;

    pthread_cond_destroy(&sem->m_cond);
    pthread_mutex_destroy(&sem->m_mutex);

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

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */
