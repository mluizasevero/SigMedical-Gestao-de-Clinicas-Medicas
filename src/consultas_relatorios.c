#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultas.h"
#include "utils.h"
#include "validador.h"
#include "consultas_relatorios.h"

#define CONSULTAS_FILE DATA_DIR PATH_SEPARATOR "consultas.dat"

// Função auxiliar para exibir um título de seção
static void exibir_titulo_relatorio_consulta(const char *titulo)
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
static void exibir_mensagem_sem_resultado_consulta(int tem_registro)
{
    if (!tem_registro)
    {
        printf("Nenhum registro encontrado.\n");
    }
}

void relatorio_consultas_por_medico(void)
{
    char nome_medico_pesquisa[50];
    int tem_registro = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    char data_hora[18];

    exibir_titulo_relatorio_consulta("Consultas por Medico");

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
    exibir_mensagem_sem_resultado_consulta(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_consultas_por_periodo(void)
{
    char data_inicio_str[11], data_fim_str[11];
    int tem_registro = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    char data_hora[18];
    long data_inicio_int, data_fim_int, data_consulta_int;

    exibir_titulo_relatorio_consulta("Consultas por Periodo");

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
    exibir_mensagem_sem_resultado_consulta(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_consultas_por_paciente(void)
{
    char termo_busca[50];
    int opcao_filtro;
    int tem_registro = 0;
    char bufferOpcao[5];
    Consulta consulta_lida;
    FILE *arq_consultas;
    char data_hora[18];
    char medico_esp[105];

    exibir_titulo_relatorio_consulta("Consultas por Paciente");

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
            if (!validarCPF(termo_busca))
            {
                printf("! CPF invalido. Tente novamente.\n");
            }
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
                // Compara strings (pode usar strstr para busca parcial se desejar, aqui manteve exata)
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
    exibir_mensagem_sem_resultado_consulta(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_consultas_agendadas(void)
{
    Consulta consulta_lida;
    FILE *arq_consultas;
    int tem_registro = 0;
    char data_hora[18];

    exibir_titulo_relatorio_consulta("Consultas Agendadas");

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL)
    {
        printf("Nenhuma consulta no sistema.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Consultas com status 'agendada' ═══\n");
    printf("╔══════════════════════════╦═════════════════╦══════════════════════════╦══════════════════════════╗\n");
    printf("║ Paciente                 ║ Data/Hora       ║ Medico                   ║ Especialidade            ║\n");
    printf("╠══════════════════════════╬═════════════════╬══════════════════════════╬══════════════════════════╣\n");

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (consulta_lida.ativo == 1 && strcmp(consulta_lida.status, "agendada") == 0)
        {
            snprintf(data_hora, sizeof(data_hora), "%s %s", consulta_lida.data, consulta_lida.hora);
            printf("║ %-24s ║ %-15s ║ %-24s ║ %-24s ║\n",
                   consulta_lida.nome_paciente, data_hora, consulta_lida.nome_medico, consulta_lida.especialidade);
            tem_registro = 1;
        }
    }
    printf("╚══════════════════════════╩═════════════════╩══════════════════════════╩══════════════════════════╝\n");
    exibir_mensagem_sem_resultado_consulta(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_consultas_canceladas(void)
{
    Consulta consulta_lida;
    FILE *arq_consultas;
    int tem_registro = 0;
    char data_hora[18];

    exibir_titulo_relatorio_consulta("Consultas Canceladas");

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL)
    {
        printf("Nenhuma consulta no sistema.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Consultas com status 'cancelada' ═══\n");
    printf("╔══════════════════════════╦═════════════════╦══════════════════════════╦══════════════════════════╗\n");
    printf("║ Paciente                 ║ Data/Hora       ║ Medico                   ║ Especialidade            ║\n");
    printf("╠══════════════════════════╬═════════════════╬══════════════════════════╬══════════════════════════╣\n");

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (consulta_lida.ativo == 0 && strcmp(consulta_lida.status, "cancelada") == 0)
        {
            snprintf(data_hora, sizeof(data_hora), "%s %s", consulta_lida.data, consulta_lida.hora);
            printf("║ %-24s ║ %-15s ║ %-24s ║ %-24s ║\n",
                   consulta_lida.nome_paciente, data_hora, consulta_lida.nome_medico, consulta_lida.especialidade);
            tem_registro = 1;
        }
    }
    printf("╚══════════════════════════╩═════════════════╩══════════════════════════╩══════════════════════════╝\n");
    exibir_mensagem_sem_resultado_consulta(tem_registro);

    fclose(arq_consultas);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

void relatorio_consultas_submenu(void)
{
    int opcao;
    char bufferOpcao[5];

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║       Relatorios de Consultas          ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ 1. Consultas por Médico                ║\n");
        printf("║ 2. Consultas por Paciente (Nome/CPF)   ║\n");
        printf("║ 3. Consultas por Período               ║\n");
        printf("║ 4. Consultas Agendadas (Geral)         ║\n");
        printf("║ 5. Consultas Canceladas (Geral)        ║\n");
        printf("║ 0. Voltar                              ║\n");
        printf("╚════════════════════════════════════════╝\n");

        // Leitura de menu segura
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
            relatorio_consultas_por_medico();
            break;
        case 2:
            relatorio_consultas_por_paciente();
            break;
        case 3:
            relatorio_consultas_por_periodo();
            break;
        case 4:
            relatorio_consultas_agendadas();
            break;
        case 5:
            relatorio_consultas_canceladas();
            break;
        case 0:
            break;
        }
    } while (opcao != 0);
}
