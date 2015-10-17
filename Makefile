CC := gcc
CFLAGS := -pthread -ggdb
INCLUDE := -I include

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

PERFORMANCE := \
	performance1 \
	performance3 \
	performance4

#performance2 

UTIL := \
	calc

CLEAN := $(PERFORMANCE)	$(UTIL)

all: $(PERFORMANCE) $(UTIL)

%: performance/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
%: util/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN)
