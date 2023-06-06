//
// Created by Тихонов Александр on 11.12.2022.
//

#ifndef INC_12_COMMANDLINEINTERFACE_H
#define INC_12_COMMANDLINEINTERFACE_H
#include <stdbool.h>


/// @todo Получение значений аргументов etc getParams? ...

//Создание нового (typedef) перечисляемого (enum) типа данных CLI_ComType
typedef enum
{
    CLI_TYPE_HELP,
    CLI_TYPE_SHOW,
    CLI_TYPE_GET,
    CLI_TYPE_SET,

    CLI_TYPE_UNKNOWN
}CLI_ComType;

typedef enum {
    CMND_VARI_HELP,

    CMND_VARI_SHOW,
    CMND_VARI_GET,
    CMND_VARI_SET,
    CMND_VARI_VALUE,
    CMND_VARI_FILE,

    CMND_VARI_SIZE
} eCmndVariaty;

//Функция, проверяющая, что введено правильное число аргументов командной строки (для help - 2, для set - 4)
CLI_ComType CLI_CheckInputArgs(int argc, char **mas);

void CLI_HelpView(void);


void CLI_InitCmndPos( CLI_ComType crntCommType, int argc, char **mas );
const char * getArg( const char *opt );
int CLI_GetCmndPos( eCmndVariaty cmnd );


#endif //INC_12_COMMANDLINEINTERFACE_H
