#CC           = avr-gcc
#CFLAGS       = -Wall -mmcu=atmega16 -Os -Wl,-Map,test.map
#OBJCOPY      = avr-objcopy
CC           = gcc
CCPP         = g++
LD           = gcc
AR           = ar
ARFLAGS      = rcs
CFLAGS       = -Wall -Os -c
LDFLAGS      = -Wall -Os -Wl,-Map,set1.map
ifdef AES192
CFLAGS += -DAES192=1
endif
ifdef AES256
CFLAGS += -DAES256=1
endif

OBJCOPYFLAGS = -j .text -O ihex
OBJCOPY      = objcopy

# include path to AVR library
INCLUDE_PATH = /usr/lib/avr/include
# splint static check
SPLINT       = splint set1.cpp aes.c -I$(INCLUDE_PATH) +charindex -unrecog

default: set2.elf

.SILENT:
.PHONY:  lint clean

set1.o : set1.cpp aes.hpp aes.o cryptoutils.hpp cryptoutils.o
	echo [CCPP] $@ $(CFLAGS)
	$(CCPP) $(CFLAGS) -o  $@ $<

set1.elf : aes.o cryptoutils.o set1.o
	echo [LD] $@
	$(CCPP) $(LDFLAGS) -o $@ $^

set2.o : set2.cpp aes.hpp aes.o cryptoutils.hpp cryptoutils.o
	echo [CCPP] $@ $(CFLAGS)
	$(CCPP) $(CFLAGS) -o  $@ $<

set2.elf : aes.o cryptoutils.o set2.o
	echo [LD] $@
	$(CCPP) $(LDFLAGS) -o $@ $^

aes.o : aes.c aes.h
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o $@ $<

cryptoutils.o : cryptoutils.cpp cryptoutils.hpp
	echo [CCPP] $@ $(CFLAGS)
	$(CCPP) $(CFLAGS) -o $@ $<

clean:
	rm -f *.OBJ *.LST *.o *.gch *.out *.hex *.map *.elf *.a

all:
	make clean && make set1.elf && make set2.elf

set1:
	make set1.elf && ./set1.elf && make clean

set2:
	make set2.elf && ./set2.elf && make clean

lint:
	$(call SPLINT)
