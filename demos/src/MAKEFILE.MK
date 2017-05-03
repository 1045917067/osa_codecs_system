
include $(PROJECT_PATH)/makerules/common_header_host.mk

INCLUDE+= $(OSA_INC_DIR) -I../include -I../../osa/include

LIBS=$(LIB_DIR)/codecs_system_demos.a $(LIB_DIR)/codecs_system.a $(LIB_DIR)/osa.a $(DVRRDK_LD_OPTS) $(COMMON_LIBS)

include $(PROJECT_PATH)/makerules/common_footer_host.mk
