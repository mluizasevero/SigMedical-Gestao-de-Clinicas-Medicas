#ifndef MEDICOS_H
#define MEDICOS_H

typedef struct {
    int id;
    char nome[50];
    char cpf[15];
    char especialidade[50];
    char telefone[15];
} Medico;

void salvar_medicos(Medico medicos[], int total_medicos);
int ler_medicos(Medico medicos[]);

void modulo_medicos(Medico medicos[], int* total_medicos);
void cadastrar_medico(Medico medicos[], int* total_medicos);
void pesquisar_medico(Medico medicos[], int total_medicos);
void alterar_medico(Medico medicos[], int total_medicos);
void excluir_medico(Medico medicos[], int* total_medicos);

#endif


