#include <flow.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <pthread.h>
#include <unistd.h>
#include "MQTTAsync.h"
#include "global.h"

void delete_solicitation(int id)
{
  strcpy(TOPICS_PENDENTS[id], "");
}

void pub_msg(char *topic, char *payload, MQTTAsync client)
{
  int rc;

  MQTTAsync_message message = MQTTAsync_message_initializer;
  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

  opts.onSuccess = onSend;
  opts.onFailure = onSendFailure;
  opts.context = client;

  message.payload = payload;
  message.payloadlen = (int)strlen(payload);
  message.qos = QOS;

  if ((rc = MQTTAsync_sendMessage(client, topic, &message, &opts)) != MQTTASYNC_SUCCESS)
  {
    MQTTAsync_destroy(client);
    exit(0);
  }
}

void sub_topic(char *topic_sub, MQTTAsync client)
{
  int rc;

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

  opts.onSuccess = onSubscribe;
  opts.onFailure = onSubscribeFailure;
  opts.context = client;

  if ((rc = MQTTAsync_subscribe(client, topic_sub, QOS, &opts)) != MQTTASYNC_SUCCESS)
  {
    printf("Erro: %d\n", rc);
    MQTTAsync_destroy(&client);
    exit(0);
  }
}

void temp_chat(char *rec_client, MQTTAsync client)
{
  sprintf(TOPICS_PENDENTS[atoi(rec_client)], "%.2s_Solicitation", rec_client);
}

void accept_chat(char *rec_client, MQTTAsync client)
{
  char topic_rec[20] = "", topic_chat[50] = "";
  int session_id = rand() % 1000;

  sprintf(topic_rec, "%.2s_Client", rec_client);
  sprintf(topic_chat, "%.2s_%.2s_Chat_%d", CLIENT_ID, rec_client, session_id);

  pthread_mutex_lock(&sub_topic_mutex);
  sub_topic(topic_chat, client);
  pthread_mutex_unlock(&sub_topic_mutex);

  pthread_mutex_lock(&topics_online_mutex);
  sprintf(TOPICS_ONLINE[atoi(rec_client)], "%s", topic_chat);
  pthread_mutex_unlock(&topics_online_mutex);

  sprintf(topic_chat, "AC_%.2s_%.2s_%.2s_Chat_%d", CLIENT_ID, CLIENT_ID, rec_client, session_id);

  pthread_mutex_lock(&pub_msg_mutex);
  pub_msg(topic_rec, topic_chat, client);
  pthread_mutex_unlock(&pub_msg_mutex);
}

void deny_chat(char *rec_client, MQTTAsync client)
{
  char topic_rec[20] = "", deny_msg[30] = "";
  int session_id = rand() % 1000;

  sprintf(topic_rec, "%.2s_Client", rec_client);
  sprintf(deny_msg, "DN_%.2s", CLIENT_ID);

  pthread_mutex_lock(&pub_msg_mutex);
  pub_msg(topic_rec, deny_msg, client);
  pthread_mutex_unlock(&pub_msg_mutex);
}

int msgarrvd(void *context, char *topic_name, int topic_len, MQTTAsync_message *message)
{
  int rc;
  char client_id[5];
  MQTTAsync client = (MQTTAsync)context;

  if (!strcmp(topic_name, CLIENT_TOPIC_CONTROL))
  {
    strncpy(client_id, (char *)message->payload, 2);
    printf("Usuário %.2s Enviou uma solicitação, selecione a opção 2 para mais informações!\n", (char *)message->payload);

    temp_chat(client_id, client);
  }
  else if (!strcmp(topic_name, CLIENT_TOPIC))
  {

    char *rep = strtok((char *)message->payload, "_");
    char *client_rec = strtok(NULL, "_");

    if (!strcmp(rep, "AC"))
    {
      printf("Chat do cliente %s foi aceito!\n", client_rec);

      char *new_topic = strtok(NULL, ";");

      pthread_mutex_lock(&sub_topic_mutex);
      sub_topic(new_topic, client);
      pthread_mutex_unlock(&sub_topic_mutex);

      pthread_mutex_lock(&topics_online_mutex);
      sprintf(TOPICS_ONLINE[atoi(client_rec)], "%s", new_topic);
      pthread_mutex_unlock(&topics_online_mutex);
    }
    else if (!strcmp(rep, "DN"))
    {
      printf("Chat do cliente %s foi negado!\n", client_rec);
    }
  }
  else
  {
    char *sender = strtok((char *)message->payload, ";");
    char *client_rec = strtok(NULL, ";");

    if (strcmp(client_rec, CLIENTE_ID_MASTER))
    {

      if (!strcmp(sender, "US"))
      {

        char *message_rec = strtok(NULL, ";");

        printf("\n------------ Nova Mensagem ------------\n");
        printf("Usuário %s: %s", client_rec, message_rec);
        printf("\n----------------------------------------\n");
      }

      else if (!strcmp(sender, "GP"))
      {

        char *rec_group = strtok(NULL, ";");
        char *message_rec = strtok(NULL, ";");

        printf("\n------------ Nova Mensagem ------------\n");
        printf("Usuário %s: %s", client_rec, message_rec);
        printf("\n----------------------------------------\n");
      }

      sleep(4);
      terminal_headers();
    }
  }
  return 1;
}

void accept_request(MQTTAsync client)
{
  int option, confirm, solicitations = 0;
  char client_id[4];

  for (int i = 0; i < 99; i++)
  {
    if (strlen(TOPICS_PENDENTS[i]) > 6)
    {
      printf("Identificação: %d, usuário:  %2d\n", i, i);
      solicitations++;
    }
  }

  if (!solicitations)
  {
    printf("Nenhuma solicitação encontrada\n");
    return;
  }

  printf("Aceitar ou recusar, número de indentificação: ");
  scanf("%d", &option);

  printf("Aceitar (1) ou recusar (2): ");
  scanf("%d", &confirm);

  strncat(client_id, TOPICS_PENDENTS[option], 2);

  delete_solicitation(option);

  if (confirm == 1)
  {
    printf("confirmação em andamento!\n");
    accept_chat(client_id, client);
  }
  else
  {
    printf("Excluindo solicitação\n");
    deny_chat(client_id, client);
  }
}

void send_message(MQTTAsync client)
{
  int sel, vrf = 0;
  char msg_topic[20] = "", message[64], payload[90];

  for (int i = 0; i < 99; i++)
  {
    if (strlen(TOPICS_ONLINE[i]) > 5)
    {
      printf("%d - Usuário %2d\n", i, i);
      vrf++;
    }
  }

  if (!vrf)
  {
    printf("Nenhum chat aberto\n");
    return;
  }
  printf("Mandar mensagem para o chat: ");

  scanf("%d", &sel);
  strcpy(msg_topic, TOPICS_ONLINE[sel]);

  printf("Mensagem: ");

  __fpurge(stdin);

  fgets(message, sizeof(message), stdin);

  sprintf(payload, "US;%s;%s;", CLIENTE_ID_MASTER, message);

  pthread_mutex_lock(&pub_msg_mutex);
  pub_msg(msg_topic, payload, client);
  pthread_mutex_unlock(&pub_msg_mutex);
}

void send_message_to_group(MQTTAsync client)
{
  int sel;
  char msg_topic[20] = "", message[64], payload[120];
  for (int i = 1; i < group_control; i++)
  {
    printf("%d - %s\n", i, GP_TOPICS_ONLINE[i]);
  }

  if (group_control == 1)
  {
    printf("Nenhum grupo encontrado!\n");
    return;
  }

  printf("Chat para mandar mensagem: ");

  scanf("%d", &sel);
  strcpy(msg_topic, GP_TOPICS_ONLINE[sel]);

  printf("Mensagem: ");

  __fpurge(stdin);

  fgets(message, sizeof(message), stdin);

  sprintf(payload, "GP;%s;%s;%s;", CLIENTE_ID_MASTER, msg_topic, message);

  pthread_mutex_lock(&pub_msg_mutex);
  pub_msg(msg_topic, payload, client);
  pthread_mutex_unlock(&pub_msg_mutex);
}

void subscribe_group(MQTTAsync client)
{
  int rc;
  char group[20], group_in_file[2], topic[30] = "";

  printf("Entrar no grupo: ");

  __fpurge(stdin);
  fgets(group, sizeof(group), stdin);

  size_t ln = strlen(group) - 1;
  if (*group && group[ln] == '\n')
    group[ln] = '\0';

  sprintf(topic, "%s_Group", group);

  printf("%s", topic);

  pthread_mutex_lock(&topics_online_mutex);
  strcpy(GP_TOPICS_ONLINE[group_control], topic);
  pthread_mutex_unlock(&topics_online_mutex);

  group_control++;

  pthread_mutex_lock(&sub_topic_mutex);
  sub_topic(topic, client);
  pthread_mutex_unlock(&sub_topic_mutex);
}

void start_chat(MQTTAsync client)
{
  __fpurge(stdin);

  char inicialize_client_id[10], topic[20] = "";

  printf("Iniciar chat com usuário (ID): ");

  fgets(inicialize_client_id, sizeof(inicialize_client_id), stdin);

  strncat(topic, inicialize_client_id, 2);
  strcat(topic, "_Control");

  printf("Enviando solicitação...\n");

  pthread_mutex_lock(&pub_msg_mutex);
  pub_msg(topic, CLIENT_ID, client);
  pthread_mutex_unlock(&pub_msg_mutex);
}