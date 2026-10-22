#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "list.h"

list *make_list()
{
	list *l = حافظه_ورکړه(حجم(list));
	l->اندازه = 0;
	l->front = 0;
	l->شا = 0;
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

فراغ *list_راوباسه(list *l){
    إذا(!l->شا) أرجع 0;
    غوټه *b = l->شا;
    فراغ *val = b->val;
    l->شا = b->prev;
    إذا(l->شا) l->شا->بل = 0;
    حرر(b);
    --l->اندازه;
    
    أرجع val;
}

فراغ list_ورننباسه(list *l, فراغ *val)
{
	غوټه *new = حافظه_ورکړه(حجم(غوټه));
	new->val = val;
	new->بل = 0;

	إذا(!l->شا){
		l->front = new;
		new->prev = 0;
	}وإلا{
		l->شا->بل = new;
		new->prev = l->شا;
	}
	l->شا = new;
	++l->اندازه;
}

فراغ free_غوټه(غوټه *n)
{
	غوټه *بل;
	طالما(n) {
		بل = n->بل;
		حرر(n);
		n = بل;
	}
}

فراغ free_list(list *l)
{
	free_غوټه(l->front);
	حرر(l);
}

فراغ free_list_contents(list *l)
{
	غوټه *n = l->front;
	طالما(n){
		حرر(n->val);
		n = n->بل;
	}
}

فراغ **list_منزل_array(list *l)
{
    فراغ **a = حافظه_صفر_ورکړه(l->اندازه, حجم(فراغ*));
    صحيح شمېر = 0;
    غوټه *n = l->front;
    طالما(n){
        a[شمېر++] = n->val;
        n = n->بل;
    }
    أرجع a;
}
