#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "primitives.h"
#include "sequential.h"
#include "inputs.h"

int sequential_record_find(FILE *fp, const char *key, RECORD *rout){
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF || strcmp(block.records[i].event_id, key) > 0){
                return STAT_EOF;
            }
            if(strcmp(block.records[i].event_id, key) == 0 && block.records[i].flags != F_DEL){
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

int sequential_record_edit(FILE *fp, const char *key){
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF || strcmp(block.records[i].event_id, key) > 0){
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

int sequential_record_add(FILE *fp, RECORD r){

    r.flags = F_USE;

    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF){
                if(i < BLOCK_F - 1){
                    memcpy(&block.records[i], &r, RECORD_SIZE);
                    block.records[i + 1].flags = F_EOF;
                    block_write(fp, block, index * BLOCK_SIZE);
                    return STAT_OK;
                }
                else{
                    memcpy(&block.records[i], &r, RECORD_SIZE);
                    block_write(fp, block, index * BLOCK_SIZE);
                    BLOCK new = block_init();
                    block_write(fp, new, (index + 1) * BLOCK_SIZE);
                    return STAT_OK;
                }
            }
            if(strcmp(block.records[i].event_id, r.event_id) == 0){
                if(block.records[i].flags == F_DEL){
                    memcpy(&block.records[i], &r, RECORD_SIZE);
                    block_write(fp, block, index * BLOCK_SIZE);
                    return STAT_OK;
                }
                else return STAT_ERR;
            }
            if(strcmp(block.records[i].event_id, r.event_id) > 0){
                RECORD temp;
                memcpy(&temp, &block.records[i], RECORD_SIZE);
                memcpy(&block.records[i], &r, RECORD_SIZE);
                memcpy(&r, &temp, RECORD_SIZE);
                if(i == BLOCK_F - 1){
                    block_write(fp, block, index * BLOCK_SIZE);
                }
            }
            i++;
        }
        index++;
        status = block_read(fp, &block, index * BLOCK_SIZE);
    }
    return STAT_ERR;
}

int sequential_logical_delete(FILE *fp, const char *key){
    rewind(fp);
    BLOCK block = block_init();
    size_t index = 0;
    int status = block_read(fp, &block, index * BLOCK_SIZE);
    while(status == STAT_OK){
        int i = 0;
        while(i < BLOCK_F){
            if(block.records[i].flags == F_EOF || strcmp(block.records[i].event_id, key) > 0){
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