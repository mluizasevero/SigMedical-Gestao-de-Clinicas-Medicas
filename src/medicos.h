#ifndef MEDICOS_H
#define MEDICOS_H

typedef struct {
    int  id;
    char nome[50];
    char cpf[15];
    char especialidade[50];
    char telefone[15];
    int  ativo;             
} Medico;


void modulo_medicos(void);
void cadastrar_medico(void);
void pesquisar_medico(void);
void alterar_medico(void);
void excluir_medico(void);
void listar_medicos(void);

#endif



