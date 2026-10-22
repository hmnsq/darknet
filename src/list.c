#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ "list.h"

list *make_list()
{
	list *l = జ్ఞాపకం_ఇవ్వు(పరిమాణం(list));
	l->కొలత = 0;
	l->front = 0;
	l->వెనుక = 0;
	తిరిగి l;
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

శూన్య *list_తీయి(list *l){
    ఒకవేళ(!l->వెనుక) తిరిగి 0;
    కణుపు *b = l->వెనుక;
    శూన్య *val = b->val;
    l->వెనుక = b->prev;
    ఒకవేళ(l->వెనుక) l->వెనుక->తదుపరి = 0;
    విడుదల(b);
    --l->కొలత;
    
    తిరిగి val;
}

శూన్య list_చొప్పించు(list *l, శూన్య *val)
{
	కణుపు *new = జ్ఞాపకం_ఇవ్వు(పరిమాణం(కణుపు));
	new->val = val;
	new->తదుపరి = 0;

	ఒకవేళ(!l->వెనుక){
		l->front = new;
		new->prev = 0;
	}లేకపోతే{
		l->వెనుక->తదుపరి = new;
		new->prev = l->వెనుక;
	}
	l->వెనుక = new;
	++l->కొలత;
}

శూన్య free_కణుపు(కణుపు *n)
{
	కణుపు *తదుపరి;
	అయితే(n) {
		తదుపరి = n->తదుపరి;
		విడుదల(n);
		n = తదుపరి;
	}
}

శూన్య free_list(list *l)
{
	free_కణుపు(l->front);
	విడుదల(l);
}

శూన్య free_list_contents(list *l)
{
	కణుపు *n = l->front;
	అయితే(n){
		విడుదల(n->val);
		n = n->తదుపరి;
	}
}

శూన్య **list_గమ్యం_array(list *l)
{
    శూన్య **a = సున్నా_జ్ఞాపకం_ఇవ్వు(l->కొలత, పరిమాణం(శూన్య*));
    పూర్ణాంకం లెక్క = 0;
    కణుపు *n = l->front;
    అయితే(n){
        a[లెక్క++] = n->val;
        n = n->తదుపరి;
    }
    తిరిగి a;
}
