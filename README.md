# Trabalho 1 - Redes de Computadores I

## Descrição
Implementar um cliente e um servidor que façam troca de informações da seguinte forma:

* Quando um cliente se conectar ao servidor, o mesmo deverá perguntar: “Em que posso ajudar?”

  * Para isso vocês irão desenvolver um protocolo baseado em troca de códigos onde o servidor atenderá apenas 3 ações.

* O cliente mandará uma solicitação ao servidor, se a opção existir na lista do servidor ele irá atender, caso contrário o mesmo deverá responder “Não compreendi a sua solicitação”.

* As opções de resposta ou requisição podem ser:

* Qual o telefone do João? Servidor responde com telefone.

* Qual a data atual? O servidor responde a data atual.

* Que horas são? O servidor devolve a sua hora ao cliente.

* Pode ser implementado em Java.

* Deve atender a mais de uma solicitação por vez.

* Pode ser em grupo, dupla ou individual.

---
## Requisitos

* `g++`
  
---

## Como usar

Para executar o programa, ainda com o terminal/prompt de comando aberto, siga os passos abaixo:
* `sh script.sh`
* `cd bin/`
* `./server 1234`
* `./client localhost 1234`

---