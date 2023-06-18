SRC_DIRS := 
BUILD_DIR = ../obj/libfdt

LIBFDT_dir = .
LIBFDT_archive = libfdt.a
LIBFDT_lib = $(LIBFDT_dir)/$(LIBFDT_LIB)
LIBFDT_include = $(addprefix $(LIBFDT_dir)/,$(LIBFDT_INCLUDES))
LIBFDT_version = $(addprefix $(LIBFDT_dir)/,$(LIBFDT_VERSION))
CFLAGS = -Wpointer-arith -Wcast-qual -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes \
-Wredundant-decls -Wshadow -Wsuggest-attribute=format -Wwrite-strings -I../libc/obj/include \
-I../libc/arch/aarch64 -I../libc/include -I../libc/arch/generic


ifeq ($(STATIC_BUILD),1)
	CFLAGS += -static
	LIBFDT_dep = $(LIBFDT_archive)
else
	LIBFDT_dep = $(LIBFDT_lib)
endif

include Makefile.libfdt

.PHONY: libfdt


all: $(LIBFDT_archive)

SRCS := $(shell find $(SRC_DIRS) *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
-include $(DEPS)



$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	aarch64-linux-gnu-gcc $(CFLAGS) -L../libc/lib/ -lc -c -o $@ $<	

$(LIBFDT_archive): $(OBJS)
	aarch64-linux-gnu-ar rcs $@ $(OBJS)


ifneq ($(DEPTARGETS),)
-include $(LIBFDT_OBJS:%.o=$(LIBFDT_dir)/%.d)
endif

clean:
	rm -f *.o libfdt.a $(BUILD_DIR)/*