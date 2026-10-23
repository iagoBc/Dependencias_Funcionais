#!/usr/bin/env python3

import os
import subprocess
import glob
import re
from rich.console import Console
from rich.panel import Panel
from rich.rule import Rule
from rich.theme import Theme

# Configuração de temas para o Rich
tema = Theme({
    "info": "bold cyan",
    "sucesso": "bold green",
    "erro": "bold red"
})
console = Console(theme=tema)

APP = "./fdtool"
PASTA_EXEMPLOS = "exemplos"

def extrair_numero(caminho_arquivo):
    """Extrai o número do nome do ficheiro para ordenar corretamente (e4 antes de e10)"""
    nome = os.path.basename(caminho_arquivo)
    match = re.search(r'\d+', nome)
    return int(match.group()) if match else 0

def extrair_atributos(caminho_arquivo):
    """Lê o ficheiro .fds e extrai os conjuntos do lado esquerdo das dependências."""
    try:
        with open(caminho_arquivo, 'r', encoding='utf-8') as f:
            conteudo = f.read()
        match = re.search(r'F\s*=\s*\{([^}]*)\}', conteudo)
        if match:
            conjuntos_lhs = []
            for dependencia in match.group(1).split(','):
                lhs, separador, _ = dependencia.partition('->')
                if separador:
                    lhs = re.sub(r'\s+', '', lhs)
                    if lhs and lhs not in conjuntos_lhs:
                        conjuntos_lhs.append(lhs)
            return conjuntos_lhs
    except Exception as e:
        console.print(f"Erro ao ler os conjuntos LHS do ficheiro {caminho_arquivo}: {e}", style="erro")
    return []

def main():
    subprocess.run("make")
    subprocess.run(["clear"])  # Limpa o terminal antes de iniciar os testes
    if not os.path.isfile(APP) or not os.access(APP, os.X_OK):
        console.print(f"Erro: Executável '{APP}' não encontrado ou sem permissão de execução.", style="erro")
        return

    padrao = os.path.join(PASTA_EXEMPLOS, "e*.fds")
    arquivos = glob.glob(padrao)
    arquivos.sort(key=extrair_numero)

    if not arquivos:
        console.print(f"Nenhum ficheiro encontrado no padrão {padrao}", style="erro")
        return

    for arquivo in arquivos:
        console.print(Rule(title=f"[bold blue]ARQUIVO: {arquivo}[/bold blue]", style="blue"))

        # Adiciona os testes fixos
        comandos = [
            ("MINCOVER", [APP, "mincover", "--fds", arquivo]),
            ("KEYS", [APP, "keys", "--fds", arquivo])
        ]

        # Extrai os atributos do ficheiro e gera um teste de fecho para cada um
        atributos = extrair_atributos(arquivo)
        for attr in atributos:
            comandos.append((f"CLOSURE (Atributo '{attr}')", [APP, "closure", "--fds", arquivo, "--X", attr]))

        for nome_teste, comando in comandos:
            console.print(f"\n[sucesso]>> Testando: {nome_teste}[/sucesso]")
            
            try:
                resultado = subprocess.run(comando, capture_output=True, text=True)
                
                if resultado.stdout:
                    console.print(Panel(resultado.stdout.strip(), title=f"Saída - {nome_teste}", border_style="green", expand=False))
                
                if resultado.stderr:
                    console.print(Panel(resultado.stderr.strip(), title=f"Erro - {nome_teste}", border_style="red", expand=False))
                    
            except Exception as e:
                console.print(f"Erro crítico ao tentar rodar o comando: {e}", style="erro")
            
        console.print("\n")

    console.print(Rule(title="[bold green]Todos os testes foram concluídos![/bold green]", style="green"))

if __name__ == "__main__":
    main()