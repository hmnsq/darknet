#ifndef UTILS_H
#define UTILS_H
#شامل <stdio.h>
#شامل <time.h>
#شامل "darknet.h"
#شامل "list.h"

#define TIME(a) \
    انجامبده { \
    دوگانه start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } وقتی (0)

#define TWO_PI 6.2831853071795864769252866f

دوگانه what_time_is_it_now();
تهی shuffle(تهی *arr, size_t n, size_t size);
تهی sorta_shuffle(تهی *arr, size_t n, size_t size, size_t sections);
تهی free_ptrs(تهی **ptrs, صحیح n);
صحیح alphanum_to_int(نویسه c);
نویسه int_to_alphanum(صحیح i);
صحیح read_int(صحیح fd);
تهی write_int(صحیح fd, صحیح n);
تهی read_all(صحیح fd, نویسه *buffer, size_t bytes);
تهی write_all(صحیح fd, نویسه *buffer, size_t bytes);
صحیح read_all_fail(صحیح fd, نویسه *buffer, size_t bytes);
صحیح write_all_fail(صحیح fd, نویسه *buffer, size_t bytes);
تهی find_replace(نویسه *str, نویسه *orig, نویسه *rep, نویسه *output);
تهی malloc_error();
تهی file_error(نویسه *s);
تهی strip(نویسه *s);
تهی strip_char(نویسه *s, نویسه bad);
list *split_str(نویسه *s, نویسه delim);
نویسه *fgetl(FILE *fp);
list *parse_csv_سطر(نویسه *سطر);
نویسه *copy_string(نویسه *s);
صحیح شمار_fields(نویسه *سطر);
اعشاری *parse_fields(نویسه *سطر, صحیح n);
تهی translate_array(اعشاری *a, صحیح n, اعشاری s);
اعشاری constrain(اعشاری min, اعشاری max, اعشاری a);
صحیح constrain_int(صحیح a, صحیح min, صحیح max);
اعشاری rand_scale(اعشاری s);
صحیح rand_int(صحیح min, صحیح max);
تهی mean_arrays(اعشاری **a, صحیح n, صحیح els, اعشاری *avg);
اعشاری dist_array(اعشاری *a, اعشاری *b, صحیح n, صحیح sub);
اعشاری **one_hot_encode(اعشاری *a, صحیح n, صحیح k);
اعشاری sec(clock_t clocks);
تهی print_statistics(اعشاری *a, صحیح n);
صحیح int_index(صحیح *a, صحیح val, صحیح n);

#endif

