
#ifndef ESTOQUE_H
#define ESTOQUE_H

typedef struct {
    int  id;
    char nome[50];
    int  quantidade;
    char validade[11]; 
    int  ativo;        
} Produto;


void modulo_estoque(void);
void cadastrar_produto(void);
void pesquisar_produto(void);
void movimentar_estoque(void);


void listar_produtos(void); 
void gerar_relatorios_estoque(void);
void relatorio_itens_falta(void);
void relatorio_validade_proxima(void);

#endif

