#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ "list.h"

list *make_list()
{
	list *l = malloc(വലുപ്പം(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	തിരികെ l;
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

ശൂന്യം *list_എടുക്കുക(list *l){
    എങ്കിൽ(!l->back) തിരികെ 0;
    കണ്ണി *b = l->back;
    ശൂന്യം *val = b->val;
    l->back = b->prev;
    എങ്കിൽ(l->back) l->back->next = 0;
    സ്വതന്ത്രം(b);
    --l->size;
    
    തിരികെ val;
}

ശൂന്യം list_തിരുകുക(list *l, ശൂന്യം *val)
{
	കണ്ണി *new = malloc(വലുപ്പം(കണ്ണി));
	new->val = val;
	new->next = 0;

	എങ്കിൽ(!l->back){
		l->front = new;
		new->prev = 0;
	}അല്ലെങ്കിൽ{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

ശൂന്യം free_കണ്ണി(കണ്ണി *n)
{
	കണ്ണി *next;
	ആകുമ്പോൾ(n) {
		next = n->next;
		സ്വതന്ത്രം(n);
		n = next;
	}
}

ശൂന്യം free_list(list *l)
{
	free_കണ്ണി(l->front);
	സ്വതന്ത്രം(l);
}

ശൂന്യം free_list_contents(list *l)
{
	കണ്ണി *n = l->front;
	ആകുമ്പോൾ(n){
		സ്വതന്ത്രം(n->val);
		n = n->next;
	}
}

ശൂന്യം **list_to_array(list *l)
{
    ശൂന്യം **a = calloc(l->size, വലുപ്പം(ശൂന്യം*));
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    കണ്ണി *n = l->front;
    ആകുമ്പോൾ(n){
        a[എണ്ണം++] = n->val;
        n = n->next;
    }
    തിരികെ a;
}
