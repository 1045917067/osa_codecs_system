#
#   Common softwares install directory.
#

ifeq ($(PROJECT_PATH), )

# Default build environment, windows or linux
ifeq ($(OS), )
  OS := linux
endif

PROJECT_RELPATH = $(notdir $(CURDIR))

ifeq ($(OS),Windows_NT)
  PROJECT_BASE     := $(CURDIR)/..
endif

ifeq ($(OS),linux)
  PROJECT_BASE     := $(shell pwd)/..
endif

PROJECT_PATH     := $(PROJECT_BASE)/$(PROJECT_RELPATH)

# Code gen tools
CODEGEN_PATH_HOST:= ~/workingshop/arago
CODEGEN_PREFIX   := $(CODEGEN_PATH_HOST)/bin/arm-arago-linux-gnueabi-
CSTOOL_PREFIX    := arm-arago-linux-gnueabi-
ifeq ($(OS),Windows_NT)
CODEGEN_PREFIX   := $(CODEGEN_PATH_HOST)/bin/arm-arago-linux-gnueabi-
CSTOOL_PREFIX    := arm-arago-linux-gnueabi-
endif


# DVR RDK installation path
DVR_RDK_PATH := $(shell pwd)/system

ifeq ($(DVR_RDK_PATH), )
$(error DVR RDK path is not set, please set it correctly!!!)
endif

ifeq ($(CORE), )
  CORE := host
endif

# Default platform
ifeq ($(PLATFORM), )
  PLATFORM := linux
endif

# Default configuration
ifeq ($(CONFIG), )
#  CONFIG := debug
endif

endif

TREAT_WARNINGS_AS_ERROR=no

#
#   Export global environment variables.
#
export OS
export PLATFORM
export CORE
export PROJECT_PATH
export DVR_RDK_PATH
export TREAT_WARNINGS_AS_ERROR
export CODEGEN_PREFIX
export CONFIG
