#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios termios_struct;
void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_struct);
}

void enableRawMode(){
	tcgetattr(STDIN_FILENO, &termios_struct);
	atexit(disableRawMode);

	//Hacemos una copia de termios
	struct termios copy_struct = termios_struct;
	copy_struct.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &copy_struct);
}

int main() {
	enableRawMode();
	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		return 0;
	}
}
