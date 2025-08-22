#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "movimentacao.h"

typedef struct {
    int id;
    char nome[50];
    int quantidade;
    char validade[11]; 
} Produto;


int ler_produtos(Produto produtos[]);
void salvar_produtos(Produto produtos[], int total);
void modulo_estoque(Produto produtos[], int *total, Movimentacao movimentacoes[], int *total_movimentacoes);


void cadastrar_produto(Produto produtos[], int *total_produtos);
void pesquisar_produto(Produto produtos[], int total_produtos);
void gerenciar_lotes(Produto produtos[], int total_produtos);
void movimentar_estoque(Produto produtos[], int total_produtos, Movimentacao movimentacoes[], int *total_movimentacoes);


void gerar_relatorios_estoque(Produto produtos[], int total_produtos);
void relatorio_itens_falta(Produto produtos[], int total_produtos);
void relatorio_validade_proxima(Produto produtos[], int total_produtos);
void relatorio_historico_movimentacoes(void);

#endif

