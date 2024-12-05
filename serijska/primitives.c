#include <stdio.h>
#include <string.h>

#include "primitives.h"

int file_init(const char *name, FILE **fp){
    *fp = fopen(name, "wb");
    if(*fp == NULL){
        err("Kreiranje fajla");
        return STAT_ERR;
    }
    return STAT_OK;
}

BLOCK block_init(){
    char zeros[BLOCK_SIZE] = {};
    BLOCK block;
    memcpy(&block, zeros, BLOCK_SIZE);
    block.records[0].flags = F_EOF;
    return block;
}

int block_write(FILE *fp, BLOCK block, long pos){
    fseek(fp, pos, SEEK_SET);
    if(fwrite(&block, BLOCK_SIZE, 1, fp) != 1){
        err("Upis bloka");
        return STAT_ERR;
    }
    return STAT_OK;
}

int block_read(FILE *fp, BLOCK *block, long pos){
    if(fp == NULL){
        err("Citanje bloka; handle ka datoteci nevalidan");
        return STAT_ERR;
    }
    fseek(fp, pos, SEEK_SET);
    if(fread(block, BLOCK_SIZE, 1, fp) != 1){
        return STAT_EOF;
    }
    return STAT_OK;
}

void record_print(RECORD r){
    printf("-------SLOG-------\n");
    printf("Flags: %d\n", r.flags);
    printf("Evidencioni broj: %s\n", r.event_id);
    printf("Vreme dogadjaja: %s\n", r.date_time);
    printf("Tip dogadjaja: %s\n", r.event_type);
    printf("ID korisnika: %s\n", r.user_id);
    printf("Naziv dogadjaja: %s\n", r.event_name);
    printf("------------------\n");
}