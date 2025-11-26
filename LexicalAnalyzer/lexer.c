#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static FILE *source = NULL;   
Token token;

static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

static const char* operators[] = {
    "+", "-", "*", "/", "%", "=", "!", "<", ">", "&", "|",
    "==", "!=", "<=", ">=", "&&", "||", "++", "--",
    "+=", "-=", "*=", "/=", "%=",
    NULL
};
static const char* specialCharacters = ",;{}()[]";

void initializeLexer(const char* filename)
{
    source = fopen(filename, "r");
    if (source == NULL)
    {
        printf("FAILED TO INITIALISE THE FILE\n");
        return;
    }
}

int isKeyword(const char* str)
{
    for (int i = 0; i < MAX_KEYWORDS; i++)
        if (strcmp(keywords[i], str) == 0)
            return 1;
    return 0;
}

int isOperator(const char *str)
{
    for (int i = 0; operators[i] != NULL; i++)
    {
        if (strcmp(operators[i], str) == 0)
            return 1;
    }
    return 0;
}

int isNumber(const char *s)
{
    int i = 0, has_digit = 0, has_dot = 0, has_exp = 0;

    // Optional leading + or -
    if (s[i] == '+' || s[i] == '-')
        i++;

    while (s[i] != '\0')
    {
        char c = s[i];

        if (isdigit(c))
        {
            has_digit = 1;
        }
        else if (c == '.')
        {
            if (has_dot || has_exp) 
                return 0;   // only one dot, no dot after exponent

            has_dot = 1;
        }
        else if (c == 'e' || c == 'E')
        {
            // exponent only allowed if digits exist before
            if (!has_digit || has_exp)
                return 0;

            has_exp = 1;
            has_digit = 0;   // require digits AFTER exponent

            // optional sign after exponent
            if (s[i+1] == '+' || s[i+1] == '-')
                i++;
        }
        else
        {
            return 0;   // invalid char
        }

        i++;
    }

    return has_digit;  // must have digits at end
}


int isSpecialCharacter(char ch)
{
    for (int i = 0; specialCharacters[i]; i++)
        if (specialCharacters[i] == ch)
            return 1;
    return 0;
}

int isConstant(const char* str)
{
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i]))
            return 0;
    return 1;
}

int isIdentifier(const char* str)
{
    if (!isalpha(str[0]) && str[0] != '_')
        return 0;
    for (int i = 1; str[i]; i++)
        if (!isalnum(str[i]) && str[i] != '_')
            return 0;
    return 1;
}

void categorizeToken(Token* token)
{
    if (token->type == PREPROCESSOR || token->type == STRING_LITERAL)
        return;
    if (isKeyword(token->lexeme))
        token->type = KEYWORD;
    else if (isOperator(token->lexeme))
        token->type = OPERATOR;
    else if (isSpecialCharacter(token->lexeme[0]))
        token->type = SPECIAL_CHARACTER;
    else if (isNumber(token->lexeme))
        token->type = CONSTANT;
    else if (isConstant(token->lexeme))
        token->type = CONSTANT;
    else if (isIdentifier(token->lexeme))
        token->type = IDENTIFIER;
    else
        token->type = UNKNOWN;
}

Token getNextToken()
{
    Token token;
    token.type = UNKNOWN; 
    char ch;
    int i = 0;
    char buffer[256];

    #define IS_OP_CHAR(c) (strchr("+-*/%=!<>|&", (c)) != NULL)

    /* ---------------------------------------
       Skip whitespace
       --------------------------------------- */
    while ((ch = fgetc(source)) != EOF && isspace(ch));

    /* ---------------------------------------
       EOF Protection
       --------------------------------------- */
    if (ch == EOF) 
    {
        token.lexeme[0] = '\0';
        token.type = UNKNOWN;
        return token;
    }

    /* ---------------------------------------
       COMMENT HANDLING
       --------------------------------------- */
    if (ch == '/')
    {
        char next = fgetc(source);

        /* 1. // Single-line comment */
        if (next == '/')
        {
            while ((ch = fgetc(source)) != EOF && ch != '\n');
            return getNextToken();
        }

        /* 2. Multi-line comment */ 
        if (next == '*')
        {
            int prev = 0;
            while ((ch = fgetc(source)) != EOF)
            {
                if (prev == '*' && ch == '/')
                    break;
                prev = ch;
            }
            return getNextToken();
        }

        /* It's just '/' operator */
        ungetc(next, source);
    }

    /* ---------------------------------------
       1. PREPROCESSOR DIRECTIVE (# ... \n)
       --------------------------------------- */
    if (ch == '#') 
    {
        i = 0;
        buffer[i++] = ch;

        while ((ch = fgetc(source)) != EOF && ch != '\n')
            buffer[i++] = ch;

        buffer[i] = '\0';
        strcpy(token.lexeme, buffer);
        token.type = PREPROCESSOR;
        return token;
    }

    /* ---------------------------------------
       2. STRING LITERAL "...."
       --------------------------------------- */
    if (ch == '"') 
    {
        buffer[i++] = ch;
        while ((ch = fgetc(source)) != EOF) 
        {
            buffer[i++] = ch;

            if (ch == '\\')      // Escape seq
            {
                char next = fgetc(source);
                if (next == EOF) break;
                buffer[i++] = next;
            }

            if (ch == '"')       // Closing "
                break;
        }

        buffer[i] = '\0';
        strcpy(token.lexeme, buffer);
        token.type = STRING_LITERAL;
        return token;
    }

    /* ---------------------------------------
       3. MULTI-CHAR OPERATOR
       --------------------------------------- */
    if (IS_OP_CHAR(ch))
    {
        char op[3] = {0};
        op[0] = ch;

        char next = fgetc(source);
        if (next != EOF)
        {
            op[1] = next;

            /* If not valid 2-char operator, rollback */
            if (!isOperator(op))
            {
                op[1] = '\0';
                ungetc(next, source);
            }
        }

        strcpy(token.lexeme, op);
        categorizeToken(&token);
        return token;
    }

    /* ---------------------------------------
       4. SPECIAL CHARACTER
       --------------------------------------- */
    if (isSpecialCharacter(ch))
    {
        buffer[0] = ch;
        buffer[1] = '\0';
        strcpy(token.lexeme, buffer);
        categorizeToken(&token);
        return token;
    }

    /* ---------------------------------------
       5. IDENTIFIER / NUMBER / KEYWORD
       --------------------------------------- */
    buffer[i++] = ch;

    while ((ch = fgetc(source)) != EOF &&
           !isspace(ch) &&
           !IS_OP_CHAR(ch) &&
           !isSpecialCharacter(ch))
    {
        buffer[i++] = ch;
    }

    buffer[i] = '\0';

    if (ch != EOF)
        ungetc(ch, source);

    strcpy(token.lexeme, buffer);
    categorizeToken(&token);  
    return token;
}
