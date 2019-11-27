#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

char *source_file = "test.in";
char *destination_file = "test.out";

#define BUFF_SIZE (4)

int main() {

	int source_file_fd = open(source_file, O_RDONLY);
	if (source_file_fd == -1) {
		printf("coudn't open the source file");
		return 0;
	}
	
	int destination_file_fd = open(destination_file,  O_WRONLY);
	if (destination_file_fd == -1) {
		if (errno == ENOENT) {
			destination_file_fd = open(destination_file,  O_CREAT | O_WRONLY);
			if (destination_file_fd == -1) {
				//printf("coudn't create create destination file");
				perror(errno);
				return errno;
			} 
		} else {
			//printf("Coudln't open destination file");
			perror(errno);
			return errno;
		}	
	}

	int read_bytes = 0;
	char *in_buffer[BUFF_SIZE];
	while ( (read_bytes = read(source_file_fd,in_buffer, BUFF_SIZE)) != 0 ) {
		//write(1, in_buffer, read_bytes);
		int left_bytes = read_bytes;
		int offset = 0;
		while (left_bytes != 0 ) {
			int written_bytes = write(destination_file_fd, in_buffer + offset, left_bytes);
			left_bytes -= written_bytes;
			offset += written_bytes; 
		}
	} 
	
	close(source_file_fd);
	close(destination_file_fd);	

	return 0;
}
