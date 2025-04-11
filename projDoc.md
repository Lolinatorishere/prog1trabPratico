Engenharia Informática
Programação I
Trabalho Prático A – Turno A
Ano letivo de 2024/2025
Gestão candidatos a CTeSP
Objetivo:

Este trabalho tem como objetivo principal a implementar na linguagem C de um
programa de gestão das candidaturas aos Cursos Técnicos Superiores (CTeSP) do IPVC e
respetivas colocações.

Descrição do programa

O IPVC tem um conjunto de 20 CTeSP. Cada CTeSP tem um código, um nome, uma área
(informática, desporto, turismo, etc.), o número de vagas e pertence a uma das 6 Escola do
IPVC (ESTG, ESS, ESE, ESDL, ESA, ESCE).

Em cada ano letivo o IPVC recebe um número indeterminado de candidaturas. Um
candidato pode candidatar-se a um curso após introduzir os seus dados (nome, etc.). O
programa deve numerar sequencialmente os candidatos.

O programa deve ter 2 tipos de utilizadores: o administrador e o candidato.

Programa:
Um utilizador deve estar devidamente registado. Existem 2 tipos de utilizadores: o

administrador e o candidato. O administrador (user Admin) faz a gestão das dos cursos e

seriação final dos candidatos. O candidato pode gerir a sua própria candidatura.

Assim, a implementação do trabalho está dividida em várias partes:
1. Numa primeira parte pretende-se implementar a gestão dos cursos que podem

receber candidaturas num determinado ano letivo.

2. Na segunda parte pretende-se gerir as candidaturas.

3. Na terceira parte, após receberem todas as candidaturas o programa deve

apresentar os candidatos colocados em cada curso.

4. O programa deve guardar em memória as candidaturas numa lista ligada. O

programa deve manter a informação quando corre novamente.

Descrição do programa:
1. Como administrador, o programa deve permitir:

  - o Acrescentar um curso.
  - o Alterar os dados de um curso, nomeadamente a quantidade de vagas.
  - o Colocar um curso como Aberto/Não aberto.
  - o Listar cursos por ordem alfabética do nome.
  - o Listar cursos de uma determinada escola.
  - o Listar todos os candidatos a um curso.
  - o Fechar as candidaturas.
  - o Seriar os candidatos:
  Os candidatos são seriados da seguinte forma:
    - Primeiro são colocados os candidatos cujo curso do secundário é da mesma área do
      curso a que se candidata.
    - Dentro destes, são colocados por média (ordem decrescente) da nota do secundário;
    - Se ainda sobrarem vagas, são colocados os restantes candidatos igualmente ordenados
      pode ordem decrescente da média final do curso.
    - Caso exista um empate na nota do último candidato,
      todos os empatados (com a mesma nota) devem ficar colocados.
    - o Imprimir para ficheiro de texto a lista de candidatos (num e nome) com a
    respetiva situação (nome do curso em que ficou colocado/ou não colocado)
    - o Imprimir para ficheiro de texto a lista de colocados num curso.
    - o nome do ficheiro deve ter o nome do curso seguido do ano letivo. O nome do curso
    deve ser recebido do utilizador.
    - o Imprimir para ficheiro de texto a lista de colocados numa escola, nome do
    candidato seguido do nome do curso em que ficou colocado. Os dados devem
    - ser ordenados pelo nome do curso seguido do nome do candidato.
    - o Fazer analise das candidaturas: para cada curso, indicar a quantidade de
    candidatos, quantidade de colocados, % alunos colocados, etc..

2. Para o candidato, o programa deve permitir:
  - o Fazer o registo: No arranque, o programa deve pedir a identificação (user e
    password) ao utilizador. Se um user não existir, o programa deve perguntar se
    se pretende registar.
    Se sim, deve proceder ao seu registo,
    ou seja, o candidato deve introduzir o nome completo, o nCC, o NIF, data de
    nascimento, nome e área do curso secundário, nome da escola que frequentou
    e média final do curso.
  - o Depois disso deve selecionar o curso a que se candidata.
  - o Listar a informação sobre a sua candidatura.
  - o Enquanto as candidaturas estão abertas, os candidatos podem alterar os seus
    dados e alterar o curso a que se candidata.
  - o Depois de seriados, os candidatos podem verificar se ficou ou não colocado e
    em que curso ficaram colocados.
  - o Imprimir para ficheiro de texto os seus dados.

NOTA: Os requisitos do programa não podem ser alterados.
Estrela Ferreira Cruz
