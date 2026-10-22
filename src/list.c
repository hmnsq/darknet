#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "list.h"

list *make_list()
{
	list *l = दो_स्मृति(परिमाण(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	प्रत्यागम l;
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

शून्य *list_निकालो(list *l){
    यदि(!l->back) प्रत्यागम 0;
    गाँठ *b = l->back;
    शून्य *val = b->val;
    l->back = b->prev;
    यदि(l->back) l->back->next = 0;
    मुक्त(b);
    --l->size;
    
    प्रत्यागम val;
}

शून्य list_जोड़ो(list *l, शून्य *val)
{
	गाँठ *new = दो_स्मृति(परिमाण(गाँठ));
	new->val = val;
	new->next = 0;

	यदि(!l->back){
		l->front = new;
		new->prev = 0;
	}अन्यथा{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

शून्य free_गाँठ(गाँठ *n)
{
	गाँठ *next;
	यावत्(n) {
		next = n->next;
		मुक्त(n);
		n = next;
	}
}

शून्य free_list(list *l)
{
	free_गाँठ(l->front);
	मुक्त(l);
}

शून्य free_list_contents(list *l)
{
	गाँठ *n = l->front;
	यावत्(n){
		मुक्त(n->val);
		n = n->next;
	}
}

शून्य **list_to_array(list *l)
{
    शून्य **a = सुस्मृति(l->size, परिमाण(शून्य*));
    पूर्णाङ्क गिनती = 0;
    गाँठ *n = l->front;
    यावत्(n){
        a[गिनती++] = n->val;
        n = n->next;
    }
    प्रत्यागम a;
}
