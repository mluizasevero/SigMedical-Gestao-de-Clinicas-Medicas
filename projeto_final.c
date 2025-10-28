#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Adicionado para usar strncpy/strlen/etc.
#include "src/clientes.h"
#include "src/medicos.h"
#include "src/consultas.h"
#include "src/estoque.h"
#include "src/utils.h"
#include "src/validador.h" 

#define MAX_OPCAO 10 // Buffer suficiente para a opção do menu (1 dígito + '\n' + '\0')


int main(void) {
    
    criar_pasta_data();

    int opcao_principal;
    char input[MAX_OPCAO]; // Buffer para leitura segura da opção

    // estrutura de navegação intacta
    do {
        TelaMenuPrincipal();

        // SUBSTITUIÇÃO: Leitura segura para a opção do menu
        // 1. Lemos a entrada como string para garantir que o buffer seja limpo.
        lerString(input, MAX_OPCAO); 

        // 2. Tentamos converter a string para inteiro.
        // Se a string contiver apenas um único dígito válido, esta é uma conversão segura.
        if (sscanf(input, "%d", &opcao_principal) != 1) {
            // Se a conversão falhar (ex: usuário digitou "abc"), definimos como inválida.
            opcao_principal = -1;
        }


        switch (opcao_principal) {
            case 1:
                modulo_clientes();
                break;
            case 2:
                modulo_medicos();
                break;
            case 3:
                modulo_consultas();
                break;
            case 4:
                modulo_estoque();
                break;
            case 5:
                tela_equipe();
                press_enter_to_continue();
                break;
            case 0:
                break;
            default:
                printf("\nOpcao invalida. Pressione ENTER para tentar novamente.\n");
                press_enter_to_continue();
                break;
        }
    } while (opcao_principal != 0);

    finalizar_programa();
    return 0;
}


