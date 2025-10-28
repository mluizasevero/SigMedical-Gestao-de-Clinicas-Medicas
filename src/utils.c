
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Adicionado para usar strlen/strtok/etc. se necessário
#include <sys/stat.h>
#include <ctype.h>
#include "utils.h"

#ifdef _WIN32
    #include <direct.h>
#endif




static void limpar_buffer(void) {
    int c;
    // Continua lendo e descartando caracteres até encontrar \n ou EOF
    while ((c = getchar()) != '\n' && c != EOF);
}



// NOTA: Esta função substitui todos os 'scanf(" %49[^\n]", ...)' e 'while(getchar() != '\n')'.

char* lerString(char *str, int size) {
    if (str == NULL || size <= 0) {
        return NULL;
    }

    // Leitura com fgets (segura contra buffer overflow)
    if (fgets(str, size, stdin) == NULL) {
        return NULL; // Erro de leitura
    }

    // Procura e remove o '\n' do final
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    } else {
        // Se o '\n' não foi encontrado, a entrada foi maior que 'size - 1'.
        // Limpar o restante do buffer para evitar interferência na próxima leitura.
        limpar_buffer();
    }
    
    return str;
}


void criar_pasta_data(void) {
    #ifdef _WIN32
        _mkdir("data");
    #else
        // Use 'mkdir' com permissões (0755)
        mkdir("data", 0755); 
    #endif
}

void limpar_tela(void) {
    system("clear||cls");
}

void press_enter_to_continue(void) {
    printf("Pressione ENTER para continuar...");
    // MUDANÇA: Substitui 'while(getchar() != '\n');' por 'limpar_buffer()' 
    // e um único getchar para esperar o comando.
    limpar_buffer(); 
    if (getchar() != EOF) {
        // Espera o ENTER e não faz nada, apenas continua
    }
}

void maiuscula(char *texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = (char)toupper((unsigned char)texto[i]);
    }
}

void desenhar_cabecalho_base(void) {
    printf("///////////////////////////////////////////////////////////////////////////////\n");
    printf("///                                                                         ///\n");
    printf("///             Universidade Federal do Rio Grande do Norte (UFRN)          ///\n");
    printf("///               Centro de Ensino Superior do Serido (CERES)               ///\n");
    printf("///             Departamento de Computacao e Tecnologia (DCT)               ///\n");
    printf("///                 Disciplina DCT1106 -- Programacao                       ///\n");
    printf("///                                                                         ///\n");
    printf("///       Projeto SigMedical :: Sistema de Gestao de Clinicas Medicas       ///\n");
    printf("///                                                                         ///\n");
}

void tela_equipe(void) {
    limpar_tela();
    const char* info =
        "///          Este projeto foi desenvolvido por:                               ///\n"
        "///                                                                         ///\n"
        "///          Maria Luiza Severo                                               ///\n"
        "///          E-mail: maria.severo.063@ufrn.edu.br                             ///\n"
        "///                                                                         ///\n"
        "///          Marcus Vinicius de Souza Azevedo                                 ///\n"
        "///          Email: infobasicifrn2017marcusv@gmail.com                        ///\n"
        "///                                                                         ///\n"
        "///          Thiago Gomes de Oliveira                                         ///\n"
        "///          E-mail: Thiago.oliveira712@ufrn.edu.br                           ///";

    desenhar_cabecalho_base();
    printf("%s\n", info);
    printf("///////////////////////////////////////////////////////////////////////////////\n");
}


void TelaMenuPrincipal(void) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///          MENU PRINCIPAL          ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  1. Modulo Clientes              ///\n");
    printf("///  2. Modulo Medicos               ///\n");
    printf("///  3. Modulo Consultas             ///\n");
    printf("///  4. Modulo Estoque               ///\n");
    printf("///  5. Sobre a Equipe               ///\n");
    printf("///  0. Sair                         ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}


void TelaMenuMedicos(void) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Modulo de Medicos        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  1. Cadastrar Medico             ///\n");
    printf("///  2. Pesquisar Medico             ///\n");
    printf("///  3. Alterar Medico               ///\n");
    printf("///  4. Excluir Medico               ///\n");
    printf("///  5. Listar Medicos               ///\n");
    printf("///  0. Voltar ao Menu Principal     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}



void TelaMenuClientes(void) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Modulo de Clientes       ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  1. Cadastrar Cliente            ///\n");
    printf("///  2. Pesquisar Cliente            ///\n");
    printf("///  3. Alterar Cliente              ///\n");
    printf("///  4. Excluir Cliente              ///\n");
    printf("///  5. Listar Clientes              ///\n");
    printf("///  0. Voltar ao Menu Principal     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}


void TelaMenuConsultas(void) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///        Modulo de Consultas       ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  1. Agendar Nova Consulta        ///\n");
    printf("///  2. Pesquisar Consultas          ///\n");
    printf("///  3. Gerenciar Agendamentos       ///\n");
    printf("///  4. Gerar Relatorios             ///\n");
    printf("///  0. Voltar ao Menu Principal     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}

void TelaMenuEstoque(void) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Modulo de Estoque        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  1. Cadastrar Produto            ///\n");
    printf("///  2. Pesquisar Produto            ///\n");
    printf("///  3. Listar Produtos              ///\n");
    printf("///  4. Movimentar Estoque           ///\n");
    printf("///  5. Gerar Relatorios             ///\n");
    printf("///  0. Voltar ao Menu Principal     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opcao desejada: ");
}


void TelaCadastrarMedicos() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Cadastrar Médico         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  ID do Médico:                   ///\n");
    printf("///  Nome completo:                  ///\n");
    printf("///  CPF (apenas numeros):           ///\n");
    printf("///  Especialidade:                  ///\n");
    printf("///  Telefone:                       ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarMedicos() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Pesquisar Médico         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  Informe o CPF do Médico:        ///\n"); 
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}


void TelaAlterarMedicos() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///          Alterar Médico          ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  Informe o CPF do Médico:        ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}


void TelaExcluirMedicos() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///          Excluir Médico          ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  Informe o CPF do Médico:        ///\n"); 
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}


void TelaConfirmarExclusao() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///      Confirmar Exclusão?         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///     1. Sim (S/s)                 ///\n");
    printf("///     2. Nao (N/n)                 ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaCadastrarClientes() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Cadastrar Cliente        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  CPF:                            ///\n");
    printf("///  Nome:                           ///\n");
    printf("///  Telefone:                       ///\n");
    printf("///  E-mail:                         ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaPesquisarClientes() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Pesquisar Cliente        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  Informe o CPF do cliente:       ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaAlterarClientes() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///          Alterar Cliente         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  Informe o CPF do cliente:       ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaExcluirClientes() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///          Excluir Cliente         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  Informe o CPF do cliente:       ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaConfirmarExclusaoCliente() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///      Confirmar Exclusão?         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///     1. Sim (S/s)                 ///\n");
    printf("///     2. Nao (N/n)                 ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaAgendarNovaConsulta() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///      Agendar Nova Consulta       ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      Nome do Paciente:           ///\n");
    printf("///      CPF do Paciente:            ///\n");
    printf("///      Data (dd/mm/aaaa):          ///\n");
    printf("///      Hora (hh:mm):               ///\n");
    printf("///      Nome do Médico:             ///\n");
    printf("///      Especialidade:              ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf("\n");
}

void TelaGerenciarAgendamentos() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///      Gerenciar Agendamentos      ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///    1. Alterar Consulta Agendada  ///\n");
    printf("///    2. Cancelar Consulta          ///\n");
    printf("///    3. Confirmar Presenca         ///\n");
    printf("///    0. Voltar                     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaGerarRelatorios() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Gerar Relatórios         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Consultas por Médico     ///\n");
    printf("///      2. Consultas por Período    ///\n");
    printf("///      3. Consultas Agendadas      ///\n");
    printf("///      4. Consultas Canceladas     ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaCadastrarProduto() {
    limpar_tela();
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
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Pesquisar Produto        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Pesquisar por ID         ///\n");
    printf("///      2. Pesquisar por Nome       ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaGerenciarLotes() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///          Gerenciar Lotes         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Visualizar Lotes         ///\n");
    printf("///      2. Adicionar Lote           ///\n");
    printf("///      3. Remover Lote             ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaMovimentarEstoque() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///        Movimentar Estoque        ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///      1. Saida de Material        ///\n");
    printf("///      2. Entrada de Material      ///\n");
    printf("///      0. Voltar                   ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaGerarRelatoriosEstoque() {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///         Gerar Relatórios         ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///    1. Itens em Falta             ///\n");
    printf("///    2. Itens com Validade Próxima ///\n");
    printf("///    3. Histórico de Movimentações ///\n");
    printf("///    0. Voltar                     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaGerarRelatoriosMovimentacoes(void) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("/// Gerar Relatórios Movimentações   ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  1. Relatorio por Tipo           ///\n");
    printf("///  2. Relatorio por Produto        ///\n");
    printf("///  0. Voltar                       ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void TelaMenuMovimentacao(void) {
    limpar_tela();
    printf("----------------------------------------\n");
    printf("///     Módulo de Movimentação       ///\n");
    printf("----------------------------------------\n");
    printf("///                                  ///\n");
    printf("///  1. Registrar Movimentação       ///\n");
    printf("///  2. Gerar Relatórios             ///\n");
    printf("///  0. Voltar ao Menu Principal     ///\n");
    printf("///                                  ///\n");
    printf("----------------------------------------\n");
    printf(">>> Escolha a opção desejada: ");
}

void maiuscula(char *texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper(texto[i]);
    }
}


void finalizar_programa() {
    limpar_tela();
    printf("\n\n");
    printf("////////////////////////////////////////////////////////////////\n");
    printf("///                                                            ///\n");
    printf("///             Programa finalizado. Volte sempre!             ///\n");
    printf("///                                                            ///\n");
    printf("////////////////////////////////////////////////////////////////\n");
    printf("\n");
}


