
TARGET = sigmedical


SRC = clientes.c funcionarios.c consultas.c estoque.c movimentacao.c utils.c


OBJ = $(SRC:.c=.o)


CC = gcc


CFLAGS = -Wall -std=c11 -Isrc


$(TARGET): $(OBJ)
	$(CC) $(OBJ) Projeto_Final.c -o $(TARGET)


%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)