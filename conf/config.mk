#Directories used to build and install the GIAPI C++ language glue. 

#mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
#mkfile_dir := $(dir $(mkfile_path))
#$(info path: $(mkfile_path)  $(mkfile_dir) values)

ifndef GIAPI_ROOT
       #GIAPI_ROOT:=$(shell pwd | sed '-E -e s/(src|test).*//' )
       GIAPI_ROOT:=$(shell pwd | sed -E 's/(src|test).*//' )
       $(info GIAPI_ROOT is not defined, using  $(GIAPI_ROOT) )
endif

#Installation directory
INSTALL_DIR := ${GIAPI_ROOT}/install

#External libraries dependencies, used during build
EXTERNAL_LIB := ${GIAPI_ROOT}/external

#Directory where distribution files are going to be stored
DISTRIBUTION_DIR := ${GIAPI_ROOT}/dist
