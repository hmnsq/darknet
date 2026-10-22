#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "list.h"

list *make_list()
{
	list *l = malloc(परिमाण(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	पटिनिवत्त l;
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

शून्य *list_निष्कासय(list *l){
    सचे(!l->back) पटिनिवत्त 0;
    ग्रन्थिः *b = l->back;
    शून्य *val = b->val;
    l->back = b->prev;
    सचे(l->back) l->back->next = 0;
    मुक्त(b);
    --l->size;
    
    पटिनिवत्त val;
}

शून्य list_निवेशय(list *l, शून्य *val)
{
	ग्रन्थिः *new = malloc(परिमाण(ग्रन्थिः));
	new->val = val;
	new->next = 0;

	सचे(!l->back){
		l->front = new;
		new->prev = 0;
	}अञ्ञथा{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

शून्य free_ग्रन्थिः(ग्रन्थिः *n)
{
	ग्रन्थिः *next;
	याव(n) {
		next = n->next;
		मुक्त(n);
		n = next;
	}
}

शून्य free_list(list *l)
{
	free_ग्रन्थिः(l->front);
	मुक्त(l);
}

शून्य free_list_contents(list *l)
{
	ग्रन्थिः *n = l->front;
	याव(n){
		मुक्त(n->val);
		n = n->next;
	}
}

शून्य **list_to_array(list *l)
{
    शून्य **a = calloc(l->size, परिमाण(शून्य*));
    पूर्णाङ्क गणनम् = 0;
    ग्रन्थिः *n = l->front;
    याव(n){
        a[गणनम्++] = n->val;
        n = n->next;
    }
    पटिनिवत्त a;
}
