# SIGmedical - Sistema de Gestão de Clínicas Médicas
# Makefile

TARGET = projeto_final

CC = gcc
CFLAGS = -Wall -g -Isrc -finput-charset=UTF-8 -fexec-charset=UTF-8

# Arquivos fonte
SOURCES = projeto_final.c \
          src/utils.c \
          src/validador.c \
          src/clientes.c \
          src/clientes_relatorios.c \
          src/medicos.c \
          src/medicos_relatorios.c \
          src/consultas.c \
          src/consultas_relatorios.c \
          src/estoque.c \
          src/estoque_relatorios.c \
          src/movimentacao.c

# Arquivos objeto
OBJECTS = $(SOURCES:.c=.o)

# Regra principal
all: $(TARGET)

# Linkagem
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compilação de arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
ifeq ($(OS),Windows_NT)
	@if exist *.o del /Q *.o
	@if exist src\*.o del /Q src\*.o
	@if exist $(TARGET).exe del /Q $(TARGET).exe
else
	rm -f $(OBJECTS) $(TARGET)
endif

# Recompilar tudo
rebuild: clean all

.PHONY: all clean rebuild