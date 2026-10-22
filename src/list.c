#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "list.h"

list *make_list()
{
	list *l = حافظہ_دیو(حجم(list));
	l->ناپ = 0;
	l->front = 0;
	l->پٹھ = 0;
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

فراغ *list_نکالو(list *l){
    إذا(!l->پٹھ) أرجع 0;
    گرہ *b = l->پٹھ;
    فراغ *val = b->val;
    l->پٹھ = b->prev;
    إذا(l->پٹھ) l->پٹھ->اگلا = 0;
    حرر(b);
    --l->ناپ;
    
    أرجع val;
}

فراغ list_داخل_کرو(list *l, فراغ *val)
{
	گرہ *new = حافظہ_دیو(حجم(گرہ));
	new->val = val;
	new->اگلا = 0;

	إذا(!l->پٹھ){
		l->front = new;
		new->prev = 0;
	}وإلا{
		l->پٹھ->اگلا = new;
		new->prev = l->پٹھ;
	}
	l->پٹھ = new;
	++l->ناپ;
}

فراغ free_گرہ(گرہ *n)
{
	گرہ *اگلا;
	طالما(n) {
		اگلا = n->اگلا;
		حرر(n);
		n = اگلا;
	}
}

فراغ free_list(list *l)
{
	free_گرہ(l->front);
	حرر(l);
}

فراغ free_list_contents(list *l)
{
	گرہ *n = l->front;
	طالما(n){
		حرر(n->val);
		n = n->اگلا;
	}
}

فراغ **list_منزل_array(list *l)
{
    فراغ **a = صفر_حافظہ_دیو(l->ناپ, حجم(فراغ*));
    صحيح گنتی = 0;
    گرہ *n = l->front;
    طالما(n){
        a[گنتی++] = n->val;
        n = n->اگلا;
    }
    أرجع a;
}
