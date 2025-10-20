#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estoque.h"
#include "utils.h"
#include "movimentacao.h" 

#ifdef _WIN32
    #include <direct.h>
    #define PATH_SEPARATOR "\\"
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define PATH_SEPARATOR "/"
#endif

#define DATA_DIR "data"
#define PRODUTOS_FILE DATA_DIR "/produtos.dat"

void criar_pasta_data_se_nao_existir() {
    #ifdef _WIN32
        _mkdir(DATA_DIR);
    #else
        mkdir(DATA_DIR, 0777);
    #endif
}

void cadastrar_produto(void) {
    Produto novo_produto;
    FILE* arq_produtos;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///      Cadastrar Novo Produto      ///\n");
    printf("----------------------------------------\n");
    
    printf("\nInforme o ID unico do produto: ");
    scanf("%d", &novo_produto.id);
    while (getchar() != '\n');

    printf("Informe o nome do produto: ");
    scanf(" %49[^\n]", novo_produto.nome);
    while (getchar() != '\n');

    printf("Informe a quantidade inicial em estoque: ");
    scanf("%d", &novo_produto.quantidade);
    while (getchar() != '\n');

    printf("Informe a data de validade (dd/mm/aaaa): ");
    scanf("%10s", novo_produto.validade);
    while (getchar() != '\n');
    
    novo_produto.ativo = 1; 

    arq_produtos = fopen(PRODUTOS_FILE, "ab");
    if (arq_produtos == NULL) {
        printf("\nErro ao abrir o arquivo de produtos!\n");
        press_enter_to_continue();
        return;
    }
    
    fwrite(&novo_produto, sizeof(Produto), 1, arq_produtos);
    fclose(arq_produtos);

    printf("\nProduto cadastrado com sucesso no estoque!\n");
    press_enter_to_continue();
}


void pesquisar_produto(void) {
    int encontrado = 0;
    int id_busca;
    Produto produto_lido;
    FILE* arq_produtos;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Pesquisar Produto por ID     ///\n");
    printf("----------------------------------------\n");
    printf("Informe o ID do produto: ");
    scanf("%d", &id_busca);
    while (getchar() != '\n');

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL) {
        printf("\nNenhum produto cadastrado.\n");
        press_enter_to_continue();
        return;
    }
    
    while(fread(&produto_lido, sizeof(Produto), 1, arq_produtos)) {
        if (produto_lido.id == id_busca && produto_lido.ativo == 1) {
            printf("\n--- Produto Encontrado ---\n");
            printf("ID: %d\n", produto_lido.id);
            printf("Nome: %s\n", produto_lido.nome);
            printf("Quantidade em Estoque: %d\n", produto_lido.quantidade);
            printf("Validade: %s\n", produto_lido.validade);
            printf("--------------------------\n");
            encontrado = 1;
            break;
        }
    }
    fclose(arq_produtos);

    if (!encontrado) {
        printf("\nProduto com ID %d nao foi encontrado.\n", id_busca);
    }
    press_enter_to_continue();
}


void movimentar_estoque(void) {
    int opcao, id, quantidade, encontrado = 0;
    Produto produto_lido;
    FILE* arq_produtos;
    long int pos;
    
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///        Movimentar Estoque        ///\n");
    printf("----------------------------------------\n");
    printf("1. Entrada de Material\n");
    printf("2. Saida de Material\n");
    printf("0. Voltar\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao: ");
    
    if (scanf("%d", &opcao) != 1) { opcao = -1; }
    while (getchar() != '\n');
    
    if (opcao == 1 || opcao == 2) {
        printf("Informe o ID do produto para movimentar: ");
        scanf("%d", &id);
        while (getchar() != '\n');
        printf("Informe a quantidade: ");
        scanf("%d", &quantidade);
        while (getchar() != '\n');

        arq_produtos = fopen(PRODUTOS_FILE, "r+b");
        if (arq_produtos == NULL) {
            printf("\nArquivo de produtos nao encontrado.\n");
            press_enter_to_continue();
            return;
        }

        while(fread(&produto_lido, sizeof(Produto), 1, arq_produtos)) {
            if (produto_lido.id == id && produto_lido.ativo == 1) {
                encontrado = 1;
                pos = ftell(arq_produtos) - sizeof(Produto);

                if (opcao == 1) { // Entrada
                    produto_lido.quantidade += quantidade;
                    registrar_movimentacao(id, "Entrada", quantidade);
                    printf("\nEntrada de %d unidades de '%s' registrada com sucesso!\n", quantidade, produto_lido.nome);
                } else { // Saída
                    if (produto_lido.quantidade >= quantidade) {
                        produto_lido.quantidade -= quantidade;
                        registrar_movimentacao(id, "Saida", quantidade);
                        printf("\nSaida de %d unidades de '%s' registrada com sucesso!\n", quantidade, produto_lido.nome);
                    } else {
                        printf("\nQuantidade insuficiente em estoque. Disponivel: %d\n", produto_lido.quantidade);
                    }
                }
                
                fseek(arq_produtos, pos, SEEK_SET);
                fwrite(&produto_lido, sizeof(Produto), 1, arq_produtos);
                break;
            }
        }
        fclose(arq_produtos);

        if (!encontrado) {
            printf("\nProduto com ID %d nao encontrado.\n", id);
        }
    } else if (opcao != 0) {
        printf("\nOpcao invalida.\n");
    }
    press_enter_to_continue();
}


void listar_produtos(void) {
    Produto produto_lido;
    FILE* arq_produtos;
    int tem_produto = 0;
    
    limpar_tela();
    printf("-----------------------------------------------\n");
    printf("///     Listagem de Produtos em Estoque     ///\n");
    printf("-----------------------------------------------\n");
    
    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL) {
        printf("Nenhum produto cadastrado no estoque.\n");
        press_enter_to_continue();
        return;
    }
    
    printf("ID | Nome do Produto        | Quantidade | Validade\n");
    printf("---|------------------------|------------|----------\n");
    while(fread(&produto_lido, sizeof(Produto), 1, arq_produtos)) {
        if (produto_lido.ativo == 1) {
            printf("%-2d | %-22s | %-10d | %s\n",
                   produto_lido.id, produto_lido.nome,
                   produto_lido.quantidade, produto_lido.validade);
            tem_produto = 1;
        }
    }
    fclose(arq_produtos);

    if (!tem_produto) {
        printf("\nNenhum produto ativo encontrado no estoque.\n");
    }
    press_enter_to_continue();
}

void relatorio_itens_falta(void) {
    Produto produto_lido;
    FILE* arq_produtos;
    int encontrado = 0;
    const int LIMITE_MINIMO = 5;

    limpar_tela();
    printf("------------------------------------------\n");
    printf("///   Itens em Falta (Estoque Baixo)   ///\n");
    printf("------------------------------------------\n");
    printf("Itens com quantidade igual ou inferior a %d:\n\n", LIMITE_MINIMO);
    
    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL) {
        printf("Nenhum item em falta no estoque.\n");
        press_enter_to_continue();
        return;
    }

    while(fread(&produto_lido, sizeof(Produto), 1, arq_produtos)) {
        if (produto_lido.ativo == 1 && produto_lido.quantidade <= LIMITE_MINIMO) {
            printf("-> ID: %d | Nome: %s | Quantidade: %d\n",
                   produto_lido.id, produto_lido.nome, produto_lido.quantidade);
            encontrado = 1;
        }
    }
    fclose(arq_produtos);

    if (!encontrado) {
        printf("Nenhum item em falta no estoque.\n");
    }
    press_enter_to_continue();
}


void gerar_relatorios_estoque(void) {
    int opcao;
    do {
        limpar_tela();
        printf("----------------------------------------\n");
        printf("///      Relatorios de Estoque       ///\n");
        printf("----------------------------------------\n");
        printf("1. Itens em Falta (Estoque Baixo)\n");
        printf("2. Historico de Movimentacoes\n");
        printf("0. Voltar\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) { opcao = -1; }
        while (getchar() != '\n');

        switch (opcao) {
            case 1: 
                relatorio_itens_falta(); 
                break;
            case 2: 
                exibir_historico_movimentacoes();
                break;
            case 0: 
                break;
            default: 
                printf("\nOpcao invalida.\n"); 
                press_enter_to_continue(); 
                break;
        }
    } while (opcao != 0);
}


void modulo_estoque(void) {
    int opcao;
    criar_pasta_data();

    do {
        TelaMenuEstoque(); 
        if (scanf("%d", &opcao) != 1) { 
            opcao = -1; 
        }
        while (getchar() != '\n'); 

        switch (opcao) {
            case 1: cadastrar_produto(); break;
            case 2: pesquisar_produto(); break;
            case 3: listar_produtos(); break;
            case 4: movimentar_estoque(); break;
            case 5: gerar_relatorios_estoque(); break;
            case 0: break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao != 0);
}
