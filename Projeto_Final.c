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

void desenhar_cabecalho_base(void);
void tela_equipe(void);
void limpar_tela(void);

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

void finalizar_programa(void);

// Funções de Cabeçalho e Equipe
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

void modulo_clientes(void) {
    int opcao_clientes;
    do {
        TelaMenuClientes();
        scanf("%d", &opcao_clientes);
        while (getchar() != '\n'); 

        switch (opcao_clientes) {
            case 1:
                TelaCadastrarClientes();
                printf("Módulo de Cadastrar Clientes em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 2:
                TelaPesquisarClientes();
                printf("Módulo de Pesquisar Clientes em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 3:
                TelaAlterarClientes();
                printf("Módulo de Alterar Clientes em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 4:
                TelaExcluirClientes();
                printf("Módulo de Excluir Clientes em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 0:
                break;
            default:
                printf("\nOpção inválida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_clientes != 0);
}

void salvar_clientes(Cliente clientes[], int total_clientes);
int ler_clientes(Cliente clientes[]);

void modulo_consultas(void) {
    int opcao_consultas;
    do {
        TelaMenuConsultas();
        scanf("%d", &opcao_consultas);
        while (getchar() != '\n'); // Limpa o buffer do teclado

        switch (opcao_consultas) {
            case 1:
                TelaAgendarNovaConsulta();
                printf("Módulo de Agendar Nova Consulta em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 2:
                printf("Módulo de Pesquisar Consultas em desenvolvimento...\n");
                printf("Pressione ENTER para voltar...\n");
                getchar();
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
                printf("\nOpção inválida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_consultas != 0);
}

void modulo_estoque(void) {
    int opcao_estoque;
    do {
        TelaMenuEstoque();
        scanf("%d", &opcao_estoque);
        while (getchar() != '\n');
        
        switch (opcao_estoque) {
            case 1:
                TelaCadastrarProduto();
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 2:
                TelaPesquisarProduto();
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 3:
                TelaGerenciarLotes();
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 4:
                TelaMovimentarEstoque();
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 5:
                TelaGerarRelatoriosEstoque();
                printf("Pressione ENTER para voltar...\n");
                getchar();
                break;
            case 0:
                break;
            default:
                printf("\nOpção inválida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_estoque != 0);
}

int main() {
    int opcao_principal;
    do {
        TelaMenuPrincipal();
        scanf("%d", &opcao_principal);
        while (getchar() != '\n'); 
        
        switch (opcao_principal) {
            case 1:
                modulo_clientes();
                break;
            case 2:
                modulo_funcionarios();
                break;
            case 3:
                modulo_consultas();
                break;
            case 4:
                modulo_estoque();
                break;
            case 0:
                finalizar_programa();
                break;
            default:
                printf("\nOpção inválida. Pressione ENTER para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao_principal != 0);

    return 0;

}
