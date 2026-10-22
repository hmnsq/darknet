#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "list.h"

list *make_list()
{
	list *l = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(list));
	l->ಗಾತ್ರ_2 = 0;
	l->front = 0;
	l->ಹಿಂಭಾಗ = 0;
	ಹಿಂತಿರುಗಿ l;
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
    ಆದರೆ(!l->ಹಿಂಭಾಗ) ಹಿಂತಿರುಗಿ 0;
    ಗಂಟು *b = l->ಹಿಂಭಾಗ;
    ಶೂನ್ಯ *val = b->val;
    l->ಹಿಂಭಾಗ = b->prev;
    ಆದರೆ(l->ಹಿಂಭಾಗ) l->ಹಿಂಭಾಗ->ಮುಂದಿನ = 0;
    ಬಿಡುಗಡೆ(b);
    --l->ಗಾತ್ರ_2;
    
    ಹಿಂತಿರುಗಿ val;
}

ಶೂನ್ಯ list_ಸೇರಿಸು(list *l, ಶೂನ್ಯ *val)
{
	ಗಂಟು *new = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ಗಂಟು));
	new->val = val;
	new->ಮುಂದಿನ = 0;

	ಆದರೆ(!l->ಹಿಂಭಾಗ){
		l->front = new;
		new->prev = 0;
	}ಇಲ್ಲದಿದ್ದರೆ{
		l->ಹಿಂಭಾಗ->ಮುಂದಿನ = new;
		new->prev = l->ಹಿಂಭಾಗ;
	}
	l->ಹಿಂಭಾಗ = new;
	++l->ಗಾತ್ರ_2;
}

ಶೂನ್ಯ free_ಗಂಟು(ಗಂಟು *n)
{
	ಗಂಟು *ಮುಂದಿನ;
	ಆಗ(n) {
		ಮುಂದಿನ = n->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(n);
		n = ಮುಂದಿನ;
	}
}

ಶೂನ್ಯ free_list(list *l)
{
	free_ಗಂಟು(l->front);
	ಬಿಡುಗಡೆ(l);
}

ಶೂನ್ಯ free_list_contents(list *l)
{
	ಗಂಟು *n = l->front;
	ಆಗ(n){
		ಬಿಡುಗಡೆ(n->val);
		n = n->ಮುಂದಿನ;
	}
}

ಶೂನ್ಯ **list_ಗಮ್ಯ_array(list *l)
{
    ಶೂನ್ಯ **a = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l->ಗಾತ್ರ_2, ಗಾತ್ರ(ಶೂನ್ಯ*));
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಂಟು *n = l->front;
    ಆಗ(n){
        a[ಎಣಿಕೆ_2++] = n->val;
        n = n->ಮುಂದಿನ;
    }
    ಹಿಂತಿರುಗಿ a;
}
