#
# Makefile for the input core drivers.
#

# Each configuration option enables a list of files.
obj-y += fts_ts.o 
fts_ts-y += focaltech_core.o focaltech_ctl.o focaltech_ex_fun.o focaltech_gesture.o focaltech_flash.o  
extlibs :=$(PWD)/drivers/input/touchscreen/FT7811/ft_gesture_32bit_lib_v1.0_20140820.a 
#extlibs := /home/bruce/guitar-android/kernel/drivers/input/touchscreen/FT7811/ft_gesture_32bit_lib_v1.0_20140820.a
ldflagobj-y += fts_ts.o 
obj-y s-y +=--whole-archive $(extlibs) --no-whole-archive
