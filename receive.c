//
// Created by fabie on 11/04/2021.
//

#include "receive.h"

//
// Created by fabie on 11/04/2021.
//

#include "send.h"
#include "utils.h"

int recoitFichier(size_t rest,rio_t *rio,char *buf, int fd) {
    int n, toRead;
    size_t size=rest;
    toRead = (MAXLINE <= rest) ? MAXLINE : rest;
    while (rest > 0 && (n = Rio_readnb(rio, buf, toRead)) > 0) {
        printf("|");
        rest -= n;
        Write(fd, buf, n);
        toRead = (MAXLINE <= rest) ? MAXLINE : rest;
    }
    printf("\n");
    Close(fd);
    if (rest == 0) printf("Received a total of %ld bytes\n", size);
    else printf("%ld bytes are missing\n", rest);
    return 0;
}

int lireMessage(rio_t *rio,char *buf,int connfd) {
    if (connfd >= 0){
        if ( rio_readlineb(rio, buf, MAXLINE) == 0) {
            fprintf(stderr,"An error occured during the transfer\n");
            return -1;
        }
        return 0;
    }
    return -1;
}

int lire(rio_t *rio,void *buf, size_t n, int connfd) {
    if (connfd >= 0){
        if ( rio_readnb(rio, buf, n) == 0 && n!=0) {
            fprintf(stderr,"An error occured during the transfer.");
            return -1;
        }
        return 0;
    }
    return -1;
}
