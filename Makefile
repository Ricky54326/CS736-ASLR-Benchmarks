CC := gcc
CFLAGS := -pthread -ggdb
INCLUDE := -I include

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

PERFORMANCE := \
	performance1 \
	performance1_helper \
	performance2 \
	performance3 \
	performance3_helper \
	performance4 \
	performance4_helper \
	test

APERFORMANCE := \
	write

ENTROPY := \
	entropy1 \
	entropy2 \
	entropy3 \
	entropy4


UTIL := \
	calc \
	run_test

CLEAN := $(PERFORMANCE)	$(APERFORMANCE) $(UTIL) $(ENTROPY)
JUNK := output.txt child.txt parent.txt

all: $(APERFORMANCE) $(PERFORMANCE) $(UTIL) $(ENTROPY)

performance1_helper:
	$(CC) $(CFLAGS) $(INCLUDE) -nostdlib performance/performance1_helper.c -o $@ write

%: entropy/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -fPIE -pie $< -o $@
%: performance/%.S
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
%: performance/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@ write
%: util/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(CLEAN) $(JUNK)
