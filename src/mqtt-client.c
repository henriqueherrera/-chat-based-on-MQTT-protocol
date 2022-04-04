#include <flow.h>
#include <terminal.h>
#include <funcs.h>
#include "global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <unistd.h>
#include "MQTTAsync.h"

int execute_chat()
{
  MQTTAsync client;
  MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
  MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
  int option, rc;

  __fpurge(stdin);

  printf("Qual o seu id único: ");
  fgets(CLIENT_ID, sizeof(CLIENT_ID), stdin);

  strncat(CLIENTE_ID_MASTER, CLIENT_ID, 2);

  strncat(CLIENT_TOPIC_CONTROL, CLIENT_ID, 2);
  strcat(CLIENT_TOPIC_CONTROL, "_Control");

  strncat(CLIENT_TOPIC, CLIENT_ID, 2);
  strcat(CLIENT_TOPIC, "_Client");

  disc_opts.onSuccess = onDisconnect;
  disc_opts.onFailure = onDisconnectFailure;

  if ((rc = MQTTAsync_create(&client, ADDRESS, CLIENTE_ID_MASTER, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
  {
    rc = EXIT_FAILURE;
    goto exit;
  }

  else if ((rc = MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL)) != MQTTASYNC_SUCCESS)
  {
    rc = EXIT_FAILURE;
    goto destroy_exit;
  }

  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 0;
  conn_opts.context = client;
  conn_opts.onSuccess = onConnect;
  conn_opts.onFailure = onConnectFailure;

  if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
  {
    rc = EXIT_FAILURE;
    goto destroy_exit;
  }

  do
  {
    erase();
    printf("**************** MQTT-CHAT ****************\n");
    terminal_headers();
    scanf("%d", &option);

    switch (option)
    {
    case 1:
      start_chat(client);
      break;
    case 2:
      accept_request(client);
      break;
    case 3:
      send_message(client);
      break;
    case 4:
      subscribe_group(client);
      break;
    case 5:
      send_message_to_group(client);
      break;
    case 6:
      rc = MQTTAsync_disconnect(client, &disc_opts);

      if (rc != MQTTASYNC_SUCCESS)
      {
        rc = EXIT_FAILURE;
      }
      goto exit;
      break;
    default:
      printf("Escolha uma opção valida!\n");
      break;
    }

  } while (option != 6);

destroy_exit:
  MQTTAsync_destroy(&client);
exit:
  return rc;
}