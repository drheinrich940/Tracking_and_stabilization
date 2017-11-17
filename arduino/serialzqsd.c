#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

struct termios tty_attr_old;

void tty_raw() {
  struct termios tty_attr;

  tcgetattr(0, &tty_attr);

  tty_attr.c_lflag &= (~(ICANON | ECHO));
  tty_attr.c_cc[CTIME] = 0;
  tty_attr.c_cc[VMIN] = 1;

  tcsetattr(0, TCSANOW, &tty_attr);
}

int main(int argc, char **argv) {
  tcgetattr(0, &tty_attr_old);
  tty_raw();

  // File descriptor à l'ouverture de la liaison
  int fd = open("/dev/ttyACM0", 777);
  printf("%s\n", strerror(errno));
  char c = 'q';

  while(1) {
    read(0, &c, 1);
    if (c == 'q')
      break;
    else if (c)
      write(fd, &c, 1);
  }

  close(fd); // On ferme le file descriptor

  tcsetattr(0, TCSANOW, &tty_attr_old);
  return 0;
}
