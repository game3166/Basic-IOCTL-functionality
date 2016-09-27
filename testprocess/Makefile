obj-m := test.o

COMPATH = /lib/modules/3.2.0-29-generic-pae/build M=$(PWD)  
#COMPATH = /lib/modules/2.6.32.70/build M=$(PWD)

all:
	make -C $(COMPATH) modules
clean:
	make -C $(COMPATH) clean
