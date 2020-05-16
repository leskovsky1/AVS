/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   server.h
 * Author: Petooo
 *
 * Created on December 17, 2017, 10:37 AM
 */

#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_POCET 5

/*globalne premenne*/    
extern char escsq[8];                   
extern char pomoc[7];              
extern char ukazMeno[10];             
extern char aktivniKlienti[9];
extern char buffer[1024];

/*
 * Struktura klient
 */
typedef struct klient{
    struct sockaddr_in addr;
    int idSoket;
    char meno[32];
} KLIENT;

/*
 * Struktura list
 */
typedef struct Klienti {
    KLIENT *klienti[MAX_POCET];
    int pocetKlientov;
} LIST;

/*
 * List klientov
 */
extern LIST zoznamKlientov;

void vypisChybu(char *str);
bool pridajKlienta(KLIENT * klient);
bool vymazKlienta(KLIENT *klient);
void posliVsetkym(const char *sprava,const KLIENT *klient);
void posliSebe(const char *sprava, const KLIENT *klient);
void ukazAktivnychKlientov(KLIENT *klient);
bool *obsluhaKlienta(KLIENT *klient);
void *nacitaj(void);

#ifdef __cplusplus
}
#endif

#endif /* SERVER_H */

