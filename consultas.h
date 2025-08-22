#ifndef CONSULTAS_H
#define CONSULTAS_H

typedef struct {
    char nome_paciente[50];
    char cpf_paciente[15];
    char data[11];
    char hora[6];
    char nome_medico[50];
    char especialidade[50];
    char status[15];
} Consulta;

void agendar_consulta(Consulta consultas[], int* total_consultas);
void salvar_consultas(Consulta consultas[], int total_consultas);
int ler_consultas(Consulta consultas[]);
void modulo_consultas(Consulta consultas[], int* total_consultas);

void pesquisar_consulta(Consulta consultas[], int total_consultas);
void alterar_consulta(Consulta consultas[], int total_consultas);
void excluir_consulta(Consulta consultas[], int* total_consultas);
void gerenciar_agendamentos(Consulta consultas[], int* total_consultas);
void confirmar_presenca(Consulta consultas[], int total_consultas);

void gerar_relatorios_consultas(Consulta consultas[], int total_consultas);
void relatorio_consultas_medico(Consulta consultas[], int total_consultas);
void relatorio_consultas_agendadas(Consulta consultas[], int total_consultas);
void relatorio_consultas_por_periodo(Consulta consultas[], int total_consultas);
void relatorio_consultas_canceladas(Consulta consultas[], int total_consultas);

#endif
