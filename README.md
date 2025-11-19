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

# Desenvolvedores:

<div style="display: flex; gap: 20px; align-items: center;">

  <div style="text-align: center;">
    <img src="https://avatars.githubusercontent.com/u/226151666?v=4" width="100" style="border-radius:50%;">
    <p>
      Maria Luiza Severo de Lima<br>
      <a href="mailto:maria.severo.063@ufrn.edu.br">maria.severo.063@ufrn.edu.br</a><br>
      <a href="https://github.com/mluizasevero" target="_blank">GitHub</a>
    </p>
  </div>

  <div style="text-align: center;">
    <img src="https://avatars.githubusercontent.com/u/226164279?v=4" width="100" style="border-radius:50%;">
    <p>
      Thiago Gomes de Oliveira<br>
      <a href="mailto:thiago.oliveira.712@ufrn.edu.br">thiago.oliveira.712@ufrn.edu.br</a><br>
      <a href="https://github.com/thiagogomes1805" target="_blank">GitHub</a>
    </p>
  </div>

  <div style="text-align: center;">
    <img src="https://avatars.githubusercontent.com/u/104383688?v=4" width="100" style="border-radius:50%;">
    <p>
      Marcus Vinicius de Souza Azevedo<br>
      <a href="mailto:infobasicifrn2017marcusv@gmail.com">infobasicifrn2017marcusv@gmail.com</a><br>
      <a href="https://github.com/MViniciusCoffe" target="_blank">GitHub</a>
    </p>
  </div>

  <div style="text-align: center;">
    <img src="https://avatars.githubusercontent.com/u/226083576?s=400&v=4" width="100" style="border-radius:50%;">
    <p>
      Luis Felipe Linhares Pereira<br>
      <a href="mailto:luis.felipe.linhares.701@ufrn.edu.br">luis.felipe.linhares.701@ufrn.edu.br</a><br>
      <a href="https://github.com/Luisfelipelinhares" target="_blank">GitHub</a>
    </p>
  </div>

</div>



# To do
1. Cadastrar dados genéricos e commitar o `/data`
2. Colocar os relatórios como tabela
3. Colocar filtros em todos os relatórios (1 filtro por módulo)
4. Atualizar informações do projeto
5. Ajeitar as telas
