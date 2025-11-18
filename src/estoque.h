#ifndef ESTOQUE_H
#define ESTOQUE_H
#include "movimentacao.h"

typedef struct
{
    int id;
    char nome[50];
    int quantidade;
    char validade[11];
    int ativo;
} Produto;

void modulo_estoque(void);
void cadastrar_produto(void);
void pesquisar_produto(void);
void gerenciar_lotes(void);
void movimentar_estoque(void);

void gerar_relatorios_estoque(void);
void relatorio_itens_falta(void);
void relatorio_validade_proxima(void);

// Funções de relatório adicionais
void relatorio_completo_estoque(void);
void relatorio_por_nome_estoque(void);
void relatorio_por_status_estoque(void);
void submenu_relatorios_estoque(void);

// Função auxiliar (Para uso interno)
int id_produto_existe(int id);

#endif