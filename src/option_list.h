#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#અન્તર્ભાવ "list.h"

પ્રકારવ્યાખ્યા સંરચના{
    અક્ષર *ચાવી;
    અક્ષર *val;
    પૂર્ણાંક used;
} kvp;


પૂર્ણાંક read_option(અક્ષર *s, list *options);
શૂન્ય option_ઉમેરો(list *l, અક્ષર *ચાવી, અક્ષર *val);
અક્ષર *option_find(list *l, અક્ષર *ચાવી);
દશાંશ option_find_float(list *l, અક્ષર *ચાવી, દશાંશ def);
દશાંશ option_find_float_quiet(list *l, અક્ષર *ચાવી, દશાંશ def);
શૂન્ય option_unused(list *l);

#endif
