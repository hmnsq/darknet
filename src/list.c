#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ "list.h"

list *make_list()
{
	list *l = malloc(કદ(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	પાછા l;
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

શૂન્ય *list_કાઢો(list *l){
    જો(!l->back) પાછા 0;
    ગાંઠ *b = l->back;
    શૂન્ય *val = b->val;
    l->back = b->prev;
    જો(l->back) l->back->next = 0;
    મુક્ત(b);
    --l->size;
    
    પાછા val;
}

શૂન્ય list_ઉમેરો(list *l, શૂન્ય *val)
{
	ગાંઠ *new = malloc(કદ(ગાંઠ));
	new->val = val;
	new->next = 0;

	જો(!l->back){
		l->front = new;
		new->prev = 0;
	}નહીંતર{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

શૂન્ય free_ગાંઠ(ગાંઠ *n)
{
	ગાંઠ *next;
	જ્યારે(n) {
		next = n->next;
		મુક્ત(n);
		n = next;
	}
}

શૂન્ય free_list(list *l)
{
	free_ગાંઠ(l->front);
	મુક્ત(l);
}

શૂન્ય free_list_contents(list *l)
{
	ગાંઠ *n = l->front;
	જ્યારે(n){
		મુક્ત(n->val);
		n = n->next;
	}
}

શૂન્ય **list_to_array(list *l)
{
    શૂન્ય **a = calloc(l->size, કદ(શૂન્ય*));
    પૂર્ણાંક ગણતરી = 0;
    ગાંઠ *n = l->front;
    જ્યારે(n){
        a[ગણતરી++] = n->val;
        n = n->next;
    }
    પાછા a;
}
