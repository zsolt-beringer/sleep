#include <stdint.h>
#include <string.h>
#include <time.h>

void *syscall();

#define ERROR(msg, code) return syscall(1, 2, msg, strlen(msg)), code // sys_write

int main(int argc, char const *argv[]) {
	if (argc != 2) ERROR("Usage: sleep SECONDS\n", 1);

	struct timespec a;
	a.tv_sec = 0;
	a.tv_nsec = 0;

	char const *s = argv[1];
	uint32_t p = 1e8;
	char c;

state0:
	switch (c = *s++) {
	case 0: goto end;
	case '.': goto state1; 

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		a.tv_sec = a.tv_sec * 10 + c - '0';
		goto state0;

	default: goto error;
	}

state1:
	switch (c = *s++) {
	case 0: goto end;

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		a.tv_nsec += (c - '0') * p;
		p = (p >> 1) * (uint32_t)3435973837; // 3435973837 = 1/5 modulo 2 ** 32
		goto state1;

	default: goto error;
	}

error:
	ERROR("Invalid number\n", 2);

end:
	syscall(35, &a, 0); // sys_nanosleep
}