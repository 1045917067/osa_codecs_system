/** ============================================================================
 *
 *  @File name:	osa_mailbox.h
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

#if !defined (__OSA_MAILBOX_H)
#define __OSA_MAILBOX_H

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

#define MAILBOX_INVALID_MBX     (~(0u))

#define MAILBOX_MBX_MAX         (32)

#define MAILBOX_BROADCAST_MAX   (MAILBOX_MBX_MAX)

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

typedef unsigned int mailbox_t;

typedef	enum {
	MAILBOX_MSGQ_CMD = 0,
	MAILBOX_MSGQ_ACK = 1,
	MAILBOX_MSGQ_MAX = 2,
} mailbox_msgq_t;

struct __mailbox_params_t; typedef struct __mailbox_params_t mailbox_params_t;
struct __mailbox_params_t
{
    char            m_name[32];
};

struct __mailbox_system_prm_t;
typedef struct __mailbox_system_prm_t mailbox_system_prm_t;
struct __mailbox_system_prm_t
{
    unsigned short  m_mbx_cnt;
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

status_t mailbox_system_init(mailbox_system_prm_t *prm);
status_t mailbox_system_register(const char *name, mailbox_t *mbx);
status_t mailbox_system_unregister(const char *name, mailbox_t mbx);
status_t mailbox_system_find(const char *name, mailbox_t *mbx);
status_t mailbox_system_deinit(void);

status_t mailbox_open(mailbox_t *mbx, mailbox_params_t *prm);
status_t mailbox_recv_msg(mailbox_t mbx, msg_t **msg, msg_type_t msgt, unsigned int timeout);
status_t mailbox_send_msg(mailbox_t to, mailbox_t frm, msg_t *msg, msg_type_t msgt, unsigned int timeout);
status_t mailbox_broadcast(mailbox_t mbx_lists[], mailbox_t frm, msg_t *msg);

status_t mailbox_alloc_msg(unsigned short size, msg_t **msg);
status_t mailbox_free_msg (unsigned short size, msg_t *msg);

status_t mailbox_wait_msg(mailbox_t mbx, msg_t **msg, msg_type_t msgt);
status_t mailbox_check_msg(mailbox_t mbx, msg_t **msg, msg_type_t msgt);
status_t mailbox_get_msg_count(mailbox_t mbx, unsigned int *cnt, msg_type_t msgt);
status_t mailbox_wait_cmd(mailbox_t mbx, msg_t **msg, unsigned short cmd);
status_t mailbox_wait_ack(mailbox_t mbx, msg_t **msg, unsigned int id);
status_t mailbox_flush(mailbox_t mbx);
status_t mailbox_close(mailbox_t mbx, mailbox_params_t *prm);

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__OSA_MAILBOX_H) */
