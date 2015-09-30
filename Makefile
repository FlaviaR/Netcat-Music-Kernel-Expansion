obj-m += netcat.o
netcat-y = netcat_main.o
netcat-y+= tracks/trk1.o
netcat-y+= tracks/trk2.o
netcat-y+= tracks/trk3.o
netcat-y+= tracks/trk4.o
netcat-y+= tracks/trk5.o
netcat-y+= tracks/trk6.o
netcat-y+= tracks/trk7.o
netcat-y+= tracks/trk8.o
netcat-y+= tracks/trk9.o
netcat-y+= tracks/trk10.o


KERNELDIR	?=  /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
