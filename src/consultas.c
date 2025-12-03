#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "consultas.h"
#include "utils.h"
#include "validador.h"
#include "consultas_relatorios.h"

#define CONSULTAS_FILE DATA_DIR PATH_SEPARATOR "consultas.dat"

void agendar_consulta(void)
{
    Consulta nova_consulta;
    FILE *arq_consultas;

    // Buffer para leitura segura
    char buffer[51];

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║         Agendar Nova Consulta          ║\n");
    printf("╚════════════════════════════════════════╝\n");

    // Loop de validação para Nome do Paciente
    do
    {
        printf("\nInforme o nome do paciente: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer));
    strcpy(nova_consulta.nome_paciente, buffer);

    // Loop de validação para CPF
    do
    {
        printf("Informe o CPF do paciente: ");
        lerString(buffer, 15);
        if (!validarCPF(buffer))
        {
            printf("! CPF invalido. Tente novamente.\n");
        }
    } while (!validarCPF(buffer));
    strcpy(nova_consulta.cpf_paciente, buffer);

    // Loop de validação para Data
    do
    {
        printf("Informe a data da consulta (dd/mm/aaaa): ");
        lerString(buffer, 11);
    } while (!validarData(buffer));
    strcpy(nova_consulta.data, buffer);

    // Loop de validação para Hora
    do
    {
        printf("Informe a hora da consulta (hh:mm): ");
        lerString(buffer, 6);
    } while (!validarHora(buffer));
    strcpy(nova_consulta.hora, buffer);

    // Loop de validação para Nome do Médico
    do
    {
        printf("Informe o nome do medico: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer));
    strcpy(nova_consulta.nome_medico, buffer);

    // Loop de validação para Especialidade (usando validarNome)
    do
    {
        printf("Informe a especialidade do medico: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer)); // Reutilizando validarNome (mín 3 letras, sem num)
    strcpy(nova_consulta.especialidade, buffer);

    strcpy(nova_consulta.status, "agendada");
    nova_consulta.ativo = 1;

    arq_consultas = fopen(CONSULTAS_FILE, "ab");
    if (arq_consultas == NULL)
    {
        printf("\nErro ao abrir o arquivo para agendamento!\n");
        pressioneEnterParaContinuar();
        return;
    }

    fwrite(&nova_consulta, sizeof(Consulta), 1, arq_consultas);
    fclose(arq_consultas);

    printf("\nConsulta agendada com sucesso!\n");
    pressioneEnterParaContinuar();
}

void pesquisar_consulta(void)
{
    char cpf_pesquisa[15];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    char data_hora[18];
    /* Espaço suficiente para "nome_medico (especialidade)" quando cada campo pode ter até 50 chars */
    char medico_esp[105];

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║           Pesquisar Consulta           ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("Informe o CPF do paciente: ");
    lerString(cpf_pesquisa, 15);

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL)
    {
        printf("\nNenhuma consulta agendada ou erro ao abrir arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Consultas Encontradas para o CPF: %s ═══\n", cpf_pesquisa);
    printf("╔══════════════════════════╦═════════════════╦════════════════════════════════════╦══════════╗\n");
    printf("║ Paciente                 ║ Data/Hora       ║ Medico (Especialidade)             ║ Status   ║\n");
    printf("╠══════════════════════════╬═════════════════╬════════════════════════════════════╬══════════╣\n");

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (strcmp(consulta_lida.cpf_paciente, cpf_pesquisa) == 0 && consulta_lida.ativo == 1)
        {
            snprintf(data_hora, sizeof(data_hora), "%s %s", consulta_lida.data, consulta_lida.hora);
            snprintf(medico_esp, sizeof(medico_esp), "%s (%s)", consulta_lida.nome_medico, consulta_lida.especialidade);
            printf("║ %-24s ║ %-15s ║ %-34s ║ %-8s ║\n",
                   consulta_lida.nome_paciente, data_hora, medico_esp, consulta_lida.status);
            encontrado = 1;
        }
    }
    printf("╚══════════════════════════╩═════════════════╩════════════════════════════════════╩══════════╝\n");
    fclose(arq_consultas);

    if (!encontrado)
    {
        printf("\nNenhuma consulta ativa para o CPF %s foi encontrada.\n", cpf_pesquisa);
    }
    pressioneEnterParaContinuar();
}

void alterar_consulta(void)
{
    char cpf_busca[15];
    char data_busca[11];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    long int pos;

    // Buffer para leitura
    char buffer[51];

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║            Alterar Consulta            ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("Informe o CPF do paciente: ");
    lerString(cpf_busca, 15);

    // Validação da Data
    do
    {
        printf("Informe a DATA da consulta a alterar (dd/mm/aaaa): ");
        lerString(data_busca, 11);
    } while (!validarData(data_busca));

    arq_consultas = fopen(CONSULTAS_FILE, "r+b");
    if (arq_consultas == NULL)
    {
        printf("\nErro ao abrir arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (strcmp(consulta_lida.cpf_paciente, cpf_busca) == 0 &&
            strcmp(consulta_lida.data, data_busca) == 0 &&
            consulta_lida.ativo == 1)
        {

            pos = ftell(arq_consultas) - sizeof(Consulta);

            printf("\nConsulta encontrada. Informe os novos dados:\n");

            // Validação da Nova Data
            do
            {
                printf("Data atual: %s\nNova Data (dd/mm/aaaa): ", consulta_lida.data);
                lerString(buffer, 11);
            } while (!validarData(buffer));
            strcpy(consulta_lida.data, buffer);

            // Validação da Nova Hora
            do
            {
                printf("Hora atual: %s\nNova Hora (hh:mm): ", consulta_lida.hora);
                lerString(buffer, 6);
            } while (!validarHora(buffer));
            strcpy(consulta_lida.hora, buffer);

            fseek(arq_consultas, pos, SEEK_SET);
            fwrite(&consulta_lida, sizeof(Consulta), 1, arq_consultas);

            encontrado = 1;
            printf("\nConsulta alterada com sucesso!\n");
            break;
        }
    }
    fclose(arq_consultas);

    if (!encontrado)
    {
        printf("\nConsulta para o CPF %s na data %s nao encontrada.\n", cpf_busca, data_busca);
    }
    pressioneEnterParaContinuar();
}

void excluir_consulta(void)
{
    char cpf_busca[15];
    char data_busca[11];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    long int pos;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║       Cancelar/Excluir Consulta        ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("Informe o CPF do paciente: ");
    lerString(cpf_busca, 15);
    // Validação da Data
    do
    {
        printf("Informe a DATA da consulta a excluir (dd/mm/aaaa): ");
        lerString(data_busca, 11);
    } while (!validarData(data_busca));

    arq_consultas = fopen(CONSULTAS_FILE, "r+b");
    if (arq_consultas == NULL)
    {
        printf("\nErro ao abrir arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (strcmp(consulta_lida.cpf_paciente, cpf_busca) == 0 &&
            strcmp(consulta_lida.data, data_busca) == 0 &&
            consulta_lida.ativo == 1)
        {

            pos = ftell(arq_consultas) - sizeof(Consulta);

            char confirmacao;
            char bufferConfirm[5]; // Buffer para confirmação

            printf("\nConsulta encontrada:\n");
            printf("Paciente: %s\nData: %s | Hora: %s\n", consulta_lida.nome_paciente, consulta_lida.data, consulta_lida.hora);

            // Loop de confirmação
            do
            {
                printf("Deseja realmente cancelar esta consulta (S/N)? ");
                lerString(bufferConfirm, 5);
                confirmacao = toupper(bufferConfirm[0]); // Pega o primeiro caractere
                if (confirmacao != 'S' && confirmacao != 'N')
                {
                    printf("! Opcao invalida. Digite 'S' para Sim ou 'N' para Nao.\n");
                }
            } while (confirmacao != 'S' && confirmacao != 'N');

            if (confirmacao == 'S')
            {
                consulta_lida.ativo = 0;                   // Exclusão lógica
                strcpy(consulta_lida.status, "cancelada"); // Atualiza status

                fseek(arq_consultas, pos, SEEK_SET);
                fwrite(&consulta_lida, sizeof(Consulta), 1, arq_consultas);
                printf("\nConsulta cancelada com sucesso!\n");
            }
            else
            {
                printf("\nOperacao cancelada.\n");
            }
            encontrado = 1;
            break;
        }
    }
    fclose(arq_consultas);

    if (!encontrado)
    {
        printf("\nConsulta para o CPF %s na data %s nao encontrada.\n", cpf_busca, data_busca);
    }
    pressioneEnterParaContinuar();
}

void confirmar_presenca(void)
{
    char cpf_busca[15];
    char data_busca[11];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;
    long int pos;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║           Confirmar Presença           ║\n");
    printf("╚════════════════════════════════════════╝\n");

    // Validação do CPF
    do
    {
        printf("Informe o CPF do paciente: ");
        lerString(cpf_busca, 15);
    } while (!validarCPF(cpf_busca));

    // Validação da Data
    do
    {
        printf("Informe a data da consulta (dd/mm/aaaa): ");
        lerString(data_busca, 11);
    } while (!validarData(data_busca));

    arq_consultas = fopen(CONSULTAS_FILE, "r+b");
    if (arq_consultas == NULL)
    {
        printf("\nErro ao abrir arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (strcmp(consulta_lida.cpf_paciente, cpf_busca) == 0 &&
            strcmp(consulta_lida.data, data_busca) == 0 &&
            consulta_lida.ativo == 1)
        {

            pos = ftell(arq_consultas) - sizeof(Consulta);

            printf("\nConsulta encontrada para o paciente %s.\n", consulta_lida.nome_paciente);
            strcpy(consulta_lida.status, "concluida");

            fseek(arq_consultas, pos, SEEK_SET);
            fwrite(&consulta_lida, sizeof(Consulta), 1, arq_consultas);

            printf("Presenca confirmada e status da consulta atualizado!\n");
            encontrado = 1;
            break;
        }
    }
    fclose(arq_consultas);

    if (!encontrado)
    {
        printf("\nNenhuma consulta para o CPF %s na data %s encontrada.\n", cpf_busca, data_busca);
    }
    pressioneEnterParaContinuar();
}

// ... (Funções relatorio_consultas_agendadas e relatorio_consultas_canceladas
//     não recebem input, então não precisam de mudanças, exceto pela
//     checagem de status em relatorio_consultas_canceladas que está correta) ...

void gerenciar_agendamentos(void)
{
    int opcao;
    char bufferOpcao[5]; // Buffer de leitura

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║         Gerenciar Agendamentos         ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ 1. Alterar Data/Hora da Consulta       ║\n");
        printf("║ 2. Cancelar/Excluir Consulta           ║\n");
        printf("║ 3. Confirmar Presença (Concluir)       ║\n");
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
        } while (!validarOpcaoMenu(opcao, 0, 3));

        switch (opcao)
        {
        case 1:
            alterar_consulta();
            break;
        case 2:
            excluir_consulta();
            break;
        case 3:
            confirmar_presenca();
            break;
        case 0:
            break;
            // Default não é mais necessário
        }
    } while (opcao != 0);
}

void gerar_relatorios_consultas(void)
{
    relatorio_consultas_submenu();
}

void modulo_consultas(void)
{
    int opcao;
    char bufferOpcao[5]; // Buffer de leitura

    criarPastaData();

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║          Módulo de Consultas           ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ 1. Agendar Consulta                    ║\n");
        printf("║ 2. Pesquisar Consultas do Paciente     ║\n");
        printf("║ 3. Gerenciar Agendamentos              ║\n");
        printf("║ 4. Gerar Relatórios                    ║\n");
        printf("║ 0. Voltar ao menu principal            ║\n");
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
        } while (!validarOpcaoMenu(opcao, 0, 4));

        switch (opcao)
        {
        case 1:
            agendar_consulta();
            break;
        case 2:
            pesquisar_consulta();
            break;
        case 3:
            gerenciar_agendamentos();
            break;
        case 4:
            gerar_relatorios_consultas();
            break;
        case 0:
            break;
        }
    } while (opcao != 0);
}
