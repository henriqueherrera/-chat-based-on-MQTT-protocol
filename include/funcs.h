#include "MQTTAsync.h"

void delete_solicitation(int id);
void pub_msg(char *topic, char *payload, MQTTAsync client);
void sub_topic(char *topic_sub, MQTTAsync client);
void temp_chat(char *rec_client, MQTTAsync client);
void accept_chat(char *rec_client, MQTTAsync client);
void deny_chat(char *rec_client, MQTTAsync client);
int msgarrvd(void *context, char *topic_name, int topic_len, MQTTAsync_message *message);
void accept_request(MQTTAsync client);
void send_message(MQTTAsync client);
void send_message_to_group(MQTTAsync client);
void subscribe_group(MQTTAsync client);
void start_chat(MQTTAsync client);