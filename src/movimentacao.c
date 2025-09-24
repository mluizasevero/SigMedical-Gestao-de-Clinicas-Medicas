#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "movimentacao.h"
#include "utils.h"

#ifdef _WIN32
    #include <direct.h> 
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

#define DATA_DIR "data"
#define MOVIMENTACOES_FILE DATA_DIR PATH_SEPARATOR "movimentacoes.dat"

int ler_movimentacoes(Movimentacao movimentacoes[]) {
    FILE *arquivo = fopen(MOVIMENTACOES_FILE, "r");
    if (arquivo == NULL) {
        return 0; 
    }

    int i = 0;
    while(fscanf(arquivo, "%d;%[^;];%d;%[^\n]\n",
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
        sprintf(nova.data, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

        movimentacoes[*total_movimentacoes] = nova;
        (*total_movimentacoes)++;
    } else {
        printf("Nao e possivel registrar mais movimentacoes. Limite atingido.\n");
    }
}

void salvar_movimentacoes(Movimentacao movimentacoes[], int total_movimentacoes) {
    FILE *arquivo = fopen(MOVIMENTACOES_FILE, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo 'movimentacoes.dat' para escrita.\n");
        return;
    }

    for (int i = 0; i < total_movimentacoes; i++) {
        fprintf(arquivo, "%d;%s;%d;%s\n",
                movimentacoes[i].id_produto,
                movimentacoes[i].tipo,
                movimentacoes[i].quantidade,
                movimentacoes[i].data);
    }

    fclose(arquivo);
}



