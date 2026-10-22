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
    གལ་ཏེ (gettimeofday(&time,NULL)){
        ཕྱིར 0;
    }
    ཕྱིར (dviguu_na)time.tv_sec + (dviguu_na)time.tv_usec * .000001;
}

ཧྲིལ *read_intlist(var_na *gpu_list, ཧྲིལ *ngpus, ཧྲིལ d)
{
    ཧྲིལ *gpus = 0;
    གལ་ཏེ(gpu_list){
        ཧྲིལ len = strlen(gpu_list);
        *ngpus = 1;
        ཧྲིལ i;
        ཆེད་དུ(i = 0; i < len; ++i){
            གལ་ཏེ (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, parimaa_na(ཧྲིལ));
        ཆེད་དུ(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } དེ་མིན {
        gpus = calloc(1, parimaa_na(float));
        *gpus = d;
        *ngpus = 1;
    }
    ཕྱིར gpus;
}

ཧྲིལ *read_map(var_na *filename)
{
    ཧྲིལ n = 0;
    ཧྲིལ *map = 0;
    var_na *str;
    FILE *file = fopen(filename, "r");
    གལ་ཏེ(!file) file_error(filename);
    སྐབས((str=fgetl(file))){
        ++n;
        map = realloc(map, n*parimaa_na(ཧྲིལ));
        map[n-1] = atoi(str);
    }
    ཕྱིར map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    ཆེད་དུ(i = 0; i < sections; ++i){
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
    ཆེད་དུ(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

ཧྲིལ *random_index_order(ཧྲིལ min, ཧྲིལ max)
{
    ཧྲིལ *inds = calloc(max-min, parimaa_na(ཧྲིལ));
    ཧྲིལ i;
    ཆེད་དུ(i = min; i < max; ++i){
        inds[i] = i;
    }
    ཆེད་དུ(i = min; i < max-1; ++i){
        ཧྲིལ swap = inds[i];
        ཧྲིལ index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ཕྱིར inds;
}

void del_arg(ཧྲིལ argc, var_na **argv, ཧྲིལ index)
{
    ཧྲིལ i;
    ཆེད་དུ(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ཧྲིལ find_arg(ཧྲིལ argc, var_na* argv[], var_na *arg)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < argc; ++i) {
        གལ་ཏེ(!argv[i]) continue;
        གལ་ཏེ(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ཕྱིར 1;
        }
    }
    ཕྱིར 0;
}

ཧྲིལ find_int_arg(ཧྲིལ argc, var_na **argv, var_na *arg, ཧྲིལ def)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < argc-1; ++i){
        གལ་ཏེ(!argv[i]) continue;
        གལ་ཏེ(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ཕྱིར def;
}

float find_float_arg(ཧྲིལ argc, var_na **argv, var_na *arg, float def)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < argc-1; ++i){
        གལ་ཏེ(!argv[i]) continue;
        གལ་ཏེ(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ཕྱིར def;
}

var_na *find_char_arg(ཧྲིལ argc, var_na **argv, var_na *arg, var_na *def)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < argc-1; ++i){
        གལ་ཏེ(!argv[i]) continue;
        གལ་ཏེ(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ཕྱིར def;
}


var_na *basecfg(var_na *cfgfile)
{
    var_na *c = cfgfile;
    var_na *next;
    སྐབས((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    གལ་ཏེ (next) *next = 0;
    ཕྱིར c;
}

ཧྲིལ alphanum_to_int(var_na c)
{
    ཕྱིར (c < 58) ? c - 48 : c-87;
}
var_na int_to_alphanum(ཧྲིལ i)
{
    གལ་ཏེ (i == 36) ཕྱིར '.';
    ཕྱིར (i < 10) ? i + 48 : i + 87;
}

void pm(ཧྲིལ M, ཧྲིལ N, float *A)
{
    ཧྲིལ i,j;
    ཆེད་དུ(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        ཆེད་དུ(j = 0; j < N; ++j){
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
    གལ་ཏེ(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ཕྱིར;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    ཕྱིར (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, ཧྲིལ n, ཧྲིལ k, ཧྲིལ *index)
{
    ཧྲིལ i,j;
    ཆེད་དུ(j = 0; j < k; ++j) index[j] = -1;
    ཆེད་དུ(i = 0; i < n; ++i){
        ཧྲིལ curr = i;
        ཆེད་དུ(j = 0; j < k; ++j){
            གལ་ཏེ((index[j] < 0) || a[curr] > a[index[j]]){
                ཧྲིལ swap = curr;
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
    ཕྱིར text;
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
    ཆེད་དུ(i = 0; i < len; ++i){
        གལ་ཏེ(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    ཕྱིར l;
}

void strip(var_na *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ཆེད་དུ(i = 0; i < len; ++i){
        var_na c = s[i];
        གལ་ཏེ(c==' '||c=='\t'||c=='\n') ++offset;
        དེ་མིན s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(var_na *s, var_na bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ཆེད་དུ(i = 0; i < len; ++i){
        var_na c = s[i];
        གལ་ཏེ(c==bad) ++offset;
        དེ་མིན s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, ཧྲིལ n)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

var_na *fgetl(FILE *fp)
{
    གལ་ཏེ(feof(fp)) ཕྱིར 0;
    size_t size = 512;
    var_na *line = malloc(size*parimaa_na(var_na));
    གལ་ཏེ(!fgets(line, size, fp)){
        free(line);
        ཕྱིར 0;
    }

    size_t curr = strlen(line);

    སྐབས((line[curr-1] != '\n') && !feof(fp)){
        གལ་ཏེ(curr == size-1){
            size *= 2;
            line = realloc(line, size*parimaa_na(var_na));
            གལ་ཏེ(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        གལ་ཏེ(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    གལ་ཏེ(line[curr-1] == '\n') line[curr-1] = '\0';

    ཕྱིར line;
}

ཧྲིལ read_int(ཧྲིལ fd)
{
    ཧྲིལ n = 0;
    ཧྲིལ next = read(fd, &n, parimaa_na(ཧྲིལ));
    གལ་ཏེ(next <= 0) ཕྱིར -1;
    ཕྱིར n;
}

void write_int(ཧྲིལ fd, ཧྲིལ n)
{
    ཧྲིལ next = write(fd, &n, parimaa_na(ཧྲིལ));
    གལ་ཏེ(next <= 0) error("read failed");
}

ཧྲིལ read_all_fail(ཧྲིལ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    སྐབས(n < bytes){
        ཧྲིལ next = read(fd, buffer + n, bytes-n);
        གལ་ཏེ(next <= 0) ཕྱིར 1;
        n += next;
    }
    ཕྱིར 0;
}

ཧྲིལ write_all_fail(ཧྲིལ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    སྐབས(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        གལ་ཏེ(next <= 0) ཕྱིར 1;
        n += next;
    }
    ཕྱིར 0;
}

void read_all(ཧྲིལ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    སྐབས(n < bytes){
        ཧྲིལ next = read(fd, buffer + n, bytes-n);
        གལ་ཏེ(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(ཧྲིལ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    སྐབས(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        གལ་ཏེ(next <= 0) error("write failed");
        n += next;
    }
}


var_na *copy_string(var_na *s)
{
    var_na *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ཕྱིར copy;
}

list *parse_csv_line(var_na *line)
{
    list *l = make_list();
    var_na *c, *p;
    ཧྲིལ in = 0;
    ཆེད་དུ(c = line, p = line; *c != '\0'; ++c){
        གལ་ཏེ(*c == '"') in = !in;
        དེ་མིན གལ་ཏེ(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    ཕྱིར l;
}

ཧྲིལ count_fields(var_na *line)
{
    ཧྲིལ count = 0;
    ཧྲིལ done = 0;
    var_na *c;
    ཆེད་དུ(c = line; !done; ++c){
        done = (*c == '\0');
        གལ་ཏེ(*c == ',' || done) ++count;
    }
    ཕྱིར count;
}

float *parse_fields(var_na *line, ཧྲིལ n)
{
    float *field = calloc(n, parimaa_na(float));
    var_na *c, *p, *end;
    ཧྲིལ count = 0;
    ཧྲིལ done = 0;
    ཆེད་དུ(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        གལ་ཏེ(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            གལ་ཏེ(p == c) field[count] = nan("");
            གལ་ཏེ(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    ཕྱིར field;
}

float sum_array(float *a, ཧྲིལ n)
{
    ཧྲིལ i;
    float sum = 0;
    ཆེད་དུ(i = 0; i < n; ++i) sum += a[i];
    ཕྱིར sum;
}

float mean_array(float *a, ཧྲིལ n)
{
    ཕྱིར sum_array(a,n)/n;
}

void mean_arrays(float **a, ཧྲིལ n, ཧྲིལ els, float *avg)
{
    ཧྲིལ i;
    ཧྲིལ j;
    memset(avg, 0, els*parimaa_na(float));
    ཆེད་དུ(j = 0; j < n; ++j){
        ཆེད་དུ(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    ཆེད་དུ(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, ཧྲིལ n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, ཧྲིལ n)
{
    ཧྲིལ i;
    float sum = 0;
    float mean = mean_array(a, n);
    ཆེད་དུ(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    ཕྱིར variance;
}

ཧྲིལ constrain_int(ཧྲིལ a, ཧྲིལ min, ཧྲིལ max)
{
    གལ་ཏེ (a < min) ཕྱིར min;
    གལ་ཏེ (a > max) ཕྱིར max;
    ཕྱིར a;
}

float constrain(float min, float max, float a)
{
    གལ་ཏེ (a < min) ཕྱིར min;
    གལ་ཏེ (a > max) ཕྱིར max;
    ཕྱིར a;
}

float dist_array(float *a, float *b, ཧྲིལ n, ཧྲིལ sub)
{
    ཧྲིལ i;
    float sum = 0;
    ཆེད་དུ(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ཕྱིར sqrt(sum);
}

float mse_array(float *a, ཧྲིལ n)
{
    ཧྲིལ i;
    float sum = 0;
    ཆེད་དུ(i = 0; i < n; ++i) sum += a[i]*a[i];
    ཕྱིར sqrt(sum/n);
}

void normalize_array(float *a, ཧྲིལ n)
{
    ཧྲིལ i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    ཆེད་དུ(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, ཧྲིལ n, float s)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, ཧྲིལ n)
{
    ཧྲིལ i;
    float sum = 0;
    ཆེད་དུ(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ཕྱིར sqrt(sum);
}

void scale_array(float *a, ཧྲིལ n, float s)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ཧྲིལ sample_array(float *a, ཧྲིལ n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        r = r - a[i];
        གལ་ཏེ (r <= 0) ཕྱིར i;
    }
    ཕྱིར n-1;
}

ཧྲིལ max_int_index(ཧྲིལ *a, ཧྲིལ n)
{
    གལ་ཏེ(n <= 0) ཕྱིར -1;
    ཧྲིལ i, max_i = 0;
    ཧྲིལ max = a[0];
    ཆེད་དུ(i = 1; i < n; ++i){
        གལ་ཏེ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ཕྱིར max_i;
}

ཧྲིལ max_index(float *a, ཧྲིལ n)
{
    གལ་ཏེ(n <= 0) ཕྱིར -1;
    ཧྲིལ i, max_i = 0;
    float max = a[0];
    ཆེད་དུ(i = 1; i < n; ++i){
        གལ་ཏེ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ཕྱིར max_i;
}

ཧྲིལ int_index(ཧྲིལ *a, ཧྲིལ val, ཧྲིལ n)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        གལ་ཏེ(a[i] == val) ཕྱིར i;
    }
    ཕྱིར -1;
}

ཧྲིལ rand_int(ཧྲིལ min, ཧྲིལ max)
{
    གལ་ཏེ (max < min){
        ཧྲིལ s = min;
        min = max;
        max = s;
    }
    ཧྲིལ r = (rand()%(max - min + 1)) + min;
    ཕྱིར r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static ཧྲིལ haveSpare = 0;
    static dviguu_na rand1, rand2;

    གལ་ཏེ(haveSpare)
    {
        haveSpare = 0;
        ཕྱིར sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((dviguu_na) RAND_MAX);
    གལ་ཏེ(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((dviguu_na) RAND_MAX)) * TWO_PI;

    ཕྱིར sqrt(rand1) * cos(rand2);
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
    ཕྱིར  ((size_t)(rand()&0xff) << 56) | 
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
    གལ་ཏེ(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    ཕྱིར ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    གལ་ཏེ(rand()%2) ཕྱིར scale;
    ཕྱིར 1./scale;
}

float **one_hot_encode(float *a, ཧྲིལ n, ཧྲིལ k)
{
    ཧྲིལ i;
    float **t = calloc(n, parimaa_na(float*));
    ཆེད་དུ(i = 0; i < n; ++i){
        t[i] = calloc(k, parimaa_na(float));
        ཧྲིལ index = (ཧྲིལ)a[i];
        t[i][index] = 1;
    }
    ཕྱིར t;
}

