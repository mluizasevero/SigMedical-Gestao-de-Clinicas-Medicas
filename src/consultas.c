#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultas.h"
#include "utils.h"
#include "validador.h" // MUDANÇA: Incluindo a biblioteca

#define CONSULTAS_FILE DATA_DIR PATH_SEPARATOR "consultas.dat"

void agendar_consulta(void) {
    Consulta nova_consulta;
    FILE* arq_consultas;
    
    // MUDANÇA: Buffer para leitura segura
    char buffer[51];

    limpar_tela(); 
    printf("----------------------------------------\n");
    printf("///    Agendar Nova Consulta         ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Loop de validação para Nome do Paciente
    do {
        printf("\nInforme o nome do paciente: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer));
    strcpy(nova_consulta.nome_paciente, buffer);

    // MUDANÇA: Loop de validação para CPF
    do {
        printf("Informe o CPF do paciente: ");
        lerString(buffer, 15);
        if (!validarCPF(buffer)) {
             printf("! CPF invalido. Tente novamente.\n");
        }
    } while (!validarCPF(buffer));
    strcpy(nova_consulta.cpf_paciente, buffer);

    // MUDANÇA: Loop de validação para Data
    do {
        printf("Informe a data da consulta (dd/mm/aaaa): ");
        lerString(buffer, 11);
    } while (!validarData(buffer));
    strcpy(nova_consulta.data, buffer);

    // MUDANÇA: Loop de validação para Hora
    do {
        printf("Informe a hora da consulta (hh:mm): ");
        lerString(buffer, 6);
    } while (!validarHora(buffer));
    strcpy(nova_consulta.hora, buffer);

    // MUDANÇA: Loop de validação para Nome do Médico
    do {
        printf("Informe o nome do medico: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer));
    strcpy(nova_consulta.nome_medico, buffer);

    // MUDANÇA: Loop de validação para Especialidade (usando validarNome)
    do {
        printf("Informe a especialidade do medico: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer)); // Reutilizando validarNome (mín 3 letras, sem num)
    strcpy(nova_consulta.especialidade, buffer);

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
    printf("///       Pesquisar Consulta         ///\n");
    printf("----------------------------------------\n");
    
    // MUDANÇA: Validação do CPF
    do {
        printf("Informe o CPF do paciente: ");
        lerString(cpf_pesquisa, 15);
        if (!validarCPF(cpf_pesquisa)) {
             printf("! Formato de CPF invalido. Tente novamente.\n");
        }
    } while (!validarCPF(cpf_pesquisa));

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
    
    // MUDANÇA: Buffer para leitura
    char buffer[51];

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///       Alterar Consulta           ///\n");
    printf("----------------------------------------\n");
    
    // MUDANÇA: Validação do CPF
    do {
        printf("Informe o CPF do paciente: ");
        lerString(cpf_busca, 15);
    } while (!validarCPF(cpf_busca));
    
    // MUDANÇA: Validação da Data
    do {
        printf("Informe a DATA da consulta a alterar (dd/mm/aaaa): ");
        lerString(data_busca, 11);
    } while (!validarData(data_busca));


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
            
            // MUDANÇA: Validação da Nova Data
            do {
                printf("Data atual: %s\nNova Data (dd/mm/aaaa): ", consulta_lida.data);
                lerString(buffer, 11);
            } while (!validarData(buffer));
            strcpy(consulta_lida.data, buffer);
            
            // MUDANÇA: Validação da Nova Hora
            do {
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
    
    // MUDANÇA: Validação do CPF
    do {
        printf("Informe o CPF do paciente: ");
        lerString(cpf_busca, 15);
    } while (!validarCPF(cpf_busca));
    
    // MUDANÇA: Validação da Data
    do {
        printf("Informe a DATA da consulta a excluir (dd/mm/aaaa): ");
        lerString(data_busca, 11);
    } while (!validarData(data_busca));

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
            char bufferConfirm[5]; // MUDANÇA: Buffer para confirmação
            
            printf("\nConsulta encontrada:\n");
            printf("Paciente: %s\nData: %s | Hora: %s\n", consulta_lida.nome_paciente, consulta_lida.data, consulta_lida.hora);
            
            // MUDANÇA: Loop de confirmação
            do {
                printf("Deseja realmente cancelar esta consulta (S/N)? ");
                lerString(bufferConfirm, 5);
                confirmacao = toupper(bufferConfirm[0]); // Pega o primeiro caractere
                if (confirmacao != 'S' && confirmacao != 'N') {
                    printf("! Opcao invalida. Digite 'S' para Sim ou 'N' para Nao.\n");
                }
            } while (confirmacao != 'S' && confirmacao != 'N');


            if (confirmacao == 'S') {
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
    printf("///     Confirmar Presenca           ///\n");
    printf("----------------------------------------\n");
    
    // MUDANÇA: Validação do CPF
    do {
        printf("Informe o CPF do paciente: ");
        lerString(cpf_busca, 15);
    } while (!validarCPF(cpf_busca));
    
    // MUDANÇA: Validação da Data
    do {
        printf("Informe a data da consulta (dd/mm/aaaa): ");
        lerString(data_busca, 11);
    } while (!validarData(data_busca));

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

void relatorio_consultas_medico(void) {
    char nome_medico_pesquisa[50];
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Consultas por Medico         ///\n");
    printf("----------------------------------------\n");
    
    // MUDANÇA: Validação do nome
    do {
        printf("Informe o nome do medico: ");
        lerString(nome_medico_pesquisa, 50);
    } while (!validarNome(nome_medico_pesquisa));

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
    int encontrado = 0;
    Consulta consulta_lida;
    FILE* arq_consultas;
    
    // MUDANÇA: Variáveis para datas convertidas
    long data_inicio_int, data_fim_int, data_consulta_int;
    
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Consultas por Periodo        ///\n");
    printf("----------------------------------------\n");
    
    // MUDANÇA: Validação das datas
    do {
        printf("Informe a data de inicio (dd/mm/aaaa): ");
        lerString(data_inicio_str, 11);
    } while (!validarData(data_inicio_str));
    
    do {
        printf("Informe a data de fim (dd/mm/aaaa): ");
        lerString(data_fim_str, 11);
    } while (!validarData(data_fim_str));
    
    // MUDANÇA: Converte datas para inteiros para comparação correta
    data_inicio_int = converterDataParaInt(data_inicio_str);
    data_fim_int = converterDataParaInt(data_fim_str);
    
    if (data_fim_int < data_inicio_int) {
        printf("\n! Erro: A data final nao pode ser anterior a data inicial.\n");
        press_enter_to_continue();
        return;
    }

    printf("\nRelatorio de Consultas no Periodo de %s a %s:\n", data_inicio_str, data_fim_str);

    arq_consultas = fopen(CONSULTAS_FILE, "rb");
    if (arq_consultas == NULL) {
        printf("Nenhuma consulta no sistema.\n");
        press_enter_to_continue();
        return;
    }

    while(fread(&consulta_lida, sizeof(Consulta), 1, arq_consultas)) {
        if (consulta_lida.ativo == 1) {
            
            // MUDANÇA: Comparação numérica de datas (YYYYMMDD)
            data_consulta_int = converterDataParaInt(consulta_lida.data);
            
            if (data_consulta_int >= data_inicio_int && data_consulta_int <= data_fim_int) {
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

// ... (Funções relatorio_consultas_agendadas e relatorio_consultas_canceladas
//     não recebem input, então não precisam de mudanças, exceto pela
//     checagem de status em relatorio_consultas_canceladas que está correta) ...

void gerenciar_agendamentos(void) {
    int opcao;
    char bufferOpcao[5]; // MUDANÇA: Buffer de leitura

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
        
        // MUDANÇA: Leitura de menu segura
        do {
            printf(">>> Escolha a opcao: ");
            lerString(bufferOpcao, 5);
            char* endptr;
            opcao = strtol(bufferOpcao, &endptr, 10);
            if (endptr == bufferOpcao || *endptr != '\0') {
                opcao = -1;
            }
        } while (!validarOpcaoMenu(opcao, 0, 3));


        switch (opcao) {
            case 1: alterar_consulta(); break;
            case 2: excluir_consulta(); break;
            case 3: confirmar_presenca(); break;
            case 0: break;
            // Default não é mais necessário
        }
    } while (opcao != 0);
}

void gerar_relatorios_consultas(void) {
    int opcao;
    char bufferOpcao[5]; // MUDANÇA: Buffer de leitura

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
        
        // MUDANÇA: Leitura de menu segura
        do {
            printf(">>> Escolha a opcao: ");
            lerString(bufferOpcao, 5);
            char* endptr;
            opcao = strtol(bufferOpcao, &endptr, 10);
            if (endptr == bufferOpcao || *endptr != '\0') {
                opcao = -1;
            }
        } while (!validarOpcaoMenu(opcao, 0, 4));

        switch (opcao) {
            case 1: relatorio_consultas_medico(); break;
            case 2: relatorio_consultas_por_periodo(); break;
            case 3: relatorio_consultas_agendadas(); break;
            case 4: relatorio_consultas_canceladas(); break;
            case 0: break;
        }
    } while (opcao != 0);
}

void modulo_consultas(void) {
    int opcao;
    char bufferOpcao[5]; // MUDANÇA: Buffer de leitura
    
    criar_pasta_data();

    do {
        // MUDANÇA: Substituí TelaMenuConsultas() (de utils.c) pelo
        //          menu inline para consistência com modulo_clientes.c
        limpar_tela();
        printf("----------------------------------------\n");
        printf("///       Modulo de Consultas        ///\n");
        printf("----------------------------------------\n");
        printf("1. Agendar Consulta\n");
        printf("2. Pesquisar Consultas do Paciente\n");
        printf("3. Gerenciar Agendamentos\n");
        printf("4. Gerar Relatorios\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------------\n");
        
        // MUDANÇA: Leitura de menu segura
        do {
            printf(">>> Escolha a opcao: ");
            lerString(bufferOpcao, 5);
            char* endptr;
            opcao = strtol(bufferOpcao, &endptr, 10);
            if (endptr == bufferOpcao || *endptr != '\0') {
                opcao = -1;
            }
        } while (!validarOpcaoMenu(opcao, 0, 4));


        switch (opcao) {
            case 1: agendar_consulta(); break;
            case 2: pesquisar_consulta(); break;
            case 3: gerenciar_agendamentos(); break;
            case 4: gerar_relatorios_consultas(); break;
            case 0: break;
        }
    } while (opcao != 0);
