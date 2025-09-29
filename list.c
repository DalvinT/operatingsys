// list/list.c
//
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

static node_t *getNode(elem value); // keep prototype private to this file

list_t *list_alloc() {
  list_t *mylist = (list_t *)malloc(sizeof(list_t));
  if (!mylist) return NULL;
  mylist->head = NULL;
  return mylist;
}

void list_free(list_t *l) {
  if (!l) return;
  node_t *cur = l->head;
  while (cur) {
    node_t *next = cur->next;
    free(cur);
    cur = next;
  }
  free(l);
}

void list_print(list_t *l) {
  if (!l) { printf("NULL\n"); return; }
  node_t *cur = l->head;
  while (cur) {
    printf("%d->", cur->value);
    cur = cur->next;
  }
  printf("NULL\n");
}

char *listToString(list_t *l) {
  // simple, fixed-size buffer for debugging; caller must free
  char *buf = (char *)malloc(sizeof(char) * 1024);
  if (!buf) return NULL;
  buf[0] = '\0';

  char tbuf[32];
  node_t *curr = l ? l->head : NULL;
  while (curr != NULL) {
    snprintf(tbuf, sizeof(tbuf), "%d->", curr->value);
    strncat(buf, tbuf, 1023 - strlen(buf));
    curr = curr->next;
  }
  strncat(buf, "NULL", 1023 - strlen(buf));
  return buf;
}

int list_length(list_t *l) {
  int n = 0;
  for (node_t *cur = l ? l->head : NULL; cur; cur = cur->next) n++;
  return n;
}

void list_add_to_back(list_t *l, elem value) {
  if (!l) return;
  node_t *nd = getNode(value);
  if (!l->head) { l->head = nd; return; }
  node_t *cur = l->head;
  while (cur->next) cur = cur->next;
  cur->next = nd;
}

void list_add_to_front(list_t *l, elem value) {
  if (!l) return;
  node_t *cur_node = getNode(value);
  cur_node->next = l->head;
  l->head = cur_node;
}

static node_t *getNode(elem value) {
  node_t *mynode = (node_t *)malloc(sizeof(node_t));
  mynode->value = value;
  mynode->next = NULL;
  return mynode;
}

void list_add_at_index(list_t *l, elem value, int index) {
  if (!l) return;
  if (index <= 0 || !l->head) { list_add_to_front(l, value); return; }

  node_t *prev = l->head;
  int i = 0;
  while (prev->next && i < index - 1) { prev = prev->next; i++; }
  // insert after prev
  node_t *nd = getNode(value);
  nd->next = prev->next;
  prev->next = nd;
}

elem list_remove_from_front(list_t *l) {
  if (!l || !l->head) return -1;
  node_t *rm = l->head;
  elem v = rm->value;
  l->head = rm->next;
  free(rm);
  return v;
}

elem list_remove_from_back(list_t *l) {
  if (!l || !l->head) return -1;
  if (!l->head->next) {
    elem v = l->head->value;
    free(l->head);
    l->head = NULL;
    return v;
  }
  node_t *prev = l->head;
  while (prev->next && prev->next->next) prev = prev->next;
  node_t *rm = prev->next;
  elem v = rm->value;
  prev->next = NULL;
  free(rm);
  return v;
}

elem list_remove_at_index(list_t *l, int index) {
  if (!l || !l->head) return -1;
  if (index <= 0) return list_remove_from_front(l);

  node_t *prev = l->head;
  int i = 0;
  while (prev->next && i < index - 1) { prev = prev->next; i++; }
  if (!prev->next) return -1; // out of range
  node_t *rm = prev->next;
  elem v = rm->value;
  prev->next = rm->next;
  free(rm);
  return v;
}

bool list_is_in(list_t *l, elem value) {
  for (node_t *cur = l ? l->head : NULL; cur; cur = cur->next)
    if (cur->value == value) return true;
  return false;
}

elem list_get_elem_at(list_t *l, int index) {
  if (!l || index < 0) return -1;
  node_t *cur = l->head;
  int i = 0;
  while (cur && i < index) { cur = cur->next; i++; }
  return cur ? cur->value : -1;
}

int list_get_index_of(list_t *l, elem value) {
  int i = 0;
  for (node_t *cur = l ? l->head : NULL; cur; cur = cur->next, i++)
    if (cur->value == value) return i;
  return -1;
}

// main.c
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(void) {
  list_t *l = list_alloc();

  // basic adds
  list_add_to_front(l, 2);
  list_add_to_front(l, 1);
  list_add_to_back(l, 3);
  list_add_at_index(l, 99, 2); // 1 -> 2 -> 99 -> 3

  list_print(l);
  printf("len=%d\n", list_length(l));

  // queries
  printf("is 99 in? %s\n", list_is_in(l, 99) ? "yes" : "no");
  printf("elem at 2 = %d\n", list_get_elem_at(l, 2));
  printf("index of 3 = %d\n", list_get_index_of(l, 3));

  // removes
  printf("pop_front = %d\n", list_remove_from_front(l));
  printf("pop_back  = %d\n", list_remove_from_back(l));
  printf("pop at 1  = %d\n", list_remove_at_index(l, 1));
  list_print(l);

  char *s = listToString(l);
  printf("as string: %s\n", s);
  free(s);

  list_free(l);
  return 0;
}

