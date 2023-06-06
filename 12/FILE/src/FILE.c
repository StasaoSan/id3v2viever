//
// Created by Тихонов Александр on 13.12.2022.
//
#include "FILE.h"
#include <stdio.h>
FILE_status FILE_read(const char *name_file, uint8_t mas[], size_t MAX_COUNT){
    FILE *file = fopen(name_file, "rb");
    if (NULL == file){
        return FILE_DOESNT_EXISTS;
    }
    unsigned pos = 0;
    while (!feof(file) && pos < MAX_COUNT)
    {
        mas[ pos++ ] = fgetc( file );
    }
    fclose(file);
    return FILE_STATUS_SUCCESS;
}

FILE_status FILE_rewrite(const char *name_file, uint8_t mas[], size_t MAS_SIZE){
    FILE *file = fopen(name_file, "wb");
    if (NULL == file){
        return FILE_DOESNT_EXISTS;
    }
    unsigned pos = 0;
    while (pos < MAS_SIZE){
        fputc( mas[ pos++ ], file );
    }
    fclose (file);
    return FILE_STATUS_SUCCESS;
}
