#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "primitives.h"
#include "serial.h"
#include "inputs.h"

int serial_init(const char *name){
    FILE *fp = fopen(name, "wb");
    if(fp == NULL) return STAT_ERR;
    BLOCK block = block_init();
    block_write(fp, block, 0);
    fclose(fp);
    return STAT_OK;
}

int serial_content_list(FILE *fp){
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_USE){
                record_print(block.records[i]);
            }
            i++;
        }
        index++;
        status = block_read(fp, &block, index * BLOCK_SIZE);
    }
    return STAT_OK;
}

int serial_record_find(FILE *fp, const char *key, RECORD *rout){
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF){      // Prvo se proverava EOF, vraca se status EOF
                return STAT_EOF;
            }
            if(strcmp(block.records[i].event_id, key) == 0 && block.records[i].flags != F_DEL){        // Ako je to taj kljuc i ako NIJE LOGICKI OBRISAN
                *rout = block.records[i];
                return STAT_OK;
            }
            i++;
        }
        index++;
        status = block_read(fp, &block, index * BLOCK_SIZE);
    }
    return STAT_ERR;
}

int serial_record_edit(FILE *fp, const char *key){
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF){
                return STAT_ERR;
            }
            if(strcmp(block.records[i].event_id, key) == 0 && block.records[i].flags != F_DEL){
                RECORD r = input_thingy();
                strcpy(r.event_id, key);
                memcpy(&block.records[i], &r, RECORD_SIZE);
                block_write(fp, block, index * BLOCK_SIZE);
                return STAT_OK;
            }
            i++;
        }
        index++;
        status = block_read(fp, &block, index * BLOCK_SIZE);
    }
    return STAT_ERR;
}

int serial_record_add(FILE *fp, RECORD r){

    r.flags = F_USE;                                                                                // NAJBITNIJE!!!

    RECORD temp;
    if(serial_record_find(fp, r.event_id, &temp) == STAT_OK) return STAT_ERR;                       // Provera da li blok sa tim kljucem postoji
    BLOCK block = block_init();
    fseek(fp, -(int)BLOCK_SIZE, SEEK_END);
    block_read(fp, &block, ftell(fp));
    int i = 0;
    while(i < BLOCK_F){
        if(block.records[i].flags == F_EOF){
            memcpy(&block.records[i], &r, RECORD_SIZE);
            i++;
            break;
        }
        i++;
    }
    if(i < BLOCK_F){
        fseek(fp, -(int)BLOCK_SIZE, SEEK_END);
        block.records[i].flags = F_EOF;
        block_write(fp, block, ftell(fp));
        return STAT_OK;
    }
    else{
        fseek(fp, -(int)BLOCK_SIZE, SEEK_END);
        block_write(fp, block, ftell(fp));

        BLOCK new = block_init();
        block_write(fp, new, ftell(fp));
        return STAT_OK;
    }
    return STAT_ERR;
}

int serial_logical_delete(FILE *fp, const char *key){
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF){
                return STAT_ERR;
            }
            if(strcmp(block.records[i].event_id, key) == 0){
                block.records[i].flags = F_DEL;
                block_write(fp, block, index * BLOCK_SIZE);
                return STAT_OK;
            }
            i++;
        }
        index++;
        status = block_read(fp, &block, index * BLOCK_SIZE);
    }
    return STAT_ERR;
}

int serial_physical_delete(FILE *fp, const char *key){

    RECORD temp;
    if(serial_record_find(fp, key, &temp) != STAT_OK){
        return STAT_ERR;
    }

    rewind(fp);
    BLOCK block = block_init();
    BLOCK block_next = block_init();
    size_t index = 0;
    char cur_key[13];
    strcpy(cur_key, key);
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF){
                if(i == 0){
                    fseek(fp, 0, SEEK_END);
                    long a = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    ftruncate(fileno(fp), a - (int)BLOCK_SIZE);
                }
                return STAT_OK;
            }
            if(strcmp(block.records[i].event_id, cur_key) == 0){
                int j = i + 1;
                while(j < BLOCK_F){
                    memcpy(&block.records[j-1], &block.records[j], RECORD_SIZE);
                    j++;
                }
                int status_next = block_read(fp, &block_next, (index + 1)*BLOCK_SIZE);
                if(status_next == STAT_OK){
                    memcpy(&block.records[BLOCK_F - 1], &block_next.records[0], RECORD_SIZE);
                    strcpy(cur_key, block_next.records[0].event_id);
                }
                block_write(fp, block, index * BLOCK_SIZE);

                i = BLOCK_SIZE - 1;                                                                 // GLAVNI STOS

            }
            i++;
        }
        index++;
        status = block_read(fp, &block, index * BLOCK_SIZE);
    }
    return STAT_OK;                                                         // VRACA SE OK!
}
