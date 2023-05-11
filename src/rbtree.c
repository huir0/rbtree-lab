#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)malloc(sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t* nil = (node_t*)malloc(sizeof(node_t));
  p->root = nil;
  p->nil = nil;
  p->nil->color = RBTREE_BLACK;
  return p;
}

void delete_node(rbtree *t, node_t* node) {
  if (node != t->nil) {
    delete_node(t, node->left);
    delete_node(t, node->right);
    free(node);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t* x) {
  node_t * y;
  y = x->right;
  x->right = y->left;
  if (y->left != t->nil) y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil) t->root = y;
  else if(x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree* t, node_t* x) {
  node_t * y;
  y = x->left;
  x->left = y->right;
  if (y->right != t->nil)    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)   t->root = y;
  else if (x == x->parent->right) {x->parent->right = y;}
  else x->parent->left = y;
  y->right = x;
  x->parent = y;
}

void rb_insert_fixup(rbtree * t, node_t* z) {
  while (z->parent->color == RBTREE_RED)
    if (z->parent == z->parent->parent->left){
      node_t* y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t,z);          
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);        
      } 
    } else {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t * z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  node_t * y = (node_t *) t->nil;
  node_t * x = (node_t *) t->root;
  while (x != t->nil){
    y = x;
    if (z->key < x->key)   x = x->left;
    else x = x->right;}
  z->parent = y;
  if (y == t->nil) t->root = z;
  else if (z->key < y->key) y->left = z;
  else y->right = z;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rb_insert_fixup(t,z);
  return z;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t * x = t->root;
  
 
  while (x != t->nil && x->key != key) {
    if (x->key > key) x = x->left;
    else x = x->right;
  }
  if (x == t->nil)
    return NULL;
  if (x->key == key)
    return x;
  return NULL;
}





node_t * _rbtree_min(const rbtree* t, node_t* p) {
  node_t * current = p;
  while (current && current->left != t->nil)
    current = current->left;
  return current;
}

node_t * rbtree_min(const rbtree* t) {
  // TODO: implement find
  return _rbtree_min(t, t->root);
}

node_t * _rbtree_max(const rbtree* t, node_t* p) {
  node_t* current = p;
  while (current && current->right != t->nil)
    current = current->right;
  return current;
}


node_t *rbtree_max(const rbtree* t) {
  // TODO: implement find
  return _rbtree_max(t, t->root);
}

void rb_transplant(rbtree * t,node_t* u, node_t* v) {
  if (u->parent == t->nil) t->root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
}



void rb_erase_fixup(rbtree * t, node_t*x) {
  
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t * w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else {
      node_t * w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else { if (w->left->color == RBTREE_BLACK) {
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t,w);
        w = x->parent->left;
      }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->left->color = RBTREE_BLACK;
      right_rotate(t,x->parent);
      x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t * x;
  node_t * y = p;
  color_t y_original_color = y->color;
  if (p->left == t->nil) {
    x = p->right;
    rb_transplant(t, p, p->right);
  }
  else if (p->right == t->nil) {
    x = p->left;
    rb_transplant(t,p,p->left);
  }
  else {
    y = _rbtree_min(t,p->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == p) x->parent = y;
    else {
      rb_transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_original_color == RBTREE_BLACK)
    rb_erase_fixup(t,x);
  free(p);
  return 0;
}


void inorder(const rbtree *t, node_t *node, key_t *arr, int *i, const size_t n) {
  if (node != t->nil) {
    inorder(t, node->left, arr, i, n);
    if (*i < n) {arr[*i] = node->key;
    * i = *i + 1;}
    inorder(t,node->right, arr, i, n);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int i = 0;
  inorder(t,t->root,arr, &i, n);
  return 0;
}
