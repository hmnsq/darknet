#ifndef LIST_H
#define LIST_H
#અન્તર્ભાવ "darknet.h"

list *make_list();
પૂર્ણાંક list_find(list *l, શૂન્ય *val);

શૂન્ય list_ઉમેરો(list *, શૂન્ય *);


શૂન્ય free_list_contents(list *l);

#endif
