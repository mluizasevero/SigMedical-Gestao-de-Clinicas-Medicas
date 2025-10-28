#ifndef UTILS_H
#define UTILS_H


char* lerString(char *str, int size);



// FUNÇÕES UTILITÁRIAS EXISTENTES
void desenhar_cabecalho_base(void);
void tela_equipe(void);
void criar_pasta_data(void);


void limpar_tela(void);
void press_enter_to_continue(void);
void maiuscula(char *texto);
void finalizar_programa(void);



// FUNÇÕES DE TELA (MENUS)
void TelaMenuPrincipal(void);
void TelaMenuMedicos(void);
void TelaMenuClientes(void);
void TelaMenuConsultas(void);
void TelaMenuEstoque(void);
void TelaMenuMovimentacao(void);



// FUNÇÕES DE TELA (CADASTRO/CRUD)
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

