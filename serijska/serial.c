#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "primitives.h"
#include "serial.h"

int serial_init(const char *name){
    FILE *fp;
    if(file_init(name, &fp) == STAT_OK){
        BLOCK block = block_init();
        block_write(fp, block, 0);
        fclose(fp);
        return STAT_OK;
    }
    err("Inicijalizacija datoteke");
    return STAT_ERR;
}


int serial_content_list(FILE *fp){
    if(fp == NULL){
        err("Listanje sadrzaja datoteke; handle ka datoteci nevalidan");
        return STAT_ERR;
    }
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int i;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags & (F_EOF|F_DEL) != 0){
                record_print(block.records[i]);
            }
            i++;
        }
        index++;
        status = block_read(fp, &block, index * BLOCK_SIZE);
    }
    return STAT_OK;
}

int serial_record_find(FILE *haystack, const char *needle, RECORD *r){
    if(haystack == NULL){
        err("Trazenje sloga; handle ka datoteci nevalidan");
        return STAT_ERR;
    }
    rewind(haystack);
    BLOCK block = block_init();
    size_t index = 0;
    int i;
    int status = block_read(haystack, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        i = 0;
        while(i < BLOCK_F){
            if((block.records[i].flags & F_EOF) != 0){
                return STAT_EOF;
            }
            if(strcmp(block.records[i].event_id, needle) == 0 && (block.records[i].flags & F_DEL) == 0){
                *r = block.records[i];
                return STAT_OK;
            }
            i++;
        }
        index++;
        status = block_read(haystack, &block, index * BLOCK_SIZE);
    }
    err("Trazenje sloga; \n\n...\n\n\n...how did we get here?");
    return STAT_ERR;
}

int serial_record_add(FILE *fp, RECORD r){
    if(fp == NULL){
        err("Upis sloga; handle ka datoteci nevalidan");
        return STAT_ERR;
    }
    RECORD old;
    if(serial_record_find(fp, r.event_id, &old) == STAT_OK){
        err("Upis sloga; kljuc vec postoji");
        return STAT_ERR;
    }
    BLOCK block = block_init();
    fseek(fp, -(int)BLOCK_SIZE, SEEK_END);
    block_read(fp, &block, ftell(fp));
    int i = 0;
    while(i < BLOCK_F){
        if((block.records[i].flags & F_EOF) != 0){
            memcpy(&block.records[i], &r, RECORD_SIZE);
            i++;
            break;
        }
        i++;
    }
    if(i < BLOCK_F){
        block.records[i].flags = F_EOF;
        fseek(fp, -(int)BLOCK_SIZE, SEEK_END);
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
}