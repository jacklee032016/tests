# $Id$

# 1.9.2. Oct. 15, 2019
NAME=rangeMgt

ROOT_DIR:=$(shell pwd)
RULE_DIR:=$(ROOT_DIR)
# /../

ARCH=
#arm

export ROOT_DIR
export RULE_DIR

EXTENSION=

#BUILDTIME := $(shell TZ=UTC date -u "+%Y_%m_%d-%H_%M")
BUILDTIME := $(shell TZ=CN date -u "+%Y_%m_%d")
#GCC_VERSION := $(shell $(CC) -dumpversion )


RELEASES_NAME=$(NAME)_$(GCC_VERSION)_$(ARCH)_$(EDITION)_$(BUILDTIME).tar.gz  

# this directory defined the configuration file for this program


export ARCH
export BUILDTIME
export RELEASES_NAME


SUBDIRS += c/src
SUBDIRS += c/tests

SUBDIRS += cpp/src
SUBDIRS += cpp/tests


all: 
	for i in $(SUBDIRS) ; do ( cd $$i && $(MAKE) $@ ) ; done

clean: 
	rm -rf Linux.bin.* 
	rm -rf *.log
	- find . -name Linux.obj.* -prune -exec rm -r -f {} \;

#	rm -rf $(NAME)_*
	
# all	
install:
	@$(SHELL) $(ROOT_DIR)/buildver.sh $(ROOT_DIR)
	@$(SHELL) $(ROOT_DIR)/install.sh $(ROOT_DIR)/Linux.bin.$(ARCH) $(ROOT_DIR)/releases  

#	@$(SHELL) $(ROOT_DIR)/buildver.sh $(ROOT_DIR)

package:clean
	cd ..; tar -cvjf $(NAME).$(BUILDTIME).tar.bz2 $(NAME) 

