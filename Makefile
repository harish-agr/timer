PROGS = dump_list del_timer timer_call

TIMER_SRC = src/timer.c 

all: $(TIMER_SRC) $(PROGS)

dump_list: test/dump_list.c
	$(CC) $(TIMER_SRC) $< -o test/dump_list

del_timer: test/del_timer.c
	$(CC) $(TIMER_SRC) $< -o test/del_timer

timer_call: test/timer_call.c
	$(CC) $(TIMER_SRC) $< -o test/timer_call

clean:
	
