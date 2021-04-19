//
// Created by fabie on 29/03/2021.
//

#ifndef SERVERFTP_FTP_H
#define SERVERFTP_FTP_H

#define FICHIER_VIDE 0
#define CMD_INCONNU 1
#define FICHIER_MANQUANT 3

int ftp(int *connfd,char **cmd);

#endif //SERVERFTP_FTP_H
