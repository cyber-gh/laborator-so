#include <unistd.h>

const char* buffer = "hello world";

int main() {

	write(1, buffer, 10);
	return 0;
}
