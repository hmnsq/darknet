#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "list.h"

list *make_list()
{
	list *l = दो_स्मृति(परिमाण(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	फर्काऊ l;
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

शून्य *list_झिक(list *l){
    यदि(!l->back) फर्काऊ 0;
    गाँठो *b = l->back;
    शून्य *val = b->val;
    l->back = b->prev;
    यदि(l->back) l->back->next = 0;
    मुक्त(b);
    --l->size;
    
    फर्काऊ val;
}

शून्य list_घुसाउ(list *l, शून्य *val)
{
	गाँठो *new = दो_स्मृति(परिमाण(गाँठो));
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

शून्य free_गाँठो(गाँठो *n)
{
	गाँठो *next;
	जबसम्म(n) {
		next = n->next;
		मुक्त(n);
		n = next;
	}
}

शून्य free_list(list *l)
{
	free_गाँठो(l->front);
	मुक्त(l);
}

शून्य free_list_contents(list *l)
{
	गाँठो *n = l->front;
	जबसम्म(n){
		मुक्त(n->val);
		n = n->next;
	}
}

शून्य **list_to_array(list *l)
{
    शून्य **a = सुस्मृति(l->size, परिमाण(शून्य*));
    पूर्णाङ्क गन्ती = 0;
    गाँठो *n = l->front;
    जबसम्म(n){
        a[गन्ती++] = n->val;
        n = n->next;
    }
    फर्काऊ a;
}
