#اگر_غیر_تعریف_شدہ UTILS_H
#تعریف_کرو UTILS_H
#شامل <stdio.h>
#شامل <time.h>
#شامل "darknet.h"
#شامل "list.h"

#تعریف_کرو وقت_2(a) \
    کرو { \
    دوہرا آغاز = what_وقت_is_it_now(); \
    a; \
    لکھو("%s took: %f seconds\n", #a, what_وقت_is_it_now() - آغاز); \
    } جب (0)

#تعریف_کرو TWO_PI 6.2831853071795864769252866f

دوہرا what_وقت_is_it_now();
خالی shuffle(خالی *arr, size_t n, size_t سائز);
خالی sorta_shuffle(خالی *arr, size_t n, size_t سائز, size_t sections);
خالی free_ptrs(خالی **ptrs, صحیح n);
صحیح alphanum_مقصد_int(حرف c);
حرف int_مقصد_alphanum(صحیح i);
صحیح read_int(صحیح fd);
خالی write_int(صحیح fd, صحیح n);
خالی read_all(صحیح fd, حرف *بفر, size_t بائٹیں);
خالی write_all(صحیح fd, حرف *بفر, size_t بائٹیں);
صحیح read_all_fail(صحیح fd, حرف *بفر, size_t بائٹیں);
صحیح write_all_fail(صحیح fd, حرف *بفر, size_t بائٹیں);
خالی ڈھونڈو_replace(حرف *str, حرف *orig, حرف *rep, حرف *output);
خالی malloc_error();
خالی file_error(حرف *s);
خالی strip(حرف *s);
خالی strip_char(حرف *s, حرف bad);
list *بانٹو_str(حرف *s, حرف delim);
حرف *fgetl(FILE *fp);
list *parse_csv_سطر(حرف *سطر);
حرف *copy_string(حرف *s);
صحیح گنتی_fields(حرف *سطر);
اعشاری *parse_fields(حرف *سطر, صحیح n);
خالی translate_array(اعشاری *a, صحیح n, اعشاری s);
اعشاری constrain(اعشاری min, اعشاری max, اعشاری a);
صحیح constrain_int(صحیح a, صحیح min, صحیح max);
اعشاری rand_scale(اعشاری s);
صحیح rand_int(صحیح min, صحیح max);
خالی اوسط_arrays(اعشاری **a, صحیح n, صحیح els, اعشاری *avg);
اعشاری dist_array(اعشاری *a, اعشاری *b, صحیح n, صحیح sub);
اعشاری **one_hot_encode(اعشاری *a, صحیح n, صحیح k);
اعشاری sec(clock_t clocks);
خالی print_statistics(اعشاری *a, صحیح n);
صحیح int_اشاریہ(صحیح *a, صحیح val, صحیح n);

#اگر_ختم

