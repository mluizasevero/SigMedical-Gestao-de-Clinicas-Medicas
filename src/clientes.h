#ifndef CLIENTES_H
#define CLIENTES_H

typedef struct
{
    char cpf[15];
    char nome[50];
    char telefone[15];
    char email[50];
    int ativo;
} Cliente;

void modulo_clientes(void);

void cadastrar_cliente(void);
void pesquisar_cliente(void);
void alterar_cliente(void);
void excluir_cliente(void);
void listar_clientes(void);
void listar_clientes_ordenado(void);

#endif