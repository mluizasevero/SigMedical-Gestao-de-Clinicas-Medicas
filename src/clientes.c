#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "utils.h" 

// Definição de CLIENTES FILE fora de utils.h
#define CLIENTES_FILE DATA_DIR PATH_SEPARATOR "clientes.dat"

// ele salva um único cliente no final do arquivo binário
void cadastrar_cliente(void) {
    Cliente novo_cliente;
    FILE* arq_clientes;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Cadastrar Novo Cliente       ///\n");
    printf("----------------------------------------\n");

    printf("\nInforme o CPF do cliente (apenas numeros): ");
    scanf("%14s", novo_cliente.cpf);
    while (getchar() != '\n');

    printf("Informe o nome do cliente: ");
    scanf(" %49[^\n]", novo_cliente.nome);
    while (getchar() != '\n');

    printf("Informe o telefone do cliente: ");
    scanf("%14s", novo_cliente.telefone);
    while (getchar() != '\n');

    printf("Informe o email do cliente: ");
    scanf("%49s", novo_cliente.email);
    while (getchar() != '\n');

    novo_cliente.ativo = 1; // define um cliente novo como "ativo" (existente)

    // abre o arquivo em modo "ab" (append binary) ---> é add no final
    arq_clientes = fopen(CLIENTES_FILE, "ab");
    if (arq_clientes == NULL) {
        printf("\nErro fatal ao abrir o arquivo para escrita!\n");
        press_enter_to_continue();
        return;
    }

    // escreve a struct inteira
    fwrite(&novo_cliente, sizeof(Cliente), 1, arq_clientes);
    fclose(arq_clientes);

    printf("\nCliente cadastrado com sucesso!\n");
    press_enter_to_continue();
}

// lê o arquivo de forma sequencial para encontrar um cliente
void pesquisar_cliente(void) {
    char cpf_pesquisa[15];
    int encontrado = 0;
    Cliente cliente_lido;
    FILE* arq_clientes;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///        Pesquisar Cliente         ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do cliente para pesquisa: ");
    scanf("%14s", cpf_pesquisa);
    while (getchar() != '\n');

    // abre o arquivo em read binary
    arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL) {
        printf("\nNenhum cliente cadastrado ou erro ao abrir o arquivo.\n");
        press_enter_to_continue();
        return;
    }

    // lê o arquivo struct por struct
    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes)) {
        // compara o CPF lido com o CPF pesquisado + verifica se o registro está ativo
        if (strcmp(cliente_lido.cpf, cpf_pesquisa) == 0 && cliente_lido.ativo == 1) {
            printf("\nCliente encontrado:\n");
            printf("CPF: %s\n", cliente_lido.cpf);
            printf("Nome: %s\n", cliente_lido.nome);
            printf("Telefone: %s\n", cliente_lido.telefone);
            printf("Email: %s\n", cliente_lido.email);
            encontrado = 1;
            break; // para o loop assim que encontrar
        }
    }
    fclose(arq_clientes);

    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_pesquisa);
    }
    press_enter_to_continue();
}

// encontra e altera um registro no arquivo.
void alterar_cliente(void) {
    char cpf_alteracao[15];
    int encontrado = 0;
    Cliente cliente_lido;
    FILE* arq_clientes;
    long int pos;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Alterar Dados do Cliente     ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do cliente que deseja alterar: ");
    scanf("%14s", cpf_alteracao);
    while (getchar() != '\n');

    // abre leitura e escrita binária
    arq_clientes = fopen(CLIENTES_FILE, "r+b");
    if (arq_clientes == NULL) {
        printf("\nErro ao abrir arquivo. Nenhum cliente cadastrado?\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes)) {
        if (strcmp(cliente_lido.cpf, cpf_alteracao) == 0 && cliente_lido.ativo == 1) {
            // guarda a posição do início do registro
            pos = ftell(arq_clientes) - sizeof(Cliente);

            printf("\nCliente encontrado. Informe os novos dados:\n");
            printf("Nome atual: %s\nNovo Nome: ", cliente_lido.nome);
            scanf(" %49[^\n]", cliente_lido.nome);
            while (getchar() != '\n');

            printf("Telefone atual: %s\nNovo Telefone: ", cliente_lido.telefone);
            scanf("%14s", cliente_lido.telefone);
            while (getchar() != '\n');

            printf("Email atual: %s\nNovo Email: ", cliente_lido.email);
            scanf("%49s", cliente_lido.email);
            while (getchar() != '\n');

            
            fseek(arq_clientes, pos, SEEK_SET);
            // sobrescreve o registro antigo com os dados atualizados
            fwrite(&cliente_lido, sizeof(Cliente), 1, arq_clientes);
            
            encontrado = 1;
            printf("\nCliente alterado com sucesso!\n");
            break;
        }
    }
    fclose(arq_clientes);

    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_alteracao);
    }
    press_enter_to_continue();
}

// encontra e marca um registro como inativo
void excluir_cliente(void) {
    char cpf_exclusao[15];
    int encontrado = 0;
    Cliente cliente_lido;
    FILE* arq_clientes;
    long int pos;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Excluir Cliente          ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do cliente que deseja excluir: ");
    scanf("%14s", cpf_exclusao);
    while (getchar() != '\n');

    arq_clientes = fopen(CLIENTES_FILE, "r+b");
    if (arq_clientes == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes)) {
        if (strcmp(cliente_lido.cpf, cpf_exclusao) == 0 && cliente_lido.ativo == 1) {
            pos = ftell(arq_clientes) - sizeof(Cliente);
            
            // apenas muda o status para 0 (inativo)
            cliente_lido.ativo = 0;

            fseek(arq_clientes, pos, SEEK_SET);
            fwrite(&cliente_lido, sizeof(Cliente), 1, arq_clientes);
            
            encontrado = 1;
            printf("\nCliente excluido com sucesso!\n");
            break;
        }
    }
    fclose(arq_clientes);

    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_exclusao);
    }
    press_enter_to_continue();
}

// mostra todos os clientes ativos no arquivo.
void listar_clientes(void) {
    FILE* arq_clientes;
    Cliente cliente_lido;
    int nenhum_cliente = 1;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///       Listagem de Clientes       ///\n");
    printf("----------------------------------------\n");

    arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL) {
        printf("\nNenhum cliente cadastrado.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes)) {
        if (cliente_lido.ativo == 1) {
            printf("Nome: %s\n", cliente_lido.nome);
            printf("CPF: %s\n", cliente_lido.cpf);
            printf("Telefone: %s\n", cliente_lido.telefone);
            printf("Email: %s\n", cliente_lido.email);
            printf("----------------------------------------\n");
            nenhum_cliente = 0; 
        }
    }
    fclose(arq_clientes);

    if (nenhum_cliente) {
        printf("\nNenhum cliente ativo para exibir.\n");
    }
    press_enter_to_continue();
}


// Módulo principal
void modulo_clientes(void) {
    int opcao;
    
    // garante que a pasta ./data exista antes de qualquer operação
    criar_pasta_data();

    do {
        limpar_tela();
        printf("----------------------------------------\n");
        printf("///      Modulo de Clientes        ///\n");
        printf("----------------------------------------\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Pesquisar Cliente\n");
        printf("3. Alterar Cliente\n");
        printf("4. Excluir Cliente\n");
        printf("5. Listar Clientes\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
        }
        while (getchar() != '\n'); 

        switch (opcao) {
            case 1:
                cadastrar_cliente();
                break;
            case 2:
                pesquisar_cliente();
                break;
            case 3:
                alterar_cliente();
                break;
            case 4:
                excluir_cliente();
                break;
            case 5:
                listar_clientes();
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
