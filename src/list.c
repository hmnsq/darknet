#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "list.h"

list *make_list()
{
	list *l = malloc(حجم(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	إرجاع l;
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

فراغ *list_اسحب(list *l){
    إذا(!l->back) إرجاع 0;
    عقدة *b = l->back;
    فراغ *val = b->val;
    l->back = b->prev;
    إذا(l->back) l->back->next = 0;
    حرر(b);
    --l->size;
    
    إرجاع val;
}

فراغ list_أدرج(list *l, فراغ *val)
{
	عقدة *new = malloc(حجم(عقدة));
	new->val = val;
	new->next = 0;

	إذا(!l->back){
		l->front = new;
		new->prev = 0;
	}وإلا{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

فراغ free_عقدة(عقدة *n)
{
	عقدة *next;
	بينما(n) {
		next = n->next;
		حرر(n);
		n = next;
	}
}

فراغ free_list(list *l)
{
	free_عقدة(l->front);
	حرر(l);
}

فراغ free_list_contents(list *l)
{
	عقدة *n = l->front;
	بينما(n){
		حرر(n->val);
		n = n->next;
	}
}

فراغ **list_to_array(list *l)
{
    فراغ **a = calloc(l->size, حجم(فراغ*));
    صحيح تعداد_2 = 0;
    عقدة *n = l->front;
    بينما(n){
        a[تعداد_2++] = n->val;
        n = n->next;
    }
    إرجاع a;
}
