CC := gcc
CFLAGS := -pthread -ggdb
INCLUDE := -I include

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

PERFORMANCE := \
	performance3 \
	performance4

UTIL := \
	calc

#	performance1 
#	performance2 

CLEAN := $(PERFORMANCE)	$(UTIL)

all: $(PERFORMANCE) $(UTIL)

%: performance/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
%: util/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN)
