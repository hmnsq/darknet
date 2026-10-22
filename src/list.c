#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "list.h"

list *make_list()
{
	list *l = malloc(حجم(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	החזר l;
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

فراغ *list_pop(list *l){
    אם(!l->back) החזר 0;
    node *b = l->back;
    فراغ *val = b->val;
    l->back = b->prev;
    אם(l->back) l->back->next = 0;
    حرر(b);
    --l->size;
    
    החזר val;
}

فراغ list_insert(list *l, فراغ *val)
{
	node *new = malloc(حجم(node));
	new->val = val;
	new->next = 0;

	אם(!l->back){
		l->front = new;
		new->prev = 0;
	}אחרת{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

فراغ free_node(node *n)
{
	node *next;
	כלעוד(n) {
		next = n->next;
		حرر(n);
		n = next;
	}
}

فراغ free_list(list *l)
{
	free_node(l->front);
	حرر(l);
}

فراغ free_list_contents(list *l)
{
	node *n = l->front;
	כלעוד(n){
		حرر(n->val);
		n = n->next;
	}
}

فراغ **list_to_array(list *l)
{
    فراغ **a = calloc(l->size, حجم(فراغ*));
    صحيح count = 0;
    node *n = l->front;
    כלעוד(n){
        a[count++] = n->val;
        n = n->next;
    }
    החזר a;
}
