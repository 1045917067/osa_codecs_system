
codecs_system.base:
	$(MAKE) -fMAKEFILE.MK libs TARGET=depend
	$(MAKE) -fMAKEFILE.MK libs
	$(MAKE) -fMAKEFILE.MK libs TARGET=so

codecs_system:
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/codecs_system/src MODULE=codecs_system $(TARGET)

clean:
	$(MAKE) -fMAKEFILE.MK libs TARGET=clean

libs: codecs_system


all: clean codecs_system.base

.PHONY: all codecs_system.base codecs_system clean libs
