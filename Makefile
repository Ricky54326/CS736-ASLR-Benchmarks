CC := gcc
CFLAGS := -pthread -ggdb
INCLUDE := -I .

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

PERFORMANCE :=
	performance2

PERFORMANCE_SOURCES := $(addprefix performance/, $(PERFORMANCE))
PERFORMANCE_SOURCES := $(addsuffix .c, $(PERFORMANCE_SOURCES))

CLEAN := $(PERFORMANCE)	

all: $(PERFORMANCE)

%: performance/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN)
