
#ifndef VALIDADORES_H
#define VALIDADORES_H

/**
 * Verifica se o usuário digitou "0" para cancelar a operação.
 * Retorna 1 se for cancelamento, 0 caso contrário.
 */
int verificarCancelamento(const char *entrada);

/**
 * Valida um CPF (formato e dígitos verificadores).
 * Retorna 1 se válido, 0 se inválido.
 */
int validarCPF(const char *cpf);

/**
 * Valida um nome (mínimo 3 caracteres, não pode conter números).
 * Retorna 1 se válido, 0 se inválido.
 */
int validarNome(const char *nome);

/**
 * Valida um telefone (deve ter 10 ou 11 dígitos).
 * Retorna 1 se válido, 0 se inválido.
 */
int validarTelefone(const char *telefone);

/**
 * Valida um email (formato básico: "local@dominio.com").
 * Retorna 1 se válido, 0 se inválido.
 */
int validarEmail(const char *email);

/**
 * Valida uma opção de menu (se está num intervalo).
 * Retorna 1 se válido, 0 se inválido.
 */
int validarOpcaoMenu(int opcao, int min, int max);

/**
 * Valida uma data no formato dd/mm/aaaa.
 * Verifica o formato e os intervalos básicos de dia/mês/ano.
 * Retorna 1 se válido, 0 se inválido.
 */
int validarData(const char *data);

/**
 * Valida uma hora no formato hh:mm.
 * Verifica o formato e os intervalos de hora/minuto.
 * Retorna 1 se válido, 0 se inválido.
 */
int validarHora(const char *hora);

/**
 * Valida se a string contem apenas digitos e representa um inteiro positivo.
 * Retorna o inteiro validado ou -1 se for inválido.
 */
int validarInteiroPositivo(const char *str);

#endif
