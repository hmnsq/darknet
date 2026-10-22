#شامل <stdlib.h>
#شامل <string.h>
#شامل "list.h"

list *make_list()
{
	list *l = malloc(اندازه(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	بازگشت l;
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

تهی *list_برداشت(list *l){
    اگر(!l->back) بازگشت 0;
    گره *b = l->back;
    تهی *val = b->val;
    l->back = b->prev;
    اگر(l->back) l->back->next = 0;
    آزاد(b);
    --l->size;
    
    بازگشت val;
}

تهی list_درج(list *l, تهی *val)
{
	گره *new = malloc(اندازه(گره));
	new->val = val;
	new->next = 0;

	اگر(!l->back){
		l->front = new;
		new->prev = 0;
	}وگرنه{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

تهی free_گره(گره *n)
{
	گره *next;
	وقتی(n) {
		next = n->next;
		آزاد(n);
		n = next;
	}
}

تهی free_list(list *l)
{
	free_گره(l->front);
	آزاد(l);
}

تهی free_list_contents(list *l)
{
	گره *n = l->front;
	وقتی(n){
		آزاد(n->val);
		n = n->next;
	}
}

تهی **list_to_array(list *l)
{
    تهی **a = calloc(l->size, اندازه(تهی*));
    صحیح شمار = 0;
    گره *n = l->front;
    وقتی(n){
        a[شمار++] = n->val;
        n = n->next;
    }
    بازگشت a;
}
