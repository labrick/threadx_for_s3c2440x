#ifndef _THREAD_H_
#define _THREAD_H_

//#include <signal.h>
//#include <stdio.h>
//#include <time.h>
//#include <setjmp.h>
#define _JBLEN 23

typedef	int jmp_buf[_JBLEN];

#define _SIGMAX  100

#if defined(SOLARIS20) && !defined(_SIGMAX)
#define _SIGMAX MAXSIG
#endif

#if defined(_WIN32) && !defined(_SIGMAX)
#define _SIGMAX (NSIG-1)
#endif

typedef void (*SignalHandler)(int);
struct tm {
	/*
	 * the number of seconds after the minute, normally in the range
	 * 0 to 59, but can be up to 60 to allow for leap seconds
	 */
	int tm_sec;
	/* the number of minutes after the hour, in the range 0 to 59*/
	int tm_min;
	/* the number of hours past midnight, in the range 0 to 23 */
	int tm_hour;
	/* the day of the month, in the range 1 to 31 */
	int tm_mday;
	/* the number of months since January, in the range 0 to 11 */
	int tm_mon;
	/* the number of years since 1900 */
	long tm_year;
	/* the number of days since Sunday, in the range 0 to 6 */
	int tm_wday;
	/* the number of days since January 1, in the range 0 to 365 */
	int tm_yday;
};

typedef struct
{
	int			Errno;
	SignalHandler 		SignalHandlers[_SIGMAX];
//	char			tmpnam_space[L_tmpnam];
	char			tmpnam_space[256];

	char			asctime_buff[30];
	char			*strtok_saved_pos;
	struct tm		gmtime_temp;
	/* C++ pointer for exception handling */
	void 			*__eh_globals;
} ThreadLocalStorage;

#ifdef use__ghs_threadlocalstorage
#define GetThreadLocalStorage() ((ThreadLocalStorage *)__ghs_threadlocalstorage)
#else
ThreadLocalStorage *GetThreadLocalStorage(void);
#endif

void __ghsLock(void);
void __ghsUnlock(void);
#ifndef EMBEDDED
#if 0
__inline void __ghsLock(void) { }
__inline void __ghsUnlock(void) { }
#endif
#endif

int  __ghs_SaveSignalContext(jmp_buf);
void __ghs_RestoreSignalContext(jmp_buf);

/* macros used in stdio library source */
#ifdef __ghs_thread_safe
# define LOCKFILE(f)	flockfile(f);
# define TRYLOCKFILE(f)	ftrylockfile(f);
# define UNLOCKFILE(f)	funlockfile(f);
# define LOCKCREATE(f)	flockcreate(f);
# define LOCKCLEANUP(f)	flockdestroy(f);
/* prototypes for FILE lock routines (not in POSIX API) */
void __ghs_flock_file(void *);
void __ghs_funlock_file(void *);
int __ghs_ftrylock_file(void *);
void __ghs_flock_create(void **);
void __ghs_flock_destroy(void *);
/* End New */
#else
# define LOCKFILE(f)
# define TRYLOCKFILE(f)	-1;	/* no lock obtained */
# define UNLOCKFILE(f)
# define LOCKCREATE(f)	
# define LOCKCLEANUP(f)	
#endif

#pragma weak signal_init
void signal_init(ThreadLocalStorage *);
#pragma weak iob_init
extern void iob_init(void);
#pragma weak error_init
extern void error_init(void);
#pragma weak lock_init
extern void lock_init(void);
#pragma weak __cpp_exception_init
extern void __cpp_exception_init(void **);
#pragma weak __cpp_exception_cleanup
extern void __cpp_exception_cleanup(void **);

#endif /* _THREAD_H_ */
