#ifndef PP_HAT_RUNTIME_H
#define PP_HAT_RUNTIME_H
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include  "runtime.h"


#define LOAD_FACTOR 0.75
#define FNV_OFFSET 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3
#define PPHAT_TALBE_GROWTH_RATE 2




typedef struct {
    size_t id;
    const  char* key;
    RunTimeValue value;
} EntryRunTime; 

typedef struct {
    size_t capaciy;
    size_t count;
    EntryRunTime* entries;
} PpHatRunTime;


PpHatRunTime* pphat_create_opt_runtime(size_t capacity,size_t count);
PpHatRunTime* pphat_create_runtime();  
void pphat_free_runtime(PpHatRunTime* table); 
void pphat_insert_runtime(PpHatRunTime** table,const char* key,RunTimeValue value);   
size_t pphat_get_hash_runtime(const char* id);
size_t pphat_fnv_runtime(const char* id);
void pphat_print_runtime(const PpHatRunTime* table);
PpHatRunTime* pphat_resize_runtime(PpHatRunTime* table);

void pphat_remove_runtime(const PpHatRunTime* table,const char* key);  
RunTimeValue pphat_get_runtime(const PpHatRunTime* table,const char* key);
bool pphat_contains_runtime(const PpHatRunTime* table,const char* key);


PpHatRunTime* pphat_create_opt_runtime(size_t capacity,size_t count) {
    PpHatRunTime* table  = (PpHatRunTime*) calloc(1,sizeof(PpHatRunTime));
    table->entries = (EntryRunTime*) calloc(capacity,sizeof(EntryRunTime));

    for (size_t i = 0; i < capacity; i++){
        table->entries[i].value = (RunTimeValue) { .type =  Noth ,.as = {0} };
    }
    

    table->capaciy = capacity;
    table->count = count;
    return table;
}
PpHatRunTime* pphat_create_runtime() {
    return pphat_create_opt_runtime(INIT_CAPACITY,0);
}





void pphat_free_runtime(PpHatRunTime* table) { 
    table->capaciy = 0;
    table->count = 0;
    free(table->entries);
    free(table);
}


void pphat_insert_runtime(PpHatRunTime** table,const char* key,RunTimeValue value) {    
    if((*table)->count + 1 >= (*table)->capaciy * LOAD_FACTOR) {
        (*table) = pphat_resize_runtime(*table);
    }

    size_t id = pphat_get_hash_runtime(key);
    size_t idx = id % (*table)->capaciy; 
    
    while (1) {
        if((*table)->entries[idx].key == NULL) {
            if((*table)->entries[idx].value.type == Noth) {
                (*table)->count++;
            }
            (*table)->entries[idx].key = key;
            (*table)->entries[idx].value = value;
            break;
        } else if(pphat_str_eq(key,(*table)->entries[idx].key)){
            (*table)->entries[idx].value = value;
            break;
        }
        idx = (idx + 1) % (*table)->capaciy;
    }
    

}

PpHatRunTime* pphat_resize_runtime(PpHatRunTime* table) {
    PpHatRunTime* tbl = pphat_create_opt_runtime(table->capaciy * PPHAT_TALBE_GROWTH_RATE,table->count);
    
    size_t idx = 0; 
    while (idx < table->capaciy) {
        if(table->entries[idx].key != NULL) {
            pphat_insert_runtime(&tbl,table->entries[idx].key,table->entries[idx].value);
        }
        idx++;
    }
    pphat_free_runtime(table);
    return tbl;
}


size_t pphat_get_hash_runtime(const char* id)  {
    return pphat_fnv_runtime(id);
}
size_t pphat_fnv_runtime(const char* id) {
    size_t hash = FNV_OFFSET;
    size_t curr = 0;
    while(id[curr] != 0) {
        hash = hash * FNV_PRIME;
        hash = hash ^ id[curr];
        curr++;
    }
    return hash;
}


RunTimeValue pphat_get_runtime(const PpHatRunTime* table,const char* key) {
    if(table->count  == 0) {
        return (RunTimeValue){.type = Noth,.as= 0};
    }   

    size_t id = pphat_get_hash_runtime(key);
    size_t idx = id % table->capaciy; 
    
    while (1) {
        if(table->entries[idx].key == NULL) {
            return (RunTimeValue){.type = Noth,.as= 0};
            break;
        } else if(pphat_str_eq(key,table->entries[idx].key)){
            return table->entries[idx].value;
            break;
        }
        idx = (idx + 1) % table->capaciy;
    }
}     

bool pphat_contains_runtime(const PpHatRunTime* table,const char* key) {
      if(table->count  == 0) {
        return false;
    }   

    size_t id = pphat_get_hash_runtime(key);
    size_t idx = id % table->capaciy; 
    
    while (1) {
        if(table->entries[idx].key == NULL) {
            return false;
            break;
        } else if(pphat_str_eq(key,table->entries[idx].key)){
            return true;
            break;
        }
        idx = (idx + 1) % table->capaciy;
    }  
}
void pphat_remove_runtime(const PpHatRunTime* table,const char* key) {
    if(table->count  == 0) {
        return;
    }   

    size_t id = pphat_get_hash_runtime(key);
    size_t idx = id % table->capaciy; 
    while (1) {
        if(table->entries[idx].key == NULL) {
            return;
        } else if(pphat_str_eq(key,table->entries[idx].key)){
            table->entries[idx].value.type = TombStone;
            table->entries[idx].key = NULL;
            return;
        }
        idx = (idx + 1) % table->capaciy;
    }
}  


void pphat_print_runtime(const PpHatRunTime* table) {
    printf("{\n");
    size_t idx = 0; 
    while(idx < table->capaciy) {
        if(table->entries[idx].key != NULL) {
            printf("  '%s' : ",table->entries[idx].key);
            runtime_print(table->entries[idx].value);
        }
        idx++;
    }
    printf("}\n");
}


#endif // PP_HAT_RUNTIME_H