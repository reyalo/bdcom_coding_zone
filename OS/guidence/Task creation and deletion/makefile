#***********************************************************************#
#*                                                                     *#
#*   MODULE:  makefile                                                 *#
#*   AUTHOR:  Zeng.Zhao.Heng.                                          *#
#*   DATE:    2002/12/11  					       *#
#*   PURPOSE: This file contains the makefile rules for building the   *#
#*	      library     					       *#
#*                                                                     *#
#***********************************************************************#

LIBNAME=os-demo.a
LIBDIRNAME=os-demo

EXTRA_INCLUDE=-I$(SWITCH_BASE)/apps/$(SWITCH_NAME) -I$(SWITCH_BASE)/include 

SUBDIRS	=
OBJS    = os-demo.o os-demo-cmd.o os-demo-cmd-impl.o

include $(SWITCH_BASE)/configs/rules.library

ifeq ($(findstring $(SWITCH_NAME),2026 2224), $(SWITCH_NAME))
CFLAGS += -funaligned-pointers 
endif

CC_OPTIM= -O0 -g 
