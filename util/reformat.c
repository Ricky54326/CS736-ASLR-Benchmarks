#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <stdint.h>

#define TMP "/tmp/reformat.tmp"

static int readln(int fd, char* dst, size_t sz)
{
	if(sz == 0 || dst == NULL) return 0;

	/* Zero the buffer */
	memset(dst, 0, sz);

	int pos = 0;
	char c = 0;

	do
	{
		int bytes = read(fd, &c, 1);

		/* Check for end of file */
		if(bytes == 0) return -1;

		/* Check for end of line */
		if(c == '\n') break;

		/* Assign the character */
		dst[pos] = c;
	
		/* Iterate index */
		pos++;
	} while(pos < sz - 1);

	return pos;
}

static void file_cp(const char* src, const char* dst)
{
	char buffer[512];

	int src_fd = open(src, O_RDONLY);
	int dst_fd = open(dst, O_RDWR | O_CREAT, 0664);

	int bytes_read = 0;
	do
	{
		bytes_read = read(src_fd, buffer, 512);
		write(dst_fd, buffer, bytes_read);
	}while(bytes_read != 0);

	close(src_fd);
	close(dst_fd);
}

static void to_lower(char* dst, const char* src, size_t sz)
{
	memset(dst, 0, sz);
	int x;
	for(x = 0;x < sz && x < strlen(src);x++)
		dst[x] = src[x];
}

#define OUT_HEX 0x01
#define OUT_DEC 0x02
#define OUT_OCT 0x03

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		printf(
	"Usage: ./reformat [-p <padding>] [-o <output-type>] [-l <length>] -f <file> <shift amount>\n"
	"\tParameters: <required> [optional]\n"
	"\n"
	"\tValid output formats: dec, hex\n"
	"\n");
		return 1;
	}

	char* file = NULL;
	int amount = -1;
	int length = -1;
	int padding = -1;
	char padding_str[128];
	memset(padding_str, 128, 0);
	char output_type = OUT_HEX; 

	int x;
	for(x = 1;x < argc;x++)
	{
		if(!strncmp(argv[x], "-f", 2))
		{
			file = argv[x + 1];
			x++;
		} else if(!strncmp(argv[x], "-o", 2))
		{
			char out_fmt[16];
			to_lower(argv[x + 1], out_fmt, 16);
			if(!strncmp(out_fmt, "dec", 3))
			{
				output_type = OUT_HEX;
			} else if(!strncmp(out_fmt, "hex", 3))
			{
				output_type = OUT_DEC;
			} else {
				printf("reformat: %s invalid format.\n",
					out_fmt);
			}
			
		} else if(!strncmp(argv[x], "-l", 2))
		{
			length = atoi(argv[x + 1]);
			x++;
		} else if(!strncmp(argv[x], "-p", 2))
		{
			padding = atoi(argv[x + 1]);
			x++;
		} else {
			amount = atoi(argv[x]);
		}
	}
	
	if(padding > 0)
		snprintf(padding_str, 128, "%d", padding);

	if(file == NULL)
	{
		printf("reformat: No file provided.\n");
		return 1;
	}

	if(amount < 0)
	{
		printf("reformat: Shift amount is invalid.\n");
		return 1;
	}

	/* Open the file */
	int fd = open(file, O_RDONLY);

	if(fd < 0)
	{
		printf("reformat: %s: Permission Denied.\n", file);
		return -1;
	}

	/* Create a temporary file */
	unlink(TMP);

	int fd_tmp = open(TMP, O_RDWR | O_CREAT, 0664);

	if(fd_tmp < 0)
	{
		printf("reformat: Could not create tmp file!\n");
		return -1;
	}

	char result[128];
	uintptr_t addr;
	int length_mask = (1 << (length + 1)) - 1;

	while(readln(fd, result, 128) > 0)
	{
		char formatted[128];
		memset(formatted, 0, 128);
		char* res;
		addr = strtoul(result, &res, 16);
		addr >>= amount;

		if(length > 0)
			addr &= length_mask;

		char fmt_str[128];
		memset(fmt_str, 0, 128);
	
		switch(output_type)
		{
			case OUT_DEC:
				if(padding > 0)
				{
					snprintf(fmt_str, 128, "%%0");
					strncat(fmt_str, padding_str, 128);
					strncat(fmt_str, "ld\n", 128);
				} else snprintf(fmt_str, 128, "%%lu\n");
				break;
			case OUT_HEX:
				if(padding > 0)
				{
					snprintf(fmt_str, 128, "0x%%0");
					strncat(fmt_str, padding_str, 128);
					strncat(fmt_str, "lx\n", 128);
				} else snprintf(fmt_str, 128, "0x%%lx\n");
				break;
			default:
				printf("reformat: invalid output mode.\n");
				return 1;
		}	

		snprintf(formatted, 128, fmt_str, addr);

		write(fd_tmp, formatted, strlen(formatted));
	}

	/* move the current file */
	close(fd);
	if(rename(file, "test.tmp"))
	{
		printf("reformat: Warning: orig not saved.\n");
		return -1;
	}

	/* Copy temp here */
	file_cp(TMP, file);

	/* Delete original */
	unlink("test.tmp");

	/* Delete tmp */
	unlink(TMP);

	return 0;
}
