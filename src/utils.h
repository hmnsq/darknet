#ifndef UTILS_H
#define UTILS_H
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव "darknet.h"
#अन्तर्भाव "list.h"

#define TIME(a) \
    करा { \
    द्विगुण start = what_time_is_it_now(); \
    a; \
    म_लिखो("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } पर्यंत (0)

#define TWO_PI 6.2831853071795864769252866f

द्विगुण what_time_is_it_now();
शून्य shuffle(शून्य *arr, माप_प्रकार n, माप_प्रकार size);
शून्य sorta_shuffle(शून्य *arr, माप_प्रकार n, माप_प्रकार size, माप_प्रकार sections);
शून्य free_ptrs(शून्य **ptrs, पूर्णांक n);
पूर्णांक alphanum_to_int(अक्षर c);
अक्षर int_to_alphanum(पूर्णांक i);
पूर्णांक read_int(पूर्णांक fd);
शून्य write_int(पूर्णांक fd, पूर्णांक n);
शून्य read_all(पूर्णांक fd, अक्षर *buffer, माप_प्रकार bytes);
शून्य write_all(पूर्णांक fd, अक्षर *buffer, माप_प्रकार bytes);
पूर्णांक read_all_fail(पूर्णांक fd, अक्षर *buffer, माप_प्रकार bytes);
पूर्णांक write_all_fail(पूर्णांक fd, अक्षर *buffer, माप_प्रकार bytes);
शून्य find_replace(अक्षर *str, अक्षर *orig, अक्षर *rep, अक्षर *output);
शून्य malloc_error();
शून्य file_error(अक्षर *s);
शून्य strip(अक्षर *s);
शून्य strip_char(अक्षर *s, अक्षर bad);
list *split_str(अक्षर *s, अक्षर delim);
अक्षर *fgetl(खाता *fp);
list *parse_csv_ओळ(अक्षर *ओळ);
अक्षर *copy_string(अक्षर *s);
पूर्णांक मोजणी_fields(अक्षर *ओळ);
दशांश *parse_fields(अक्षर *ओळ, पूर्णांक n);
शून्य translate_array(दशांश *a, पूर्णांक n, दशांश s);
दशांश constrain(दशांश min, दशांश max, दशांश a);
पूर्णांक constrain_int(पूर्णांक a, पूर्णांक min, पूर्णांक max);
दशांश rand_scale(दशांश s);
पूर्णांक rand_int(पूर्णांक min, पूर्णांक max);
शून्य mean_arrays(दशांश **a, पूर्णांक n, पूर्णांक els, दशांश *avg);
दशांश dist_array(दशांश *a, दशांश *b, पूर्णांक n, पूर्णांक sub);
दशांश **one_hot_encode(दशांश *a, पूर्णांक n, पूर्णांक k);
दशांश sec(घड़ी_प्रकार clocks);
शून्य print_statistics(दशांश *a, पूर्णांक n);
पूर्णांक int_index(पूर्णांक *a, पूर्णांक val, पूर्णांक n);

#endif

