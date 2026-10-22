#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <float.h>
#অন্তর্ভাৱ <limits.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <sys/time.h>

#অন্তর্ভাৱ "utils.h"


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

দ্ৱিগুণ what_time_is_it_now()
{
    সংরচনা timeval time;
    যদি (gettimeofday(&time,রিক্ত)){
        ঘূৰাই 0;
    }
    ঘূৰাই (দ্ৱিগুণ)time.tv_sec + (দ্ৱিগুণ)time.tv_usec * .000001;
}

পূর্ণাঙ্ক *read_intlist(ৱর্ণ *gpu_list, পূর্ণাঙ্ক *ngpus, পূর্ণাঙ্ক d)
{
    পূর্ণাঙ্ক *gpus = 0;
    যদি(gpu_list){
        পূর্ণাঙ্ক len = strlen(gpu_list);
        *ngpus = 1;
        পূর্ণাঙ্ক i;
        বাবে(i = 0; i < len; ++i){
            যদি (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, পরিমাণ(পূর্ণাঙ্ক));
        বাবে(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } অন্যথা {
        gpus = calloc(1, পরিমাণ(ভিন্ন));
        *gpus = d;
        *ngpus = 1;
    }
    ঘূৰাই gpus;
}

পূর্ণাঙ্ক *read_map(ৱর্ণ *filename)
{
    পূর্ণাঙ্ক n = 0;
    পূর্ণাঙ্ক *map = 0;
    ৱর্ণ *str;
    FILE *file = fopen(filename, "r");
    যদি(!file) file_error(filename);
    যেতিয়ালৈ((str=fgetl(file))){
        ++n;
        map = realloc(map, n*পরিমাণ(পূর্ণাঙ্ক));
        map[n-1] = atoi(str);
    }
    ঘূৰাই map;
}

শূন্য sorta_shuffle(শূন্য *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    বাবে(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t অঙ্ক = end-start;
        shuffle(arr+(start*size), অঙ্ক, size);
    }
}

শূন্য shuffle(শূন্য *arr, size_t n, size_t size)
{
    size_t i;
    শূন্য *swp = calloc(1, size);
    বাবে(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

পূর্ণাঙ্ক *random_index_order(পূর্ণাঙ্ক min, পূর্ণাঙ্ক max)
{
    পূর্ণাঙ্ক *inds = calloc(max-min, পরিমাণ(পূর্ণাঙ্ক));
    পূর্ণাঙ্ক i;
    বাবে(i = min; i < max; ++i){
        inds[i] = i;
    }
    বাবে(i = min; i < max-1; ++i){
        পূর্ণাঙ্ক swap = inds[i];
        পূর্ণাঙ্ক index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ঘূৰাই inds;
}

শূন্য del_arg(পূর্ণাঙ্ক argc, ৱর্ণ **argv, পূর্ণাঙ্ক index)
{
    পূর্ণাঙ্ক i;
    বাবে(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

পূর্ণাঙ্ক find_arg(পূর্ণাঙ্ক argc, ৱর্ণ* argv[], ৱর্ণ *arg)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < argc; ++i) {
        যদি(!argv[i]) অনুৱর্ত;
        যদি(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ঘূৰাই 1;
        }
    }
    ঘূৰাই 0;
}

পূর্ণাঙ্ক find_int_arg(পূর্ণাঙ্ক argc, ৱর্ণ **argv, ৱর্ণ *arg, পূর্ণাঙ্ক def)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < argc-1; ++i){
        যদি(!argv[i]) অনুৱর্ত;
        যদি(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ভঙ্গ;
        }
    }
    ঘূৰাই def;
}

ভিন্ন find_float_arg(পূর্ণাঙ্ক argc, ৱর্ণ **argv, ৱর্ণ *arg, ভিন্ন def)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < argc-1; ++i){
        যদি(!argv[i]) অনুৱর্ত;
        যদি(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ভঙ্গ;
        }
    }
    ঘূৰাই def;
}

ৱর্ণ *find_char_arg(পূর্ণাঙ্ক argc, ৱর্ণ **argv, ৱর্ণ *arg, ৱর্ণ *def)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < argc-1; ++i){
        যদি(!argv[i]) অনুৱর্ত;
        যদি(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ভঙ্গ;
        }
    }
    ঘূৰাই def;
}


ৱর্ণ *basecfg(ৱর্ণ *cfgfile)
{
    ৱর্ণ *c = cfgfile;
    ৱর্ণ *next;
    যেতিয়ালৈ((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    যদি (next) *next = 0;
    ঘূৰাই c;
}

পূর্ণাঙ্ক alphanum_to_int(ৱর্ণ c)
{
    ঘূৰাই (c < 58) ? c - 48 : c-87;
}
ৱর্ণ int_to_alphanum(পূর্ণাঙ্ক i)
{
    যদি (i == 36) ঘূৰাই '.';
    ঘূৰাই (i < 10) ? i + 48 : i + 87;
}

শূন্য pm(পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, ভিন্ন *A)
{
    পূর্ণাঙ্ক i,j;
    বাবে(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        বাবে(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

শূন্য find_replace(ৱর্ণ *str, ৱর্ণ *orig, ৱর্ণ *rep, ৱর্ণ *output)
{
    ৱর্ণ buffer[4096] = {0};
    ৱর্ণ *p;

    sprintf(buffer, "%s", str);
    যদি(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ঘূৰাই;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

ভিন্ন sec(clock_t clocks)
{
    ঘূৰাই (ভিন্ন)clocks/CLOCKS_PER_SEC;
}

শূন্য শীৰ্ষ_k(ভিন্ন *a, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k, পূর্ণাঙ্ক *index)
{
    পূর্ণাঙ্ক i,j;
    বাবে(j = 0; j < k; ++j) index[j] = -1;
    বাবে(i = 0; i < n; ++i){
        পূর্ণাঙ্ক curr = i;
        বাবে(j = 0; j < k; ++j){
            যদি((index[j] < 0) || a[curr] > a[index[j]]){
                পূর্ণাঙ্ক swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

শূন্য error(ধ্রুৱ ৱর্ণ *s)
{
    perror(s);
    প্রতিজ্ঞা(0);
    exit(-1);
}

unsigned ৱর্ণ *read_file(ৱর্ণ *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned ৱর্ণ *text = calloc(size+1, পরিমাণ(ৱর্ণ));
    fread(text, 1, size, fp);
    fclose(fp);
    ঘূৰাই text;
}

শূন্য malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

শূন্য file_error(ৱর্ণ *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(ৱর্ণ *s, ৱর্ণ delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_ঢোকাও(l, s);
    বাবে(i = 0; i < len; ++i){
        যদি(s[i] == delim){
            s[i] = '\0';
            list_ঢোকাও(l, &(s[i+1]));
        }
    }
    ঘূৰাই l;
}

শূন্য strip(ৱর্ণ *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    বাবে(i = 0; i < len; ++i){
        ৱর্ণ c = s[i];
        যদি(c==' '||c=='\t'||c=='\n') ++offset;
        অন্যথা s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

শূন্য strip_char(ৱর্ণ *s, ৱর্ণ bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    বাবে(i = 0; i < len; ++i){
        ৱর্ণ c = s[i];
        যদি(c==bad) ++offset;
        অন্যথা s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

শূন্য free_ptrs(শূন্য **ptrs, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i) মুক্ত(ptrs[i]);
    মুক্ত(ptrs);
}

ৱর্ণ *fgetl(FILE *fp)
{
    যদি(feof(fp)) ঘূৰাই 0;
    size_t size = 512;
    ৱর্ণ *সাৰি = malloc(size*পরিমাণ(ৱর্ণ));
    যদি(!fgets(সাৰি, size, fp)){
        মুক্ত(সাৰি);
        ঘূৰাই 0;
    }

    size_t curr = strlen(সাৰি);

    যেতিয়ালৈ((সাৰি[curr-1] != '\n') && !feof(fp)){
        যদি(curr == size-1){
            size *= 2;
            সাৰি = realloc(সাৰি, size*পরিমাণ(ৱর্ণ));
            যদি(!সাৰি) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        যদি(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&সাৰি[curr], readsize, fp);
        curr = strlen(সাৰি);
    }
    যদি(সাৰি[curr-1] == '\n') সাৰি[curr-1] = '\0';

    ঘূৰাই সাৰি;
}

পূর্ণাঙ্ক read_int(পূর্ণাঙ্ক fd)
{
    পূর্ণাঙ্ক n = 0;
    পূর্ণাঙ্ক next = পঠ(fd, &n, পরিমাণ(পূর্ণাঙ্ক));
    যদি(next <= 0) ঘূৰাই -1;
    ঘূৰাই n;
}

শূন্য write_int(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক next = লিখ(fd, &n, পরিমাণ(পূর্ণাঙ্ক));
    যদি(next <= 0) error("read failed");
}

পূর্ণাঙ্ক read_all_fail(পূর্ণাঙ্ক fd, ৱর্ণ *buffer, size_t bytes)
{
    size_t n = 0;
    যেতিয়ালৈ(n < bytes){
        পূর্ণাঙ্ক next = পঠ(fd, buffer + n, bytes-n);
        যদি(next <= 0) ঘূৰাই 1;
        n += next;
    }
    ঘূৰাই 0;
}

পূর্ণাঙ্ক write_all_fail(পূর্ণাঙ্ক fd, ৱর্ণ *buffer, size_t bytes)
{
    size_t n = 0;
    যেতিয়ালৈ(n < bytes){
        size_t next = লিখ(fd, buffer + n, bytes-n);
        যদি(next <= 0) ঘূৰাই 1;
        n += next;
    }
    ঘূৰাই 0;
}

শূন্য read_all(পূর্ণাঙ্ক fd, ৱর্ণ *buffer, size_t bytes)
{
    size_t n = 0;
    যেতিয়ালৈ(n < bytes){
        পূর্ণাঙ্ক next = পঠ(fd, buffer + n, bytes-n);
        যদি(next <= 0) error("read failed");
        n += next;
    }
}

শূন্য write_all(পূর্ণাঙ্ক fd, ৱর্ণ *buffer, size_t bytes)
{
    size_t n = 0;
    যেতিয়ালৈ(n < bytes){
        size_t next = লিখ(fd, buffer + n, bytes-n);
        যদি(next <= 0) error("write failed");
        n += next;
    }
}


ৱর্ণ *copy_string(ৱর্ণ *s)
{
    ৱর্ণ *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ঘূৰাই copy;
}

list *parse_csv_সাৰি(ৱর্ণ *সাৰি)
{
    list *l = make_list();
    ৱর্ণ *c, *p;
    পূর্ণাঙ্ক in = 0;
    বাবে(c = সাৰি, p = সাৰি; *c != '\0'; ++c){
        যদি(*c == '"') in = !in;
        অন্যথা যদি(*c == ',' && !in){
            *c = '\0';
            list_ঢোকাও(l, copy_string(p));
            p = c+1;
        }
    }
    list_ঢোকাও(l, copy_string(p));
    ঘূৰাই l;
}

পূর্ণাঙ্ক গুনতি_fields(ৱর্ণ *সাৰি)
{
    পূর্ণাঙ্ক গুনতি = 0;
    পূর্ণাঙ্ক done = 0;
    ৱর্ণ *c;
    বাবে(c = সাৰি; !done; ++c){
        done = (*c == '\0');
        যদি(*c == ',' || done) ++গুনতি;
    }
    ঘূৰাই গুনতি;
}

ভিন্ন *parse_fields(ৱর্ণ *সাৰি, পূর্ণাঙ্ক n)
{
    ভিন্ন *field = calloc(n, পরিমাণ(ভিন্ন));
    ৱর্ণ *c, *p, *end;
    পূর্ণাঙ্ক গুনতি = 0;
    পূর্ণাঙ্ক done = 0;
    বাবে(c = সাৰি, p = সাৰি; !done; ++c){
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
    ঘূৰাই field;
}

ভিন্ন sum_array(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    বাবে(i = 0; i < n; ++i) sum += a[i];
    ঘূৰাই sum;
}

ভিন্ন mean_array(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    ঘূৰাই sum_array(a,n)/n;
}

শূন্য mean_arrays(ভিন্ন **a, পূর্ণাঙ্ক n, পূর্ণাঙ্ক els, ভিন্ন *avg)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক j;
    memset(avg, 0, els*পরিমাণ(ভিন্ন));
    বাবে(j = 0; j < n; ++j){
        বাবে(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    বাবে(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

শূন্য print_statistics(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    ভিন্ন m = mean_array(a, n);
    ভিন্ন v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

ভিন্ন variance_array(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    ভিন্ন mean = mean_array(a, n);
    বাবে(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    ভিন্ন variance = sum/n;
    ঘূৰাই variance;
}

পূর্ণাঙ্ক constrain_int(পূর্ণাঙ্ক a, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max)
{
    যদি (a < min) ঘূৰাই min;
    যদি (a > max) ঘূৰাই max;
    ঘূৰাই a;
}

ভিন্ন constrain(ভিন্ন min, ভিন্ন max, ভিন্ন a)
{
    যদি (a < min) ঘূৰাই min;
    যদি (a > max) ঘূৰাই max;
    ঘূৰাই a;
}

ভিন্ন dist_array(ভিন্ন *a, ভিন্ন *b, পূর্ণাঙ্ক n, পূর্ণাঙ্ক sub)
{
    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    বাবে(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ঘূৰাই sqrt(sum);
}

ভিন্ন mse_array(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    বাবে(i = 0; i < n; ++i) sum += a[i]*a[i];
    ঘূৰাই sqrt(sum/n);
}

শূন্য normalize_array(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    ভিন্ন mu = mean_array(a,n);
    ভিন্ন sigma = sqrt(variance_array(a,n));
    বাবে(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

শূন্য translate_array(ভিন্ন *a, পূর্ণাঙ্ক n, ভিন্ন s)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        a[i] += s;
    }
}

ভিন্ন mag_array(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    বাবে(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ঘূৰাই sqrt(sum);
}

শূন্য scale_array(ভিন্ন *a, পূর্ণাঙ্ক n, ভিন্ন s)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

পূর্ণাঙ্ক sample_array(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    ভিন্ন sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    ভিন্ন r = rand_uniform(0, 1);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        r = r - a[i];
        যদি (r <= 0) ঘূৰাই i;
    }
    ঘূৰাই n-1;
}

পূর্ণাঙ্ক max_int_index(পূর্ণাঙ্ক *a, পূর্ণাঙ্ক n)
{
    যদি(n <= 0) ঘূৰাই -1;
    পূর্ণাঙ্ক i, max_i = 0;
    পূর্ণাঙ্ক max = a[0];
    বাবে(i = 1; i < n; ++i){
        যদি(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ঘূৰাই max_i;
}

পূর্ণাঙ্ক max_index(ভিন্ন *a, পূর্ণাঙ্ক n)
{
    যদি(n <= 0) ঘূৰাই -1;
    পূর্ণাঙ্ক i, max_i = 0;
    ভিন্ন max = a[0];
    বাবে(i = 1; i < n; ++i){
        যদি(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ঘূৰাই max_i;
}

পূর্ণাঙ্ক int_index(পূর্ণাঙ্ক *a, পূর্ণাঙ্ক val, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        যদি(a[i] == val) ঘূৰাই i;
    }
    ঘূৰাই -1;
}

পূর্ণাঙ্ক rand_int(পূর্ণাঙ্ক min, পূর্ণাঙ্ক max)
{
    যদি (max < min){
        পূর্ণাঙ্ক s = min;
        min = max;
        max = s;
    }
    পূর্ণাঙ্ক r = (rand()%(max - min + 1)) + min;
    ঘূৰাই r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
ভিন্ন rand_normal()
{
    স্থির পূর্ণাঙ্ক haveSpare = 0;
    স্থির দ্ৱিগুণ rand1, rand2;

    যদি(haveSpare)
    {
        haveSpare = 0;
        ঘূৰাই sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((দ্ৱিগুণ) RAND_MAX);
    যদি(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((দ্ৱিগুণ) RAND_MAX)) * TWO_PI;

    ঘূৰাই sqrt(rand1) * cos(rand2);
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

size_t rand_size_t()
{
    ঘূৰাই  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

ভিন্ন rand_uniform(ভিন্ন min, ভিন্ন max)
{
    যদি(max < min){
        ভিন্ন swap = min;
        min = max;
        max = swap;
    }
    ঘূৰাই ((ভিন্ন)rand()/RAND_MAX * (max - min)) + min;
}

ভিন্ন rand_scale(ভিন্ন s)
{
    ভিন্ন scale = rand_uniform(1, s);
    যদি(rand()%2) ঘূৰাই scale;
    ঘূৰাই 1./scale;
}

ভিন্ন **one_hot_encode(ভিন্ন *a, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k)
{
    পূর্ণাঙ্ক i;
    ভিন্ন **t = calloc(n, পরিমাণ(ভিন্ন*));
    বাবে(i = 0; i < n; ++i){
        t[i] = calloc(k, পরিমাণ(ভিন্ন));
        পূর্ণাঙ্ক index = (পূর্ণাঙ্ক)a[i];
        t[i][index] = 1;
    }
    ঘূৰাই t;
}

