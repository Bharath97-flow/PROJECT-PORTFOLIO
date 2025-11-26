#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    initializeLexer(argv[1]); // opens file

    Token token;
    while (1)
    {
        token = getNextToken();

        if (token.lexeme[0] == '\0') // EOF
            break;

        printf("Lexeme: %-20s Type: ", token.lexeme);

        switch (token.type)
        {
            case KEYWORD: printf("KEYWORD\n"); break;
            case IDENTIFIER: printf("IDENTIFIER\n"); break;
            case OPERATOR: printf("OPERATOR\n"); break;
            case CONSTANT: printf("CONSTANT\n"); break;
            case SPECIAL_CHARACTER: printf("SPECIAL_CHARACTER\n"); break;
            case PREPROCESSOR: printf("PREPROCESSOR\n"); break;
            case STRING_LITERAL: printf("STRING_LITERAL\n"); break;
            default: printf("UNKNOWN\n"); break;
        }
    }

    return 0;
}
