//
// Created by fabie on 11/04/2021.
//
#include "csapp.h"

#ifndef SERVERFTP_RECEIVE_H
#define SERVERFTP_RECEIVE_H

int lireMessage(rio_t *rio,char *buf, int connfd);
int lire(rio_t *rio,void *buf, size_t n, int connfd);
int recoitFichier(size_t rest,rio_t *rio,char *buf, int fd);
#endif //SERVERFTP_RECEIVE_H
