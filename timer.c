#include <signal.h>
#include <sys/time.h>
#include "list.h"
#include "timer.h"


static struct dl_list timer_list_header;

kjnt init_timer_list(void)
{
    dl_list_list(&timer_list_header);
}

int deinit_timer_list(void)
{

}

int register_reltimer(int second, int microsecond,struct timer_hander hander,void *user_ctx)
{


}

int cancel_timer(struct timer *t)
{


}


static void alarm_entry(int sig)
{
    if ((sig != SIGALARM) ||
        dl_list_empty(timer_list_header)){
        return ;
    }




}

