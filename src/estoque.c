#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estoque.h"
#include "utils.h"
#include "movimentacao.h"
#include "validador.h"

#define PRODUTOS_FILE DATA_DIR PATH_SEPARATOR "produtos.dat"

// Função auxiliar para exibir um título de seção
void exibir_titulo_relatorio_estoque(const char *titulo)
{
    limparTela();
    printf("-----------------------------------------------\n");
    printf("///    %s    ///\n", titulo);
    printf("-----------------------------------------------\n");
}

// Função auxiliar para exibir mensagem se não houver resultados
void exibir_mensagem_sem_resultado_estoque(int tem_registro)
{
    if (!tem_registro)
    {
        printf("Nenhum registro encontrado.\n");
    }
}

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
    printf("╔════════════════════════════════════════╗\n");
    printf("║        Cadastrar Novo Produto          ║\n");
    printf("╚════════════════════════════════════════╝\n");


    // Validação de ID único e positivo
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

    // Validação do Nome do Produto
    do
    {
        printf("Informe o nome do produto: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer)); // Reutilizando validarNome
    strcpy(novo_produto.nome, buffer);

    // Validação da Quantidade (deve ser não-negativa)
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

    // Validação da Data de Validade
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
    char buffer[51]; // Buffer

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║      Pesquisar Produto por ID          ║\n");
    printf("╚════════════════════════════════════════╝\n");


    // Validação do ID
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
            printf("╔════════════════════════════════════════════════════════╗\n");
            printf("║ ID  ║ Nome do Produto      ║ Qtde Estoque ║ Validade   ║\n");
            printf("╠════════════════════════════════════════════════════════╣\n");
            printf("║ %-3d ║ %-20s ║ %-12d ║ %-10s ║\n", 
                    produto_lido.id, produto_lido.nome, produto_lido.quantidade, produto_lido.validade);
            
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
    printf("╔════════════════════════════════════════╗\n");
    printf("║         Movimentar Estoque             ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║ 1. Entrada de Material                 ║\n");
    printf("║ 2. Saída de Material                   ║\n");
    printf("║ 0. Voltar                              ║\n");
    printf("╚════════════════════════════════════════╝\n");

    // Leitura segura da Opção
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
        // Validação do ID
        do
        {
            printf("Informe o ID do produto para movimentar: ");
            lerString(bufferValor, 10);
            id = validarInteiroPositivo(bufferValor);
        } while (id <= 0);

        // Validação da Quantidade
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
    printf("╔════════════════════════════════════════╗\n");
    printf("║    Listagem de Produtos em Estoque     ║\n");
    printf("╚════════════════════════════════════════╝\n");


    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado no estoque.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║ID | Nome do Produto        | Qtde       | Validade    ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1)
        {
            printf("║%-2d | %-22s | %-10d | %s\n",
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
    printf("╔════════════════════════════════════════╗\n");
    printf("║    Itens em Falta (Estoque Baixo)      ║\n");
    printf("╚════════════════════════════════════════╝\n");
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

// Função para relatório de produtos com validade próxima
// Assumindo que a data de validade está no formato "dd/mm/aaaa"
void relatorio_validade_proxima(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int encontrado = 0;
    const int DIAS_LIMITE = 30; // Produtos com validade em até 30 dias

    time_t t = time(NULL);

    limparTela();
    printf("------------------------------------------\n");
    printf("/// Produtos com Validade Proxima (%d dias) ///\n", DIAS_LIMITE);
    printf("------------------------------------------\n");
    printf("Produtos com validade em ate %d dias:\n\n", DIAS_LIMITE);

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1)
        {
            int dia, mes, ano;
            if (sscanf(produto_lido.validade, "%d/%d/%d", &dia, &mes, &ano) == 3)
            {
                struct tm tm_validade = {0};
                tm_validade.tm_mday = dia;
                tm_validade.tm_mon = mes - 1; // Janeiro é 0
                tm_validade.tm_year = ano - 1900;

                // Converter para time_t para comparação
                time_t t_validade = mktime(&tm_validade);

                if (t_validade != -1)
                {
                    double diff_segundos = difftime(t_validade, t);
                    int diff_dias = (int)(diff_segundos / (60 * 60 * 24));

                    if (diff_dias >= 0 && diff_dias <= DIAS_LIMITE)
                    {
                        printf("-> ID: %d | Nome: %s | Validade: %s | Dias Restantes: %d\n",
                               produto_lido.id, produto_lido.nome, produto_lido.validade, diff_dias);
                        encontrado = 1;
                    }
                }
            }
        }
    }
    fclose(arq_produtos);

    if (!encontrado)
    {
        printf("Nenhum produto com validade proxima.\n");
    }
    pressioneEnterParaContinuar();
}

// Submenu de relatórios existente (atualizado para incluir o novo relatório)
void gerar_relatorios_estoque(void)
{
    int opcao;
    char bufferOpcao[5];

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║         Relatórios de Estoque          ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ 1. Itens em Falta (Estoque Baixo)      ║\n");
        printf("║ 2. Histórico de Movimentações          ║\n");
        printf("║ 0. Voltar                              ║\n");
        printf("╚════════════════════════════════════════╝\n");

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
        } while (!validarOpcaoMenu(opcao, 0, 3)); // Ajustando o limite

        switch (opcao)
        {
        case 1:
            relatorio_itens_falta();
            break;
        case 2:
            relatorio_validade_proxima();
            break;
        case 3:
            exibir_historico_movimentacoes();
            break;
        case 0:
            break;
        }
    } while (opcao != 0);
}

// ---------------------------------------
// NOVOS RELATÓRIOS                      |
// ---------------------------------------

// Função para exibir o relatório completo de produtos ativos e inativos
void relatorio_completo_estoque(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int total_ativos = 0;
    int total_inativos = 0;
    int total_geral = 0;
    int tem_registro = 0;

    exibir_titulo_relatorio_estoque("Relatorio Completo de Estoque");

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    // Primeira passagem para contagem
    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1)
        {
            total_ativos++;
        }
        else
        {
            total_inativos++;
        }
        total_geral++;
    }
    fclose(arq_produtos);

    // Impressão das estatísticas
    printf("\n--- Estatisticas Gerais ---\n");
    printf("Total de produtos cadastrados: %d\n", total_geral);
    printf("Total de produtos ATIVOS:      %d\n", total_ativos);
    printf("Total de produtos INATIVOS:    %d\n", total_inativos);
    printf("---------------------------\n\n");

    // Segunda passagem para listagem detalhada de ativos
    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos != NULL)
    {
        printf("--- Produtos ATIVOS ---\n");
        if (total_ativos > 0)
        {
            printf("ID | Nome do Produto        | Quantidade | Validade\n");
            printf("---|------------------------|------------|----------\n");
        }
        while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
        {
            if (produto_lido.ativo == 1)
            {
                printf("%-2d | %-22s | %-10d | %s\n",
                       produto_lido.id, produto_lido.nome,
                       produto_lido.quantidade, produto_lido.validade);
                tem_registro = 1;
            }
        }
        exibir_mensagem_sem_resultado_estoque(tem_registro);
        printf("\n");

        // Reposiciona o ponteiro do arquivo para o início novamente
        rewind(arq_produtos);
        tem_registro = 0; // Reset para a próxima listagem

        printf("--- Produtos INATIVOS ---\n");
        if (total_inativos > 0)
        {
            printf("ID | Nome do Produto        | Quantidade | Validade\n");
            printf("---|------------------------|------------|----------\n");
        }
        while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
        {
            if (produto_lido.ativo == 0)
            {
                printf("%-2d | %-22s | %-10d | %s\n",
                       produto_lido.id, produto_lido.nome,
                       produto_lido.quantidade, produto_lido.validade);
                tem_registro = 1;
            }
        }
        exibir_mensagem_sem_resultado_estoque(tem_registro);
        fclose(arq_produtos);
    }

    printf("\n--- Fim do Relatorio ---\n");
    pressioneEnterParaContinuar();
}

// Função para exibir o relatório filtrado por nome (parcial)
void relatorio_por_nome_estoque(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    char nome_filtro[51];
    int tem_registro = 0;

    exibir_titulo_relatorio_estoque("Relatorio por Nome (Parcial)");

    printf("Informe parte do nome para filtrar: ");
    lerString(nome_filtro, 50);

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n--- Produtos com nome contendo '%s' ---\n", nome_filtro);
    printf("ID | Nome do Produto        | Quantidade | Validade\n");
    printf("---|------------------------|------------|----------\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1 && strstr(produto_lido.nome, nome_filtro) != NULL)
        {
            printf("%-2d | %-22s | %-10d | %s\n",
                   produto_lido.id, produto_lido.nome,
                   produto_lido.quantidade, produto_lido.validade);
            tem_registro = 1;
        }
    }
    exibir_mensagem_sem_resultado_estoque(tem_registro);

    fclose(arq_produtos);
    printf("\n--- Fim do Relatorio ---\n");
    pressioneEnterParaContinuar();
}

// Função para exibir o relatório filtrado por status (ativo/inativo)
void relatorio_por_status_estoque(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int status_filtro;
    int tem_registro = 0;
    char status_texto[10];

    exibir_titulo_relatorio_estoque("Relatorio por Status");

    do
    {
        printf("Deseja filtrar por (1) ATIVOS ou (2) INATIVOS? (1/2): ");
        char buffer[3];
        lerString(buffer, 2);
        status_filtro = validarInteiroPositivo(buffer);
        if (status_filtro != 2 && status_filtro != 1)
        {
            printf("Opcao invalida. Por favor, digite 1 para ATIVO ou 2 para INATIVO.\n");
        }
    } while (status_filtro != 2 && status_filtro != 1);

    if (status_filtro == 1)
    {
        strcpy(status_texto, "ATIVO");
    }
    else
    {
        strcpy(status_texto, "INATIVO");
    }

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n--- Produtos com status '%s' ---\n", status_texto);
    printf("ID | Nome do Produto        | Quantidade | Validade\n");
    printf("---|------------------------|------------|----------\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == status_filtro)
        {
            printf("%-2d | %-22s | %-10d | %s\n",
                   produto_lido.id, produto_lido.nome,
                   produto_lido.quantidade, produto_lido.validade);
            tem_registro = 1;
        }
    }
    exibir_mensagem_sem_resultado_estoque(tem_registro);

    fclose(arq_produtos);
    printf("\n--- Fim do Relatorio ---\n");
    pressioneEnterParaContinuar();
}

// Sub-menu para os relatórios de estoque (novo)
void submenu_relatorios_estoque(void)
{
    int opcao_relatorio;
    char bufferOpcao[5];

    do
    {
        limparTela();
        printf("----------------------------------------\n");
        printf("///      Submenu de Relatorios       ///\n");
        printf("----------------------------------------\n");
        printf("1. Relatorio Completo (Ativos e Inativos)\n");
        printf("2. Relatorio por Nome (Parcial)\n");
        printf("3. Relatorio por Status (Ativo/Inativo)\n");
        printf("4. Itens em Falta (Estoque Baixo)\n");
        printf("5. Produtos com Validade Proxima\n");
        printf("0. Voltar ao menu principal de estoque\n");
        printf("----------------------------------------\n");

        // Leitura segura de Opção
        do
        {
            printf(">>> Escolha a opcao: ");
            lerString(bufferOpcao, 5);
            char *endptr;
            opcao_relatorio = strtol(bufferOpcao, &endptr, 10);
            if (endptr == bufferOpcao || *endptr != '\0')
            {
                opcao_relatorio = -1;
            }
        } while (!validarOpcaoMenu(opcao_relatorio, 0, 5)); // Ajustando o limite

        switch (opcao_relatorio)
        {
        case 1:
            relatorio_completo_estoque();
            break;
        case 2:
            relatorio_por_nome_estoque();
            break;
        case 3:
            relatorio_por_status_estoque();
            break;
        case 4:
            relatorio_itens_falta(); // Chamando a função existente
            break;
        case 5:
            relatorio_validade_proxima(); // Chamando a função existente
            break;
        case 0:
            break;
        }
    } while (opcao_relatorio != 0);
}

// -------
// MENU  |
// -------
void modulo_estoque(void)
{
    int opcao;
    char bufferOpcao[5];
    criarPastaData();

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║           Módulo de Estoque            ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ 1. Cadastrar Produto                   ║\n");
        printf("║ 2. Pesquisar Produto (por ID)          ║\n");
        printf("║ 3. Listar Produtos                     ║\n");
        printf("║ 4. Movimentar Estoque (Entrada/Saída)  ║\n");
        printf("║ 5. Gerar Relatórios                    ║\n");
        printf("║ 0. Voltar ao menu principal            ║\n");
        printf("╚════════════════════════════════════════╝\n");

        // Leitura segura de Opção
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
        // Chamando o novo submenu
        case 5:
            submenu_relatorios_estoque();
            break;
        case 0:
            break;
        }
    } while (opcao != 0);
}