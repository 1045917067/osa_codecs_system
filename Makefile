#
#
#

PLATFORM := 

#
# include local rules.make
#
include Rules.make

# include global Rules.mak
-include $(DVR_RDK_PATH)/Rules.mak

##########################################
#                                        #
# Codecs System Top Level Build Targets  #
#                                        #
##########################################

.PHONY : osa codecs_system demos clean

osa:
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/osa MODULE=osa

osa_clean:
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/osa MODULE=osa clean

codecs_system: osa
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/codecs_system MODULE=codecs_system

codecs_system_clean: osa_clean
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/codecs_system MODULE=codecs_system clean

demos: codecs_system
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/demos MODULE=codecs_system_demos

demos_clean: codecs_system_clean
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/demos MODULE=codecs_system_demos clean

clean: osa_clean codecs_system_clean demos_clean

all: clean osa codecs_system demos
