#ifndef LIST_H
#define LIST_H
#ضمن "darknet.h"

list *make_list();
صحيح list_find(list *l, فراغ *val);

فراغ list_قىستۇرۇش(list *, فراغ *);


فراغ free_list_contents(list *l);

#endif
