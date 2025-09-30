#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "src/clientes.h"
#include "src/medicos.h"
#include "src/consultas.h"
#include "src/estoque.h"
#include "src/movimentacao.h"
#include "src/utils.h"

int data_existe(const char *caminho) {
    // A estrutura struct stat é definida na biblioteca <sys/stat.h> e é usada para armazenar informações sobre um arquivo ou diretório
    struct stat stats;
    // A função stat verifica as propriedades do caminho especificado em caminho e preenche a estrutura stats com essas informações.
    // Ela retorna 0 se o caminho existe (seja um arquivo, diretório, ou outro tipo de entrada no sistema de arquivos) 
    // e um valor negativo (geralmente -1) se o caminho não existe ou houve um erro (ex.: falta de permissão).
    // O segundo argumento, &stats, é um ponteiro para a estrutura stats, onde as informações serão armazenadas.
    return stat(caminho, &stats) == 0 && S_ISDIR(stats.st_mode);
    // stat(caminho, &stats) == 0: Verifica se o caminho existe (a chamada a stat foi bem-sucedida).
    // S_ISDIR(stats.st_mode): Verifica se o caminho é um diretório. A macro S_ISDIR testa o campo st_mode 
    // da estrutura stats, que indica o tipo do item (diretório, arquivo, link simbólico, etc.).
}

int main(void) { 
    

    Cliente      todos_clientes[100];
    Medico       todos_medicos[100]; 
    Consulta     todas_consultas[100];
    Produto      todos_produtos[100];
    Movimentacao todas_movimentacoes[1000];

    int total_clientes      = 0;
    int total_medicos       = 0; 
    int total_consultas     = 0;
    int total_produtos      = 0;
    int total_movimentacoes = 0;
    
    if (data_existe("./data")) {
        total_clientes      = ler_clientes(todos_clientes);
        total_medicos       = ler_medicos(todos_medicos); 
        total_consultas     = ler_consultas(todas_consultas);
        total_produtos      = ler_produtos(todos_produtos);
        total_movimentacoes = ler_movimentacoes(todas_movimentacoes);

        printf("Dados carregados com sucesso!\n");
    } else {
        printf("Pasta '/data' nao encontrada. Iniciando com dados vazios.\n");
    }
    
    press_enter_to_continue(); 

    int opcao_principal;
    do {
        TelaMenuPrincipal(); 
        

        scanf("%d", &opcao_principal);
        while (getchar() != '\n'); 

        switch (opcao_principal) {
            case 1:
                modulo_clientes(todos_clientes, &total_clientes);
                break;
            case 2:
                modulo_medicos(todos_medicos, &total_medicos);
                break;
            case 3:
                modulo_consultas(todas_consultas, &total_consultas);
                break;
            case 4:
                modulo_estoque(todos_produtos, &total_produtos, todas_movimentacoes, &total_movimentacoes);
                break;
            case 5:
                tela_equipe();
                press_enter_to_continue();
                break;
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao_principal != 0);

    printf("\nSalvando todas as alteracoes nos arquivos. Por favor, aguarde...\n");
    salvar_clientes(todos_clientes, total_clientes);
    salvar_medicos(todos_medicos, total_medicos); 
    salvar_consultas(todas_consultas, total_consultas);
    salvar_produtos(todos_produtos, total_produtos);
    salvar_movimentacoes(todas_movimentacoes, total_movimentacoes);
    
    finalizar_programa();
    return 0;
}
