#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ "list.h"

list *make_list()
{
	list *l = malloc(পরিমাণ(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	ঘূৰাই l;
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

শূন্য *list_বেৰ_কৰো(list *l){
    যদি(!l->back) ঘূৰাই 0;
    গ্ৰন্থি *b = l->back;
    শূন্য *val = b->val;
    l->back = b->prev;
    যদি(l->back) l->back->next = 0;
    মুক্ত(b);
    --l->size;
    
    ঘূৰাই val;
}

শূন্য list_ঢোকাও(list *l, শূন্য *val)
{
	গ্ৰন্থি *new = malloc(পরিমাণ(গ্ৰন্থি));
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

শূন্য free_গ্ৰন্থি(গ্ৰন্থি *n)
{
	গ্ৰন্থি *next;
	যেতিয়ালৈ(n) {
		next = n->next;
		মুক্ত(n);
		n = next;
	}
}

শূন্য free_list(list *l)
{
	free_গ্ৰন্থি(l->front);
	মুক্ত(l);
}

শূন্য free_list_contents(list *l)
{
	গ্ৰন্থি *n = l->front;
	যেতিয়ালৈ(n){
		মুক্ত(n->val);
		n = n->next;
	}
}

শূন্য **list_to_array(list *l)
{
    শূন্য **a = calloc(l->size, পরিমাণ(শূন্য*));
    পূর্ণাঙ্ক গুনতি = 0;
    গ্ৰন্থি *n = l->front;
    যেতিয়ালৈ(n){
        a[গুনতি++] = n->val;
        n = n->next;
    }
    ঘূৰাই a;
}
