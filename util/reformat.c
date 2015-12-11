#include <stdio.h>

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
		if(strncmp(argv[x], "-f", 2))
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
	unlink("/tmp/reformat.txt");

	int fd_tmp = open("/tmp/reformat.txt", O_RDWR | O_CREAT, 0664);

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
		
		snprintf(formatted, 128, "0x%lu\n", addr);
		write(fd_tmp, formatted, strlen(formatted));
	}

	/* move the current file */
	close(fd);
	rename(file, "test.tmp");

	/* Move the tmp file to here */
	close(fd_tmp);
	rename("/tmp/reformat.txt", file);

	/* Delete original */
	unlink("test.tmp");

	return 0;
}
