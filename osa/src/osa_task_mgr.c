/** ============================================================================
 *
 *  @File name:	osa_task_mgr.c
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

/*  --------------------- Include user headers   ---------------------------- */

#include "osa.h"
#include "osa_task_mgr.h"
#include "osa_msgq.h"
#include "osa_mailbox.h"
#include "osa_task.h"
#include "dlist.h"
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

#define TASK_MGR_CRITICAL_ENTER()           \
    do {                                    \
        mutex_lock(&glb_tsk_mgr_mutex);     \
    } while (0)

#define TASK_MGR_CRITICAL_LEAVE()           \
    do {                                    \
        mutex_unlock(&glb_tsk_mgr_mutex);   \
    } while (0)

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

struct __task_mgr_t; typedef struct __task_mgr_t task_mgr_t;
struct __task_mgr_t
{
    unsigned int    m_reserved[2];
    unsigned int    m_initialized;
    unsigned int    m_msg_id;
    unsigned short  m_msg_cnt;
    unsigned short  m_tsk_cnt;
    msg_t         * m_msgs;
    mutex_t         m_mutex;
    dlist_t         m_tsklists;
    dlist_t         m_free_list;
    task_object_t   m_mgr_tsk;
};

typedef struct __task_mgr_t * task_mgr_handle;

struct __task_mgr_find_prm_t;
typedef struct __task_mgr_find_prm_t task_mgr_find_prm_t;
struct __task_mgr_find_prm_t
{
    unsigned char * m_name;     //  [IN]
    task_object_t * m_task;     //  [OUT]
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

static task_mgr_prm_t glb_tsk_mgr_prm  = {
    .m_msg_cnt    = TASK_MGR_MSG_MAX,
    .m_tsk_cnt    = TASK_MGR_TSK_MAX
};

static task_mgr_t glb_tsk_mgr_obj = {
    .m_initialized = FALSE,
    .m_msg_id = 0,
    .m_mgr_tsk = {
        .m_name = TASK_MGR_TSK_NAME,
    }
};

static unsigned int glb_cur_init = 0;

OSA_DECLARE_AND_INIT_MUTEX(glb_tsk_mgr_mutex);

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
__task_mgr_env_initialize(task_mgr_prm_t *prm);

static status_t
__task_mgr_env_deinitialize(void);

static status_t
__task_mgr_initialize(task_mgr_t *tskmgr, task_mgr_prm_t *prm);

static status_t
__task_mgr_alloc_msg(task_mgr_t *tskmgr, msg_t **msg);

static status_t
__task_mgr_free_msg(task_mgr_t *tskmgr, msg_t *msg);

static status_t
__task_mgr_register_tsk(task_mgr_handle hdl, task_object_t *tsk);

static status_t
__task_mgr_start_tsk(task_mgr_handle hdl, task_object_t *tsk);

static status_t
__task_mgr_stop_tsk(task_mgr_handle hdl, task_object_t *tsk);

static status_t
__task_mgr_unregister_tsk(task_mgr_handle hdl, task_object_t *tsk);

static status_t
__task_mgr_deinitialize(task_mgr_t *tskmgr);

static status_t __task_mgr_internal_main(void *userdata, task_t tsk, msg_t **msg);

static status_t
__task_mgr_broadcast(task_mgr_t *tskmgr, unsigned short cmd, void *prm, unsigned int size, unsigned int flags);

static status_t
__task_mgr_synchronize(task_t to, task_t frm, unsigned short cmd, void *prm, unsigned int size, unsigned int flags);

static status_t
__task_mgr_task_instruments(task_mgr_t *tskmgr);

static status_t
__task_mgr_thdpool_instruments(task_mgr_t *tskmgr);

static status_t
__task_mgr_find(task_mgr_t *tskmgr, task_mgr_find_prm_t *prm);

static inline unsigned int
__task_mgr_get_msg_id(task_mgr_t *tskmgr)
{
    unsigned int id;

    mutex_lock(&tskmgr->m_mutex);

    id = tskmgr->m_msg_id++;

    mutex_unlock(&tskmgr->m_mutex);

    return id;
}

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

status_t task_mgr_init(task_mgr_prm_t *prm)
{
    status_t status = OSA_SOK;
	
	/* Initialize task manager object */
    TASK_MGR_CRITICAL_ENTER();

	if (glb_cur_init++ == 0) {
		status = __task_mgr_initialize(&glb_tsk_mgr_obj, prm);
	}

    TASK_MGR_CRITICAL_LEAVE();
	
	return status;
}

status_t task_mgr_register(task_object_t *tsk)
{
    return __task_mgr_synchronize(glb_tsk_mgr_obj.m_mgr_tsk.m_task,
            glb_tsk_mgr_obj.m_mgr_tsk.m_task, TASK_MGR_CMD_CREATE_TASK, tsk, sizeof(task_object_t), MSG_FLAGS_WAIT_ACK);
}

status_t task_mgr_start(task_object_t *tsk)
{
	return __task_mgr_start_tsk(&glb_tsk_mgr_obj, tsk);
}

status_t task_mgr_stop(task_object_t *tsk)
{
	return __task_mgr_stop_tsk(&glb_tsk_mgr_obj, tsk);
}

status_t task_mgr_unregister(task_object_t *tsk)
{
    return __task_mgr_synchronize(glb_tsk_mgr_obj.m_mgr_tsk.m_task,
            glb_tsk_mgr_obj.m_mgr_tsk.m_task, TASK_MGR_CMD_DELETE_TASK, tsk, sizeof(task_object_t), MSG_FLAGS_WAIT_ACK);
}

status_t task_mgr_broadcast(unsigned short cmd, void *prm, unsigned int size, unsigned int flags)
{
    return __task_mgr_broadcast(&glb_tsk_mgr_obj, cmd, prm, size, flags);
}

status_t task_mgr_synchronize(task_object_t *tsk, unsigned short cmd, void *prm, unsigned int size, unsigned int flags)
{
    return __task_mgr_synchronize(tsk->m_task,
            glb_tsk_mgr_obj.m_mgr_tsk.m_task, cmd, prm, size, flags);
}

status_t task_mgr_synchronize_main(unsigned short cmd, void *prm, unsigned int size, unsigned int flags)
{
	int i;
	int msg_cnt = 0;

	msg_t *msg = NULL;
	status_t status = OSA_SOK;
	task_object_t 	tsklists[TASK_MGR_TSK_MAX];
	task_state_t    tsk_state;
	task_object_t * cur_tsk_node = NULL;
	task_object_t * nex_tsk_node = NULL;

	for (i = 0; i < OSA_ARRAYSIZE(tsklists); i++) {
		memset(tsklists, 0, sizeof(tsklists));
	}
	task_mgr_t *tskmgr = &glb_tsk_mgr_obj;
	mutex_lock(&tskmgr->m_mutex);

	status = dlist_first(&tskmgr->m_tsklists, (dlist_element_t **)&cur_tsk_node);
	OSA_assert(OSA_SOK == status);

	do {
		status = dlist_next(&tskmgr->m_tsklists,
				(dlist_element_t *) cur_tsk_node,
				(dlist_element_t **)&nex_tsk_node
				);
		if (!OSA_ISERROR(status) && nex_tsk_node != NULL) {
			status = task_get_state(nex_tsk_node->m_task, &tsk_state);
			OSA_assert(OSA_SOK == status);
			if ((tsk_state != TASK_STATE_EXIT) && (nex_tsk_node->m_main == NULL)) {
				tsklists[msg_cnt++] = *nex_tsk_node;
			}
			cur_tsk_node = nex_tsk_node;
		}
	} while (!OSA_ISERROR(status) && (nex_tsk_node != NULL));

	mutex_unlock(&tskmgr->m_mutex);

	if (msg_cnt == 0) {
		return OSA_SOK;
	}

	for (i = 0; i < msg_cnt; i++) {
		if(!OSA_ISERROR(status)) {
			status |= __task_mgr_synchronize(tsklists[i].m_task, tskmgr->m_mgr_tsk.m_task, cmd, prm, size, flags);
		}
	}

	return status;
}

status_t task_mgr_find(const char *name, task_object_t **ptsk)
{
    status_t status = OSA_SOK;

    (*ptsk) = NULL;

    /* TODO */
    task_mgr_find_prm_t prm;

    prm.m_name = (unsigned char *)name;
    prm.m_task = NULL;

    status = __task_mgr_synchronize(glb_tsk_mgr_obj.m_mgr_tsk.m_task,
            glb_tsk_mgr_obj.m_mgr_tsk.m_task, TASK_MGR_CMD_FIND_TASK, &prm, sizeof(prm), MSG_FLAGS_WAIT_ACK);

    if (!OSA_ISERROR(status)) {
        (*ptsk) = prm.m_task;
    }

    return status;
}

status_t task_mgr_task_instruments(void)
{
    return __task_mgr_synchronize(glb_tsk_mgr_obj.m_mgr_tsk.m_task,
            glb_tsk_mgr_obj.m_mgr_tsk.m_task, TASK_MGR_CMD_TASK_INSTRUMENTS, NULL, 0, MSG_FLAGS_WAIT_ACK);
}

status_t task_mgr_thdpool_instruments(void)
{
    return __task_mgr_synchronize(glb_tsk_mgr_obj.m_mgr_tsk.m_task,
            glb_tsk_mgr_obj.m_mgr_tsk.m_task, TASK_MGR_CMD_THDPOOL_INSTRUMENTS, NULL, 0, MSG_FLAGS_WAIT_ACK);
}

status_t task_mgr_deinit(void)
{
    status_t status = OSA_SOK;
	
	/* Deintialize task manager object */
    TASK_MGR_CRITICAL_ENTER();

	if (--glb_cur_init == 0) {
		status = __task_mgr_deinitialize(&glb_tsk_mgr_obj);
	}

    TASK_MGR_CRITICAL_LEAVE();
	
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
__task_mgr_env_initialize(task_mgr_prm_t *prm)
{
    status_t status = OSA_SOK;
    msgq_mgr_prm_t msgq_mgr_prm;
    mailbox_system_prm_t mbx_sys_prm;
    tasklist_params_t tsklist_prm;

    msgq_mgr_prm.m_msgq_cnt = prm->m_tsk_cnt * 2;
    mbx_sys_prm.m_mbx_cnt   = prm->m_tsk_cnt;
    tsklist_prm.m_tsk_cnt   = prm->m_tsk_cnt;

    status |= msgq_mgr_init(&msgq_mgr_prm);

    status |= mailbox_system_init(&mbx_sys_prm);

    status |= tasklist_init(&tsklist_prm);
    
    OSA_assert(OSA_SOK == status);

    return status;
}

static status_t
__task_mgr_env_deinitialize(void)
{
    status_t status = OSA_SOK;

    status |= tasklist_deinit();

    status |= mailbox_system_deinit();

    status |= msgq_mgr_deinit();

    return status;
}

static status_t
__task_mgr_initialize(task_mgr_t *tskmgr, task_mgr_prm_t *prm)
{
    int i;
	status_t status = OSA_SOK;

    /*
     *  Initialize task manager env.
     */
    status = __task_mgr_env_initialize(prm);
    if (OSA_ISERROR(status)) {
        return status;
    }

    tskmgr->m_msg_id  = 0;
    tskmgr->m_msg_cnt = prm->m_msg_cnt;
    tskmgr->m_tsk_cnt = 0;
    tskmgr->m_msgs    = NULL;

    status = mutex_create(&tskmgr->m_mutex);

    status = dlist_init(&tskmgr->m_tsklists);
    status = dlist_init(&tskmgr->m_free_list);

    /* Allocate msgs */
    status = task_alloc_msg(sizeof(msg_t) * tskmgr->m_msg_cnt, (msg_t **)&tskmgr->m_msgs);
    if (OSA_ISERROR(status)) {
        mutex_delete(&tskmgr->m_mutex);
        __task_mgr_env_deinitialize();
        return status;
    }

    for (i = 0; i < tskmgr->m_msg_cnt; i++) {
        status = dlist_initialize_element((dlist_element_t *)(tskmgr->m_msgs + i));
        status = dlist_put_tail(&tskmgr->m_free_list, (dlist_element_t *)(tskmgr->m_msgs + i));
    }

    /* Register task manager */
    tskmgr->m_mgr_tsk.m_main       = __task_mgr_internal_main;
    tskmgr->m_mgr_tsk.m_find       = NULL;
    tskmgr->m_mgr_tsk.m_pri        = 0;
    tskmgr->m_mgr_tsk.m_stack_size = 0;
    tskmgr->m_mgr_tsk.m_init_state = 0;
    tskmgr->m_mgr_tsk.m_userdata   = (void *)tskmgr;

    status = __task_mgr_register_tsk(tskmgr, &tskmgr->m_mgr_tsk);

	if (OSA_ISERROR(status)) {
        task_free_msg(sizeof(msg_t) * tskmgr->m_msg_cnt, (msg_t *)tskmgr->m_msgs);
        tskmgr->m_msgs = NULL;
        mutex_delete(&tskmgr->m_mutex);
        __task_mgr_env_deinitialize();
        return status;
    }

    /* Send INIT cmd */
    status = __task_mgr_synchronize(tskmgr->m_mgr_tsk.m_task,
            tskmgr->m_mgr_tsk.m_task, TASK_CMD_INIT, NULL, 0, MSG_FLAGS_WAIT_ACK);
    if (OSA_ISERROR(status)) {
        __task_mgr_synchronize(tskmgr->m_mgr_tsk.m_task,
                tskmgr->m_mgr_tsk.m_task, TASK_CMD_EXIT, NULL, 0, MSG_FLAGS_WAIT_ACK);

        __task_mgr_unregister_tsk(tskmgr, &tskmgr->m_mgr_tsk);

        task_free_msg(sizeof(msg_t) * tskmgr->m_msg_cnt, (msg_t *)tskmgr->m_msgs);
        tskmgr->m_msgs = NULL;
        mutex_delete(&tskmgr->m_mutex);
        __task_mgr_env_deinitialize();
        return status;
    }

    /* Send PROC cmd */
    status = __task_mgr_synchronize(tskmgr->m_mgr_tsk.m_task,
            tskmgr->m_mgr_tsk.m_task, TASK_CMD_PROC, NULL, 0, MSG_FLAGS_WAIT_ACK);
    if (OSA_ISERROR(status)) {
        __task_mgr_synchronize(tskmgr->m_mgr_tsk.m_task,
                tskmgr->m_mgr_tsk.m_task, TASK_CMD_EXIT, NULL, 0, MSG_FLAGS_WAIT_ACK);

        __task_mgr_unregister_tsk(tskmgr, &tskmgr->m_mgr_tsk);

        task_free_msg(sizeof(msg_t) * tskmgr->m_msg_cnt, (msg_t *)tskmgr->m_msgs);
        tskmgr->m_msgs = NULL;
        mutex_delete(&tskmgr->m_mutex);
        __task_mgr_env_deinitialize();
        return status;
    }

    tskmgr->m_initialized = TRUE;

    OSA_assert(OSA_SOK == status);

	return status;
}

static status_t
__task_mgr_alloc_msg(task_mgr_t *tskmgr, msg_t **msg)
{
    status_t status = OSA_ENOENT;

    (*msg) = NULL;

    mutex_lock(&tskmgr->m_mutex);

    if (!dlist_is_empty(&tskmgr->m_free_list)) {
        status = dlist_get_head(&tskmgr->m_free_list, (dlist_element_t **)msg);

        OSA_assert(OSA_SOK == status);
    }

    mutex_unlock(&tskmgr->m_mutex);

    return status;
}

static status_t
__task_mgr_free_msg(task_mgr_t *tskmgr, msg_t *msg)
{
    status_t status = OSA_SOK;

    mutex_lock(&tskmgr->m_mutex);

    status |= dlist_put_tail(&tskmgr->m_free_list, (dlist_element_t *)msg);

    OSA_assert(OSA_SOK == status);

    mutex_unlock(&tskmgr->m_mutex);

    return status;
}

static status_t
__task_mgr_deinitialize(task_mgr_t *tskmgr)
{
    status_t status = OSA_SOK;
    task_object_t * cur_tsk_node = NULL;
    task_object_t * nex_tsk_node = NULL;

	/* 
     * Deinitialize task manager object.
     *
     */

    /* Delete the tasks from the tasklists */
    status = dlist_first(&tskmgr->m_tsklists, (dlist_element_t **)&cur_tsk_node);

    while (!OSA_ISERROR(status) && (cur_tsk_node != NULL)) {

        status = dlist_next(&tskmgr->m_tsklists,
                            (dlist_element_t *) cur_tsk_node,
                            (dlist_element_t **)&nex_tsk_node
                            );

        if (!OSA_ISERROR(status) && (nex_tsk_node != NULL)) {
            /* Send DELETE_TASK cmd to delete task from tasklists */
            __task_mgr_synchronize(tskmgr->m_mgr_tsk.m_task,
                    tskmgr->m_mgr_tsk.m_task, TASK_MGR_CMD_DELETE_TASK,
                    (void *)nex_tsk_node, sizeof(*nex_tsk_node), MSG_FLAGS_WAIT_ACK);
        }
        
        cur_tsk_node = nex_tsk_node;
    }

    /* Send EXIT cmd to task manager itself */
    status |= __task_mgr_synchronize(tskmgr->m_mgr_tsk.m_task,
            tskmgr->m_mgr_tsk.m_task, TASK_CMD_EXIT, NULL, 0, MSG_FLAGS_WAIT_ACK);

    /* Delete task manager itself from tasklists */
    status |= __task_mgr_unregister_tsk(tskmgr, &tskmgr->m_mgr_tsk);

    status |= task_free_msg(sizeof(msg_t) * tskmgr->m_msg_cnt, (msg_t *)tskmgr->m_msgs);

    tskmgr->m_msgs = NULL;

    status |= mutex_delete(&tskmgr->m_mutex);

    /*
     *  Deinitialize task mansger env.
     */
    status |= __task_mgr_env_deinitialize();

    tskmgr->m_initialized = FALSE;

	return status;
}

static status_t
__task_mgr_broadcast(task_mgr_t *tskmgr, unsigned short cmd, void *prm, unsigned int size, unsigned int flags)
{
    int i;
    int msg_cnt = 0;

    msg_t *msg = NULL;
    status_t status = OSA_SOK;
    task_t tsklists[TASK_MGR_TSK_MAX];
    task_state_t    tsk_state;
    task_object_t * cur_tsk_node = NULL;
    task_object_t * nex_tsk_node = NULL;

    for (i = 0; i < OSA_ARRAYSIZE(tsklists); i++) {
        tsklists[i] = TASK_INVALID_TSK;
    }

    mutex_lock(&tskmgr->m_mutex);

    status = dlist_first(&tskmgr->m_tsklists, (dlist_element_t **)&cur_tsk_node);
    OSA_assert(OSA_SOK == status);

    do {
        status = dlist_next(&tskmgr->m_tsklists,
                            (dlist_element_t *) cur_tsk_node,
                            (dlist_element_t **)&nex_tsk_node
                           );
        if (!OSA_ISERROR(status) && nex_tsk_node != NULL) {
            status = task_get_state(nex_tsk_node->m_task, &tsk_state);
            OSA_assert(OSA_SOK == status);
            if ((nex_tsk_node->m_main != NULL) && (tsk_state != TASK_STATE_EXIT)) {
                tsklists[msg_cnt++] = nex_tsk_node->m_task;
            }
            cur_tsk_node = nex_tsk_node;
        }
    } while (!OSA_ISERROR(status) && (nex_tsk_node != NULL));

    mutex_unlock(&tskmgr->m_mutex);

    if (msg_cnt == 0) {
        return OSA_SOK;
    }

    for (i = 0; i < msg_cnt; i++) {
        status |= __task_mgr_synchronize(tsklists[i], tskmgr->m_mgr_tsk.m_task, cmd, prm, size, flags);
    }

    return status;
}

static status_t
__task_mgr_synchronize(task_t to, task_t frm, unsigned short cmd, void *prm, unsigned int size, unsigned int flags)
{
    msg_t *msg = NULL;
    msg_t *rcv_msg = NULL;
    status_t status = OSA_SOK;
    unsigned int id;
	
    /* Synchronize task */
    if (flags & MSG_FLAGS_WAIT_ACK) {
        status = __task_mgr_alloc_msg(&glb_tsk_mgr_obj, &msg);
    } else {
        status = task_alloc_msg(sizeof(*msg), &msg);
    }

    if (OSA_ISERROR(status)) {
        return status;
    }

    id = __task_mgr_get_msg_id(&glb_tsk_mgr_obj);

    msg_init(msg);
    msg_set_cmd(msg, cmd);
    msg_set_payload_ptr(msg, prm);
    msg_set_payload_size(msg, size);
    msg_set_flags(msg, flags);
    msg_set_msg_size(msg, sizeof(*msg));
    msg_set_msg_id(msg, id);

    status = task_send_msg(to, frm, msg, MSG_TYPE_CMD);
    if (OSA_ISERROR(status)) {
        return status;
    }

    if (flags & MSG_FLAGS_WAIT_ACK) {
        status = task_wait_ack(frm, &rcv_msg, id);
        if (OSA_ISERROR(status)) {
            return status;
        }

        OSA_assert(msg == rcv_msg);

        status = msg_get_status(rcv_msg);

        __task_mgr_free_msg(&glb_tsk_mgr_obj, msg);
    }

	return status;
}

static status_t
__task_mgr_register_tsk(task_mgr_handle hdl, task_object_t *tsk)
{
    status_t status = OSA_SOK;
    task_mgr_find_prm_t prm;

    if (tsk == NULL) {
        return OSA_EINVAL;
    }

    /*
     *  Check the task wheter it is already registered.
     */
    prm.m_name = (unsigned char *)tsk->m_name;
    prm.m_task = NULL;
    status = __task_mgr_find(hdl, &prm);

    if (!OSA_ISERROR(status)) {
        OSA_assert(prm.m_task == tsk);
        return status;
    }

    /* Create task */
    status = task_create(tsk->m_name,
                         tsk->m_main,
                         tsk->m_pri,
                         tsk->m_stack_size,
                         tsk->m_init_state,
                         tsk->m_userdata,
                         &tsk->m_task
                        );

    if (OSA_ISERROR(status)) {
        return status;
    }

    /* Register task object */
    mutex_lock(&hdl->m_mutex);

    status = dlist_initialize_element((dlist_element_t *)tsk);
    status = dlist_put_tail(&hdl->m_tsklists, (dlist_element_t *)tsk);

    hdl->m_tsk_cnt += 1;

    mutex_unlock(&hdl->m_mutex);

    return status;
}

static status_t
__task_mgr_start_tsk(task_mgr_handle hdl, task_object_t *tsk)
{
    status_t status = OSA_SOK;
    task_mgr_find_prm_t prm;

    if (tsk == NULL) {
        return OSA_EINVAL;
    }

    /*
     *  Check the task wheter it is already registered.
     */
    prm.m_name = (unsigned char *)tsk->m_name;
    prm.m_task = NULL;
    status = __task_mgr_find(hdl, &prm);

    if (OSA_ISERROR(status)) {
        return status;
    }

    status = __task_mgr_synchronize(tsk->m_task, hdl->m_mgr_tsk.m_task, TASK_CMD_INIT, NULL, 0, MSG_FLAGS_WAIT_ACK);
    if (OSA_ISERROR(status)) {
        return status;
    }

    status = __task_mgr_synchronize(tsk->m_task, hdl->m_mgr_tsk.m_task, TASK_CMD_PROC, NULL, 0, MSG_FLAGS_WAIT_ACK);

    return status;
}

static status_t
__task_mgr_stop_tsk(task_mgr_handle hdl, task_object_t *tsk)
{
    status_t status = OSA_SOK;
    task_mgr_find_prm_t prm;

    if (tsk == NULL) {
        return OSA_EINVAL;
    }

    /*
     *  Check the task wheter it is already registered.
     */
    prm.m_name = (unsigned char *)tsk->m_name;
    prm.m_task = NULL;
    status = __task_mgr_find(hdl, &prm);

    if (OSA_ISERROR(status)) {
        return status;
    }

    return __task_mgr_synchronize(tsk->m_task, hdl->m_mgr_tsk.m_task, TASK_CMD_EXIT, NULL, 0, MSG_FLAGS_WAIT_ACK);
}

static status_t
__task_mgr_unregister_tsk(task_mgr_handle hdl, task_object_t *tsk)
{
    status_t status = OSA_SOK;
    task_mgr_find_prm_t prm;

    if (tsk == NULL) {
        return OSA_EINVAL;
    }

    /*
     *  Check the task wheter it is already registered.
     */
    prm.m_name = (unsigned char *)tsk->m_name;
    prm.m_task = NULL;
    status = __task_mgr_find(hdl, &prm);

    if (OSA_ISERROR(status)) {
        return status;
    }

    /* Delete task */
    status = task_delete(&tsk->m_task);

    /* Unregister task object */
    mutex_lock(&hdl->m_mutex);

    status = dlist_remove_element(&hdl->m_tsklists, (dlist_element_t *)tsk);

    mutex_unlock(&hdl->m_mutex);

    return status;
}

static status_t
__task_mgr_instruments_apply_fxn(dlist_element_t *elem, void *data)
{
    static int      cnt      = 0;
    status_t        status   = OSA_SOK;
    task_state_t    state;
    task_mgr_t    * tskmgr   = NULL;
    task_object_t * tsk_node = NULL;

    tsk_node = (task_object_t *)elem;
    tskmgr   = (task_mgr_t *   )data;

    status = task_get_state(tsk_node->m_task, &state);
    OSA_assert(OSA_SOK == status);

    fprintf(stdout, "    [%02d]    | [0x%08x] |     [%02d]     | [%s]\n", 
            cnt++, tsk_node->m_task, state, tsk_node->m_name);

    if (cnt >= tskmgr->m_tsk_cnt) {
        cnt = 0;
    }

    return status;
}

static status_t
__task_mgr_task_instruments(task_mgr_t *tskmgr)
{
    status_t status = OSA_SOK;

    fprintf(stdout, "\nTASK_MGR: Satatistics.\n"
                    "\n     ID     |     TASK     |     STATE    |     NAME"
                    "\n--------------------------------------------------------------------\n"
                    );

    mutex_lock(&tskmgr->m_mutex);

    status = dlist_map(&tskmgr->m_tsklists, __task_mgr_instruments_apply_fxn, (void *)tskmgr);

    mutex_unlock(&tskmgr->m_mutex);

    return status;
}

static status_t
__task_mgr_thdpool_instruments(task_mgr_t *tskmgr)
{
    return tasklist_instruments();
}

static bool
__task_mgr_find_match_fxn(dlist_element_t *elem, void *data)
{
    return (strcmp(((task_object_t *)elem)->m_name,
                   ((task_mgr_find_prm_t *)data)->m_name) == 0);
}

static status_t
__task_mgr_find(task_mgr_t *tskmgr, task_mgr_find_prm_t *prm)
{
    status_t        status   = OSA_SOK;
    task_object_t * tsk_node = NULL;

    prm->m_task = NULL;

    mutex_lock(&tskmgr->m_mutex);

    status = dlist_search_element(&tskmgr->m_tsklists, (void *)prm,
                                 (dlist_element_t **)&tsk_node, __task_mgr_find_match_fxn);
    if (!OSA_ISERROR(status)) {
        prm->m_task = tsk_node;
        status = OSA_SOK;
    } else {
        status = OSA_ENOENT;
    }

    mutex_unlock(&tskmgr->m_mutex);

    return status;
}

static status_t
__task_mgr_internal_main(void *userdata, task_t tsk, msg_t **msg)
{
	status_t	   status = OSA_SOK;
    task_state_t   state;
    task_mgr_handle hdl = NULL;
	
    hdl = (task_mgr_handle)userdata;

    OSA_assert(hdl != NULL);

    switch(msg_get_cmd((*msg)))
    {
        case TASK_CMD_INIT:
            status = task_set_state(tsk, TASK_STATE_INIT);
            break;

        case TASK_CMD_PROC:
            status = task_set_state(tsk, TASK_STATE_PROC);
            break;

        case TASK_CMD_EXIT:
            status = task_set_state(tsk, TASK_STATE_EXIT);
            break;

        /* Cmds for task manager */
        case TASK_MGR_CMD_CREATE_TASK:
            status = __task_mgr_register_tsk(hdl, (task_object_t *)msg_get_payload_ptr((*msg)));
            break;

        case TASK_MGR_CMD_START_TASK:
            status = __task_mgr_start_tsk(hdl, (task_object_t *)msg_get_payload_ptr((*msg)));
            break;

        case TASK_MGR_CMD_STOP_TASK:
            status = __task_mgr_stop_tsk(hdl, (task_object_t *)msg_get_payload_ptr((*msg)));
            break;

        case TASK_MGR_CMD_DELETE_TASK:
            status = __task_mgr_unregister_tsk(hdl, (task_object_t *)msg_get_payload_ptr((*msg)));
            break;

        case TASK_MGR_CMD_TASK_INSTRUMENTS:
            status = __task_mgr_task_instruments(hdl);
            break;

        case TASK_MGR_CMD_THDPOOL_INSTRUMENTS:
            status = __task_mgr_thdpool_instruments(hdl);
            break;

        case TASK_MGR_CMD_FIND_TASK:
            status = __task_mgr_find(hdl, (task_mgr_find_prm_t *)msg_get_payload_ptr((*msg)));
            break;

        default:
            break;
    }

	return (status);
}

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */
