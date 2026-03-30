# https://stackoverflow.com/questions/24975377/kvm-module-verification-failed-signature-and-or-required-key-missing-taintin
CONFIG_MODULE_SIG=n

obj-m += main.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

