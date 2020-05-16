/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Petooo
 *
 * Created on December 4, 2017, 12:14 PM
 */

#include "klient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>
#include <errno.h>

#include "CMemLeak.h"
/*
 * 
 */

/*
 * Hlavna funkcia main
 */
int main(int argc, char** argv) {
    
    if (argc < 4){
        vypisChybu("Klienta je nutne spustit s nasledujucimi argumentmi: adresa port pouzivatel.");
    }
    
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        vypisChybu("Zadany server neexistuje.");
    }
    
    int port = atoi(argv[2]);
    if (port <= 0) {
	vypisChybu("Port musi byt cele cislo vacsie ako 0.");
    } 
    
    
    
    char meno[100];
    strcpy(meno,argv[3]);
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        vypisChybu("Chyba - socket.");        
    }
    
    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);
    
    if (connect(sock,(struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        vypisChybu("Chyba - connect.");        
    }
    printf ("Klient sa uspesne pripojil na server... \n\n");
    
    vypis();
    
    pthread_t thread;
    pthread_create(&thread, NULL,prichadzajuceSpravy, (void*)sock);
    
    write(sock, meno, strlen(meno));
    
    int n;
    while (1){
        fgets(retazec,1023,stdin);   
        n = write(sock,retazec,strlen(retazec)); 
        if (n < 0) 
          vypisChybu("Chyba - write");        
        bzero(retazec,1024);
    } 
    return (0);
}

