#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicos.h"
#include "utils.h"
#include "validador.h"
#include "relatorios.h"

#define MEDICOS_FILE DATA_DIR PATH_SEPARATOR "medicos.dat"

/**
 * Verifica se um médico com o CPF fornecido ou ID (se o ID for checado) já existe no arquivo.
 * O 'id_a_ignorar' é usado na função alterar, para ignorar o médico que está sendo alterado.
 */
int medico_existe(const char *cpf, int id_a_ignorar)
{
    Medico medico_lido;
    FILE *arq_medicos = fopen(MEDICOS_FILE, "rb");

    if (arq_medicos == NULL)
    {
        return 0; // Se o arquivo não existe, o CPF/ID é único
    }

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (medico_lido.ativo == 1)
        {
            // Checa CPF
            if (strcmp(medico_lido.cpf, cpf) == 0)
            {
                if (medico_lido.id != id_a_ignorar)
                {
                    fclose(arq_medicos);
                    return 1; // CPF encontrado (e não é o médico que estamos alterando)
                }
            }
            // Checa ID (usado apenas no cadastro para garantir unicidade do ID)
            if (id_a_ignorar == 0 && medico_lido.id == id_a_ignorar)
            {
                fclose(arq_medicos);
                return 2; // ID encontrado (código 2 para diferenciar ID/CPF)
            }
        }
    }
    fclose(arq_medicos);
    return 0; // Não encontrado
}

// Estrutura para lista dinâmica
typedef struct No {
    Medico medico;
    struct No *proximo;
} No;

// Função para criar um novo nó
No* criar_no(Medico medico) {
    No *novo_no = (No *)malloc(sizeof(No));
    if (novo_no != NULL) {
        novo_no->medico = medico;
        novo_no->proximo = NULL;
    }
    return novo_no;
}

// Função para adicionar um médico à lista
void adicionar_no(No **cabeca, Medico medico) {
    No *novo_no = criar_no(medico);
    if (*cabeca == NULL) {
        *cabeca = novo_no;
    } else {
        No *temp = *cabeca;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = novo_no;
    }
}

// Função para liberar a memória da lista
void liberar_lista_medicos(No *cabeca) {
    No *temp;
    while (cabeca != NULL) {
        temp = cabeca;
        cabeca = cabeca->proximo;
        free(temp);
    }
}

// Função para carregar médicos do arquivo para a lista dinâmica
No* carregar_medicos_para_lista() {
    FILE *arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL) {
        return NULL;
    }

    No *cabeca = NULL;
    Medico medico_lido;
    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos)) {
        adicionar_no(&cabeca, medico_lido);
    }

    fclose(arq_medicos);
    return cabeca;
}

// ----------
// CADASTRO |
// ----------
void cadastrar_medico(void)
{
    Medico novo_medico;
    FILE *arq_medicos;
    char buffer[51];
    int id_temp;

    limparTela();
    printf("╔══════════════════════════════════════╗\n");
    printf("║        Cadastrar Novo Medico         ║\n");
    printf("╚══════════════════════════════════════╝\n");


    // Validação de ID único e positivo
    do
    {
        printf("\nInforme o ID do medico (inteiro positivo): ");
        lerString(buffer, 5);
        id_temp = validarInteiroPositivo(buffer);

        if (id_temp > 0 && medico_existe("", id_temp) == 2)
        { // Checa apenas a unicidade do ID
            printf("! Erro: ID %d ja cadastrado ou ativo. Use outro ID.\n", id_temp);
            id_temp = -1;
        }
    } while (id_temp <= 0);
    novo_medico.id = id_temp;

    // Validação do Nome
    do
    {
        printf("Informe o nome completo: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer));
    strcpy(novo_medico.nome, buffer);

    // Validação e Unicidade do CPF
    do
    {
        printf("Informe o CPF (apenas numeros, 11 digitos): ");
        lerString(buffer, 15);
        if (!validarCPF(buffer))
        { 
            continue;
        }
        if (medico_existe(buffer, -1))
        { // -1 para não ignorar nenhum ID
            printf("! Erro: CPF ja cadastrado.\n");
        }
    } while (!validarCPF(buffer) || medico_existe(buffer, -1));
    strcpy(novo_medico.cpf, buffer);

    // Validação da Especialidade
    do
    {
        printf("Informe a especialidade: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer)); // Reutilizando validarNome
    strcpy(novo_medico.especialidade, buffer);

    // Validação do Telefone
    do
    {
        printf("Informe o telefone (DDNNNNNNNN): ");
        lerString(buffer, 15);
    } while (!validarTelefone(buffer));
    strcpy(novo_medico.telefone, buffer);

    novo_medico.ativo = 1;

    arq_medicos = fopen(MEDICOS_FILE, "ab");
    if (arq_medicos == NULL)
    {
        printf("\nErro ao abrir o arquivo de medicos!\n");
        pressioneEnterParaContinuar();
        return;
    }

    fwrite(&novo_medico, sizeof(Medico), 1, arq_medicos);
    fclose(arq_medicos);

    printf("\nMedico cadastrado com sucesso!\n");
    pressioneEnterParaContinuar();
}

// -----------
// PESQUISA  |
// -----------
void pesquisar_medico(void)
{
    char cpf_busca[15];
    int encontrado = 0;
    Medico medico_lido;
    FILE *arq_medicos;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║        Pesquisar Medico por CPF        ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("Informe o CPF do medico a ser pesquisado: ");
    lerString(cpf_busca, 15);

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL)
    {
        printf("\nNenhum medico cadastrado ou erro ao abrir o arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (strcmp(medico_lido.cpf, cpf_busca) == 0 && medico_lido.ativo == 1)
        {
            printf("\n═══ Medico Encontrado ═══\n");
            printf("ID: %d\n", medico_lido.id);
            printf("Nome: %s\n", medico_lido.nome);
            printf("CPF: %s\n", medico_lido.cpf);
            printf("Especialidade: %s\n", medico_lido.especialidade);
            printf("Telefone: %s\n", medico_lido.telefone);
            printf("══════════════════════════\n");
            encontrado = 1;
            break;
        }
    }
    fclose(arq_medicos);

    if (!encontrado)
    {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    pressioneEnterParaContinuar();
}

// ------------
// ALTERAÇÃO  |
// ------------
void alterar_medico(void)
{
    char cpf_busca[15];
    int encontrado = 0;
    Medico medico_lido;
    FILE *arq_medicos;
    long int pos;
    char buffer[51];

    limparTela();
    printf("╔══════════════════════════════════════╗\n");
    printf("║       Alterar Dados de Medico        ║\n");
    printf("╚══════════════════════════════════════╝\n");


    // Validação do CPF de busca
    do
    {
        printf("Informe o CPF do medico que deseja alterar: ");
        lerString(cpf_busca, 15);
    } while (!validarCPF(cpf_busca));

    arq_medicos = fopen(MEDICOS_FILE, "r+b");
    if (arq_medicos == NULL)
    {
        printf("\nErro ao abrir arquivo. Nenhum medico cadastrado?\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (strcmp(medico_lido.cpf, cpf_busca) == 0 && medico_lido.ativo == 1)
        {
            pos = ftell(arq_medicos) - sizeof(Medico);

            printf("\nMedico encontrado. Informe os novos dados (deixe em branco para manter o atual):\n");

            // Validação do Nome (e mantém o antigo se vazio)
            do
            {
                printf("Nome atual: %s\nNovo nome: ", medico_lido.nome);
                lerString(buffer, 50);
                if (strlen(buffer) > 0 && validarNome(buffer))
                {
                    strcpy(medico_lido.nome, buffer);
                    break;
                }
                else if (strlen(buffer) == 0)
                { // Se vazio, mantém o nome atual
                    break;
                }
            } while (1);

            // Validação da Especialidade (e mantém o antigo se vazio)
            do
            {
                printf("Especialidade atual: %s\nNova especialidade: ", medico_lido.especialidade);
                lerString(buffer, 50);
                if (strlen(buffer) > 0 && validarNome(buffer))
                { // Reutilizando validarNome
                    strcpy(medico_lido.especialidade, buffer);
                    break;
                }
                else if (strlen(buffer) == 0)
                {
                    break;
                }
            } while (1);

            // Validação do Telefone (e mantém o antigo se vazio)
            do
            {
                printf("Telefone atual: %s\nNovo telefone: ", medico_lido.telefone);
                lerString(buffer, 15);
                if (strlen(buffer) > 0 && validarTelefone(buffer))
                {
                    strcpy(medico_lido.telefone, buffer);
                    break;
                }
                else if (strlen(buffer) == 0)
                {
                    break;
                }
            } while (1);

            // CPF e ID não são alterados, pois são identificadores únicos

            fseek(arq_medicos, pos, SEEK_SET);
            fwrite(&medico_lido, sizeof(Medico), 1, arq_medicos);

            encontrado = 1;
            printf("\nDados do medico alterados com sucesso!\n");
            break;
        }
    }
    fclose(arq_medicos);

    if (!encontrado)
    {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    pressioneEnterParaContinuar();
}

// -----------
// EXCLUSÃO  |
// ------------
void excluir_medico(void)
{
    char cpf_busca[15];
    int encontrado = 0;
    Medico medico_lido;
    FILE *arq_medicos;
    long int pos;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║             Excluir Medico             ║\n");
    printf("╚════════════════════════════════════════╝\n");


    // Validação do CPF de busca
    do
    {
        printf("Informe o CPF do medico que deseja excluir: ");
        lerString(cpf_busca, 15);
    } while (!validarCPF(cpf_busca));

    arq_medicos = fopen(MEDICOS_FILE, "r+b");
    if (arq_medicos == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (strcmp(medico_lido.cpf, cpf_busca) == 0 && medico_lido.ativo == 1)
        {
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

    if (!encontrado)
    {
        printf("\nMedico com CPF %s nao encontrado.\n", cpf_busca);
    }
    pressioneEnterParaContinuar();
}

// ---------------------------------------
// LISTAGEM (Não requer input do usuário) |
// ---------------------------------------
void listar_medicos(void)
{
    Medico medico_lido;
    FILE *arq_medicos;
    int tem_medico = 0;

    limparTela();
    printf("╔══════════════════════════════════════╗\n");
    printf("║         Listagem de Medicos          ║\n");
    printf("╚══════════════════════════════════════╝\n");

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL)
    {
        printf("Nenhum medico cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║ ID ║ Nome do Medico        ║ CPF           ║ Especialidade ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");


    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (medico_lido.ativo == 1)
        {
            printf("║ %-2d ║ %-22s║ %-11s║ %s\n",
                   medico_lido.id, medico_lido.nome,
                   medico_lido.cpf, medico_lido.especialidade);
            tem_medico = 1;
        }
    }
    fclose(arq_medicos);

    if (!tem_medico)
    {
        printf("Nenhum medico ativo encontrado.\n");
    }
    pressioneEnterParaContinuar();
}

// ---------------------------------------
// RELATÓRIOS                            |
// ---------------------------------------

// Relatórios agora centralizados em src/relatorios.c

void relatorio_completo(void) {
    relatorios_medicos_completo();
}

void relatorio_por_especialidade(void) {
    relatorios_medicos_por_especialidade();
}

void relatorio_por_nome(void) {
    relatorios_medicos_por_nome();
}

void relatorio_por_status(void) {
    relatorios_medicos_por_status();
}


void submenu_relatorios(void) {
    relatorios_submenu_medicos();
}

// -------
// MENU  |
// -------
void modulo_medicos(void)
{
    int opcao;
    char bufferOpcao[5];
    criarPastaData();

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║           Modulo de Medicos            ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ 1. Cadastrar Medico                    ║\n");
        printf("║ 2. Pesquisar Medico                    ║\n");
        printf("║ 3. Alterar Medico                      ║\n");
        printf("║ 4. Excluir Medico                      ║\n");
        printf("║ 5. Listar Medicos                      ║\n");
        printf("║ 6. Relatórios                          ║\n");
        printf("║ 0. Voltar ao menu principal            ║\n");
        printf("╚════════════════════════════════════════╝\n");

        // Leitura segura de Opção
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
        } while (!validarOpcaoMenu(opcao, 0, 6));

        switch (opcao)
        {
        case 1:
            cadastrar_medico();
            break;
        case 2:
            pesquisar_medico();
            break;
        case 3:
            alterar_medico();
            break;
        case 4:
            excluir_medico();
            break;
        case 5:
            listar_medicos();
            break;
        // Chama o submenu de relatórios
        case 6:
            submenu_relatorios();
            break;
        case 0:
            break;
        }
    } while (opcao != 0);
}