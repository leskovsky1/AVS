/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   klient.h
 * Author: Petooo
 *
 * Created on December 17, 2017, 9:53 AM
 */

#ifndef KLIENT_H
#define KLIENT_H

#include <pthread.h>
#ifdef __cplusplus
extern "C" {
#endif
    
#define DLZKA_RETAZCA 1024  

/*
 * Globalna premenna
 */
extern char retazec[DLZKA_RETAZCA]; 

void vypisChybu(char *str);
void vypis(void);
void *prichadzajuceSpravy(int sock);

#ifdef __cplusplus
}
#endif

#endif /* KLIENT_H */

