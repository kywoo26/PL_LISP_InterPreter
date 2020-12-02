/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages.
 * Pearson, 10th edition.
 *
 */

 /* front.c - a lexical analyzer system for simple arithmetic expressions */
#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "lex.h"

/* Global Variable */
char* sentence;

int nextToken;
int charClass;
char lexeme[100];
char nextChar;
int lexLen = 0;
int nextToken;

//FILE* in_fp;
//FILE* out_fp;

/* Local Function declarations */

/******************************************/
/* main driver                            */
/******************************************/
//int main()
//{
//    char* line = malloc(sizeof(char) * MAX_LINE_LENGTH);
//
//    printf("> ");
//    gets(line);
//
//    lex(line);
//
//    free(line);
//
//    return 0;
//}

/******************************************
 * lookup - a function to lookup operators
 * and parentheses and return the token
 ******************************************/
int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LPAREN;
        break;
    case ')':
        addChar();
        nextToken = RPAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case ';':
        addChar();
        nextToken = SEMI_COLON;
        break;
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        break;
    case '<':
        addChar();
        nextToken = LESS_OP;
        break;
    case '>':
        addChar();
        nextToken = GREAT_OP;
        break;
    case '\'':
        addChar();
        nextToken = QUOTES;
        break;
    case '\"':
        addChar();
        nextToken = DOUBLE_QUOTES;
        break;
    case '#':
        addChar();
        nextToken = NUMBER_SIGN;
        break;
    case '.':
        addChar();
        nextToken = COMMA;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/**************************************************/
/* addChar - a function to add nextChar to lexeme */
/**************************************************/
void addChar() {
    if (lexLen <= 98) {  // max length of Lexime is 99
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0; // '\0'
    }
    else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character
          of input and determine its character class */
          /*****************************************************/
void getChar() {
    if ((nextChar = *sentence) != NULL) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = SYMBOL;

        sentence++;
    }
    else {
        charClass = EOF;
    }


}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
           returns a non-whitespace character        */
           /*****************************************************/
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}


//int lex(char* input)
//{
//    sentence = *input;
//
//    for (int i = 0; i < strlen(input); i++)
//    {
//        // Ignore the space
//        if (isspace(input[i]))
//            continue;
//        
//    }
//}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
         expressions                                 */
         /*****************************************************/
Token* lex(char* input) {
    sentence = input;

    Token* tokens = malloc(sizeof(Token));
    tokens->type = EOF;

    int index = 0;
    getChar();
    while (nextChar != NULL)
    {
        lexLen = 0;
        getNonBlank();

        switch (charClass) {
            /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = ID;
            break;

            /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT;
            break;

            /* Parentheses and operators */
        case SYMBOL:
            lookup(nextChar);
            getChar();
            break;
            /* EOF */
        /*case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;*/
        } /* End of switch */

        tokens[index].type = nextToken;
        strcpy(tokens[index].value, lexeme);
        index++;

        // The last element of tokens has 'EOF' as tokenType.
        void* newTokens = realloc(tokens, sizeof(Token) * (index + 1));
        if (newTokens != NULL)
        {
            tokens = newTokens;
            tokens[index].type = EOF;   // 필요 없을 듯한데? 내일 확인
        }
    }

    return tokens;
} /* End of function lex */
