#ifndef TREAP_H
#define TREAP_H

typedef struct TreapNode
{
    char *key;
    int value;
    int priority;
    struct TreapNode *left;
    struct TreapNode *right;
} treap_node_t;

treap_node_t *treap_alloc (const char *key, int value);
void treap_free (treap_node_t *treap);

treap_node_t *treap_insert (treap_node_t *treap, const char *key, int value);
const treap_node_t *treap_find (treap_node_t *treap, const char *key);
treap_node_t *treap_remove (treap_node_t *treap, const char *key);
int treap_size (const treap_node_t *treap);
void treap_print (const treap_node_t *treap);

#endif