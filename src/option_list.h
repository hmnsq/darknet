#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ಅನ್ತರ್ಭಾವ "list.h"

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ{
    ವರ್ಣ *ಕೀಲಿ;
    ವರ್ಣ *val;
    ಪೂರ್ಣಾಙ್ಕ used;
} kvp;


ಪೂರ್ಣಾಙ್ಕ read_option(ವರ್ಣ *s, list *options);
ಶೂನ್ಯ option_ಸೇರಿಸು(list *l, ವರ್ಣ *ಕೀಲಿ, ವರ್ಣ *val);
ವರ್ಣ *option_find(list *l, ವರ್ಣ *ಕೀಲಿ);
ಭಿನ್ನ option_find_float(list *l, ವರ್ಣ *ಕೀಲಿ, ಭಿನ್ನ def);
ಭಿನ್ನ option_find_float_quiet(list *l, ವರ್ಣ *ಕೀಲಿ, ಭಿನ್ನ def);
ಶೂನ್ಯ option_unused(list *l);

#endif
