
#include "MQTTAsync.h"

extern void connlost(void *context, char *cause);
extern void onDisconnectFailure(void *context, MQTTAsync_failureData *response);
extern void onDisconnect(void *context, MQTTAsync_successData *response);
extern void onSendFailure(void *context, MQTTAsync_failureData *response);
extern void onSend(void *context, MQTTAsync_successData *response);
extern void onSubscribe(void *context, MQTTAsync_successData *response);
extern void onSubscribeFailure(void *context, MQTTAsync_failureData *response);
extern void onConnect(void *context, MQTTAsync_successData *response);
extern void onConnectFailure(void *context, MQTTAsync_failureData *response);