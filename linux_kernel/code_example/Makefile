obj-m := chardev.o
KERNELBUILD :=/lib/modules/$(shell uname -r)/build
all:
	g++ chardev_test.cpp -o test
	make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	rm -rf *.o *.ko *.mod.c .*.cmd *.markers *.order *.symvers .tmp_versions