#अगर_अघोषित OPTION_LIST_H
#घोषणा OPTION_LIST_H
#समावेश "list.h"

प्रकार काष्ठा{
    अक्षर *कुंजी;
    अक्षर *val;
    पूर्णांक used;
} kvp;


पूर्णांक read_option(अक्षर *s, list *options);
व्योम option_जोड़ो(list *l, अक्षर *कुंजी, अक्षर *val);
अक्षर *option_find(list *l, अक्षर *कुंजी);
भग्न option_find_float(list *l, अक्षर *कुंजी, भग्न def);
भग्न option_find_float_quiet(list *l, अक्षर *कुंजी, भग्न def);
व्योम option_unused(list *l);

#पूर्ण_अगर
