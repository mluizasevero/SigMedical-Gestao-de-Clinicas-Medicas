#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "consultas.h"
#include "utils.h"
#include "validador.h"

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

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║         Pesquisar Consulta             ║\n");
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

    printf("\n--- Consultas Encontradas para o CPF: %s ---\n", cpf_pesquisa);
    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (strcmp(consulta_lida.cpf_paciente, cpf_pesquisa) == 0 && consulta_lida.ativo == 1)
        {
           printf("╔════════════════════════════════════════╗\n");
           printf("║ Paciente: %-33s ║\n", consulta_lida.nome_paciente);
           printf("║ Data: %-10s | Hora: %-8s       ║\n", consulta_lida.data, consulta_lida.hora);
           printf("║ Médico: %-25s (%-10s) ║\n", consulta_lida.nome_medico, consulta_lida.especialidade);
           printf("║ Status: %-33s ║\n", consulta_lida.status);
           printf("╚════════════════════════════════════════╝\n");

            encontrado = 1;
        }
    }
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
    printf("║          Alterar Consulta              ║\n");
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
    printf("║      Cancelar/Excluir Consulta         ║\n");
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
    printf("║        Confirmar Presença              ║\n");
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

void relatorio_consultas_medico(void)
{
    char nome_medico_pesquisa[50];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║        Consultas por Médico            ║\n");
    printf("╚════════════════════════════════════════╝\n");

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

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (consulta_lida.ativo == 1 && strcmp(consulta_lida.nome_medico, nome_medico_pesquisa) == 0)
        {
            printf("----------------------------------------\n");
            printf("Paciente: %s (CPF: %s)\n", consulta_lida.nome_paciente, consulta_lida.cpf_paciente);
            printf("Data: %s | Hora: %s\n", consulta_lida.data, consulta_lida.hora);
            printf("Status: %s\n", consulta_lida.status);
            encontrado = 1;
        }
    }
    fclose(arq_consultas);

    if (!encontrado)
    {
        printf("\nNenhuma consulta ativa encontrada para este medico.\n");
    }
    pressioneEnterParaContinuar();
}

void relatorio_consultas_por_periodo(void)
{
    char data_inicio_str[11], data_fim_str[11];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE *arq_consultas;

    // Variáveis para datas convertidas
    long data_inicio_int, data_fim_int, data_consulta_int;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║        Consultas por Período           ║\n");
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

    // Converte datas para inteiros para comparação correta
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

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas))
    {
        if (consulta_lida.ativo == 1)
        {

            // Comparação numérica de datas (YYYYMMDD)
            data_consulta_int = converterDataParaInt(consulta_lida.data);

            if (data_consulta_int >= data_inicio_int && data_consulta_int <= data_fim_int)
            {
                printf("╔════════════════════════════════════════╗\n");
                printf("║ Paciente: %-33s ║\n", consulta_lida.nome_paciente);
                printf("║ Data: %-10s | Hora: %-8s       ║\n", consulta_lida.data, consulta_lida.hora);
                printf("║ Médico: %-33s ║\n", consulta_lida.nome_medico);
                printf("║ Status: %-33s ║\n", consulta_lida.status);
                printf("╚════════════════════════════════════════╝\n");
                encontrado = 1;
            }
        }
    }
    fclose(arq_consultas);

    if (!encontrado)
    {
        printf("Nenhuma consulta encontrada neste Periodo.\n");
    }
    pressioneEnterParaContinuar();
}

void relatorio_consultas_paciente(void)
{
    char termo_busca[50];
    int opcao_filtro;
    int encontrado = 0;
    char bufferOpcao[5];
    Consulta consulta_lida;
    FILE *arq_consultas;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║      Relatório de Consultas Paciente   ║\n");
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
        printf("\n--- Consultas encontradas para o NOME: %s ---\n", termo_busca);
    }
    else
    {
        // Busca por CPF
        do
        {
            printf("\nInforme o CPF do paciente: ");
            lerString(termo_busca, 15);
        } while (!validarCPF(termo_busca));
        printf("\n--- Consultas encontradas para o CPF: %s ---\n", termo_busca);
    }

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL)
    {
        printf("\nNenhuma consulta no sistema ou erro ao abrir arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

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
                printf("╔════════════════════════════════════════╗\n");
                printf("║ Paciente: %-33s ║\n", consulta_lida.nome_paciente);
                printf("║ CPF: %-38s ║\n", consulta_lida.cpf_paciente);
                printf("║ Data: %-10s | Hora: %-8s       ║\n", consulta_lida.data, consulta_lida.hora);
                printf("║ Médico: %-25s (%-10s) ║\n", consulta_lida.nome_medico, consulta_lida.especialidade);
                printf("║ Status: %-33s ║\n", consulta_lida.status);
                printf("╚════════════════════════════════════════╝\n");
                encontrado = 1;
            }
        }
    }
    fclose(arq_consultas);

    if (!encontrado)
    {
        printf("\nNenhuma consulta encontrada para os dados informados.\n");
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
        printf("║       Gerenciar Agendamentos           ║\n");
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
    int opcao;
    char bufferOpcao[5]; // Buffer de leitura

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║           Gerar Relatórios             ║\n");
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
            relatorio_consultas_medico();
            break;
        case 2:
            relatorio_consultas_paciente(); // Chamada da nova função
            break;
        case 3:
            relatorio_consultas_por_periodo();
            break;
        case 4:
            // relatorio_consultas_agendadas(); 
            printf("\nFuncionalidade em desenvolvimento.\n");
            pressioneEnterParaContinuar();
            break;
        case 5:
            // relatorio_consultas_canceladas(); 
            printf("\nFuncionalidade em desenvolvimento.\n");
            pressioneEnterParaContinuar();
            break;
        case 0:
            break;
        }
    } while (opcao != 0);
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
        printf("║           Módulo de Consultas          ║\n");
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
