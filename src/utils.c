#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <float.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/time.h>

#অন্তর্ভাব "utils.h"


/*
// old timing. is it better? who knows!!
double get_wall_time()
{
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
*/

দ্বিগুণ what_time_is_it_now()
{
    কাঠামো timeval time;
    যদি (gettimeofday(&time,ফাঁকা)){
        ফেরত 0;
    }
    ফেরত (দ্বিগুণ)time.tv_sec + (দ্বিগুণ)time.tv_usec * .000001;
}

পূর্ণসংখ্যা *read_intlist(অক্ষর *gpu_list, পূর্ণসংখ্যা *ngpus, পূর্ণসংখ্যা d)
{
    পূর্ণসংখ্যা *gpus = 0;
    যদি(gpu_list){
        পূর্ণসংখ্যা len = strlen(gpu_list);
        *ngpus = 1;
        পূর্ণসংখ্যা i;
        জন্য(i = 0; i < len; ++i){
            যদি (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, আকার(পূর্ণসংখ্যা));
        জন্য(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } অন্যথা {
        gpus = calloc(1, আকার(দশমিক));
        *gpus = d;
        *ngpus = 1;
    }
    ফেরত gpus;
}

পূর্ণসংখ্যা *read_map(অক্ষর *filename)
{
    পূর্ণসংখ্যা n = 0;
    পূর্ণসংখ্যা *map = 0;
    অক্ষর *str;
    FILE *file = fopen(filename, "r");
    যদি(!file) file_error(filename);
    যখন((str=fgetl(file))){
        ++n;
        map = realloc(map, n*আকার(পূর্ণসংখ্যা));
        map[n-1] = atoi(str);
    }
    ফেরত map;
}

শূন্য sorta_shuffle(শূন্য *arr, size_t n, size_t আকার_2, size_t sections)
{
    size_t i;
    জন্য(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t অঙ্ক = end-start;
        shuffle(arr+(start*আকার_2), অঙ্ক, আকার_2);
    }
}

শূন্য shuffle(শূন্য *arr, size_t n, size_t আকার_2)
{
    size_t i;
    শূন্য *swp = calloc(1, আকার_2);
    জন্য(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*আকার_2), আকার_2);
        memcpy(arr+(j*আকার_2), arr+(i*আকার_2), আকার_2);
        memcpy(arr+(i*আকার_2), swp,          আকার_2);
    }
}

পূর্ণসংখ্যা *random_সূচক_order(পূর্ণসংখ্যা min, পূর্ণসংখ্যা max)
{
    পূর্ণসংখ্যা *inds = calloc(max-min, আকার(পূর্ণসংখ্যা));
    পূর্ণসংখ্যা i;
    জন্য(i = min; i < max; ++i){
        inds[i] = i;
    }
    জন্য(i = min; i < max-1; ++i){
        পূর্ণসংখ্যা swap = inds[i];
        পূর্ণসংখ্যা সূচক = i + rand()%(max-i);
        inds[i] = inds[সূচক];
        inds[সূচক] = swap;
    }
    ফেরত inds;
}

শূন্য del_arg(পূর্ণসংখ্যা argc, অক্ষর **argv, পূর্ণসংখ্যা সূচক)
{
    পূর্ণসংখ্যা i;
    জন্য(i = সূচক; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

পূর্ণসংখ্যা find_arg(পূর্ণসংখ্যা argc, অক্ষর* argv[], অক্ষর *arg)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < argc; ++i) {
        যদি(!argv[i]) চালু;
        যদি(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ফেরত 1;
        }
    }
    ফেরত 0;
}

পূর্ণসংখ্যা find_int_arg(পূর্ণসংখ্যা argc, অক্ষর **argv, অক্ষর *arg, পূর্ণসংখ্যা def)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < argc-1; ++i){
        যদি(!argv[i]) চালু;
        যদি(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            বিরতি;
        }
    }
    ফেরত def;
}

দশমিক find_float_arg(পূর্ণসংখ্যা argc, অক্ষর **argv, অক্ষর *arg, দশমিক def)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < argc-1; ++i){
        যদি(!argv[i]) চালু;
        যদি(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            বিরতি;
        }
    }
    ফেরত def;
}

অক্ষর *find_char_arg(পূর্ণসংখ্যা argc, অক্ষর **argv, অক্ষর *arg, অক্ষর *def)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < argc-1; ++i){
        যদি(!argv[i]) চালু;
        যদি(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            বিরতি;
        }
    }
    ফেরত def;
}


অক্ষর *basecfg(অক্ষর *cfgfile)
{
    অক্ষর *c = cfgfile;
    অক্ষর *পরবর্তী;
    যখন((পরবর্তী = strchr(c, '/')))
    {
        c = পরবর্তী+1;
    }
    c = copy_string(c);
    পরবর্তী = strchr(c, '.');
    যদি (পরবর্তী) *পরবর্তী = 0;
    ফেরত c;
}

পূর্ণসংখ্যা alphanum_to_int(অক্ষর c)
{
    ফেরত (c < 58) ? c - 48 : c-87;
}
অক্ষর int_to_alphanum(পূর্ণসংখ্যা i)
{
    যদি (i == 36) ফেরত '.';
    ফেরত (i < 10) ? i + 48 : i + 87;
}

শূন্য pm(পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, দশমিক *A)
{
    পূর্ণসংখ্যা i,j;
    জন্য(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        জন্য(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

শূন্য find_replace(অক্ষর *str, অক্ষর *orig, অক্ষর *rep, অক্ষর *output)
{
    অক্ষর বাফার[4096] = {0};
    অক্ষর *p;

    sprintf(বাফার, "%s", str);
    যদি(!(p = strstr(বাফার, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ফেরত;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", বাফার, rep, p+strlen(orig));
}

দশমিক sec(clock_t clocks)
{
    ফেরত (দশমিক)clocks/CLOCKS_PER_SEC;
}

শূন্য শীর্ষ_k(দশমিক *a, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, পূর্ণসংখ্যা *সূচক)
{
    পূর্ণসংখ্যা i,j;
    জন্য(j = 0; j < k; ++j) সূচক[j] = -1;
    জন্য(i = 0; i < n; ++i){
        পূর্ণসংখ্যা curr = i;
        জন্য(j = 0; j < k; ++j){
            যদি((সূচক[j] < 0) || a[curr] > a[সূচক[j]]){
                পূর্ণসংখ্যা swap = curr;
                curr = সূচক[j];
                সূচক[j] = swap;
            }
        }
    }
}

শূন্য error(ধ্রুবক অক্ষর *s)
{
    perror(s);
    প্রতিজ্ঞা(0);
    exit(-1);
}

unsigned অক্ষর *read_file(অক্ষর *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t আকার_2;

    fseek(fp, 0, SEEK_END); 
    আকার_2 = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned অক্ষর *text = calloc(আকার_2+1, আকার(অক্ষর));
    fread(text, 1, আকার_2, fp);
    fclose(fp);
    ফেরত text;
}

শূন্য malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

শূন্য file_error(অক্ষর *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(অক্ষর *s, অক্ষর delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_ঢোকাও(l, s);
    জন্য(i = 0; i < len; ++i){
        যদি(s[i] == delim){
            s[i] = '\0';
            list_ঢোকাও(l, &(s[i+1]));
        }
    }
    ফেরত l;
}

শূন্য strip(অক্ষর *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    জন্য(i = 0; i < len; ++i){
        অক্ষর c = s[i];
        যদি(c==' '||c=='\t'||c=='\n') ++offset;
        অন্যথা s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

শূন্য strip_char(অক্ষর *s, অক্ষর bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    জন্য(i = 0; i < len; ++i){
        অক্ষর c = s[i];
        যদি(c==bad) ++offset;
        অন্যথা s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

শূন্য free_ptrs(শূন্য **ptrs, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i) মুক্ত(ptrs[i]);
    মুক্ত(ptrs);
}

অক্ষর *fgetl(FILE *fp)
{
    যদি(feof(fp)) ফেরত 0;
    size_t আকার_2 = 512;
    অক্ষর *সারি = malloc(আকার_2*আকার(অক্ষর));
    যদি(!fgets(সারি, আকার_2, fp)){
        মুক্ত(সারি);
        ফেরত 0;
    }

    size_t curr = strlen(সারি);

    যখন((সারি[curr-1] != '\n') && !feof(fp)){
        যদি(curr == আকার_2-1){
            আকার_2 *= 2;
            সারি = realloc(সারি, আকার_2*আকার(অক্ষর));
            যদি(!সারি) {
                printf("%ld\n", আকার_2);
                malloc_error();
            }
        }
        size_t readsize = আকার_2-curr;
        যদি(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&সারি[curr], readsize, fp);
        curr = strlen(সারি);
    }
    যদি(সারি[curr-1] == '\n') সারি[curr-1] = '\0';

    ফেরত সারি;
}

পূর্ণসংখ্যা read_int(পূর্ণসংখ্যা fd)
{
    পূর্ণসংখ্যা n = 0;
    পূর্ণসংখ্যা পরবর্তী = পঠ(fd, &n, আকার(পূর্ণসংখ্যা));
    যদি(পরবর্তী <= 0) ফেরত -1;
    ফেরত n;
}

শূন্য write_int(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা পরবর্তী = লিখ(fd, &n, আকার(পূর্ণসংখ্যা));
    যদি(পরবর্তী <= 0) error("read failed");
}

পূর্ণসংখ্যা read_all_fail(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes)
{
    size_t n = 0;
    যখন(n < bytes){
        পূর্ণসংখ্যা পরবর্তী = পঠ(fd, বাফার + n, bytes-n);
        যদি(পরবর্তী <= 0) ফেরত 1;
        n += পরবর্তী;
    }
    ফেরত 0;
}

পূর্ণসংখ্যা write_all_fail(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes)
{
    size_t n = 0;
    যখন(n < bytes){
        size_t পরবর্তী = লিখ(fd, বাফার + n, bytes-n);
        যদি(পরবর্তী <= 0) ফেরত 1;
        n += পরবর্তী;
    }
    ফেরত 0;
}

শূন্য read_all(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes)
{
    size_t n = 0;
    যখন(n < bytes){
        পূর্ণসংখ্যা পরবর্তী = পঠ(fd, বাফার + n, bytes-n);
        যদি(পরবর্তী <= 0) error("read failed");
        n += পরবর্তী;
    }
}

শূন্য write_all(পূর্ণসংখ্যা fd, অক্ষর *বাফার, size_t bytes)
{
    size_t n = 0;
    যখন(n < bytes){
        size_t পরবর্তী = লিখ(fd, বাফার + n, bytes-n);
        যদি(পরবর্তী <= 0) error("write failed");
        n += পরবর্তী;
    }
}


অক্ষর *copy_string(অক্ষর *s)
{
    অক্ষর *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ফেরত copy;
}

list *parse_csv_সারি(অক্ষর *সারি)
{
    list *l = make_list();
    অক্ষর *c, *p;
    পূর্ণসংখ্যা in = 0;
    জন্য(c = সারি, p = সারি; *c != '\0'; ++c){
        যদি(*c == '"') in = !in;
        অন্যথা যদি(*c == ',' && !in){
            *c = '\0';
            list_ঢোকাও(l, copy_string(p));
            p = c+1;
        }
    }
    list_ঢোকাও(l, copy_string(p));
    ফেরত l;
}

পূর্ণসংখ্যা গুনতি_fields(অক্ষর *সারি)
{
    পূর্ণসংখ্যা গুনতি = 0;
    পূর্ণসংখ্যা done = 0;
    অক্ষর *c;
    জন্য(c = সারি; !done; ++c){
        done = (*c == '\0');
        যদি(*c == ',' || done) ++গুনতি;
    }
    ফেরত গুনতি;
}

দশমিক *parse_fields(অক্ষর *সারি, পূর্ণসংখ্যা n)
{
    দশমিক *field = calloc(n, আকার(দশমিক));
    অক্ষর *c, *p, *end;
    পূর্ণসংখ্যা গুনতি = 0;
    পূর্ণসংখ্যা done = 0;
    জন্য(c = সারি, p = সারি; !done; ++c){
        done = (*c == '\0');
        যদি(*c == ',' || done){
            *c = '\0';
            field[গুনতি] = strtod(p, &end);
            যদি(p == c) field[গুনতি] = nan("");
            যদি(end != c && (end != c-1 || *end != '\r')) field[গুনতি] = nan(""); //DOS file formats!
            p = c+1;
            ++গুনতি;
        }
    }
    ফেরত field;
}

দশমিক যোগ_array(দশমিক *a, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    জন্য(i = 0; i < n; ++i) যোগ += a[i];
    ফেরত যোগ;
}

দশমিক mean_array(দশমিক *a, পূর্ণসংখ্যা n)
{
    ফেরত যোগ_array(a,n)/n;
}

শূন্য mean_arrays(দশমিক **a, পূর্ণসংখ্যা n, পূর্ণসংখ্যা els, দশমিক *avg)
{
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা j;
    memset(avg, 0, els*আকার(দশমিক));
    জন্য(j = 0; j < n; ++j){
        জন্য(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    জন্য(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

শূন্য print_statistics(দশমিক *a, পূর্ণসংখ্যা n)
{
    দশমিক m = mean_array(a, n);
    দশমিক v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

দশমিক variance_array(দশমিক *a, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    দশমিক mean = mean_array(a, n);
    জন্য(i = 0; i < n; ++i) যোগ += (a[i] - mean)*(a[i]-mean);
    দশমিক variance = যোগ/n;
    ফেরত variance;
}

পূর্ণসংখ্যা constrain_int(পূর্ণসংখ্যা a, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max)
{
    যদি (a < min) ফেরত min;
    যদি (a > max) ফেরত max;
    ফেরত a;
}

দশমিক constrain(দশমিক min, দশমিক max, দশমিক a)
{
    যদি (a < min) ফেরত min;
    যদি (a > max) ফেরত max;
    ফেরত a;
}

দশমিক dist_array(দশমিক *a, দশমিক *b, পূর্ণসংখ্যা n, পূর্ণসংখ্যা sub)
{
    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    জন্য(i = 0; i < n; i += sub) যোগ += pow(a[i]-b[i], 2);
    ফেরত sqrt(যোগ);
}

দশমিক mse_array(দশমিক *a, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    জন্য(i = 0; i < n; ++i) যোগ += a[i]*a[i];
    ফেরত sqrt(যোগ/n);
}

শূন্য normalize_array(দশমিক *a, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    দশমিক mu = mean_array(a,n);
    দশমিক sigma = sqrt(variance_array(a,n));
    জন্য(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

শূন্য translate_array(দশমিক *a, পূর্ণসংখ্যা n, দশমিক s)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        a[i] += s;
    }
}

দশমিক mag_array(দশমিক *a, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    জন্য(i = 0; i < n; ++i){
        যোগ += a[i]*a[i];   
    }
    ফেরত sqrt(যোগ);
}

শূন্য scale_array(দশমিক *a, পূর্ণসংখ্যা n, দশমিক s)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

পূর্ণসংখ্যা sample_array(দশমিক *a, পূর্ণসংখ্যা n)
{
    দশমিক যোগ = যোগ_array(a, n);
    scale_array(a, n, 1./যোগ);
    দশমিক r = rand_uniform(0, 1);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        r = r - a[i];
        যদি (r <= 0) ফেরত i;
    }
    ফেরত n-1;
}

পূর্ণসংখ্যা max_int_সূচক(পূর্ণসংখ্যা *a, পূর্ণসংখ্যা n)
{
    যদি(n <= 0) ফেরত -1;
    পূর্ণসংখ্যা i, max_i = 0;
    পূর্ণসংখ্যা max = a[0];
    জন্য(i = 1; i < n; ++i){
        যদি(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ফেরত max_i;
}

পূর্ণসংখ্যা max_সূচক(দশমিক *a, পূর্ণসংখ্যা n)
{
    যদি(n <= 0) ফেরত -1;
    পূর্ণসংখ্যা i, max_i = 0;
    দশমিক max = a[0];
    জন্য(i = 1; i < n; ++i){
        যদি(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ফেরত max_i;
}

পূর্ণসংখ্যা int_সূচক(পূর্ণসংখ্যা *a, পূর্ণসংখ্যা val, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        যদি(a[i] == val) ফেরত i;
    }
    ফেরত -1;
}

পূর্ণসংখ্যা rand_int(পূর্ণসংখ্যা min, পূর্ণসংখ্যা max)
{
    যদি (max < min){
        পূর্ণসংখ্যা s = min;
        min = max;
        max = s;
    }
    পূর্ণসংখ্যা r = (rand()%(max - min + 1)) + min;
    ফেরত r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
দশমিক rand_normal()
{
    স্থির পূর্ণসংখ্যা haveSpare = 0;
    স্থির দ্বিগুণ rand1, rand2;

    যদি(haveSpare)
    {
        haveSpare = 0;
        ফেরত sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((দ্বিগুণ) RAND_MAX);
    যদি(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((দ্বিগুণ) RAND_MAX)) * TWO_PI;

    ফেরত sqrt(rand1) * cos(rand2);
}

/*
   float rand_normal()
   {
   int n = 12;
   int i;
   float sum= 0;
   for(i = 0; i < n; ++i) sum += (float)rand()/RAND_MAX;
   return sum-n/2.;
   }
 */

size_t rand_আকার_t()
{
    ফেরত  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

দশমিক rand_uniform(দশমিক min, দশমিক max)
{
    যদি(max < min){
        দশমিক swap = min;
        min = max;
        max = swap;
    }
    ফেরত ((দশমিক)rand()/RAND_MAX * (max - min)) + min;
}

দশমিক rand_scale(দশমিক s)
{
    দশমিক scale = rand_uniform(1, s);
    যদি(rand()%2) ফেরত scale;
    ফেরত 1./scale;
}

দশমিক **one_hot_encode(দশমিক *a, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k)
{
    পূর্ণসংখ্যা i;
    দশমিক **t = calloc(n, আকার(দশমিক*));
    জন্য(i = 0; i < n; ++i){
        t[i] = calloc(k, আকার(দশমিক));
        পূর্ণসংখ্যা সূচক = (পূর্ণসংখ্যা)a[i];
        t[i][সূচক] = 1;
    }
    ফেরত t;
}

