#ifndef CLIENTES_H
#define CLIENTES_H

typedef struct {
    char cpf[15];
    char nome[50];
    char telefone[15];
    char email[50];
} Cliente;


void salvar_clientes(Cliente clientes[], int total_clientes);
int ler_clientes(Cliente clientes[]);


void cadastrar_cliente(Cliente clientes[], int* total_clientes);

void modulo_clientes(Cliente clientes[], int* total_clientes);
void pesquisar_cliente(Cliente clientes[], int total_clientes);
void alterar_cliente(Cliente clientes[], int total_clientes);
void excluir_cliente(Cliente clientes[], int* total_clientes);

#endif