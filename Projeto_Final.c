#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct {
    char cpf[12];
    char nome[50];
    char telefone[15];
    char email[50];
} Cliente;

typedef struct {
    int id;
    char nome[50];
    int quantidade;
    char validade[11];
} Produto;

typedef struct {
    char nome_paciente[50];
    char cpf_paciente[12];
    char data[11];
    char hora[6];
    char nome_medico[50];
    char especialidade[50];
} Consulta;

void salvar_clientes(Cliente clientes[], int total_clientes);
int ler_clientes(Cliente clientes[]);
void salvar_produtos(Produto produtos[], int total_produtos);
int ler_produtos(Produto produtos[]);
void salvar_consultas(Consulta consultas[], int total_consultas);
int ler_consultas(Consulta consultas[]);
void desenhar_cabecalho_base(void);
void tela_equipe(void);
void limpar_tela(void);
void finalizar_programa(void);

void TelaMenuPrincipal(void);
void TelaMenuFuncionarios(void);
void TelaMenuClientes(void);
void TelaMenuConsultas(void);
void TelaMenuEstoque(void);

void TelaCadastrarFuncionarios(void);
void TelaPesquisarFuncionarios(void);
void TelaAlterarFuncionarios(void);
void TelaExcluirFuncionarios(void);
void TelaConfirmarExclusao(void);
void TelaCadastrarClientes(void);
void TelaPesquisarClientes(void);
void TelaAlterarClientes(void);
void TelaExcluirClientes(void);
void TelaConfirmarExclusaoCliente(void);
void TelaAgendarNovaConsulta(void);
void TelaGerenciarAgendamentos(void);
void TelaGerarRelatorios(void);
void TelaCadastrarProduto(void);
void TelaPesquisarProduto(void);
void TelaGerenciarLotes(void);
void TelaMovimentarEstoque(void);
void TelaGerarRelatoriosEstoque(void);

void modulo_clientes(Cliente clientes[], int* total_clientes);
void modulo_consultas(Consulta consultas[], int* total_consultas);
void modulo_estoque(Produto produtos[], int* total_produtos);
void modulo_funcionarios();

void pesquisar_cliente(Cliente clientes[], int total_clientes);
void alterar_cliente(Cliente clientes[], int total_clientes);
void excluir_cliente(Cliente clientes[], int* total_clientes);

void pesquisar_consulta(Consulta consultas[], int total_consultas);
void alterar_consulta(Consulta consultas[], int total_consultas);
void excluir_consulta(Consulta consultas[], int* total_consultas);

void pesquisar_produto(Produto produtos[], int total_produtos);
void movimentar_estoque(Produto produtos[], int total_produtos);

void salvar_clientes(Cliente clientes[], int total_clientes) {
    FILE *arq_clientes = fopen("clientes.dat", "w");
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
    FILE *arq_clientes = fopen("clientes.dat", "r");
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

void salvar_produtos(Produto produtos[], int total_produtos) {
    FILE *arq_produtos = fopen("produtos.dat", "w");
    if (arq_produtos == NULL) {
        printf("Erro ao abrir o arquivo 'produtos.dat' para escrita.\n");
        return;
    }
    for (int i = 0; i < total_produtos; i++) {
        fprintf(arq_produtos, "%d;%s;%d;%s\n", 
                produtos[i].id, produtos[i].nome, 
                produtos[i].quantidade, produtos[i].validade);
    }
    fclose(arq_produtos);
}

int ler_produtos(Produto produtos[]) {
    FILE *arq_produtos = fopen("produtos.dat", "r");
    if (arq_produtos == NULL) {
        return 0; 
    }
    int i = 0;
    while(fscanf(arq_produtos, "%d;%[^;];%d;%[^\n]\n", 
                 &produtos[i].id, produtos[i].nome, 
                 &produtos[i].quantidade, produtos[i].validade) == 4) {
        i++;
    }
    fclose(arq_produtos);
    return i; 
}

void salvar_consultas(Consulta consultas[], int total_consultas) {
    FILE *arq_consultas = fopen("consultas.dat", "w");
    if (arq_consultas == NULL) {
        printf("Erro ao abrir o arquivo 'consultas.dat' para escrita.\n");
        return;
    }
    for (int i = 0; i < total_consultas; i++) {
        fprintf(arq_consultas, "%s;%s;%s;%s;%s;%s\n", 
                consultas[i].nome_paciente, consultas[i].cpf_paciente, 
                consultas[i].data, consultas[i].hora, 
                consultas[i].nome_medico, consultas[i].especialidade);
    }
    fclose(arq_consultas);
}

int ler_consultas(Consulta consultas[]) {
    FILE *arq_consultas = fopen("consultas.dat", "r");
    if (arq_consultas == NULL) {
        return 0; 
    }
    int i = 0;
    while(fscanf(arq_consultas, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n", 
                 consultas[i].nome_paciente, consultas[i].cpf_paciente, 
                 consultas[i].data, consultas[i].hora, 
                 consultas[i].nome_medico, consultas[i].especialidade) == 6) {
        i++;
    }
    fclose(arq_consultas);
    return i; 
}

// Funções de cabeçalho e equipe
void desenhar_cabecalho_base(void) {
    printf("///////////////////////////////////////////////////////////////////////////////\n");
    printf("///                                                                         ///\n");
    printf("///              Universidade Federal do Rio Grande do Norte (UFRN)         ///\n");
    printf("///                Centro de Ensino Superior do Serido (CERES)              ///\n");
    printf("///              Departamento de Computacao e Tecnologia (DCT)              ///\n");
    printf("///                     Disciplina DCT1106 -- Programacao                   ///\n");
    printf("///                                                                         ///\n");
    printf("///       Projeto SigMedical :: Sistema de Gestao de Clinicas Medicas       ///\n");
    printf("///       Developed by Maria Luiza Severo and Thiago Gomes de Oliveira      ///\n");
    printf("///                                                                         ///\n");
    printf("///////////////////////////////////////////////////////////////////////////////\n");
}

void tela_equipe(void) {
    const char* info =
        "///      Este projeto foi desenvolvido por:                                 ///\n"
        "///                                                                         ///\n"
        "///      Maria Luiza Severo                                                 ///\n"
        "///      E-mail: maria.severo.063@ufrn.edu.br                               ///\n"
        "///                                                                         ///\n"
        "///      Thiago Gomes de Oliveira                                           ///\n"
        "///      E-mail: Thiago.oliveira712@ufrn.edu.br                             ///";

    desenhar_cabecalho_base();
    printf("%s\n", info);
    printf("///////////////////////////////////////////////////////////////////////////////\n");
}

void limpar_tela(void) {
    system("cls");
}

// Funções de Menus
void TelaMenuPrincipal() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///           MENU PRINCIPAL          ///\n");
    printf("----------------------------------------\n");
    printf("///                                   ///\n");
    printf("///      1. Módulo Clientes           ///\n");
    printf("///      2. Módulo Funcionários       ///\n");
    printf("///      3. Módulo Consultas          ///\n");
    printf("///      4. Módulo Estoque            ///\n");
    printf("///      0. Sair                      ///\n");
    printf("///                                   ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaMenuFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          Módulo Funcionários        ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///     1. Cadastrar Funcionários       ///\n");
    printf("///     2. Pesquisar Funcionários       ///\n");
    printf("///     3. Alterar Funcionários         ///\n");
    printf("///     4. Excluir Funcionários         ///\n");
    printf("///     0. Voltar ao Menu Principal     ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaMenuClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///           Módulo Clientes          ///\n");
    printf("----------------------------------------\n");
    printf("///                                    ///\n");
    printf("///     1. Cadastrar Clientes          ///\n");
    printf("///     2. Pesquisar Clientes          ///\n");
    printf("///     3. Alterar Clientes            ///\n");
    printf("///     4. Excluir Clientes            ///\n");
    printf("///     0. Voltar ao Menu Principal    ///\n");
    printf("///                                    ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaMenuConsultas() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          Módulo Consultas          ///\n");
    printf("----------------------------------------\n");
    printf("///                                    ///\n");
    printf("///     1. Agendar Nova Consulta       ///\n");
    printf("///     2. Pesquisar Consultas         ///\n");
    printf("///     3. Gerenciar Agendamentos      ///\n");
    printf("///     4. Gerar Relatórios            ///\n");
    printf("///     0. Voltar ao Menu Principal    ///\n");
    printf("///                                    ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaMenuEstoque() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///           Módulo Estoque           ///\n");
    printf("----------------------------------------\n");
    printf("///                                    ///\n");
    printf("///     1. Cadastrar Clientes          ///\n");
    printf("///     2. Pesquisar Clientes          ///\n");
    printf("///     3. Alterar Clientes            ///\n");
    printf("///     4. Excluir Clientes            ///\n");
    printf("///     0. Voltar ao Menu Principal    ///\n");
    printf("///                                    ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

// Funções de Sub-Menus
void TelaCadastrarFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Cadastrar Funcionário        ///\n");
    printf("----------------------------------------\n");
    printf("///                                    ///\n");
    printf("///      CPF:                          ///\n");
    printf("///      Nome:                         ///\n");
    printf("///      Telefone:                     ///\n");
    printf("///      E-mail:                       ///\n");
    printf("///                                    ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Pesquisar Funcionário        ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///    Informe o CPF do funcionário:    ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaAlterarFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Alterar Funcionário         ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///    Informe o CPF do funcionário:    ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaExcluirFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          Excluir Funcionário        ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///    Informe o CPF do funcionário:    ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaConfirmarExclusao() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Confirmar Exclusão?         ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      1. Sim (S/s)                   ///\n");
    printf("///      2. Não (N/n)                   ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaCadastrarClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Cadastrar Cliente           ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      CPF:                           ///\n");
    printf("///      Nome:                          ///\n");
    printf("///      Telefone:                      ///\n");
    printf("///      E-mail:                        ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Pesquisar Cliente           ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      Informe o CPF do cliente:      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaAlterarClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          Alterar Cliente            ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      Informe o CPF do cliente:      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaExcluirClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          Excluir Cliente            ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      Informe o CPF do cliente:      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaConfirmarExclusaoCliente() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Confirmar Exclusão?         ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      1. Sim (S/s)                   ///\n");
    printf("///      2. Não (N/n)                   ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaAgendarNovaConsulta() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Agendar Nova Consulta         ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      Nome do Paciente:              ///\n");
    printf("///      CPF do Paciente:               ///\n");
    printf("///      Data (dd/mm/aaaa):             ///\n");
    printf("///      Hora (hh:mm):                  ///\n");
    printf("///      Nome do Medico:                ///\n");
    printf("///      Especialidade:                 ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaGerenciarAgendamentos() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///      Gerenciar Agendamentos         ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      1. Alterar Consulta Agendada   ///\n");
    printf("///      2. Cancelar Consulta           ///\n");
    printf("///      3. Confirmar Presença          ///\n");
    printf("///      0. Voltar                      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaGerarRelatorios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Gerar Relatórios            ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      1. Consultas por Médico        ///\n");
    printf("///      2. Consultas por Período       ///\n");
    printf("///      3. Consultas Agendadas         ///\n");
    printf("///      4. Consultas Canceladas        ///\n");
    printf("///      0. Voltar                      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaCadastrarProduto() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Cadastrar Produto            ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      ID do Produto:                 ///\n");
    printf("///      Nome do Produto:               ///\n");
    printf("///      Quantidade:                    ///\n");
    printf("///      Data de Validade:              ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarProduto() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Pesquisar Produto            ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      1. Pesquisar por ID            ///\n");
    printf("///      2. Pesquisar por Nome          ///\n");
    printf("///      0. Voltar                      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaGerenciarLotes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Gerenciar Lotes              ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      1. Visualizar Lotes            ///\n");
    printf("///      2. Adicionar Lote              ///\n");
    printf("///      3. Remover Lote                ///\n");
    printf("///      0. Voltar                      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaMovimentarEstoque() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Movimentar Estoque            ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///      1. Saída de Material           ///\n");
    printf("///      2. Entrada de Material         ///\n");
    printf("///      0. Voltar                      ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaGerarRelatoriosEstoque() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Gerar Relatórios            ///\n");
    printf("----------------------------------------\n");
    printf("///                                     ///\n");
    printf("///     1. Itens em Falta               ///\n");
    printf("///     2. Itens com Validade Próxima   ///\n");
    printf("///     3. Histórico de Movimentações   ///\n");
    printf("///     0. Voltar                       ///\n");
    printf("///                                     ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void finalizar_programa() {
    system("cls");

    printf("\n\n");
    printf("////////////////////////////////////////////////////////////////\n");
    printf("///                                                          ///\n");
    printf("///          Programa finalizado. Volte sempre!              ///\n");
    printf("///                                                          ///\n");
    printf("////////////////////////////////////////////////////////////////\n");
    printf("\n");
    printf("            Pressione ENTER para sair...");
    
    getchar(); 
}

void TelaCadastrarClientes() { limpar_tela(); printf("----------------------------------------\n///         Cadastrar Cliente        ///\n----------------------------------------\n"); }
void TelaPesquisarClientes() { limpar_tela(); printf("----------------------------------------\n///        Pesquisar Cliente         ///\n----------------------------------------\n"); }
void TelaAlterarClientes() { limpar_tela(); printf("----------------------------------------\n///         Alterar Cliente          ///\n----------------------------------------\n"); }
void TelaExcluirClientes() { limpar_tela(); printf("----------------------------------------\n///         Excluir Cliente          ///\n----------------------------------------\n"); }
void TelaConfirmarExclusaoCliente() { limpar_tela(); printf("----------------------------------------\n///       Confirmar Exclusao?        ///\n----------------------------------------\n///      1. Sim (S/s)                ///\n///      2. Nao (N/n)                ///\n----------------------------------------\n>>> Escolha a opcao desejada: "); }
void TelaAgendarNovaConsulta() { limpar_tela(); printf("----------------------------------------\n///     Agendar Nova Consulta        ///\n----------------------------------------\n"); }
void TelaGerenciarAgendamentos() { limpar_tela(); printf("----------------------------------------\n///    Gerenciar Agendamentos        ///\n----------------------------------------\n"); }
void TelaGerarRelatorios() { limpar_tela(); printf("----------------------------------------\n///        Gerar Relatorios          ///\n----------------------------------------\n"); }
void TelaCadastrarProduto() { limpar_tela(); printf("----------------------------------------\n///       Cadastrar Produto          ///\n----------------------------------------\n"); }
void TelaPesquisarProduto() { limpar_tela(); printf("----------------------------------------\n///        Pesquisar Produto         ///\n----------------------------------------\n"); }
void TelaGerenciarLotes() { limpar_tela(); printf("----------------------------------------\n///       Gerenciar Lotes            ///\n----------------------------------------\n"); }
void TelaMovimentarEstoque() { limpar_tela(); printf("----------------------------------------\n///      Movimentar Estoque          ///\n----------------------------------------\n"); }
void TelaGerarRelatoriosEstoque() { limpar_tela(); printf("----------------------------------------\n///       Gerar Relatorios           ///\n----------------------------------------\n"); }

void pesquisar_cliente(Cliente clientes[], int total_clientes) {
    char cpf_pesquisa[12];
    int encontrado = 0;
    TelaPesquisarClientes();
    printf("Informe o CPF do cliente: ");
    scanf("%s", cpf_pesquisa);
    while (getchar() != '\n'); 

    for (int i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_pesquisa) == 0) {
            printf("\nCliente encontrado:\n");
            printf("CPF: %s\n", clientes[i].cpf);
            printf("Nome: %s\n", clientes[i].nome);
            printf("Telefone: %s\n", clientes[i].telefone);
            printf("E-mail: %s\n", clientes[i].email);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_pesquisa);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void alterar_cliente(Cliente clientes[], int total_clientes) {
    char cpf_alteracao[12];
    int encontrado = 0;
    TelaAlterarClientes();
    printf("Informe o CPF do cliente que deseja alterar: ");
    scanf("%s", cpf_alteracao);
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
            scanf("%s", clientes[i].telefone);
            while (getchar() != '\n');

            printf("E-mail atual: %s\n", clientes[i].email);
            printf("Novo E-mail: ");
            scanf("%s", clientes[i].email);
            while (getchar() != '\n');

            printf("\nCliente alterado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_alteracao);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void excluir_cliente(Cliente clientes[], int* total_clientes) {
    char cpf_exclusao[12];
    int encontrado = 0;
    TelaExcluirClientes();
    printf("Informe o CPF do cliente que deseja excluir: ");
    scanf("%s", cpf_exclusao);
    while (getchar() != '\n');

    for (int i = 0; i < *total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_exclusao) == 0) {
            char confirmacao;
            printf("\nCliente encontrado. Deseja realmente excluir este cliente (S/N)? ");
            scanf(" %c", &confirmacao);
            while (getchar() != '\n');
            if (confirmacao == 'S' || confirmacao == 's') {
                for (int j = i; j < *total_clientes - 1; j++) {
                    clientes[j] = clientes[j + 1];
                }
                (*total_clientes)--;
                printf("\nCliente excluido com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_exclusao);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void pesquisar_consulta(Consulta consultas[], int total_consultas) {
    char cpf_pesquisa[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///       Pesquisar Consulta         ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente: ");
    scanf("%s", cpf_pesquisa);
    while (getchar() != '\n'); 

    for (int i = 0; i < total_consultas; i++) {
        if (strcmp(consultas[i].cpf_paciente, cpf_pesquisa) == 0) {
            printf("\nConsulta encontrada:\n");
            printf("Nome do Paciente: %s\n", consultas[i].nome_paciente);
            printf("CPF: %s\n", consultas[i].cpf_paciente);
            printf("Data: %s\n", consultas[i].data);
            printf("Hora: %s\n", consultas[i].hora);
            printf("Medico: %s\n", consultas[i].nome_medico);
            printf("Especialidade: %s\n", consultas[i].especialidade);
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("\nConsulta para o CPF %s nao encontrada.\n", cpf_pesquisa);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void gerenciar_agendamentos(Consulta consultas[], int* total_consultas) {
    int opcao;
    do {
        TelaGerenciarAgendamentos();
        printf(">>> Escolha a opcao desejada: ");
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n'); 
        }

        switch (opcao) {
            case 1: // Alterar
                alterar_consulta(consultas, *total_consultas);
                break;
            case 2: // Cancelar (Excluir)
                excluir_consulta(consultas, total_consultas);
                break;
            case 3: // Confirmar Presença (em desenvolvimento)
                printf("Modulo de Confirmar Presenca em desenvolvimento.\n");
                press_enter_to_continue();
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao != 0);
}

void alterar_consulta(Consulta consultas[], int total_consultas) {
    char cpf_alteracao[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///       Alterar Consulta           ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente cuja consulta deseja alterar: ");
    scanf("%s", cpf_alteracao);
    while (getchar() != '\n');

    for (int i = 0; i < total_consultas; i++) {
        if (strcmp(consultas[i].cpf_paciente, cpf_alteracao) == 0) {
            printf("\nConsulta encontrada. Informe os novos dados:\n");
            
            printf("Data atual: %s\n", consultas[i].data);
            printf("Nova Data (dd/mm/aaaa): ");
            scanf("%s", consultas[i].data);
            while (getchar() != '\n');

            printf("Hora atual: %s\n", consultas[i].hora);
            printf("Nova Hora (hh:mm): ");
            scanf("%s", consultas[i].hora);
            while (getchar() != '\n');

            printf("Medico atual: %s\n", consultas[i].nome_medico);
            printf("Novo Medico: ");
            scanf(" %49[^\n]", consultas[i].nome_medico);
            while (getchar() != '\n');

            printf("Especialidade atual: %s\n", consultas[i].especialidade);
            printf("Nova Especialidade: ");
            scanf(" %49[^\n]", consultas[i].especialidade);
            while (getchar() != '\n');

            printf("\nConsulta alterada com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nNenhuma consulta para o CPF %s encontrada.\n", cpf_alteracao);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void pesquisar_produto(Produto produtos[], int total_produtos) {
    int id_pesquisa;
    int encontrado = 0;
    TelaPesquisarProduto();
    printf("Informe o ID do produto: ");
    scanf("%d", &id_pesquisa);
    while (getchar() != '\n');

    for (int i = 0; i < total_produtos; i++) {
        if (produtos[i].id == id_pesquisa) {
            printf("\nProduto encontrado:\n");
            printf("ID: %d\n", produtos[i].id);
            printf("Nome: %s\n", produtos[i].nome);
            printf("Quantidade: %d\n", produtos[i].quantidade);
            printf("Validade: %s\n", produtos[i].validade);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nProduto com ID %d nao encontrado.\n", id_pesquisa);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void excluir_cliente(Cliente clientes[], int* total_clientes) {
    char cpf_exclusao[12];
    int encontrado = 0;
    TelaExcluirClientes();
    printf("Informe o CPF do cliente que deseja excluir: ");
    scanf("%s", cpf_exclusao);
    while (getchar() != '\n');

    for (int i = 0; i < *total_clientes; i++) {
        if (strcmp(clientes[i].cpf, cpf_exclusao) == 0) {
            char confirmacao;
            printf("\nCliente encontrado. Deseja realmente excluir este cliente (S/N)? ");
            scanf(" %c", &confirmacao);
            while (getchar() != '\n');
            if (confirmacao == 'S' || confirmacao == 's') {
                for (int j = i; j < *total_clientes - 1; j++) {
                    clientes[j] = clientes[j + 1];
                }
                (*total_clientes)--;
                printf("\nCliente excluido com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nCliente com CPF %s nao encontrado.\n", cpf_exclusao);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void excluir_consulta(Consulta consultas[], int* total_consultas) {
    char cpf_exclusao[12];
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///        Excluir Consulta          ///\n");
    printf("----------------------------------------\n");
    printf("Informe o CPF do paciente cuja consulta deseja excluir: ");
    scanf("%s", cpf_exclusao);
    while (getchar() != '\n');

    for (int i = 0; i < *total_consultas; i++) {
        if (strcmp(consultas[i].cpf_paciente, cpf_exclusao) == 0) {
            char confirmacao;
            printf("\nConsulta encontrada. Deseja realmente excluir esta consulta (S/N)? ");
            scanf(" %c", &confirmacao);
            while (getchar() != '\n');
            if (confirmacao == 'S' || confirmacao == 's') {
                for (int j = i; j < *total_consultas - 1; j++) {
                    consultas[j] = consultas[j + 1];
                }
                (*total_consultas)--;
                printf("\nConsulta excluida com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nConsulta para o CPF %s nao encontrada.\n", cpf_exclusao);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void excluir_produto(Produto produtos[], int* total_produtos) {
    int id_exclusao;
    int encontrado = 0;
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///       Excluir Produto            ///\n");
    printf("----------------------------------------\n");
    printf("Informe o ID do produto que deseja excluir: ");
    scanf("%d", &id_exclusao);
    while (getchar() != '\n');

    for (int i = 0; i < *total_produtos; i++) {
        if (produtos[i].id == id_exclusao) {
            char confirmacao;
            printf("\nProduto encontrado. Deseja realmente excluir este produto (S/N)? ");
            scanf(" %c", &confirmacao);
            while (getchar() != '\n');
            if (confirmacao == 'S' || confirmacao == 's') {
                for (int j = i; j < *total_produtos - 1; j++) {
                    produtos[j] = produtos[j + 1];
                }
                (*total_produtos)--;
                printf("\nProduto excluido com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nProduto com ID %d nao encontrado.\n", id_exclusao);
    }
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

void modulo_clientes(Cliente clientes[], int* total_clientes) { 
    int opcao_clientes;
    do {
        TelaMenuClientes();
        if (scanf("%d", &opcao_clientes) != 1) {
            opcao_clientes = -1; 
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n'); 
        }

        switch (opcao_clientes) {
            case 1:
                TelaCadastrarClientes();
                printf("CPF: ");
                scanf("%s", clientes[*total_clientes].cpf);
                while (getchar() != '\n');
                printf("Nome: ");
                scanf(" %49[^\n]", clientes[*total_clientes].nome);
                while (getchar() != '\n');
                printf("Telefone: ");
                scanf("%s", clientes[*total_clientes].telefone);
                while (getchar() != '\n');
                printf("E-mail: ");
                scanf("%s", clientes[*total_clientes].email);
                while (getchar() != '\n');
                
                (*total_clientes)++; 
                printf("\nCliente cadastrado com sucesso! Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 2:
                pesquisar_cliente(clientes, *total_clientes);
                break;
            case 3:
                TelaAlterarClientes();
                printf("Módulo de Alterar Clientes em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 4:
                excluir_cliente(clientes, total_clientes);
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_clientes != 0);
}

void modulo_consultas(Consulta consultas[], int* total_consultas) { 
    int opcao_consultas;
    do {
        TelaMenuConsultas();
        if (scanf("%d", &opcao_consultas) != 1) {
            opcao_consultas = -1; 
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n'); 
        }

        switch (opcao_consultas) {
            case 1:
                TelaAgendarNovaConsulta();
                printf("Nome do Paciente: ");
                scanf(" %49[^\n]", consultas[*total_consultas].nome_paciente);
                while (getchar() != '\n');
                printf("CPF do Paciente: ");
                scanf("%s", consultas[*total_consultas].cpf_paciente);
                while (getchar() != '\n');
                printf("Data (dd/mm/aaaa): ");
                scanf("%s", consultas[*total_consultas].data);
                while (getchar() != '\n');
                printf("Hora (hh:mm): ");
                scanf("%s", consultas[*total_consultas].hora);
                while (getchar() != '\n');
                printf("Nome do Medico: ");
                scanf(" %49[^\n]", consultas[*total_consultas].nome_medico);
                while (getchar() != '\n');
                printf("Especialidade: ");
                scanf(" %49[^\n]", consultas[*total_consultas].especialidade);
                while (getchar() != '\n');

                (*total_consultas)++;
                printf("\nConsulta agendada com sucesso! Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 2:
                pesquisar_consulta(consultas, *total_consultas);
                break;
            case 3:
                TelaGerenciarAgendamentos();
                printf("Módulo de Gerenciar Agendamentos em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 4:
                TelaGerarRelatorios();
                printf("Módulo de Gerar Relatórios em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_consultas != 0);
}

void modulo_estoque(Produto produtos[], int* total_produtos) {
    int opcao_estoque;
    do {
        TelaMenuEstoque();
        if (scanf("%d", &opcao_estoque) != 1) {
            opcao_estoque = -1; 
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n');
        }
        
        switch (opcao_estoque) {
            case 1:
                TelaCadastrarProduto();
                printf("ID do Produto: ");
                scanf("%d", &produtos[*total_produtos].id);
                while (getchar() != '\n');
                printf("Nome do Produto: ");
                scanf(" %49[^\n]", produtos[*total_produtos].nome);
                while (getchar() != '\n');
                printf("Quantidade: ");
                scanf("%d", &produtos[*total_produtos].quantidade);
                while (getchar() != '\n');
                printf("Data de Validade: ");
                scanf("%s", produtos[*total_produtos].validade);
                while (getchar() != '\n');
                
                (*total_produtos)++;
                printf("\nProduto cadastrado com sucesso! Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 2:
                pesquisar_produto(produtos, *total_produtos);
                break;
            case 3:
                TelaGerenciarLotes();
                printf("Módulo de Gerenciar Lotes em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 4:
                TelaMovimentarEstoque();
                printf("Módulo de Movimentar Estoque em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 5:
                TelaGerarRelatoriosEstoque();
                printf("Módulo de Gerar Relatórios em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_estoque != 0);
}

int main() {
    int opcao_principal;
    Cliente clientes[100]; 
    int total_clientes = ler_clientes(clientes);

    Produto produtos[100];
    int total_produtos = ler_produtos(produtos);

    Consulta consultas[100];
    int total_consultas = ler_consultas(consultas);

    do {
        TelaMenuPrincipal();
        if (scanf("%d", &opcao_principal) != 1) {
            opcao_principal = -1; 
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n'); 
        }
        
        switch (opcao_principal) {
            case 1:
                modulo_clientes(clientes, &total_clientes);
                break;
            case 2:
                modulo_funcionarios();
                break;
            case 3:
                modulo_consultas(consultas, &total_consultas);
                break;
            case 4:
                modulo_estoque(produtos, &total_produtos);
                break;
            case 0:
                salvar_clientes(clientes, total_clientes); 
                salvar_produtos(produtos, total_produtos);
                salvar_consultas(consultas, total_consultas);
                finalizar_programa();
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_principal != 0);

    return 0;
}