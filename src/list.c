#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "list.h"

list *make_list()
{
	list *l = स्मृतिं_देहि(परिमाण(list));
	l->आकार = 0;
	l->front = 0;
	l->पृष्ठभागः = 0;
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

शून्य *list_निष्कासय(list *l){
    यदि(!l->पृष्ठभागः) प्रत्यागम 0;
    ग्रन्थिः *b = l->पृष्ठभागः;
    शून्य *val = b->val;
    l->पृष्ठभागः = b->पूर्व;
    यदि(l->पृष्ठभागः) l->पृष्ठभागः->अग्र = 0;
    मुक्त(b);
    --l->आकार;
    
    प्रत्यागम val;
}

शून्य list_निवेशय(list *l, शून्य *val)
{
	ग्रन्थिः *new = स्मृतिं_देहि(परिमाण(ग्रन्थिः));
	new->val = val;
	new->अग्र = 0;

	यदि(!l->पृष्ठभागः){
		l->front = new;
		new->पूर्व = 0;
	}अन्यथा{
		l->पृष्ठभागः->अग्र = new;
		new->पूर्व = l->पृष्ठभागः;
	}
	l->पृष्ठभागः = new;
	++l->आकार;
}

शून्य free_ग्रन्थिः(ग्रन्थिः *n)
{
	ग्रन्थिः *अग्र;
	यावत्(n) {
		अग्र = n->अग्र;
		मुक्त(n);
		n = अग्र;
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
	यावत्(n){
		मुक्त(n->val);
		n = n->अग्र;
	}
}

शून्य **list_गन्तव्यम्_array(list *l)
{
    शून्य **a = शून्यस्मृतिं_देहि(l->आकार, परिमाण(शून्य*));
    पूर्णाङ्क गणनम् = 0;
    ग्रन्थिः *n = l->front;
    यावत्(n){
        a[गणनम्++] = n->val;
        n = n->अग्र;
    }
    प्रत्यागम a;
}
