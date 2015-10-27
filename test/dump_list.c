#include <stdio.h>
#include "../src/timer.h"

void timer_func1(void *data)
{
	return;	

}

void timer_func2(void *data)
{
	
	return;	

}

void timer_func3(void *data)
{
	
	return;	

}

int main(void)
{
	char s[10];
	register_reltimer(2,0,timer_func1,&s[2]);
	register_reltimer(3,0,timer_func2,&s[3]);
	register_reltimer(1,0,timer_func3,&s[1]);

	display_timer_list();

}
