#pragma once

#include <pthread.h>

#define ADDRESS "tcp://localhost:1883"
#define QOS 1
#define TIMEOUT 10000L

extern char CLIENT_ID[5];
extern char CLIENTE_ID_MASTER[3];

extern char TOPICS_ONLINE[100][30];
extern char TOPICS_PENDENTS[100][30];
extern char GP_TOPICS_ONLINE[99][30];

extern char CLIENT_TOPIC_CONTROL[30];
extern char CLIENT_TOPIC[30];
extern int group_control;
extern int is_finished, finished;

extern pthread_mutex_t pub_msg_mutex;
extern pthread_mutex_t sub_topic_mutex;
extern pthread_mutex_t topics_online_mutex;
