#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "defines.h"
#include "occlist.h"
#include "codelist.h"
#include "huffmantree.h"


void Set_HuffmanTree(uint64_t total_occ, uint16_t nb_char, occlist_t *link, tree_t *tree)
{
  occlist_t *tmpLink = NULL;
	uint64_t left_occ = 0;
	uint64_t right_occ = 0;
  uint64_t nb_right_char = 0;
  uint64_t nb_left_char = 0;
  
	if (nb_char > 2) 
	{
    tmpLink = OL_Get(link, nb_left_char);
    do
		{
			left_occ += tmpLink->occ;
			nb_left_char++;
		}
    while ((tmpLink = OL_Get(link, nb_left_char)) && left_occ + tmpLink->occ <= total_occ / 2);
		
    tree->left = NewTree(NO_FLAG);
		
		if (nb_left_char > 0) Set_HuffmanTree(left_occ, nb_left_char, link, tree->left);
    link = tmpLink;


		while ((tmpLink = OL_Get(link, nb_right_char)) && (right_occ + left_occ) < total_occ)
		{
			right_occ += tmpLink->occ;
			nb_right_char++;
		}
    tree->right = NewTree(NO_FLAG);
		
		if (nb_right_char > 0) Set_HuffmanTree(right_occ, nb_right_char, link, tree->right);
	}
	else
	{
		if (nb_char == 2)
    {
      tree->left = NewTree(MALLOC_leaf_t);
      tree->left->leaf->byte = link->next->byte;

      tree->right = NewTree(MALLOC_leaf_t);
      tree->right->leaf->byte = link->byte;
    }
    else if (nb_char == 1)
    {
      tree->leaf = malloc(sizeof(leaf_t));
      tree->leaf->byte = link->byte;
    }
	}
}

void Set_CodeList(tree_t *tree, codelist_t **code, int32_t road, uint32_t cursor)
{
  if (tree->left)
  {
    Set_CodeList(tree->left, code, road, cursor << 1);
  }
  if (tree->right) 
  {
    road |= cursor;
    Set_CodeList(tree->right, code, road, cursor << 1);
  }
  if (tree->leaf) 
  {
    CL_Add(code, tree->leaf->byte, road, cursor);
  }
}

void Free_Tree(tree_t **tree)
{
	tree_t *tmpTree = *tree;

	if (tmpTree)
  {
    if (tmpTree->left) {
      Free_Tree(&tmpTree->left);
    }
    if (tmpTree->right) {
      Free_Tree(&tmpTree->right);
    } 
    if (tmpTree->leaf) {
      free(tmpTree->leaf);
    }
      
    free(tmpTree);
  }

	*tree = NULL;
}

tree_t *NewTree(char flag)
{
  tree_t *tree;
  if ((tree = malloc(sizeof(tree_t))) == NULL) goto error;
  tree->left = NULL;
  tree->right = NULL;

  if (flag == MALLOC_leaf_t) {
    if ((tree->leaf = malloc(sizeof(leaf_t))) == NULL) goto error;
  }
  else tree->leaf = NULL;

  return tree;

  error:
    fprintf(stderr, "ECHEC malloc fichier %s ligne %d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}

