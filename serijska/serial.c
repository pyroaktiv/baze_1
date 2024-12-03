#include <stdio.h>
#include <stdlib.h>

#include "primitives.h"
#include "serial.h"

int serial_init(const char *name){
    FILE *fp;
    if(file_init(name, &fp) == STAT_OK){
        BLOCK block = block_init();
        block_write(fp, block, SEEK_SET);
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
    fseek(fp, 0, SEEK_SET);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
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