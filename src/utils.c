#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/time.h>

#ଅନ୍ତର୍ଭାଵ "utils.h"


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

ଦ୍ୱିଗୁଣ what_time_is_it_now()
{
    ସଂରଚନା timeval time;
    ଯଦି (gettimeofday(&time,ଖାଲି)){
        ଫେରନ୍ତୁ 0;
    }
    ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ)time.tv_sec + (ଦ୍ୱିଗୁଣ)time.tv_usec * .000001;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା *read_intlist(ଅକ୍ଷର *gpu_list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ngpus, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *gpus = 0;
    ଯଦି(gpu_list){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(gpu_list);
        *ngpus = 1;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < len; ++i){
            ଯଦି (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        ପାଇଁ(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } ଅନ୍ୟଥା {
        gpus = calloc(1, ଆକାର(ଦଶମିକ));
        *gpus = d;
        *ngpus = 1;
    }
    ଫେରନ୍ତୁ gpus;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା *read_map(ଅକ୍ଷର *filename)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *map = 0;
    ଅକ୍ଷର *str;
    FILE *file = fopen(filename, "r");
    ଯଦି(!file) file_error(filename);
    ଯେତେବେଳେ((str=fgetl(file))){
        ++n;
        map = realloc(map, n*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        map[n-1] = atoi(str);
    }
    ଫେରନ୍ତୁ map;
}

ଶୂନ୍ୟ sorta_shuffle(ଶୂନ୍ୟ *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    ପାଇଁ(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t ଅଙ୍କ = end-start;
        shuffle(arr+(start*size), ଅଙ୍କ, size);
    }
}

ଶୂନ୍ୟ shuffle(ଶୂନ୍ୟ *arr, size_t n, size_t size)
{
    size_t i;
    ଶୂନ୍ୟ *swp = calloc(1, size);
    ପାଇଁ(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା *random_index_order(ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *inds = calloc(max-min, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = min; i < max; ++i){
        inds[i] = i;
    }
    ପାଇଁ(i = min; i < max-1; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା swap = inds[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ଫେରନ୍ତୁ inds;
}

ଶୂନ୍ୟ del_arg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_arg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର* argv[], ଅକ୍ଷର *arg)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < argc; ++i) {
        ଯଦି(!argv[i]) ଜାରି;
        ଯଦି(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ଫେରନ୍ତୁ 1;
        }
    }
    ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_int_arg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା def)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < argc-1; ++i){
        ଯଦି(!argv[i]) ଜାରି;
        ଯଦି(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ଭାଙ୍ଗ;
        }
    }
    ଫେରନ୍ତୁ def;
}

ଦଶମିକ find_float_arg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, ଅକ୍ଷର *arg, ଦଶମିକ def)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < argc-1; ++i){
        ଯଦି(!argv[i]) ଜାରି;
        ଯଦି(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ଭାଙ୍ଗ;
        }
    }
    ଫେରନ୍ତୁ def;
}

ଅକ୍ଷର *find_char_arg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, ଅକ୍ଷର *arg, ଅକ୍ଷର *def)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < argc-1; ++i){
        ଯଦି(!argv[i]) ଜାରି;
        ଯଦି(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ଭାଙ୍ଗ;
        }
    }
    ଫେରନ୍ତୁ def;
}


ଅକ୍ଷର *basecfg(ଅକ୍ଷର *cfgfile)
{
    ଅକ୍ଷର *c = cfgfile;
    ଅକ୍ଷର *next;
    ଯେତେବେଳେ((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    ଯଦି (next) *next = 0;
    ଫେରନ୍ତୁ c;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା alphanum_to_int(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ (c < 58) ? c - 48 : c-87;
}
ଅକ୍ଷର int_to_alphanum(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
    ଯଦି (i == 36) ଫେରନ୍ତୁ '.';
    ଫେରନ୍ତୁ (i < 10) ? i + 48 : i + 87;
}

ଶୂନ୍ୟ pm(ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *A)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        ପାଇଁ(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

ଶୂନ୍ୟ find_replace(ଅକ୍ଷର *str, ଅକ୍ଷର *orig, ଅକ୍ଷର *rep, ଅକ୍ଷର *output)
{
    ଅକ୍ଷର buffer[4096] = {0};
    ଅକ୍ଷର *p;

    sprintf(buffer, "%s", str);
    ଯଦି(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ଫେରନ୍ତୁ;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

ଦଶମିକ sec(clock_t clocks)
{
    ଫେରନ୍ତୁ (ଦଶମିକ)clocks/CLOCKS_PER_SEC;
}

ଶୂନ୍ୟ ଶୀର୍ଷ_k(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(j = 0; j < k; ++j) index[j] = -1;
    ପାଇଁ(i = 0; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା curr = i;
        ପାଇଁ(j = 0; j < k; ++j){
            ଯଦି((index[j] < 0) || a[curr] > a[index[j]]){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

ଶୂନ୍ୟ error(ଅଚଳ ଅକ୍ଷର *s)
{
    perror(s);
    ପ୍ରତିଜ୍ଞା(0);
    exit(-1);
}

unsigned ଅକ୍ଷର *read_file(ଅକ୍ଷର *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned ଅକ୍ଷର *text = calloc(size+1, ଆକାର(ଅକ୍ଷର));
    fread(text, 1, size, fp);
    fclose(fp);
    ଫେରନ୍ତୁ text;
}

ଶୂନ୍ୟ malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

ଶୂନ୍ୟ file_error(ଅକ୍ଷର *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(ଅକ୍ଷର *s, ଅକ୍ଷର delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_ଭର୍ତ୍ତି(l, s);
    ପାଇଁ(i = 0; i < len; ++i){
        ଯଦି(s[i] == delim){
            s[i] = '\0';
            list_ଭର୍ତ୍ତି(l, &(s[i+1]));
        }
    }
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ strip(ଅକ୍ଷର *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ପାଇଁ(i = 0; i < len; ++i){
        ଅକ୍ଷର c = s[i];
        ଯଦି(c==' '||c=='\t'||c=='\n') ++offset;
        ଅନ୍ୟଥା s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

ଶୂନ୍ୟ strip_char(ଅକ୍ଷର *s, ଅକ୍ଷର bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ପାଇଁ(i = 0; i < len; ++i){
        ଅକ୍ଷର c = s[i];
        ଯଦି(c==bad) ++offset;
        ଅନ୍ୟଥା s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

ଶୂନ୍ୟ free_ptrs(ଶୂନ୍ୟ **ptrs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i) ମୁକ୍ତ(ptrs[i]);
    ମୁକ୍ତ(ptrs);
}

ଅକ୍ଷର *fgetl(FILE *fp)
{
    ଯଦି(feof(fp)) ଫେରନ୍ତୁ 0;
    size_t size = 512;
    ଅକ୍ଷର *ଧାଡ଼ି = malloc(size*ଆକାର(ଅକ୍ଷର));
    ଯଦି(!fgets(ଧାଡ଼ି, size, fp)){
        ମୁକ୍ତ(ଧାଡ଼ି);
        ଫେରନ୍ତୁ 0;
    }

    size_t curr = strlen(ଧାଡ଼ି);

    ଯେତେବେଳେ((ଧାଡ଼ି[curr-1] != '\n') && !feof(fp)){
        ଯଦି(curr == size-1){
            size *= 2;
            ଧାଡ଼ି = realloc(ଧାଡ଼ି, size*ଆକାର(ଅକ୍ଷର));
            ଯଦି(!ଧାଡ଼ି) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        ଯଦି(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&ଧାଡ଼ି[curr], readsize, fp);
        curr = strlen(ଧାଡ଼ି);
    }
    ଯଦି(ଧାଡ଼ି[curr-1] == '\n') ଧାଡ଼ି[curr-1] = '\0';

    ଫେରନ୍ତୁ ଧାଡ଼ି;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = ପଠ(fd, &n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ଯଦି(next <= 0) ଫେରନ୍ତୁ -1;
    ଫେରନ୍ତୁ n;
}

ଶୂନ୍ୟ write_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = ଲିଖ(fd, &n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ଯଦି(next <= 0) error("read failed");
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_all_fail(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes)
{
    size_t n = 0;
    ଯେତେବେଳେ(n < bytes){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = ପଠ(fd, buffer + n, bytes-n);
        ଯଦି(next <= 0) ଫେରନ୍ତୁ 1;
        n += next;
    }
    ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_all_fail(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes)
{
    size_t n = 0;
    ଯେତେବେଳେ(n < bytes){
        size_t next = ଲିଖ(fd, buffer + n, bytes-n);
        ଯଦି(next <= 0) ଫେରନ୍ତୁ 1;
        n += next;
    }
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ read_all(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes)
{
    size_t n = 0;
    ଯେତେବେଳେ(n < bytes){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = ପଠ(fd, buffer + n, bytes-n);
        ଯଦି(next <= 0) error("read failed");
        n += next;
    }
}

ଶୂନ୍ୟ write_all(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buffer, size_t bytes)
{
    size_t n = 0;
    ଯେତେବେଳେ(n < bytes){
        size_t next = ଲିଖ(fd, buffer + n, bytes-n);
        ଯଦି(next <= 0) error("write failed");
        n += next;
    }
}


ଅକ୍ଷର *copy_string(ଅକ୍ଷର *s)
{
    ଅକ୍ଷର *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ଫେରନ୍ତୁ copy;
}

list *parse_csv_ଧାଡ଼ି(ଅକ୍ଷର *ଧାଡ଼ି)
{
    list *l = make_list();
    ଅକ୍ଷର *c, *p;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା in = 0;
    ପାଇଁ(c = ଧାଡ଼ି, p = ଧାଡ଼ି; *c != '\0'; ++c){
        ଯଦି(*c == '"') in = !in;
        ଅନ୍ୟଥା ଯଦି(*c == ',' && !in){
            *c = '\0';
            list_ଭର୍ତ୍ତି(l, copy_string(p));
            p = c+1;
        }
    }
    list_ଭର୍ତ୍ତି(l, copy_string(p));
    ଫେରନ୍ତୁ l;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_fields(ଅକ୍ଷର *ଧାଡ଼ି)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା done = 0;
    ଅକ୍ଷର *c;
    ପାଇଁ(c = ଧାଡ଼ି; !done; ++c){
        done = (*c == '\0');
        ଯଦି(*c == ',' || done) ++ଗଣନା_2;
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
}

ଦଶମିକ *parse_fields(ଅକ୍ଷର *ଧାଡ଼ି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଦଶମିକ *field = calloc(n, ଆକାର(ଦଶମିକ));
    ଅକ୍ଷର *c, *p, *end;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା done = 0;
    ପାଇଁ(c = ଧାଡ଼ି, p = ଧାଡ଼ି; !done; ++c){
        done = (*c == '\0');
        ଯଦି(*c == ',' || done){
            *c = '\0';
            field[ଗଣନା_2] = strtod(p, &end);
            ଯଦି(p == c) field[ଗଣନା_2] = nan("");
            ଯଦି(end != c && (end != c-1 || *end != '\r')) field[ଗଣନା_2] = nan(""); //DOS file formats!
            p = c+1;
            ++ଗଣନା_2;
        }
    }
    ଫେରନ୍ତୁ field;
}

ଦଶମିକ sum_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ପାଇଁ(i = 0; i < n; ++i) sum += a[i];
    ଫେରନ୍ତୁ sum;
}

ଦଶମିକ mean_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଫେରନ୍ତୁ sum_array(a,n)/n;
}

ଶୂନ୍ୟ mean_arrays(ଦଶମିକ **a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା els, ଦଶମିକ *avg)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    memset(avg, 0, els*ଆକାର(ଦଶମିକ));
    ପାଇଁ(j = 0; j < n; ++j){
        ପାଇଁ(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    ପାଇଁ(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

ଶୂନ୍ୟ print_statistics(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଦଶମିକ m = mean_array(a, n);
    ଦଶମିକ v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

ଦଶମିକ variance_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ଦଶମିକ mean = mean_array(a, n);
    ପାଇଁ(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    ଦଶମିକ variance = sum/n;
    ଫେରନ୍ତୁ variance;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା constrain_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ଯଦି (a < min) ଫେରନ୍ତୁ min;
    ଯଦି (a > max) ଫେରନ୍ତୁ max;
    ଫେରନ୍ତୁ a;
}

ଦଶମିକ constrain(ଦଶମିକ min, ଦଶମିକ max, ଦଶମିକ a)
{
    ଯଦି (a < min) ଫେରନ୍ତୁ min;
    ଯଦି (a > max) ଫେରନ୍ତୁ max;
    ଫେରନ୍ତୁ a;
}

ଦଶମିକ dist_array(ଦଶମିକ *a, ଦଶମିକ *b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sub)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ପାଇଁ(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ଫେରନ୍ତୁ sqrt(sum);
}

ଦଶମିକ mse_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ପାଇଁ(i = 0; i < n; ++i) sum += a[i]*a[i];
    ଫେରନ୍ତୁ sqrt(sum/n);
}

ଶୂନ୍ୟ normalize_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ mu = mean_array(a,n);
    ଦଶମିକ sigma = sqrt(variance_array(a,n));
    ପାଇଁ(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

ଶୂନ୍ୟ translate_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        a[i] += s;
    }
}

ଦଶମିକ mag_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ପାଇଁ(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ଫେରନ୍ତୁ sqrt(sum);
}

ଶୂନ୍ୟ scale_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sample_array(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଦଶମିକ sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    ଦଶମିକ r = rand_uniform(0, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        r = r - a[i];
        ଯଦି (r <= 0) ଫେରନ୍ତୁ i;
    }
    ଫେରନ୍ତୁ n-1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_int_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଯଦି(n <= 0) ଫେରନ୍ତୁ -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, max_i = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା max = a[0];
    ପାଇଁ(i = 1; i < n; ++i){
        ଯଦି(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ଫେରନ୍ତୁ max_i;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_index(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଯଦି(n <= 0) ଫେରନ୍ତୁ -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, max_i = 0;
    ଦଶମିକ max = a[0];
    ପାଇଁ(i = 1; i < n; ++i){
        ଯଦି(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ଫେରନ୍ତୁ max_i;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା int_index(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା val, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଯଦି(a[i] == val) ଫେରନ୍ତୁ i;
    }
    ଫେରନ୍ତୁ -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା rand_int(ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ଯଦି (max < min){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = min;
        min = max;
        max = s;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = (rand()%(max - min + 1)) + min;
    ଫେରନ୍ତୁ r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
ଦଶମିକ rand_normal()
{
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା haveSpare = 0;
    ସ୍ଥିର ଦ୍ୱିଗୁଣ rand1, rand2;

    ଯଦି(haveSpare)
    {
        haveSpare = 0;
        ଫେରନ୍ତୁ sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((ଦ୍ୱିଗୁଣ) RAND_MAX);
    ଯଦି(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((ଦ୍ୱିଗୁଣ) RAND_MAX)) * TWO_PI;

    ଫେରନ୍ତୁ sqrt(rand1) * cos(rand2);
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
    ଫେରନ୍ତୁ  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

ଦଶମିକ rand_uniform(ଦଶମିକ min, ଦଶମିକ max)
{
    ଯଦି(max < min){
        ଦଶମିକ swap = min;
        min = max;
        max = swap;
    }
    ଫେରନ୍ତୁ ((ଦଶମିକ)rand()/RAND_MAX * (max - min)) + min;
}

ଦଶମିକ rand_scale(ଦଶମିକ s)
{
    ଦଶମିକ scale = rand_uniform(1, s);
    ଯଦି(rand()%2) ଫେରନ୍ତୁ scale;
    ଫେରନ୍ତୁ 1./scale;
}

ଦଶମିକ **one_hot_encode(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ **t = calloc(n, ଆକାର(ଦଶମିକ*));
    ପାଇଁ(i = 0; i < n; ++i){
        t[i] = calloc(k, ଆକାର(ଦଶମିକ));
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)a[i];
        t[i][index] = 1;
    }
    ଫେରନ୍ତୁ t;
}

