//
// Created by Тихонов Александр on 14.12.2022.
//

#ifndef INC_12_TAG_PRIVATE_H
#define INC_12_TAG_PRIVATE_H

#include <stdint.h>

static const uint8_t TAG_ID[] = {'I', 'D', '3'};
static const uint8_t TAG_VER[] = {4, 0};

static inline uint32_t swapByteOrder( uint32_t data );

static  bool checkHeaderCorrect( const TAG_id3v2Header * restrict header );

static uint32_t TAG_size32To28( uint32_t data );

static uint32_t TAG_GetTagSize(uint32_t data);



#endif //INC_12_TAG_PRIVATE_H
