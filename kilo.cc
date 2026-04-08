#include <ctype.h>
#include <stdio.h>
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

	struct termios copy_struct = termios_struct;
	copy_struct.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &copy_struct);
}

int main() {
	enableRawMode();
	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c)) {
    	printf("%d\n", c);
		} else {
			printf("%d ('%c')\n", c, c);
		}
		return 0;
	}
}
