#lpc2388 makefile template
#Copyright (C) 2022 Kumohakase
#Please consider supporting me through kofi.com https://ko-fi.com/kumohakase

CC=arm-none-eabi-gcc
CFLAGS=-mcpu=arm7tdmi-s -marm -I . -g3
OBJS=main.o start.o

all: $(OBJS)
	$(CC) -nostartfiles -T flash.ld -mcpu=arm7tdmi-s -marm $^
	arm-none-eabi-objcopy -O ihex a.out a.hex
	arm-none-eabi-objdump -hdS > report

clean:
	rm -f $(OBJS) a.out a.hex report
