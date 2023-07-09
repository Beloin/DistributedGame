//
// Created by beloin on 09/07/23.
//

#ifndef DISTRIBUTED_SYS_SERVER_H
#define DISTRIBUTED_SYS_SERVER_H
#define MAX_SERVICES 4

char *services[MAX_SERVICES];

int server(char *port);
int client(char *port);



#endif //DISTRIBUTED_SYS_SERVER_H
