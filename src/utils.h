#ifndef UTILS_H
#define UTILS_H


void desenhar_cabecalho_base(void);
void tela_equipe(void);
void criar_pasta_data(void);


void limpar_tela(void);
void press_enter_to_continue(void);
void maiuscula(char *texto);
void finalizar_programa(void);


void TelaMenuPrincipal(void);
void TelaMenuMedicos(void);
void TelaMenuClientes(void);
void TelaMenuConsultas(void);
void TelaMenuEstoque(void);
void TelaMenuMovimentacao(void);


void TelaCadastrarMedicos(void);
void TelaPesquisarMedicos(void);
void TelaAlterarMedicos(void);
void TelaExcluirMedicos(void);
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
void TelaGerarRelatoriosMovimentacoes(void);

#endif


// ======= CONFIGURAÇÃO DA PASTA DATA =======

#ifdef _WIN32
    #include <direct.h>
    #define PATH_SEPARATOR "\\"
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define PATH_SEPARATOR "/"
#endif

#define DATA_DIR "data"

// ==========================================