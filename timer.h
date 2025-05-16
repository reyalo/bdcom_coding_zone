#ifdef SWITCH
#include <sys/types.h>
#else
#include <ip/types.h>
#endif
#include <libsys/vos/vos_types.h>

#ifdef __cplusplus
    extern "C" {
#endif

#define TIMER_RESOLUTION_MASK	0xF0000000
#define TIMER_RESOLUTION_S		0x00000000	/*in unit of 1 second*/
#define TIMER_RESOLUTION_S10	0x10000000	/*in unit of 0.1 second*/
#define TIMER_RESOLUTION_S100	0x20000000	/*in unit of 0.01 second*/

#define TIMER_MSG_METHOD			0
#define TIMER_CALLBACK_METHOD		1
#define TIMER_CALLBACK2_METHOD		2
/****************************************************************/
/*	Passive timer												*/
/*	This type of timer will neither call function back, nor 	*/
/*	send a message back. It will simply set a flag to identify  */
/*	the current state of the timer.	It will provide some 		*/
/* 	functions to investigate the status of a timer.				*/
/*	The owner of this kind of timer should be responsible for 	*/
/*	calling such functions and doing the relevant transactions  */
/*  While create such a timer, the TIMER_USER_DATA will be 		*/
/* 	of no use and will be ignored.								*/
/****************************************************************/
#define TIMER_PASSIVE				3
#define TIMER_VMSG_METHOD			4
#define TIMER_SMMSG_METHOD			5
#define TIMER_MAX_METHOD			6
#define TIMER_LOOP 					0x8000

#ifndef SYS_NOERR
#define SYS_NOERR					0
#endif
#define SYS_ERR_TIMER_TABLE_FULL	-1
#define SYS_ERR_TIMER_ID_INVALID	-2
#define SYS_ERR_TIMER_TYPE_INVALID	-3
#define SYS_ERR_TIMER_DURATION_TOO_LONG -4

#ifdef NTIMER
#define INVALID_TIMER_ID			0
#else
#define INVALID_TIMER_ID 			0xFFFFFFFF		/* 0 is also invalid*/
#endif

typedef struct _msg_data{
	MSG_Q_ID qid;/* id of msg queue*/
	ULONG	msg_buf[4];
}TIMER_MSG_DATA;

typedef struct _callback_data{
	void (*fun)(ULONG arg);
	ULONG	arg;
}TIMER_CALLBACK_DATA;

typedef struct _callback2_data{
	void (*fun)(ULONG arg1,ULONG arg2);
	ULONG	arg1;
	ULONG	arg2;
}TIMER_CALLBACK2_DATA;

typedef union _timer_user_data{
	TIMER_MSG_DATA	msg;
	TIMER_CALLBACK_DATA cb;
	TIMER_CALLBACK2_DATA cb2;
}TIMER_USER_DATA;

/********************************************************
 * routine:	sys_add_timer
 *
 * description:
 *	Add a designated timer,but not start it!!
 *  when timeout,it will send a user specified msg to the queue
 *	in case TIMER_MSG_METHOD,or callback the user supplied fun with the specified arg.
 *
 * arguments:
 *	type		TIMER_CALLBACK_METHOD,TIMER_MSG_METHOD,TIMER_CALLBACK2_METHOD
 *  user_data	ptr to TIMER_USER_DATA
 *	timer_id	ptr to timer_id ,later stop_timer() use this
 *
 * return code:
 *	SYS_NOERR						successful
 *	SYS_ERR_TIMER_TABLE_FULL		error: timer table full
 *  SYS_ERR_TIMER_TYPE_INVALID		error:timer type invalid
 * Note:
 *  1.the timeout value should be requested in units of 1 second
 *  2.must call this after task root runing.
 *	3.if use MSG_METHOD,qid is not verified during start_timer(),but at q_send it is verified.
 * !4.if use callback ,the fun must be unblock
 * !5.if you task may be deleted by others when you are calling start_timer(),
 *    this may cause timer_table dead_lock,to resolve this ,start_timers() must 
 *	  be modified ,please let me know.   --tan
 ********************************************************/
ULONG	sys_add_timer(USHORT type,TIMER_USER_DATA *user_data,ULONG *timer_id);
/********************************************************
 * routine:	sys_delete_timer
 *
 * description:
 *	delete the designated timer.
 *
 * arguments:
  *	timer_id		index to the table
 *
 * return code:
 *	SYS_NOERR						successful
 *  SYS_ERR_TIMER_ID_INVALID		error: timer_id invalid
 *
 ********************************************************/
ULONG	sys_delete_timer(ULONG timer_id);
/********************************************************
 * routine:	sys_start_timer
 *
 * description:
 *	activate the designated timer.
 *
 * arguments:
  *	timer_id		index to the table
 *	timeout			has 2 parts
 *					1.timeout&TIMER_RESOLUTION_MASK means the resolution
 *					2.timeout&(~TIMER_RESOLUTION_MASK) means the value
 * return code:
 *	SYS_NOERR						successful
 *  SYS_ERR_TIMER_ID_INVALID		error: timer_id invalid
 *
 ********************************************************/
ULONG	sys_start_timer(ULONG timer_id,ULONG timeout);
/********************************************************
 * routine:	sys_stop_timer
 *
 * description:
 *	Stop the designated timer,but not delete it!!
 *
 * arguments:
 *	timer_id		index to the table
 *
 * return code:
 *	SYS_NOERR						successful
 *  SYS_ERR_TIMER_ID_INVALID		error: timer_id invalid
 *
 ********************************************************/
ULONG	sys_stop_timer(ULONG timer_id);
/****************************************************************/
/*	routine : sys_timer_run										*/
/*	description :												*/
/*		Get the running state of the specified timer.			*/
/*	Arguments :													*/
/*		timer_id :	the id of the specified timer.				*/
/*					get from sys_add_timer						*/
/*	Returns :													*/
/*		0 :			The timer is not running.					*/
/*		non-zero:	The timer is running.						*/
/****************************************************************/
int sys_timer_run(ULONG timer_id );

#define PTIMER_RUNNING	1
#define PTIMER_EXPIRE	2
#define PTIMER_SLEEP	3
/****************************************************************/
/*	routine : sys_timer_state									*/
/*	description :												*/
/*		Get the state of the specified timer.					*/
/*	Arguments :													*/
/*		timer_id :	the id of the specified timer.				*/
/*					get from sys_add_timer						*/
/*	Returns :													*/
/*		0 :				The timer id is invalid.				*/
/*		PTIMER_RUNNING:	The timer is running.					*/
/*		PTIMER_EXPIRE:	The timer has run out of the time		*/
/*		PTIMER_SLEEP:	The timer is not started yet.			*/
/****************************************************************/
int	sys_timer_state(unsigned long timer_id);


/****************************************************************/
/*	routine : sys_read_timer									*/
/*	description :												*/
/*		Get the milliseconds remaining on this timer			*/
/*	Arguments :													*/
/*		timer_id :	the id of the specified timer.				*/
/*					get from sys_add_timer						*/
/*	Returns :													*/
/*		milliseconds remaining on this timer					*/
/****************************************************************/
unsigned long sys_read_timer(unsigned long timer_id);

/********************************************************
 * routine:	sys_set_timer
 *
 * description:
 *	set the timer. only worked when timer is TIMER_STOP state
 *
 * arguments:
 *	type		TIMER_CALLBACK_METHOD,TIMER_CALLBACK2_METHOD,TIMER_MSG_METHOD
 *  user_data	ptr to TIMER_USER_DATA
 *	timer_id	timer_id ,later start_timer() use this
 *
 * return code:
 *	SYS_NOERR						successful
 *	SYS_ERR_TIMER_ID_INVALID		error:timer id invalid
 *  SYS_ERR_TIMER_TYPE_INVALID		error:timer type invalid
 *  1.the timeout value should be requested in units of 1 second
 *  2.must call this after task root runing.
 *	3.if use MSG_METHOD,qid is not verified during start_timer(),but at q_send it is verified.
 * !4.if use callback ,the fun must be unblock
 * !5.can't call from ISR's context
 ********************************************************/
ULONG	sys_set_timer(ULONG timer_id, USHORT type_in, TIMER_USER_DATA *user_data);
void timer_main(void);

void sys_get_current_timeStr(char *str);

#ifdef __cplusplus
    }
#endif

#endif /*_LIBSYS_TIMER_H*/
