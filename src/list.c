#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ "list.h"

list *make_list()
{
	list *l = malloc(ଆକାର(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	ଫେରନ୍ତୁ l;
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

ଶୂନ୍ୟ *list_କାଢ଼(list *l){
    ଯଦି(!l->back) ଫେରନ୍ତୁ 0;
    ଗଣ୍ଠି *b = l->back;
    ଶୂନ୍ୟ *val = b->val;
    l->back = b->prev;
    ଯଦି(l->back) l->back->next = 0;
    ମୁକ୍ତ(b);
    --l->size;
    
    ଫେରନ୍ତୁ val;
}

ଶୂନ୍ୟ list_ଭର୍ତ୍ତି(list *l, ଶୂନ୍ୟ *val)
{
	ଗଣ୍ଠି *new = malloc(ଆକାର(ଗଣ୍ଠି));
	new->val = val;
	new->next = 0;

	ଯଦି(!l->back){
		l->front = new;
		new->prev = 0;
	}ଅନ୍ୟଥା{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

ଶୂନ୍ୟ free_ଗଣ୍ଠି(ଗଣ୍ଠି *n)
{
	ଗଣ୍ଠି *next;
	ଯେତେବେଳେ(n) {
		next = n->next;
		ମୁକ୍ତ(n);
		n = next;
	}
}

ଶୂନ୍ୟ free_list(list *l)
{
	free_ଗଣ୍ଠି(l->front);
	ମୁକ୍ତ(l);
}

ଶୂନ୍ୟ free_list_contents(list *l)
{
	ଗଣ୍ଠି *n = l->front;
	ଯେତେବେଳେ(n){
		ମୁକ୍ତ(n->val);
		n = n->next;
	}
}

ଶୂନ୍ୟ **list_to_array(list *l)
{
    ଶୂନ୍ୟ **a = calloc(l->size, ଆକାର(ଶୂନ୍ୟ*));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ଗଣ୍ଠି *n = l->front;
    ଯେତେବେଳେ(n){
        a[ଗଣନା_2++] = n->val;
        n = n->next;
    }
    ଫେରନ୍ତୁ a;
}
