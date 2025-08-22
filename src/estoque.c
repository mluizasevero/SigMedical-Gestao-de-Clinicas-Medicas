#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estoque.h"
#include "utils.h"
#include "movimentacao.h" 


void cadastrar_produto(Produto produtos[], int* total_produtos) {
    TelaCadastrarProduto(); 
    if (*total_produtos < 100) {
        printf("\nInforme o ID do produto: ");
        scanf("%d", &produtos[*total_produtos].id);
        while (getchar() != '\n');

        printf("Informe o nome do produto: ");
        scanf(" %49[^\n]", produtos[*total_produtos].nome);
        while (getchar() != '\n');

        printf("Informe a quantidade: ");
        scanf("%d", &produtos[*total_produtos].quantidade);
        while (getchar() != '\n');

        printf("Informe a validade (dd/mm/aaaa): ");
        scanf("%10s", produtos[*total_produtos].validade);
        while (getchar() != '\n');
        
        (*total_produtos)++;
        printf("\nProduto cadastrado com sucesso!\n");
    } else {
        printf("\nNao e possivel cadastrar mais produtos. Limite atingido.\n");
    }
    press_enter_to_continue();
}

void salvar_produtos(Produto produtos[], int total_produtos) {
    FILE *arq_produtos = fopen("produtos.dat", "w");
    if (arq_produtos == NULL) {
        printf("Erro ao abrir o arquivo 'produtos.dat' para escrita.\n");
        return;
    }
    for (int i = 0; i < total_produtos; i++) {
        fprintf(arq_produtos, "%d;%s;%d;%s\n",
                produtos[i].id, produtos[i].nome,
                produtos[i].quantidade, produtos[i].validade);
    }
    fclose(arq_produtos);
}

int ler_produtos(Produto produtos[]) {
    FILE *arq_produtos = fopen("produtos.dat", "r");
    if (arq_produtos == NULL) {
        return 0;
    }
    int i = 0;
    while(fscanf(arq_produtos, "%d;%[^;];%d;%[^\n]\n",
                  &produtos[i].id, produtos[i].nome,
                  &produtos[i].quantidade, produtos[i].validade) == 4) {
        i++;
    }
    fclose(arq_produtos);
    return i;
}

void pesquisar_produto(Produto produtos[], int total_produtos) {
    int opcao;
    int encontrado = 0;
    char busca_str[50];
    int busca_int;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Pesquisar Produto   ///\n");
    printf("----------------------------------------\n");
    printf("///   1. Por ID                 ///\n");
    printf("///   2. Por Nome               ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao de pesquisa: ");
    
    if (scanf("%d", &opcao) != 1) {
        opcao = -1;
        while (getchar() != '\n');
    } else {
        while (getchar() != '\n');
    }

    if (opcao == 1) {
        printf("Informe o ID do produto: ");
        scanf("%d", &busca_int);
        while (getchar() != '\n');
        for (int i = 0; i < total_produtos; i++) {
            if (produtos[i].id == busca_int) {
                printf("\nProduto encontrado:\n");
                printf("ID: %d\n", produtos[i].id);
                printf("Nome: %s\n", produtos[i].nome);
                printf("Quantidade: %d\n", produtos[i].quantidade);
                printf("Validade: %s\n", produtos[i].validade);
                encontrado = 1;
                break;
            }
        }
    } else if (opcao == 2) {
        printf("Informe o Nome do produto: ");
        scanf(" %49[^\n]", busca_str);
        while (getchar() != '\n');
        for (int i = 0; i < total_produtos; i++) {
            if (strcmp(produtos[i].nome, busca_str) == 0) {
                printf("\nProduto encontrado:\n");
                printf("ID: %d\n", produtos[i].id);
                printf("Nome: %s\n", produtos[i].nome);
                printf("Quantidade: %d\n", produtos[i].quantidade);
                printf("Validade: %s\n", produtos[i].validade);
                encontrado = 1;
                break;
            }
        }
    } else {
        printf("Opcao invalida.\n");
    }

    if (!encontrado) {
        printf("\nProduto nao encontrado.\n");
    }
    press_enter_to_continue();
}

void movimentar_estoque(Produto produtos[], int total_produtos, Movimentacao movimentacoes[], int* total_movimentacoes) {
    int opcao, id, quantidade, encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///       Movimentar Estoque         ///\n");
    printf("----------------------------------------\n");
    printf("///       1. Entrada de Material     ///\n");
    printf("///       2. Saida de Material       ///\n");
    printf("///       0. Voltar                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
    if (scanf("%d", &opcao) != 1) {
        opcao = -1;
        while (getchar() != '\n');
    } else {
        while (getchar() != '\n');
    }
    
    if (opcao == 1 || opcao == 2) {
        printf("Informe o ID do produto: ");
        scanf("%d", &id);
        while (getchar() != '\n');
        printf("Informe a quantidade: ");
        scanf("%d", &quantidade);
        while (getchar() != '\n');

        for (int i = 0; i < total_produtos; i++) {
            if (produtos[i].id == id) {
                encontrado = 1;
                if (opcao == 1) {
                    produtos[i].quantidade += quantidade;
                    registrar_movimentacao(id, "Entrada", quantidade, movimentacoes, total_movimentacoes);
                    printf("\nEntrada de %d unidades de %s registrada com sucesso!\n", quantidade, produtos[i].nome);
                } else {
                    if (produtos[i].quantidade >= quantidade) {
                        produtos[i].quantidade -= quantidade;
                        registrar_movimentacao(id, "Saida", quantidade, movimentacoes, total_movimentacoes);
                        printf("\nSaida de %d unidades de %s registrada com sucesso!\n", quantidade, produtos[i].nome);
                    } else {
                        printf("\nQuantidade insuficiente em estoque.\n");
                    }
                }
                break;
            }
        }
        if (!encontrado) {
            printf("\nProduto com ID %d nao encontrado.\n", id);
        }
    } else if (opcao != 0) {
        printf("\nOpcao invalida.\n");
    }
    press_enter_to_continue();
}


void gerenciar_lotes(Produto produtos[], int total_produtos) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Gerenciar Lotes (Todos os Produtos)   ///\n");
    printf("----------------------------------------\n");
    if (total_produtos == 0) {
        printf("Nenhum produto cadastrado no estoque.\n");
    } else {
        printf("ID | Nome | Quantidade | Validade \n");
        printf("---|------------------|------------|-------------\n");
        for (int i = 0; i < total_produtos; i++) {
            printf("%-2d | %-18s | %-10d | %s\n",
                   produtos[i].id, produtos[i].nome,
                   produtos[i].quantidade, produtos[i].validade);
        }
    }
    press_enter_to_continue();
}

void relatorio_itens_falta(Produto produtos[], int total_produtos) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Itens em Falta (Estoque Baixo)   ///\n");
    printf("----------------------------------------\n");
    int encontrado = 0;
    int limite = 5;
    printf("Itens com quantidade igual ou inferior a %d:\n", limite);
    
    for (int i = 0; i < total_produtos; i++) {
        if (produtos[i].quantidade <= limite) {
            printf("\nID: %d\n", produtos[i].id);
            printf("Nome: %s\n", produtos[i].nome);
            printf("Quantidade: %d\n", produtos[i].quantidade);
            printf("Validade: %s\n", produtos[i].validade);
            printf("----------------------------------------\n");
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("Nenhum item em falta no estoque.\n");
    }
    press_enter_to_continue();
}

void relatorio_validade_proxima(Produto produtos[], int total_produtos) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Itens com Validade Proxima   ///\n");
    printf("----------------------------------------\n");
    int encontrado = 0;
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int dia_atual = tm.tm_mday;
    int mes_atual = tm.tm_mon + 1;
    int ano_atual = tm.tm_year + 1900;
    
    printf("Itens com validade nos proximos 90 dias:\n");
    for (int i = 0; i < total_produtos; i++) {
        int dia_validade, mes_validade, ano_validade;
        sscanf(produtos[i].validade, "%d/%d/%d", &dia_validade, &mes_validade, &ano_validade);
        
        if (ano_validade < ano_atual) continue;
        if (ano_validade == ano_atual && mes_validade < mes_atual) continue;
        if (ano_validade == ano_atual && mes_validade == mes_atual && dia_validade < dia_atual) continue;
        
        if (ano_validade == ano_atual && mes_validade <= (mes_atual + 3) && ano_validade == ano_atual) {
            printf("\nID: %d\n", produtos[i].id);
            printf("Nome: %s\n", produtos[i].nome);
            printf("Quantidade: %d\n", produtos[i].quantidade);
            printf("Validade: %s\n", produtos[i].validade);
            printf("----------------------------------------\n");
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("Nenhum item com validade proxima encontrada.\n");
    }
    press_enter_to_continue();
}

void relatorio_historico_movimentacoes(void) {
    Movimentacao movimentacoes[1000];
    int total_movimentacoes = ler_movimentacoes(movimentacoes);
    
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Historico de Movimentacoes   ///\n");
    printf("----------------------------------------\n");

    if (total_movimentacoes == 0) {
        printf("Nenhuma movimentacao de estoque registrada.\n");
    } else {
        printf("ID Produto | Tipo | Quantidade | Data \n");
        printf("-----------|------|------------|-------------\n");
        for (int i = 0; i < total_movimentacoes; i++) {
            printf("%-10d | %-5s | %-10d | %s\n",
                   movimentacoes[i].id_produto, movimentacoes[i].tipo,
                   movimentacoes[i].quantidade, movimentacoes[i].data);
        }
    }
    press_enter_to_continue();
}

void gerar_relatorios_estoque(Produto produtos[], int total_produtos) {
    int opcao;
    do {
        TelaGerarRelatoriosEstoque();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                relatorio_itens_falta(produtos, total_produtos);
                break;
            case 2:
                relatorio_validade_proxima(produtos, total_produtos);
                break;
            case 3:
                relatorio_historico_movimentacoes();
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao != 0);
}


void modulo_estoque(Produto produtos[], int* total_produtos, Movimentacao movimentacoes[], int* total_movimentacoes) {
    int opcao;
    do {
        TelaMenuEstoque();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                cadastrar_produto(produtos, total_produtos);
                break;
            case 2:
                pesquisar_produto(produtos, *total_produtos);
                break;
            case 3:
                gerenciar_lotes(produtos, *total_produtos);
                break;
            case 4:
                movimentar_estoque(produtos, *total_produtos, movimentacoes, total_movimentacoes);
                break;
            case 5:
                gerar_relatorios_estoque(produtos, *total_produtos);
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER...\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao != 0);
}
