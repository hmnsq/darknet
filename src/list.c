#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব "list.h"

list *make_list()
{
	list *l = malloc(পরিমাণ(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	প্রত্যাগম l;
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

শূন্য *list_বের_করো(list *l){
    যদি(!l->back) প্রত্যাগম 0;
    গ্রন্থি *b = l->back;
    শূন্য *val = b->val;
    l->back = b->prev;
    যদি(l->back) l->back->next = 0;
    মুক্ত(b);
    --l->size;
    
    প্রত্যাগম val;
}

শূন্য list_ঢোকাও(list *l, শূন্য *val)
{
	গ্রন্থি *new = malloc(পরিমাণ(গ্রন্থি));
	new->val = val;
	new->next = 0;

	যদি(!l->back){
		l->front = new;
		new->prev = 0;
	}অন্যথা{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

শূন্য free_গ্রন্থি(গ্রন্থি *n)
{
	গ্রন্থি *next;
	যাবত্(n) {
		next = n->next;
		মুক্ত(n);
		n = next;
	}
}

শূন্য free_list(list *l)
{
	free_গ্রন্থি(l->front);
	মুক্ত(l);
}

শূন্য free_list_contents(list *l)
{
	গ্রন্থি *n = l->front;
	যাবত্(n){
		মুক্ত(n->val);
		n = n->next;
	}
}

শূন্য **list_to_array(list *l)
{
    শূন্য **a = calloc(l->size, পরিমাণ(শূন্য*));
    পূর্ণাঙ্ক গুনতি = 0;
    গ্রন্থি *n = l->front;
    যাবত্(n){
        a[গুনতি++] = n->val;
        n = n->next;
    }
    প্রত্যাগম a;
}
