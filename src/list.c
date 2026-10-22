#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব "list.h"

list *make_list()
{
	list *l = malloc(আকার(list));
	l->আকার_2 = 0;
	l->front = 0;
	l->back = 0;
	ফেরত l;
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
    যদি(!l->back) ফেরত 0;
    গ্রন্থি *b = l->back;
    শূন্য *val = b->val;
    l->back = b->পূর্ব;
    যদি(l->back) l->back->পরবর্তী = 0;
    মুক্ত(b);
    --l->আকার_2;
    
    ফেরত val;
}

শূন্য list_ঢোকাও(list *l, শূন্য *val)
{
	গ্রন্থি *new = malloc(আকার(গ্রন্থি));
	new->val = val;
	new->পরবর্তী = 0;

	যদি(!l->back){
		l->front = new;
		new->পূর্ব = 0;
	}অন্যথা{
		l->back->পরবর্তী = new;
		new->পূর্ব = l->back;
	}
	l->back = new;
	++l->আকার_2;
}

শূন্য free_গ্রন্থি(গ্রন্থি *n)
{
	গ্রন্থি *পরবর্তী;
	যখন(n) {
		পরবর্তী = n->পরবর্তী;
		মুক্ত(n);
		n = পরবর্তী;
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
	যখন(n){
		মুক্ত(n->val);
		n = n->পরবর্তী;
	}
}

শূন্য **list_to_array(list *l)
{
    শূন্য **a = calloc(l->আকার_2, আকার(শূন্য*));
    পূর্ণসংখ্যা গুনতি = 0;
    গ্রন্থি *n = l->front;
    যখন(n){
        a[গুনতি++] = n->val;
        n = n->পরবর্তী;
    }
    ফেরত a;
}
