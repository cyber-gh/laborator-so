#include <unistd.h>
#include <errno.h>

const char* buffer = "hello world";

int main() {

	if (write(1, buffer, 5) == -1) {
		perror("error writing");
		return errno;
	}
	return 0;
}
