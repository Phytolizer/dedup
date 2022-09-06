SRCS := repo/sha256.c
OBJS := $(patsubst repo/%.c,%.o,$(SRCS))
CFLAGS :=
LDFLAGS :=
LIB := libsha256.a

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: repo/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(LIB)
