#ifndef RELATORIOS_H
#define RELATORIOS_H

/* Relatórios - centraliza relatórios de medicos, estoque e consultas */

/* Medicos */
void relatorios_submenu_medicos(void);
void relatorios_medicos_completo(void);
void relatorios_medicos_por_especialidade(void);
void relatorios_medicos_por_nome(void);
void relatorios_medicos_por_status(void);

/* Estoque */
void relatorios_submenu_estoque(void);
void relatorios_estoque_completo(void);
void relatorios_estoque_por_nome(void);
void relatorios_estoque_por_status(void);
void relatorios_estoque_itens_falta(void);
void relatorios_estoque_validade_proxima(void);

/* Consultas */
void relatorios_consultas_medico(void);
void relatorios_consultas_por_periodo(void);
void relatorios_consultas_paciente(void);
void relatorios_consultas_agendadas(void);
void relatorios_consultas_canceladas(void);

#endif
