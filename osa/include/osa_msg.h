/** ============================================================================
 *
 *  @File name:	osa_msg.h
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

#if !defined (__OSA_MSG_H)
#define __OSA_MSG_H

/*  --------------------- Include system headers ---------------------------- */

#include <string.h>
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

#define	msg_get_src(msg)				(((msg_t *)(msg))->u.m_std_msg.m_reserved1)
#define	msg_get_dst(msg)				(((msg_t *)(msg))->u.m_std_msg.m_reserved2)
#define	msg_get_payload_ptr(msg) 		(((msg_t *)(msg))->u.m_std_msg.m_prm)
#define	msg_get_payload_size(msg)		(((msg_t *)(msg))->u.m_std_msg.m_size)
#define	msg_get_cmd(msg)				(((msg_t *)(msg))->u.m_std_msg.m_cmd)
#define	msg_get_flags(msg)				(((msg_t *)(msg))->u.m_std_msg.m_flags)
#define	msg_get_status(msg)				(((msg_t *)(msg))->u.m_std_msg.m_status)
#define msg_get_msg_size(msg)           (((msg_t *)(msg))->u.m_std_msg.m_msg_size)
#define msg_get_msg_id(msg)             (((msg_t *)(msg))->u.m_std_msg.m_msg_id)

#define	msg_set_src(msg, src)			(((msg_t *)(msg))->u.m_std_msg.m_reserved1 = (src))
#define	msg_set_dst(msg, dst)			(((msg_t *)(msg))->u.m_std_msg.m_reserved2 = (dst))
#define	msg_set_payload_ptr(msg, pt) 	(((msg_t *)(msg))->u.m_std_msg.m_prm = (pt))
#define	msg_set_payload_size(msg, sz)	(((msg_t *)(msg))->u.m_std_msg.m_size = (sz))
#define	msg_set_cmd(msg, cmd)			(((msg_t *)(msg))->u.m_std_msg.m_cmd = (cmd))
#define	msg_set_flags(msg, flag)		(((msg_t *)(msg))->u.m_std_msg.m_flags |= (flag))
#define	msg_set_status(msg, rt)			(((msg_t *)(msg))->u.m_std_msg.m_status = (rt))
#define msg_set_msg_size(msg, size)     (((msg_t *)(msg))->u.m_std_msg.m_msg_size = (size))
#define msg_set_msg_id(msg, id)         (((msg_t *)(msg))->u.m_std_msg.m_msg_id = (id))

#define	msg_clear_flags(msg, flag)		(((msg_t *)(msg))->u.m_std_msg.m_flags &= ~(flag))

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

struct __std_msg_t; typedef struct __std_msg_t std_msg_t;
struct __std_msg_t {
    unsigned int	    m_reserved[2];
    unsigned int	    m_reserved1;
    unsigned int	    m_reserved2;
    void              * m_prm;
    unsigned int	    m_size;
    unsigned short	    m_cmd;
    unsigned short	    m_flags;
    unsigned   int	    m_status;
    unsigned int        m_msg_size;
    unsigned int        m_msg_id;

    unsigned char       __pading[24];
};

struct __msgq_msg_t; typedef struct __msgq_msg_t msgq_msg_t;
struct __msgq_msg_t
{
    struct __std_msg_t  m_msg;
};

struct __mbx_msg_t; typedef struct __mbx_msg_t mbx_msg_t;
struct __mbx_msg_t
{
    struct __msgq_msg_t m_header;
    unsigned int        m_to;
    unsigned int        m_frm;
};

struct __tsk_msg_t; typedef struct __tsk_msg_t tsk_msg_t;
struct __tsk_msg_t
{
    struct __mbx_msg_t  m_header;
    unsigned int        m_to;
    unsigned int        m_frm;
};

struct __msg_t; typedef struct __msg_t msg_t;
struct __msg_t
{
    union {
        std_msg_t       m_std_msg;
        msgq_msg_t      m_msgq_msg;
        mbx_msg_t       m_mbx_msg;
        tsk_msg_t       m_tsk_msg;
    } u;
};

typedef enum msg_type_t {
    MSG_TYPE_CMD = 0,
    MSG_TYPE_ACK = 1,
    MSG_TYPE_MAX = 2,
} msg_type_t;

enum __msg_flags_t; typedef enum __msg_flags_t msg_flags_t;
enum __msg_flags_t
{
    MSG_FLAGS_WAIT_ACK = 1 << 0,
    MSG_FLAGS_FREE_PRM = 1 << 1,
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

static inline void msg_init(msg_t *msg)
{
    memset(msg, 0, sizeof(*msg));
}

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__OSA_MSG_H) */
