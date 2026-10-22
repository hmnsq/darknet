#شامل <stdlib.h>
#شامل <string.h>
#شامل "list.h"

list *make_list()
{
	list *l = حافظہ_دو(حجم(list));
	l->سائز = 0;
	l->front = 0;
	l->عقب = 0;
	واپس l;
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

خالی *list_نکالو(list *l){
    اگر(!l->عقب) واپس 0;
    گرہ *b = l->عقب;
    خالی *val = b->val;
    l->عقب = b->پچھلا;
    اگر(l->عقب) l->عقب->اگلا = 0;
    آزاد(b);
    --l->سائز;
    
    واپس val;
}

خالی list_داخل_کرو(list *l, خالی *val)
{
	گرہ *new = حافظہ_دو(حجم(گرہ));
	new->val = val;
	new->اگلا = 0;

	اگر(!l->عقب){
		l->front = new;
		new->پچھلا = 0;
	}ورنہ{
		l->عقب->اگلا = new;
		new->پچھلا = l->عقب;
	}
	l->عقب = new;
	++l->سائز;
}

خالی free_گرہ(گرہ *n)
{
	گرہ *اگلا;
	جب(n) {
		اگلا = n->اگلا;
		آزاد(n);
		n = اگلا;
	}
}

خالی free_list(list *l)
{
	free_گرہ(l->front);
	آزاد(l);
}

خالی free_list_contents(list *l)
{
	گرہ *n = l->front;
	جب(n){
		آزاد(n->val);
		n = n->اگلا;
	}
}

خالی **list_مقصد_array(list *l)
{
    خالی **a = صفر_حافظہ_دو(l->سائز, حجم(خالی*));
    صحیح گنتی = 0;
    گرہ *n = l->front;
    جب(n){
        a[گنتی++] = n->val;
        n = n->اگلا;
    }
    واپس a;
}
