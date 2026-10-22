#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "list.h"

list *make_list()
{
	list *l = malloc(ಪರಿಮಾಣ(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	ಪ್ರತ್ಯಾಗಮ l;
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

ಶೂನ್ಯ *list_ತೆಗೆ(list *l){
    ಯದಿ(!l->back) ಪ್ರತ್ಯಾಗಮ 0;
    ಗಂಟು *b = l->back;
    ಶೂನ್ಯ *val = b->val;
    l->back = b->prev;
    ಯದಿ(l->back) l->back->next = 0;
    ಮುಕ್ತ(b);
    --l->size;
    
    ಪ್ರತ್ಯಾಗಮ val;
}

ಶೂನ್ಯ list_ಸೇರಿಸು(list *l, ಶೂನ್ಯ *val)
{
	ಗಂಟು *new = malloc(ಪರಿಮಾಣ(ಗಂಟು));
	new->val = val;
	new->next = 0;

	ಯದಿ(!l->back){
		l->front = new;
		new->prev = 0;
	}ಅನ್ಯಥಾ{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

ಶೂನ್ಯ free_ಗಂಟು(ಗಂಟು *n)
{
	ಗಂಟು *next;
	ಯಾವತ್(n) {
		next = n->next;
		ಮುಕ್ತ(n);
		n = next;
	}
}

ಶೂನ್ಯ free_list(list *l)
{
	free_ಗಂಟು(l->front);
	ಮುಕ್ತ(l);
}

ಶೂನ್ಯ free_list_contents(list *l)
{
	ಗಂಟು *n = l->front;
	ಯಾವತ್(n){
		ಮುಕ್ತ(n->val);
		n = n->next;
	}
}

ಶೂನ್ಯ **list_to_array(list *l)
{
    ಶೂನ್ಯ **a = calloc(l->size, ಪರಿಮಾಣ(ಶೂನ್ಯ*));
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಗಂಟು *n = l->front;
    ಯಾವತ್(n){
        a[ಎಣಿಕೆ++] = n->val;
        n = n->next;
    }
    ಪ್ರತ್ಯಾಗಮ a;
}
