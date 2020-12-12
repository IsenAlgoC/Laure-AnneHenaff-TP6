#pragma once
#include "personne.h"
// LISTES UNILATERES **************************************************
// #define VERSION 3.0
// #define SQUELET

#define UP 0
#define DOWN 1


typedef struct SingleLinkedListElem {
	Enregistrement pers;
	struct SingleLinkedListElem *next;
} SingleLinkedListElem;

typedef struct LinkedList {
	SingleLinkedListElem *head;
	int size;
	SingleLinkedListElem *tail;
} LinkedList;
// Cr�e une liste cha�n�e unilat�re vide et renvoie un pointeur sur celle-ci
LinkedList *NewLinkedList();

// Cr�e un nouveau maillon qui contient une nouvelle personne et renvoie un pointeur sur ce maillon
SingleLinkedListElem  *NewLinkedListElement(Enregistrement pers);

// Renvoie un pointeur sur le maillon qui se trouve en i�me position � partir de 1
SingleLinkedListElem *GetElementAt(LinkedList *liste, int i);

// Ajoute une nouvelle personne dans la liste cha�n�e en i�me position
// Cette fonction fait appel � la fonction NewLinkedListElement(Enregistrement pers) pour cr�er un maillon

int InsertElementAt(LinkedList *liste, int i, Enregistrement pers);

// Suppression et destruction d'un �l�ment de liste chain�e
int DeleteLinkedListElem(LinkedList *list, SingleLinkedListElem *Elem);

