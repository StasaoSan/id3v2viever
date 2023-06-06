//
// Created by Тихонов Александр on 14.12.2022.
//

#include "TAG.h"
#include "TAG_private.h"
#include "FILE.h"
#include <string.h>
#include <stdio.h>
/* ---  Определение API --- */

sID3Status TAG_readTagHeader(const uint8_t *data, TAG_id3v2Header *header)
{
    if( !data || !header )
        return ID3_STATUS_UNCORRECT_PARAM;

    memcpy( header, data, sizeof( TAG_id3v2Header ) );

    if( !checkHeaderCorrect( header ) )
        return ID3_STATUS_UNCORRECT_DATA;

    return ID3_STATUS_SUCCESS;
}


static volatile void *TEST = NULL;



TAG_FrameHeader *TAG_GetFirstFrame(TAG_id3v2Header *header){
    uint32_t shift = sizeof(TAG_id3v2Header);

    TEST = header;

    if (header->TAG_flag & (1 << 6) ) {
        sID3v2ExtendedHeader *extHeader = (uint8_t *) header + shift;
        shift += TAG_GetTagSize(extHeader->mSize );
    }
    return (TAG_FrameHeader * )( (uint8_t *)header + shift );
}

static uint32_t TAG_size32To28( uint32_t data ){
    // Инициализация, разбираем слово побайтово
    uint8_t bytes[ sizeof( data ) ] = {};
    static const uint8_t LEAST_7_BIT_MASK = 0x7F;

    for( unsigned byte = 0; byte < sizeof( data ); ++byte ) {
        const uint8_t shift = 8 * byte;
        bytes[ byte ] = ( data & ( LEAST_7_BIT_MASK << shift ) ) >> shift;
    }
    // Заполнение

    uint32_t res = 0;
    for( unsigned byte = 0; byte < sizeof( data ); ++byte ) {
        const uint8_t shift = byte ? 8 * byte - byte : 0;
        res |= ( bytes[ byte ] << shift );
    }
    return res;
}


TAG_FrameHeader *TAG_GetNEXTFrameHeader(const TAG_FrameHeader *data) {
    uint8_t *memPtr = (uint8_t *) data;
    memPtr += (sizeof(TAG_FrameHeader) + TAG_GetTagSize(data->size));
    TAG_FrameHeader *nextFrameHeader = (TAG_FrameHeader *) memPtr;

    if (0 == TAG_GetTagSize(nextFrameHeader->size))
        nextFrameHeader = NULL;
    return nextFrameHeader;
}

void TAG_frameHeaderInfo(const TAG_FrameHeader *crntFrame)
{
    char id[5] = {};
    memcpy(id, crntFrame, 4);
    printf("Frame ID: %s \n"
           "Frame Size: %d,\n",
           id, swapByteOrder(crntFrame->size ) - 2 ); // - 2 - еще есть флаги, которые как раз и занимают эти 2 байта
}

void TAG_frameData(const TAG_FrameHeader *crntFrame){
    const uint8_t *data = ( ( uint8_t * )crntFrame ) + sizeof( TAG_FrameHeader );

    printf("Frame Data: ");

    while( data < (( uint8_t * )crntFrame + (sizeof(TAG_FrameHeader) +
                                             swapByteOrder(crntFrame->size))) /*( uint8_t * )next*/ )
    {
        printf( "%c", *data++ );
    }
    printf( "\n\n");
}

int TAG_Show (int argc, char **argv) {
    enum {
        MAS_SIZE = 100000000
    };
    static uint8_t mas[MAS_SIZE] = {0};
    const char *file_name = getArg(argv[ CLI_GetCmndPos(CMND_VARI_FILE) ]);
    if (FILE_read(file_name, mas, MAS_SIZE)) {
        printf("File open error!\n");
        return -2;
    } else {
        TAG_id3v2Header header;
        if( TAG_readTagHeader(mas, &header) ) {
            printf("Uncorrect Header!\n");
            return -1;
        }
        TAG_PrintFrameInfo( ( TAG_id3v2Header* ) mas );
    }
    return 0;
}


void TAG_PrintFrameInfo(TAG_id3v2Header *header){

    TAG_FrameHeader *frame = TAG_GetFirstFrame(header);
    TAG_frameHeaderInfo(frame);
    TAG_frameData(frame);

    while (( uint8_t* )frame < ( (uint8_t*)header + TAG_GetTagSize(header->TAG_size) )){

        frame = TAG_GetNEXTFrameHeader(frame);

        if (frame == NULL){
            break;
        }
        else {
            TAG_frameHeaderInfo(frame);
            TAG_frameData(frame);
        }
    }
}

/// @param [out] dataSize
/// @return Data's ptr
uint8_t * getFrameData(  TAG_FrameHeader *crntHeader, int *dataSize ) {

    if (crntHeader == NULL){
        return NULL;
    }
    *dataSize = crntHeader->size; // Если в size не входит размер заголовка
    // Иначе crntHeader->size - sizeof( FrameHeader )

    return (uint8_t *)(crntHeader + sizeof( TAG_FrameHeader ));
}


static bool checkHeaderCorrect( const TAG_id3v2Header * restrict header ) {
    return ! ( memcmp(TAG_ID, header->TAG_ID3, sizeof(TAG_ID) / sizeof (TAG_ID[0])) ||
               memcmp(TAG_VER, header->TAG_ver, sizeof(TAG_VER) / sizeof (TAG_VER[0])) );
}

static inline uint32_t swapByteOrder( uint32_t data ) {
    const uint8_t mass[4] = {(data & 0xFF), (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF };
    return mass[3] | (mass[2] << 8) | (mass[1] << 16) | (mass[0] << 24);
}

static uint32_t TAG_GetTagSize(uint32_t data){
    return TAG_size32To28(swapByteOrder(data));
}
