#
# Common footer host makefile.
#

ifndef $(COMMON_FOOTER_MK)
COMMON_FOOTER_MK = 1

OBJS=$(subst .c,.o, $(FILES) )

OBJ_DIR=$(OBJ_BASE_DIR)/$(MODULE)/$(CONFIG)

vpath %.o $(OBJ_DIR)

LIB_STATIC=$(MODULE).a

LIB_DYNAMIC=$(MODULE).so

EXE=$(EXE_BASE_DIR)/$(CONFIG)/$(MODULE).out

SO_LIB=$(EXE_BASE_DIR)/$(CONFIG)/lib$(MODULE).so

.c.o:
	@echo \# $(MODULE): $(PLATFORM): Compiling $<
	@$(CC) $(CC_OPTS) $(OPTI_OPTS) $(DEFINE) $(INCLUDE) -o $(OBJ_DIR)/$@ $<


lib : $(LIB_DIR)/$(LIB_STATIC) $(LIB_DIR)/$(LIB_DYNAMIC)

$(LIB_DIR)/$(LIB_STATIC) : $(OBJS)
	@echo \# $(MODULE): $(PLATFORM): Creating archive $(LIB_STATIC)
	$(AR) $(AR_OPTS) $(LIB_DIR)/$(LIB_STATIC) $(OBJ_DIR)/*.o

$(LIB_DIR)/$(LIB_DYNAMIC) : $(OBJS)
	@echo \# $(MODULE): $(PLATFORM): Creating archive $(LIB_DYNAMIC)
	$(CC) $(LD_OPTS) -shared -fpic -o $(LIB_DIR)/$(LIB_DYNAMIC) $(OBJ_DIR)/*.o

obj: $(OBJS)

clean:
	@echo \# $(MODULE): $(PLATFORM): Deleting temporary files
	-rm -f MAKEFILE.DEPEND
	-rm -f $(LIB_DIR)/$(LIB_STATIC)
	-rm -f $(LIB_DIR)/$(LIB_DYNAMIC)
	-rm -f $(OBJ_DIR)/*.*

depend:
#	@echo \# $(MODULE): $(PLATFORM): Making directories, if not already created
	-mkdir -p $(LIB_DIR)
	-mkdir -p $(OBJ_DIR)
	-mkdir -p $(EXE_BASE_DIR)/$(CONFIG)
	@echo \# $(MODULE): $(PLATFORM): Building dependancies
	$(CC) $(DEFINE) $(INCLUDE) $(FILES) -M > MAKEFILE.DEPEND

so:
	@echo \# $(MODULE): $(PLATFORM): Linking to .so
	$(CC) $(LD_OPTS) -shared -fpic -o $(SO_LIB) $(OBJ_DIR)/*.o $(LIBS)
	@echo \# Final shared library $(SO_LIB) !!!	
	@echo \#

exe:
	@echo \# $(MODULE): $(PLATFORM): Linking ...
	$(LD) -o $(EXE) $(LIBS) $(LD_OPTS)
	@echo \# Final executable $(EXE) !!!	
	@echo \#

install:


-include MAKEFILE.DEPEND

endif #	ifndef $(COMMON_FOOTER_MK)
