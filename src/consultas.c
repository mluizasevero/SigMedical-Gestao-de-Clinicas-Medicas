#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultas.h"
#include "utils.h"

#define CONSULTAS_FILE DATA_DIR PATH_SEPARATOR "consultas.dat"

void agendar_consulta(void) {
    Consulta nova_consulta;
    FILE* arq_consultas;

    limpar_tela(); 
    printf("----------------------------------------\n");
    printf("///    Agendar Nova Consulta       ///\n");
    printf("----------------------------------------\n");

    printf("\nInforme o nome do paciente: ");
    scanf(" %49[^\n]", nova_consulta.nome_paciente);
    while (getchar() != '\n');

    printf("Informe o CPF do paciente (apenas numeros): ");
    scanf("%14s", nova_consulta.cpf_paciente);
    while (getchar() != '\n');

    printf("Informe a data da consulta (dd/mm/aaaa): ");
    scanf("%10s", nova_consulta.data);
    while (getchar() != '\n');

    printf("Informe a hora da consulta (hh:mm): ");
    scanf("%5s", nova_consulta.hora);
    while (getchar() != '\n');

    printf("Informe o nome do medico: ");
    scanf(" %49[^\n]", nova_consulta.nome_medico);
    while (getchar() != '\n');

    printf("Informe a especialidade do medico: ");
    scanf(" %49[^\n]", nova_consulta.especialidade);
    while (getchar() != '\n');

    strcpy(nova_consulta.status, "agendada");
    nova_consulta.ativo = 1;

    arq_consultas = fopen(CONSULTAS_FILE, "ab");
    if (arq_consultas == NULL) {
        printf("\nErro ao abrir o arquivo para agendamento!\n");
        press_enter_to_continue();
        return;
    }

    fwrite(&nova_consulta, sizeof(Consulta), 1, arq_consultas);
    fclose(arq_consultas);

    printf("\nConsulta agendada com sucesso!\n");
    press_enter_to_continue();
}


void pesquisar_consulta(void) {
    char cpf_pesquisa[15];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///      Pesquisar Consulta        ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente: ");
    scanf("%14s", cpf_pesquisa);
    while (getchar() != '\n');

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL) {
        printf("\nNenhuma consulta agendada ou erro ao abrir arquivo.\n");
        press_enter_to_continue();
        return;
    }

    printf("\n--- Consultas Encontradas para o CPF: %s ---\n", cpf_pesquisa);
    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (strcmp(consulta_lida.cpf_paciente, cpf_pesquisa) == 0 && consulta_lida.ativo == 1) {
            printf("Paciente: %s\n", consulta_lida.nome_paciente);
            printf("Data: %s | Hora: %s\n", consulta_lida.data, consulta_lida.hora);
            printf("Medico: %s (%s)\n", consulta_lida.nome_medico, consulta_lida.especialidade);
            printf("Status: %s\n", consulta_lida.status);
            printf("----------------------------------------\n");
            encontrado = 1;
        }
    }
    fclose(arq_consultas);

    if (!encontrado) {
        printf("\nNenhuma consulta ativa para o CPF %s foi encontrada.\n", cpf_pesquisa);
    }
    press_enter_to_continue();
}


void alterar_consulta(void) {
    char cpf_busca[15];
    char data_busca[11];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;
    long int pos;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///      Alterar Consulta          ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');
    printf("Informe a DATA da consulta a alterar (dd/mm/aaaa): ");
    scanf("%10s", data_busca);
    while (getchar() != '\n');


    arq_consultas = fopen(CONSULTAS_FILE, "r+b");
    if (arq_consultas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (strcmp(consulta_lida.cpf_paciente, cpf_busca) == 0 && 
            strcmp(consulta_lida.data, data_busca) == 0 && 
            consulta_lida.ativo == 1) {
            
            pos = ftell(arq_consultas) - sizeof(Consulta);

            printf("\nConsulta encontrada. Informe os novos dados:\n");
            printf("Data atual: %s\nNova Data (dd/mm/aaaa): ", consulta_lida.data);
            scanf("%10s", consulta_lida.data);
            while (getchar() != '\n');
            
            printf("Hora atual: %s\nNova Hora (hh:mm): ", consulta_lida.hora);
            scanf("%5s", consulta_lida.hora);
            while (getchar() != '\n');
            
            fseek(arq_consultas, pos, SEEK_SET);
            fwrite(&consulta_lida, sizeof(Consulta), 1, arq_consultas);
            
            encontrado = 1;
            printf("\nConsulta alterada com sucesso!\n");
            break;
        }
    }
    fclose(arq_consultas);

    if (!encontrado) {
        printf("\nConsulta para o CPF %s na data %s nao encontrada.\n", cpf_busca, data_busca);
    }
    press_enter_to_continue();
}


void excluir_consulta(void) {
    char cpf_busca[15];
    char data_busca[11];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;
    long int pos;
    
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Cancelar/Excluir Consulta    ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');
    printf("Informe a DATA da consulta a excluir (dd/mm/aaaa): ");
    scanf("%10s", data_busca);
    while (getchar() != '\n');

    arq_consultas = fopen(CONSULTAS_FILE, "r+b");
    if (arq_consultas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (strcmp(consulta_lida.cpf_paciente, cpf_busca) == 0 && 
            strcmp(consulta_lida.data, data_busca) == 0 && 
            consulta_lida.ativo == 1) {
            
            pos = ftell(arq_consultas) - sizeof(Consulta);
            
            char confirmacao;
            printf("\nConsulta encontrada:\n");
            printf("Paciente: %s\nData: %s | Hora: %s\n", consulta_lida.nome_paciente, consulta_lida.data, consulta_lida.hora);
            printf("Deseja realmente cancelar esta consulta (S/N)? ");
            scanf(" %c", &confirmacao);
            while (getchar() != '\n');

            if (confirmacao == 'S' || confirmacao == 's') {
                consulta_lida.ativo = 0; // Exclusão lógica
                strcpy(consulta_lida.status, "cancelada"); // Atualiza status
                
                fseek(arq_consultas, pos, SEEK_SET);
                fwrite(&consulta_lida, sizeof(Consulta), 1, arq_consultas);
                printf("\nConsulta cancelada com sucesso!\n");
            } else {
                printf("\nOperacao cancelada.\n");
            }
            encontrado = 1;
            break;
        }
    }
    fclose(arq_consultas);

    if (!encontrado) {
        printf("\nConsulta para o CPF %s na data %s nao encontrada.\n", cpf_busca, data_busca);
    }
    press_enter_to_continue();
}


void confirmar_presenca(void) {
    char cpf_busca[15];
    char data_busca[11];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;
    long int pos;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Confirmar Presenca         ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');
    printf("Informe a data da consulta (dd/mm/aaaa): ");
    scanf("%10s", data_busca);
    while (getchar() != '\n');

    arq_consultas = fopen(CONSULTAS_FILE, "r+b");
    if (arq_consultas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (strcmp(consulta_lida.cpf_paciente, cpf_busca) == 0 && 
            strcmp(consulta_lida.data, data_busca) == 0 && 
            consulta_lida.ativo == 1) {
            
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

    if (!encontrado) {
        printf("\nNenhuma consulta para o CPF %s na data %s encontrada.\n", cpf_busca, data_busca);
    }
    press_enter_to_continue();
}


// lendo o arqv em binário

void relatorio_consultas_medico(void) {
    char nome_medico_pesquisa[50];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Consultas por Medico       ///\n");
    printf("----------------------------------------\n");
    printf("Informe o nome do medico: ");
    scanf(" %49[^\n]", nome_medico_pesquisa);
    while (getchar() != '\n');

    printf("\nRelatorio de Consultas para o Medico: %s\n", nome_medico_pesquisa);
    
    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL) {
        printf("Nenhuma consulta no sistema.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (consulta_lida.ativo == 1 && strcmp(consulta_lida.nome_medico, nome_medico_pesquisa) == 0) {
            printf("----------------------------------------\n");
            printf("Paciente: %s (CPF: %s)\n", consulta_lida.nome_paciente, consulta_lida.cpf_paciente);
            printf("Data: %s | Hora: %s\n", consulta_lida.data, consulta_lida.hora);
            printf("Status: %s\n", consulta_lida.status);
            encontrado = 1;
        }
    }
    fclose(arq_consultas);

    if (!encontrado) {
        printf("\nNenhuma consulta ativa encontrada para este medico.\n");
    }
    press_enter_to_continue();
}

void relatorio_consultas_por_periodo(void) {
    char data_inicio_str[11], data_fim_str[11];
    // declarações de dia/mês/ano
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;
    
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Consultas por Periodo      ///\n");
    printf("----------------------------------------\n");
    printf("Informe a data de inicio (dd/mm/aaaa): ");
    scanf("%10s", data_inicio_str);
    while (getchar() != '\n');
    printf("Informe a data de fim (dd/mm/aaaa): ");
    scanf("%10s", data_fim_str);
    while (getchar() != '\n');
    
    // sscanf(data_inicio_str, ...);
    // sscanf(data_fim_str, ...);

    printf("\nRelatorio de Consultas no Periodo de %s a %s:\n", data_inicio_str, data_fim_str);

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL) {
        printf("Nenhuma consulta no sistema.\n");
        press_enter_to_continue();
        return;
    }

    while(fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (consulta_lida.ativo == 1) {
            if (strcmp(consulta_lida.data, data_inicio_str) >= 0 && strcmp(consulta_lida.data, data_fim_str) <= 0) {
                 printf("----------------------------------------\n");
                 printf("Paciente: %s\n", consulta_lida.nome_paciente);
                 printf("Data: %s | Hora: %s\n", consulta_lida.data, consulta_lida.hora);
                 printf("Medico: %s\n", consulta_lida.nome_medico);
                 printf("Status: %s\n", consulta_lida.status);
                 encontrado = 1;
            }
        }
    }
    fclose(arq_consultas);

    if (!encontrado) {
        printf("Nenhuma consulta encontrada neste Periodo.\n");
    }
    press_enter_to_continue();
}

void relatorio_consultas_agendadas(void) {
    Consulta consulta_lida;
    FILE* arq_consultas;
    int encontrado = 0;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Todas as Consultas Agendadas   ///\n");
    printf("----------------------------------------\n");

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL) {
        printf("Nenhuma consulta no sistema.\n");
        press_enter_to_continue();
        return;
    }
    
    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (consulta_lida.ativo == 1 && strcmp(consulta_lida.status, "agendada") == 0) {
            printf("Paciente: %s (CPF: %s)\n", consulta_lida.nome_paciente, consulta_lida.cpf_paciente);
            printf("Data: %s | Hora: %s\n", consulta_lida.data, consulta_lida.hora);
            printf("Medico: %s (%s)\n", consulta_lida.nome_medico, consulta_lida.especialidade);
            printf("----------------------------------------\n");
            encontrado = 1;
        }
    }
    fclose(arq_consultas);

    if(!encontrado){
        printf("Nenhuma consulta agendada no momento.\n");
    }
    press_enter_to_continue();
}

void relatorio_consultas_canceladas(void) {
    Consulta consulta_lida;
    FILE* arq_consultas;
    int encontrado = 0;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Consultas Canceladas       ///\n");
    printf("----------------------------------------\n");

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL) {
        printf("Nenhuma consulta no sistema.\n");
        press_enter_to_continue();
        return;
    }
    
    while (fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        // Mostra canceladas (ativo == 0) E as ativas com status "cancelada"
        if (strcmp(consulta_lida.status, "cancelada") == 0) {
            printf("Paciente: %s (CPF: %s)\n", consulta_lida.nome_paciente, consulta_lida.cpf_paciente);
            printf("Data: %s | Hora: %s\n", consulta_lida.data, consulta_lida.hora);
            printf("Medico: %s (%s)\n", consulta_lida.nome_medico, consulta_lida.especialidade);
            printf("----------------------------------------\n");
            encontrado = 1;
        }
    }
    fclose(arq_consultas);

    if(!encontrado){
        printf("Nenhuma consulta cancelada encontrada.\n");
    }
    press_enter_to_continue();
}

void gerenciar_agendamentos(void) {
    int opcao;
    do {
        limpar_tela(); 
        printf("----------------------------------------\n");
        printf("///   Gerenciar Agendamentos       ///\n");
        printf("----------------------------------------\n");
        printf("1. Alterar Data/Hora da Consulta\n");
        printf("2. Cancelar/Excluir Consulta\n");
        printf("3. Confirmar Presenca (Concluir)\n");
        printf("0. Voltar\n");
        printf("----------------------------------------\n");
        printf(">>> Escolha a opcao: ");
        
        if (scanf("%d", &opcao) != 1) { opcao = -1; }
        while (getchar() != '\n');

        switch (opcao) {
            case 1: alterar_consulta(); break;
            case 2: excluir_consulta(); break;
            case 3: confirmar_presenca(); break;
            case 0: break;
            default: printf("\nOpcao invalida.\n"); press_enter_to_continue(); break;
        }
    } while (opcao != 0);
}

void gerar_relatorios_consultas(void) {
    int opcao;
    do {
        limpar_tela(); 
        printf("----------------------------------------\n");
        printf("///     Gerar Relatorios           ///\n");
        printf("----------------------------------------\n");
        printf("1. Consultas por Medico\n");
        printf("2. Consultas por Periodo\n");
        printf("3. Consultas Agendadas\n");
        printf("4. Consultas Canceladas\n");
        printf("0. Voltar\n");
        printf("----------------------------------------\n");
        printf(">>> Escolha a opcao: ");
        
        if (scanf("%d", &opcao) != 1) { opcao = -1; }
        while (getchar() != '\n');

        switch (opcao) {
            case 1: relatorio_consultas_medico(); break;
            case 2: relatorio_consultas_por_periodo(); break;
            case 3: relatorio_consultas_agendadas(); break;
            case 4: relatorio_consultas_canceladas(); break;
            case 0: break;
            default: printf("\nOpcao invalida.\n"); press_enter_to_continue(); break;
        }
    } while (opcao != 0);
}

void modulo_consultas(void) {
    int opcao;
    criar_pasta_data();

    do {
        TelaMenuConsultas(); // Função de UI do utils.c
        if (scanf("%d", &opcao) != 1) { 
            opcao = -1; 
        }
        while (getchar() != '\n');

        switch (opcao) {
            case 1: agendar_consulta(); break;
            case 2: pesquisar_consulta(); break;
            case 3: gerenciar_agendamentos(); break;
            case 4: gerar_relatorios_consultas(); break;
            case 0: break;
            default: 
                printf("\nOpcao invalida.\n"); 
                press_enter_to_continue(); 
                break;
        }
    } while (opcao != 0);
}
