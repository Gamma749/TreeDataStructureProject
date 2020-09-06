#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "mylib.h"

/**
 * Define some functions to test the colour of a tree node.
 */
#define IS_BLACK(x) ((NULL==(x) || (BLACK == (x)->color))
#define IS_RED(x) ((NULL!=(x))&&(RED==(x)->color))

/**
 * Definition of the enumerated type representing the tree color.
 */
typedef enum {RED, BLACK} tree_color;

/**
 * Define the static variable that will determine if we are using
 * Red-Black Trees or Binary Searh Trees. The values of this have
 * been defined in tree.h
 */
static tree_t tree_type;

/**
 * Define the tree_node structure, which will hold all of the data fields
 * needed for both binary search trees and red black trees.
 *
 * key is the string that is held at this node, the value of that node.
 * color is the color of that node, only used in red-black trees
 * left is the tree to the left of this node. NULL if there is none
 * right is the tree to the right of this node. NULL if there is none
 * frequency is the number of times this key has been inserted into the tree
 */
struct tree_node{
    char *key;
    tree_color color;
    tree left;
    tree right;
    int frequency;
};

/**
 * Create a new tree and give it back to the user.
 * Please note that all allocation of memory occurs in the insert function
 * This is to avoid having a seperate case for the root of the tree, which
 * could impact efficency.
 *
 * @param t the tree type to use, from the enum tree_t in tree.h
 *        select between RBT (red-black tree) and BST (binary-search tree).
 *
 * @return NULL which is treated as a tree by the user, as memory is
 *         allocated later
 */
tree tree_new(tree_t t){
    tree_type = t;
    return NULL;
}


/**
 * Rotate the tree right about the node t.
 * @param t the node to rotate right about
 * @return the new tree root after rotating right
 */
static tree tree_rotate_right(tree t){
    tree temp=t;
    t=t->left;
    temp->left = t->right;
    t->right = temp;
    return t;
}


/**
 * Rotate the tree left about the node t.
 * @param t the node to rotate left about
 * @return the new tree root after rotating left
 */
static tree tree_rotate_left(tree t){
    tree temp = t;
    t = t->right;
    temp->right = t->left;
    t->left = temp;
    return t;
}

/**
 * Fix the red-black tree structure after something has been inserted.
 * Inserting into a red-black tree can result in breaking of the assumptions
 * of the data structure. This method will recursively fix the tree to
 * ensure the assumptions hold. The code is fairly self explanitory. For
 * futher explaination see the table in the COSC242 lab book page 88-89.
 * @param t the tree node to fix
 * @param the tree root after fixing has occured.
 */
static tree tree_fix(tree t){
    if(IS_RED(t->left)){
        if(IS_RED(t->left->left)){
            if(IS_RED(t->right)){
                t->color = RED;
                if(t->left!=NULL){
                    t->left->color = BLACK;
                }
                if(t->right!=NULL){
                    t->right->color = BLACK;
                }
            } else {
                t=tree_rotate_right(t);
                t->color = BLACK;
                t->right->color = RED;
            }
        }

        if(IS_RED(t->left->right)){
            if(IS_RED(t->right)){
                t->color = RED;
                if(t->left!=NULL){
                    t->left->color = BLACK;
                }
                if(t->right!=NULL){
                    t->right->color = BLACK;
                }
            } else {
                t->left = tree_rotate_left(t->left);
                t = tree_rotate_right(t);
                t->color = BLACK;
                t->right->color = RED;
            }
        }
    }

    if(IS_RED(t->right)){
        if(IS_RED(t->right->left)){
            if(IS_RED(t->left)){
                t->color = RED;
                if(t->left!=NULL){
                    t->left->color = BLACK;
                }
                if(t->right!=NULL){
                    t->right->color = BLACK;
                }
            } else {
                t->right = tree_rotate_right(t->right);
                t = tree_rotate_left(t);
                t->color = BLACK;
                t->left->color = RED;
            }
        }

        if(IS_RED(t->right->right)){
            if(IS_RED(t->left)){
                t->color = RED;
                if(t->left!=NULL){
                    t->left->color = BLACK;
                }
                if(t->right!=NULL){
                    t->right->color = BLACK;
                }
            } else {
                t=tree_rotate_left(t);
                t->color = BLACK;
                t->left->color = RED;
            }
        } 
    }

    return t;
}


/**
 * Recursively search the tree for an item.
 * @param t the tree to search
 * @param key the string to search for
 * @return 0 if the item was not found, 1 if it was.
 */
int tree_search(tree t, char *key){
    if(t == NULL){
        /*Item was not found*/
        return 0;
    } else if(strcmp(t->key, key)==0){
        /*We found it!*/
        return 1;
    } else if(strcmp(t->key, key)>0) {
        /*The node is too big, search left*/
        return tree_search(t->left, key);
    } else {
        /*The node is too small, seach right*/
        return tree_search(t->right, key);
    }
}

/**
 * An internal method to insert items into the tree. Where the brute of
 * insertion actually occurs.
 * @param t the tree to insert into
 * @param str the string to insert into the tree
 * @return the root of the tree after inserting str into it
 */
static tree internal_tree_insert(tree t, char *str){
    /*Handle the empty tree case*/
    if(t==NULL){
        /*Allocate memory for the node, set the node data fields, return it*/
        tree t = emalloc(sizeof *t);
        t->key = emalloc(sizeof(str[0]) * strlen(str));
        strcpy(t -> key, str);
        t->color = RED;
        t->frequency++;
        return t;
    }
    
    if(strcmp(t->key, str) == 0){
        /*We've got a repeat!*/
        t->frequency++;
        return t;
    }
    /*Now check if key to insert needs to go left or right*/
    else if(strcmp(t->key, str)>0){
        t->left = internal_tree_insert(t->left, str);
    } else {
        t->right = internal_tree_insert(t->right,str);
    }

    /*Check if tree is a RBT, and call fix*/
    if(tree_type == RBT){
        /*Fix any issues that may have arrised from insertion*/
        t=tree_fix(t);
    }
    
    return t;
}

/**
 * A user facing method to call to insert. Note the brute of insertion
 * occurs in internal_tree_insert. The purpose of this method is to set
 * the root node to black as per the red-black tree requirements.
 * @param t the tree to insert into
 * @param str the string to insert into t
 * @return the root of the tree after inserting str into it
 */
tree tree_insert(tree t, char *str){
    /*An internal method to ensure that the root is colored black*/
    t = internal_tree_insert(t, str);
    t->color = BLACK;
    
    return t;
}

/**
 * Traverse the tree inorder, applying a function that takes each nodes
 * frequency and key and operates on them.
 * @param t the tree to traverse inorder
 * @param f the void function to apply to each node, given frequency and key
 */
void tree_inorder(tree t, void f(int i, char *str)){
    if(t==NULL){
        return;
    }
    tree_inorder(t->left, f);
    f(t->frequency, t->key);
    tree_inorder(t->right, f);
}

/**
 * Traverse the tree preorder, applying a function that takes each nodes
 * frequency and key and operates on them.
 * @param t the tree to traverse preorder
 * @param f the void function to apply to each node, given frequency and key
 */
void tree_preorder(tree t, void f(int i, char *str)){
    if(t==NULL){
        return;
    }
    f(t->frequency, t->key);
    tree_preorder(t->left, f);
    tree_preorder(t->right, f);
}


/**
 * Apply a recursive search to find the tree depth for a node. The tree depth
 * is the distance to the furthest leaf node.
 * @param t the tree node to find the depth of
 * @return the integer tree depth of the node t
 */
int tree_depth(tree t){
    int leftDepth=0, rightDepth=0;
    if(t->left == NULL && t->right==NULL){
        /*we're at the bottom! Return up, this should give a depth of 0*/
        return 0;
    }
    if(t->left != NULL){
        leftDepth = tree_depth(t->left);
    }
    if(t->right != NULL){
        rightDepth = tree_depth(t->right);
    }
    if(leftDepth>rightDepth){
        return 1+leftDepth;
    } else {
        return 1+rightDepth;
    }
}

/**
 * Free the entire tree and all associated memory, using a postorder recursion.
 * @param t the tree to free
 */
void tree_free(tree t){
    if(t==NULL){
        return;
    }
    if(t->left != NULL){
        tree_free(t->left);
    }
    if(t->right != NULL){
        tree_free(t->right);
    }
    free(t->key);
    free(t);
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == tree_type && RED == t->color) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}


/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}
