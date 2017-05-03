#
#	Common headers directory.
#

ifndef $(INCLUDES_MK)
INCLUDES_MK = 1

#
#	Common headers
#

OSA_INC_DIR= $(DVRRDK_INC_DIR)

#
#	Common library.
#





#
# Set the library path.
#
ifeq ($(PLATFORM), )
  PLATFORM := 
endif

DVRRDK_LIB_DIR = $(DVR_RDK_PATH)/lib/$(PLATFORM)

DVRRDK_LIBS = $(DVRRDK_LIB_DIR)

DVRRDK_BIN_DIR = $(DVR_RDK_PATH)/bin


#
# Export DVR_RDK global environment variables.
#
export DVRRDK_INC_DIR
export DVRRDK_LIB_DIR
export DVRRDK_LD_OPTS

endif # ifndef $(INCLUDES_MK)

