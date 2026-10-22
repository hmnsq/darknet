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
    ប្រសិនបើ (gettimeofday(&time,NULL)){
        ត្រឡប់ 0;
    }
    ត្រឡប់ (dviguu_na)time.tv_sec + (dviguu_na)time.tv_usec * .000001;
}

ចំនួនគត់ *read_intlist(var_na *gpu_list, ចំនួនគត់ *ngpus, ចំនួនគត់ d)
{
    ចំនួនគត់ *gpus = 0;
    ប្រសិនបើ(gpu_list){
        ចំនួនគត់ len = strlen(gpu_list);
        *ngpus = 1;
        ចំនួនគត់ i;
        សម្រាប់(i = 0; i < len; ++i){
            ប្រសិនបើ (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, parimaa_na(ចំនួនគត់));
        សម្រាប់(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } បើមិនដូច្នេះ {
        gpus = calloc(1, parimaa_na(float));
        *gpus = d;
        *ngpus = 1;
    }
    ត្រឡប់ gpus;
}

ចំនួនគត់ *read_map(var_na *filename)
{
    ចំនួនគត់ n = 0;
    ចំនួនគត់ *map = 0;
    var_na *str;
    FILE *file = fopen(filename, "r");
    ប្រសិនបើ(!file) file_error(filename);
    ខណៈពេល((str=fgetl(file))){
        ++n;
        map = realloc(map, n*parimaa_na(ចំនួនគត់));
        map[n-1] = atoi(str);
    }
    ត្រឡប់ map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    សម្រាប់(i = 0; i < sections; ++i){
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
    សម្រាប់(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

ចំនួនគត់ *random_index_order(ចំនួនគត់ min, ចំនួនគត់ max)
{
    ចំនួនគត់ *inds = calloc(max-min, parimaa_na(ចំនួនគត់));
    ចំនួនគត់ i;
    សម្រាប់(i = min; i < max; ++i){
        inds[i] = i;
    }
    សម្រាប់(i = min; i < max-1; ++i){
        ចំនួនគត់ swap = inds[i];
        ចំនួនគត់ index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ត្រឡប់ inds;
}

void del_arg(ចំនួនគត់ argc, var_na **argv, ចំនួនគត់ index)
{
    ចំនួនគត់ i;
    សម្រាប់(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ចំនួនគត់ find_arg(ចំនួនគត់ argc, var_na* argv[], var_na *arg)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < argc; ++i) {
        ប្រសិនបើ(!argv[i]) continue;
        ប្រសិនបើ(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ត្រឡប់ 1;
        }
    }
    ត្រឡប់ 0;
}

ចំនួនគត់ find_int_arg(ចំនួនគត់ argc, var_na **argv, var_na *arg, ចំនួនគត់ def)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < argc-1; ++i){
        ប្រសិនបើ(!argv[i]) continue;
        ប្រសិនបើ(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ត្រឡប់ def;
}

float find_float_arg(ចំនួនគត់ argc, var_na **argv, var_na *arg, float def)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < argc-1; ++i){
        ប្រសិនបើ(!argv[i]) continue;
        ប្រសិនបើ(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ត្រឡប់ def;
}

var_na *find_char_arg(ចំនួនគត់ argc, var_na **argv, var_na *arg, var_na *def)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < argc-1; ++i){
        ប្រសិនបើ(!argv[i]) continue;
        ប្រសិនបើ(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ត្រឡប់ def;
}


var_na *basecfg(var_na *cfgfile)
{
    var_na *c = cfgfile;
    var_na *next;
    ខណៈពេល((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    ប្រសិនបើ (next) *next = 0;
    ត្រឡប់ c;
}

ចំនួនគត់ alphanum_to_int(var_na c)
{
    ត្រឡប់ (c < 58) ? c - 48 : c-87;
}
var_na int_to_alphanum(ចំនួនគត់ i)
{
    ប្រសិនបើ (i == 36) ត្រឡប់ '.';
    ត្រឡប់ (i < 10) ? i + 48 : i + 87;
}

void pm(ចំនួនគត់ M, ចំនួនគត់ N, float *A)
{
    ចំនួនគត់ i,j;
    សម្រាប់(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        សម្រាប់(j = 0; j < N; ++j){
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
    ប្រសិនបើ(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ត្រឡប់;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    ត្រឡប់ (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, ចំនួនគត់ n, ចំនួនគត់ k, ចំនួនគត់ *index)
{
    ចំនួនគត់ i,j;
    សម្រាប់(j = 0; j < k; ++j) index[j] = -1;
    សម្រាប់(i = 0; i < n; ++i){
        ចំនួនគត់ curr = i;
        សម្រាប់(j = 0; j < k; ++j){
            ប្រសិនបើ((index[j] < 0) || a[curr] > a[index[j]]){
                ចំនួនគត់ swap = curr;
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
    ត្រឡប់ text;
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
    សម្រាប់(i = 0; i < len; ++i){
        ប្រសិនបើ(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    ត្រឡប់ l;
}

void strip(var_na *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    សម្រាប់(i = 0; i < len; ++i){
        var_na c = s[i];
        ប្រសិនបើ(c==' '||c=='\t'||c=='\n') ++offset;
        បើមិនដូច្នេះ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(var_na *s, var_na bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    សម្រាប់(i = 0; i < len; ++i){
        var_na c = s[i];
        ប្រសិនបើ(c==bad) ++offset;
        បើមិនដូច្នេះ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

var_na *fgetl(FILE *fp)
{
    ប្រសិនបើ(feof(fp)) ត្រឡប់ 0;
    size_t size = 512;
    var_na *line = malloc(size*parimaa_na(var_na));
    ប្រសិនបើ(!fgets(line, size, fp)){
        free(line);
        ត្រឡប់ 0;
    }

    size_t curr = strlen(line);

    ខណៈពេល((line[curr-1] != '\n') && !feof(fp)){
        ប្រសិនបើ(curr == size-1){
            size *= 2;
            line = realloc(line, size*parimaa_na(var_na));
            ប្រសិនបើ(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        ប្រសិនបើ(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    ប្រសិនបើ(line[curr-1] == '\n') line[curr-1] = '\0';

    ត្រឡប់ line;
}

ចំនួនគត់ read_int(ចំនួនគត់ fd)
{
    ចំនួនគត់ n = 0;
    ចំនួនគត់ next = read(fd, &n, parimaa_na(ចំនួនគត់));
    ប្រសិនបើ(next <= 0) ត្រឡប់ -1;
    ត្រឡប់ n;
}

void write_int(ចំនួនគត់ fd, ចំនួនគត់ n)
{
    ចំនួនគត់ next = write(fd, &n, parimaa_na(ចំនួនគត់));
    ប្រសិនបើ(next <= 0) error("read failed");
}

ចំនួនគត់ read_all_fail(ចំនួនគត់ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ខណៈពេល(n < bytes){
        ចំនួនគត់ next = read(fd, buffer + n, bytes-n);
        ប្រសិនបើ(next <= 0) ត្រឡប់ 1;
        n += next;
    }
    ត្រឡប់ 0;
}

ចំនួនគត់ write_all_fail(ចំនួនគត់ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ខណៈពេល(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ប្រសិនបើ(next <= 0) ត្រឡប់ 1;
        n += next;
    }
    ត្រឡប់ 0;
}

void read_all(ចំនួនគត់ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ខណៈពេល(n < bytes){
        ចំនួនគត់ next = read(fd, buffer + n, bytes-n);
        ប្រសិនបើ(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(ចំនួនគត់ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ខណៈពេល(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ប្រសិនបើ(next <= 0) error("write failed");
        n += next;
    }
}


var_na *copy_string(var_na *s)
{
    var_na *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ត្រឡប់ copy;
}

list *parse_csv_line(var_na *line)
{
    list *l = make_list();
    var_na *c, *p;
    ចំនួនគត់ in = 0;
    សម្រាប់(c = line, p = line; *c != '\0'; ++c){
        ប្រសិនបើ(*c == '"') in = !in;
        បើមិនដូច្នេះ ប្រសិនបើ(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    ត្រឡប់ l;
}

ចំនួនគត់ count_fields(var_na *line)
{
    ចំនួនគត់ count = 0;
    ចំនួនគត់ done = 0;
    var_na *c;
    សម្រាប់(c = line; !done; ++c){
        done = (*c == '\0');
        ប្រសិនបើ(*c == ',' || done) ++count;
    }
    ត្រឡប់ count;
}

float *parse_fields(var_na *line, ចំនួនគត់ n)
{
    float *field = calloc(n, parimaa_na(float));
    var_na *c, *p, *end;
    ចំនួនគត់ count = 0;
    ចំនួនគត់ done = 0;
    សម្រាប់(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        ប្រសិនបើ(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            ប្រសិនបើ(p == c) field[count] = nan("");
            ប្រសិនបើ(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    ត្រឡប់ field;
}

float sum_array(float *a, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    float sum = 0;
    សម្រាប់(i = 0; i < n; ++i) sum += a[i];
    ត្រឡប់ sum;
}

float mean_array(float *a, ចំនួនគត់ n)
{
    ត្រឡប់ sum_array(a,n)/n;
}

void mean_arrays(float **a, ចំនួនគត់ n, ចំនួនគត់ els, float *avg)
{
    ចំនួនគត់ i;
    ចំនួនគត់ j;
    memset(avg, 0, els*parimaa_na(float));
    សម្រាប់(j = 0; j < n; ++j){
        សម្រាប់(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    សម្រាប់(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, ចំនួនគត់ n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    float sum = 0;
    float mean = mean_array(a, n);
    សម្រាប់(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    ត្រឡប់ variance;
}

ចំនួនគត់ constrain_int(ចំនួនគត់ a, ចំនួនគត់ min, ចំនួនគត់ max)
{
    ប្រសិនបើ (a < min) ត្រឡប់ min;
    ប្រសិនបើ (a > max) ត្រឡប់ max;
    ត្រឡប់ a;
}

float constrain(float min, float max, float a)
{
    ប្រសិនបើ (a < min) ត្រឡប់ min;
    ប្រសិនបើ (a > max) ត្រឡប់ max;
    ត្រឡប់ a;
}

float dist_array(float *a, float *b, ចំនួនគត់ n, ចំនួនគត់ sub)
{
    ចំនួនគត់ i;
    float sum = 0;
    សម្រាប់(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ត្រឡប់ sqrt(sum);
}

float mse_array(float *a, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    float sum = 0;
    សម្រាប់(i = 0; i < n; ++i) sum += a[i]*a[i];
    ត្រឡប់ sqrt(sum/n);
}

void normalize_array(float *a, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    សម្រាប់(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, ចំនួនគត់ n, float s)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    float sum = 0;
    សម្រាប់(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ត្រឡប់ sqrt(sum);
}

void scale_array(float *a, ចំនួនគត់ n, float s)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ចំនួនគត់ sample_array(float *a, ចំនួនគត់ n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        r = r - a[i];
        ប្រសិនបើ (r <= 0) ត្រឡប់ i;
    }
    ត្រឡប់ n-1;
}

ចំនួនគត់ max_int_index(ចំនួនគត់ *a, ចំនួនគត់ n)
{
    ប្រសិនបើ(n <= 0) ត្រឡប់ -1;
    ចំនួនគត់ i, max_i = 0;
    ចំនួនគត់ max = a[0];
    សម្រាប់(i = 1; i < n; ++i){
        ប្រសិនបើ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ត្រឡប់ max_i;
}

ចំនួនគត់ max_index(float *a, ចំនួនគត់ n)
{
    ប្រសិនបើ(n <= 0) ត្រឡប់ -1;
    ចំនួនគត់ i, max_i = 0;
    float max = a[0];
    សម្រាប់(i = 1; i < n; ++i){
        ប្រសិនបើ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ត្រឡប់ max_i;
}

ចំនួនគត់ int_index(ចំនួនគត់ *a, ចំនួនគត់ val, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        ប្រសិនបើ(a[i] == val) ត្រឡប់ i;
    }
    ត្រឡប់ -1;
}

ចំនួនគត់ rand_int(ចំនួនគត់ min, ចំនួនគត់ max)
{
    ប្រសិនបើ (max < min){
        ចំនួនគត់ s = min;
        min = max;
        max = s;
    }
    ចំនួនគត់ r = (rand()%(max - min + 1)) + min;
    ត្រឡប់ r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static ចំនួនគត់ haveSpare = 0;
    static dviguu_na rand1, rand2;

    ប្រសិនបើ(haveSpare)
    {
        haveSpare = 0;
        ត្រឡប់ sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((dviguu_na) RAND_MAX);
    ប្រសិនបើ(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((dviguu_na) RAND_MAX)) * TWO_PI;

    ត្រឡប់ sqrt(rand1) * cos(rand2);
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
    ត្រឡប់  ((size_t)(rand()&0xff) << 56) | 
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
    ប្រសិនបើ(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    ត្រឡប់ ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    ប្រសិនបើ(rand()%2) ត្រឡប់ scale;
    ត្រឡប់ 1./scale;
}

float **one_hot_encode(float *a, ចំនួនគត់ n, ចំនួនគត់ k)
{
    ចំនួនគត់ i;
    float **t = calloc(n, parimaa_na(float*));
    សម្រាប់(i = 0; i < n; ++i){
        t[i] = calloc(k, parimaa_na(float));
        ចំនួនគត់ index = (ចំនួនគត់)a[i];
        t[i][index] = 1;
    }
    ត្រឡប់ t;
}

