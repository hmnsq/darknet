#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "list.h"

list *make_list()
{
	list *l = يادگيري_ڏيو(حجم(list));
	l->سائيز = 0;
	l->front = 0;
	l->پٺ = 0;
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

فراغ *list_ڪڍو(list *l){
    إذا(!l->پٺ) أرجع 0;
    ڳنڍ *b = l->پٺ;
    فراغ *val = b->val;
    l->پٺ = b->prev;
    إذا(l->پٺ) l->پٺ->ايندڙ = 0;
    حرر(b);
    --l->سائيز;
    
    أرجع val;
}

فراغ list_وجهو(list *l, فراغ *val)
{
	ڳنڍ *new = يادگيري_ڏيو(حجم(ڳنڍ));
	new->val = val;
	new->ايندڙ = 0;

	إذا(!l->پٺ){
		l->front = new;
		new->prev = 0;
	}وإلا{
		l->پٺ->ايندڙ = new;
		new->prev = l->پٺ;
	}
	l->پٺ = new;
	++l->سائيز;
}

فراغ free_ڳنڍ(ڳنڍ *n)
{
	ڳنڍ *ايندڙ;
	طالما(n) {
		ايندڙ = n->ايندڙ;
		حرر(n);
		n = ايندڙ;
	}
}

فراغ free_list(list *l)
{
	free_ڳنڍ(l->front);
	حرر(l);
}

فراغ free_list_contents(list *l)
{
	ڳنڍ *n = l->front;
	طالما(n){
		حرر(n->val);
		n = n->ايندڙ;
	}
}

فراغ **list_منزل_array(list *l)
{
    فراغ **a = صفر_يادگيري_ڏيو(l->سائيز, حجم(فراغ*));
    صحيح ڳڻپ = 0;
    ڳنڍ *n = l->front;
    طالما(n){
        a[ڳڻپ++] = n->val;
        n = n->ايندڙ;
    }
    أرجع a;
}
