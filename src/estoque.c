#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estoque.h"
#include "utils.h"
#include "movimentacao.h"
#include "validador.h" // MUDANÇA: Incluindo a biblioteca de validação

#define PRODUTOS_FILE DATA_DIR PATH_SEPARATOR "produtos.dat"

// Declaração da função auxiliar (Para uso interno)
int id_produto_existe(int id);

// ------------------------------------------
// FUNÇÃO AUXILIAR: Checa a unicidade do ID |
// ------------------------------------------
int id_produto_existe(int id)
{
    Produto produto_lido;
    FILE *arq_produtos = fopen(PRODUTOS_FILE, "rb");

    if (arq_produtos == NULL)
    {
        return 0; // Se o arquivo não existe, o ID é único
    }

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.id == id && produto_lido.ativo == 1)
        {
            fclose(arq_produtos);
            return 1; // ID encontrado
        }
    }
    fclose(arq_produtos);
    return 0; // ID não encontrado
}

// ----------
// CADASTRO |
// ----------
void cadastrar_produto(void)
{
    Produto novo_produto;
    FILE *arq_produtos;
    char buffer[51];
    int id_temp;

    limparTela();
    printf("----------------------------------------\n");
    printf("///       Cadastrar Novo Produto     ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Validação de ID único e positivo
    do
    {
        printf("\nInforme o ID unico do produto: ");
        lerString(buffer, 5); // ID é pequeno, 5 é suficiente
        id_temp = validarInteiroPositivo(buffer);

        if (id_temp > 0 && id_produto_existe(id_temp))
        {
            printf("! Erro: ID %d ja cadastrado ou ativo. Use outro ID.\n", id_temp);
            id_temp = -1; // Força repetição
        }
    } while (id_temp <= 0);
    novo_produto.id = id_temp;

    // MUDANÇA: Validação do Nome do Produto
    do
    {
        printf("Informe o nome do produto: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer)); // Reutilizando validarNome
    strcpy(novo_produto.nome, buffer);

    // MUDANÇA: Validação da Quantidade (deve ser não-negativa)
    do
    {
        printf("Informe a quantidade inicial em estoque: ");
        lerString(buffer, 10);
        id_temp = validarInteiroPositivo(buffer); // Reutilizando (ajustar se aceitar zero)
        if (id_temp < 0)
        { // Se a validação retornar -1
            id_temp = -1;
        }
    } while (id_temp < 0);
    novo_produto.quantidade = id_temp;

    // MUDANÇA: Validação da Data de Validade
    do
    {
        printf("Informe a data de validade (dd/mm/aaaa): ");
        lerString(buffer, 11);
    } while (!validarData(buffer));
    strcpy(novo_produto.validade, buffer);

    novo_produto.ativo = 1;

    arq_produtos = fopen(PRODUTOS_FILE, "ab");
    if (arq_produtos == NULL)
    {
        printf("\nErro ao abrir o arquivo de produtos!\n");
        pressioneEnterParaContinuar();
        return;
    }

    fwrite(&novo_produto, sizeof(Produto), 1, arq_produtos);
    fclose(arq_produtos);

    printf("\nProduto cadastrado com sucesso no estoque!\n");
    pressioneEnterParaContinuar();
}

// ----------
// PESQUISA |
// ----------
void pesquisar_produto(void)
{
    int encontrado = 0;
    int id_busca;
    Produto produto_lido;
    FILE *arq_produtos;
    char buffer[51]; // MUDANÇA: Buffer

    limparTela();
    printf("----------------------------------------\n");
    printf("///     Pesquisar Produto por ID     ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Validação do ID
    do
    {
        printf("Informe o ID do produto: ");
        lerString(buffer, 5);
        id_busca = validarInteiroPositivo(buffer);
    } while (id_busca <= 0);

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("\nNenhum produto cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.id == id_busca && produto_lido.ativo == 1)
        {
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

    if (!encontrado)
    {
        printf("\nProduto com ID %d nao foi encontrado.\n", id_busca);
    }
    pressioneEnterParaContinuar();
}

// --------------
// MOVIMENTAÇÃO |
// --------------
void movimentar_estoque(void)
{
    int opcao, id, quantidade, encontrado = 0;
    Produto produto_lido;
    FILE *arq_produtos;
    long int pos;
    char bufferOpcao[5];
    char bufferValor[10];

    limparTela();
    printf("----------------------------------------\n");
    printf("///        Movimentar Estoque        ///\n");
    printf("----------------------------------------\n");
    printf("1. Entrada de Material\n");
    printf("2. Saida de Material\n");
    printf("0. Voltar\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Leitura segura da Opção
    do
    {
        printf(">>> Escolha a opcao: ");
        lerString(bufferOpcao, 5);
        char *endptr;
        opcao = strtol(bufferOpcao, &endptr, 10);
        if (endptr == bufferOpcao || *endptr != '\0')
        {
            opcao = -1;
        }
    } while (!validarOpcaoMenu(opcao, 0, 2));

    if (opcao == 1 || opcao == 2)
    {
        // MUDANÇA: Validação do ID
        do
        {
            printf("Informe o ID do produto para movimentar: ");
            lerString(bufferValor, 10);
            id = validarInteiroPositivo(bufferValor);
        } while (id <= 0);

        // MUDANÇA: Validação da Quantidade
        do
        {
            printf("Informe a quantidade: ");
            lerString(bufferValor, 10);
            quantidade = validarInteiroPositivo(bufferValor);
        } while (quantidade <= 0);

        arq_produtos = fopen(PRODUTOS_FILE, "r+b");
        if (arq_produtos == NULL)
        {
            printf("\nArquivo de produtos nao encontrado.\n");
            pressioneEnterParaContinuar();
            return;
        }

        while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
        {
            if (produto_lido.id == id && produto_lido.ativo == 1)
            {
                encontrado = 1;
                pos = ftell(arq_produtos) - sizeof(Produto);

                if (opcao == 1)
                { // Entrada
                    produto_lido.quantidade += quantidade;
                    registrar_movimentacao(id, "Entrada", quantidade);
                    printf("\nEntrada de %d unidades de '%s' registrada com sucesso!\n", quantidade, produto_lido.nome);
                }
                else
                { // Saída
                    if (produto_lido.quantidade >= quantidade)
                    {
                        produto_lido.quantidade -= quantidade;
                        registrar_movimentacao(id, "Saida", quantidade);
                        printf("\nSaida de %d unidades de '%s' registrada com sucesso!\n", quantidade, produto_lido.nome);
                    }
                    else
                    {
                        printf("\nQuantidade insuficiente em estoque. Disponivel: %d\n", produto_lido.quantidade);
                    }
                }

                fseek(arq_produtos, pos, SEEK_SET);
                fwrite(&produto_lido, sizeof(Produto), 1, arq_produtos);
                break;
            }
        }
        fclose(arq_produtos);

        if (!encontrado)
        {
            printf("\nProduto com ID %d nao encontrado.\n", id);
        }
    }
    pressioneEnterParaContinuar();
}

// -------------------------------------------------------
// LISTAGEM E RELATÓRIOS (Não requerem input, não mudam) |
// ------------------------------------------------------
void listar_produtos(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int tem_produto = 0;

    limparTela();
    printf("-----------------------------------------------\n");
    printf("///     Listagem de Produtos em Estoque     ///\n");
    printf("-----------------------------------------------\n");

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado no estoque.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("ID | Nome do Produto        | Quantidade | Validade\n");
    printf("---|------------------------|------------|----------\n");
    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1)
        {
            printf("%-2d | %-22s | %-10d | %s\n",
                   produto_lido.id, produto_lido.nome,
                   produto_lido.quantidade, produto_lido.validade);
            tem_produto = 1;
        }
    }
    fclose(arq_produtos);

    if (!tem_produto)
    {
        printf("\nNenhum produto ativo encontrado no estoque.\n");
    }
    pressioneEnterParaContinuar();
}

void relatorio_itens_falta(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int encontrado = 0;
    const int LIMITE_MINIMO = 5;

    limparTela();
    printf("------------------------------------------\n");
    printf("///   Itens em Falta (Estoque Baixo)   ///\n");
    printf("------------------------------------------\n");
    printf("Itens com quantidade igual ou inferior a %d:\n\n", LIMITE_MINIMO);

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum item em falta no estoque.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1 && produto_lido.quantidade <= LIMITE_MINIMO)
        {
            printf("-> ID: %d | Nome: %s | Quantidade: %d\n",
                   produto_lido.id, produto_lido.nome, produto_lido.quantidade);
            encontrado = 1;
        }
    }
    fclose(arq_produtos);

    if (!encontrado)
    {
        printf("Nenhum item em falta no estoque.\n");
    }
    pressioneEnterParaContinuar();
}

void gerar_relatorios_estoque(void)
{
    int opcao;
    char bufferOpcao[5];

    do
    {
        limparTela();
        printf("----------------------------------------\n");
        printf("///        Relatorios de Estoque     ///\n");
        printf("----------------------------------------\n");
        printf("1. Itens em Falta (Estoque Baixo)\n");
        printf("2. Historico de Movimentacoes\n");
        printf("0. Voltar\n");
        printf("----------------------------------------\n");

        // MUDANÇA: Leitura segura de Opção
        do
        {
            printf(">>> Escolha a opcao: ");
            lerString(bufferOpcao, 5);
            char *endptr;
            opcao = strtol(bufferOpcao, &endptr, 10);
            if (endptr == bufferOpcao || *endptr != '\0')
            {
                opcao = -1;
            }
        } while (!validarOpcaoMenu(opcao, 0, 2));

        switch (opcao)
        {
        case 1:
            relatorio_itens_falta();
            break;
        case 2:
            // Assumindo que exibir_historico_movimentacoes() existe em movimentacao.h
            exibir_historico_movimentacoes();
            break;
        case 0:
            break;
            // Default não é mais necessário
        }
    } while (opcao != 0);
}

void modulo_estoque(void)
{
    int opcao;
    char bufferOpcao[5];
    criarPastaData();

    do
    {
        // MUDANÇA: Substituindo TelaMenuEstoque() pelo menu inline para consistência
        limparTela();
        printf("----------------------------------------\n");
        printf("///          Modulo Estoque          ///\n");
        printf("----------------------------------------\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Pesquisar Produto (por ID)\n");
        printf("3. Listar Produtos\n");
        printf("4. Movimentar Estoque (Entrada/Saida)\n");
        printf("5. Gerar Relatorios\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------------\n");

        // MUDANÇA: Leitura segura de Opção
        do
        {
            printf(">>> Escolha a opcao: ");
            lerString(bufferOpcao, 5);
            char *endptr;
            opcao = strtol(bufferOpcao, &endptr, 10);
            if (endptr == bufferOpcao || *endptr != '\0')
            {
                opcao = -1;
            }
        } while (!validarOpcaoMenu(opcao, 0, 5));

        switch (opcao)
        {
        case 1:
            cadastrar_produto();
            break;
        case 2:
            pesquisar_produto();
            break;
        case 3:
            listar_produtos();
            break;
        case 4:
            movimentar_estoque();
            break;
        case 5:
            gerar_relatorios_estoque();
            break;
        case 0:
            break;
            // Default não é mais necessário
        }
    } while (opcao != 0);
}
