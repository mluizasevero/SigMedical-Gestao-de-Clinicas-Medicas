#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "movimentacao.h"
#include "utils.h"

// formato de leitura para CSV
int ler_movimentacoes(Movimentacao movimentacoes[]) {
    // p/ abrir o arquivo csv no modo de leitura "r"
    FILE *arquivo = fopen("movimentacoes.csv", "r");
    if (arquivo == NULL) {
        return 0; // retorna 0 movimentações se não existir
    }


    char linha_cabecalho[256];
    fgets(linha_cabecalho, 256, arquivo);

    int i = 0;
    while(fscanf(arquivo, "%d,%[^,],%d,%[^\n]\n",
                   &movimentacoes[i].id_produto,
                   movimentacoes[i].tipo,
                   &movimentacoes[i].quantidade,
                   movimentacoes[i].data) == 4) {
        i++;
    }

    fclose(arquivo);
    return i;
}

void registrar_movimentacao(int id_produto, const char* tipo_mov, int quantidade, Movimentacao movimentacoes[], int* total_movimentacoes) {
    if (*total_movimentacoes < 1000) {
        Movimentacao nova;
        nova.id_produto = id_produto;
        strcpy(nova.tipo, tipo_mov);
        nova.quantidade = quantidade;

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        strftime(nova.data, sizeof(nova.data), "%d/%m/%Y", &tm);

        // add movimentação no array
        movimentacoes[*total_movimentacoes] = nova;
        (*total_movimentacoes)++;
    } else {
        printf("Nao e possivel registrar mais movimentacoes. Limite atingido.\n");
    }
}

void salvar_movimentacoes(Movimentacao movimentacoes[], int total_movimentacoes) {
    FILE *arquivo = fopen("movimentacoes.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo 'movimentacoes.csv' para escrita.\n");
        return;
    }

    fprintf(arquivo, "id_produto,tipo,quantidade,data\n");

    int i = 0; 
    while (i < total_movimentacoes) { 
        fprintf(arquivo, "%d,%s,%d,%s\n",
                movimentacoes[i].id_produto,
                movimentacoes[i].tipo,
                movimentacoes[i].quantidade,
                movimentacoes[i].data);
        i++; 
    }

    fclose(arquivo);
}




