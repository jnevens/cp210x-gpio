#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
	unsigned gpio = 0;

	if(argc < 4) {
		fprintf(stderr, "Usage: %s [device] PIN set|unset\n", argv[0]);
		return -1;
	}

	int pin = atoi(argv[2]);
	bool set = (strcmp(argv[3], "set") == 0) ? true : false;

	int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY );
	if(fd <= 0)
		perror("failed opening port");

	printf("Port open!\n");

	if(ioctl(fd, 0x8000, &gpio)) {
		perror("read gpio of cp210x");
	}
	printf("gpio state: %X\n", gpio);

	if(!set) {
		gpio |= 1 << pin;
	} else {
		gpio &= ~(1 << pin);
	}
	
	gpio = gpio << 8;
	gpio |= 0x00FF;

	printf("gpio set: %X\n", gpio);

	if(ioctl(fd, 0x8001, &gpio)) {
		perror("write gpio of cp210x");
	}

	if(ioctl(fd, 0x8000, &gpio)) {
		perror("read gpio of cp210x");
	}
	printf("gpio state: %X\n", gpio);


	close(fd);


	return 0;
}
