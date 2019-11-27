#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

char *source_file = "test.in";
char *destination_file = "test.out";

#define BUFF_SIZE (4)
int total_bytes = 0;
int main() {

	int source_file_fd = open(source_file, O_RDONLY);
	if (source_file_fd == -1) {
		perror("coudn't open the source file");
		return 0;
	}

	struct stat sb;
	if (stat(source_file, &sb)) {
		perror(source_file);
		return errno;
	}
	
	int destination_file_fd = open(destination_file, O_CREAT | O_WRONLY);
	if (destination_file_fd == -1) {
		if (0) {
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
	int total_read = 0;
	while ( (read_bytes = read(source_file_fd,in_buffer, BUFF_SIZE)) != 0 ) {
		//write(1, in_buffer, read_bytes);
		total_read += read_bytes;
		int left_bytes = read_bytes;
		int offset = 0;
		while (left_bytes != 0 ) {
			int written_bytes = write(destination_file_fd, in_buffer + offset, left_bytes);
			left_bytes -= written_bytes;
			offset += written_bytes; 
		}
	} 

	if (total_read != sb.st_size) {
		perror("bad read");
		return -1;
	}

	
	close(source_file_fd);
	close(destination_file_fd);	

	return 0;
}
