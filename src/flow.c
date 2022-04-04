#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <pthread.h>
#include <unistd.h>
#include "MQTTAsync.h"
#include "global.h"

void connlost(void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\nConnection lost\n");
	if (cause)
		printf(" Causa: %s\n", cause);

	printf("Reconnecting...\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		finished = 1;
	}
}

void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
	is_finished = 1;
}

void onSuccess(void *context, MQTTAsync_successData *response)
{
	is_finished = 1;
}

void onDisconnect(void *context, MQTTAsync_successData *response)
{
	is_finished = 1;
}

void onSendFailure(void *context, MQTTAsync_failureData *response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	printf("Erro ao enviar a mensagem %d. Erro %d\n", response->token, response->code);
	opts.onSuccess = onDisconnect;
	opts.onFailure = onDisconnectFailure;
	opts.context = client;
}

void onSend(void *context, MQTTAsync_successData *response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	opts.onSuccess = onDisconnect;
	opts.onFailure = onDisconnectFailure;
	opts.context = client;
}

void onSubscribe(void *context, MQTTAsync_successData *response)
{
	printf("Iniciado com sucesso!\n");
}

void onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
	printf("Erro ao iniciar.\n");
}

void onConnect(void *context, MQTTAsync_successData *response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	int rc;

	printf("Conectado com sucesso!\n");

	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;

	if ((rc = MQTTAsync_subscribe(client, CLIENT_TOPIC_CONTROL, QOS, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Falha ao iniciar.\n");
		finished = 1;
	}

	if ((rc = MQTTAsync_subscribe(client, CLIENT_TOPIC, QOS, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Falha ao iniciar");
		finished = 1;
	}
}

void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
	printf("Falha na conexão com o broker. Erro: %d\n", response->code);
	finished = 1;
}
