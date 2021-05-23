
# Game Engine do zero 

## Video 2 - Entrypoints, dllexport e dllimport

https://www.youtube.com/watch?v=CQEakNJl7NU&ab_channel=gamedevlog

## Start

- Pré requisitos:
    - Abrir o terminal e executar o comando: `vcvars64`
    - Executar os comandos com `cmake` no mesmo diretório do arquivo `CMakeLists.txt`.

### Gerar uma solução do Visual Studio

```cmd

cmake -S . -G "Visual Studio 16 2019" -B build

```

A solução acima é apenas demonstrativa para ver o funcionamento da ferramenta. 

Para continuar a aula delete a pasta "build", e execute os passos abaixo.

### Gerar uma solução NMake Makefiles

```cmd

cmake -S . -G "NMake Makefiles" -B build

```

### Compilar e Rodar


```cmd

cd build

nmake

```
