
#ifndef VALIDADORES_H
#define VALIDADORES_H

/**
 * Lê uma string do terminal de forma segura, prevenindo overflow.
 * Substitui o combo scanf/getchar.
 *
 * @param buffer Onde armazenar a string lida.
 * @param tamanho O tamanho máximo do buffer (incluindo o \0).
 */
void lerString(char* buffer, int tamanho);

/**
 * Valida um CPF (formato e dígitos verificadores).
 * Retorna 1 se válido, 0 se inválido.
 */
int validarCPF(const char* cpf);

/**
 * Valida um nome (mínimo 3 caracteres, não pode conter números).
 * Retorna 1 se válido, 0 se inválido.
 */
int validarNome(const char* nome);

/**
 * Valida um telefone (deve ter 10 ou 11 dígitos).
 * Retorna 1 se válido, 0 se inválido.
 */
int validarTelefone(const char* telefone);

/**
 * Valida um email (formato básico: "local@dominio.com").
 * Retorna 1 se válido, 0 se inválido.
 */
int validarEmail(const char* email);

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
int validarData(const char* data);

/**
 * Valida uma hora no formato hh:mm.
 * Verifica o formato e os intervalos de hora/minuto.
 * Retorna 1 se válido, 0 se inválido.
 */
int validarHora(const char* hora);

/**
 * Converte uma data "dd/mm/aaaa" para um inteiro no formato YYYYMMDD.
 * Essencial para comparar datas corretamente (ex: relatórios por período).
 * Retorna um long int (ex: 20241231) ou 0 em caso de erro.
 */
long converterDataParaInt(const char* dataStr);


/**
 * Valida se a string contem apenas digitos e representa um inteiro positivo.
 * Retorna o inteiro validado ou -1 se for inválido.
 */
int validarInteiroPositivo(const char* str);


#endif
