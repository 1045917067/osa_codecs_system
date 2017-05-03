/** ============================================================================
 *
 *  @File name:	osa_msgq.h
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

#if !defined (__OSA_MSGQ_H)
#define __OSA_MSGQ_H

/*  --------------------- Include system headers ---------------------------- */

/*  --------------------- Include user headers   ---------------------------- */

#include "osa_msg.h"
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

#define MSGQ_INVALID_MSGQ   (~(0u))
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

typedef unsigned int msgq_t;

typedef status_t (*msgq_wait)(void *userdata, unsigned int timeout);
typedef status_t (*msgq_signal)(void *userdata);

typedef struct msgq_attrs_tag {
	msgq_wait		m_wait;
	msgq_signal		m_signal;
	void		  * m_userdata;
} msgq_attrs_t;

struct __msgq_locate_attrs_t;
typedef struct __msgq_locate_attrs_t msgq_locate_attrs_t;
struct __msgq_locate_attrs_t
{
    char            m_padding[32];
};

struct __msgq_mgr_prm_t; typedef struct __msgq_mgr_prm_t msgq_mgr_prm_t;
struct __msgq_mgr_prm_t
{
    unsigned short  m_msgq_cnt;
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

status_t msgq_mgr_init(msgq_mgr_prm_t *prm);
status_t msgq_mgr_register(const char *name, msgq_t *msgq, msgq_attrs_t *attrs);
status_t msgq_mgr_find(const char *name, msgq_t *msgq);
status_t msgq_mgr_unregister(const char *name, msgq_t msgq);
status_t msgq_mgr_deinit(void);

status_t msgq_open(const char *name, msgq_t *msgq, msgq_attrs_t *attrs);
status_t msgq_locate(const char *name, msgq_t *msgq, msgq_locate_attrs_t *attrs);
status_t msgq_release(msgq_t msgq);
status_t msgq_alloc(unsigned short size, msg_t **msg);
status_t msgq_free(unsigned short size, msg_t *msg);
status_t msgq_send(msgq_t msgq, msg_t *msg, unsigned int timeout);
status_t msgq_recv(msgq_t msgq, msg_t **msg, unsigned int timeout);
status_t msgq_get_src_queue(msg_t *msg, msgq_t *msgq);
status_t msgq_set_src_queue(msg_t *msg, msgq_t msgq);
status_t msgq_get_dst_queue(msg_t *msg, msgq_t *msgq);
status_t msgq_set_dst_queue(msg_t *msg, msgq_t msgq);
status_t msgq_count(msgq_t msgq, unsigned int *cnt);
status_t msgq_close(const char *name, msgq_t msgq);


/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__OSA_MSGQ_H) */
