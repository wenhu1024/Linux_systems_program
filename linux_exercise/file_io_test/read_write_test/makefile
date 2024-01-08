src=$(wildcard *.c)
target=$(patsubst %.c,%,$(src))

myargs=-Wall -g

ALL:$(target)

$(target):%:%.c
	gcc $< -o $@ $(myargs) 

clean:
	-rm -rf $(target)
.PHONY:clean ALL
