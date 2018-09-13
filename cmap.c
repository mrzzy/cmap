/*
 * map.c
 * C Map Implemented using binary search trees (BST)
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmap.h"

/* MAP NODE */
/* Construct and return a new map node the given key and value */
map_node *construct_map_node(void *key, void *value)
{
    map_node *node = malloc(sizeof(map_node));
    node->key = key;
    node->value = value;
    node->left_child = NULL;
    node->right_child = NULL;
    
    return node;
}

/* Destroy a map node and its children 
 * Does not do anything if node is NULL
*/
void destory_map_node(map_node *node) {
    if(node == NULL) return;
    
    // Destroy left and right subtrees
    destory_map_node(node->left_child);
    destory_map_node(node->right_child);
    
    // Free memory for node to destory current node
    free(node);
}

/* MAP */
/* Construct and return a new map */
map *construct_map(key_cmp cmp) 
{
    map *new_map = malloc(sizeof(map));
    new_map->root = NULL;
    new_map->cmp = cmp;

    return new_map;
}

/* Search using given cmp and Locate position in the BST node recursively to 
 * place the given key and value into the BST, creating a new map_node if necessary
 * Returns the updated BST map_node
*/
map_node *put(map_node *node, void *key, void *value, key_cmp cmp) 
{
    // Base case: no node exists for key, so create new one
    if(node == NULL) return construct_map_node(key, value);
    else if(cmp(key, node->key) < 0) // key is less than node key, place on left
        node->left_child = put(node->left_child, key, value, cmp); 
    else if(cmp(key, node->key) > 0) // key is more than than node key, place on right
        node->right_child = put(node->right_child, key, value, cmp); 
    else // key same as node key, overwrite old value with new value
        node->value = value;
    
    return node;
}

/* Associate the given value for the given key into the given map target 
 * If key is already in the map, will overwrite old value of key with given value 
*/
void put_map(map *target, void *key, void *value)
{
    // Delegate to recursive routinue to place the key and value
    target->root = put(target->root, key, value, target->cmp);
}

/* Search using given cmp and locate node for the given key 
 * Returns the value of that node, otherwise NULL
*/
void *get(map_node *node, void *key, key_cmp cmp)
{
    //Base case: node is null, return null for no value
    if(node == NULL) return NULL;
    else if(cmp(key, node->key) < 0) // key is less than node key, search on left
        return get(node->left_child, key, cmp); 
    else if(cmp(key, node->key) > 0) // key is more than node key, search on right
        return get(node->right_child, key, cmp); 
    else // found key
        return node->value;
}


/* Retrieve the value for the given value from the map target
 * Returns the retreived value
*/
void *get_map(map *target, void *key) 
{
    // Delegate to recursive routinue to get the value for key
    return get(target->root, key, target->cmp);
}

/* Find and delete the minimum node of the given BST node 
 * Returns the updated BST map_node
*/
map_node *delete_min(map_node *node)
{
    // Base case: node is minimum
    if(node->left_child == NULL) 
        return node->right_child; // replace with other sibiling
    else 
    {
        node->left_child = delete_min(node->left_child);
        return node;
    }
}


/* Search using given cmp and locate node for the given key 
 * Deletes the node using hibbard deletion algorithm
 * Returns the updated BST map_node
*/
int which_one = 0;
map_node *delete(map_node *node, void *key, key_cmp cmp)
{
    //Base case: node is null, nothing to delete
    if(node == NULL) return NULL;
    else if(cmp(key, node->key) < 0) // key is less than node key, search on left
        node->left_child = delete(node->left_child, key, cmp); 
    else if(cmp(key, node->key) > 0) // key is more than node key, search on right
        node->right_child = delete(node->right_child, key, cmp); 
    else // Found node to delete
    {
        map_node *delete_node = node;
        
        // Untangle the node marked for deletion from the binary search tree
        // check if node has both left and right child
        if(delete_node->left_child != NULL && delete_node->right_child != NULL)
        {
            // Find Minimum on right subtree
            map_node *current_node = delete_node->right_child;
            while(current_node->left_child != NULL)
                current_node = current_node->left_child;
            map_node *min_node = current_node;

            // Remove minimum on right subtree
            delete_node->right_child = delete_min(node->right_child);
            
            // Replace node to delete with minimum on right subtree
            min_node->left_child = delete_node->left_child;
            min_node->right_child = delete_node->right_child;
            node = min_node;
        } 
        else if(node->left_child != NULL) // only has left subtree
        {
            // Replace with left child
            node = delete_node->left_child;
        }
        else if(node->right_child != NULL) // only has right subtree
        {
            // Replace with right child
            node = delete_node->right_child;
        }
        else // replace with NULL since no children
            node = NULL; 
        
        // Free the node marked for deletion only (not its children)
        free(delete_node);
    }

    return node;
}

/* Deletes the given key and asscoaited value from the target map */
void delete_map(map *target, void *key) 
{
    // Delegate to recursive routinue to remove the node specified by key
    target->root = delete(target->root, key, target->cmp);
}

/* Checks whether the map target contains the given key 
 * Returns true if key is present in the map, false otherwise 
*/
bool has_map(map *target, void *key) 
{
    return get_map(target, key) != NULL;
}
