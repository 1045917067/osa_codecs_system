/** ============================================================================
 *
 *  @File name:	osa.h
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

#if !defined (__OSA_H)
#define __OSA_H

/*  --------------------- Include system headers ---------------------------- */

#include <stdio.h>
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

#define OSA_memAlloc(size, ptr) (mem_alloc((size), NULL, ((void **)(ptr))))

#define OSA_memFree(size, ptr)  (mem_free((size), NULL, ((void *)(ptr))))

#ifdef  TRUE
#undef  TRUE
#endif
#define TRUE    (1)

#ifdef  FALSE
#undef  FALSE
#endif
#define FALSE   (0)

#define OSA_ARRAYSIZE(array)    ((sizeof(array) / sizeof((array)[0])))

#define OSA_TIMEOUT_NONE        (0)
#define OSA_TIMEOUT_FOREVER     (~(0u))

#define OSA_assert(x)  \
{ \
  if( (x) == 0) { \
    fprintf(stderr, " ASSERT (%s|%s|%d)\r\n", __FILE__, __func__, __LINE__); \
    while (getchar()!='q')  \
      ; \
  } \
}

/*
 *  --------------------- Data type definition ---------------------------------
 */

typedef int (*Fxn)();

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

/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */

#endif  /* if !defined (__OSA_H) */
