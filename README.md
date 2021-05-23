# Game Engine Tutorial

- https://github.com/marciovmf/smol

### Ferramentas

- [Visual Studio](https://visualstudio.microsoft.com/pt-br/vs/) 
  - Instalar pacote "Desenvolvimento para Desktop com C++"

- [NeoVim](https://neovim.io/)

```cmd
choco install neovim
```

- [CMake](https://cmake.org/download/)

---

### Variáveis de Ambiente

- Pesquisar por "Editar Variáveis de Ambiente do Sistema".
- Em "Variáveis do Sistema":
  - Adicionar uma nova variável com o caminho do diretório de instalação do Visual Studio, ex:

	Nome da variável: `MSVC2019`\
	Valor da variável: `C:\Program Files (x86)\Microsoft Visual Studio\2019`

- Em "Variáveis de usuário":
  - Adicionar o caminho do diretório de build do Visual Studio à variável "Path" clicando em:
    - [Editar...]
    - [Novo]
    - Digitar o caminho do diretório, ex:

    	`%MSVC2019%\Community\VC\Auxiliary\Build`

- Salve as configurações e abra um terminal.

- Execute os comandos:

```cmd
vcvars64
````

> `vcvars64` Configura todas as variáveis ambiente para dar acesso direto a todas as ferramentas via terminal

output:

```cmd
**********************************************************************
** Visual Studio 2019 Developer Command Prompt v16.6.2
** Copyright (c) 2020 Microsoft Corporation
**********************************************************************
[vcvarsall.bat] Environment initialized for: 'x64'
```

---

```cmd
cl
```

> `cl` chama o compilador

output:

```cmd
Microsoft (R) C/C++ Optimizing Compiler Versão 19.26.28806 para x64
Copyright (C) Microsoft Corporation. Todos os direitos reservados.

uso: cl [ option... ] filename... [ /link linkoption... ]
```

---

```bash
link
```

> `link` chama o linker

output:

```
Microsoft (R) Incremental Linker Version 14.26.28806.0
Copyright (C) Microsoft Corporation.  All rights reserved.

 uso: LINK [opções] [arquivos] [@commandfile]

 ...
````

---

## Compilação C++

Um Compilador traduz o codigo para instruções de máquinas.

Antes que o código fonte chegue a compilador no processo de build,
existe uma etapa anterior que passa pelo Pré Processador.

Instruções com `#`, exemplo: 

`#include <stdio.h>`

São instruções que o Pré Processador terá que lidar antes de enviar para o Compilador.

O Compilador trabalha com a saída do Pré Processador.

Exemplos:

```c++
#include <stdio.h>
```

```c++
#include <stdio.h>

#define TESTE

int main() {

#if defined(TESTE)
	println("TESTE DEFINED");
#else
	println("TESTE NOT DEFINED");
#endif

	return 0;
}
```