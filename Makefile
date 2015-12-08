CC := gcc48
CFLAGS := -pthread -ggdb -gdwarf-2
INCLUDE := -I include
LIBS := -lsqlite3

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

APERFORMANCE := \
	write

PERFORMANCE := \
	performance1 \
	performance1_helper \
	performance2 \
	performance3 \
	performance3_helper \
	performance4 \
	performance4_helper

ENTROPY := \
	entropy1 \
	entropy2 \
	entropy3 \
	entropy4


UTIL := \
	calc

CLEAN := $(APERFORMANCE) $(PERFORMANCE) $(UTIL) $(ENTROPY)
JUNK := output.txt child.txt parent.txt

all: $(APERFORMANCE) $(PERFORMANCE) $(UTIL) $(ENTROPY)

performance1_helper:
	$(CC) $(CFLAGS) $(INCLUDE) -nostdlib performance/performance1_helper.c -o $@ write

%: entropy/%.c
	$(CC) $(CFLAGS) $(INCLUDE)  $< -o $@ $(LIBS)
%: performance/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
%: performance/%.S
	$(CC) $(CFLAGS) $(INCLUDE) $< -c -o $@
%: util/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN) $(JUNK)
