#include <stdio.h>
#include <stdlib.h>
#include "funcionarios.h"
#include "utils.h"


void cadastrar_funcionario(void) {
}

void pesquisar_funcionario(void) {
}

void alterar_funcionario(void) {
}

void excluir_funcionario(void) {
}


void modulo_funcionarios(void) {
    int opcao;
    do {
        TelaMenuFuncionarios();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                TelaCadastrarFuncionarios();
                printf("Ainda não implementado.\n");
                press_enter_to_continue();
                break;
            case 2:
                TelaPesquisarFuncionarios();
                printf("Ainda não implementado.\n");
                press_enter_to_continue();
                break;
            case 3:
                TelaAlterarFuncionarios();
                printf("Ainda não implementado.\n");
                press_enter_to_continue();
                break;
            case 4:
                TelaExcluirFuncionarios();
                printf("Ainda não implementado.\n");
                press_enter_to_continue();
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER...\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao != 0);
}