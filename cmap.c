/*
 * map.c
 * C Map Implemented using Red Black Trees (RBT)
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "cmap.h"

/* MAP NODE */
/* Construct and return a new map node the given key and value */
map_node *construct_map_node(void *key, void *value)
{
    map_node *node = malloc(sizeof(map_node));
    node->key = key;
    node->value = value;
    node->color = NODE_COLOR_RED; // all new RBT nodes are red

    node->left_child = NULL;
    node->right_child = NULL;
    node->parent = NULL;

    return node;
}

/* destroy a map node and its children 
 * Does not do anything if node is NULL
*/
void destroy_map_node(map_node *node) {
    if(node == NULL) return;
    
    // destroy left and right subtrees
    destroy_map_node(node->left_child);
    destroy_map_node(node->right_child);
    
    // Free memory for node to destroy current node
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

/* destroy and frees the resources of the given map */
void destroy_map(map *target) 
{
    destroy_map_node(target->root);
    free(target);
}

/* Red Black Tree operations */
/* Retrieves the color of the given map_node node
 * Returns the retrieved color
*/
map_node_color retrieve_color(map_node *node) 
{
    if(node == NULL) return NODE_COLOR_BLACK; // null nodes are black
    else return node->color;
}

/* Recolor the given black map node target with 2 red children by swaping colors
 * with the children: Children nodes are colored black while the target node
 * is recolored red
*/
void recolor(map_node *target) 
{
    // Check whether operation is permitted 
    assert(target->left_child != NULL && target->right_child != NULL);
    assert(retrieve_color(target) == NODE_COLOR_BLACK);
    assert(retrieve_color(target->left_child) == NODE_COLOR_RED);
    assert(retrieve_color(target->right_child) == NODE_COLOR_RED);
    
    // Recolor nodes
    target->color = NODE_COLOR_RED;
    target->left_child->color = NODE_COLOR_BLACK;
    target->right_child->color = NODE_COLOR_BLACK;
}

/* Left rotate the given map node target with its right child such that the
 * target becomes the left child of the originally rignt child.
 * Note that the right child must be colored red.
 * Returns the original right child the target
*/
map_node *rotate_left(map_node *target)
{
    map_node *partner = target->right_child; 
    // Check whether operation is permitted 
    assert(partner != NULL);
    assert(retrieve_color(partner) == NODE_COLOR_RED);
    
    // Swap colors
    map_node_color target_color = target->color;
    target->color = partner->color;
    partner->color = target_color;
    
    // Rotate left against partner node
    /*
     *   o              x  
     *  / \            / \
     * .   x   to     o   .
     *    / \        / \
     *   +   .      .   +  
    */
    target->right_child = partner->left_child;
    partner->left_child = target;

    return partner;
}

/* Right rotate the given map node target with its left child such that the
 * target becomes the right child of the originally left child.
 * Note that the left child must be colored red.
 * Returns the original left child of the target
*/
map_node *rotate_right(map_node *target)
{
    map_node *partner = target->left_child; 
    // Check whether operation is permitted 
    assert(partner != NULL);
    assert(retrieve_color(partner) == NODE_COLOR_RED);
    
    // Swap colors
    map_node_color target_color = target->color;
    target->color = partner->color;
    partner->color = target_color;
    
    // Rotate right against partner node
    /*
     *     o          x
     *    / \        / \
     *   x   . to   .   o
     *  / \            / \
     * .   +          +   .
    */
    target->left_child = partner->right_child;
    partner->right_child = target;

    return partner;
}

/* Apply RBT transformations to attempt to balance the tree specified by the 
 * given map node.
 * Returns a balanced version of the tree as a map node.
*/
map_node *balance_tree(map_node *tree)
{
    // Right child red, left child black: rotate left
    if(retrieve_color(tree->right_child) == NODE_COLOR_RED &&
            retrieve_color(tree->left_child) == NODE_COLOR_BLACK) 
        tree = rotate_left(tree);
    // Left child red, left-left grandchild red: rotate right
    else if(retrieve_color(tree->left_child) == NODE_COLOR_RED &&
            retrieve_color(tree->left_child->left_child))
        tree = rotate_right(tree);
    // Both left and right child red: recolor
    else if(retrieve_color(tree->right_child) == NODE_COLOR_RED &&
            retrieve_color(tree->left_child) == NODE_COLOR_RED) 
        recolor(tree);

    return tree;
}


/* Utilities */
/* A key and value pair */
typedef struct pair
{
    void *key;
    void *value;
}pair;

/* Generic Transformation on a map_node */
typedef map_node* (transformation)(map_node *, void *arg);

/* Applies the given transform on the map_node specified by the given key in the  
 * given BST node. If no node is found for the given key, passes NULL to the
 * given transform callback. When calling the transform callback, passes the given
 * arg to the callback
 * Returns the BST with the transform applied to the targeted node
*/
map_node *apply(map_node *node, void *key, key_cmp cmp, transformation transform,
                void *arg) 
{
    if(node == NULL) 
        node = transform(NULL, arg);
    else if(cmp(key, node->key) < 0) // key is less than node key, place on left
    {
        node->left_child = apply(node->left_child, key, cmp, transform, arg); 
   
        // Update parent pointer if child is not NULL
        if(node->left_child != NULL) node->left_child->parent = node;
    }
    else if(cmp(key, node->key) > 0) // key is more than than node key, place on right
    {
        node->right_child = apply(node->right_child, key, cmp, transform, arg); 
        if(node->right_child != NULL) node->right_child->parent = node;
    }
    else 
        node = transform(node, arg);
    
    return node;

}

/* CRUD operations */
/* Transform to put the given key value pair into the BST node */
map_node *transform_put(map_node *node, void *arg_pair) 
{
    pair *kv_pair = (pair *)arg_pair;
    // Create node if it does not already exist
    if(node == NULL)
        node = construct_map_node(kv_pair->key, kv_pair->value);
    else
        node->value = kv_pair->value;

    node = balance_tree(node); // Balance tree

    return node;
}

/* Associate the given value for the given key into the given map target 
 * If key is already in the map, will overwrite old value of key with given value 
*/
void put_map(map *target, void *key, void *value)
{
    // Construct key value pair
    pair kv_pair;
    kv_pair.key = key;
    kv_pair.value = value;
    
    // Apply transformation to put the key value pair
    target->root = apply(target->root, key, target->cmp, transform_put, &kv_pair);
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

/* Transform the given map_node node by deleting it from the BST using the hibbard
 * deletion algorithm. Checks the map_node key against given before performing 
 * deletion
 * Returns the updated node
*/
map_node *transform_delete(map_node *node, void *key)
{
    //Check delete operation performed on the correct node
    assert(strcmp(key, node->key) == 0);
    
    // Check if delete node is already null, if so, there is nothing to do..
    if(node == NULL) return NULL;
        
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

    // Update parent pointer from delete_node to replacement node
    if(delete_node->right_child != NULL) delete_node->right_child->parent = node;
    if(delete_node->left_child != NULL) delete_node->left_child->parent = node;

    // Free the node marked for deletion only (not its children)
    free(delete_node);
    
    return node;
}

/* Deletes the given key and asscoaited value from the target map */
void delete_map(map *target, void *key) 
{
    // Apply transformation to delete node
    target->root = apply(target->root, key, target->cmp, transform_delete, key);
}

/* Checks whether the map target contains the given key 
 * Returns true if key is present in the map, false otherwise 
*/
bool has_map(map *target, void *key) 
{
    return get_map(target, key) != NULL;
}
