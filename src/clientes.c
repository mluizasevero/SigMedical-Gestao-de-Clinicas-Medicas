#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "utils.h"
#include "validador.h" // MUDANÇA: Incluindo a nova biblioteca

// Definição de CLIENTES FILE fora de utils.h
#define CLIENTES_FILE DATA_DIR PATH_SEPARATOR "clientes.dat"

// Função interna para verificar se um CPF já existe
// Retorna 1 se já existe, 0 se não
int cpfJaCadastrado(const char* cpf) {
    FILE* arq = fopen(CLIENTES_FILE, "rb");
    if (arq == NULL) {
        return 0; // Arquivo não existe, então CPF não está cadastrado
    }
    
    Cliente c;
    while(fread(&c, sizeof(Cliente), 1, arq)) {
        if (strcmp(c.cpf, cpf) == 0 && c.ativo == 1) {
            fclose(arq);
            return 1; // Encontrou CPF ativo
        }
    }
    fclose(arq);
    return 0; // Não encontrou
}


// ele salva um único cliente no final do arquivo binário
void cadastrar_cliente(void) {
    Cliente novo_cliente;
    FILE* arq_clientes;
    
    // MUDANÇA: Buffers temporários para leitura e validação
    char buffer[51]; // 50 + 1 para o \0

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Cadastrar Novo Cliente       ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Loop de validação para CPF
    do {
        printf("\nInforme o CPF do cliente (apenas numeros ou com ./): ");
        lerString(buffer, 15); // Usa lerString (tamanho 15)
        if (!validarCPF(buffer)) {
            printf("! CPF invalido. Formato ou digitos verificadores incorretos. Tente novamente.\n");
        } else if (cpfJaCadastrado(buffer)) {
             printf("! CPF ja cadastrado no sistema. Tente novamente.\n");
        }
    } while (!validarCPF(buffer) || cpfJaCadastrado(buffer));
    strcpy(novo_cliente.cpf, buffer); // Copia o dado validado

    // MUDANÇA: Loop de validação para Nome
    do {
        printf("Informe o nome do cliente: ");
        lerString(buffer, 50); // Usa lerString (tamanho 50)
        // A função validarNome já imprime o erro se houver
    } while (!validarNome(buffer));
    strcpy(novo_cliente.nome, buffer); // Copia o dado validado

    // MUDANÇA: Loop de validação para Telefone
    do {
        printf("Informe o telefone do cliente (com DDD): ");
        lerString(buffer, 15); // Usa lerString (tamanho 15)
    } while (!validarTelefone(buffer));
    strcpy(novo_cliente.telefone, buffer); // Copia o dado validado

    // MUDANÇA: Loop de validação para Email
    do {
        printf("Informe o email do cliente: ");
        lerString(buffer, 50); // Usa lerString (tamanho 50)
    } while (!validarEmail(buffer));
    strcpy(novo_cliente.email, buffer); // Copia o dado validado


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
    printf("///       Pesquisar Cliente          ///\n");
    printf("----------------------------------------\n");
    
    // MUDANÇA: Validação do CPF de pesquisa (opcional, mas bom)
    do {
        printf("Informe o CPF do cliente para pesquisa: ");
        lerString(cpf_pesquisa, 15);
        if (!validarCPF(cpf_pesquisa)) {
             printf("! Formato de CPF invalido. Tente novamente.\n");
        }
    } while (!validarCPF(cpf_pesquisa));


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
    
    // MUDANÇA: Buffer para novos dados
    char buffer[51];

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Alterar Dados do Cliente     ///\n");
    printf("----------------------------------------\n");
    
    // MUDANÇA: Validação do CPF de alteração
    do {
        printf("Informe o CPF do cliente que deseja alterar: ");
        lerString(cpf_alteracao, 15);
         if (!validarCPF(cpf_alteracao)) {
             printf("! Formato de CPF invalido. Tente novamente.\n");
        }
    } while (!validarCPF(cpf_alteracao));


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
            
            // MUDANÇA: Loop de validação para Nome
            do {
                printf("Nome atual: %s\nNovo Nome: ", cliente_lido.nome);
                lerString(buffer, 50);
            } while (!validarNome(buffer));
            strcpy(cliente_lido.nome, buffer); // Copia

            // MUDANÇA: Loop de validação para Telefone
            do {
                printf("Telefone atual: %s\nNovo Telefone: ", cliente_lido.telefone);
                lerString(buffer, 15);
            } while (!validarTelefone(buffer));
            strcpy(cliente_lido.telefone, buffer); // Copia

            // MUDANÇA: Loop de validação para Email
            do {
                printf("Email atual: %s\nNovo Email: ", cliente_lido.email);
                lerString(buffer, 50);
            } while (!validarEmail(buffer));
            strcpy(cliente_lido.email, buffer); // Copia
            
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
    
    // MUDANÇA: Validação do CPF de exclusão
    do {
        printf("Informe o CPF do cliente que deseja excluir: ");
        lerString(cpf_exclusao, 15);
         if (!validarCPF(cpf_exclusao)) {
             printf("! Formato de CPF invalido. Tente novamente.\n");
        }
    } while (!validarCPF(cpf_exclusao));


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
    char bufferOpcao[5]; // MUDANÇA: Buffer para ler a opção

    // garante que a pasta ./data exista antes de qualquer operação
    criar_pasta_data();

    do {
        limpar_tela();
        printf("----------------------------------------\n");
        printf("///       Modulo de Clientes         ///\n");
        printf("----------------------------------------\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Pesquisar Cliente\n");
        printf("3. Alterar Cliente\n");
        printf("4. Excluir Cliente\n");
        printf("5. Listar Clientes\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------------\n");
        

        // MUDANÇA: Leitura e validação da opção de menu
        do {
            printf("Escolha uma opcao: ");
            lerString(bufferOpcao, 5); // Lê como string
            
            // Tenta converter string para long int
            char* endptr;
            opcao = strtol(bufferOpcao, &endptr, 10);

            // Verifica se a conversão falhou (ex: "abc") ou se não era um número inteiro ("1.5")
            // ou se a opção está fora do intervalo
            if (endptr == bufferOpcao || *endptr != '\0') {
                opcao = -1; // Seta uma opção inválida para forçar o erro
            }
        
        } while (!validarOpcaoMenu(opcao, 0, 5)); // Valida o intervalo


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
            // MUDANÇA: Default não é mais necessário para "opção inválida"
            // pois o loop 'do-while' já trata isso.
        }
    } while (opcao != 0);
}
