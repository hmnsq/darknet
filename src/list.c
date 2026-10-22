#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "list.h"

list *make_list()
{
	list *l = حافظه_بده(حجم(list));
	l->اندازه = 0;
	l->front = 0;
	l->پشت = 0;
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

فراغ *list_برداشت(list *l){
    إذا(!l->پشت) أرجع 0;
    گره *b = l->پشت;
    فراغ *val = b->val;
    l->پشت = b->prev;
    إذا(l->پشت) l->پشت->بعدی = 0;
    حرر(b);
    --l->اندازه;
    
    أرجع val;
}

فراغ list_درج(list *l, فراغ *val)
{
	گره *new = حافظه_بده(حجم(گره));
	new->val = val;
	new->بعدی = 0;

	إذا(!l->پشت){
		l->front = new;
		new->prev = 0;
	}وإلا{
		l->پشت->بعدی = new;
		new->prev = l->پشت;
	}
	l->پشت = new;
	++l->اندازه;
}

فراغ free_گره(گره *n)
{
	گره *بعدی;
	طالما(n) {
		بعدی = n->بعدی;
		حرر(n);
		n = بعدی;
	}
}

فراغ free_list(list *l)
{
	free_گره(l->front);
	حرر(l);
}

فراغ free_list_contents(list *l)
{
	گره *n = l->front;
	طالما(n){
		حرر(n->val);
		n = n->بعدی;
	}
}

فراغ **list_مقصد_array(list *l)
{
    فراغ **a = حافظه_صفر_بده(l->اندازه, حجم(فراغ*));
    صحيح شمار = 0;
    گره *n = l->front;
    طالما(n){
        a[شمار++] = n->val;
        n = n->بعدی;
    }
    أرجع a;
}
