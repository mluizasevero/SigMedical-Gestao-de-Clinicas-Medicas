#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H


typedef struct {
    int id_produto;
    char tipo[10];       
    int quantidade;
    char data[11];       
} Movimentacao;

void registrar_movimentacao(int id_produto, const char* tipo, int quantidade,
                            Movimentacao movimentacoes[], int* total_movimentacoes);

int ler_movimentacoes(Movimentacao movimentacoes[]);

void salvar_movimentacoes(Movimentacao movimentacoes[], int total_movimentacoes);

#endif
