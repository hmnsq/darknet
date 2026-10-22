#اگر_تعریف_نشده UTILS_H
#تعریف_کن UTILS_H
#ضمن <stdio.h>
#ضمن <time.h>
#ضمن "darknet.h"
#ضمن "list.h"

#تعریف_کن زمان_2(a) \
    افعل { \
    مزدوج شروع = what_زمان_is_it_now(); \
    a; \
    چاپ_کن("%s took: %f seconds\n", #a, what_زمان_is_it_now() - شروع); \
    } طالما (0)

#تعریف_کن TWO_PI 6.2831853071795864769252866f

مزدوج what_زمان_is_it_now();
فراغ shuffle(فراغ *arr, size_t n, size_t اندازه);
فراغ sorta_shuffle(فراغ *arr, size_t n, size_t اندازه, size_t sections);
فراغ free_ptrs(فراغ **ptrs, صحيح n);
صحيح alphanum_مقصد_int(حرف c);
حرف int_مقصد_alphanum(صحيح i);
صحيح read_int(صحيح fd);
فراغ write_int(صحيح fd, صحيح n);
فراغ read_all(صحيح fd, حرف *میانگیر, size_t بایتها);
فراغ write_all(صحيح fd, حرف *میانگیر, size_t بایتها);
صحيح read_all_fail(صحيح fd, حرف *میانگیر, size_t بایتها);
صحيح write_all_fail(صحيح fd, حرف *میانگیر, size_t بایتها);
فراغ پیدا_کن_replace(حرف *str, حرف *orig, حرف *rep, حرف *output);
فراغ malloc_error();
فراغ file_error(حرف *s);
فراغ strip(حرف *s);
فراغ strip_char(حرف *s, حرف bad);
list *تقسیم_str(حرف *s, حرف delim);
حرف *fgetl(FILE *fp);
list *parse_csv_سطر(حرف *سطر);
حرف *copy_string(حرف *s);
صحيح شمار_fields(حرف *سطر);
عائم *parse_fields(حرف *سطر, صحيح n);
فراغ translate_array(عائم *a, صحيح n, عائم s);
عائم constrain(عائم min, عائم max, عائم a);
صحيح constrain_int(صحيح a, صحيح min, صحيح max);
عائم rand_scale(عائم s);
صحيح rand_int(صحيح min, صحيح max);
فراغ میانگین_arrays(عائم **a, صحيح n, صحيح els, عائم *avg);
عائم dist_array(عائم *a, عائم *b, صحيح n, صحيح sub);
عائم **one_hot_encode(عائم *a, صحيح n, صحيح k);
عائم sec(clock_t clocks);
فراغ print_statistics(عائم *a, صحيح n);
صحيح int_شاخص(صحيح *a, صحيح val, صحيح n);

#پایان_اگر

