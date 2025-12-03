#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicos.h"
#include "utils.h"
#include "validador.h"
#include "medicos_relatorios.h"

#define MEDICOS_FILE DATA_DIR PATH_SEPARATOR "medicos.dat"

// Estrutura para lista dinâmica (lista ligada)
typedef struct NoMedico
{
    Medico medico;
    struct NoMedico *proximo;
} NoMedico;

// Função para criar um novo nó
static NoMedico *criar_no_medico(Medico medico)
{
    NoMedico *novo_no = (NoMedico *)malloc(sizeof(NoMedico));
    if (novo_no != NULL)
    {
        novo_no->medico = medico;
        novo_no->proximo = NULL;
    }
    return novo_no;
}

// Função para adicionar um médico à lista
static void adicionar_no_medico(NoMedico **cabeca, Medico medico)
{
    NoMedico *novo_no = criar_no_medico(medico);
    if (*cabeca == NULL)
    {
        *cabeca = novo_no;
    }
    else
    {
        NoMedico *temp = *cabeca;
        while (temp->proximo != NULL)
        {
            temp = temp->proximo;
        }
        temp->proximo = novo_no;
    }
}

// Função para liberar a memória da lista
static void liberar_lista_medicos_relatorio(NoMedico *cabeca)
{
    NoMedico *temp;
    while (cabeca != NULL)
    {
        temp = cabeca;
        cabeca = cabeca->proximo;
        free(temp);
    }
}

// Função para carregar médicos do arquivo para a lista dinâmica
static NoMedico *carregar_medicos_para_lista_relatorio(void)
{
    FILE *arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL)
    {
        return NULL;
    }

    NoMedico *cabeca = NULL;
    Medico medico_lido;
    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        adicionar_no_medico(&cabeca, medico_lido);
    }

    fclose(arq_medicos);
    return cabeca;
}

// Função auxiliar para exibir um título de seção
static void exibir_titulo_relatorio_medico(const char *titulo)
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
static void exibir_mensagem_sem_resultado_medico(int tem_registro)
{
    if (!tem_registro)
    {
        printf("Nenhum registro encontrado.\n");
    }
}

void relatorio_medicos_completo(void)
{
    exibir_titulo_relatorio_medico("Relatorio Completo de Medicos");

    NoMedico *lista_medicos = carregar_medicos_para_lista_relatorio();
    if (lista_medicos == NULL)
    {
        printf("Nenhum medico cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    int total_ativos = 0, total_inativos = 0, total_geral = 0;

    // Contagem de médicos
    NoMedico *temp = lista_medicos;
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

    liberar_lista_medicos_relatorio(lista_medicos);

    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_medicos_por_especialidade(void)
{
    Medico medico_lido;
    FILE *arq_medicos;
    char especialidade_filtro[51];
    int tem_registro = 0;

    exibir_titulo_relatorio_medico("Relatorio por Especialidade");

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
    exibir_mensagem_sem_resultado_medico(tem_registro);

    fclose(arq_medicos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_medicos_por_nome(void)
{
    Medico medico_lido;
    FILE *arq_medicos;
    char nome_filtro[51];
    int tem_registro = 0;

    exibir_titulo_relatorio_medico("Relatorio por Nome (Parcial)");

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
    exibir_mensagem_sem_resultado_medico(tem_registro);

    fclose(arq_medicos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_medicos_por_status(void)
{
    Medico medico_lido;
    FILE *arq_medicos;
    int status_filtro;
    int tem_registro = 0;
    char status_texto[10];

    exibir_titulo_relatorio_medico("Relatorio por Status");

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
        if ((status_filtro == 1 && medico_lido.ativo == 1) ||
            (status_filtro == 2 && medico_lido.ativo == 0))
        {
            printf("║ %-2d ║ %-22s ║ %-13s ║ %-16s ║\n",
                   medico_lido.id, medico_lido.nome,
                   medico_lido.cpf, medico_lido.especialidade);
            tem_registro = 1;
        }
    }
    printf("╚════╩════════════════════════╩═══════════════╩══════════════════╝\n");
    exibir_mensagem_sem_resultado_medico(tem_registro);

    fclose(arq_medicos);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_medicos_submenu(void)
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
            relatorio_medicos_completo();
            break;
        case 2:
            relatorio_medicos_por_especialidade();
            break;
        case 3:
            relatorio_medicos_por_nome();
            break;
        case 4:
            relatorio_medicos_por_status();
            break;
        case 0:
            break;
        }
    } while (opcao_relatorio != 0);
}
