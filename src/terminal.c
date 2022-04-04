#include <terminal.h>

void erase()
{
  fprintf(stdout, "\033[2J\033[0;0f");
}

void terminal_headers()
{
  printf("\n\n------------ Escolha uma opção ------------\n\n");
  printf("(1) Solicitar conexão a um usuário\n");
  printf("(2) Aceitar solicitação\n");
  printf("(3) Enviar mensagem para um chat\n");
  printf("(4) Criar ou entrar em um grupo\n");
  printf("(5) Enviar mensagem para um grupo \n");
  printf("(6) Desconectar do MQTT-CHAT \n");
}