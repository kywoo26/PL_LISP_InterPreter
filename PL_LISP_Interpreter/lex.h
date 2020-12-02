#pragma once
#include <stdio.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define SYMBOL 99

/* Token codes */
#define INT 10
#define ID 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LPAREN 25
#define RPAREN 26
#define SEMI_COLON 27
#define LESS_OP 28
#define GREAT_OP 29
#define QUOTES 30
#define DOUBLE_QUOTES 31
#define NUMBER_SIGN 32
#define COMMA 33

/* Token Struct */
typedef struct Token
{
    int type;
    char value[100];
} Token;

void addChar();
void getChar();
void getNonBlank();

#ifdef __cplusplus
extern "C" {
#endif

    Token* lex(char* input);

#ifdef __cplusplus
}
#endif
