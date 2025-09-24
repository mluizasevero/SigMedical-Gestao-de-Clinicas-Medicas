#ifndef UTILS_H
#define UTILS_H

void criar_pasta_data(void);

void TelaMenuPrincipal();
void TelaCadastrarClientes();
void TelaPesquisarClientes();
void TelaAlterarClientes();
void TelaExcluirClientes();
void TelaMenuClientes();
void TelaMenuFuncionarios();
void TelaCadastrarFuncionarios();
void TelaPesquisarFuncionarios();
void TelaAlterarFuncionarios();
void TelaExcluirFuncionarios();

void desenhar_cabecalho_base(void);
void tela_equipe(void);
void limpar_tela(void);
void finalizar_programa(void);
void TelaMenuConsultas(void);
void TelaMenuEstoque(void);
void TelaConfirmarExclusao(void);
void TelaConfirmarExclusaoCliente(void);
void TelaAgendarNovaConsulta(void);
void TelaGerenciarAgendamentos(void);
void TelaGerarRelatorios(void);
void TelaCadastrarProduto(void);
void TelaPesquisarProduto(void);
void TelaGerenciarLotes(void);
void TelaMovimentarEstoque(void);
void TelaGerarRelatoriosEstoque(void);
void press_enter_to_continue(void);
void TelaGerarRelatoriosMovimentacoes(void); // <-- CORRIGIDO AQUI
void TelaMenuMovimentacao(void);
void maiuscula(char *texto);

#endif
