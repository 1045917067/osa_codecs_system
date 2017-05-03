
demos.base:
	$(MAKE) -fMAKEFILE.MK libs TARGET=depend
	$(MAKE) -fMAKEFILE.MK libs
	$(MAKE) -fMAKEFILE.MK libs TARGET=exe

demos:
	$(MAKE) -fMAKEFILE.MK -C$(PROJECT_PATH)/demos/src MODULE=codecs_system_demos $(TARGET)

clean:
	$(MAKE) -fMAKEFILE.MK libs TARGET=clean

libs: demos


all: clean demos.base

.PHONY: all demos.base demos clean libs
