TARGET = sigmedical

CC = gcc
CFLAGS = -Wall -g -Isrc


SOURCES = projeto_final.c \
          src/utils.c \
          src/clientes.c \
          src/medicos.c \
          src/consultas.c \
          src/estoque.c \
          src/movimentacao.c \
          src/relatorios.c \
		  src/validador.c


OBJECTS = $(SOURCES:.c=.o)


all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	@echo "Limpando arquivos gerados..."
ifeq ($(OS),Windows_NT)
	del /F /Q $(OBJECTS) $(TARGET).exe
else
	rm -f $(OBJECTS) $(TARGET)
endif