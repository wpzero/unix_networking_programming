/* include signal */
#include	"unp.h"

Sigfunc *
signal(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
/* 这个signal通常用于IO时间过期的中断 */
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
/* 这个和SA_RESTART相反 */
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
/* When the flag is set, a system call interrupted by this */
/* signal will be automatically restarted by the kernel */
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}
/* end signal */

Sigfunc *
Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
	Sigfunc	*sigfunc;

	if ( (sigfunc = signal(signo, func)) == SIG_ERR)
		err_sys("signal error");
	return(sigfunc);
}
