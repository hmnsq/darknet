#جے_بنا_تعریف UTILS_H
#تعریف_کرو UTILS_H
#ضمن <stdio.h>
#ضمن <time.h>
#ضمن "darknet.h"
#ضمن "list.h"

#تعریف_کرو ویلا_2(a) \
    افعل { \
    مزدوج شروع = what_ویلا_is_it_now(); \
    a; \
    لکھو("%s took: %f seconds\n", #a, what_ویلا_is_it_now() - شروع); \
    } طالما (0)

#تعریف_کرو TWO_PI 6.2831853071795864769252866f

مزدوج what_ویلا_is_it_now();
فراغ shuffle(فراغ *arr, size_t n, size_t ناپ);
فراغ sorta_shuffle(فراغ *arr, size_t n, size_t ناپ, size_t sections);
فراغ free_ptrs(فراغ **ptrs, صحيح n);
صحيح alphanum_منزل_int(حرف c);
حرف int_منزل_alphanum(صحيح i);
صحيح read_int(صحيح fd);
فراغ write_int(صحيح fd, صحيح n);
فراغ read_all(صحيح fd, حرف *بفر, size_t بائٹاں);
فراغ write_all(صحيح fd, حرف *بفر, size_t بائٹاں);
صحيح read_all_fail(صحيح fd, حرف *بفر, size_t بائٹاں);
صحيح write_all_fail(صحيح fd, حرف *بفر, size_t بائٹاں);
فراغ لبھو_replace(حرف *str, حرف *orig, حرف *rep, حرف *output);
فراغ malloc_error();
فراغ file_error(حرف *s);
فراغ strip(حرف *s);
فراغ strip_char(حرف *s, حرف bad);
list *ونڈو_str(حرف *s, حرف delim);
حرف *fgetl(FILE *fp);
list *parse_csv_سطر(حرف *سطر);
حرف *copy_string(حرف *s);
صحيح گنتی_fields(حرف *سطر);
عائم *parse_fields(حرف *سطر, صحيح n);
فراغ translate_array(عائم *a, صحيح n, عائم s);
عائم constrain(عائم min, عائم max, عائم a);
صحيح constrain_int(صحيح a, صحيح min, صحيح max);
عائم rand_scale(عائم s);
صحيح rand_int(صحيح min, صحيح max);
فراغ اوسط_arrays(عائم **a, صحيح n, صحيح els, عائم *avg);
عائم dist_array(عائم *a, عائم *b, صحيح n, صحيح sub);
عائم **one_hot_encode(عائم *a, صحيح n, صحيح k);
عائم sec(clock_t clocks);
فراغ print_statistics(عائم *a, صحيح n);
صحيح int_اشاریہ(صحيح *a, صحيح val, صحيح n);

#جے_انت

