CC = gcc
AR = ar

CFLAGS = -O0 -g -Wall -std=gnu99

OBJS =  display_transfer_client.o 

run_transfer:$(OBJS)
	$(CC) -o run_transfer $(OBJS)

$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -frv  $(OBJS)
