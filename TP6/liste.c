#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "liste.h"

// #define VERSION 3.0
// #define SQUELET


// crée une nouvelle liste chaine unilataire vide et renvoie un pointeur sur cette liste
LinkedList *NewLinkedList() {
	LinkedList *tmp;
	tmp = (LinkedList*)malloc(sizeof(LinkedList));
	if (tmp != NULL) {
		tmp->head = NULL;
		tmp->tail = NULL;
		tmp->size = 0;
	}
	return tmp;
}
// crée un nouveau maillon qui contient la personne passée en paramètre
SingleLinkedListElem  *NewLinkedListElement(Enregistrement pers) {
	SingleLinkedListElem *tmp;
	tmp = (SingleLinkedListElem *)malloc(sizeof(SingleLinkedListElem));
	if (tmp != NULL) {	
	tmp->pers = pers;
	tmp->next = NULL;
	}
	return(tmp);
}
// renvoie un pointeur sur l'élèment en ième position dans la liste
SingleLinkedListElem *GetElementAt(LinkedList *Liste, int i) {
	int CurrentIndex = 0;
	SingleLinkedListElem *Element;
	if ((Liste == NULL) || (i < 0) || (i >= Liste->size)) return(NULL);
	if (i == 0) return(Liste->head);
	if (i == Liste->size - 1) return(Liste->tail);
	Element = Liste->head;
	while (CurrentIndex != i  && Element != NULL) {
		Element = Element->next;
		CurrentIndex++;
	}
	return(Element);
}

// Ajoute une nouvelle personne dans la liste chaînée en ième position
// Cette fonction fait appel à la fonction NewLinkedListElement(Enregistrement pers) pour créer un maillon


int InsertElementAt(LinkedList *Liste, int i, Enregistrement pers) {
	SingleLinkedListElem *CurrentElement, *NewElement;
	if (Liste == NULL) return(0);
	// recherche de l'élément qui se trouve déjà en position i
	CurrentElement = GetElementAt(Liste, i);
	// s'il y en a un
	if (CurrentElement != NULL) {
		// on insère un nouvel élèment
		NewElement = NewLinkedListElement(pers);
		// son suivant est alors l'élèment courant
		NewElement->next = CurrentElement;

		if (i == 0) {
			// si l'insertion est en tête
			// le nouvel élément devient la tête
			Liste->head = NewElement;
		}
		else {
			// sinon il faut rétablir le chainage depuis l'élément précédent
			CurrentElement = GetElementAt(Liste, i - 1);
			// le successeur du précédent devient le nouvel élément
			CurrentElement->next = NewElement;
		}
		Liste->size++;
		return(1);
	}
	else {
		if (Liste->size == 0) { // insertion en tête de l'unique élément
			NewElement = NewLinkedListElement(pers);
			if (NewElement != NULL) {
				Liste->head = NewElement;
				Liste->size = 1;
		}
			else {
				return(0);
			}
		}
		if (Liste->size <= i) { // insertion en queue
			NewElement = NewLinkedListElement(pers);
			if (NewElement != NULL) {
				Liste->tail = NewElement;
				Liste->size = Liste->size + 1;
			}
			else {
				return(0);
			}
		}

	}
	return(0);
}

//
// Suppression d'un élément de la liste chaînée
//

int DeleteLinkedListElem(LinkedList * list, SingleLinkedListElem * item) {
	if (list == NULL) return(0); // La liste n'existe pas
	if ((list->head == NULL) || (list->tail == NULL)) return(0); // liste vide ou anomalie
	if ((list->head == list->tail) && (list->size != 1)) return(0); // anomalie
	if ((list->size == 0) || (item == NULL)) return(0); // pas d'élément dans la liste ou item invalide

	if ((item == list->head) && (item == list->tail)) { //1 seul élément dans la liste
		list->head = NULL;
		list->tail = NULL; 
		list->size = 0;
		free(item); //la liste devient vide
		return(1);
	}

	if (item == list->head) { //si l'élément est en tête, on supprime la tête
		list->head = item->next;
		list->size--;
		free(item);
		return(1);
	}
	
	SingleLinkedListElem* tmp = list->head;
	SingleLinkedListElem* previous = NULL;

	while((tmp != NULL) && (tmp != item)) {
		previous = tmp;
		tmp = tmp->next;
	}

	if((previous != NULL) && (tmp == item) && (tmp->next == NULL)) { //s'il est en queue, on supprime la queue
		list->tail= previous;
		previous->next= NULL;
		list->size--;
		free(item);
		return(1);
	}

	if((previous != NULL) && (tmp == item) && (tmp->next != NULL)) { // s'il est au milieu, on supprime l'élémentif
		previous->next = item->next;
		list->size--;
		free(item);
		return(1);
	}

	return(0);  // pas trouvé
}

