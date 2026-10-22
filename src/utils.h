#ifndef UTILS_H
#define UTILS_H
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ "darknet.h"
#ଅନ୍ତର୍ଭାଵ "list.h"

#define TIME(a) \
    କର { \
    ଦ୍ୱିଗୁଣ start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } ଯେତେବେଳେ (0)

#define TWO_PI 6.2831853071795864769252866f

ଦ୍ୱିଗୁଣ what_time_is_it_now();
ଶୂନ୍ୟ shuffle(ଶୂନ୍ୟ *arr, size_t n, size_t size);
ଶୂନ୍ୟ sorta_shuffle(ଶୂନ୍ୟ *arr, size_t n, size_t size, size_t sections);
ଶୂନ୍ୟ free_ptrs(ଶୂନ୍ୟ **ptrs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା alphanum_to_int(ଅକ୍ଷର c);
ଅକ୍ଷର int_to_alphanum(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd);
ଶୂନ୍ୟ write_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ଶୂନ୍ୟ read_all(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes);
ଶୂନ୍ୟ write_all(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_all_fail(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_all_fail(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes);
ଶୂନ୍ୟ find_replace(ଅକ୍ଷର *str, ଅକ୍ଷର *orig, ଅକ୍ଷର *rep, ଅକ୍ଷର *output);
ଶୂନ୍ୟ malloc_error();
ଶୂନ୍ୟ file_error(ଅକ୍ଷର *s);
ଶୂନ୍ୟ strip(ଅକ୍ଷର *s);
ଶୂନ୍ୟ strip_char(ଅକ୍ଷର *s, ଅକ୍ଷର bad);
list *split_str(ଅକ୍ଷର *s, ଅକ୍ଷର delim);
ଅକ୍ଷର *fgetl(FILE *fp);
list *parse_csv_ଧାଡ଼ି(ଅକ୍ଷର *ଧାଡ଼ି);
ଅକ୍ଷର *copy_string(ଅକ୍ଷର *s);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_fields(ଅକ୍ଷର *ଧାଡ଼ି);
ଦଶମିକ *parse_fields(ଅକ୍ଷର *ଧାଡ଼ି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ଶୂନ୍ୟ translate_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ s);
ଦଶମିକ constrain(ଦଶମିକ min, ଦଶମିକ max, ଦଶମିକ a);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା constrain_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max);
ଦଶମିକ rand_scale(ଦଶମିକ s);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା rand_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max);
ଶୂନ୍ୟ mean_arrays(ଦଶମିକ **a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା els, ଦଶମିକ *avg);
ଦଶମିକ dist_array(ଦଶମିକ *a, ଦଶମିକ *b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sub);
ଦଶମିକ **one_hot_encode(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k);
ଦଶମିକ sec(clock_t clocks);
ଶୂନ୍ୟ print_statistics(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା int_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା val, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);

#endif

