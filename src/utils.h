#ifndef UTILS_H
#define UTILS_H

// ======= CONFIGURAÇÃO DA PASTA DATA ======= //

#ifdef _WIN32
    #include <direct.h>
    #define PATH_SEPARATOR "\\"
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define PATH_SEPARATOR "/"
#endif

#define DATA_DIR "data"

// ========================================== //

// ========= FUNÇÕES DE TELA (MENUS) ========= //

// MENU PRINCIPAL
void TelaMenuPrincipal(void);

// CRUD MÉDICOS
void TelaMenuMedicos(void);
void TelaCadastrarMedicos(void);
void TelaPesquisarMedicos(void);
void TelaAlterarMedicos(void);
void TelaExcluirMedicos(void);

// CRUD CLIENTES
void TelaMenuClientes(void);
void TelaCadastrarClientes(void);
void TelaPesquisarClientes(void);
void TelaAlterarClientes(void);
void TelaExcluirClientes(void);
void TelaConfirmarExclusaoCliente(void);

// OPÇÕES CONSULTAS
void TelaMenuConsultas(void);
void TelaAgendarNovaConsulta(void);
void TelaGerenciarAgendamentos(void);

// OPÇÕES RELATÓRIOS
void TelaGerarRelatorios(void);
void TelaGerarRelatoriosEstoque(void);

// CRUD PRODUTOS
void TelaCadastrarProduto(void);
void TelaPesquisarProduto(void);
void TelaGerenciarLotes(void);
void TelaMovimentarEstoque(void);
void TelaMenuEstoque(void);
void TelaGerarRelatoriosMovimentacoes(void);
void TelaMenuMovimentacao(void);

// Outras Telas
void TelaConfirmarExclusao(void); // CORRIGIR
void desenharCabecalhoBase(void);
void finalizarPrograma(void);
void telaEquipe(void);

// ========================================== //

// ===== FUNÇÕES UTILITÁRIAS EXISTENTES ===== //

char *lerString(char *str, int size);
void pressioneEnterParaContinuar(void);
void criarPastaData(void);
void limparTela(void);
void transformarEmMaiusculas(char *texto);

// ========================================== //


#endif
