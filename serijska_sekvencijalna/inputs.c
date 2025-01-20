#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "serial.h"
#include "sequential.h"
#include "inputs.h"
#include "primitives.h"

void header(){
    printf("\n1. INICIJALIZUJ DATOTEKU\n");
    printf("2. UPISI NOVU STVARCICU\n");
    printf("3. ISPISI SVE STVARCICE\n");
    printf("4. TRAZI STVARCICU PO KLJUCU\n");
    printf("5. IZMENI STVARCICU\n");
    printf("6. LOGICKI OBRISI STVARCICU\n");
    printf("7. FIZICKI OBRISI STVARCICU\n");
    printf("0. IZLAZ\n");
}

RECORD input_thingy(){
    RECORD ret;
    ret.flags = F_USE;
    char flags;
    char event_id[13];
    char date_time[20];
    char event_type[8];
    char user_id[11];
    char event_name[21];

    printf("Unesite event_id: ");
    scanf("%s", event_id);
    printf("Unesite date_time: ");
    scanf("%s", date_time);
    printf("Unesite event_type: ");
    scanf("%s", event_type);
    printf("Unesite user_id: ");
    scanf("%s", user_id);
    printf("Unesite event_name: ");
    scanf("%s", event_name);

    strcpy(ret.event_id, event_id);
    strcpy(ret.date_time, date_time);
    strcpy(ret.event_type, event_type);
    strcpy(ret.user_id, user_id);
    strcpy(ret.event_name, event_name);

    return ret; 
}

void main_loop(){
    while(1){
        header();
        printf("Odaberite opciju: ");
        char c;
        scanf("%c", &c);
        if(!isdigit(c)){
            printf("GRESKA!");
        }
        else{
        switch(c){
            case '1':{
                char ime[31];
                printf("Unesite ime nove datoteke: ");
                scanf("%s", ime);
                serial_init(ime);
                break;
            }
            case '2':{
                char ime[31];
                printf("Unesite ime datoteke: ");
                scanf("%s", ime);
                FILE *fp = fopen(ime, "rb+");
                if(fp == NULL){
                    printf("Greska pri otvaranju datoteke!\n");
                    break;
                }
                RECORD r = input_thingy();
                sequential_record_add(fp, r);
                fclose(fp);
                break;
            }
            case '3':{
                char ime[31];
                printf("Unesite ime datoteke: ");
                scanf("%s", ime);
                FILE *fp = fopen(ime, "rb+");
                if(fp == NULL){
                    printf("Greska pri otvaranju datoteke!\n");
                    break;
                }
                serial_content_list(fp);
                fclose(fp);
                break;
            }
            case '4':{
                char ime[31];
                printf("Unesite ime datoteke: ");
                scanf("%s", ime);
                FILE *fp = fopen(ime, "rb+");
                if(fp == NULL){
                    printf("Greska pri otvaranju datoteke!\n");
                    break;
                }
                RECORD r;
                char key[13];
                printf("Unesite kljuc: ");
                scanf("%s", key);
                if(sequential_record_find(fp, key, &r) != STAT_OK){
                    printf("greska!\n");
                    break;
                }
                record_print(r);
                fclose(fp);
                break;
            }
            case '5':{
                char ime[31];
                printf("Unesite ime datoteke: ");
                scanf("%s", ime);
                FILE *fp = fopen(ime, "rb+");
                if(fp == NULL){
                    printf("Greska pri otvaranju datoteke!\n");
                    break;
                }
                char key[13];
                printf("Unesite kljuc: ");
                scanf("%s", key);

                if(sequential_record_edit(fp, key) != STAT_OK){
                    printf("Greska!");
                }
                fclose(fp);
                break;
            }

            case '6':{
                char ime[31];
                printf("Unesite ime datoteke: ");
                scanf("%s", ime);
                FILE *fp = fopen(ime, "rb+");
                if(fp == NULL){
                    printf("Greska pri otvaranju datoteke!\n");
                    break;
                }
                char key[13];
                printf("Unesite kljuc: ");
                scanf("%s", key);

                if(sequential_logical_delete(fp, key) != STAT_OK){
                    printf("greska!");
                }
                fclose(fp);
                break;
            }
            case '7':{
                char ime[31];
                printf("Unesite ime datoteke: ");
                scanf("%s", ime);
                FILE *fp = fopen(ime, "rb+");
                if(fp == NULL){
                    printf("Greska pri otvaranju datoteke!\n");
                    break;
                }
                char key[13];
                printf("Unesite kljuc: ");
                scanf("%s", key);

                if(serial_physical_delete(fp, key) != STAT_OK){
                    printf("greska!");
                }
                fclose(fp);
                break;
            }
            case '0': return;
            default: break;
        }
        
        }
        char dummy;
        scanf("%c", &dummy);
    }
}