#include <pthread.h>

char CLIENT_TOPIC_CONTROL[30] = "";
char CLIENT_TOPIC[30] = "";
int group_control = 1;
int is_finished = 0, finished = 0;

char CLIENT_ID[5];
char CLIENTE_ID_MASTER[3];

char TOPICS_ONLINE[100][30];
char TOPICS_PENDENTS[100][30];
char GP_TOPICS_ONLINE[99][30];

char CLIENT_TOPIC_CONTROL[30];
char CLIENT_TOPIC[30];
int group_control;
int is_finished, finished;

pthread_mutex_t pub_msg_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sub_topic_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t topics_online_mutex = PTHREAD_MUTEX_INITIALIZER;
