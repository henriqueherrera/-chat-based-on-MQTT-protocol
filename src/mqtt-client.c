#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"
#include <unistd.h>
#include <global.h>

#define ADDRESS     "tcp://localhost:1883"
#define TOPIC       "MQTT Examples"
#define QOS         1
#define TIMEOUT     10000L

int disc_finished = 0;
int subscribed = 0;

volatile MQTTAsync_token deliveredtoken;

int finished = 0;

MQTTAsync connection;

void connlost(void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);

	printf("Reconnecting\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
 		finished = 1;
	}
}


void onDisconnect(void* context, MQTTAsync_successData* response)
{
	printf("Successful disconnection\n");
	finished = 1;
}

void onSend(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;


	//opts.onSuccess = onDisconnect;
	opts.context = client;
}

void sendMessage(char topic[50], char message[50]) {
	MQTTAsync client = (MQTTAsync) connection;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

    
    opts.onSuccess = onSend;
	opts.context = client;

	pubmsg.payload = message;
	pubmsg.payloadlen = strlen(message);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	deliveredtoken = 0;

	if ((rc = MQTTAsync_sendMessage(client, topic, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start sendMessage, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void inputMessage() {
	char topic[50];
    char message[50];

    printf("Topico: ");
    scanf("%s", topic);

    printf("Mensagem: ");
    scanf("%s", message);

	sendMessage(topic, message);
}

void onSubscribe(void* context, MQTTAsync_successData* response)
{
	printf("Subscribe succeeded\n");
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Subscribe failed, rc %d\n", response->code);
}

void inputSubscribe() {
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

	char topic[50];

	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = connection;

 	printf("Topico: ");
    scanf("%s", topic);
	
	if (MQTTAsync_subscribe(connection, topic, QOS, &opts) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start subscribe, return code \n");
		finished = 1;
	}
}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Connect failed, rc %d\n", response ? response->code : 0);
	finished = 1;
}


void onConnect(void* context, MQTTAsync_successData* response)
{
	char message[0];
	sendMessage(client_id, message);

	printf("Successful connection\n");
    is_loading = 0;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

void * start_client (void *vargp)
{
 	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;
	int ch;

	MQTTAsync_create(&connection, ADDRESS, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = connection;

	if ((rc = MQTTAsync_connect(connection, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	
	if ((rc = MQTTAsync_setCallbacks(connection, connection, connlost, msgarrvd, NULL)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to set callbacks, return code %d\n", rc);
		rc = EXIT_FAILURE;
	}

	while (!finished)
		usleep(10000L);

	if (finished)
		goto exit;

	destroy_exit:
		MQTTAsync_destroy(&connection);
	exit:
		return NULL;
}