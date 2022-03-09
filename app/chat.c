#include <global.h>
#include <terminal.h>
#include <mqtt-client.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

int main(int argc, char *argv[]) {
  pthread_t thread_id, thread_id_2;

  while (strlen(client_id) == 0)
  {
    printf("Insira o id: ");
    scanf("%s", client_id);
  }
  
  pthread_create(&thread_id, NULL, terminal_headers, NULL);
  pthread_create(&thread_id_2, NULL, start_client, NULL);
  
  pthread_join(thread_id, NULL);
  pthread_join(thread_id_2, NULL);
  /*  */

  
  return 0;
}