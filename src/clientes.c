#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "utils.h"
#include "validador.h" // Incluindo a nova biblioteca

// Definição de CLIENTES FILE fora de utils.h
#define CLIENTES_FILE DATA_DIR PATH_SEPARATOR "clientes.dat"

// Estrutura para lista dinâmica direta
typedef struct {
    Cliente *clientes;
    size_t tamanho;
    size_t capacidade;
} ListaClientes;

// Função para inicializar a lista dinâmica direta
void inicializar_lista(ListaClientes *lista, size_t capacidade_inicial) {
    lista->clientes = (Cliente *)malloc(capacidade_inicial * sizeof(Cliente));
    lista->tamanho = 0;
    lista->capacidade = capacidade_inicial;
}

// Função para adicionar um cliente à lista dinâmica direta
void adicionar_cliente(ListaClientes *lista, Cliente cliente) {
    if (lista->tamanho == lista->capacidade) {
        lista->capacidade *= 2;
        lista->clientes = (Cliente *)realloc(lista->clientes, lista->capacidade * sizeof(Cliente));
    }
    lista->clientes[lista->tamanho++] = cliente;
}

// Função para liberar a memória da lista dinâmica direta
void liberar_lista_clientes(ListaClientes *lista) {
    free(lista->clientes);
    lista->clientes = NULL;
    lista->tamanho = 0;
    lista->capacidade = 0;
}

// Função para carregar clientes do arquivo para a lista dinâmica direta
void carregar_clientes_para_lista(ListaClientes *lista) {
    FILE *arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL) {
        return;
    }

    Cliente cliente_lido;
    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes)) {
        adicionar_cliente(lista, cliente_lido);
    }

    fclose(arq_clientes);
}

// Função interna para verificar se um CPF já existe
// Retorna 1 se já existe, 0 se não
int verifica_cpf_cliente_cadastrado(const char *cpf)
{
    FILE *arq = fopen(CLIENTES_FILE, "rb");
    if (arq == NULL)
    {
        return 0; // Arquivo não existe, então CPF não está cadastrado
    }

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, arq))
    {
        if (strcmp(c.cpf, cpf) == 0 && c.ativo == 1)
        {
            fclose(arq);
            return 1; // Encontrou CPF ativo
        }
    }
    fclose(arq);
    return 0; // Não encontrou
}

// Função auxiliar para exibir um título de seção
void exibir_titulo_relatorio_cliente(const char* titulo) {
    limparTela();
    printf("╔═════════════════════════════════════════════╗\n");
    int len = strlen(titulo);
    int pad_left = (45 - len) / 2;
    int pad_right = 45 - len - pad_left;
    printf("║%*s%s%*s║\n", pad_left, "", titulo, pad_right, "");
    printf("╚═════════════════════════════════════════════╝\n");
}

// Função auxiliar para exibir mensagem se não houver resultados
void exibir_mensagem_sem_resultado_cliente(int tem_registro) {
    if (!tem_registro) {
        printf("Nenhum registro encontrado.\n");
    }
}

// ----------
// CADASTRO |
// ----------
void cadastrar_cliente(void)
{
    Cliente novo_cliente;
    FILE *arq_clientes;

    // Buffers temporários para leitura e validação
    char buffer[51]; // 50 + 1 para o \0

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║         Cadastrar Novo Cliente         ║\n");
    printf("╚════════════════════════════════════════╝\n");


    // Loop de validação para CPF
    do
    {
        printf("\nInforme o CPF do cliente (apenas numeros ou com ./): ");
        lerString(buffer, 15); // Usa lerString (tamanho 15)
        if (!validarCPF(buffer))
        {
            printf("! CPF invalido. Formato ou digitos verificadores incorretos. Tente novamente.\n");
        }
        else if (verifica_cpf_cliente_cadastrado(buffer))
        {
            printf("! CPF ja cadastrado no sistema. Tente novamente.\n");
        }
    } while (!validarCPF(buffer) || verifica_cpf_cliente_cadastrado(buffer));
    strcpy(novo_cliente.cpf, buffer); // Copia o dado validado

    // Loop de validação para Nome
    do
    {
        printf("Informe o nome do cliente: ");
        lerString(buffer, 50); // Usa lerString (tamanho 50)
        // A função validarNome já imprime o erro se houver
    } while (!validarNome(buffer));
    strcpy(novo_cliente.nome, buffer); // Copia o dado validado

    // Loop de validação para Telefone
    do
    {
        printf("Informe o telefone do cliente (com DDD): ");
        lerString(buffer, 15); // Usa lerString (tamanho 15)
    } while (!validarTelefone(buffer));
    strcpy(novo_cliente.telefone, buffer); // Copia o dado validado

    // Loop de validação para Email
    do
    {
        printf("Informe o email do cliente: ");
        lerString(buffer, 50); // Usa lerString (tamanho 50)
    } while (!validarEmail(buffer));
    strcpy(novo_cliente.email, buffer); // Copia o dado validado

    novo_cliente.ativo = 1; // define um cliente novo como "ativo" (existente)

    // abre o arquivo em modo "ab" (append binary) ---> é add no final
    arq_clientes = fopen(CLIENTES_FILE, "ab");
    if (arq_clientes == NULL)
    {
        printf("\nErro fatal ao abrir o arquivo para escrita!\n");
        pressioneEnterParaContinuar();
        return;
    }

    // escreve a struct inteira
    fwrite(&novo_cliente, sizeof(Cliente), 1, arq_clientes);
    fclose(arq_clientes);

    printf("\nCliente cadastrado com sucesso!\n");
    pressioneEnterParaContinuar();
}

// -----------
// PESQUISA  |
// -----------
void pesquisar_cliente(void)
{
    char cpf_pesquisa[15];
    int encontrado = 0;
    Cliente cliente_lido;
    FILE *arq_clientes;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║           Pesquisar Cliente            ║\n");
    printf("╚════════════════════════════════════════╝\n");


    // Validação do CPF de pesquisa (opcional, mas bom)
    printf("Informe o CPF do cliente para pesquisa: ");
    lerString(cpf_pesquisa, 15);

    // abre o arquivo em read binary
    arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL)
    {
        printf("\nNenhum cliente cadastrado ou erro ao abrir o arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    // lê o arquivo struct por struct
    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes))
    {
        // compara o CPF lido com o CPF pesquisado + verifica se o registro está ativo
        if (strcmp(cliente_lido.cpf, cpf_pesquisa) == 0 && cliente_lido.ativo == 1)
        {
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

    if (!encontrado)
    {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_pesquisa);
    }
    pressioneEnterParaContinuar();
}

// ------------
// ALTERAÇÃO  |
// ------------
void alterar_cliente(void)
{
    char cpf_alteracao[15];
    int encontrado = 0;
    Cliente cliente_lido;
    FILE *arq_clientes;
    long int pos;

    // Buffer para novos dados
    char buffer[51];

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║        Alterar Dados do Cliente        ║\n");
    printf("╚════════════════════════════════════════╝\n");


    // Validação do CPF de alteração
    printf("Informe o CPF do cliente que deseja alterar: ");
    lerString(cpf_alteracao, 15);

    // abre leitura e escrita binária
    arq_clientes = fopen(CLIENTES_FILE, "r+b");
    if (arq_clientes == NULL)
    {
        printf("\nErro ao abrir arquivo. Nenhum cliente cadastrado?\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes))
    {
        if (strcmp(cliente_lido.cpf, cpf_alteracao) == 0 && cliente_lido.ativo == 1)
        {
            // guarda a posição do início do registro
            pos = ftell(arq_clientes) - sizeof(Cliente);

            printf("\nCliente encontrado. Informe os novos dados:\n");

            // Loop de validação para Nome
            do
            {
                printf("Nome atual: %s\nNovo Nome: ", cliente_lido.nome);
                lerString(buffer, 50);
            } while (!validarNome(buffer));
            strcpy(cliente_lido.nome, buffer); // Copia

            // Loop de validação para Telefone
            do
            {
                printf("Telefone atual: %s\nNovo Telefone: ", cliente_lido.telefone);
                lerString(buffer, 15);
            } while (!validarTelefone(buffer));
            strcpy(cliente_lido.telefone, buffer); // Copia

            // Loop de validação para Email
            do
            {
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

    if (!encontrado)
    {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_alteracao);
    }
    pressioneEnterParaContinuar();
}

// ------------
// EXCLUSÃO   |
// ------------
void excluir_cliente(void)
{
    char cpf_exclusao[15];
    int encontrado = 0;
    Cliente cliente_lido;
    FILE *arq_clientes;
    long int pos;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║             Excluir Cliente            ║\n");
    printf("╚════════════════════════════════════════╝\n");


    printf("Informe o CPF do cliente que deseja excluir: ");
    lerString(cpf_exclusao, 15);

    arq_clientes = fopen(CLIENTES_FILE, "r+b");
    if (arq_clientes == NULL)
    {
        printf("\nErro ao abrir arquivo.\n");
        pressioneEnterParaContinuar();
        return;
    }

    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes))
    {
        if (strcmp(cliente_lido.cpf, cpf_exclusao) == 0 && cliente_lido.ativo == 1)
        {
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

    if (!encontrado)
    {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_exclusao);
    }
    pressioneEnterParaContinuar();
}

// ---------------------------------------
// LISTAGEM (Não requer input do usuário) |
// ---------------------------------------
void listar_clientes(void)
{
    FILE *arq_clientes;
    Cliente cliente_lido;
    int nenhum_cliente = 1;

    limparTela();
    printf("╔════════════════════════════════════════╗\n");
    printf("║          Listagem de Clientes          ║\n");
    printf("╚════════════════════════════════════════╝\n");

    arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL)
    {
        printf("\nNenhum cliente cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }
    // Cabeçalho da tabela
    printf("╔══════════════════════════╦═══════════════╦═══════════════╦═════════════════╗\n");
    printf("║ Nome do Cliente          ║ CPF           ║ Telefone      ║ Email           ║\n");
    printf("╠══════════════════════════╬═══════════════╬═══════════════╬═════════════════╣\n");

    // Leitura dos clientes
    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes) == 1)
    {
        nenhum_cliente = 0;

        printf("║ %-24s ║ %-13s ║ %-13s ║ %-17s ║\n",
               cliente_lido.nome,
               cliente_lido.cpf,
               cliente_lido.telefone,
               cliente_lido.email);
    }
    printf("╚══════════════════════════╩═══════════════╩═══════════════╩═════════════════╝\n");
    fclose(arq_clientes);

    if (nenhum_cliente)
    {
        printf("\nNenhum cliente ativo para exibir.\n");
    }

    pressioneEnterParaContinuar();
}

// ---------------------------------------
// RELATÓRIOS                            |
// ---------------------------------------

// Função para exibir o relatório completo de clientes ativos e inativos
void relatorio_completo_clientes(void) {
    exibir_titulo_relatorio_cliente("Relatorio Completo de Clientes");

    ListaClientes lista;
    inicializar_lista(&lista, 10);
    carregar_clientes_para_lista(&lista);

    if (lista.tamanho == 0) {
        printf("Nenhum cliente cadastrado.\n");
        liberar_lista_clientes(&lista);
        pressioneEnterParaContinuar();
        return;
    }

    int total_ativos = 0, total_inativos = 0;

    // Contagem de clientes
    for (size_t i = 0; i < lista.tamanho; i++) {
        if (lista.clientes[i].ativo == 1) {
            total_ativos++;
        } else {
            total_inativos++;
        }
    }

// Impressão das estatísticas
    printf("\n═══ Estatisticas Gerais ═══\n");
    printf("Total de clientes cadastrados: %zu\n", lista.tamanho);
    printf("Total de clientes ATIVOS:      %d\n", total_ativos);
    printf("Total de clientes INATIVOS:    %d\n", total_inativos);
    printf("═══════════════════════════\n\n");

    // Listagem detalhada de clientes ativos
    printf("═══ Clientes ATIVOS ═══\n");
    printf("╔══════════════════════════╦═══════════════╦═══════════════╦═════════════════╗\n");
    printf("║ Nome do Cliente          ║ CPF           ║ Telefone      ║ Email           ║\n");
    printf("╠══════════════════════════╬═══════════════╬═══════════════╬═════════════════╣\n");
    for (size_t i = 0; i < lista.tamanho; i++) {
        if (lista.clientes[i].ativo == 1) {
            printf("║ %-24s ║ %-13s ║ %-13s ║ %-17s ║\n",
                   lista.clientes[i].nome, lista.clientes[i].cpf,
                   lista.clientes[i].telefone, lista.clientes[i].email);
        }
    }
    printf("╚══════════════════════════╩═══════════════╩═══════════════╩═════════════════╝\n");

    // Listagem detalhada de clientes inativos
    printf("\n═══ Clientes INATIVOS ═══\n");
    printf("╔══════════════════════════╦═══════════════╦═══════════════╦═════════════════╗\n");
    printf("║ Nome do Cliente          ║ CPF           ║ Telefone      ║ Email           ║\n");
    printf("╠══════════════════════════╬═══════════════╬═══════════════╬═════════════════╣\n");
    for (size_t i = 0; i < lista.tamanho; i++) {
        if (lista.clientes[i].ativo == 0) {
            printf("║ %-24s ║ %-13s ║ %-13s ║ %-17s ║\n",
                   lista.clientes[i].nome, lista.clientes[i].cpf,
                   lista.clientes[i].telefone, lista.clientes[i].email);
        }
    }
    printf("╚══════════════════════════╩═══════════════╩═══════════════╩═════════════════╝\n");

    liberar_lista_clientes(&lista);

    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

// Função para exibir o relatório filtrado por nome (parcial)
void relatorio_por_nome_cliente(void) {
    Cliente cliente_lido;
    FILE *arq_clientes;
    char nome_filtro[51];
    int tem_registro = 0;

    exibir_titulo_relatorio_cliente("Relatorio por Nome (Parcial)");

    printf("Informe parte do nome para filtrar: ");
    lerString(nome_filtro, 50);

    arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL)
    {
        printf("Nenhum cliente cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Clientes com nome contendo '%s' ═══\n", nome_filtro);
    printf("╔══════════════════════════╦═══════════════╦═══════════════╦═════════════════╗\n");
    printf("║ Nome do Cliente          ║ CPF           ║ Telefone      ║ Email           ║\n");
    printf("╠══════════════════════════╬═══════════════╬═══════════════╬═════════════════╣\n");

    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes))
    {
        if (cliente_lido.ativo == 1 && strstr(cliente_lido.nome, nome_filtro) != NULL)
        {
            printf("║ %-24s ║ %-13s ║ %-13s ║ %-17s ║\n",
                   cliente_lido.nome, cliente_lido.cpf,
                   cliente_lido.telefone, cliente_lido.email);
            tem_registro = 1;
        }
    }
    printf("╚══════════════════════════╩═══════════════╩═══════════════╩═════════════════╝\n");
    exibir_mensagem_sem_resultado_cliente(tem_registro);

    fclose(arq_clientes);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

// Função para exibir o relatório filtrado por status (ativo/inativo)
void relatorio_por_status_cliente(void) {
    Cliente cliente_lido;
    FILE *arq_clientes;
    int status_filtro;
    int tem_registro = 0;
    char status_texto[10];

    exibir_titulo_relatorio_cliente("Relatorio por Status");

    do {
        printf("Deseja filtrar por (1) ATIVOS ou (2) INATIVOS? (1/2): ");
        char buffer[3];
        lerString(buffer, 2);
        status_filtro = validarInteiroPositivo(buffer);
        if (status_filtro != 2 && status_filtro != 1) {
            printf("Opcao invalida. Por favor, digite 1 para ATIVO ou 2 para INATIVO.\n");
        }
    } while (status_filtro != 2 && status_filtro != 1);

    if (status_filtro == 1) {
        strcpy(status_texto, "ATIVO");
    } else {
        strcpy(status_texto, "INATIVO");
    }

    arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL)
    {
        printf("Nenhum cliente cadastrado.\n");
        pressioneEnterParaContinuar();
        return;
    }

    printf("\n═══ Clientes com status '%s' ═══\n", status_texto);
    printf("╔══════════════════════════╦═══════════════╦═══════════════╦═════════════════╗\n");
    printf("║ Nome do Cliente          ║ CPF           ║ Telefone      ║ Email           ║\n");
    printf("╠══════════════════════════╬═══════════════╬═══════════════╬═════════════════╣\n");

    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes))
    {
        if (cliente_lido.ativo == status_filtro)
        {
            printf("║ %-24s ║ %-13s ║ %-13s ║ %-17s ║\n",
                   cliente_lido.nome, cliente_lido.cpf,
                   cliente_lido.telefone, cliente_lido.email);
            tem_registro = 1;
        }
    }
    printf("╚══════════════════════════╩═══════════════╩═══════════════╩═════════════════╝\n");
    exibir_mensagem_sem_resultado_cliente(tem_registro);

    fclose(arq_clientes);
    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}


// Sub-menu para os relatórios de clientes
void submenu_relatorios_clientes(void) {
    int opcao_relatorio;
    char bufferOpcao[5];

    do
    {
        limparTela();
        printf("╔═════════════════════════════════════════════╗\n");
        printf("║            Submenu de Relatorios            ║\n");
        printf("╠═════════════════════════════════════════════╣\n");
        printf("║ 1. Relatorio Completo (Ativos e Inativos)   ║\n");
        printf("║ 2. Relatorio por Nome (Parcial)             ║\n");
        printf("║ 3. Relatorio por Status (Ativo/Inativo)     ║\n");
        printf("║ 0. Voltar ao menu principal de clientes     ║\n");
        printf("╚═════════════════════════════════════════════╝\n");
        // Leitura segura de Opção
        do
        {
            printf(">>> Escolha a opcao: ");
            lerString(bufferOpcao, 5);
            char *endptr;
            opcao_relatorio = strtol(bufferOpcao, &endptr, 10);
            if (endptr == bufferOpcao || *endptr != '\0')
            {
                opcao_relatorio = -1;
            }
        } while (!validarOpcaoMenu(opcao_relatorio, 0, 3));

        switch (opcao_relatorio)
        {
        case 1:
            relatorio_completo_clientes();
            break;
        case 2:
            relatorio_por_nome_cliente();
            break;
        case 3:
            relatorio_por_status_cliente();
            break;
        case 0:
            break;
        }
    } while (opcao_relatorio != 0);
}


// -------
// MENU  |
// -------
void modulo_clientes(void)
{
    int opcao;
    char bufferOpcao[5]; // Buffer para ler a opção

    // garante que a pasta ./data exista antes de qualquer operação
    criarPastaData();

    do
    {
        limparTela();
        printf("╔════════════════════════════════════════╗\n");
        printf("║           Modulo de Clientes           ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ 1. Cadastrar Cliente                   ║\n");
        printf("║ 2. Pesquisar Cliente                   ║\n");
        printf("║ 3. Alterar Cliente                     ║\n");
        printf("║ 4. Excluir Cliente                     ║\n");
        printf("║ 5. Listar Clientes                     ║\n");
        printf("║ 6. Relatórios                          ║\n");
        printf("║ 0. Voltar ao menu principal            ║\n");
        printf("╚════════════════════════════════════════╝\n");

        // Leitura e validação da opção de menu
        do
        {
            printf("Escolha uma opcao: ");
            lerString(bufferOpcao, 5); // Lê como string

            // Tenta converter string para long int
            char *endptr;
            opcao = strtol(bufferOpcao, &endptr, 10);

            // Verifica se a conversão falhou (ex: "abc") ou se não era um número inteiro ("1.5")
            // ou se a opção está fora do intervalo
            if (endptr == bufferOpcao || *endptr != '\0')
            {
                opcao = -1; // Seta uma opção inválida para forçar o erro
            }

        } while (!validarOpcaoMenu(opcao, 0, 6)); // Ajustando o valor máximo

        switch (opcao)
        {
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
        // Adicionando o case para o submenu de relatórios
        case 6:
            submenu_relatorios_clientes();
            break;
        case 0:
            break;
            // Default não é mais necessário para "opção inválida"
            // pois o loop 'do-while' já trata isso.
        }
    } while (opcao != 0);
}