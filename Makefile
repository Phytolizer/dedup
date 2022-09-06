SRCS := main.c str.c recdir.c
OBJS := $(SRCS:.c=.o)
EXE := dedup
LIBS := sha256
LIBS_FILES := $(patsubst %,lib%.a,$(LIBS))
LIBDIR := crypto-algorithms
LIB_LDFLAGS := $(patsubst %,-l%,$(LIBS))

CFLAGS := -Wall \
	-Wextra \
	-Wpedantic \
	-Wmissing-prototypes \
	-Wno-unused-function \
	-std=c11 \
	-ggdb \
	-fsanitize=address,undefined \
	-D_DEFAULT_SOURCE \
	-fno-omit-frame-pointer \
	-I$(LIBDIR)/repo \
	-MMD \
	-MP

LDFLAGS := -fsanitize=address,undefined

.PHONY: all clean libs

all: $(EXE)

libs:
	$(MAKE) -C $(LIBDIR)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJS) libs
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -L./$(LIBDIR) $(LIB_LDFLAGS)

clean:
	rm -f $(OBJS) $(EXE)

-include $(OBJS:.o=.d)
