# Portfólio: Projeto de Codificação de Huffman

<!-- Badges -->

![Last Commit](https://img.shields.io/github/last-commit/SEU_USUARIO/SEU_REPOSITORIO?style=flat-square)
![C 100.0%](https://img.shields.io/badge/C-100%25-blue?style=flat-square\&logo=c)
![Languages](https://img.shields.io/github/languages/count/SEU_USUARIO/SEU_REPOSITORIO?style=flat-square)

## Descrição

Este repositório faz parte do meu portfólio e apresenta uma implementação completa do **algoritmo de Codificação de Huffman** em C. O projeto inclui:

* Construção de uma tabela de frequências a partir de um arquivo de entrada.
* Montagem da árvore de Huffman com nós ordenados por frequência.
* Geração recursiva dos códigos binários para cada byte.
* Compressão do arquivo original e gravação do resultado em formato `.huff`.

## Tecnologias Utilizadas

* **Linguagem**: C
* **Ferramentas**: GCC, Makefile (opcional)
* **Conceitos**: Estruturas de Dados, Árvores Binárias, Filas de Prioridade (lista ordenada), Alocação Dinâmica

## Estrutura do Projeto

```
/ (raiz do projeto)
├── src/                       # Código fonte em C
│   ├── codigo.c/.h           # Funções para manipulação de códigos binários
│   ├── huffman.c/.h          # Implementação da árvore de Huffman
│   ├── gera_codigos.c/.h     # Geração dos códigos recursivos
│   ├── lista_ordenada.c/.h   # Lista ordenada para filas de prioridade
│   ├── tabela_de_frequencias.c/.h  # Tabela de frequências dos bytes
│   ├── myTypesClion.h        # Definições de tipos personalizados (I8, U8, etc.)
│   └── main.c                # Função principal de leitura, compressão e escrita
└── README.md                 # Guia de uso e documentação
```

## Como Compilar e Executar

1. **Compilar**:

   ```bash
    gcc -o compacta \                      
      main.c \          
      tabela_de_frequencias.c \
      lista_ordenada.c \
      gera_codigos.c \
      codigo.c \                
      huffman.c \


   ```
   ```bash
   gcc -o descompacta \                      
    descompactador.c \
    tabela_de_frequencias.c \
    lista_ordenada.c \
    huffman.c \

   ```
2. **Executar**:

   ```bash
   ./compacta arquivo.extensao   
   ```
   ```bash
   ./descompacta saida.huff saida.extensao
   ```

   * Por padrão, o programa lê `entrada.txt` e gera `saida.huff`.

## Funcionalidades

* **Leitura Binária** de arquivos de entrada.
* **Cálculo de Frequências** por byte.
* **Construção Dinâmica** da árvore de Huffman.
* **Geração Recursiva** dos códigos de cada caractere.
* **Escrita** de arquivo comprimido com cabeçalho de frequências e bits codificados.

## Built with the tools and technologies:

![Built with: C](https://img.shields.io/badge/Built%20with-C-lightgrey?style=flat-square\&logo=c\&logoColor=black)

## Contato

* **Nome**: \[Seu Nome]
* **Email**: [seu.email@exemplo.com](mailto:seu.email@exemplo.com)
* **LinkedIn**: [https://www.linkedin.com/in/seu-perfil](https://www.linkedin.com/in/seu-perfil)

> Este projeto demonstra conhecimentos em desenvolvimento em C, manipulação de estruturas de dados e otimização de algoritmos de compressão para inclusão no meu portfólio profissional.
