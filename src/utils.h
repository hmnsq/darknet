#ifndef UTILS_H
#define UTILS_H
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "list.h"

#define TIME(a) \
    করো { \
    দ্বিগুণ start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } যখন (0)

#define TWO_PI 6.2831853071795864769252866f

দ্বিগুণ what_time_is_it_now();
শূন্য shuffle(শূন্য *arr, size_t n, size_t আকার_2);
শূন্য sorta_shuffle(শূন্য *arr, size_t n, size_t আকার_2, size_t sections);
শূন্য free_ptrs(শূন্য **ptrs, পূর্ণসংখ্যা n);
পূর্ণসংখ্যা alphanum_to_int(অক্ষর c);
অক্ষর int_to_alphanum(পূর্ণসংখ্যা i);
পূর্ণসংখ্যা read_int(পূর্ণসংখ্যা fd);
শূন্য write_int(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা n);
শূন্য read_all(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes);
শূন্য write_all(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes);
পূর্ণসংখ্যা read_all_fail(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes);
পূর্ণসংখ্যা write_all_fail(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes);
শূন্য find_replace(অক্ষর *str, অক্ষর *orig, অক্ষর *rep, অক্ষর *output);
শূন্য malloc_error();
শূন্য file_error(অক্ষর *s);
শূন্য strip(অক্ষর *s);
শূন্য strip_char(অক্ষর *s, অক্ষর bad);
list *split_str(অক্ষর *s, অক্ষর delim);
অক্ষর *fgetl(FILE *fp);
list *parse_csv_সারি(অক্ষর *সারি);
অক্ষর *copy_string(অক্ষর *s);
পূর্ণসংখ্যা গুনতি_fields(অক্ষর *সারি);
দশমিক *parse_fields(অক্ষর *সারি, পূর্ণসংখ্যা n);
শূন্য translate_array(দশমিক *a, পূর্ণসংখ্যা n, দশমিক s);
দশমিক constrain(দশমিক min, দশমিক max, দশমিক a);
পূর্ণসংখ্যা constrain_int(পূর্ণসংখ্যা a, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max);
দশমিক rand_scale(দশমিক s);
পূর্ণসংখ্যা rand_int(পূর্ণসংখ্যা min, পূর্ণসংখ্যা max);
শূন্য mean_arrays(দশমিক **a, পূর্ণসংখ্যা n, পূর্ণসংখ্যা els, দশমিক *avg);
দশমিক dist_array(দশমিক *a, দশমিক *b, পূর্ণসংখ্যা n, পূর্ণসংখ্যা sub);
দশমিক **one_hot_encode(দশমিক *a, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k);
দশমিক sec(clock_t clocks);
শূন্য print_statistics(দশমিক *a, পূর্ণসংখ্যা n);
পূর্ণসংখ্যা int_সূচক(পূর্ণসংখ্যা *a, পূর্ণসংখ্যা val, পূর্ণসংখ্যা n);

#endif

