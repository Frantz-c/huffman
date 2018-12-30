#include "huffman.h"

int         upgrade_sort(void *a, void *b)
{
    return (*(int*)a - *(int*)b);
}

void        set_occurrences(int o[], int file)
{
    unsigned char    buf[2048];
    int              len;
    int              i;

    while ((len = read(file, buf, 2048)))
    {
        i = 0;
        while (i < len)
        {
            o[buf[i]]++;
            i++;
        }
    }
}

t_myarray   *get_occur_array(int cpy[], int occur[])
{
    t_myarray   *array;
    int         i;
    int         j;

    array = malloc(sizeof(t_myarray));
    array->length = 0;
    i = 0;
    while (i < 256)
    {
        j = 0;
        if (cpy[i] == 0)
            break ;
        while (occur[j] != cpy[i])
            j++;
        array->data[i].occ = cpy[i];
        array->data[i].chr = j;
        array->length++;
        i++;
    }
    return (array);
}

t_myarray   *get_occurrences(char *org)
{
    t_myarray   *ret;
    int         occur[256] = {0};
    int         copy[256];
    int         file;

    if ((file = open(org, O_RDONLY)) == -1)
    {
        printf("fichier \"%s\" inexistant\n", org);
        exit(-1);
    }
    set_occurrences(occur, file);
    close(file);
    memcpy(copy, occur, 256 * 4);
    merge_sort(copy, sizeof(int), 256, upgrade_sort);
    return (get_occur_array(copy, occur));
}

t_btree        *new_branch(void)
{
    t_btree     *ptr;

    ptr = malloc(sizeof(t_btree));
    ptr->data = -1;
    ptr->l = NULL;
    ptr->r = NULL;
    return (ptr);
}

t_btree         *new_leaf(int data)
{
    t_btree     *ptr;

    ptr = malloc(sizeof(t_btree));
    ptr->data = data;
    ptr->l = NULL;
    ptr->r = NULL;
    return (ptr);
}

void        set_btree(t_myarray *a, int length, int offset, int occ, t_btree **btree)
{
    int     i;
    int     total;

    if (length == 1)
    {
        *btree = new_leaf(a->data[offset].chr);
        return ;
    }
    i = offset;
    *btree = new_branch();
    while (i < length + offset && total <= occ / 2)
        total += a->data[i++].occ;
    set_btree(a, i - offset, offset, total, btree->left); //left
    set_btree(a, length - (i - offset), i - offset, occ - total, btree->right); //right
}

int         get_total_occurrences(t_myarray *a)
{
    int     i;
    int     total;

    i = 0;
    total = 0;
    while (i < a->length)
        total += a->data[i++];
    return (total);
}

t_btree     *get_binary_tree(t_myarray *a)
{
    t_btree     *tree;
    int         total_occurrences;

    total_occurrences = get_total_occurrences(a);
    set_btree(a, a->length, 0, total_occurrences, &tree);
}

void        compression(char *org, char *dest)
{
    t_myarray   *occurrences;
    t_btree     *btree;

    occurrences = get_occurrences(org);
    btree = get_binary_tree(occurrences);
}
