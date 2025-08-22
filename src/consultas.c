#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultas.h"
#include "utils.h"

void agendar_consulta(Consulta consultas[], int* total_consultas) {
    TelaAgendarNovaConsulta();
    if (*total_consultas < 100) {
        printf("\nInforme o nome do paciente: ");
        scanf(" %49[^\n]", consultas[*total_consultas].nome_paciente);
        while (getchar() != '\n');

        printf("Informe o CPF do paciente (apenas numeros): ");
        scanf("%11s", consultas[*total_consultas].cpf_paciente);
        while (getchar() != '\n');

        printf("Informe a data da consulta (dd/mm/aaaa): ");
        scanf("%10s", consultas[*total_consultas].data);
        while (getchar() != '\n');

        printf("Informe a hora da consulta (hh:mm): ");
        scanf("%5s", consultas[*total_consultas].hora);
        while (getchar() != '\n');

        printf("Informe o nome do medico: ");
        scanf(" %49[^\n]", consultas[*total_consultas].nome_medico);
        while (getchar() != '\n');

        printf("Informe a especialidade do medico: ");
        scanf(" %49[^\n]", consultas[*total_consultas].especialidade);
        while (getchar() != '\n');

        strcpy(consultas[*total_consultas].status, "agendada");

        (*total_consultas)++;
        printf("\nConsulta agendada com sucesso!\n");
    } else {
        printf("\nNao e possivel agendar mais consultas. Limite atingido.\n");
    }
    press_enter_to_continue();
}

void salvar_consultas(Consulta consultas[], int total_consultas) {
    FILE *arq_consultas = fopen("consultas.dat", "w");
    if (arq_consultas == NULL) {
        printf("Erro ao abrir o arquivo 'consultas.dat' para escrita.\n");
        return;
    }
    for (int i = 0; i < total_consultas; i++) {
        fprintf(arq_consultas, "%s;%s;%s;%s;%s;%s;%s\n",
                consultas[i].nome_paciente, consultas[i].cpf_paciente,
                consultas[i].data, consultas[i].hora,
                consultas[i].nome_medico, consultas[i].especialidade,
                consultas[i].status);
    }
    fclose(arq_consultas);
}

int ler_consultas(Consulta consultas[]) {
    FILE *arq_consultas = fopen("consultas.dat", "r");
    if (arq_consultas == NULL) {
        return 0;
    }
    int i = 0;
    while(fscanf(arq_consultas, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                consultas[i].nome_paciente, consultas[i].cpf_paciente,
                consultas[i].data, consultas[i].hora,
                consultas[i].nome_medico, consultas[i].especialidade,
                consultas[i].status) == 7) {
        i++;
    }
    fclose(arq_consultas);
    return i;
}

void pesquisar_consulta(Consulta consultas[], int total_consultas) {
    char cpf_pesquisa[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Pesquisar Consulta   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente: ");
    scanf("%11s", cpf_pesquisa);
    while (getchar() != '\n');

    for (int i = 0; i < total_consultas; i++) {
        if (strcmp(consultas[i].cpf_paciente, cpf_pesquisa) == 0) {
            printf("\nConsulta encontrada:\n");
            printf("Nome do Paciente: %s\n", consultas[i].nome_paciente);
            printf("CPF: %s\n", consultas[i].cpf_paciente);
            printf("Data: %s\n", consultas[i].data);
            printf("Hora: %s\n", consultas[i].hora);
            printf("Medico: %s\n", consultas[i].nome_medico);
            printf("Especialidade: %s\n", consultas[i].especialidade);
            printf("Status: %s\n", consultas[i].status);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nConsulta para o CPF %s nao encontrada.\n", cpf_pesquisa);
    }
    press_enter_to_continue();
}

void alterar_consulta(Consulta consultas[], int total_consultas) {
    char cpf_alteracao[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Alterar Consulta   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente que deseja alterar: ");
    scanf("%11s", cpf_alteracao);
    while (getchar() != '\n');

    for (int i = 0; i < total_consultas; i++) {
        if (strcmp(consultas[i].cpf_paciente, cpf_alteracao) == 0) {
            printf("\nConsulta encontrada. Informe os novos dados:\n");
            printf("Data atual: %s\n", consultas[i].data);
            printf("Nova Data (dd/mm/aaaa): ");
            scanf("%10s", consultas[i].data);
            while (getchar() != '\n');
            
            printf("Hora atual: %s\n", consultas[i].hora);
            printf("Nova Hora (hh:mm): ");
            scanf("%5s", consultas[i].hora);
            while (getchar() != '\n');
            
            printf("\nConsulta alterada com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nConsulta com CPF %s nao encontrada.\n", cpf_alteracao);
    }
    press_enter_to_continue();
}

void excluir_consulta(Consulta consultas[], int* total_consultas) {
    char cpf_exclusao[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Excluir Consulta   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente da consulta que deseja excluir: ");
    scanf("%11s", cpf_exclusao);
    while (getchar() != '\n');

    for (int i = 0; i < *total_consultas; i++) {
        if (strcmp(consultas[i].cpf_paciente, cpf_exclusao) == 0) {
            char confirmacao;
            printf("\nConsulta encontrada. Deseja realmente excluir esta consulta (S/N)? ");
            scanf(" %c", &confirmacao);
            while (getchar() != '\n');
            if (confirmacao == 'S' || confirmacao == 's') {
                for (int j = i; j < *total_consultas - 1; j++) {
                    consultas[j] = consultas[j + 1];
                }
                (*total_consultas)--;
                printf("\nConsulta excluida com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nConsulta com CPF %s nao encontrada.\n", cpf_exclusao);
    }
    press_enter_to_continue();
}

void gerenciar_agendamentos(Consulta consultas[], int* total_consultas) {
    int opcao;
    do {
        TelaGerenciarAgendamentos();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                alterar_consulta(consultas, *total_consultas);
                break;
            case 2:
                excluir_consulta(consultas, total_consultas);
                break;
            case 3:
                confirmar_presenca(consultas, *total_consultas);
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

void relatorio_consultas_medico(Consulta consultas[], int total_consultas) {
    char nome_medico_pesquisa[50];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Consultas por Medico   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o nome do medico para o relatorio: ");
    scanf(" %49[^\n]", nome_medico_pesquisa);
    while (getchar() != '\n');

    printf("\nRelatorio de Consultas para o Medico %s:\n", nome_medico_pesquisa);
    printf("----------------------------------------\n");

    for (int i = 0; i < total_consultas; i++) {
        if (strcmp(consultas[i].nome_medico, nome_medico_pesquisa) == 0) {
            printf("Paciente: %s\n", consultas[i].nome_paciente);
            printf("Data: %s\n", consultas[i].data);
            printf("Hora: %s\n", consultas[i].hora);
            printf("Especialidade: %s\n", consultas[i].especialidade);
            printf("Status: %s\n", consultas[i].status);
            printf("----------------------------------------\n");
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("Nenhuma consulta encontrada para este medico.\n");
    }
    press_enter_to_continue();
}

void relatorio_consultas_por_periodo(Consulta consultas[], int total_consultas) {
    char data_inicio_str[11], data_fim_str[11];
    int dia_inicio, mes_inicio, ano_inicio;
    int dia_fim, mes_fim, ano_fim;
    int dia_consulta, mes_consulta, ano_consulta;
    int encontrado = 0;
    
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Consultas por Periodo   ///\n");
    printf("----------------------------------------\n");
    printf("Informe a data de inicio (dd/mm/aaaa): ");
    scanf("%10s", data_inicio_str);
    while (getchar() != '\n');
    printf("Informe a data de fim (dd/mm/aaaa): ");
    scanf("%10s", data_fim_str);
    while (getchar() != '\n');
    
    sscanf(data_inicio_str, "%d/%d/%d", &dia_inicio, &mes_inicio, &ano_inicio);
    sscanf(data_fim_str, "%d/%d/%d", &dia_fim, &mes_fim, &ano_fim);

    printf("\nRelatorio de Consultas no Periodo de %s a %s:\n", data_inicio_str, data_fim_str);
    printf("----------------------------------------\n");

    for (int i = 0; i < total_consultas; i++) {
        sscanf(consultas[i].data, "%d/%d/%d", &dia_consulta, &mes_consulta, &ano_consulta);
        
        if (ano_consulta >= ano_inicio && ano_consulta <= ano_fim) {
            if (ano_consulta > ano_inicio && ano_consulta < ano_fim) {
                printf("Paciente: %s\n", consultas[i].nome_paciente);
                printf("Data: %s\n", consultas[i].data);
                printf("Hora: %s\n", consultas[i].hora);
                printf("Medico: %s\n", consultas[i].nome_medico);
                printf("Especialidade: %s\n", consultas[i].especialidade);
                printf("Status: %s\n", consultas[i].status);
                printf("----------------------------------------\n");
                encontrado = 1;
            } else if (ano_consulta == ano_inicio) {
                if (mes_consulta >= mes_inicio) {
                    if (mes_consulta > mes_inicio || (mes_consulta == mes_inicio && dia_consulta >= dia_inicio)) {
                        printf("Paciente: %s\n", consultas[i].nome_paciente);
                        printf("Data: %s\n", consultas[i].data);
                        printf("Hora: %s\n", consultas[i].hora);
                        printf("Medico: %s\n", consultas[i].nome_medico);
                        printf("Especialidade: %s\n", consultas[i].especialidade);
                        printf("Status: %s\n", consultas[i].status);
                        printf("----------------------------------------\n");
                        encontrado = 1;
                    }
                }
            } else if (ano_consulta == ano_fim) {
                if (mes_consulta <= mes_fim) {
                    if (mes_consulta < mes_fim || (mes_consulta == mes_fim && dia_consulta <= dia_fim)) {
                        printf("Paciente: %s\n", consultas[i].nome_paciente);
                        printf("Data: %s\n", consultas[i].data);
                        printf("Hora: %s\n", consultas[i].hora);
                        printf("Medico: %s\n", consultas[i].nome_medico);
                        printf("Especialidade: %s\n", consultas[i].especialidade);
                        printf("Status: %s\n", consultas[i].status);
                        printf("----------------------------------------\n");
                        encontrado = 1;
                    }
                }
            }
        }
    }

    if (!encontrado) {
        printf("Nenhuma consulta encontrada neste periodo.\n");
    }
    press_enter_to_continue();
}

void relatorio_consultas_agendadas(Consulta consultas[], int total_consultas) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Todas as Consultas Agendadas   ///\n");
    printf("----------------------------------------\n");
    if (total_consultas == 0) {
        printf("Nenhuma consulta agendada no momento.\n");
    } else {
        for (int i = 0; i < total_consultas; i++) {
            if (strcmp(consultas[i].status, "agendada") == 0) {
                printf("Paciente: %s\n", consultas[i].nome_paciente);
                printf("Data: %s\n", consultas[i].data);
                printf("Hora: %s\n", consultas[i].hora);
                printf("Medico: %s\n", consultas[i].nome_medico);
                printf("Especialidade: %s\n", consultas[i].especialidade);
                printf("----------------------------------------\n");
            }
        }
    }
    press_enter_to_continue();
}

void relatorio_consultas_canceladas(Consulta consultas[], int total_consultas) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Consultas Canceladas   ///\n");
    printf("----------------------------------------\n");
    int encontrado = 0;
    for (int i = 0; i < total_consultas; i++) {
        if (strcmp(consultas[i].status, "cancelada") == 0) {
            printf("Paciente: %s\n", consultas[i].nome_paciente);
            printf("Data: %s\n", consultas[i].data);
            printf("Hora: %s\n", consultas[i].hora);
            printf("Medico: %s\n", consultas[i].nome_medico);
            printf("Especialidade: %s\n", consultas[i].especialidade);
            printf("----------------------------------------\n");
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("Nenhuma consulta cancelada encontrada.\n");
    }
    press_enter_to_continue();
}

void gerar_relatorios_consultas(Consulta consultas[], int total_consultas) {
    int opcao;
    do {
        TelaGerarRelatorios();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                relatorio_consultas_medico(consultas, total_consultas);
                break;
            case 2:
                relatorio_consultas_por_periodo(consultas, total_consultas);
                break;
            case 3:
                relatorio_consultas_agendadas(consultas, total_consultas);
                break;
            case 4:
                relatorio_consultas_canceladas(consultas, total_consultas);
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

void confirmar_presenca(Consulta consultas[], int total_consultas) {
    char cpf_busca[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Confirmar Presenca   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente para confirmar a presenca: ");
    scanf("%11s", cpf_busca);
    while (getchar() != '\n');

    for (int i = 0; i < total_consultas; i++) {
        if (strcmp(consultas[i].cpf_paciente, cpf_busca) == 0) {
            printf("\nConsulta encontrada para o paciente %s.\n", consultas[i].nome_paciente);
            strcpy(consultas[i].status, "concluida");
            printf("Presenca confirmada e status da consulta atualizado!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nNenhuma consulta para o CPF %s encontrada.\n", cpf_busca);
    }
    press_enter_to_continue();
}

void modulo_consultas(Consulta consultas[], int* total_consultas) {
    int opcao;
    do {
        TelaMenuConsultas();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                agendar_consulta(consultas, total_consultas);
                break;
            case 2:
                pesquisar_consulta(consultas, *total_consultas);
                break;
            case 3:
                gerenciar_agendamentos(consultas, total_consultas);
                break;
            case 4:
                gerar_relatorios_consultas(consultas, *total_consultas);
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
