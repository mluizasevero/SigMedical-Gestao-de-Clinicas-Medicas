#ifndef CONSULTAS_H
#define CONSULTAS_H

typedef struct
{
    char nome_paciente[50];
    char cpf_paciente[15];
    char data[11];
    char hora[6];
    char nome_medico[50];
    char especialidade[50];
    char status[15];
    int ativo;
} Consulta;

void modulo_consultas(void);
void agendar_consulta(void);
void pesquisar_consulta(void);

void gerenciar_agendamentos(void);
void alterar_consulta(void);
void excluir_consulta(void);
void confirmar_presenca(void);

void gerar_relatorios_consultas(void);
void relatorio_consultas_medico(void);
void relatorio_consultas_por_periodo(void);
void relatorio_consultas_agendadas(void); // Ainda não implementado
void relatorio_consultas_canceladas(void); // Ainda não implementado

#endif
