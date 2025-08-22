#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clientes.h"
#include "consultas.h"
#include "estoque.h"
#include "movimentacao.h"
#include "utils.h"
#include "funcionarios.h" 

int main() {
    int opcao_principal;
    Cliente clientes[100];
    int total_clientes = ler_clientes(clientes);

    Produto produtos[100];
    int total_produtos = ler_produtos(produtos);

    Consulta consultas[100];
    int total_consultas = ler_consultas(consultas);
    
    Movimentacao movimentacoes[1000];
    int total_movimentacoes = ler_movimentacoes(movimentacoes);

    do {
        TelaMenuPrincipal();
        if (scanf("%d", &opcao_principal) != 1) {
            opcao_principal = -1;
            while (getchar() != '\n');
           } else {
            while (getchar() != '\n');
           }

            switch (opcao_principal) {
            case 1:
                modulo_clientes(clientes, &total_clientes);
                break;
            case 2:
                modulo_funcionarios();
                break;
            case 3:
                modulo_consultas(consultas, &total_consultas);
                break;
            case 4:
                modulo_estoque(produtos, &total_produtos, movimentacoes, &total_movimentacoes);
                break;
            case 0:
                salvar_clientes(clientes, total_clientes);
                salvar_produtos(produtos, total_produtos);
                salvar_consultas(consultas, total_consultas);
                salvar_movimentacoes(movimentacoes, total_movimentacoes);
                finalizar_programa();
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para continuar.\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao_principal != 0);

    return 0;
}
