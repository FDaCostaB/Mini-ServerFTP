//
// Created by fabie on 11/04/2021.
//

#include "csapp.h"

#ifndef SERVERFTP_UTILS_H
#define SERVERFTP_UTILS_H

int getCommand(char ** cmd, char *cmdLine);
int commandeReconnu(char** cmd);
int lireEntier(rio_t *rio1, int *val, int defaultVal);
int sizeFile(char *name);
void closeConnection(int *fd);
void afficher_code(int code);

#endif //SERVERFTP_UTILS_H
