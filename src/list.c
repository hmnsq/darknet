#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "list.h"

list *make_list()
{
	list *l = दो_स्मृति(आकार(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	परत l;
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

शून्य *list_काढा(list *l){
    जर(!l->back) परत 0;
    गाठ *b = l->back;
    शून्य *val = b->val;
    l->back = b->prev;
    जर(l->back) l->back->next = 0;
    मुक्त(b);
    --l->size;
    
    परत val;
}

शून्य list_घाला(list *l, शून्य *val)
{
	गाठ *new = दो_स्मृति(आकार(गाठ));
	new->val = val;
	new->next = 0;

	जर(!l->back){
		l->front = new;
		new->prev = 0;
	}अन्यथा{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
}

शून्य free_गाठ(गाठ *n)
{
	गाठ *next;
	पर्यंत(n) {
		next = n->next;
		मुक्त(n);
		n = next;
	}
}

शून्य free_list(list *l)
{
	free_गाठ(l->front);
	मुक्त(l);
}

शून्य free_list_contents(list *l)
{
	गाठ *n = l->front;
	पर्यंत(n){
		मुक्त(n->val);
		n = n->next;
	}
}

शून्य **list_to_array(list *l)
{
    शून्य **a = सुस्मृति(l->size, आकार(शून्य*));
    पूर्णांक मोजणी = 0;
    गाठ *n = l->front;
    पर्यंत(n){
        a[मोजणी++] = n->val;
        n = n->next;
    }
    परत a;
}
