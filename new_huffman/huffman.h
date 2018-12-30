#ifndef HUFFMAN_H
# define HUFFMAN_H

# define COMPRESS    0
# define DECOMP      1
# define VERBOSE     2
# define ORG         3
# define DEST        4

typedef struct  s_myarray_elem
{
    int     occ;
    int     chr;
}               t_myarray_elem;

typedef struct  s_myarray
{
    int             length;
    t_myarray_elem  data[256];
}               t_myarray;

typedef struct  s_btree
{
    struct s_btree  *left;
    struct s_btree  *right;
    int             data;
}               t_btree;

#endif
