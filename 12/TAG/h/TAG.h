//
// Created by Тихонов Александр on 14.12.2022.
//

#ifndef INC_12_TAG_H
#define INC_12_TAG_H


#include <stdint.h>
#include <stdbool.h>
#include "commandLineInterface.h"
#include "FILE.h"

#pragma pack(push, 1)
typedef struct {
    uint8_t TAG_ID3[3];
    uint8_t TAG_ver[2];
    uint8_t TAG_flag;
    uint32_t TAG_size;
}TAG_id3v2Header;


typedef struct {
    uint8_t FrameId[4];     
    uint32_t size;
    uint16_t flags;
}TAG_FrameHeader;

typedef struct {
    uint32_t mSize;
    uint8_t mNumOfFlagBytes;
    uint8_t mFlags; // Maybe more ?
} sID3v2ExtendedHeader;
#pragma pack(pop)



typedef enum
{
    ID3_STATUS_SUCCESS, 
    ID3_STATUS_UNCORRECT_DATA, ///< Содержимое не совпало с ожиданмем (нет ID тэга, не та версия...)
    ID3_STATUS_UNCORRECT_PARAM ///< Пользователь передал некорректные параметры в функцию

} sID3Status;


/// @param [out] header
sID3Status TAG_readTagHeader(const uint8_t * data, TAG_id3v2Header  *header);

TAG_FrameHeader *TAG_GetNEXTFrameHeader(const TAG_FrameHeader *data);

TAG_FrameHeader *TAG_GetFirstFrame(TAG_id3v2Header *header);

int TAG_Show (int argc, char **argv);

void TAG_frameHeaderInfo(const TAG_FrameHeader *crntFrame);

void TAG_frameData(const TAG_FrameHeader *crntFrame);

void TAG_PrintFrameInfo(TAG_id3v2Header *header);

#endif //INC_12_TAG_H

