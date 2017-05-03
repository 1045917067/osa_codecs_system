/** ============================================================================
 *
 *  @File name:	codecs_main.c
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

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <linux/watchdog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/*  --------------------- Include user headers   ---------------------------- */

#include "codecs_main.h"
#include "codecs_system.h"
#include "osa_debugger.h"
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


#define VALID_OPTIONS                          ("sdl:h")

#define CODECS_LOCK_FILE                       ("/var/lock/codecs_main.pid")
#define CODECS_LOCK_MODE                       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define CODECS_LOG_DIR                         ("/home/root/ved_app/codecs_system/log")

#define CODECS_WORKMODE_DEBUG                  (0)

#define CODECS_WORKMODE_DAEMON                 (1)

#define CODECS_WORK_MODE_IS_DAEMON(mode)       ((mode) == CODECS_WORKMODE_DAEMON)


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

/*
 *  --------------------- Global variable definition ---------------------------
 */

/** ----------------------------------------------------------------------------
 *  @Name:          Variable name
 *
 *  @Description:   Description of the variable.
 * -----------------------------------------------------------------------------
 */

codecs_object_t        glb_codecs_obj;

static unsigned int glb_codecs_exit = 0;

static const char * const GT_NAME = "codecs_main";

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

static status_t __codecs_daemon_opts_process(codecs_opts_t *popts, int argc, char *argv[]);

static status_t __codecs_daemon_daemonize(void);

static int      __codecs_daemon_already_running(void);

static status_t __codecs_daemon_main(codecs_object_t *pobj);

static status_t __codecs_daemon_init(codecs_object_t *pobj);

static status_t __codecs_daemon_start(codecs_object_t *pobj);

static status_t __codecs_daemon_run(codecs_object_t *pobj);

static status_t __codecs_daemon_stop(codecs_object_t *pobj);

static status_t __codecs_daemon_exit(codecs_object_t *pobj);

static status_t __codecs_daemon_config_init(codecs_object_t *pobj);

static status_t
__codecs_daemon_do_process(codecs_object_t *pobj, task_t tsk, msg_t *msg);

static status_t
__codecs_daemon_codecs_system_init(codecs_object_t *pobj);

static status_t
__codecs_daemon_codecs_system_deinit(codecs_object_t *pobj);

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

int main(int argc, char *argv[])
{
    status_t status;

    /* process command line options */
    status = __codecs_daemon_opts_process(&glb_codecs_obj.m_codecs_opts, argc, argv);
    if(OSA_ISERROR(status)) {
        exit(EXIT_FAILURE);
    }

    if (CODECS_WORK_MODE_IS_DAEMON(glb_codecs_obj.m_codecs_opts.m_work_mode)) {
        /* The program run in daemon mode */
        status = __codecs_daemon_daemonize();

        OSA_assert(OSA_SOK == status);
    }

    /* Check program whether is running */
    if(__codecs_daemon_already_running()) {
        fprintf(stderr, "codecs main: codecs main is already running, exit.\n");
        exit(EXIT_FAILURE);
    }

    status = codecs_system_env_init(glb_codecs_obj.m_codecs_opts.m_log_level, CODECS_LOG_DIR);
    
	OSA_assert(OSA_SOK == status);
    
	status = __codecs_daemon_main(&glb_codecs_obj);

    codecs_system_env_deinit();

    if (glb_codecs_obj.m_codecs_need_reboot) {
        /* Write buffer cache to flash */
        sync();

        /* Reboot codecs system right now */
        system("/sbin/reboot -d -f");

        sleep(1);
    }

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

static void __codecs_daemon_main_usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s [option...] [argument...]\n"
                    "\n"
                    "  -s      Program run in daemon mode\n"
                    "  -d      Program run in debug  mode\n"
                    "  -l      The global debug level[1-5](default is 2)\n"
                    "\n",
                    argv[0]
                    );
}

int __codecs_daemon_lockfile(int fd)
{
    struct  flock   fl;

    fprintf(stderr, "__codecs_daemon_lockfile: Enter.\n");

    fl.l_type   = F_WRLCK;
    fl.l_start  = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len    = 0;

    fprintf(stderr, "__codecs_daemon_lockfile: Leave.\n");

    return (fcntl(fd, F_SETLK, &fl));
}

static int __codecs_daemon_already_running(void)
{
    int         fd;
    char        buf[16];

    fprintf(stderr, "__codecs_daemon_already_running: Enter.\n");

    fd = open(CODECS_LOCK_FILE, O_RDWR | O_CREAT, CODECS_LOCK_MODE);
    if(fd < 0) {
        fprintf(stderr, "__codecs_daemon_already_running: Can't open %s.\n", CODECS_LOCK_FILE);
        exit(EXIT_FAILURE);
    }

    if(__codecs_daemon_lockfile(fd) < 0) {
        if(errno == EACCES || errno == EAGAIN) {
            close(fd);
            return (1);
        }
        fprintf(stderr, "__codecs_daemon_already_running, program exit.\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);

    fprintf(stderr, "__codecs_daemon_already_running: Leave.\n");

    return (0);
}

static status_t __codecs_daemon_opts_process(codecs_opts_t *popts, int argc, char *argv[])
{
    int opt;
    status_t status = OSA_SOK;

    fprintf(stderr, "codecs opts: cmd opts enter.\n");

    /* Check the arguments passed in */
    if(popts == NULL) {
        fprintf(stderr, "codecs opts: Invalid arguments.\n");
        return OSA_EARGS;
    }

    /* Set the configuration to default vale */
    popts->m_log_level = DBG_INFO;
    popts->m_work_mode = CODECS_WORKMODE_DEBUG;

    /* Process the command line arguments */
    while((opt = getopt(argc, argv, VALID_OPTIONS)) != -1) {
        switch(opt)
        {
            case 's':
                /* Specified the daemon mode */
                popts->m_work_mode = CODECS_WORKMODE_DAEMON;
                break;

            case 'd':
                /* Specified the debug mode */
                popts->m_work_mode = CODECS_WORKMODE_DEBUG;
                break;

            case 'l':
                /* Specified the global debug level */
                popts->m_log_level = atoi(optarg);
                break;

            case 'h':
                /* Print codecs usage */
                __codecs_daemon_main_usage(argc, argv);
                status = OSA_EFAIL;
                break;

            default:/* ? */
                __codecs_daemon_main_usage(argc, argv);
                status = OSA_EFAIL;
                break;
        }
    }

    return status;
}

static status_t __codecs_daemon_daemonize(void)
{
    int                 i, fd0, fd1, fd2;
    pid_t               pid;
    struct rlimit       rl;
    struct sigaction    sa;

    fprintf(stderr, "codecs main: daemonized enter.\n");

    /* Clear the file creattion mask */
    fprintf(stderr, "codecs main: begin to set mask.\n");
    umask(0);

    /* Get the maximum number of file descriptors */
    if(getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        fprintf(stderr, "codecs main: can't get file limit.\n");
        return OSA_EFAIL;
    }

    /* Become a session leader to lose controlling TTY */
    if ((pid = fork()) < 0) {
        fprintf(stderr, "codecs main: can't fork.\n");
        return OSA_EFAIL;
    } else if (pid != 0) {   /* Parent */
        exit(EXIT_SUCCESS);
    }
    fprintf(stderr, "codecs main: begin to set session.\n");
    setsid();

    /* Ensure future opens won't allocate controlling TTY */
    fprintf(stderr, "codecs main: begin to ignore sighup.\n");
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    if(sigaction(SIGHUP, &sa, NULL) < 0) {
        fprintf(stderr, "codecs main: can't ignore SIGHUP.\n");
        return OSA_EFAIL;
    }
    fprintf(stderr, "codecs main: begin to fork 2.\n");
    if((pid = fork()) < 0) {
        fprintf(stderr, "codecs main: can't fork.\n");
        return OSA_EFAIL;
    } else if(pid != 0) {   /* Parent */
        fprintf(stderr, "codecs main: parents exit.\n");
        exit(EXIT_SUCCESS);
    }
    fprintf(stderr, "codecs main: child process go.\n");

    /* Change working directory to root */
    fprintf(stderr, "codecs main: begin to set working directory.\n");
    
	if(chdir("/") < 0) {
        fprintf(stderr, "codecs main: can't change directory to \"/\".\n");
        exit(EXIT_FAILURE);
    }
    
	fprintf(stderr, "codecs main: begin to close all fd.\n");
    /* Close all files descriptors */
    if(rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for(i = 0; i < rl.rlim_max; i++)
        close(i);

    /* Attach file descriptors 0, 1, 2 to /dev/null */
    fprintf(stderr, "codecs main: begin to open /dev/null.\n");
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    fprintf(stderr, "codecs main: daemonized leave.\n");

    return OSA_SOK;
}



static status_t __codecs_daemon_main(codecs_object_t *pobj)
{
    status_t status = OSA_SOK;

    status = __codecs_daemon_init(pobj);
    if (OSA_ISERROR(status)) {
        return status;
    }

    status = __codecs_daemon_start(pobj);
    if (OSA_ISERROR(status)) {
        return status;
    }

    status = __codecs_daemon_run(pobj);
    if (OSA_ISERROR(status)) {
        return status;
    }

    status |= __codecs_daemon_stop(pobj);
	if (OSA_ISERROR(status)) {
		return status;
	}

    status |= __codecs_daemon_exit(pobj);
	if (OSA_ISERROR(status)) {
		return status;
	}

    return status;
}


static status_t __codecs_daemon_init(codecs_object_t *pobj)
{
    int i;
    status_t status = OSA_SOK;

    memset(pobj, 0, sizeof(*pobj));

    DBG(DBG_INFO, GT_NAME, "codecs main initializing ...\n");
    
	OSA_assert(OSA_SOK == status);

    /* Load codecs system parameters and initialize codecs configuration */
    status = __codecs_daemon_config_init(pobj);

	snprintf((char *)pobj->m_codecs_tsk_name, sizeof(pobj->m_codecs_tsk_name), "%s", "CODECS_MAIN_TSK");
    pobj->m_codecs_tsk_obj.m_name = (unsigned char *)pobj->m_codecs_tsk_name;
    pobj->m_codecs_tsk_obj.m_main = NULL;
    pobj->m_codecs_tsk_obj.m_find = NULL;
    pobj->m_codecs_tsk_obj.m_pri = 0;
    pobj->m_codecs_tsk_obj.m_stack_size = 0;
    pobj->m_codecs_tsk_obj.m_init_state = 0;
    pobj->m_codecs_tsk_obj.m_userdata = (void *)pobj;
    pobj->m_codecs_tsk_obj.m_task = TASK_INVALID_TSK;

    status = task_mgr_register(&pobj->m_codecs_tsk_obj);

    DBG(DBG_INFO, GT_NAME, "codecs main initialized.\n");
    
	return status;
}

static status_t __codecs_daemon_start(codecs_object_t *pobj)
{
    status_t status = OSA_SOK;

    DBG(DBG_INFO, GT_NAME, "codecs main starting ...\n");

    status = __codecs_daemon_codecs_system_init(pobj);
    
    return status;
}

static status_t __codecs_daemon_run(codecs_object_t *pobj)
{
    msg_t *msg = NULL;
    status_t status   = OSA_SOK;
    task_state_t state = TASK_STATE_PROC;

	DBG(DBG_INFO, GT_NAME, "codecs main started.\n");
    
	while (!glb_codecs_exit) {

        status = task_wait_msg(pobj->m_codecs_tsk_obj.m_task, &msg, MSG_TYPE_CMD);

        if (OSA_ISERROR(status)) {
            continue;
        }

        status |= __codecs_daemon_do_process(pobj, pobj->m_codecs_tsk_obj.m_task, msg);

        status |= task_ack_free_msg(pobj->m_codecs_tsk_obj.m_task, msg);

        if (pobj->m_codecs_need_reboot) {
            break;
        }

        if (!OSA_ISERROR(task_get_state(pobj->m_codecs_tsk_obj.m_task, &state))
                && state == TASK_STATE_EXIT) {
            break;
        }
    }

    return status;
}

static status_t __codecs_daemon_stop(codecs_object_t *pobj)
{
    status_t status = OSA_SOK;

    if (pobj->m_codecs_initialized) {
        status |= __codecs_daemon_codecs_system_deinit(pobj);
    }

    return status;
}

static status_t __codecs_daemon_exit(codecs_object_t *pobj)
{
    status_t status = OSA_SOK;

    DBG(DBG_INFO, GT_NAME, "codecs main exiting ...\n");

	/*
	 *  Unregister codecs task.
     */
    DBG(DBG_INFO, GT_NAME, "unregister codecs task.\n");
    
	status |= task_mgr_unregister(&pobj->m_codecs_tsk_obj);

    DBG(DBG_INFO, GT_NAME, "codecs main exit.\n");
    
	return status;
}


static status_t __codecs_daemon_config_init(codecs_object_t *pobj)
{
    status_t status = OSA_SOK;


    return status;
}


static status_t
__codecs_daemon_do_process(codecs_object_t *pobj, task_t tsk, msg_t *msg)
{
    int cmd = 0;
    status_t status = OSA_SOK;

	void *          prm      = NULL;
	void *          ud       = NULL;
	unsigned int    prm_size = 0;

    switch(msg_get_cmd(msg))
    {
        case TASK_CMD_EXIT:
            status |= task_set_state(tsk, TASK_STATE_EXIT);
            break;

        default:
            break;
    }

    msg_set_status(msg, status);

    return status;
}

static status_t
__codecs_daemon_codecs_system_init(codecs_object_t *pobj)
{
    status_t status = OSA_SOK;

    DBG(DBG_INFO, GT_NAME, "Initialized codecs system: Enter.\n");

    if (pobj->m_codecs_initialized) {
        return status;
    }

    status = codecs_system_params_init(&pobj->m_codecs_ed_prm);
    
    OSA_assert(OSA_SOK == status);

    status = codecs_system_init();
    if (OSA_ISERROR(status)) {
        return status;
    }

    status = codecs_system_start();
    if (OSA_ISERROR(status)) {
        return status;
    }

    pobj->m_codecs_initialized = TRUE;

    return status;
}

static status_t
__codecs_daemon_codecs_system_deinit(codecs_object_t *pobj)
{
    status_t status = OSA_SOK;

    if (!pobj->m_codecs_initialized) {
        return status;
    }

    status = codecs_system_stop();

    OSA_assert(OSA_SOK == status);

    status = codecs_system_deinit();

    pobj->m_codecs_initialized = FALSE;

    return status;
}


/* C linkage specifiers for C++ declarations. */
#if defined(__cplusplus)
}
#endif  /* defined(__cplusplus) */
