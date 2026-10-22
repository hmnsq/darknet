#include <stdlib.h>
#include <string.h>
#include "list.h"

list *make_list()
{
	list *l = malloc(parimaa_na(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	ပွၵ်ႈ l;
}

/*
void transfer_node(list *s, list *d, node *n)
{
    node *prev, *next;
    prev = n->prev;
    next = n->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;
    --s->size;
    if(s->front == n) s->front = next;
    if(s->back == n) s->back = prev;
}
*/

void *list_pop(list *l){
    ပေႃး(!l->back) ပွၵ်ႈ 0;
    node *b = l->back;
    void *val = b->val;
    l->back = b->prev;
    ပေႃး(l->back) l->back->next = 0;
    free(b);
    --l->size;
    
    ပွၵ်ႈ val;
}

void list_insert(list *l, void *val)
{
	node *new = malloc(parimaa_na(node));
	new->val = val;
	new->next = 0;

	ပေႃး(!l->back){
		l->front = new;
		new->prev = 0;
	}ပေႃးဢမ်ႇ{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

void free_node(node *n)
{
	node *next;
	ၶၢဝ်း(n) {
		next = n->next;
		free(n);
		n = next;
	}
}

void free_list(list *l)
{
	free_node(l->front);
	free(l);
}

void free_list_contents(list *l)
{
	node *n = l->front;
	ၶၢဝ်း(n){
		free(n->val);
		n = n->next;
	}
}

void **list_to_array(list *l)
{
    void **a = calloc(l->size, parimaa_na(void*));
    puur_naa_nka count = 0;
    node *n = l->front;
    ၶၢဝ်း(n){
        a[count++] = n->val;
        n = n->next;
    }
    ပွၵ်ႈ a;
}
