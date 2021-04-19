//
// Created by fabie on 11/04/2021.
//

#include "send.h"
#include "utils.h"

void askNewCmd(int connfd,size_t code){
    size_t size = 0;
    envoyer(connfd, &size, sizeof(size_t));
    envoyer(connfd,&code,sizeof(size_t));
}

int envoyerCmd(char *buf,int clientfd){
    printf("Veuillez entrez une commande valide :\n");
    Fgets(buf, MAXLINE, stdin);
    while (strlen(buf) == 0) {
        Fgets(buf, MAXLINE, stdin);
        printf("Veuillez entrez une commande valide :\n");
    }
    //Envoie de la commande
    envoyer(clientfd, buf, strlen(buf));
    return 0;
}

int envoieFichier(size_t size, size_t offset,rio_t *rio,int *connfd){
    int n;
    int rest = size-offset;
    int toSend = (MAXLINE <= rest) ? MAXLINE : rest;
    char buf[MAXLINE];

    if(*connfd<0){
        fprintf(stderr,"An error occurred : Transfer canceled (%ld bytes sent in total).\n", size-rest);
        return -1;
    }

    while ( *connfd>0 && (n = rio_readnb(rio, buf, toSend)) != 0) {
        rest -= n;
        rio_writen(*connfd, buf, n);
        //Le client peut mourir ici renvoie 0 si client a crash
        //Pour écriture, on reçoit un signal SIGPIP
        toSend = (MAXLINE <= rest) ? MAXLINE : rest;
        printf("|");
    }
    printf("\n");
    if(n<0 || rest !=0){
        fprintf(stderr,"An error occurred : Transfer canceled (%ld bytes sent in total).\n", size-rest);
    	return -1;
    } else {
        printf("Transfer successfully completed (%ld bytes sent in total).\n", size);
    }
    return 0;
}

int envoyer(int fd, void *usrbuf, size_t n) {
    if(fd>0) {
        rio_writen(fd, usrbuf, n);
        return 0;
    }
    return -1;
}
