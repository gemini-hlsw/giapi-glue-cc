#Directories used to build and install the GIAPI C++ language glue. 


ifndef GIAPI_ROOT
       GIAPI_ROOT:=$(shell pwd | sed -E 's/(src|test).*//' )
       $(info GIAPI_ROOT is not defined, using  $(GIAPI_ROOT) )
endif

#Installation directory
INSTALL_DIR := ${GIAPI_ROOT}/install

#External libraries dependencies, used during build
EXTERNAL_LIB := ${GIAPI_ROOT}/external

#Directory where distribution files are going to be stored
DISTRIBUTION_DIR := ${GIAPI_ROOT}/dist
