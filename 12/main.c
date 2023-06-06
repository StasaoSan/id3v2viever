#include <stdio.h>
#include "commandLineInterface.h"
#include "TAG.h"


int main(int argc, char** argv) {

    const CLI_ComType type = CLI_CheckInputArgs(argc, argv);
    CLI_InitCmndPos( type, argc, argv );

    switch (type) {
        case CLI_TYPE_UNKNOWN: {
            printf("Неправильное кол-во входных параметров");
            return -1;
        };
        case CLI_TYPE_HELP: {
            CLI_HelpView();
        }
        case CLI_TYPE_SHOW:{
            TAG_Show(argc, argv);
        }
        default:
            break;
    }
    return 0;
}
