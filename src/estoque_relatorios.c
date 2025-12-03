#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estoque.h"
#include "utils.h"
#include "validador.h"
#include "estoque_relatorios.h"

#define PRODUTOS_FILE DATA_DIR PATH_SEPARATOR "produtos.dat"

// Função auxiliar para exibir um título de seção
static void exibir_titulo_relatorio_estoque(const char *titulo)
{
    limparTela();
    printf("╔═════════════════════════════════════════════╗\n");
    int len = strlen(titulo);
    int pad_left = (45 - len) / 2;
    int pad_right = 45 - len - pad_left;
    printf("║%*s%s%*s║\n", pad_left, "", titulo, pad_right, "");
    printf("╚═════════════════════════════════════════════╝\n");
}

// Função auxiliar para exibir mensagem se não houver resultados
static void exibir_mensagem_sem_resultado_estoque(int tem_registro)
{
    if (!tem_registro)
    {
        printf("Nenhum registro encontrado.\n");
    }
}

// Função auxiliar para verificar se um produto está vencido
// Retorna 1 se vencido, 0 se ainda válido
static int produto_vencido(const char *validade)
{
    int dia, mes, ano;
    if (sscanf(validade, "%d/%d/%d", &dia, &mes, &ano) != 3)
    {
        return 0; // Se não conseguir parsear, considera válido
    }

    time_t t = time(NULL);
    struct tm tm_validade = {0};
    tm_validade.tm_mday = dia;
    tm_validade.tm_mon = mes - 1;
    tm_validade.tm_year = ano - 1900;

    time_t t_validade = mktime(&tm_validade);
    if (t_validade == -1)
    {
        return 0;
    }

    double diff_segundos = difftime(t_validade, t);
    return (diff_segundos < 0) ? 1 : 0; // Se diferença negativa, está vencido
}

void relatorio_estoque_itens_falta(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int encontrado = 0;
    const int LIMITE_MINIMO = 5;

    exibir_titulo_relatorio_estoque("Itens em Falta (Estoque Baixo)");
    printf("Itens com quantidade igual ou inferior a %d (excluindo vencidos):\n\n", LIMITE_MINIMO);

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum item em falta no estoque.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("╔════╦════════════════════════╦════════════╦════════════╗\n");
    printf("║ ID ║ Nome do Produto        ║ Quantidade ║ Validade   ║\n");
    printf("╠════╬════════════════════════╬════════════╬════════════╣\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        // Só mostra se está ativo, com estoque baixo E não está vencido
        if (produto_lido.ativo == 1 && 
            produto_lido.quantidade <= LIMITE_MINIMO && 
            !produto_vencido(produto_lido.validade))
        {
            printf("║ %-2d ║ %-22s ║ %-10d ║ %-10s ║\n",
                   produto_lido.id, produto_lido.nome, produto_lido.quantidade, produto_lido.validade);
            encontrado = 1;
        }
    }
    printf("╚════╩════════════════════════╩════════════╩════════════╝\n");
    fclose(arq_produtos);

    if (!encontrado)
    {
        printf("\nNenhum item em falta no estoque (produtos validos).\n");
    }
    pressioneEnterParaContinuar();
}

void relatorio_estoque_validade_proxima(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int encontrado = 0;
    const int DIAS_LIMITE = 30;
    time_t t = time(NULL);

    exibir_titulo_relatorio_estoque("Produtos com Validade Proxima");

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Produtos com validade em ate %d dias ═══\n", DIAS_LIMITE);
    printf("╔════╦════════════════════════╦════════════╦══════════════════╗\n");
    printf("║ ID ║ Nome do Produto        ║ Validade   ║ Dias Restantes   ║\n");
    printf("╠════╬════════════════════════╬════════════╬══════════════════╣\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1)
        {
            int dia, mes, ano;
            if (sscanf(produto_lido.validade, "%d/%d/%d", &dia, &mes, &ano) == 3)
            {
                struct tm tm_validade = {0};
                tm_validade.tm_mday = dia;
                tm_validade.tm_mon = mes - 1;
                tm_validade.tm_year = ano - 1900;

                time_t t_validade = mktime(&tm_validade);
                if (t_validade != -1)
                {
                    double diff_segundos = difftime(t_validade, t);
                    int diff_dias = (int)(diff_segundos / (60 * 60 * 24));

                    if (diff_dias >= 0 && diff_dias <= DIAS_LIMITE)
                    {
                        printf("║ %-2d ║ %-22s ║ %-10s ║ %-16d ║\n",
                               produto_lido.id, produto_lido.nome, produto_lido.validade, diff_dias);
                        encontrado = 1;
                    }
                }
            }
        }
    }
    printf("╚════╩════════════════════════╩════════════╩══════════════════╝\n");
    fclose(arq_produtos);

    if (!encontrado)
    {
        printf("Nenhum produto com validade proxima.\n");
    }
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_estoque_completo(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int total_ativos = 0, total_inativos = 0, total_geral = 0;
    int tem_registro = 0;

    exibir_titulo_relatorio_estoque("Relatorio Completo de Estoque");

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
            total_ativos++;
        else
            total_inativos++;
        total_geral++;
    }
    fclose(arq_produtos);

    printf("\n═══ Estatisticas Gerais ═══\n");
    printf("Total de produtos cadastrados: %d\n", total_geral);
    printf("Total de produtos ATIVOS:      %d\n", total_ativos);
    printf("Total de produtos INATIVOS:    %d\n", total_inativos);
    printf("═══════════════════════════\n\n");

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos != NULL)
    {
        printf("═══ Produtos ATIVOS ═══\n");
        printf("╔════╦════════════════════════╦════════════╦════════════╗\n");
        printf("║ ID ║ Nome do Produto        ║ Quantidade ║ Validade   ║\n");
        printf("╠════╬════════════════════════╬════════════╬════════════╣\n");
        while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
        {
            if (produto_lido.ativo == 1)
            {
                printf("║ %-2d ║ %-22s ║ %-10d ║ %-10s ║\n",
                       produto_lido.id, produto_lido.nome,
                       produto_lido.quantidade, produto_lido.validade);
                tem_registro = 1;
            }
        }
        printf("╚════╩════════════════════════╩════════════╩════════════╝\n");
        exibir_mensagem_sem_resultado_estoque(tem_registro);
        printf("\n");

        rewind(arq_produtos);
        tem_registro = 0;

        printf("═══ Produtos INATIVOS ═══\n");
        printf("╔════╦════════════════════════╦════════════╦════════════╗\n");
        printf("║ ID ║ Nome do Produto        ║ Quantidade ║ Validade   ║\n");
        printf("╠════╬════════════════════════╬════════════╬════════════╣\n");
        while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
        {
            if (produto_lido.ativo == 0)
            {
                printf("║ %-2d ║ %-22s ║ %-10d ║ %-10s ║\n",
                       produto_lido.id, produto_lido.nome,
                       produto_lido.quantidade, produto_lido.validade);
                tem_registro = 1;
            }
        }
        printf("╚════╩════════════════════════╩════════════╩════════════╝\n");
        exibir_mensagem_sem_resultado_estoque(tem_registro);
        fclose(arq_produtos);
    }

    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_estoque_por_nome(void)
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

    printf("\n═══ Produtos com nome contendo '%s' ═══\n", nome_filtro);
    printf("╔════╦════════════════════════╦════════════╦════════════╗\n");
    printf("║ ID ║ Nome do Produto        ║ Quantidade ║ Validade   ║\n");
    printf("╠════╬════════════════════════╬════════════╬════════════╣\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1 && strstr(produto_lido.nome, nome_filtro) != NULL)
        {
            printf("║ %-2d ║ %-22s ║ %-10d ║ %-10s ║\n",
                   produto_lido.id, produto_lido.nome,
                   produto_lido.quantidade, produto_lido.validade);
            tem_registro = 1;
        }
    }
    printf("╚════╩════════════════════════╩════════════╩════════════╝\n");
    exibir_mensagem_sem_resultado_estoque(tem_registro);

    fclose(arq_produtos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_estoque_por_status(void)
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

    printf("\n═══ Produtos com status '%s' ═══\n", status_texto);
    printf("╔════╦════════════════════════╦════════════╦════════════╗\n");
    printf("║ ID ║ Nome do Produto        ║ Quantidade ║ Validade   ║\n");
    printf("╠════╬════════════════════════╬════════════╬════════════╣\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if ((status_filtro == 1 && produto_lido.ativo == 1) ||
            (status_filtro == 2 && produto_lido.ativo == 0))
        {
            printf("║ %-2d ║ %-22s ║ %-10d ║ %-10s ║\n",
                   produto_lido.id, produto_lido.nome,
                   produto_lido.quantidade, produto_lido.validade);
            tem_registro = 1;
        }
    }
    printf("╚════╩════════════════════════╩════════════╩════════════╝\n");
    exibir_mensagem_sem_resultado_estoque(tem_registro);

    fclose(arq_produtos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_estoque_produtos_vencidos(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int encontrado = 0;
    time_t t = time(NULL);

    exibir_titulo_relatorio_estoque("Produtos Vencidos");

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum produto cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Produtos com validade expirada ═══\n");
    printf("╔════╦════════════════════════╦════════════╦════════════╦══════════════════╗\n");
    printf("║ ID ║ Nome do Produto        ║ Quantidade ║ Validade   ║ Dias Vencido     ║\n");
    printf("╠════╬════════════════════════╬════════════╬════════════╬══════════════════╣\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1)
        {
            int dia, mes, ano;
            if (sscanf(produto_lido.validade, "%d/%d/%d", &dia, &mes, &ano) == 3)
            {
                struct tm tm_validade = {0};
                tm_validade.tm_mday = dia;
                tm_validade.tm_mon = mes - 1;
                tm_validade.tm_year = ano - 1900;

                time_t t_validade = mktime(&tm_validade);
                if (t_validade != -1)
                {
                    double diff_segundos = difftime(t_validade, t);
                    int diff_dias = (int)(diff_segundos / (60 * 60 * 24));

                    if (diff_dias < 0)
                    {
                        printf("║ %-2d ║ %-22s ║ %-10d ║ %-10s ║ %-16d ║\n",
                               produto_lido.id, produto_lido.nome, produto_lido.quantidade, 
                               produto_lido.validade, -diff_dias);
                        encontrado = 1;
                    }
                }
            }
        }
    }
    printf("╚════╩════════════════════════╩════════════╩════════════╩══════════════════╝\n");
    fclose(arq_produtos);

    if (!encontrado)
    {
        printf("\nNenhum produto vencido encontrado.\n");
    }
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_estoque_submenu(void)
{
    int opcao_relatorio;
    char bufferOpcao[5];

    do
    {
        limparTela();
        printf("╔═════════════════════════════════════════════╗\n");
        printf("║            Submenu de Relatorios            ║\n");
        printf("╠═════════════════════════════════════════════╣\n");
        printf("║ 1. Relatorio Completo (Ativos e Inativos)   ║\n");
        printf("║ 2. Relatorio por Nome (Parcial)             ║\n");
        printf("║ 3. Relatorio por Status (Ativo/Inativo)     ║\n");
        printf("║ 4. Itens em Falta (Estoque Baixo)           ║\n");
        printf("║ 5. Produtos com Validade Proxima            ║\n");
        printf("║ 6. Produtos Vencidos                        ║\n");
        printf("║ 0. Voltar ao menu principal de estoque      ║\n");
        printf("╚═════════════════════════════════════════════╝\n");

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
        } while (!validarOpcaoMenu(opcao_relatorio, 0, 6));

        switch (opcao_relatorio)
        {
        case 1:
            relatorio_estoque_completo();
            break;
        case 2:
            relatorio_estoque_por_nome();
            break;
        case 3:
            relatorio_estoque_por_status();
            break;
        case 4:
            relatorio_estoque_itens_falta();
            break;
        case 5:
            relatorio_estoque_validade_proxima();
            break;
        case 6:
            relatorio_estoque_produtos_vencidos();
            break;
        case 0:
            break;
        }
    } while (opcao_relatorio != 0);
}
