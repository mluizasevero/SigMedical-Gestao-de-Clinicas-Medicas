#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "movimentacao.h"
#include "utils.h"
// Não precisa de validadores.h pois não há input direto do usuário

#define MOVIMENTACOES_FILE DATA_DIR PATH_SEPARATOR "movimentacoes.dat"

void registrar_movimentacao(int id_produto, const char *tipo_mov, int quantidade)
{
    Movimentacao nova;
    FILE *arq_movimentacoes;

    // Os dados (id_produto, tipo_mov, quantidade) são assumidos como válidos,
    // pois foram validados no módulo estoque antes de chamar esta função.
    nova.id_produto = id_produto;
    // O tipo já está validado para ser "Entrada" ou "Saida" no estoque.c
    strncpy(nova.tipo, tipo_mov, sizeof(nova.tipo) - 1);
    nova.tipo[sizeof(nova.tipo) - 1] = '\0';
    nova.quantidade = quantidade;

    // data e hora atual para registrar a movimentação
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(nova.data, sizeof(nova.data), "%d/%m/%Y", &tm);

    arq_movimentacoes = fopen(MOVIMENTACOES_FILE, "ab");
    if (arq_movimentacoes == NULL)
    {
        printf("ERRO CRITICO: Nao foi possivel abrir o arquivo de movimentacoes!\n");
        return;
    }

    fwrite(&nova, sizeof(Movimentacao), 1, arq_movimentacoes);
    fclose(arq_movimentacoes);
}

// função para ler e exibir o histórico, chamada nos relatórios.
void exibir_historico_movimentacoes(void)
{
    Movimentacao mov_lida;
    FILE *arq_movimentacoes;

    limparTela();
    printf("----------------------------------------\n");
    printf("///    Historico de Movimentacoes    ///\n");
    printf("----------------------------------------\n");

    arq_movimentacoes = fopen(MOVIMENTACOES_FILE, "rb");
    if (arq_movimentacoes == NULL)
    {
        printf("Nenhuma movimentacao de estoque registrada.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("ID Produto | Tipo       | Quantidade | Data\n");
    printf("-----------|------------|------------|------------\n");

    while (fread(&mov_lida, sizeof(Movimentacao), 1, arq_movimentacoes))
    {
        printf("%-10d | %-10s | %-10d | %s\n",
               mov_lida.id_produto, mov_lida.tipo,
               mov_lida.quantidade, mov_lida.data);
    }

    fclose(arq_movimentacoes);
    pressioneEnterParaContinuar();
}
