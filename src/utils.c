#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include "utils.h"

void criar_pasta_data(void) {
    #ifdef _WIN32
        #include <direct.h>
        _mkdir("data");
    #else
        mkdir("data", 0755);
    #endif
}

void desenhar_cabecalho_base(void) {
    printf("///////////////////////////////////////////////////////////////////////////////\n");
    printf("///                                                                         ///\n");
    printf("///            Universidade Federal do Rio Grande do Norte (UFRN)           ///\n");
    printf("///            Centro de Ensino Superior do Serido (CERES)                  ///\n");
    printf("///            Departamento de Computacao e Tecnologia (DCT)                ///\n");
    printf("///              Disciplina DCT1106 -- Programacao                          ///\n");
    printf("///                                                                         ///\n");
    printf("///      Projeto SigMedical :: Sistema de Gestao de Clinicas Medicas        ///\n");
    printf("///      Developed by Maria Luiza Severo and Thiago Gomes de Oliveira       ///\n");
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

void press_enter_to_continue(void) {
    printf("Pressione ENTER para continuar...");
    while(getchar() != '\n'); 
    getchar(); 
}

void TelaMenuPrincipal() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          MENU PRINCIPAL          ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Modulo Clientes          ///\n");
    printf("///      2. Modulo Funcionarios      ///\n");
    printf("///      3. Modulo Consultas         ///\n");
    printf("///      4. Modulo Estoque           ///\n");
    printf("///      0. Sair                     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaMenuFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Modulo Funcionarios       ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   1. Cadastrar Funcionarios      ///\n");
    printf("///   2. Pesquisar Funcionarios      ///\n");
    printf("///   3. Alterar Funcionarios        ///\n");
    printf("///   4. Excluir Funcionarios        ///\n");
    printf("///   0. Voltar ao Menu Principal    ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaMenuClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          Modulo Clientes         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   1. Cadastrar Clientes          ///\n");
    printf("///   2. Pesquisar Clientes          ///\n");
    printf("///   3. Alterar Clientes            ///\n");
    printf("///   4. Excluir Clientes            ///\n");
    printf("///   0. Voltar ao Menu Principal    ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaMenuConsultas() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Modulo Consultas          ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   1. Agendar Nova Consulta       ///\n");
    printf("///   2. Pesquisar Consultas         ///\n");
    printf("///   3. Gerenciar Agendamentos      ///\n");
    printf("///   4. Gerar Relatorios            ///\n");
    printf("///   0. Voltar ao Menu Principal    ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaMenuEstoque() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///          Modulo Estoque          ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   1. Cadastrar Produtos          ///\n");
    printf("///   2. Pesquisar Produtos          ///\n");
    printf("///   3. Gerenciar Lotes             ///\n");
    printf("///   4. Movimentar Estoque          ///\n");
    printf("///   5. Gerar Relatorios Estoque    ///\n");
    printf("///   0. Voltar ao Menu Principal    ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaCadastrarFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Cadastrar Funcionario      ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   CPF:                           ///\n");
    printf("///   Nome:                          ///\n");
    printf("///   Telefone:                      ///\n");
    printf("///   E-mail:                        ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Pesquisar Funcionario      ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   Informe o CPF do funcionario:  ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaAlterarFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Alterar Funcionario      ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   Informe o CPF do funcionario:  ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaExcluirFuncionarios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Excluir Funcionario      ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   Informe o CPF do funcionario:  ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaConfirmarExclusao() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Confirmar Exclusao?        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Sim (S/s)                ///\n");
    printf("///      2. Nao (N/n)                ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaCadastrarClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Cadastrar Cliente        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   CPF:                           ///\n");
    printf("///   Nome:                          ///\n");
    printf("///   Telefone:                      ///\n");
    printf("///   E-mail:                        ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Pesquisar Cliente        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   Informe o CPF do cliente:      ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaAlterarClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///           Alterar Cliente        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   Informe o CPF do cliente:      ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaExcluirClientes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///           Excluir Cliente        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   Informe o CPF do cliente:      ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaConfirmarExclusaoCliente() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Confirmar Exclusao?       ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Sim (S/s)                ///\n");
    printf("///      2. Nao (N/n)                ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaAgendarNovaConsulta() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Agendar Nova Consulta      ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      Nome do Paciente:           ///\n");
    printf("///      CPF do Paciente:            ///\n");
    printf("///      Data (dd/mm/aaaa):          ///\n");
    printf("///      Hora (hh:mm):               ///\n");
    printf("///      Nome do Medico:             ///\n");
    printf("///      Especialidade:              ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaGerenciarAgendamentos() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///       Gerenciar Agendamentos     ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Alterar Consulta Agendada ///\n");
    printf("///      2. Cancelar Consulta        ///\n");
    printf("///      3. Confirmar Presenca       ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaGerarRelatorios() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Gerar Relatorios         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Consultas por Medico     ///\n");
    printf("///      2. Consultas por Periodo    ///\n");
    printf("///      3. Consultas Agendadas      ///\n");
    printf("///      4. Consultas Canceladas     ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaCadastrarProduto() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Cadastrar Produto        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      ID do Produto:              ///\n");
    printf("///      Nome do Produto:            ///\n");
    printf("///      Quantidade:                 ///\n");
    printf("///      Data de Validade:           ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarProduto() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Pesquisar Produto        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Pesquisar por ID         ///\n");
    printf("///      2. Pesquisar por Nome       ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaGerenciarLotes() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Gerenciar Lotes          ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Visualizar Lotes         ///\n");
    printf("///      2. Adicionar Lote           ///\n");
    printf("///      3. Remover Lote             ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaMovimentarEstoque() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///        Movimentar Estoque        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Saida de Material        ///\n");
    printf("///      2. Entrada de Material      ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaGerarRelatoriosEstoque() {
    system("cls");
    printf("----------------------------------------\n");
    printf("///         Gerar Relatorios         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Itens em Falta           ///\n");
    printf("///      2. Itens com Validade Proxima ///\n");
    printf("///      3. Historico de Movimentacoes ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaGerarRelatoriosMovimentacoes(void) {
    system("cls");
    printf("----------------------------------------\n");
    printf("///   Gerar Relatorios Movimentacoes   ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   1. Relatorio por Tipo          ///\n");
    printf("///   2. Relatorio por Produto       ///\n");
    printf("///   0. Voltar                      ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaMenuMovimentacao(void) {
    system("cls");
    printf("----------------------------------------\n");
    printf("///      Modulo de Movimentacao      ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///   1. Registrar Movimentacao      ///\n");
    printf("///   2. Gerar Relatorios            ///\n");
    printf("///   0. Voltar ao Menu Principal    ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}


void maiuscula(char *texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper(texto[i]);
    }
}


void finalizar_programa() {
    system("cls");
    printf("\n\n");
    printf("////////////////////////////////////////////////////////////////\n");
    printf("///                                                          ///\n");
    printf("///       Programa finalizado. Volte sempre!                 ///\n");
    printf("///                                                          ///\n");
    printf("////////////////////////////////////////////////////////////////\n");
    printf("\n");
    printf("      Pressione ENTER para sair...");
    
    getchar();
}