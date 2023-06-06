//
// Created by Тихонов Александр on 11.12.2022.
//

#include "commandLineInterface.h"
#include "commandLineInterfacePrivate.h"
#include <string.h>
#include <stdio.h>

static int CMND_VARIAT[ CMND_VARI_SIZE ] = { 1 };



CLI_ComType CLI_CheckInputArgs(int argc, char **argv){
    bool flag = CLI_CheckAgumentsCount(argc);
    if (flag){
        return CLI_GetCommandType(argc, argv);
    }
    return CLI_TYPE_UNKNOWN;
}

void CLI_HelpView(void){
    printf("\nЭто программа для работы с MP3 файлами типа id3v2\n\nДля того, чтобы вывести полный список тэгов необходимо:"
               "\n\t./name_programm --filepath=name_mp3.mp3 --show\n\n"
               "Для того, чтобы вывести определенный тэг необходимо:\n\t./name_programm --filepath=name_mp3.mp3 --get=name_tag"
               "\n\nДля того, чтобы изменить определенный тэг необходимо:\n\t./name_programm --filepath=name_mp3.mp3 --set=name_tag --value=new_value_tag");
}

void CLI_InitCmndPos( CLI_ComType crntCommType, int argc, char **mas )
{
    // const CLI_CommandDescription *descr = CLI_Make_Definition(crntCommType);

    switch( crntCommType )
    {
    case CLI_TYPE_SHOW:
        for( int i = 1; i < argc; ++i )
        {
            if( 0 == strcmp(mas[ i ], "--show") )
            {
                CMND_VARIAT[ CMND_VARI_SHOW ] = i;
                CMND_VARIAT[ CMND_VARI_FILE ] = i == 1 ? 2 : 1;
            }
        }
        break;

    case CLI_TYPE_HELP:
    default:

        break;

    }

}

int CLI_GetCmndPos( eCmndVariaty cmnd )
{
    return CMND_VARIAT[ cmnd ];
}


static CLI_ComType CLI_GetCommandType(int argc, char** argv){
    for (CLI_ComType type = CLI_TYPE_HELP; type < CLI_TYPE_UNKNOWN; ++type) {

        if(CLI_CheckAbstractCmd(CLI_Make_Definition(type), argc, argv)){
            return type;
        }
    }
    return CLI_TYPE_UNKNOWN;
}

static bool CLI_CheckAgumentsCount(int argc){
    static const int MAX_COUNT = 4;
    static const int MIN_COUNT = 2;
    return (argc <= MAX_COUNT && argc >= MIN_COUNT);
}

//функция, принимающая аргументы командной строки и находящая ту команду, которую мы хотим выполнить (если команда найдена, возвращает true, если не найдена - false)
static bool CLI_CheckAbstractCmd(const CLI_CommandDescription *description, int argc, char** argv){
    if (NULL == description){
        return false;
    }

    if( description->mArgc == argc )
    {
        // По количеству ожидаемых команд
        for( int cmndNumber = 0; cmndNumber < description->mArgc - 1; ++cmndNumber )
        {
            // По количеству возможных вариаций команд
            for( int cmndVariat = 0;
                 cmndVariat < description->mSignatureVariables[ cmndNumber ];
                 ++cmndVariat )
            {
                // Текущая команда
                const char * const expectedCmnd =
                        description->mSignatures[ cmndNumber ][ cmndVariat ];

                // По всем возможным позициям в командной строке
                for( int cmndLinePos = 1; // Так как 0 - имя программы
                     cmndLinePos < description->mArgc;
                     ++cmndLinePos )
                {
                    // Текущий полученный аргумент
                    const char * const currentArg = argv[ cmndLinePos ];


                    const char * isAny = strchr( expectedCmnd, '*' );



                    // Количество проверямых символов
                    const int symLen = isAny ? isAny - expectedCmnd - 1
                                             : max( strlen( currentArg ),
                                                    strlen( expectedCmnd ) );

                    // Равны
                    if( 0 == strncmp( currentArg,
                                      expectedCmnd,
                                      symLen))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

const char * getArg( const char *opt) {
    const char *delPos = strchr( opt, '=' );
    if( delPos ) {
        delPos++;
    }
    return delPos;
}

static const CLI_CommandDescription *CLI_Make_Definition(CLI_ComType type){
    const CLI_CommandDescription *result = NULL;
    static char *filepathCmndSign[] = {"--filepath=*"};
    switch (type) {

        case CLI_TYPE_HELP: {
            static const int HELP_EXPCTD_AGRS_COUNT = 2;
            static char *helpCmndSign[] = {"-h", "--help"};
            static int countCmnds[] = {sizeof(helpCmndSign) / sizeof(helpCmndSign[0])};
            static char **HELP_SIGNATURES[] = {helpCmndSign};

            //создание временной структуры CLI_CommandDescription и последующее присвоение ей значений
            static const CLI_CommandDescription EXPECTED_HELP =
                    {
                            .mArgc = HELP_EXPCTD_AGRS_COUNT,
                            .mSignatures = HELP_SIGNATURES,
                            .mSignatureVariables = countCmnds
                    };
            result = &EXPECTED_HELP;
        break;
        }
        case CLI_TYPE_GET: {
            static const int GET_EXPCTD_AGRS_COUNT = 3;
            static char *getCmndSign[] = { "--get=*"};
            static int countCmnds[] = {sizeof(getCmndSign) / sizeof(getCmndSign[0]), sizeof(filepathCmndSign) / sizeof(filepathCmndSign[0])};
            static char **GET_SIGNATURES[] = {getCmndSign, filepathCmndSign};


            static const CLI_CommandDescription EXPECTED_GET =
                    {
                            .mArgc = GET_EXPCTD_AGRS_COUNT,
                            .mSignatures = GET_SIGNATURES,
                            .mSignatureVariables = countCmnds
                    };
            result = &EXPECTED_GET;
            break;
        }
        case CLI_TYPE_SET:{
            static const int SET_EXPCTD_AGRS_COUNT = 4;
            static char *setCmndSign[] = {"--set=*"};
            static char *setValsign[] = {"--value=*"};
            static int countCmnds[] = {sizeof(setCmndSign) / sizeof(setCmndSign[0]), sizeof(filepathCmndSign) / sizeof(filepathCmndSign[0]), sizeof(setValsign) / sizeof(setValsign[0])};
            static char **SET_SIGNATURES[] = {setCmndSign, filepathCmndSign, setValsign};


            static const CLI_CommandDescription EXPECTED_SET =
                    {
                            .mArgc = SET_EXPCTD_AGRS_COUNT,
                            .mSignatures = SET_SIGNATURES,
                            .mSignatureVariables = countCmnds
                    };
            result = &EXPECTED_SET;
            break;
        }
        case CLI_TYPE_SHOW:{
            static const int SHOW_EXPCTD_AGRS_COUNT = 3;
            static char *showCmndSign[] = {"--show"};
            static int countCmnds[] = {sizeof(showCmndSign) / sizeof(showCmndSign[0]), sizeof(filepathCmndSign)/sizeof(filepathCmndSign[0])};
            static char **SHOW_SIGNATURES[] = {showCmndSign, filepathCmndSign};


            static const CLI_CommandDescription EXPECTED_SHOW =
                    {
                            .mArgc = SHOW_EXPCTD_AGRS_COUNT,
                            .mSignatures = SHOW_SIGNATURES,
                            .mSignatureVariables = countCmnds
                    };
            result = &EXPECTED_SHOW;
            break;
        }
        default:
            break;
    }
    return result;
}
