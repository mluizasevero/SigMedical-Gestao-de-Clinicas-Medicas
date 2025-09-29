#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/clientes.h"
#include "src/medicos.h"
#include "src/consultas.h"
#include "src/estoque.h"
#include "src/movimentacao.h"
#include "src/utils.h"

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
    
    total_clientes      = ler_clientes(todos_clientes);
    total_medicos       = ler_medicos(todos_medicos); 
    total_consultas     = ler_consultas(todas_consultas);
    total_produtos      = ler_produtos(todos_produtos);
    total_movimentacoes = ler_movimentacoes(todas_movimentacoes);
    
    printf("Dados carregados com sucesso!\n");
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
