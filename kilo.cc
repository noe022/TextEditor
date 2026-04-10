#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios termios_struct;

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_struct) == -1)
    die("tcsetattr");
}

void enableRawMode(){
	if (tcgetattr(STDIN_FILENO, &termios_struct) == -1) die("tcgetattr");
	atexit(disableRawMode);

	struct termios copy_struct = termios_struct;
  copy_struct.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	copy_struct.c_cflag &= ~(OPOST);
	copy_struct.c_cflag &= ~(CS8);
	copy_struct.c_lflag &= ~(ECHO | IEXTEN |ICANON | ISIG);
  copy_struct.c_cc[VMIN] = 0;
  copy_struct.c_cc[VTIME] = 1;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &copy_struct) == -1) die ("tcsetattr");
}

int main() {
	enableRawMode();
  while (true) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') break;
  }
	return 0;
}
