CC := gcc
CFLAGS := -pthread -ggdb
INCLUDE := -I include
LIBS := -lsqlite3 -pthread

CFLAGS := $(CFLAGS) $(BUILD_FLAGS)

PERFORMANCE := \
	performance1 \
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
	calc \
	sub \
	run_test

CLEAN := $(PERFORMANCE)	$(UTIL) $(ENTROPY)
JUNK := output.txt child.txt parent.txt

all: $(PERFORMANCE) $(UTIL) $(ENTROPY)

%: entropy/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -fPIE -pie $< -o $@ $(LIBS)
%: performance/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@ $(LIBS)
%: util/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

clean:
	rm -rf $(CLEAN) $(JUNK)
