#నిర్వచితం_కాకపోతే UTILS_H
#నిర్వచించు UTILS_H
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ "darknet.h"
#అన్తర్భావ "list.h"

#నిర్వచించు సమయం_2(a) \
    చేయి { \
    ద్విగుణ ప్రారంభం = what_సమయం_is_it_now(); \
    a; \
    రాయి("%s took: %f seconds\n", #a, what_సమయం_is_it_now() - ప్రారంభం); \
    } అయితే (0)

#నిర్వచించు TWO_PI 6.2831853071795864769252866f

ద్విగుణ what_సమయం_is_it_now();
శూన్య shuffle(శూన్య *arr, size_t n, size_t కొలత);
శూన్య sorta_shuffle(శూన్య *arr, size_t n, size_t కొలత, size_t sections);
శూన్య free_ptrs(శూన్య **ptrs, పూర్ణాంకం n);
పూర్ణాంకం alphanum_గమ్యం_int(అక్షరం c);
అక్షరం int_గమ్యం_alphanum(పూర్ణాంకం i);
పూర్ణాంకం read_int(పూర్ణాంకం fd);
శూన్య write_int(పూర్ణాంకం fd, పూర్ణాంకం n);
శూన్య read_all(పూర్ణాంకం fd, అక్షరం *బఫరు, size_t బైట్లు);
శూన్య write_all(పూర్ణాంకం fd, అక్షరం *బఫరు, size_t బైట్లు);
పూర్ణాంకం read_all_fail(పూర్ణాంకం fd, అక్షరం *బఫరు, size_t బైట్లు);
పూర్ణాంకం write_all_fail(పూర్ణాంకం fd, అక్షరం *బఫరు, size_t బైట్లు);
శూన్య కనుగొను_replace(అక్షరం *str, అక్షరం *orig, అక్షరం *rep, అక్షరం *output);
శూన్య malloc_error();
శూన్య file_error(అక్షరం *s);
శూన్య strip(అక్షరం *s);
శూన్య strip_char(అక్షరం *s, అక్షరం bad);
list *విభజించు_str(అక్షరం *s, అక్షరం delim);
అక్షరం *fgetl(FILE *fp);
list *parse_csv_పంక్తి(అక్షరం *పంక్తి);
అక్షరం *copy_string(అక్షరం *s);
పూర్ణాంకం లెక్క_fields(అక్షరం *పంక్తి);
దశాంశం *parse_fields(అక్షరం *పంక్తి, పూర్ణాంకం n);
శూన్య translate_array(దశాంశం *a, పూర్ణాంకం n, దశాంశం s);
దశాంశం constrain(దశాంశం min, దశాంశం max, దశాంశం a);
పూర్ణాంకం constrain_int(పూర్ణాంకం a, పూర్ణాంకం min, పూర్ణాంకం max);
దశాంశం rand_scale(దశాంశం s);
పూర్ణాంకం rand_int(పూర్ణాంకం min, పూర్ణాంకం max);
శూన్య సగటు_arrays(దశాంశం **a, పూర్ణాంకం n, పూర్ణాంకం els, దశాంశం *avg);
దశాంశం dist_array(దశాంశం *a, దశాంశం *b, పూర్ణాంకం n, పూర్ణాంకం sub);
దశాంశం **one_hot_encode(దశాంశం *a, పూర్ణాంకం n, పూర్ణాంకం k);
దశాంశం sec(clock_t clocks);
శూన్య print_statistics(దశాంశం *a, పూర్ణాంకం n);
పూర్ణాంకం int_సూచీ(పూర్ణాంకం *a, పూర్ణాంకం val, పూర్ణాంకం n);

#అయితే_ముగింపు

