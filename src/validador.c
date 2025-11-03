#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Valida um CPF (formato e dígitos verificadores).
 */
int validarCPF(const char *cpf)
{
    char cpfLimpo[12];
    int j = 0;

    // 1. Limpa o CPF (remove pontos e hifens) e verifica o formato
    for (int i = 0; cpf[i] != '\0'; i++)
    {
        if (isdigit(cpf[i]))
        {
            if (j < 11)
            { // Garante que não estoure o buffer cpfLimpo
                cpfLimpo[j++] = cpf[i];
            }
        }
        else if (cpf[i] != '.' && cpf[i] != '-')
        {
            // Se não for dígito, ponto ou hífen, é inválido
            return 0;
        }
    }
    cpfLimpo[j] = '\0';

    // 2. Verifica o tamanho
    if (strlen(cpfLimpo) != 11)
    {
        return 0;
    }

    // 3. Verifica dígitos repetidos (ex: 111.111.111-11)
    int todosIguais = 1;
    for (int i = 1; i < 11; i++)
    {
        if (cpfLimpo[i] != cpfLimpo[0])
        {
            todosIguais = 0;
            break;
        }
    }
    if (todosIguais)
    {
        return 0;
    }

    // 4. Calcula dígitos verificadores
    int soma, resto, digito1, digito2;

    // Cálculo do primeiro dígito
    soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (cpfLimpo[i] - '0') * (10 - i);
    }
    resto = 11 - (soma % 11);
    digito1 = (resto >= 10) ? 0 : resto;

    // Cálculo do segundo dígito
    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (cpfLimpo[i] - '0') * (11 - i);
    }
    resto = 11 - (soma % 11);
    digito2 = (resto >= 10) ? 0 : resto;

    // 5. Compara com os dígitos informados
    if ((digito1 == (cpfLimpo[9] - '0')) && (digito2 == (cpfLimpo[10] - '0')))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Valida um nome (mínimo 3 caracteres, não pode conter números).
 */
int validarNome(const char *nome)
{
    int len = strlen(nome);
    if (len < 3)
    {
        printf("! Erro: Nome muito curto (minimo 3 caracteres).\n");
        return 0;
    }

    for (int i = 0; i < len; i++)
    {
        if (isdigit(nome[i]))
        {
            printf("! Erro: Nome nao pode conter numeros.\n");
            return 0;
        }
    }
    return 1;
}

/**
 * Valida um telefone (deve ter 10 ou 11 dígitos).
 */
int validarTelefone(const char *telefone)
{
    char telLimpo[12];
    int j = 0;

    // Limpa o telefone (remove parênteses, espaços e hífens)
    for (int i = 0; telefone[i] != '\0'; i++)
    {
        if (isdigit(telefone[i]))
        {
            if (j < 11)
            { // Proteção de buffer
                telLimpo[j++] = telefone[i];
            }
        }
    }
    telLimpo[j] = '\0';

    int len = strlen(telLimpo);
    // Deve ter 10 (fixo) ou 11 (celular) dígitos
    if (len == 10 || len == 11)
    {
        return 1;
    }

    printf("! Erro: Telefone deve ter 10 ou 11 digitos (com DDD).\n");
    return 0;
}

/**
 * Valida um email (formato básico: "local@dominio.com").
 * Esta é uma validação simples, não uma regex completa.
 */
int validarEmail(const char *email)
{
    const char *at = strchr(email, '@'); // Encontra o '@'

    // Se não houver '@', é inválido
    if (at == NULL)
    {
        printf("! Erro: Email deve conter um '@'.\n");
        return 0;
    }

    // Se o '@' for o primeiro caractere, é inválido
    if (at == email)
    {
        printf("! Erro: Email nao pode comecar com '@'.\n");
        return 0;
    }

    // Encontra o último '.' DEPOIS do '@'
    const char *dot = strrchr(at, '.');

    // Se não houver '.' depois do '@', ou for o último caractere, é inválido
    if (dot == NULL || dot == at + 1 || dot[1] == '\0')
    {
        printf("! Erro: Email deve ter um dominio valido (ex: .com, .br).\n");
        return 0;
    }

    return 1;
}

/**
 * Valida uma opção de menu (se está num intervalo).
 */
int validarOpcaoMenu(int opcao, int min, int max)
{
    if (opcao >= min && opcao <= max)
    {
        return 1;
    }
    printf("\n! Opcao invalida. Digite um numero entre %d e %d.\n", min, max);
    return 0;
}

/**
 * Valida uma data no formato dd/mm/aaaa.
 */
int validarData(const char *data)
{
    if (strlen(data) != 10)
    {
        printf("! Erro: A data deve ter o formato dd/mm/aaaa (10 caracteres).\n");
        return 0;
    }

    // Verifica o formato (barras)
    if (data[2] != '/' || data[5] != '/')
    {
        printf("! Erro: Formato incorreto. Use dd/mm/aaaa.\n");
        return 0;
    }

    int dia, mes, ano;
    // sscanf lê de uma string, %*c ignora o caractere (a barra)
    if (sscanf(data, "%d%*c%d%*c%d", &dia, &mes, &ano) != 3)
    {
        printf("! Erro: Data deve conter apenas numeros e barras.\n");
        return 0;
    }

    // Validação de intervalos básicos (não checa dias de cada mês/anos bissextos)
    if (dia < 1 || dia > 31)
    {
        printf("! Erro: Dia invalido (1-31).\n");
        return 0;
    }
    if (mes < 1 || mes > 12)
    {
        printf("! Erro: Mes invalido (1-12).\n");
        return 0;
    }
    if (ano < 1900 || ano > 2100)
    { // Intervalo razoável
        printf("! Erro: Ano invalido (use 1900-2100).\n");
        return 0;
    }

    return 1;
}

/**
 * Valida uma hora no formato hh:mm.
 */
int validarHora(const char *hora)
{
    if (strlen(hora) != 5)
    {
        printf("! Erro: A hora deve ter o formato hh:mm (5 caracteres).\n");
        return 0;
    }

    // Verifica o formato (dois pontos)
    if (hora[2] != ':')
    {
        printf("! Erro: Formato incorreto. Use hh:mm.\n");
        return 0;
    }

    int hh, mm;
    if (sscanf(hora, "%d%*c%d", &hh, &mm) != 2)
    {
        printf("! Erro: Hora deve conter apenas numeros e dois pontos.\n");
        return 0;
    }

    // Validação de intervalos
    if (hh < 0 || hh > 23)
    {
        printf("! Erro: Hora invalida (00-23).\n");
        return 0;
    }
    if (mm < 0 || mm > 59)
    {
        printf("! Erro: Minuto invalido (00-59).\n");
        return 0;
    }

    return 1;
}

/**
 * Converte uma data "dd/mm/aaaa" para um int YYYYMMDD.
 */
long converterDataParaInt(const char *dataStr)
{
    int dia, mes, ano;
    if (sscanf(dataStr, "%d/%d/%d", &dia, &mes, &ano) == 3)
    {
        return (long)ano * 10000 + (long)mes * 100 + (long)dia;
    }
    return 0;
}

/**
 * Valida se a string contem apenas digitos e representa um inteiro positivo (e > 0).
 * Retorna o inteiro validado ou -1 se for inválido.
 */
int validarInteiroPositivo(const char *str)
{
    if (str == NULL || *str == '\0')
    {
        printf("! Erro: O valor nao pode ser vazio.\n");
        return -1;
    }

    // Verifica se a string contem apenas dígitos (e opcionalmente um sinal de +/-, que ignoramos para positivo)
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)str[i])) // "Cast" de segurança que protege de caracteres acentuados
        {
            if (i == 0 && (str[i] == '+' || str[i] == '-'))
            {
                // Permite sinal no início, mas checaremos o valor final
                continue;
            }
            printf("! Erro: Entrada deve conter apenas numeros inteiros.\n");
            return -1;
        }
    }

    // Tenta converter para inteiro
    // Recebe a string, um ponteiro para saber onde a leitura parou (Mas não precisamos disso) e a base numérica
    long num = strtol(str, NULL, 10);

    if (num <= 0)
    {
        printf("! Erro: O numero deve ser positivo (> 0).\n");
        return -1;
    }

    // Verifica se o número é maior que o limite de um int
    if (num > 2147483647)
    { // 2^31 - 1
        printf("! Erro: Numero muito grande.\n");
        return -1;
    }

    // Retorna o número convertendo de volta para int
    return (int)num;
}
