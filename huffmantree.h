/*
** tree.h
** 19.09.2017
*/

#ifndef FC_HUFFMANtree_t_151107_H
#define FC_HUFFMANtree_t_151107_H

/* DEFINE ========================*/

#define NO_FLAG 0
#define MALLOC_leaf_t 1


/* STRUCTURES ====================*/

typedef struct leaf_t
{
  uint64_t occ;
  char     byte;
}
leaf_t;

typedef struct tree_t
{
  struct leaf_t *leaf;
  struct tree_t *left;
  struct tree_t *right;
}
tree_t;



/* PROTOTYPES ====================*/

/* Création de l'arbre d'Huffman à partir de la liste des occurrences.
** - int total_occ = nombre total d'octets dans le fichier
** - int nb_char = nombre total d'octets différents dans le fichier
** - occlist_t *link = liste chainée contenant les occurrences triées par ordre décroissant
** - tree_t *tree = pointeur NULL qui contiendra l'adresse de la racine de l'arbre */
     void Set_HuffmanTree(uint64_t total_occ, uint16_t nb_char, occlist_t *link, tree_t *tree);


/* Supprime tous les noeuds de l'arbre d'Huffman. */
     void Free_Tree(tree_t **tree);


/* Copie une chaine dans une autre en ajoutant un caractère à la fin.
** - const char *org = chaine à copier
** - char *des = chaine à remplir
** - char bit = caractère à ajouter à la fin de la chaine [des] ('1' ou '0')
** - char size = taille de la nouvelle chaine */
     void copy(const char *org, char *des, char bit, char size);


/* Rempli une liste chainée avec les codes binaires de chaque feuille de l'arbre
** - tree_t *tree = arbre d'Huffman
** - codelist_t **code = pointeur sur NULL qui contiendra la liste chainées contenant
**                    les codes binaires de chaque feuille */
void Set_CodeList(tree_t *tree, codelist_t **code, int32_t road, uint32_t cursor);
/*     void Set_CodeList(tree_t *tree, codelist_t **code);
       void Set_CodeList2(tree_t *tree, codelist_t **code, codelist_t **tmpCode, bool *p_init, const char *road, uint32_t size);*/


/* Creation d'un nouveau noeud et initialise ses pointeurs à NULL
** - char flag = MALLOC_leaf_t : allocation de mémoire pour le pointeur sur leaf_t
**               NO_FLAG : allocation de mémoire pour l'arbre uniquement*/
     tree_t *NewTree(char flag);

#endif
