#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#அந்தர்பாவ "list.h"

வகைவரை கட்டமைப்பு{
    எழுத்து *சாவி;
    எழுத்து *val;
    முழு used;
} kvp;


முழு read_option(எழுத்து *s, list *options);
வெறுமை option_செருகு(list *l, எழுத்து *சாவி, எழுத்து *val);
எழுத்து *option_find(list *l, எழுத்து *சாவி);
தசம option_find_float(list *l, எழுத்து *சாவி, தசம def);
தசம option_find_float_quiet(list *l, எழுத்து *சாவி, தசம def);
வெறுமை option_unused(list *l);

#endif
