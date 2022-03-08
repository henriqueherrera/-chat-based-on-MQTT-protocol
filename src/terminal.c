#include <terminal.h>

void erase() {
  fprintf(stdout, "\033[2J\033[0;0f");
}

void terminal_headers() {
  erase();
  printf("Chat MQTT");
}