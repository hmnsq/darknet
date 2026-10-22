#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश "list.h"

list *make_list()
{
	list *l = दो_स्मृति(माप(list));
	l->आकार = 0;
	l->front = 0;
	l->back = 0;
	वापस l;
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

व्योम *list_निकालो(list *l){
    अगर(!l->back) वापस 0;
    गाँठ *b = l->back;
    व्योम *val = b->val;
    l->back = b->पूर्व;
    अगर(l->back) l->back->अग्र = 0;
    मुक्त(b);
    --l->आकार;
    
    वापस val;
}

व्योम list_जोड़ो(list *l, व्योम *val)
{
	गाँठ *new = दो_स्मृति(माप(गाँठ));
	new->val = val;
	new->अग्र = 0;

	अगर(!l->back){
		l->front = new;
		new->पूर्व = 0;
	}अन्यथा{
		l->back->अग्र = new;
		new->पूर्व = l->back;
	}
	l->back = new;
	++l->आकार;
}

व्योम free_गाँठ(गाँठ *n)
{
	गाँठ *अग्र;
	जबतक(n) {
		अग्र = n->अग्र;
		मुक्त(n);
		n = अग्र;
	}
}

व्योम free_list(list *l)
{
	free_गाँठ(l->front);
	मुक्त(l);
}

व्योम free_list_contents(list *l)
{
	गाँठ *n = l->front;
	जबतक(n){
		मुक्त(n->val);
		n = n->अग्र;
	}
}

व्योम **list_to_array(list *l)
{
    व्योम **a = सुस्मृति(l->आकार, माप(व्योम*));
    पूर्णांक गिनती = 0;
    गाँठ *n = l->front;
    जबतक(n){
        a[गिनती++] = n->val;
        n = n->अग्र;
    }
    वापस a;
}
