#include <terminal.h>
#include <global.h>
#include <mqtt-client.h>

void erase() {
  fprintf(stdout, "\033[2J\033[0;0f");
}

void *terminal_headers(void *vargp) {
  while (is_loading)
  {
    printf("Conectando, aguarde!\n");
    usleep(2000);
  }
  
  printf("************\nChat MQTT\n************");
  
  while (option != 10) {
    //erase();
    printf("\n1 - Enviar uma mensagem para um Tópico \n");
    printf("2 - Inscrever-se em um Tópico \n");

    printf("\nSelecione uma opção: ");
    scanf("%d", &option);

    switch (option)
    {
      case 1:
        inputMessage();
        break;
      case 2:
        inputSubscribe();
        break;
      default:
        break;
    }
  }

  return NULL;
}