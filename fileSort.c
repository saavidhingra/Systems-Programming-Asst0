#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
       #include <fcntl.h>


int main(int argc, char** argv){
	
	int fd = open(argv[1], O_RDONLY);
	char* buffer = malloc(sizeof(char)*256);
	int a = read(fd, buffer, 18);
	printf("%s %d\n", buffer, a);

	return 0;
}
