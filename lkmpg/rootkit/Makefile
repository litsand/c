obj-m	:= syscall3.o
obj-m	:= syscall2.o
obj-m	:= syscall.o
KDIR    := /lib/modules/$(shell uname -r)/build
PWD    := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
