#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>

#include "utils.h"


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

dviguu_na what_time_is_it_now()
{
    sa_nracanaa timeval time;
    නම් (gettimeofday(&time,NULL)){
        ආපසු 0;
    }
    ආපසු (dviguu_na)time.tv_sec + (dviguu_na)time.tv_usec * .000001;
}

පූර්ණ_සංඛ්‍යා *read_intlist(var_na *gpu_list, පූර්ණ_සංඛ්‍යා *ngpus, පූර්ණ_සංඛ්‍යා d)
{
    පූර්ණ_සංඛ්‍යා *gpus = 0;
    නම්(gpu_list){
        පූර්ණ_සංඛ්‍යා len = strlen(gpu_list);
        *ngpus = 1;
        පූර්ණ_සංඛ්‍යා i;
        සඳහා(i = 0; i < len; ++i){
            නම් (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, parimaa_na(පූර්ණ_සංඛ්‍යා));
        සඳහා(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } නැතහොත් {
        gpus = calloc(1, parimaa_na(float));
        *gpus = d;
        *ngpus = 1;
    }
    ආපසු gpus;
}

පූර්ණ_සංඛ්‍යා *read_map(var_na *filename)
{
    පූර්ණ_සංඛ්‍යා n = 0;
    පූර්ණ_සංඛ්‍යා *map = 0;
    var_na *str;
    FILE *file = fopen(filename, "r");
    නම්(!file) file_error(filename);
    අතරතුර((str=fgetl(file))){
        ++n;
        map = realloc(map, n*parimaa_na(පූර්ණ_සංඛ්‍යා));
        map[n-1] = atoi(str);
    }
    ආපසු map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    සඳහා(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t num = end-start;
        shuffle(arr+(start*size), num, size);
    }
}

void shuffle(void *arr, size_t n, size_t size)
{
    size_t i;
    void *swp = calloc(1, size);
    සඳහා(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

පූර්ණ_සංඛ්‍යා *random_index_order(පූර්ණ_සංඛ්‍යා min, පූර්ණ_සංඛ්‍යා max)
{
    පූර්ණ_සංඛ්‍යා *inds = calloc(max-min, parimaa_na(පූර්ණ_සංඛ්‍යා));
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = min; i < max; ++i){
        inds[i] = i;
    }
    සඳහා(i = min; i < max-1; ++i){
        පූර්ණ_සංඛ්‍යා swap = inds[i];
        පූර්ණ_සංඛ්‍යා index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ආපසු inds;
}

void del_arg(පූර්ණ_සංඛ්‍යා argc, var_na **argv, පූර්ණ_සංඛ්‍යා index)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

පූර්ණ_සංඛ්‍යා find_arg(පූර්ණ_සංඛ්‍යා argc, var_na* argv[], var_na *arg)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < argc; ++i) {
        නම්(!argv[i]) continue;
        නම්(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ආපසු 1;
        }
    }
    ආපසු 0;
}

පූර්ණ_සංඛ්‍යා find_int_arg(පූර්ණ_සංඛ්‍යා argc, var_na **argv, var_na *arg, පූර්ණ_සංඛ්‍යා def)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < argc-1; ++i){
        නම්(!argv[i]) continue;
        නම්(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ආපසු def;
}

float find_float_arg(පූර්ණ_සංඛ්‍යා argc, var_na **argv, var_na *arg, float def)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < argc-1; ++i){
        නම්(!argv[i]) continue;
        නම්(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ආපසු def;
}

var_na *find_char_arg(පූර්ණ_සංඛ්‍යා argc, var_na **argv, var_na *arg, var_na *def)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < argc-1; ++i){
        නම්(!argv[i]) continue;
        නම්(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ආපසු def;
}


var_na *basecfg(var_na *cfgfile)
{
    var_na *c = cfgfile;
    var_na *next;
    අතරතුර((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    නම් (next) *next = 0;
    ආපසු c;
}

පූර්ණ_සංඛ්‍යා alphanum_to_int(var_na c)
{
    ආපසු (c < 58) ? c - 48 : c-87;
}
var_na int_to_alphanum(පූර්ණ_සංඛ්‍යා i)
{
    නම් (i == 36) ආපසු '.';
    ආපසු (i < 10) ? i + 48 : i + 87;
}

void pm(පූර්ණ_සංඛ්‍යා M, පූර්ණ_සංඛ්‍යා N, float *A)
{
    පූර්ණ_සංඛ්‍යා i,j;
    සඳහා(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        සඳහා(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

void find_replace(var_na *str, var_na *orig, var_na *rep, var_na *output)
{
    var_na buffer[4096] = {0};
    var_na *p;

    sprintf(buffer, "%s", str);
    නම්(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ආපසු;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    ආපසු (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා k, පූර්ණ_සංඛ්‍යා *index)
{
    පූර්ණ_සංඛ්‍යා i,j;
    සඳහා(j = 0; j < k; ++j) index[j] = -1;
    සඳහා(i = 0; i < n; ++i){
        පූර්ණ_සංඛ්‍යා curr = i;
        සඳහා(j = 0; j < k; ++j){
            නම්((index[j] < 0) || a[curr] > a[index[j]]){
                පූර්ණ_සංඛ්‍යා swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

void error(const var_na *s)
{
    perror(s);
    assert(0);
    exit(-1);
}

unsigned var_na *read_file(var_na *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned var_na *text = calloc(size+1, parimaa_na(var_na));
    fread(text, 1, size, fp);
    fclose(fp);
    ආපසු text;
}

void malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

void file_error(var_na *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(var_na *s, var_na delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_insert(l, s);
    සඳහා(i = 0; i < len; ++i){
        නම්(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    ආපසු l;
}

void strip(var_na *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    සඳහා(i = 0; i < len; ++i){
        var_na c = s[i];
        නම්(c==' '||c=='\t'||c=='\n') ++offset;
        නැතහොත් s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(var_na *s, var_na bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    සඳහා(i = 0; i < len; ++i){
        var_na c = s[i];
        නම්(c==bad) ++offset;
        නැතහොත් s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

var_na *fgetl(FILE *fp)
{
    නම්(feof(fp)) ආපසු 0;
    size_t size = 512;
    var_na *line = malloc(size*parimaa_na(var_na));
    නම්(!fgets(line, size, fp)){
        free(line);
        ආපසු 0;
    }

    size_t curr = strlen(line);

    අතරතුර((line[curr-1] != '\n') && !feof(fp)){
        නම්(curr == size-1){
            size *= 2;
            line = realloc(line, size*parimaa_na(var_na));
            නම්(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        නම්(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    නම්(line[curr-1] == '\n') line[curr-1] = '\0';

    ආපසු line;
}

පූර්ණ_සංඛ්‍යා read_int(පූර්ණ_සංඛ්‍යා fd)
{
    පූර්ණ_සංඛ්‍යා n = 0;
    පූර්ණ_සංඛ්‍යා next = read(fd, &n, parimaa_na(පූර්ණ_සංඛ්‍යා));
    නම්(next <= 0) ආපසු -1;
    ආපසු n;
}

void write_int(පූර්ණ_සංඛ්‍යා fd, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා next = write(fd, &n, parimaa_na(පූර්ණ_සංඛ්‍යා));
    නම්(next <= 0) error("read failed");
}

පූර්ණ_සංඛ්‍යා read_all_fail(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    අතරතුර(n < bytes){
        පූර්ණ_සංඛ්‍යා next = read(fd, buffer + n, bytes-n);
        නම්(next <= 0) ආපසු 1;
        n += next;
    }
    ආපසු 0;
}

පූර්ණ_සංඛ්‍යා write_all_fail(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    අතරතුර(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        නම්(next <= 0) ආපසු 1;
        n += next;
    }
    ආපසු 0;
}

void read_all(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    අතරතුර(n < bytes){
        පූර්ණ_සංඛ්‍යා next = read(fd, buffer + n, bytes-n);
        නම්(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(පූර්ණ_සංඛ්‍යා fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    අතරතුර(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        නම්(next <= 0) error("write failed");
        n += next;
    }
}


var_na *copy_string(var_na *s)
{
    var_na *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ආපසු copy;
}

list *parse_csv_line(var_na *line)
{
    list *l = make_list();
    var_na *c, *p;
    පූර්ණ_සංඛ්‍යා in = 0;
    සඳහා(c = line, p = line; *c != '\0'; ++c){
        නම්(*c == '"') in = !in;
        නැතහොත් නම්(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    ආපසු l;
}

පූර්ණ_සංඛ්‍යා count_fields(var_na *line)
{
    පූර්ණ_සංඛ්‍යා count = 0;
    පූර්ණ_සංඛ්‍යා done = 0;
    var_na *c;
    සඳහා(c = line; !done; ++c){
        done = (*c == '\0');
        නම්(*c == ',' || done) ++count;
    }
    ආපසු count;
}

float *parse_fields(var_na *line, පූර්ණ_සංඛ්‍යා n)
{
    float *field = calloc(n, parimaa_na(float));
    var_na *c, *p, *end;
    පූර්ණ_සංඛ්‍යා count = 0;
    පූර්ණ_සංඛ්‍යා done = 0;
    සඳහා(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        නම්(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            නම්(p == c) field[count] = nan("");
            නම්(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    ආපසු field;
}

float sum_array(float *a, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    සඳහා(i = 0; i < n; ++i) sum += a[i];
    ආපසු sum;
}

float mean_array(float *a, පූර්ණ_සංඛ්‍යා n)
{
    ආපසු sum_array(a,n)/n;
}

void mean_arrays(float **a, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා els, float *avg)
{
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා j;
    memset(avg, 0, els*parimaa_na(float));
    සඳහා(j = 0; j < n; ++j){
        සඳහා(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    සඳහා(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, පූර්ණ_සංඛ්‍යා n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    float mean = mean_array(a, n);
    සඳහා(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    ආපසු variance;
}

පූර්ණ_සංඛ්‍යා constrain_int(පූර්ණ_සංඛ්‍යා a, පූර්ණ_සංඛ්‍යා min, පූර්ණ_සංඛ්‍යා max)
{
    නම් (a < min) ආපසු min;
    නම් (a > max) ආපසු max;
    ආපසු a;
}

float constrain(float min, float max, float a)
{
    නම් (a < min) ආපසු min;
    නම් (a > max) ආපසු max;
    ආපසු a;
}

float dist_array(float *a, float *b, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා sub)
{
    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    සඳහා(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ආපසු sqrt(sum);
}

float mse_array(float *a, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    සඳහා(i = 0; i < n; ++i) sum += a[i]*a[i];
    ආපසු sqrt(sum/n);
}

void normalize_array(float *a, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    සඳහා(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, පූර්ණ_සංඛ්‍යා n, float s)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    සඳහා(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ආපසු sqrt(sum);
}

void scale_array(float *a, පූර්ණ_සංඛ්‍යා n, float s)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

පූර්ණ_සංඛ්‍යා sample_array(float *a, පූර්ණ_සංඛ්‍යා n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        r = r - a[i];
        නම් (r <= 0) ආපසු i;
    }
    ආපසු n-1;
}

පූර්ණ_සංඛ්‍යා max_int_index(පූර්ණ_සංඛ්‍යා *a, පූර්ණ_සංඛ්‍යා n)
{
    නම්(n <= 0) ආපසු -1;
    පූර්ණ_සංඛ්‍යා i, max_i = 0;
    පූර්ණ_සංඛ්‍යා max = a[0];
    සඳහා(i = 1; i < n; ++i){
        නම්(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ආපසු max_i;
}

පූර්ණ_සංඛ්‍යා max_index(float *a, පූර්ණ_සංඛ්‍යා n)
{
    නම්(n <= 0) ආපසු -1;
    පූර්ණ_සංඛ්‍යා i, max_i = 0;
    float max = a[0];
    සඳහා(i = 1; i < n; ++i){
        නම්(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ආපසු max_i;
}

පූර්ණ_සංඛ්‍යා int_index(පූර්ණ_සංඛ්‍යා *a, පූර්ණ_සංඛ්‍යා val, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        නම්(a[i] == val) ආපසු i;
    }
    ආපසු -1;
}

පූර්ණ_සංඛ්‍යා rand_int(පූර්ණ_සංඛ්‍යා min, පූර්ණ_සංඛ්‍යා max)
{
    නම් (max < min){
        පූර්ණ_සංඛ්‍යා s = min;
        min = max;
        max = s;
    }
    පූර්ණ_සංඛ්‍යා r = (rand()%(max - min + 1)) + min;
    ආපසු r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static පූර්ණ_සංඛ්‍යා haveSpare = 0;
    static dviguu_na rand1, rand2;

    නම්(haveSpare)
    {
        haveSpare = 0;
        ආපසු sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((dviguu_na) RAND_MAX);
    නම්(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((dviguu_na) RAND_MAX)) * TWO_PI;

    ආපසු sqrt(rand1) * cos(rand2);
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
    ආපසු  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

float rand_uniform(float min, float max)
{
    නම්(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    ආපසු ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    නම්(rand()%2) ආපසු scale;
    ආපසු 1./scale;
}

float **one_hot_encode(float *a, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා k)
{
    පූර්ණ_සංඛ්‍යා i;
    float **t = calloc(n, parimaa_na(float*));
    සඳහා(i = 0; i < n; ++i){
        t[i] = calloc(k, parimaa_na(float));
        පූර්ණ_සංඛ්‍යා index = (පූර්ණ_සංඛ්‍යා)a[i];
        t[i][index] = 1;
    }
    ආපසු t;
}

