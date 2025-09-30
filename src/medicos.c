#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicos.h"
#include "utils.h"

#ifdef _WIN32
    #include <direct.h> 
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

#define DATA_DIR "data"
#define MEDICOS_FILE DATA_DIR PATH_SEPARATOR "medicos.csv"

int ler_medicos(Medico medicos[]) {
    FILE *arquivo = fopen(MEDICOS_FILE, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo 'medicos.csv' para leitura.\n");
        press_enter_to_continue();
        return 0;
    }

    char linha_cabecalho[256];
    fgets(linha_cabecalho, 256, arquivo); 

    int i = 0;
    while(fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%[^\n]\n",
                  &medicos[i].id, medicos[i].nome, medicos[i].cpf,
                  medicos[i].especialidade, medicos[i].telefone) == 5) {
        i++;
    }
    fclose(arquivo);
    return i;
}


void salvar_medicos(Medico medicos[], int total_medicos) {
    criar_pasta_data();
    FILE *arquivo = fopen(MEDICOS_FILE, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo 'medicos.csv' para escrita.\n");
        press_enter_to_continue();
        return;
    }

    int i = 0;
    while (i < total_medicos) {
        fprintf(arquivo, "%d,%s,%s,%s,%s\n",
                medicos[i].id, medicos[i].nome, medicos[i].cpf,
                medicos[i].especialidade, medicos[i].telefone);
        i++;
    }
    fclose(arquivo);
}


void cadastrar_medico(Medico medicos[], int* total_medicos) {
    TelaCadastrarMedicos(); // Função de UI
    if (*total_medicos < 100) {
        printf("\nInforme o ID do medico: ");
        scanf("%d", &medicos[*total_medicos].id);
        while (getchar() != '\n');

        printf("Informe o nome completo: ");
        scanf(" %49[^\n]", medicos[*total_medicos].nome);
        while (getchar() != '\n');

        printf("Informe o CPF (apenas numeros): ");
        scanf("%14s", medicos[*total_medicos].cpf);
        while (getchar() != '\n');

        printf("Informe a especialidade: ");
        scanf(" %49[^\n]", medicos[*total_medicos].especialidade);
        while (getchar() != '\n');

        printf("Informe o telefone: ");
        scanf("%14s", medicos[*total_medicos].telefone);
        while (getchar() != '\n');

        (*total_medicos)++;
        printf("\nMedico cadastrado com sucesso!\n");
    } else {
        printf("\nLimite de medicos atingido.\n");
    }
    press_enter_to_continue();
}


void pesquisar_medico(Medico medicos[], int total_medicos) {
    TelaPesquisarMedicos();
    char cpf_busca[15];
    int encontrado = 0;
    printf("Informe o CPF do medico a ser pesquisado: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');

    int i = 0;
    while (i < total_medicos) {
        if (strcmp(medicos[i].cpf, cpf_busca) == 0) {
            printf("\n--- Medico Encontrado ---\n");
            printf("ID: %d\n", medicos[i].id);
            printf("Nome: %s\n", medicos[i].nome);
            printf("CPF: %s\n", medicos[i].cpf);
            printf("Especialidade: %s\n", medicos[i].especialidade);
            printf("Telefone: %s\n", medicos[i].telefone);
            printf("---------------------------\n");
            encontrado = 1;
            break;
        }
        i++;
    }

    if (!encontrado) {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    press_enter_to_continue();
}


void alterar_medico(Medico medicos[], int total_medicos) {
    TelaAlterarMedicos();
    char cpf_busca[15];
    int encontrado = 0;
    printf("Informe o CPF do medico que deseja alterar: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');

    int i = 0;
    while (i < total_medicos) {
        if (strcmp(medicos[i].cpf, cpf_busca) == 0) {
            printf("\nMedico encontrado. Informe os novos dados:\n");
            
            printf("Nome atual: %s\nNovo nome: ", medicos[i].nome);
            scanf(" %49[^\n]", medicos[i].nome);
            while (getchar() != '\n');

            printf("Especialidade atual: %s\nNova especialidade: ", medicos[i].especialidade);
            scanf(" %49[^\n]", medicos[i].especialidade);
            while (getchar() != '\n');

            printf("Telefone atual: %s\nNovo telefone: ", medicos[i].telefone);
            scanf("%14s", medicos[i].telefone);
            while (getchar() != '\n');

            printf("\nDados alterados com sucesso!\n");
            encontrado = 1;
            break;
        }
        i++;
    }

    if (!encontrado) {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    press_enter_to_continue();
}


void excluir_medico(Medico medicos[], int* total_medicos) {
    TelaExcluirMedicos();
    char cpf_busca[15];
    int encontrado = 0;
    printf("Informe o CPF do medico que deseja excluir: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');

    int i = 0;
    while (i < *total_medicos) {
        if (strcmp(medicos[i].cpf, cpf_busca) == 0) {
            int j = i;
            while (j < *total_medicos - 1) {
                medicos[j] = medicos[j + 1];
                j++;
            }
            (*total_medicos)--;
            printf("\nMedico excluido com sucesso!\n");
            encontrado = 1;
            break;
        }
        i++;
    }

    if (!encontrado) {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    press_enter_to_continue();
}


void modulo_medicos(Medico medicos[], int* total_medicos) {
    int opcao;
    do {
        TelaMenuMedicos();
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                cadastrar_medico(medicos, total_medicos);
                salvar_medicos(medicos, *total_medicos);
                break;
            case 2:
                pesquisar_medico(medicos, *total_medicos);
                break;
            case 3:
                alterar_medico(medicos, *total_medicos);
                salvar_medicos(medicos, *total_medicos);
                break;
            case 4:
                excluir_medico(medicos, total_medicos);
                salvar_medicos(medicos, *total_medicos);
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
