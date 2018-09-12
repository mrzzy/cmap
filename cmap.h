#ifndef __CMAP_H__
#define __CMAP_H__ 1
/*
 * cmap.h
 * C Map Implemented using binary search trees (BST)
*/

/* map_node represents a node in the map's BST */
typedef struct map_node 
{
    const void *key;
    void *value;
    struct map_node *left_child; // left subtree
    struct map_node *right_child; // right subtree
} map_node;

/* Construct and return a new map node the given key and value */
map_node *construct_map_node(void *key, void *value);
/* Destroy a map node and its children */
void destory_map_node(map_node *node);

/* MAP */
/* Key comparsion callback:
 * Returns positive int on first key greater than second key
 * Returns negative int on first key smaller than second key
 * Returns 0 otherwise
*/
typedef int (*key_cmp)(const void *, const void *);

typedef struct map 
{
    map_node *root; // BST Root element
    key_cmp cmp; // key comparsion function
} map;

/* Construct and return a new map with the given key_cmp comparsion function */
map *construct_map(key_cmp cmp);
/* Associate the given value for the given key into the given map target 
 * If key is already in the map, will overwrite old value of key with given value 
*/
void put_map(map *target, void *key, void *value);
/* Retrieve the value for the given value from the map target
 * Returns the retreived value
*/
void *get_map(map *target, void *key);
/* Deletes the given key and asscoaited value from the target map */
void delete_map(map *target, void *key);
#endif /* ifndef __CMAP_H__ */
