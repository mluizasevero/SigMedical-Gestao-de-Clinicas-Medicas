#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "utils.h"

#ifdef _WIN32
    #include <direct.h> 
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

#define DATA_DIR "data"
#define CLIENTES_FILE DATA_DIR PATH_SEPARATOR "clientes.dat"

void cadastrar_cliente(Cliente clientes[], int* total_clientes) {
    if (*total_clientes < 100) {
        printf("\nInforme o CPF do cliente (apenas numeros): ");
        scanf("%11s", clientes[*total_clientes].cpf);
        while (getchar() != '\n');

        printf("Informe o nome do cliente: ");
        scanf(" %49[^\n]", clientes[*total_clientes].nome);
        while (getchar() != '\n');

        printf("Informe o telefone do cliente: ");
        scanf("%14s", clientes[*total_clientes].telefone);
        while (getchar() != '\n');

        printf("Informe o email do cliente: ");
        scanf("%49s", clientes[*total_clientes].email);
        while (getchar() != '\n');

        (*total_clientes)++;
        printf("\nCliente cadastrado com sucesso!\n");
    } else {
        printf("\nNao e possivel cadastrar mais clientes. Limite atingido.\n");
    }
    press_enter_to_continue();
}

void pesquisar_cliente(Cliente clientes[], int total_clientes) {
    char cpf_pesquisa[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Pesquisar Cliente   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do cliente para pesquisa: ");
    scanf("%11s", cpf_pesquisa);
    while (getchar() != '\n');

    for (int i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_pesquisa) == 0) {
            printf("\nCliente encontrado:\n");
            printf("CPF: %s\n", clientes[i].cpf);
            printf("Nome: %s\n", clientes[i].nome);
            printf("Telefone: %s\n", clientes[i].telefone);
            printf("Email: %s\n", clientes[i].email);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_pesquisa);
    }
    press_enter_to_continue();
}

void alterar_cliente(Cliente clientes[], int total_clientes) {
    char cpf_alteracao[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Alterar Dados do Cliente   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do cliente que deseja alterar: ");
    scanf("%11s", cpf_alteracao);
    while (getchar() != '\n');

    for (int i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_alteracao) == 0) {
            printf("\nCliente encontrado. Informe os novos dados:\n");
            printf("Nome atual: %s\n", clientes[i].nome);
            printf("Novo Nome: ");
            scanf(" %49[^\n]", clientes[i].nome);
            while (getchar() != '\n');

            printf("Telefone atual: %s\n", clientes[i].telefone);
            printf("Novo Telefone: ");
            scanf("%14s", clientes[i].telefone);
            while (getchar() != '\n');

            printf("Email atual: %s\n", clientes[i].email);
            printf("Novo Email: ");
            scanf("%49s", clientes[i].email);
            while (getchar() != '\n');
            
            printf("\nCliente alterado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_alteracao);
    }
    press_enter_to_continue();
}

void excluir_cliente(Cliente clientes[], int* total_clientes) {
    char cpf_exclusao[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///   Excluir Cliente   ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do cliente que deseja excluir: ");
    scanf("%11s", cpf_exclusao);
    while (getchar() != '\n');

    for (int i = 0; i < *total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_exclusao) == 0) {
            for (int j = i; j < *total_clientes - 1; j++) {
                clientes[j] = clientes[j + 1];
            }
            (*total_clientes)--;
            printf("\nCliente excluido com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_exclusao);
    }
    press_enter_to_continue();
}

void salvar_clientes(Cliente clientes[], int total_clientes) {
    FILE *arq_clientes = fopen(CLIENTES_FILE, "w");
    if (arq_clientes == NULL) {
        printf("Erro ao abrir o arquivo 'clientes.dat' para escrita.\n");
        return;
    }
    for (int i = 0; i < total_clientes; i++) {
        fprintf(arq_clientes, "%s;%s;%s;%s\n",
                clientes[i].cpf, clientes[i].nome,
                clientes[i].telefone, clientes[i].email);
    }
    fclose(arq_clientes);
}

int ler_clientes(Cliente clientes[]) {
    FILE *arq_clientes = fopen(CLIENTES_FILE, "r");
    if (arq_clientes == NULL) {
        return 0;
    }
    int i = 0;
    while(fscanf(arq_clientes, "%[^;];%[^;];%[^;];%[^\n]\n",
                  clientes[i].cpf, clientes[i].nome,
                  clientes[i].telefone, clientes[i].email) == 4) {
        i++;
    }
    fclose(arq_clientes);
    return i;
}

void modulo_clientes(Cliente clientes[], int* total_clientes) {
    int opcao;
    do {
        TelaMenuClientes();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                cadastrar_cliente(clientes, total_clientes);
                break;
            case 2:
                pesquisar_cliente(clientes, *total_clientes);
                break;
            case 3:
                alterar_cliente(clientes, *total_clientes);
                break;
            case 4:
                excluir_cliente(clientes, total_clientes);
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

