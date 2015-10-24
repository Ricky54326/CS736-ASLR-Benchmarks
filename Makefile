CC := gcc
CFLAGS := -pthread -ggdb
INCLUDE := -I include

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

PERFORMANCE := \
	performance1 \
	performance2 \
	performance3 \
	performance4

ENTROPY := \
	entropy1 \
	entropy2 \
	entropy3


UTIL := \
	calc

CLEAN := $(PERFORMANCE)	$(UTIL) $(ENTROPY)

all: $(PERFORMANCE) $(UTIL) $(ENTROPY)

%: entropy/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
%: performance/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
%: util/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN)
