## [M1: point 1]
#  Define the module name
#  ...
MODULE	 = lkp_rust

## [M2: point 1]
#  set the object name to the module variable + .o
#  ...
obj-m += $(MODULE).o

## [M3: point 1]
#  Set the kernel directory variable to where the modules are located
#  ...
KERNELDIR ?= /lib/modules/$(shell uname -r)/build

## [M4: point 1]
#  Specify the present working directory
#  ...
PWD := $(shell pwd)

## [M5: point 1]
#  Define the make all target aka default make target
#  ...
all: $(MODULE)


## [M6: point 1]
#  Compile the .c files into their respective object files using the define compiler -> CC
#  ...
%.o: %.c
	@echo "  CC      $<"
	@$(CC) -c $< -o $@

## [M7: point 1]
#  Run the make modules command speicfying the directory of the current kernel source 
#  and the absolute path to the external module location.
#  ...
$(MODULE):
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

## [M8: point 1]
#  Run the make clean command speicfying the directory of the current kernel source 
#  and the absolute path to the external module location.
#  ...
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
