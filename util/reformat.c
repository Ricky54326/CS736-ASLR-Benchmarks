#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <stdint.h>

#define TMP "/tmp/reformat.tmp"

int readln(int fd, char* dst, size_t sz)
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

void file_cp(const char* src, const char* dst)
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

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		printf("Usage: ./reformat -f [file] [shift amount]\n");
		return 1;
	}

	char* file = NULL;
	int amount = -1;

	int x;
	for(x = 1;x < argc;x++)
	{
		if(!strncmp(argv[x], "-f", 2))
		{
			file = argv[x + 1];
			x++;
		} else {
			amount = atoi(argv[x]);
		}
	}

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

	while(readln(fd, result, 128) > 0)
	{
		char formatted[128];
		memset(formatted, 0, 128);
		char* res;
		addr = strtoul(result, &res, 16);
		addr >>= amount;
		
		snprintf(formatted, 128, "0x%lx\n", addr);
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
