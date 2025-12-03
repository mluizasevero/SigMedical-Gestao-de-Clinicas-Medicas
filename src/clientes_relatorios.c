#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "utils.h"
#include "validador.h"
#include "clientes_relatorios.h"

#define CLIENTES_FILE DATA_DIR PATH_SEPARATOR "clientes.dat"

// Estrutura para lista dinâmica direta (usada nos relatórios)
typedef struct
{
    Cliente *clientes;
    size_t tamanho;
    size_t capacidade;
} ListaClientesRelatorio;

// Função para inicializar a lista dinâmica direta
static void inicializar_lista_relatorio(ListaClientesRelatorio *lista, size_t capacidade_inicial)
{
    lista->clientes = (Cliente *)malloc(capacidade_inicial * sizeof(Cliente));
    lista->tamanho = 0;
    lista->capacidade = capacidade_inicial;
}

// Função para adicionar um cliente à lista dinâmica direta
static void adicionar_cliente_relatorio(ListaClientesRelatorio *lista, Cliente cliente)
{
    if (lista->tamanho == lista->capacidade)
    {
        lista->capacidade *= 2;
        lista->clientes = (Cliente *)realloc(lista->clientes, lista->capacidade * sizeof(Cliente));
    }
    lista->clientes[lista->tamanho++] = cliente;
}

// Função para liberar a memória da lista dinâmica direta
static void liberar_lista_clientes_relatorio(ListaClientesRelatorio *lista)
{
    free(lista->clientes);
    lista->clientes = NULL;
    lista->tamanho = 0;
    lista->capacidade = 0;
}

// Função para carregar clientes do arquivo para a lista dinâmica direta
static void carregar_clientes_para_lista_relatorio(ListaClientesRelatorio *lista)
{
    FILE *arq_clientes = fopen(CLIENTES_FILE, "rb");
    if (arq_clientes == NULL)
    {
        return;
    }

    Cliente cliente_lido;
    while (fread(&cliente_lido, sizeof(Cliente), 1, arq_clientes))
    {
        adicionar_cliente_relatorio(lista, cliente_lido);
    }

    fclose(arq_clientes);
}

// Função auxiliar para exibir um título de seção
static void exibir_titulo_relatorio_cliente(const char *titulo)
{
    limparTela();
    printf("╔═════════════════════════════════════════════╗\n");
    int len = strlen(titulo);
    int pad_left = (45 - len) / 2;
    int pad_right = 45 - len - pad_left;
    printf("║%*s%s%*s║\n", pad_left, "", titulo, pad_right, "");
    printf("╚═════════════════════════════════════════════╝\n");
}

// Função auxiliar para exibir mensagem se não houver resultados
static void exibir_mensagem_sem_resultado_cliente(int tem_registro)
{
    if (!tem_registro)
    {
        printf("Nenhum registro encontrado.\n");
    }
}

// Função para exibir o relatório completo de clientes ativos e inativos
void relatorio_clientes_completo(void)
{
    exibir_titulo_relatorio_cliente("Relatorio Completo de Clientes");

    ListaClientesRelatorio lista;
    inicializar_lista_relatorio(&lista, 10);
    carregar_clientes_para_lista_relatorio(&lista);

    if (lista.tamanho == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
        liberar_lista_clientes_relatorio(&lista);
        pressioneEnterParaContinuar();
        return;
    }

    int total_ativos = 0, total_inativos = 0;

    // Contagem de clientes
    for (size_t i = 0; i < lista.tamanho; i++)
    {
        if (lista.clientes[i].ativo == 1)
        {
            total_ativos++;
        }
        else
        {
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
    for (size_t i = 0; i < lista.tamanho; i++)
    {
        if (lista.clientes[i].ativo == 1)
        {
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
    for (size_t i = 0; i < lista.tamanho; i++)
    {
        if (lista.clientes[i].ativo == 0)
        {
            printf("║ %-24s ║ %-13s ║ %-13s ║ %-17s ║\n",
                   lista.clientes[i].nome, lista.clientes[i].cpf,
                   lista.clientes[i].telefone, lista.clientes[i].email);
        }
    }
    printf("╚══════════════════════════╩═══════════════╩═══════════════╩═════════════════╝\n");

    liberar_lista_clientes_relatorio(&lista);

    printf("\n═══ Fim do Relatorio ═══\n");
    pressioneEnterParaContinuar();
}

// Função para exibir o relatório filtrado por nome (parcial)
void relatorio_clientes_por_nome(void)
{
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
void relatorio_clientes_por_status(void)
{
    Cliente cliente_lido;
    FILE *arq_clientes;
    int status_filtro;
    int tem_registro = 0;
    char status_texto[10];

    exibir_titulo_relatorio_cliente("Relatorio por Status");

    do
    {
        printf("Deseja filtrar por (1) ATIVOS ou (2) INATIVOS? (1/2): ");
        char buffer[3];
        lerString(buffer, 2);
        status_filtro = validarInteiroPositivo(buffer);
        if (status_filtro != 2 && status_filtro != 1)
        {
            printf("Opcao invalida. Por favor, digite 1 para ATIVO ou 2 para INATIVO.\n");
        }
    } while (status_filtro != 2 && status_filtro != 1);

    if (status_filtro == 1)
    {
        strcpy(status_texto, "ATIVO");
    }
    else
    {
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
        if ((status_filtro == 1 && cliente_lido.ativo == 1) ||
            (status_filtro == 2 && cliente_lido.ativo == 0))
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
void relatorio_clientes_submenu(void)
{
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
            relatorio_clientes_completo();
            break;
        case 2:
            relatorio_clientes_por_nome();
            break;
        case 3:
            relatorio_clientes_por_status();
            break;
        case 0:
            break;
        }
    } while (opcao_relatorio != 0);
}
