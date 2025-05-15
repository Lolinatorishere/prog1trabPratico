#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct user{
    int userId;
    int type;
    char userName[256];
    char password[256];
}USER;

typedef struct candidatura{
    candidatura *prev;
    candidatura *next;
    char candidaturaId;
    char cursoId;
    int ano;
}CANDIDATURA;

typedef struct curso{
    int cursoId;
    int escolaId;
    char codigo[64];
    char area[64];
    int ano;
    int vagas;
}CURSO;

#endif
