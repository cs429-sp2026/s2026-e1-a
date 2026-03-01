#include "treap.h"
#include "malloc.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "assert.h"

treap_node_t *treap_alloc (const char * const key, const int value)
{
    treap_node_t * const node = calloc (1, sizeof (treap_node_t));
    node->key = calloc (strlen (key) + 1, 1);
    strcpy (node->key, key);
    node->value = value;
    node->priority = rand () % 256;
    return node;
}

void treap_free (treap_node_t * const node)
{
    if (!node)
    {
        return;
    }

    treap_free (node->left);
    treap_free (node->right);
    free (node->key);
    free (node);
}

static treap_node_t *rotate_right (treap_node_t * const node)
{
    treap_node_t * const left = node->left;
    treap_node_t * const temp = left->right;
    left->right = node;
    node->left = temp;
    return left;
}

static treap_node_t *rotate_left (treap_node_t * const node)
{
    treap_node_t * const right = node->right;
    treap_node_t * const temp = right->left;
    right->left = node;
    node->right = temp;
    return right;
}

treap_node_t *treap_insert (treap_node_t *treap, const char *key, int value)
{
    if (!treap)
    {
        return treap_alloc (key, value);
    }

    const int cmp = strcmp (key, treap->key);
    if (cmp < 0)
    {
        treap->left = treap_insert (treap->left, key, value);
        if (treap->left->priority > treap->priority)
        {
            treap = rotate_right (treap);
        }
    }
    else if (cmp == 0)
    {
        treap->value = value;
        return treap;
    }
    else if (cmp > 0)
    {
        treap->right = treap_insert (treap->right, key, value);
        if (treap->right->priority > treap->priority)
        {
            treap = rotate_left (treap);
        }
    }

    return treap;
}

const treap_node_t *treap_find (treap_node_t * const treap, const char * const key)
{
    if (!treap)
    {
        return NULL;
    }

    const int cmp = strcmp (key, treap->key);
    if (cmp < 0)
    {
        return treap_find (treap->left, key);
    }
    else if (cmp > 0)
    {
        return treap_find (treap->right, key);
    }

    return treap;
}

treap_node_t *treap_remove (treap_node_t *treap, const char * const key)
{
    if (!treap)
    {
        return treap;
    }

    const int cmp = strcmp (key, treap->key);
    if (cmp < 0)
    {
        treap->left = treap_remove (treap->left, key);
    }
    else if (cmp > 0)
    {
        treap->right = treap_remove (treap->right, key);
    }
    else
    {
        if (!treap->left)
        {
            treap_node_t * const temp = treap->right;
            free (treap->key);
            free (treap);
            return temp;
        }
        else if (!treap->right)
        {
            treap_node_t * const temp = treap->left;
            free (treap->key);
            free (treap);
            return temp;
        }

        if (treap->left->priority > treap->right->priority)
        {
            treap = rotate_right (treap);
            treap->right = treap_remove (treap->right, key);
        }
        else
        {
            treap = rotate_left (treap);
            treap->left = treap_remove (treap->left, key);
        }
    }
    return treap;
}

int treap_size (const treap_node_t * const treap)
{
    if (!treap)
    {
        return 0;
    }

    return 1 + treap_size (treap->left) + treap_size (treap->right);
}

static void treap_print_recursive (const treap_node_t * const node, const int level, char *side)
{
    for (int i = 0; i < level; i++) printf ("    ");

    printf ("%s-- %s (p:%d)\n", side, node->key, node->priority);

    treap_print_recursive (node->left, level + 1, "L");
    treap_print_recursive (node->right, level + 1, "R");
}

void treap_print (const treap_node_t * const treap)
{
    printf ("--- Treap Structure (Sideways) ---\n");
    if (!treap)
    {
        printf ("Empty Treap\n");
    }
    else
    {
        treap_print_recursive (treap, 0, "ROOT");
    }
    printf ("----------------------------------\n");
}
