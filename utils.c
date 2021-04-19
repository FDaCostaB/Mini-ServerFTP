//
// Created by fabie on 11/04/2021.
//

#include "utils.h"
#include "ftp.h"

int getCommand(char ** cmd, char *cmdLine){
    if(strlen(cmdLine) <= 3){
        return -1;
    } else {
        if(cmdLine[3]=='\n'){
            cmdLine[3]='\0';
            cmd[0]=&cmdLine[0];
            cmd[1]=NULL;
        } else if (cmdLine[3]==' '){
            cmdLine[strlen(cmdLine) - 1] = '\0';
            cmdLine[3]='\0';
            cmd[0]=&cmdLine[0];
            cmd[1]=&cmdLine[4];
        } else {
            return -1;
        }
    }
    if(commandeReconnu(cmd)) return 0;
    else return -1;
}

int commandeReconnu(char** cmd){
    return (cmd != NULL  && (strcmp(cmd[0],"bye") == 0 || strcmp(cmd[0],"get")==0 || strcmp(cmd[0],"cnt")==0));
}

int lireEntier(rio_t *rio1, int *val, int defaultVal){
    if (rio_readnb(rio1, val, sizeof(size_t)) < 0) {
        printf("Error: No size received\n");
        *val=defaultVal;
        return -1;
    }
    return 0;
}

int sizeFile(char *name) {
    int size;
    FILE * f;
    if ((f = fopen(name, "r")) != NULL) {
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fclose(f);
        return size;
    } else {
        return -1;
    }
}

void closeConnection(int *fd){
    //printf("Client has exit before the end of transfer.\n");
    close(*fd);
    *fd=-1;
    printf("Connection closed\n");
}

void afficher_code(int code){
    switch (code) {
        case FICHIER_MANQUANT:
            printf("Le fichier n'existe pas sur le serveur\n");
            break;
        case FICHIER_VIDE:
            printf("Le fichier est vide\n");
            break;
        case CMD_INCONNU:
            printf("Commande inconnu\n");
            break;

    }
}
