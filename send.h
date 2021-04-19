//
// Created by fabie on 11/04/2021.
//

#include "csapp.h"

#ifndef SERVERFTP_SEND_H
#define SERVERFTP_SEND_H

int envoieFichier(size_t size, size_t offset,rio_t *rio,int *connfd);
void askNewCmd(int connfd,size_t code);
int envoyer(int fd, void *usrbuf, size_t n);
int envoyerCmd(char *buf,int clientfd);
#endif //SERVERFTP_SEND_H
