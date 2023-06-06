//
// Created by Тихонов Александр on 13.12.2022.
//

#ifndef INC_12_FILE_H
#define INC_12_FILE_H
#include <stddef.h>
#include <stdint.h>

typedef enum{
    FILE_STATUS_SUCCESS,
    FILE_DOESNT_EXISTS
}FILE_status;

FILE_status FILE_read(const char *name_file, uint8_t mas[], size_t MAX_COUNT);
FILE_status FILE_rewrite(const char *name_file, uint8_t mas[], size_t MAS_SIZE);

#endif //INC_12_FILE_H

