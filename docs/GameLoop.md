# Game Loop

O Game Loop consiste em:

- Capiturar os Inputs do jogador.
- Atualizar todas as informações a partir desse Input.
- Desenhar o Frame atualizado na tela.

Esse ciclo se repete infinitamente e por isso é chamado de Game Loop.

Dentro do Game Loop nós queremos processar a maior quantidade de Frames
na menor quantidade de tempo.

Para executar 30 Frames por segundo, cada Frame deve durar no máximo:
33,333... milissegundos ou 1000ms / 30


Para executar 60 Frames por segundo, cada Frame deve durar no máximo:
16,666... milissegundos ou 1000ms / 60

