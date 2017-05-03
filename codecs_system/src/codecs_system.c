/** ============================================================================
 *
 *  @File name:	codecs_system.c
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

#include "codecs_system.h"

#include "osa_status.h"
#include "osa_task_mgr.h"
#include "osa_debugger.h"
#include "dlist.h"
#include "mutex.h"
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

#define CODECS_SYSTEM_TASK_NUM_MAX  (32)

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

enum __codecs_system_task_t
{
    CODECS_SYSTEM_TASK_PLATFORM	= 0,
    CODECS_SYSTEM_TASK_MAX		= 4,
};

struct __codecs_system_object_t
{
    codecs_system_prm_t     m_params;
    task_mgr_prm_t          m_tsk_mgr_prm;

    unsigned int            m_tsk_cnt;
    unsigned int            m_cur_cnt;

    dlist_t                 m_cmdlists;
    
	mutex_t                 m_mutex;
    task_object_t         * m_tsklists[CODECS_SYSTEM_TASK_MAX];

    unsigned int            m_codecs_initialized;
};

typedef struct __codecs_system_object_t   codecs_system_object_t;
typedef struct __codecs_system_object_t * codecs_system_handle;

/*
 *  --------------------- Global variable definition ---------------------------
 */

/** ----------------------------------------------------------------------------
 *  @Name:          Variable name
 *
 *  @Description:   Description of the variable.
 * -----------------------------------------------------------------------------
 */

static codecs_system_object_t   glb_codecs_system_obj;

static const char * const GT_NAME = "codecs system";

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

int codecs_system_env_init(unsigned int level, const char *folder)
{
    int                    i;
    status_t               status = OSA_SOK;
    codecs_system_handle   hdl    = &glb_codecs_system_obj;

    memset(hdl, 0, sizeof(*hdl));

    /*
     *  Initialize debug module.
     */
    osa_debugger_prm_t debug_prm;

    debug_prm.m_debug_level = level;
    debug_prm.m_out = stderr;
    debug_prm.m_name = "codecs_main";
    debug_prm.m_folder = (unsigned char *)folder;

    osa_debugger_init(&debug_prm);
    osa_debugger_setlevel(level);

    /*
     *  Initialize tasklist.
     */
    hdl->m_tsk_mgr_prm.m_msg_cnt = CODECS_SYSTEM_TASK_NUM_MAX;
    hdl->m_tsk_mgr_prm.m_tsk_cnt = CODECS_SYSTEM_TASK_NUM_MAX;
    status |= task_mgr_init(&hdl->m_tsk_mgr_prm);
    DBG(DBG_ERROR, GT_NAME, "codecs_system_env_init: Initiailzed task manager.\n");

    /*
     *  Create codecs system tasks.
     */
    status |= mutex_create(&hdl->m_mutex);


	hdl->m_tsk_cnt = CODECS_SYSTEM_TASK_MAX;
    hdl->m_cur_cnt = 0;
    

    OSA_assert(OSA_SOK == status);

    return status;
}

int codecs_system_params_init(const codecs_system_prm_t *prm)
{
    status_t status = OSA_SOK;
	codecs_system_handle   hdl    = &glb_codecs_system_obj;
	memcpy(&hdl->m_params, prm, sizeof(*prm));


    return status;
}

int codecs_system_init  (void)
{
    int i;
    status_t               status = OSA_SOK;
    codecs_system_handle   hdl    = &glb_codecs_system_obj;

    for (i = 0; i < hdl->m_cur_cnt; i++) {
        status = task_mgr_register(hdl->m_tsklists[i]); 

        OSA_assert(OSA_SOK == status);
    }


    return status;
}

int codecs_system_start (void)
{
    status_t               status = OSA_SOK;
    codecs_system_handle   hdl    = &glb_codecs_system_obj;


    return status;
}

int codecs_system_stop  (void)
{
    return OSA_SOK;
}

int codecs_system_deinit(void)
{
    int i;
    status_t               status = OSA_SOK;
    codecs_system_handle   hdl    = &glb_codecs_system_obj;


    for (i = 0; i < hdl->m_cur_cnt; i++) {
        status |= task_mgr_unregister(hdl->m_tsklists[i]);
    }

    return status;
}

int codecs_system_control(unsigned int cmd, void *prm, size_t size, void *ud)
{
    status_t               status = OSA_SOK;
    codecs_system_handle   hdl    = &glb_codecs_system_obj;
    task_object_t        * tsk    = NULL;


    mutex_lock(&hdl->m_mutex);

    switch (cmd)
    {
        /* Cmds for system */
    
        default:
            status = OSA_EFAIL;
            break;
    }

    mutex_unlock(&hdl->m_mutex);

    return status;
}

int codecs_system_env_deinit (void)
{
    int                    i;
    status_t               status = OSA_SOK;
    codecs_system_handle   hdl    = &glb_codecs_system_obj;

    status |= mutex_delete(&hdl->m_mutex);

    /*
     *  Finialize tasklist.
     */
    status |= task_mgr_deinit();
    DBG(DBG_ERROR, GT_NAME, "codecs_system_env_deinit: De-initialized task manager.\n");

    /*
     *  Finalize debug module.
     */
    DBG(DBG_ERROR, GT_NAME, "codecs_system_env_deinit: De-initialized debug module.\n");
    osa_debugger_deinit();

    OSA_assert(OSA_SOK == status);

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
