#include <stdio.h>   /* pour les entrées-sorties */
#include <string.h>  /* pour les manipulations de chaînes de caractères */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <curses.h>
#include "rep.h"
#include "errno.h"

#define VERSION 3.0
#define SQUELET

/**************************************************************************/
/* Compléter votre nom ici                                                */
/*   Nom : Hénaff                        Prénom : Laure-Anne              */
/**************************************************************************/

extern bool modif;


/**********************************************************************/
/*  Ajout d'un contact dans le répertoire stocke en mémoire           */
/**********************************************************************/

int ajouter_un_contact_dans_rep(Repertoire *rep, Enregistrement enr)
{
#ifdef IMPL_TAB
	int idx;

	if (rep->nb_elts < MAX_ENREG)
	{
		modif = true;
		rep->tab[rep->nb_elts] = enr;      //ajout du contact à la fin du tableau
		rep->nb_elts = rep->nb_elts + 1;
		return(OK);
	}
	else {
		return(ERROR);
	}

	
#else
#ifdef IMPL_LIST

	bool inserted = false;
	if (rep->nb_elts == 0) {
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) {
			rep->nb_elts += 1;
			modif = true;
			rep->est_trie = true;
			return(OK);
		}

	}
	else {
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) {
			rep->nb_elts += 1; //ajout l'élèment à la fin
			modif = true;
			rep->est_trie = false;
			return(OK);
		}
	}


#endif
	
#endif


	return(OK);

} /* fin ajout */
  /**********************************************************************/
  /* supprime du répertoire l'enregistrement dont l'indice est donné en */
  /*   paramètre       et place modif = true                            */
  /**********************************************************************/

#ifdef IMPL_TAB
void supprimer_un_contact_dans_rep(Repertoire *rep, int indice) {

	if (rep->nb_elts >= 1)		/* s'il y a au moins un element ds le tableau */
	{						/* et que l'indice pointe a l'interieur */
		for (int i=indice; i<rep->nb_elts-1; i++) { //suppression de l'élément à l'indice indice
			rep->tab[i]=rep->tab[i+1];
		}
		rep->nb_elts -= 1;		/* ds ts les cas, il y a un element en moins */
		modif = true;
	}

	return;
} /* fin supprimer */

#else
#ifdef IMPL_LIST
  /************************************************************************/
  /* supprime du répertoire l'enregistrement contenu dans le maillon elem */
  /*                   et fixe modif à vrai                              */
  /************************************************************************/
  // complet

	int supprimer_un_contact_dans_rep_liste(Repertoire *rep, SingleLinkedListElem *elem) {
	
	int ret=DeleteLinkedListElem(rep->liste, elem);
	if (ret == 1) {
		rep->nb_elts--;
		modif = true;
	}

	return (0); 
	}
#endif
#endif


  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement sur une ligne à l'écran  */
  /* ex Dupont, Jean                 0320304050                         */
  /**********************************************************************/
void affichage_enreg(Enregistrement enr)
{
	printf("\n%s",enr.nom);
	printf(" %s",enr.prenom);
	printf(" %s",enr.tel);
	return;

} /* fin affichage_enreg */
  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement avec alignement des mots */
  /* pour les listes                                                    */
  /* ex | Dupont                |Jean                  |0320304050      */
  /**********************************************************************/
  
void affichage_enreg_frmt(Enregistrement enr)
{
	printf("\n| %s",enr.nom);
	for (int i=strlen(enr.nom); i<MAX_NOM;i++) {  //pour aligner tous les mots
		printf(" "); 
	}
	printf("| %s",enr.prenom);
	for(int j=strlen(enr.nom); j<MAX_NOM; j++) {
		printf(" ");
	}
	printf("| %s",enr.tel);
	return;

} /* fin affichage_enreg */


  /**********************************************************************/
  /* test si dans l'ordre alphabetique, un enregistrement est apres     */
  /* un autre                                                           */
  /**********************************************************************/
bool est_sup(Enregistrement enr1, Enregistrement enr2)
{

	if (strcmp((enr1.nom), (enr2.nom)) < 0) { //si tmp1 est avant tmp2 alors les noms sont classés dans l'ordre alphabétiques
		return (true);
	}
	if (strcmp((enr1.nom),(enr2.nom)) > 0) {
		return(false);
	}
									//si les noms sont identiques, on classe les prénoms par ordres alphabétiques
	if(strcmp((enr1.prenom),(enr2.prenom)) < 0) {  
		return(true);
	}
	if (strcmp((enr1.prenom),(enr2.prenom)) > 0) {
		return(false);
	}
 	return(false);

}
 
/*********************************************************************/
/*   Tri Alphabetique du tableau d'enregistrements                   */
/*********************************************************************/

void trier(Repertoire *rep)
{

#ifdef IMPL_TAB

	for (int i=0; i<rep->nb_elts; i++) {
		for (int j=0; j<rep->nb_elts-1-i; j++){
			if (est_sup(rep->tab[j],rep->tab[j+1])==false) {
				printf("On va échanger %s avec %s",rep->tab[j],rep->tab[j+1]);
				Enregistrement echange = rep->tab[j];
				rep->tab[j]=rep->tab[j+1];
				rep->tab[j+1]=echange;
			}
		}
	}
	
	
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// rien à faire !
	// la liste est toujours triée
#endif
#endif

	rep->est_trie = true;


} /* fin trier */

  /**********************************************************************/
  /* recherche dans le répertoire d'un enregistrement correspondant au  */
  /*   nom à partir de l'indice ind                                     */
  /*   retourne l'indice de l'enregistrement correspondant au critère ou*/
  /*   un entier négatif si la recherche est négative				    */
  /**********************************************************************/

int rechercher_nom(Repertoire *rep, char nom[], int ind)
{
	int i = ind;		    /* position (indice) de début de recherche dans tableau/liste rep */
	int ind_fin;			/* position (indice) de fin de tableau/liste rep */

	char tmp_nom[MAX_NOM];	/* 2 variables temporaires dans lesquelles */
	char tmp_nom2[MAX_NOM];	/* on place la chaine recherchee et la chaine lue dans le */
							/* tableau, afin de les convertir en majuscules et les comparer */
	bool trouve = false;		


#ifdef IMPL_TAB
	
	ind_fin = rep->nb_elts;
	strncpy_s(tmp_nom, _countof(tmp_nom), nom);
	_strupr_s(tmp_nom, strlen(tmp_nom) + 1); 
	while (trouve= false && i<ind_fin) {
		strncpy_s(tmp_nom2, _countof(tmp_nom2), rep->tab[i].nom);
		_strupr_s(tmp_nom2, strlen(tmp_nom2) + 1); 

		if (strcmp((tmp_nom),(tmp_nom2)) == 0) {
			return(trouve = true);
		}
		else {
			i++;
		}
	}

#else
#ifdef IMPL_LIST  //similaire à la partie tableau

	ind_fin = rep->nb_elts;  
	strncpy_s(tmp_nom, _countof(tmp_nom), nom);
	_strupr_s(tmp_nom, strlen(tmp_nom) + 1); 
	while (trouve= false && i<ind_fin) {
		strncpy_s(tmp_nom2, _countof(tmp_nom2),GetElementAt(rep->liste, i)->pers.nom);
		_strupr_s(tmp_nom2, strlen(tmp_nom2) + 1); 

		if (strcmp((tmp_nom),(tmp_nom2)) == 0) {
			return(trouve = true);
		}
		else {
			i++;
		}
	}
	
#endif
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_nom */

  /*********************************************************************/
  /* Supprimer tous les caractères non numériques de la chaines        */
  /*********************************************************************/
void compact(char *s)
{
	int i=0;
	int j=0;
	while (s[i]!='\0') {
		if (0x30>s[i] || 0x39>s[i]) {
			i=j;
			while (s[j+1]!='\0') {
				s[j]=s[j+1];
				j++;
			}
		}
		else {
			i++;
		}
	}
	return;
}

/**********************************************************************/
/* sauvegarde le répertoire dans le fichier dont le nom est passé en  */
/* argument                                                           */
/* retourne OK si la sauvegarde a fonctionné ou ERROR sinon           */
/**********************************************************************/
int sauvegarder(Repertoire *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
#ifdef IMPL_TAB
	
	if (fopen_s(&fic_rep, nom_fichier, "w")!=0 || fic_rep==NULL) {
		return(ERROR);
	}
	for (int i = 0; i<rep->nb_elts; i++) {
		printf(fic_rep,"%s%c",rep->tab[i].nom,SEPARATEUR);
		printf(fic_rep,"%s%c",rep->tab[i].prenom,SEPARATEUR);
		printf(fic_rep,"%s%c",rep->tab[i].tel,SEPARATEUR);

	if (feof(fic_rep)) {
		fclose(fic_rep);
	}
	
#else
#ifdef IMPL_LIST
	
	if (fopen_s(&fic_rep, nom_fichier, "w")!=0 || fic_rep==NULL) {
		return(ERROR);
	}
	for (int i = 0; i<rep->nb_elts; i++) {
		printf(fic_rep,"%s%c",GetElementAt(rep->liste, i)->pers.nom,SEPARATEUR);
		printf(fic_rep,"%s%c",GetElementAt(rep->liste, i)->pers.prenom,SEPARATEUR);
		printf(fic_rep,"%s%c",GetElementAt(rep->liste, i)->pers.tel,SEPARATEUR);

	if (feof(fic_rep)) {
		fclose(fic_rep);
	}

#endif
#endif

	return(OK);
} /* fin sauvegarder */


  /**********************************************************************/
  /*   charge dans le répertoire le contenu du fichier dont le nom est  */
  /*   passé en argument                                                */
  /*   retourne OK si le chargement a fonctionné et ERROR sinon         */
  /**********************************************************************/

int charger(Repertoire *rep, char nom_fichier[]) ;

	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Enregistrement);
	char buffer[sizeof(Enregistrement) + 1];
	int idx = 0;

	char *char_nw_line;
	
	_set_errno(0);
	if ( ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL) )
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (rep->nb_elts < MAX_ENREG))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
#ifdef IMPL_TAB
				if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		/* element à priori correct, on le comptabilise */
					}
				}
#else
#ifdef IMPL_LIST

				Enregistrement enr;
				if (lire_champ_suivant(buffer, &idx, enr.nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, enr.prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, enr.tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		/* element à priori correct, on le comptabilise */
					}
				}

#endif
#endif
			}

		}
		rep->nb_elts = num_rec;
		fclose(fic_rep);
		return(OK);
	}


} /* fin charger */