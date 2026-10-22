#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ "list.h"

list *make_list()
{
	list *l = malloc(அளவு(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	திரும்பு l;
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

வெறுமை *list_எடு(list *l){
    எனில்(!l->back) திரும்பு 0;
    முனை *b = l->back;
    வெறுமை *val = b->val;
    l->back = b->prev;
    எனில்(l->back) l->back->next = 0;
    விடுவி(b);
    --l->size;
    
    திரும்பு val;
}

வெறுமை list_செருகு(list *l, வெறுமை *val)
{
	முனை *new = malloc(அளவு(முனை));
	new->val = val;
	new->next = 0;

	எனில்(!l->back){
		l->front = new;
		new->prev = 0;
	}இல்லையெனில்{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

வெறுமை free_முனை(முனை *n)
{
	முனை *next;
	போது(n) {
		next = n->next;
		விடுவி(n);
		n = next;
	}
}

வெறுமை free_list(list *l)
{
	free_முனை(l->front);
	விடுவி(l);
}

வெறுமை free_list_contents(list *l)
{
	முனை *n = l->front;
	போது(n){
		விடுவி(n->val);
		n = n->next;
	}
}

வெறுமை **list_to_array(list *l)
{
    வெறுமை **a = calloc(l->size, அளவு(வெறுமை*));
    முழு மொத்தம் = 0;
    முனை *n = l->front;
    போது(n){
        a[மொத்தம்++] = n->val;
        n = n->next;
    }
    திரும்பு a;
}
