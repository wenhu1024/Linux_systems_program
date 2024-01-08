src=$(wildcard ./src/*.c) 
obj=$(patsubst ./src/%.c,./obj/%.o,$(src))

ALL:a.out

myargs=-Wall -g
mypath=./inc

a.out:$(obj)
	gcc $^ -o $@ $(myargs)
$(obj):./obj/%.o:./src/%.c
	gcc -c $< -o $@ $(myargs) -I $(mypath)
clean:
	-rm -rf $(obj) a.out

.PHONY: clean ALL
