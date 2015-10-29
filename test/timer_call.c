#include <stdio.h>
#include "../src/timer.h"

char *str1 = "hello";
char *str2 = "hi";
char *str3 = "bye";

void say_word(void *data){

	struct timeval now;

	get_time(&now);

	printf("[%u:%u] %s\n",now.tv_sec,now.tv_usec,(char *)data);
	return 0;
}

int main(int argc,char **argv)
{
	struct timeval now;
	struct timer *tr1,*tr2,*tr3;
	get_time(&now);
	printf("[%u:%u] register timer\n",now.tv_sec,now.tv_usec);
	tr3 = register_reltimer(3, 0,say_word,str3);
	tr2 = register_reltimer(2, 0,say_word,str2);
	tr1 = register_reltimer(1,0,say_word,str1);

	cancel_timer(tr2);

	sleep(2);
	sleep(2);
	sleep(2);

	return 0;
}
