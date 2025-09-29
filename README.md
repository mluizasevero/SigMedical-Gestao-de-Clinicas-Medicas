# SigMedical
Sistema de clínica médica: Sistema em linguagem C desenvolvido para a disciplina de Programação, com funcionalidades de gerenciamento de consultas médicas, controle de estoque de materiais e manuntenção de funcionários e clientes.

## Como Rodar:
### Pré-requisitos

Antes de começar, você precisa ter instalado no seu computador:

1. **Visual Studio Code (VS Code)** – [Download aqui](https://code.visualstudio.com/)
2. **GCC (compilador C)** – parte do pacote **MinGW** (no Windows) ou já vem instalado no Linux/Mac.
3. **Extensão "C/C++" da Microsoft no VS Code** – instale pela aba de extensões (`Ctrl+Shift+X` no VS Code).


### Passo a Passo para Compilar e Rodar

1. Abra o VS Code
Abra a pasta onde está o arquivo `projeto_final.c`.

2. Abra o Terminal no VS Code
Vá em **Terminal > Novo Terminal**
Ou pressione: `Ctrl + '` (crase)

3. Compile o programa

No terminal, digite o seguinte comando:

```
gcc projeto_final.c ./src/*.c -o sig_medical
```

Esse comando usa o `gcc` para compilar e gerar um executável chamado `sig_medical`.

4. Execute o programa

##### Se estiver no **Windows**, digite:

```bash
./sig_medical.exe
```

##### Se estiver no **Linux ou Mac**, digite:

```bash
./sig_medical
```

# Funcionalidades:
# Consultas
- Cadastrar consultas (paciente, médico, data e horário)
- Listar todas as consultas cadastradas

# Estoque
- Cadastrar produtos (nome, quantidade e validade)
- Listar todos os produtos
- Atualizar informações de produtos
- Remover produtos do estoque
  
# Médicos
- Cadastrar médicos
- Pesquisar médicos
- Alterar médicos
- Excluir médicos

# Clientes
- Cadastrar clientes
- Pesquisar clientes
- Alterar clientes
- Excluir clientes

# Como executar
1. Baixe ou clone o repositório.
2. Compile o código C.

# Criado por:
Thiago Gomes de Oliveira
thiago.oliveira.712@ufrn.edu.br

Maria Luiza Severo de Lima
maria.severo.063@ufrn.edu.br

Marcus Vinicius de Souza Azevedo
infobasicifrn2017marcusv@gmail.com

# referencias
https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.sqlshack.com%2Fworking-with-json-data-in-python%2F&psig=AOvVaw2b99UDV1hNxHfhSo7a_1Ho&ust=1758740782030000&source=images&cd=vfe&opi=89978449&ved=0CBUQjRxqFwoTCKijxI_K748DFQAAAAAdAAAAABBH

# To do
1. Refatorar o README.md (ok!)
2. Adicionar módulo de funcionários (ok!)
3. Trocar o for pelo while (ok!)
4. Consertar o Makefile (ok!)
5. Transformar os arquivos em Json
6. Ler os arquivos Json
