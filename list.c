#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "minmax.h"

/* Item Functions */

Item *nodeAlloc()
{
  Item *node;

  node = (Item *) malloc( sizeof(Item) );
  assert(node);

  node->parent = NULL;
  node->prev = NULL;
  node->next = NULL;
  node->f = node->g = node->h = 0;
  node->depth =0;

  return node;
}

void freeItem( Item *node )
{
  //assert(node);
  if (node && node->board) free(node->board);
	if (node) free(node);
}

void initList( list_t *list_p )
{
  //if (list_p == NULL) list_p = malloc(sizeof(list_t));
	assert(list_p);

  list_p->numElements = 0;
  list_p->first = NULL;
  list_p->last = NULL;
}

int listCount( list_t *list )
{
	return list->numElements;
}

// return an item with same board , or null
Item* onList( list_t *list, char *board)
{
	int j;

    assert(list);  assert(board);
	Item *cur = list->first;
	while (cur) {
        for (j = 0 ; j < cur->size ; j++) {
    	    if (cur->board[j] != board[j]) break;
        }
        if (j == cur->size) {
//            printf("%s", cur->board);
//            printf("\n");
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

// return and remove first item
Item *popFirst( list_t *list ) //
{
	Item *item = NULL;
	if (list->first) {
		item = list->first;
		list->first = item->next;
		if (list->first) list->first->prev = NULL;
		else list->last = NULL;
		item->next = NULL;
		item->prev = NULL;
		list->numElements--;
	}
	return item;
}

// return and remove last item
Item *popLast( list_t *list ) //
{
	Item *item = NULL;
	if (list->last) {
        item = list->last;
        list->last = item->prev;
        if (list->last) list->last->next = NULL;
        else list->first = NULL;
        item->next = NULL;
        item->prev = NULL;
        list->numElements--;
	}
    return item;
}

// remove a node from list
void delList( list_t *list, Item *node) {
    assert( node );
	// remove
    if (node->prev) {
        node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
    } else {
        assert( node == list->first );
        if (node->next) {
            list->first = node->next;
            if (list->first) list->first->prev = NULL;
        }
    }
    if (node->next == NULL) {
        assert( node == list->last );
        list->last = node->prev;
    }
	node->next = node->prev = NULL;
    list->numElements--;
	if (list->numElements == 0) {
		list->first = NULL;
		list->last = NULL;
	}
}

// return and remove best item with minimal f value
Item *popBest( list_t *list ) // and remove the best board from the list.
{
  double best_f=1000000.0;
  Item *node = NULL;
	// search	
	Item *cur = list->first;
  while (cur) {
		if (cur->f < best_f) {
			best_f = cur->f;
			node = cur;
		}
		cur = cur->next;
  }
	if (node) delList( list, node);
	
  return node;
}

 // add item in top
void addFirst( list_t *list, Item *node ) // add in head
{
	assert(list); assert(node);

	if (list->numElements == 0) {
        list->first = node;
        list->last = node;
    } else {
		list->first->prev = node;
		node->next = list->first;
		list->first = node;
	}
	list->numElements++;
}

 // add item in queue
void addLast( list_t *list, Item *node ) // add in tail
{
  assert(list);
  assert(node);

	if (list->numElements == 0) {
		list->first = node;
		list->last = node;
	} else {
  	    list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}
	list->numElements++;
}

void cleanupList( list_t *list )
{
    assert(list);
	
	Item *tmp;
	Item *cur = list->first;
    while (cur) {
		tmp = cur->next;
		freeItem(cur);
		cur = tmp;
	}
	list->first = NULL;
	list->last = NULL;
	list->numElements = 0;
	
}

void printList( list_t list ) {
	Item *item = list.first;
  while (item) {
    printf("%.2f [%s] - ", item->f, item->board);
    item = item->next;
  }
  printf(" (nb_items: %d)\n", list.numElements);
}
