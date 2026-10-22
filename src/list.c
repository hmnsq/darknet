#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "list.h"

list *make_list()
{
	list *l = malloc(حجم(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	أرجع l;
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

فراغ *list_کلوار(list *l){
    إذا(!l->back) أرجع 0;
    نود *b = l->back;
    فراغ *val = b->val;
    l->back = b->prev;
    إذا(l->back) l->back->next = 0;
    حرر(b);
    --l->size;
    
    أرجع val;
}

فراغ list_سيسيڤ(list *l, فراغ *val)
{
	نود *new = malloc(حجم(نود));
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

فراغ free_نود(نود *n)
{
	نود *next;
	طالما(n) {
		next = n->next;
		حرر(n);
		n = next;
	}
}

فراغ free_list(list *l)
{
	free_نود(l->front);
	حرر(l);
}

فراغ free_list_contents(list *l)
{
	نود *n = l->front;
	طالما(n){
		حرر(n->val);
		n = n->next;
	}
}

فراغ **list_to_array(list *l)
{
    فراغ **a = calloc(l->size, حجم(فراغ*));
    صحيح کيراءن = 0;
    نود *n = l->front;
    طالما(n){
        a[کيراءن++] = n->val;
        n = n->next;
    }
    أرجع a;
}
