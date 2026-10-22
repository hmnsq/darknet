#ifndef UTILS_H
#define UTILS_H
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ "darknet.h"
#અન્તર્ભાવ "list.h"

#define TIME(a) \
    કરો { \
    દ્વિગુણ start = what_time_is_it_now(); \
    a; \
    printf("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } જ્યારે (0)

#define TWO_PI 6.2831853071795864769252866f

દ્વિગુણ what_time_is_it_now();
શૂન્ય shuffle(શૂન્ય *arr, size_t n, size_t size);
શૂન્ય sorta_shuffle(શૂન્ય *arr, size_t n, size_t size, size_t sections);
શૂન્ય free_ptrs(શૂન્ય **ptrs, પૂર્ણાંક n);
પૂર્ણાંક alphanum_to_int(અક્ષર c);
અક્ષર int_to_alphanum(પૂર્ણાંક i);
પૂર્ણાંક read_int(પૂર્ણાંક fd);
શૂન્ય write_int(પૂર્ણાંક fd, પૂર્ણાંક n);
શૂન્ય read_all(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes);
શૂન્ય write_all(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes);
પૂર્ણાંક read_all_fail(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes);
પૂર્ણાંક write_all_fail(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes);
શૂન્ય find_replace(અક્ષર *str, અક્ષર *orig, અક્ષર *rep, અક્ષર *output);
શૂન્ય malloc_error();
શૂન્ય file_error(અક્ષર *s);
શૂન્ય strip(અક્ષર *s);
શૂન્ય strip_char(અક્ષર *s, અક્ષર bad);
list *split_str(અક્ષર *s, અક્ષર delim);
અક્ષર *fgetl(FILE *fp);
list *parse_csv_લીટી(અક્ષર *લીટી);
અક્ષર *copy_string(અક્ષર *s);
પૂર્ણાંક ગણતરી_fields(અક્ષર *લીટી);
દશાંશ *parse_fields(અક્ષર *લીટી, પૂર્ણાંક n);
શૂન્ય translate_array(દશાંશ *a, પૂર્ણાંક n, દશાંશ s);
દશાંશ constrain(દશાંશ min, દશાંશ max, દશાંશ a);
પૂર્ણાંક constrain_int(પૂર્ણાંક a, પૂર્ણાંક min, પૂર્ણાંક max);
દશાંશ rand_scale(દશાંશ s);
પૂર્ણાંક rand_int(પૂર્ણાંક min, પૂર્ણાંક max);
શૂન્ય mean_arrays(દશાંશ **a, પૂર્ણાંક n, પૂર્ણાંક els, દશાંશ *avg);
દશાંશ dist_array(દશાંશ *a, દશાંશ *b, પૂર્ણાંક n, પૂર્ણાંક sub);
દશાંશ **one_hot_encode(દશાંશ *a, પૂર્ણાંક n, પૂર્ણાંક k);
દશાંશ sec(clock_t clocks);
શૂન્ય print_statistics(દશાંશ *a, પૂર્ણાંક n);
પૂર્ણાંક int_index(પૂર્ણાંક *a, પૂર્ણાંક val, પૂર્ણાંક n);

#endif

