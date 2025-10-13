#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

typedef struct {
    int  id_produto;
    char tipo[10];      
    int  quantidade;
    char data[11];      
} Movimentacao;


// Registra e salva uma nova movimentação no arquivo “movimentacoes.dat”
// id_produto O ID do produto que foi movimentado.
// tipo_mov O tipo da movimentação
// quantidade A quantidade de itens movimentados

void registrar_movimentacao(int id_produto, const char* tipo_mov, int quantidade);

void exibir_historico_movimentacoes(void);

#endif
