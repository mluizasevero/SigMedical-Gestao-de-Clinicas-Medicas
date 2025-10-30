#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicos.h"
#include "utils.h"
#include "validador.h" // MUDANÇA: Incluindo a biblioteca de validação

#define MEDICOS_FILE DATA_DIR PATH_SEPARATOR "medicos.dat"

// Declaração da função auxiliar (Para uso interno)
// Verifica se o CPF já está cadastrado ou se o ID já existe
int medico_existe(const char *cpf, int id_a_ignorar);

// ----------------------------------------------------------------------------
// FUNÇÃO AUXILIAR: Checa unicidade (CPF) e ID
// ----------------------------------------------------------------------------
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
    printf("----------------------------------------\n");
    printf("///       Cadastrar Novo Medico      ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Validação de ID único e positivo
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

    // MUDANÇA: Validação do Nome
    do
    {
        printf("Informe o nome completo: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer));
    strcpy(novo_medico.nome, buffer);

    // MUDANÇA: Validação e Unicidade do CPF
    do
    {
        printf("Informe o CPF (apenas numeros, 11 digitos): ");
        lerString(buffer, 15);
        if (!validarCPF(buffer))
        { // Assumindo que validarCPF existe e checa formato
            continue;
        }
        if (medico_existe(buffer, -1))
        { // -1 para não ignorar nenhum ID
            printf("! Erro: CPF ja cadastrado.\n");
        }
    } while (!validarCPF(buffer) || medico_existe(buffer, -1));
    strcpy(novo_medico.cpf, buffer);

    // MUDANÇA: Validação da Especialidade
    do
    {
        printf("Informe a especialidade: ");
        lerString(buffer, 50);
    } while (!validarNome(buffer)); // Reutilizando validarNome
    strcpy(novo_medico.especialidade, buffer);

    // MUDANÇA: Validação do Telefone
    do
    {
        printf("Informe o telefone (DDNNNNNNNN): ");
        lerString(buffer, 15);
    } while (!validarTelefone(buffer)); // Assumindo que validarTelefone existe
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
    printf("----------------------------------------\n");
    printf("///     Pesquisar Medico por CPF     ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Validação do CPF
    do
    {
        printf("Informe o CPF do medico a ser pesquisado: ");
        lerString(cpf_busca, 15);
    } while (!validarCPF(cpf_busca)); // Validação de formato

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
    printf("----------------------------------------\n");
    printf("///      Alterar Dados de Medico     ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Validação do CPF de busca
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

            // MUDANÇA: Validação do Nome (e mantém o antigo se vazio)
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

            // MUDANÇA: Validação da Especialidade (e mantém o antigo se vazio)
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

            // MUDANÇA: Validação do Telefone (e mantém o antigo se vazio)
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
    printf("----------------------------------------\n");
    printf("///          Excluir Medico          ///\n");
    printf("----------------------------------------\n");

    // MUDANÇA: Validação do CPF de busca
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
    printf("----------------------------------------\n");
    printf("///        Listagem de Medicos       ///\n");
    printf("----------------------------------------\n");

    arq_medicos = fopen(MEDICOS_FILE, "rb");
    if (arq_medicos == NULL)
    {
        printf("Nenhum medico cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("ID | Nome do Medico         | CPF           | Especialidade\n");
    printf("---|------------------------|---------------|------------------\n");

    while (fread(&medico_lido, sizeof(Medico), 1, arq_medicos))
    {
        if (medico_lido.ativo == 1)
        {
            printf("%-2d | %-22s | %-13s | %s\n",
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
        printf("----------------------------------------\n");
        printf("///         Modulo de Medicos        ///\n");
        printf("----------------------------------------\n");
        printf("1. Cadastrar Medico\n");
        printf("2. Pesquisar Medico\n");
        printf("3. Alterar Medico\n");
        printf("4. Excluir Medico\n");
        printf("5. Listar Medicos\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------------\n");

        // MUDANÇA: Leitura segura de Opção
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
        case 0:
            break;
            // Default não é mais necessário
        }
    } while (opcao != 0);
}
