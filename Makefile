
ODIR=obj

CFLAGS = -g -Wall 
ALL_CFLAGS = $(CFLAGS) -Os

DEPS = cmd.h network.h

_DEPS = $(DEPS)

_OBJ = network.o cmd.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

-include $(shell mkdir obj 2>/dev/null)

all:  slither

$(ODIR)/%.o: %.c $(_DEPS)
	$(CC) -c -pthread -g -o $@ $< $(ALL_CFLAGS) $(DEFINE)

slither: $(OBJ) 
	$(CC) -pthread -g -o $@ $^ $(LIBS) $(ALL_CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o ipregd

