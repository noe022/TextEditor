#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct editorState {
  int rows;
  int cols;
  struct termios termios_struct;
};

struct editorState editor;

struct writeBuffer {
  char *buffer;
  int len;
};

#define ABUF_INIT {NULL,0}

void drawTildes() {
  for (int i = 0; i < editor.rows; i++) {
    write(STDOUT_FILENO, "~", 1);
    if (i < editor.rows -1) {
      write(STDOUT_FILENO, "\r\n", 2);
    }
  }
}

void refreshScreen() {
  // We refresh and put the cursor up
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

void die(const char *s) {
  refreshScreen();
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor.termios_struct) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  //tcgetattr puts in a buffer, terminal flags and configurations
	if (tcgetattr(STDIN_FILENO, &editor.termios_struct) == -1) die("tcgetattr");
	atexit(disableRawMode);
	struct termios copy_struct = editor.termios_struct;
  copy_struct.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	copy_struct.c_oflag &= ~(OPOST);
	copy_struct.c_cflag |= (CS8);
	copy_struct.c_lflag &= ~(ECHO | IEXTEN |ICANON | ISIG);
  copy_struct.c_cc[VMIN] = 0;
  copy_struct.c_cc[VTIME] = 1;
  //tcsetattr changes terminal configuration
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &copy_struct) == -1) die ("tcsetattr");
}

char readKeyPressed() {
  // Initialize c for always have cursor jumping
  char c = '\0';
  if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
  return c;
}

void handleOperations(char c) {
  if (c == CTRL_KEY('q')) {
    refreshScreen();
    exit(0);
  }
}

int alternativeWS(int *rows, int *cols) {
  write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12);
  write(STDOUT_FILENO, "\x1b[6n", 4);
  char cursor_position[32];
  // We know the answer must \x1b[24;80R, we look for the final 'R'
  for (unsigned int i = 0; i < sizeof(cursor_position); i++) {
    if (read(STDIN_FILENO, &cursor_position[i], 1) != 1) break;
    if (cursor_position[i] == 'R') break;
  }
  sscanf(&cursor_position[2], "%d;%d", rows, cols);
  return 0;
}

void getWindowSize() {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
    if(alternativeWS(&editor.rows, &editor.cols) == -1 ) die ("getWindowSize");
  } else {
    editor.rows = ws.ws_row;
    editor.cols = ws.ws_col;
  } 
}

int main() {
	enableRawMode();
  getWindowSize();
  while (1) {
    refreshScreen();
    drawTildes();
    write(STDOUT_FILENO, "\x1b[H", 3);
    handleOperations(readKeyPressed());
  }
	return 0;
}
