FLAGS+=-Wall -Werror
LDLIBS+= -lpthread -lprussdrv

all: steering_pru.bin steering_pru

clean: rm -f steering_pru *.o *.bin

steering_pru.bin: steering_pru.p
	pasm -b $^

steering_pru: steering_pru.o
