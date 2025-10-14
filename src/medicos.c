#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicos.h"
#include "utils.h"

#ifdef _WIN32
    #include <direct.h>
    #define PATH_SEPARATOR "\\"
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define PATH_SEPARATOR "/"
#endif

#define DATA_DIR "data"
#define MEDICOS_FILE DATA_DIR PATH_SEPARATOR "medicos.dat"


void criar_pasta_data_se_nao_existir() {
    #ifdef _WIN32
        _mkdir(DATA_DIR);
    #else
        mkdir(DATA_DIR, 0777);
    #endif
}



// cadastra um novo médico salva diretamente no arquivo binário

void cadastrar_medico(void) {
    Medico novo_medico;
    FILE* arq_medicos;

    limpar_tela(); // Assumindo TelaCadastrarMedicos()
    printf("----------------------------------------\n");
    printf("///      Cadastrar Novo Medico       ///\n");
    printf("----------------------------------------\n");

    printf("\nInforme o ID do medico: ");
    scanf("%d", &novo_medico.id);
    while (getchar() != '\n');

    printf("Informe o nome completo: ");
    scanf(" %49[^\n]", novo_medico.nome);
    while (getchar() != '\n');

    printf("Informe o CPF (apenas numeros): ");
    scanf("%14s", novo_medico.cpf);
    while (getchar() != '\n');

    printf("Informe a especialidade: ");
    scanf(" %49[^\n]", novo_medico.especialidade);
    while (getchar() != '\n');

    printf("Informe o telefone: ");
    scanf("%14s", novo_medico.telefone);
    while (getchar() != '\n');

    novo_medico.ativo = 1;

    arq_medicos = fopen(MEDICOS_FILE, "ab");
    if (arq_medicos == NULL) {
        printf("\nErro ao abrir o arquivo de medicos!\n");
        press_enter_to_continue();
        return;
    }

    fwrite(&novo_medico, sizeof(Medico), 1, arq_medicos);
    fclose(arq_medicos);

    printf("\nMedico cadastrado com sucesso!\n");
    press_enter_to_continue();
}

// pesquisa pelo médico

void pesquisar_medico(void) {
    char cpf_busca[15];
    int encontrado = 0;
    Medico medico_lido;
    FILE* arq_medicos;

    limpar_tela(); 
    printf("----------------------------------------\n");
    printf("///     Pesquisar Medico por CPF     ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do medico a ser pesquisado: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL) {
        printf("\nNenhum medico cadastrado ou erro ao abrir o arquivo.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos)) {
        if (strcmp(medico_lido.cpf, cpf_busca) == 0 && medico_lido.ativo == 1) {
            printf("\n--- Medico Encontrado ---\n");
            printf("ID: %d\n", medico_lido.id);
            printf("Nome: %s\n", medico_lido.nome);
            printf("CPF: %s\n", medico_lido.cpf);
            printf("Especialidade: %s\n", medico_lido.especialidade);
            printf("Telefone: %s\n", medico_lido.telefone);
            printf("---------------------------\n");
            encontrado = 1;
            break;
        }
    }
    fclose(arq_medicos);

    if (!encontrado) {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    press_enter_to_continue();
}

// altera os dados do médico

void alterar_medico(void) {
    char cpf_busca[15];
    int encontrado = 0;
    Medico medico_lido;
    FILE* arq_medicos;
    long int pos;

    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Alterar Dados de Medico      ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do medico que deseja alterar: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');

    arq_medicos = fopen(MEDICOS_FILE, "r+b");
    if (arq_medicos == NULL) {
        printf("\nErro ao abrir arquivo. Nenhum medico cadastrado?\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos)) {
        if (strcmp(medico_lido.cpf, cpf_busca) == 0 && medico_lido.ativo == 1) {
            pos = ftell(arq_medicos) - sizeof(Medico);

            printf("\nMedico encontrado. Informe os novos dados:\n");
            
            printf("Nome atual: %s\nNovo nome: ", medico_lido.nome);
            scanf(" %49[^\n]", medico_lido.nome);
            while (getchar() != '\n');

            printf("Especialidade atual: %s\nNova especialidade: ", medico_lido.especialidade);
            scanf(" %49[^\n]", medico_lido.especialidade);
            while (getchar() != '\n');

            printf("Telefone atual: %s\nNovo telefone: ", medico_lido.telefone);
            scanf("%14s", medico_lido.telefone);
            while (getchar() != '\n');

            fseek(arq_medicos, pos, SEEK_SET);
            fwrite(&medico_lido, sizeof(Medico), 1, arq_medicos);
            
            encontrado = 1;
            printf("\nDados do medico alterados com sucesso!\n");
            break;
        }
    }
    fclose(arq_medicos);

    if (!encontrado) {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    press_enter_to_continue();
}

// exlucão lógica de médicos

void excluir_medico(void) {
    char cpf_busca[15];
    int encontrado = 0;
    Medico medico_lido;
    FILE* arq_medicos;
    long int pos;

    limpar_tela(); 
    printf("----------------------------------------\n");
    printf("///          Excluir Medico          ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do medico que deseja excluir: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n');

    arq_medicos = fopen(MEDICOS_FILE, "r+b");
    if (arq_medicos == NULL) {
        printf("\nErro ao abrir o arquivo.\n");
        press_enter_to_continue();
        return;
    }

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos)) {
        if (strcmp(medico_lido.cpf, cpf_busca) == 0 && medico_lido.ativo == 1) {
            pos = ftell(arq_medicos) - sizeof(Medico);

            medico_lido.ativo = 0;

            fseek(arq_medicos, pos, SEEK_SET);
            fwrite(&medico_lido, sizeof(Medico), 1, arq_medicos);

            encontrado = 1;
            printf("\nMedico excluido com sucesso!\n");
            break;
        }
    }
    fclose(arq_medicos);

    if (!encontrado) {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    press_enter_to_continue();
}

// lê o binário e exibe o arqv de médicos

void listar_medicos(void) {
    Medico medico_lido;
    FILE* arq_medicos;
    int tem_medico = 0;
    
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///       Listagem de Medicos        ///\n");
    printf("----------------------------------------\n");

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL) {
        printf("Nenhum medico cadastrado.\n");
        press_enter_to_continue();
        return;
    }

    printf("ID | Nome do Medico         | CPF           | Especialidade\n");
    printf("---|------------------------|---------------|------------------\n");
    
    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos)) {
        if (medico_lido.ativo == 1) {
            printf("%-2d | %-22s | %-13s | %s\n",
                   medico_lido.id, medico_lido.nome,
                   medico_lido.cpf, medico_lido.especialidade);
            tem_medico = 1;
        }
    }
    fclose(arq_medicos);

    if (!tem_medico) {
        printf("Nenhum medico ativo encontrado.\n");
    }
    press_enter_to_continue();
}

// menu do módulo médicos
void modulo_medicos(void) {
    int opcao;
    criar_pasta_data_se_nao_existir();

    do {
        limpar_tela(); 
        printf("----------------------------------------\n");
        printf("///        Modulo de Medicos         ///\n");
        printf("----------------------------------------\n");
        printf("1. Cadastrar Medico\n");
        printf("2. Pesquisar Medico\n");
        printf("3. Alterar Medico\n");
        printf("4. Excluir Medico\n");
        printf("5. Listar Medicos\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        while (getchar() != '\n');

        switch (opcao) {
            case 1: cadastrar_medico(); break;
            case 2: pesquisar_medico(); break;
            case 3: alterar_medico(); break;
            case 4: excluir_medico(); break;
            case 5: listar_medicos(); break;
            case 0: break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao != 0);
}
