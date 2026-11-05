# FDTool

O **FDTool** é uma ferramenta de linha de comando escrita em C para estudar
dependências funcionais em esquemas relacionais. A partir de um arquivo de
entrada com o universo de atributos e um conjunto de dependências funcionais,
o programa pode:

- calcular o fecho de um conjunto de atributos;
- calcular uma cobertura mínima;
- encontrar as chaves candidatas;
- verificar violações das formas normais BCNF e 3FN.

O projeto não exige uma biblioteca externa em tempo de execução. O executável é
gerado a partir dos fontes C presentes em [`codigos/`](./codigos).

## Requisitos

São necessários:

- GCC ou outro compilador compatível com C11;
- GNU Make;
- um sistema operacional Unix-like, como Linux, macOS ou WSL.

Em Debian, Ubuntu ou WSL, instale as ferramentas com:

```bash
sudo apt update
sudo apt install build-essential
```

## Obtendo o projeto

Clone o repositório e entre na pasta que contém o `makefile`:

```bash
git clone https://github.com/iagoBc/Dependencias_Funcionais.git
cd Dependencias_Funcionais
```

Todos os comandos deste README devem ser executados a partir da raiz do
projeto.

## Compilação

Compile o programa com:

```bash
make
```

O `makefile`:

1. localiza os arquivos `.c` em [`codigos/`](./codigos);
2. compila cada fonte com `-std=c11 -Wall -Wextra`;
3. grava os arquivos objeto em `codigos/objetos/`;
4. gera o executável `fdtool` na raiz do projeto.

Para recompilar sem reaproveitar os arquivos objeto:

```bash
make clean
make
```

Para remover os arquivos gerados, incluindo o executável:

```bash
make purge
```

Depois da compilação, confirme que o executável foi criado:

```bash
./fdtool
```

Sem argumentos, o programa exibirá a forma geral de uso e terminará com erro:

```text
Uso: ./fdtool <closure|mincover|keys|normalform> --fds <arquivo>
```

## Formato dos arquivos `.fds`

Cada arquivo de entrada deve informar:

- `U`: o universo de atributos;
- `F`: o conjunto de dependências funcionais.

Exemplo:

```text
U = {A,B,C,D,E}
F = { A->BC, C->D, BD->E }
```

### Universo de atributos

O universo é escrito entre chaves após `U =`:

```text
U = {A,B,C,D,E}
```

Os atributos são representados por caracteres alfabéticos. Durante a leitura,
o programa coleta os caracteres alfabéticos encontrados entre `{` e `}`.
Portanto, vírgulas e espaços são apenas separadores visuais.

### Dependências funcionais

Cada dependência segue o formato:

```text
lado_esquerdo -> lado_direito
```

O lado esquerdo e o lado direito podem conter vários atributos. Por exemplo,
`A -> BC` representa as dependências `A -> B` e `A -> C` quando a operação
exige dependências com um único atributo no lado direito.

As dependências podem ser separadas por vírgulas ou ponto e vírgula:

```text
F = {A->B, B->C, AB->D}
```

```text
F = {A->B; B->C; AB->D}
```

As declarações `U` e `F` devem aparecer no arquivo para que ele seja lido
corretamente. Os arquivos de exemplo em [`exemplos/`](./exemplos) mostram
entradas válidas, como [`exemplos/e1.fds`](./exemplos/e1.fds):

```text
U = {A,B,C,D,E}
F = { A -> BC, C-> D, BD    ->E }
```

## Sintaxe geral

Depois de compilar, use:

```bash
./fdtool <comando> --fds <arquivo.fds> [opções]
```

Os comandos disponíveis são:

| Comando | Finalidade | Opções adicionais |
| --- | --- | --- |
| `closure` | Calcula o fecho de atributos | `--X <atributos>` |
| `mincover` | Calcula uma cobertura mínima | Nenhuma |
| `keys` | Encontra as chaves candidatas | Nenhuma |
| `normalform` | Verifica BCNF e 3FN | Nenhuma |

Ao abrir um arquivo válido, o programa imprime no início o universo e as
dependências que foram lidos. O resultado da operação aparece nas linhas
seguintes.

## Operações

### Fecho de atributos

Use `closure` com `--X` para informar o conjunto cujo fecho será calculado:

```bash
./fdtool closure --fds exemplos/e1.fds --X A
```

Para o arquivo `e1.fds`, a saída final é:

```text
ABCDE
```

Isso significa que `A+ = {A,B,C,D,E}` para as dependências fornecidas.
Também é possível calcular o fecho de um conjunto:

```bash
./fdtool closure --fds exemplos/e1.fds --X BD
```

Todos os atributos informados em `--X` devem pertencer ao universo `U`. Um
atributo desconhecido produz uma mensagem de erro em `stderr` e o comando
termina com código diferente de zero.

### Cobertura mínima

Use `mincover` para transformar o conjunto de dependências em uma cobertura
mínima:

```bash
./fdtool mincover --fds exemplos/e1.fds
```

O algoritmo implementado:

1. separa dependências cujo lado direito possui mais de um atributo;
2. remove atributos desnecessários dos lados esquerdos quando possível;
3. remove dependências redundantes.

As dependências que permanecem são impressas uma por linha. A ordem pode
depender da ordem das dependências no arquivo de entrada.

### Chaves candidatas

Use `keys` para encontrar todos os conjuntos mínimos de atributos cujo fecho
contém o universo:

```bash
./fdtool keys --fds exemplos/e1.fds
```

Cada chave candidata é impressa em uma linha. Uma chave que contém outra chave
já encontrada não é impressa, pois não é mínima.

O programa testa combinações de atributos; por isso, o custo e o uso de
memória crescem conforme aumenta o número de atributos do universo.

### BCNF e terceira forma normal

Use `normalform` para verificar as dependências funcionais:

```bash
./fdtool normalform --fds exemplos/e1.fds
```

Uma dependência `X -> Y` viola a BCNF quando `X` não é uma superchave. Para a
3FN, além de `X` ser superchave, o lado direito precisa ser formado por
atributos primos, isto é, atributos que pertencem a alguma chave candidata.

Quando uma violação é encontrada, a saída identifica a forma normal, a
dependência e a condição que não foi satisfeita. Por exemplo:

```text
BCNF: VIOLATIONS
3FN: VIOLATIONS
VIOLATION BCNF: C->D (C not superkey)
VIOLATION 3NF: C->D (D not prime, C not superkey)
```

Se nenhuma violação for encontrada, o programa não imprime linhas de
violação após a representação de `U` e `F`.

## Exemplos de execução

O diretório [`exemplos/`](./exemplos) contém dez arquivos `.fds`. Para executar
as quatro operações sobre um arquivo específico:

```bash
./fdtool mincover --fds exemplos/e3.fds
./fdtool keys --fds exemplos/e3.fds
./fdtool normalform --fds exemplos/e3.fds
./fdtool closure --fds exemplos/e3.fds --X A
```

Também é possível redirecionar a saída para um arquivo:

```bash
./fdtool keys --fds exemplos/e3.fds > chaves-e3.txt
```

Para inspecionar um erro sem misturar a saída normal:

```bash
./fdtool closure --fds exemplos/e3.fds --X Z > saida.txt 2> erro.txt
```

## Organização do projeto

```text
.
├── codigos/
│   ├── closure.c/.h       # Cálculo de fechos
│   ├── dfs.c/.h           # Leitura e representação das dependências
│   ├── keys.c/.h          # Cálculo de chaves candidatas
│   ├── mincover.c/.h      # Cálculo da cobertura mínima
│   ├── normalform.c/.h    # Verificação de BCNF e 3FN
│   ├── queue.c/.h         # Fila usada pelas estruturas do programa
│   └── main.c             # Interface de linha de comando
├── exemplos/              # Arquivos de entrada .fds
├── makefile               # Regras de compilação e limpeza
├── README.md              # Documentação do projeto
└── fdtool                 # Executável gerado por make
```

Os arquivos em `codigos/objetos/` são intermediários e podem ser apagados com
`make clean`. O executável `fdtool` também é um artefato de compilação e pode
ser recriado a qualquer momento com `make`.

## Erros comuns

### Comando inválido

```bash
./fdtool desconhecido --fds exemplos/e1.fds
```

O programa informa que o comando é desconhecido. Os únicos comandos aceitos
são `closure`, `mincover`, `keys` e `normalform`.

### Arquivo inexistente

```bash
./fdtool keys --fds exemplos/nao-existe.fds
```

O erro de abertura é enviado para `stderr`, e o processo termina com código
diferente de zero.

### Opção `--X` ausente

O comando `closure` exige um conjunto de atributos:

```bash
./fdtool closure --fds exemplos/e1.fds
```

Use, por exemplo, `--X A` ou `--X AB`.

### Entrada malformada

Verifique se o arquivo possui as declarações `U` e `F`, se as dependências
contêm `->` e se todas estão dentro das chaves correspondentes. Uma entrada
válida deve seguir o padrão:

```text
U = {atributos}
F = {X->Y, Z->W}
```

## Desenvolvimento

Após alterar qualquer arquivo `.c` ou `.h`, compile novamente:

```bash
make
```

Para uma reconstrução completa:

```bash
make purge
make
```

As opções `-Wall` e `-Wextra` habilitam avisos importantes do compilador e
ajudam a detectar problemas durante o desenvolvimento.
