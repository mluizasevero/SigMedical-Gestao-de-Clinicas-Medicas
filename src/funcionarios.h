#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

#include <stdio.h>

typedef struct {
    int id;
    char nome[50];
} Funcionario;

void modulo_funcionarios(void);
void cadastrar_funcionario(void);
void pesquisar_funcionario(void);
void alterar_funcionario(void);
void excluir_funcionario(void);

#endif
