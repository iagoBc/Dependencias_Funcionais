#!/usr/bin/env python3
"""
Testa `./fdtool normalform --fds exemplos/NN_nome.fds` e compara a saída
com `exemplos/NN_nome.expected.txt`.

Uso:
    ./testar.py                  # roda todos os exemplos
    ./testar.py 03 05            # roda só os que começam com 03 e 05
    ./testar.py --ignorar-ordem  # ignora a ordem das linhas VIOLATION
    ./testar.py --sem-make       # não roda o make antes
"""

import argparse
import difflib
import glob
import os
import re
import subprocess
import sys

from rich.console import Console
from rich.panel import Panel
from rich.rule import Rule
from rich.text import Text
from rich.theme import Theme

tema = Theme({
    "info": "bold cyan",
    "sucesso": "bold green",
    "erro": "bold red",
    "aviso": "bold yellow",
})
console = Console(theme=tema)

APP = "./fdtool"
PASTA_EXEMPLOS = "exemplos"
SUFIXO_ESPERADO = ".expected.txt"


def extrair_numero(caminho_arquivo):
    """Extrai o primeiro número do nome do arquivo para ordenar (02 antes de 10)."""
    nome = os.path.basename(caminho_arquivo)
    match = re.search(r"\d+", nome)
    return int(match.group()) if match else 0


def normalizar(texto, ignorar_ordem=False):
    """
    Deixa a saída comparável: remove espaços no fim de cada linha e linhas
    em branco no fim (alguns .expected.txt não terminam com \\n).
    Com ignorar_ordem, as linhas VIOLATION são ordenadas.
    """
    linhas = [l.rstrip() for l in texto.splitlines()]
    while linhas and not linhas[-1]:
        linhas.pop()

    if ignorar_ordem:
        cabecalho = [l for l in linhas if not l.startswith("VIOLATION")]
        violacoes = sorted(l for l in linhas if l.startswith("VIOLATION"))
        linhas = cabecalho + violacoes

    return linhas


def mostrar_diff(esperado, obtido):
    """Imprime um diff colorido (vermelho = esperado/faltando, verde = obtido/extra)."""
    saida = Text()
    for linha in difflib.unified_diff(esperado, obtido,
                                      fromfile="esperado", tofile="obtido",
                                      lineterm=""):
        if linha.startswith("+") and not linha.startswith("+++"):
            saida.append(linha + "\n", style="green")
        elif linha.startswith("-") and not linha.startswith("---"):
            saida.append(linha + "\n", style="red")
        elif linha.startswith("@@"):
            saida.append(linha + "\n", style="cyan")
        else:
            saida.append(linha + "\n", style="dim")
    console.print(Panel(saida, title="Diferença", border_style="red", expand=False))


def testar_arquivo(arquivo_fds, ignorar_ordem):
    """Retorna 'ok', 'falhou' ou 'sem_esperado'."""
    base = os.path.splitext(arquivo_fds)[0]
    arquivo_esperado = base + SUFIXO_ESPERADO

    console.print(Rule(title=f"[bold blue]{os.path.basename(arquivo_fds)}[/bold blue]", style="blue"))

    if not os.path.isfile(arquivo_esperado):
        console.print(f"Arquivo esperado não encontrado: {arquivo_esperado}", style="aviso")
        return "sem_esperado"

    comando = [APP, "normalform", "--fds", arquivo_fds]
    try:
        resultado = subprocess.run(comando, capture_output=True, text=True, timeout=30)
    except subprocess.TimeoutExpired:
        console.print("FALHOU: o comando passou de 30s (timeout).", style="erro")
        return "falhou"
    except Exception as e:
        console.print(f"Erro crítico ao rodar o comando: {e}", style="erro")
        return "falhou"

    with open(arquivo_esperado, "r", encoding="utf-8") as f:
        esperado = normalizar(f.read(), ignorar_ordem)
    obtido = normalizar(resultado.stdout, ignorar_ordem)

    if resultado.stderr.strip():
        console.print(Panel(resultado.stderr.strip(), title="stderr", border_style="yellow", expand=False))

    if esperado == obtido:
        console.print("[sucesso]✔ OK[/sucesso]")
        return "ok"

    console.print("[erro]✘ FALHOU[/erro]")
    mostrar_diff(esperado, obtido)
    return "falhou"


def main():
    parser = argparse.ArgumentParser(description="Compara a saída do fdtool normalform com os .expected.txt")
    parser.add_argument("prefixos", nargs="*", help="prefixos dos exemplos a rodar (ex.: 03 05)")
    parser.add_argument("--ignorar-ordem", action="store_true",
                        help="ignora a ordem das linhas VIOLATION")
    parser.add_argument("--sem-make", action="store_true", help="não roda o make antes")
    args = parser.parse_args()

    if not args.sem_make:
        if subprocess.run(["make"]).returncode != 0:
            console.print("Erro: o make falhou.", style="erro")
            return 1

    if not os.path.isfile(APP) or not os.access(APP, os.X_OK):
        console.print(f"Erro: executável '{APP}' não encontrado ou sem permissão de execução.", style="erro")
        return 1

    padrao = os.path.join(PASTA_EXEMPLOS, "*.fds")
    arquivos = sorted(glob.glob(padrao), key=extrair_numero)

    if args.prefixos:
        arquivos = [a for a in arquivos
                    if any(os.path.basename(a).startswith(p) for p in args.prefixos)]

    if not arquivos:
        console.print(f"Nenhum arquivo encontrado em {padrao}", style="erro")
        return 1

    contagem = {"ok": 0, "falhou": 0, "sem_esperado": 0}
    reprovados = []

    for arquivo in arquivos:
        status = testar_arquivo(arquivo, args.ignorar_ordem)
        contagem[status] += 1
        if status == "falhou":
            reprovados.append(os.path.basename(arquivo))

    console.print()
    console.print(Rule(title="[bold]Resumo[/bold]", style="white"))
    console.print(f"[sucesso]Passaram: {contagem['ok']}[/sucesso]  "
                  f"[erro]Falharam: {contagem['falhou']}[/erro]  "
                  f"[aviso]Sem .expected.txt: {contagem['sem_esperado']}[/aviso]")
    if reprovados:
        console.print("Reprovados: " + ", ".join(reprovados), style="erro")

    return 0 if contagem["falhou"] == 0 and contagem["sem_esperado"] == 0 else 1


if __name__ == "__main__":
    sys.exit(main())