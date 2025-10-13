
#include <stdio.h>
#include <stdlib.h>
#include "src/clientes.h"
#include "src/medicos.h"
#include "src/consultas.h"
#include "src/estoque.h"
#include "src/utils.h"

int main(void) {
    /*
     * ---------------------------------------------------------------------
     * PRIMEIRA PARTE REMOVIDA: Declaração de vetores na memória
     *
     * Cliente    todos_clientes[100];
     * int        total_clientes = 0;
     * ... etc ...
     *
     * MOTIVO: Com a nova regra, os dados não são mais guardados em vetores na
     * memória RAM. Eles residem permanentemente nos arquivos .dat.
     * ---------------------------------------------------------------------
     */


    /*
     * ---------------------------------------------------------------------
     * SEGUNDA PARTE REMOVIDA: Carregamento inicial dos dados
     *
     * if (data_existe("./data")) {
     * total_clientes = ler_clientes(todos_clientes);
     * ... etc ...
     * }
     *
     * MOTIVO: Como não usamos mais os vetores, não há para onde "carregar"
     * os dados. As funções agora leem diretamente do arquivo quando precisam.
     * ---------------------------------------------------------------------
     */

  
    criar_pasta_data();

    int opcao_principal;

// estrutura de navegação intacta
    do {
        TelaMenuPrincipal();

        if (scanf("%d", &opcao_principal) != 1) {
            opcao_principal = -1;
        }
        while (getchar() != '\n');

        switch (opcao_principal) {
            case 1:
                // não passa mais vetores
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

    /*
     * ---------------------------------------------------------------------
     * TERCEIRA PARTE REMOVIDA: Salvamento final.
     *
     * salvar_clientes(todos_clientes, total_clientes);
     * ... etc ...
     *
     * MOTIVO: Cada alteração já é salva no arquivo .dat no momento em que é
     * feita. Não há necessidade de um "salvamento geral" no final.
     * ---------------------------------------------------------------------
     */

    finalizar_programa();
    return 0;
}

