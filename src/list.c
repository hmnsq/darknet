#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "list.h"

list *make_list()
{
	list *l = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(list));
	l->ਮਾਪ = 0;
	l->front = 0;
	l->ਪਿੱਠ = 0;
	ਵਾਪਸ l;
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

ਖਾਲੀ *list_ਕੱਢੋ(list *l){
    ਜੇ(!l->ਪਿੱਠ) ਵਾਪਸ 0;
    ਗੰਢ *b = l->ਪਿੱਠ;
    ਖਾਲੀ *val = b->val;
    l->ਪਿੱਠ = b->prev;
    ਜੇ(l->ਪਿੱਠ) l->ਪਿੱਠ->ਅਗਲਾ = 0;
    ਮੁਕਤ(b);
    --l->ਮਾਪ;
    
    ਵਾਪਸ val;
}

ਖਾਲੀ list_ਪਾਓ(list *l, ਖਾਲੀ *val)
{
	ਗੰਢ *new = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਗੰਢ));
	new->val = val;
	new->ਅਗਲਾ = 0;

	ਜੇ(!l->ਪਿੱਠ){
		l->front = new;
		new->prev = 0;
	}ਨਹੀਂਤਾਂ{
		l->ਪਿੱਠ->ਅਗਲਾ = new;
		new->prev = l->ਪਿੱਠ;
	}
	l->ਪਿੱਠ = new;
	++l->ਮਾਪ;
}

ਖਾਲੀ free_ਗੰਢ(ਗੰਢ *n)
{
	ਗੰਢ *ਅਗਲਾ;
	ਜਦੋਂ(n) {
		ਅਗਲਾ = n->ਅਗਲਾ;
		ਮੁਕਤ(n);
		n = ਅਗਲਾ;
	}
}

ਖਾਲੀ free_list(list *l)
{
	free_ਗੰਢ(l->front);
	ਮੁਕਤ(l);
}

ਖਾਲੀ free_list_contents(list *l)
{
	ਗੰਢ *n = l->front;
	ਜਦੋਂ(n){
		ਮੁਕਤ(n->val);
		n = n->ਅਗਲਾ;
	}
}

ਖਾਲੀ **list_ਮੰਜ਼ਿਲ_array(list *l)
{
    ਖਾਲੀ **a = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l->ਮਾਪ, ਆਕਾਰ(ਖਾਲੀ*));
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਗੰਢ *n = l->front;
    ਜਦੋਂ(n){
        a[ਗਿਣਤੀ_2++] = n->val;
        n = n->ਅਗਲਾ;
    }
    ਵਾਪਸ a;
}
