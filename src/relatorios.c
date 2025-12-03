#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "medicos.h"
#include "estoque.h"
#include "consultas.h"
#include "utils.h"
#include "validador.h"

#define MEDICOS_FILE DATA_DIR PATH_SEPARATOR "medicos.dat"
#define PRODUTOS_FILE DATA_DIR PATH_SEPARATOR "produtos.dat"
#define CONSULTAS_FILE DATA_DIR PATH_SEPARATOR "consultas.dat"

// Declaração do tipo No para lista de médicos
typedef struct No
{
    Medico medico;
    struct No *proximo;
} No;

// Função auxiliar para carregar médicos em lista ligada
extern No *carregar_medicos_para_lista(void);
extern void liberar_lista_medicos(No *lista);

// ========================================
// Funções auxiliares de exibição
// ========================================

// Função auxiliar para exibir um título de seção
void relatorios_exibir_titulo_relatorio(const char *titulo)
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
void relatorios_exibir_mensagem_sem_resultado(int tem_registro)
{
    if (!tem_registro)
    {
        printf("Nenhum registro encontrado.\n");
    }
}

// ========================================
// Relatórios de Médicos
// ========================================

void relatorios_medicos_completo(void)
{
    relatorios_exibir_titulo_relatorio("Relatorio Completo de Medicos");

    No *lista_medicos = carregar_medicos_para_lista();
    if (lista_medicos == NULL)
    {
        printf("Nenhum medico cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    int total_ativos = 0, total_inativos = 0, total_geral = 0;

    // Contagem de médicos
    No *temp = lista_medicos;
    while (temp != NULL)
    {
        if (temp->medico.ativo == 1)
        {
            total_ativos++;
        }
        else
        {
            total_inativos++;
        }
        total_geral++;
        temp = temp->proximo;
    }

    // Impressão das estatísticas
    printf("\n═══ Estatisticas Gerais ═══\n");
    printf("Total de medicos cadastrados: %d\n", total_geral);
    printf("Total de medicos ATIVOS:      %d\n", total_ativos);
    printf("Total de medicos INATIVOS:    %d\n", total_inativos);
    printf("═══════════════════════════\n\n");

    // Listagem detalhada de médicos ativos
    printf("═══ Medicos ATIVOS ═══\n");
    printf("╔════╦════════════════════════╦═══════════════╦══════════════════╗\n");
    printf("║ ID ║ Nome do Medico         ║ CPF           ║ Especialidade    ║\n");
    printf("╠════╬════════════════════════╬═══════════════╬══════════════════╣\n");
    temp = lista_medicos;
    while (temp != NULL)
    {
        if (temp->medico.ativo == 1)
        {
            printf("║ %-2d ║ %-22s ║ %-13s ║ %-16s ║\n",
                   temp->medico.id, temp->medico.nome,
                   temp->medico.cpf, temp->medico.especialidade);
        }
        temp = temp->proximo;
    }
    printf("╚════╩════════════════════════╩═══════════════╩══════════════════╝\n");

    // Listagem detalhada de médicos inativos
    printf("\n═══ Medicos INATIVOS ═══\n");
    printf("╔════╦════════════════════════╦═══════════════╦══════════════════╗\n");
    printf("║ ID ║ Nome do Medico         ║ CPF           ║ Especialidade    ║\n");
    printf("╠════╬════════════════════════╬═══════════════╬══════════════════╣\n");
    temp = lista_medicos;
    while (temp != NULL)
    {
        if (temp->medico.ativo == 0)
        {
            printf("║ %-2d ║ %-22s ║ %-13s ║ %-16s ║\n",
                   temp->medico.id, temp->medico.nome,
                   temp->medico.cpf, temp->medico.especialidade);
        }
        temp = temp->proximo;
    }
    printf("╚════╩════════════════════════╩═══════════════╩══════════════════╝\n");

    liberar_lista_medicos(lista_medicos);

    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_medicos_por_especialidade(void)
{
    Medico medico_lido;
    FILE *arq_medicos;
    char especialidade_filtro[51];
    int tem_registro = 0;

    relatorios_exibir_titulo_relatorio("Relatorio por Especialidade");

    printf("Informe a especialidade para filtrar: ");
    lerString(especialidade_filtro, 50);

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL)
    {
        printf("Nenhum medico cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Medicos com especialidade '%s' ═══\n", especialidade_filtro);
    printf("╔════╦════════════════════════╦═══════════════╦══════════════════╗\n");
    printf("║ ID ║ Nome do Medico         ║ CPF           ║ Especialidade    ║\n");
    printf("╠════╬════════════════════════╬═══════════════╬══════════════════╣\n");

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (medico_lido.ativo == 1 && strcmp(medico_lido.especialidade, especialidade_filtro) == 0)
        {
            printf("║ %-2d ║ %-22s ║ %-13s ║ %-16s ║\n",
                   medico_lido.id, medico_lido.nome,
                   medico_lido.cpf, medico_lido.especialidade);
            tem_registro = 1;
        }
    }
    printf("╚════╩════════════════════════╩═══════════════╩══════════════════╝\n");
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_medicos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_medicos_por_nome(void)
{
    Medico medico_lido;
    FILE *arq_medicos;
    char nome_filtro[51];
    int tem_registro = 0;

    relatorios_exibir_titulo_relatorio("Relatorio por Nome (Parcial)");

    printf("Informe parte do nome para filtrar: ");
    lerString(nome_filtro, 50);

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL)
    {
        printf("Nenhum medico cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Medicos com nome contendo '%s' ═══\n", nome_filtro);
    printf("╔════╦════════════════════════╦═══════════════╦══════════════════╗\n");
    printf("║ ID ║ Nome do Medico         ║ CPF           ║ Especialidade    ║\n");
    printf("╠════╬════════════════════════╬═══════════════╬══════════════════╣\n");

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (medico_lido.ativo == 1 && strstr(medico_lido.nome, nome_filtro) != NULL)
        {
            printf("║ %-2d ║ %-22s ║ %-13s ║ %-16s ║\n",
                   medico_lido.id, medico_lido.nome,
                   medico_lido.cpf, medico_lido.especialidade);
            tem_registro = 1;
        }
    }
    printf("╚════╩════════════════════════╩═══════════════╩══════════════════╝\n");
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_medicos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_medicos_por_status(void)
{
    Medico medico_lido;
    FILE *arq_medicos;
    int status_filtro;
    int tem_registro = 0;
    char status_texto[10];

    relatorios_exibir_titulo_relatorio("Relatorio por Status");

    do
    {
        printf("Deseja filtrar por (1) ATIVOS ou (2) INATIVOS? (1/2): ");
        char buffer[3];
        lerString(buffer, 2);
        status_filtro = validarInteiroPositivo(buffer);
        if (status_filtro != 1 && status_filtro != 2)
        {
            printf("Opcao invalida. Por favor, digite 1 para ATIVO ou 2 para INATIVO.\n");
        }
    } while (status_filtro != 1 && status_filtro != 2);

    if (status_filtro == 1)
    {
        strcpy(status_texto, "ATIVO");
    }
    else
    {
        strcpy(status_texto, "INATIVO");
    }

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL)
    {
        printf("Nenhum medico cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Medicos com status '%s' ═══\n", status_texto);
    printf("╔════╦════════════════════════╦═══════════════╦══════════════════╗\n");
    printf("║ ID ║ Nome do Medico         ║ CPF           ║ Especialidade    ║\n");
    printf("╠════╬════════════════════════╬═══════════════╬══════════════════╣\n");

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (medico_lido.ativo == status_filtro)
        {
            printf("║ %-2d ║ %-22s ║ %-13s ║ %-16s ║\n",
                   medico_lido.id, medico_lido.nome,
                   medico_lido.cpf, medico_lido.especialidade);
            tem_registro = 1;
        }
    }
    printf("╚════╩════════════════════════╩═══════════════╩══════════════════╝\n");
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_medicos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_submenu_medicos(void)
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
        printf("║ 2. Relatorio por Especialidade              ║\n");
        printf("║ 3. Relatorio por Nome (Parcial)             ║\n");
        printf("║ 4. Relatorio por Status (Ativo/Inativo)     ║\n");
        printf("║ 0. Voltar ao menu principal de medicos      ║\n");
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
        } while (!validarOpcaoMenu(opcao_relatorio, 0, 4));

        switch (opcao_relatorio)
        {
        case 1:
            relatorios_medicos_completo();
            break;
        case 2:
            relatorios_medicos_por_especialidade();
            break;
        case 3:
            relatorios_medicos_por_nome();
            break;
        case 4:
            relatorios_medicos_por_status();
            break;
        case 0:
            break;
        }
    } while (opcao_relatorio != 0);
}

// ========================================
// Relatórios de Estoque
// ========================================

void relatorios_estoque_itens_falta(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int encontrado = 0;
    const int LIMITE_MINIMO = 5;

    relatorios_exibir_titulo_relatorio("Itens em Falta (Estoque Baixo)");
    printf("Itens com quantidade igual ou inferior a %d:\n\n", LIMITE_MINIMO);

    arq_produtos = fopen(PRODUTOS_FILE, "rb");
    if (arq_produtos == NULL)
    {
        printf("Nenhum item em falta no estoque.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("╔════╦════════════════════════╦════════════╗\n");
    printf("║ ID ║ Nome do Produto        ║ Quantidade ║\n");
    printf("╠════╬════════════════════════╬════════════╣\n");

    while (fread(&produto_lido, sizeof(Produto), 1, arq_produtos))
    {
        if (produto_lido.ativo == 1 && produto_lido.quantidade <= LIMITE_MINIMO)
        {
            printf("║ %-2d ║ %-22s ║ %-10d ║\n",
                   produto_lido.id, produto_lido.nome, produto_lido.quantidade);
            encontrado = 1;
        }
    }
    printf("╚════╩════════════════════════╩════════════╝\n");
    fclose(arq_produtos);

    if (!encontrado)
    {
        printf("Nenhum item em falta no estoque.\n");
    }
    pressioneEnterParaContinuar();
}

void relatorios_estoque_validade_proxima(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int encontrado = 0;
    const int DIAS_LIMITE = 30;
    time_t t = time(NULL);

    relatorios_exibir_titulo_relatorio("Produtos com Validade Proxima");

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

void relatorios_estoque_completo(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int total_ativos = 0, total_inativos = 0, total_geral = 0;
    int tem_registro = 0;

    relatorios_exibir_titulo_relatorio("Relatorio Completo de Estoque");

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
        relatorios_exibir_mensagem_sem_resultado(tem_registro);
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
        relatorios_exibir_mensagem_sem_resultado(tem_registro);
        fclose(arq_produtos);
    }

    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_estoque_por_nome(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    char nome_filtro[51];
    int tem_registro = 0;

    relatorios_exibir_titulo_relatorio("Relatorio por Nome (Parcial)");

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
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_produtos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_estoque_por_status(void)
{
    Produto produto_lido;
    FILE *arq_produtos;
    int status_filtro;
    int tem_registro = 0;
    char status_texto[10];

    relatorios_exibir_titulo_relatorio("Relatorio por Status");

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
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_produtos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_submenu_estoque(void)
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
        } while (!validarOpcaoMenu(opcao_relatorio, 0, 5));

        switch (opcao_relatorio)
        {
        case 1:
            relatorios_estoque_completo();
            break;
        case 2:
            relatorios_estoque_por_nome();
            break;
        case 3:
            relatorios_estoque_por_status();
            break;
        case 4:
            relatorios_estoque_itens_falta();
            break;
        case 5:
            relatorios_estoque_validade_proxima();
            break;
        case 0:
            break;
        }
    } while (opcao_relatorio != 0);
}

// ========================================
// Relatórios de Consultas
// ========================================

void relatorios_consultas_medico(void)
{
    char nome_medico_pesquisa[50];
    int tem_registro = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    char data_hora[18];

    relatorios_exibir_titulo_relatorio("Consultas por Medico");

    // Validação do nome
    do
    {
        printf("Informe o nome do medico: ");
        lerString(nome_medico_pesquisa, 50);
    } while (!validarNome(nome_medico_pesquisa));

    printf("\nRelatorio de Consultas para o Medico: %s\n", nome_medico_pesquisa);

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL)
    {
        printf("Nenhuma consulta no sistema.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Consultas para o Medico: %s ═══\n", nome_medico_pesquisa);
    printf("╔══════════════════════════╦═══════════════╦═════════════════╦══════════╗\n");
    printf("║ Paciente                 ║ CPF           ║ Data/Hora       ║ Status   ║\n");
    printf("╠══════════════════════════╬═══════════════╬═════════════════╬══════════╣\n");

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (consulta_lida.ativo == 1 && strcmp(consulta_lida.nome_medico, nome_medico_pesquisa) == 0)
        {
            snprintf(data_hora, sizeof(data_hora), "%s %s", consulta_lida.data, consulta_lida.hora);
            printf("║ %-24s ║ %-13s ║ %-15s ║ %-8s ║\n",
                   consulta_lida.nome_paciente, consulta_lida.cpf_paciente, data_hora, consulta_lida.status);
            tem_registro = 1;
        }
    }
    printf("╚══════════════════════════╩═══════════════╩═════════════════╩══════════╝\n");
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_consultas_por_periodo(void)
{
    char data_inicio_str[11], data_fim_str[11];
    int tem_registro = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    char data_hora[18];
    long data_inicio_int, data_fim_int, data_consulta_int;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║         Consultas por Período          ║\n");
    printf("╚════════════════════════════════════════╝\n");

    // Validação das datas
    do
    {
        printf("Informe a data de inicio (dd/mm/aaaa): ");
        lerString(data_inicio_str, 11);
    } while (!validarData(data_inicio_str));

    do
    {
        printf("Informe a data de fim (dd/mm/aaaa): ");
        lerString(data_fim_str, 11);
    } while (!validarData(data_fim_str));

    data_inicio_int = converterDataParaInt(data_inicio_str);
    data_fim_int = converterDataParaInt(data_fim_str);

    if (data_fim_int < data_inicio_int)
    {
        printf("\n! Erro: A data final nao pode ser anterior a data inicial.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\nRelatorio de Consultas no Periodo de %s a %s:\n", data_inicio_str, data_fim_str);

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL)
    {
        printf("Nenhuma consulta no sistema.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Consultas no Periodo de %s a %s ═══\n", data_inicio_str, data_fim_str);
    printf("╔══════════════════════════╦═════════════════╦══════════════════════════╦══════════╗\n");
    printf("║ Paciente                 ║ Data/Hora       ║ Medico                   ║ Status   ║\n");
    printf("╠══════════════════════════╬═════════════════╬══════════════════════════╬══════════╣\n");

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (consulta_lida.ativo == 1)
        {
            data_consulta_int = converterDataParaInt(consulta_lida.data);

            if (data_consulta_int >= data_inicio_int && data_consulta_int <= data_fim_int)
            {
                snprintf(data_hora, sizeof(data_hora), "%s %s", consulta_lida.data, consulta_lida.hora);
                printf("║ %-24s ║ %-15s ║ %-24s ║ %-8s ║\n",
                       consulta_lida.nome_paciente, data_hora, consulta_lida.nome_medico, consulta_lida.status);
                tem_registro = 1;
            }
        }
    }
    printf("╚══════════════════════════╩═════════════════╩══════════════════════════╩══════════╝\n");
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorios_consultas_paciente(void)
{
    char termo_busca[50];
    int opcao_filtro;
    int tem_registro = 0;
    char bufferOpcao[5];
    Consulta consulta_lida;
    FILE *arq_consultas;
    char data_hora[18];
    char medico_esp[105];

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║    Relatório de Consultas Paciente     ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Filtrar por:\n");
    printf("1. Nome do Paciente\n");
    printf("2. CPF do Paciente\n");

    // Validação da escolha do filtro
    do
    {
        printf(">>> Escolha a opcao (1 ou 2): ");
        lerString(bufferOpcao, 5);
        opcao_filtro = atoi(bufferOpcao);
    } while (opcao_filtro != 1 && opcao_filtro != 2);

    if (opcao_filtro == 1)
    {
        // Busca por Nome
        do
        {
            printf("\nInforme o Nome do paciente: ");
            lerString(termo_busca, 50);
        } while (!validarNome(termo_busca));
        printf("\n═══ Consultas encontradas para o NOME: %s ═══\n", termo_busca);
    }
    else
    {
        // Busca por CPF
        do
        {
            printf("\nInforme o CPF do paciente: ");
            lerString(termo_busca, 15);
        } while (!validarCPF(termo_busca));
        printf("\n═══ Consultas encontradas para o CPF: %s ═══\n", termo_busca);
    }

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL)
    {
        printf("\nNenhuma consulta no sistema ou erro ao abrir arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("╔══════════════════════════╦═══════════════╦═════════════════╦════════════════════════════════════╦══════════╗\n");
    printf("║ Paciente                 ║ CPF           ║ Data/Hora       ║ Medico (Especialidade)             ║ Status   ║\n");
    printf("╠══════════════════════════╬═══════════════╬═════════════════╬════════════════════════════════════╬══════════╣\n");

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        // Verifica se está ativo
        if (consulta_lida.ativo == 1)
        {
            int match = 0;

            if (opcao_filtro == 1)
            {
                // Compara strings (pode usar strstr para busca parcial se desejar, aqui mantive exata)
                if (strcmp(consulta_lida.nome_paciente, termo_busca) == 0)
                {
                    match = 1;
                }
            }
            else
            {
                // Compara CPF
                if (strcmp(consulta_lida.cpf_paciente, termo_busca) == 0)
                {
                    match = 1;
                }
            }

            if (match)
            {
                snprintf(data_hora, sizeof(data_hora), "%s %s", consulta_lida.data, consulta_lida.hora);
                snprintf(medico_esp, sizeof(medico_esp), "%s (%s)", consulta_lida.nome_medico, consulta_lida.especialidade);
                printf("║ %-24s ║ %-13s ║ %-15s ║ %-34s ║ %-8s ║\n",
                       consulta_lida.nome_paciente, consulta_lida.cpf_paciente, data_hora, medico_esp, consulta_lida.status);
                tem_registro = 1;
            }
        }
    }
    printf("╚══════════════════════════╩═══════════════╩═════════════════╩════════════════════════════════════╩══════════╝\n");
    relatorios_exibir_mensagem_sem_resultado(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}
