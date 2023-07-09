#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

///Estrutura do Token.
typedef struct {
    char** tokens;
    int current_token;
} Parser;

///Recebe a lista de tokens como parâmetro
///e aloca espaço para um novo objeto Parser. 
///após inicializa os membros(tokens e current_token)
///e retorna o objeto.
Parser* create_parser(char** tokens) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->current_token = 0;
    return parser;
}

///Avança o indice para o próximo token dfa lista.
void advance(Parser* parser) {
    parser->current_token++;
}

bool parse_exp(Parser* parser);
bool parse_term(Parser* parser);
bool parse_factor(Parser* parser);
bool parse_bool(Parser* parser);

///Função principal que inicia a análise do parser
///recebe um parser por parâmetro, retorna o res, da análise.
bool parse(Parser* parser) {
    return parse_exp(parser);
}

///Responsável por analisar expressões na gramática definida,
///segue a regra exp -> exp OR term | term.
bool parse_exp(Parser* parser) {
    bool left = parse_term(parser);
    while (parser->tokens[parser->current_token] != NULL && strcmp(parser->tokens[parser->current_token], "OR") == 0) {
        advance(parser);
        bool right = parse_term(parser);
        left = left || right;
    }
    return left;
}


bool parse_term(Parser* parser) {
    bool left = parse_factor(parser);
    while (parser->tokens[parser->current_token] != NULL && strcmp(parser->tokens[parser->current_token], "AND") == 0) {
        advance(parser);
        bool right = parse_factor(parser);
        left = left && right;
    }
    return left;
}

bool parse_factor(Parser* parser) {
    if (parser->tokens[parser->current_token] != NULL && strcmp(parser->tokens[parser->current_token], "(") == 0) {
        advance(parser);
        bool result = parse_exp(parser);
        if (parser->tokens[parser->current_token] != NULL && strcmp(parser->tokens[parser->current_token], ")") == 0) {
            advance(parser);
            return result;
        }
        else {
            printf("Parêntese de fechamento ausente!\n");
            exit(1);
        }
    }
    else if (parser->tokens[parser->current_token] != NULL && strcmp(parser->tokens[parser->current_token], "NOT") == 0) {
        advance(parser);
        return !parse_factor(parser);
    }
    else {
        return parse_bool(parser);
    }
}

///Responsável por analisar os valores booleanos na gramática
///definida, verifica se o token atual é "T" ou "F" e 
///avança para o próximo token, se corresponder a um valor
///booleano válido.
bool parse_bool(Parser* parser) {
    if (parser->tokens[parser->current_token] != NULL && strcmp(parser->tokens[parser->current_token], "T") == 0) {
        advance(parser);
        return true;
    }
    else if (parser->tokens[parser->current_token] != NULL && strcmp(parser->tokens[parser->current_token], "F") == 0) {
        advance(parser);
        return false;
    }
    else {
        printf("Valor booleano inválido!\n");
        exit(1);
    }
}

int main() {
    ///Vetor de tokens
    char* tokens[] = {"(", "T", "AND", "F", ")", "OR", "NOT", "T", NULL};
    int num_tokens = sizeof(tokens) / sizeof(tokens[0]) - 1;

    ///rebe uma lista de tokens.
    Parser* parser = create_parser(tokens);
    ///recebe o resultado da análise e mostra o resultado.
    bool result = parse(parser);

    if (result)
        printf("Resultado: true\n");
    else
        printf("Resultado: false\n");

    free(parser);

    return 0;
}