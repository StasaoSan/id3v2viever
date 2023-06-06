//
// Created by Тихонов Александр on 11.12.2022.
//

#ifndef INC_12_COMMANDLINEINTERFACEPRIVATE_H
#define INC_12_COMMANDLINEINTERFACEPRIVATE_H

#include "commandLineInterface.h"

//Создание нового (typedef) типа данных, структурного вида CLI_CommandDescription
typedef struct
{
    // Ожидаемое количество аргументов командной строки
    int mArgc;

    // Вариации команд [по количеству арг/команд][По количеству видов команды h/help]
    char *** mSignatures;

    // Количество вариаций по количеству команд
    int *mSignatureVariables;

} CLI_CommandDescription;


static CLI_ComType CLI_GetCommandType(int argc, char** argv);
static bool CLI_CheckAgumentsCount(int argc); //Тест на то, что число аргументов командной строки больше двух но меньше 4
static bool CLI_CheckAbstractCmd(const CLI_CommandDescription *description, int argc, char** argv); //тест на то, что
static const CLI_CommandDescription *CLI_Make_Definition(CLI_ComType type);


static inline int max(int a, int b){
    return a > b ? a : b;
}

#endif //INC_12_COMMANDLINEINTERFACEPRIVATE_H
