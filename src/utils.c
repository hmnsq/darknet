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

double what_time_is_it_now()
{
    struct timeval time;
    якщо (gettimeofday(&time,NULL)){
        повернути 0;
    }
    повернути (double)time.tv_sec + (double)time.tv_usec * .000001;
}

ціле *read_intlist(char *gpu_list, ціле *ngpus, ціле d)
{
    ціле *gpus = 0;
    якщо(gpu_list){
        ціле len = strlen(gpu_list);
        *ngpus = 1;
        ціле i;
        для(i = 0; i < len; ++i){
            якщо (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, sizeof(ціле));
        для(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } інакше {
        gpus = calloc(1, sizeof(float));
        *gpus = d;
        *ngpus = 1;
    }
    повернути gpus;
}

ціле *read_map(char *filename)
{
    ціле n = 0;
    ціле *map = 0;
    char *str;
    FILE *file = fopen(filename, "r");
    якщо(!file) file_error(filename);
    поки((str=fgetl(file))){
        ++n;
        map = realloc(map, n*sizeof(ціле));
        map[n-1] = atoi(str);
    }
    повернути map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    для(i = 0; i < sections; ++i){
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
    для(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

ціле *random_index_order(ціле min, ціле max)
{
    ціле *inds = calloc(max-min, sizeof(ціле));
    ціле i;
    для(i = min; i < max; ++i){
        inds[i] = i;
    }
    для(i = min; i < max-1; ++i){
        ціле swap = inds[i];
        ціле index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    повернути inds;
}

void del_arg(ціле argc, char **argv, ціле index)
{
    ціле i;
    для(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ціле find_arg(ціле argc, char* argv[], char *arg)
{
    ціле i;
    для(i = 0; i < argc; ++i) {
        якщо(!argv[i]) continue;
        якщо(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            повернути 1;
        }
    }
    повернути 0;
}

ціле find_int_arg(ціле argc, char **argv, char *arg, ціле def)
{
    ціле i;
    для(i = 0; i < argc-1; ++i){
        якщо(!argv[i]) continue;
        якщо(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    повернути def;
}

float find_float_arg(ціле argc, char **argv, char *arg, float def)
{
    ціле i;
    для(i = 0; i < argc-1; ++i){
        якщо(!argv[i]) continue;
        якщо(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    повернути def;
}

char *find_char_arg(ціле argc, char **argv, char *arg, char *def)
{
    ціле i;
    для(i = 0; i < argc-1; ++i){
        якщо(!argv[i]) continue;
        якщо(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    повернути def;
}


char *basecfg(char *cfgfile)
{
    char *c = cfgfile;
    char *next;
    поки((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    якщо (next) *next = 0;
    повернути c;
}

ціле alphanum_to_int(char c)
{
    повернути (c < 58) ? c - 48 : c-87;
}
char int_to_alphanum(ціле i)
{
    якщо (i == 36) повернути '.';
    повернути (i < 10) ? i + 48 : i + 87;
}

void pm(ціле M, ціле N, float *A)
{
    ціле i,j;
    для(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        для(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

void find_replace(char *str, char *orig, char *rep, char *output)
{
    char buffer[4096] = {0};
    char *p;

    sprintf(buffer, "%s", str);
    якщо(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        повернути;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    повернути (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, ціле n, ціле k, ціле *index)
{
    ціле i,j;
    для(j = 0; j < k; ++j) index[j] = -1;
    для(i = 0; i < n; ++i){
        ціле curr = i;
        для(j = 0; j < k; ++j){
            якщо((index[j] < 0) || a[curr] > a[index[j]]){
                ціле swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

void error(const char *s)
{
    perror(s);
    assert(0);
    exit(-1);
}

unsigned char *read_file(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned char *text = calloc(size+1, sizeof(char));
    fread(text, 1, size, fp);
    fclose(fp);
    повернути text;
}

void malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

void file_error(char *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(char *s, char delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_insert(l, s);
    для(i = 0; i < len; ++i){
        якщо(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    повернути l;
}

void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    для(i = 0; i < len; ++i){
        char c = s[i];
        якщо(c==' '||c=='\t'||c=='\n') ++offset;
        інакше s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    для(i = 0; i < len; ++i){
        char c = s[i];
        якщо(c==bad) ++offset;
        інакше s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, ціле n)
{
    ціле i;
    для(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

char *fgetl(FILE *fp)
{
    якщо(feof(fp)) повернути 0;
    size_t size = 512;
    char *line = malloc(size*sizeof(char));
    якщо(!fgets(line, size, fp)){
        free(line);
        повернути 0;
    }

    size_t curr = strlen(line);

    поки((line[curr-1] != '\n') && !feof(fp)){
        якщо(curr == size-1){
            size *= 2;
            line = realloc(line, size*sizeof(char));
            якщо(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        якщо(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    якщо(line[curr-1] == '\n') line[curr-1] = '\0';

    повернути line;
}

ціле read_int(ціле fd)
{
    ціле n = 0;
    ціле next = read(fd, &n, sizeof(ціле));
    якщо(next <= 0) повернути -1;
    повернути n;
}

void write_int(ціле fd, ціле n)
{
    ціле next = write(fd, &n, sizeof(ціле));
    якщо(next <= 0) error("read failed");
}

ціле read_all_fail(ціле fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    поки(n < bytes){
        ціле next = read(fd, buffer + n, bytes-n);
        якщо(next <= 0) повернути 1;
        n += next;
    }
    повернути 0;
}

ціле write_all_fail(ціле fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    поки(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        якщо(next <= 0) повернути 1;
        n += next;
    }
    повернути 0;
}

void read_all(ціле fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    поки(n < bytes){
        ціле next = read(fd, buffer + n, bytes-n);
        якщо(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(ціле fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    поки(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        якщо(next <= 0) error("write failed");
        n += next;
    }
}


char *copy_string(char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    повернути copy;
}

list *parse_csv_line(char *line)
{
    list *l = make_list();
    char *c, *p;
    ціле in = 0;
    для(c = line, p = line; *c != '\0'; ++c){
        якщо(*c == '"') in = !in;
        інакше якщо(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    повернути l;
}

ціле count_fields(char *line)
{
    ціле count = 0;
    ціле done = 0;
    char *c;
    для(c = line; !done; ++c){
        done = (*c == '\0');
        якщо(*c == ',' || done) ++count;
    }
    повернути count;
}

float *parse_fields(char *line, ціле n)
{
    float *field = calloc(n, sizeof(float));
    char *c, *p, *end;
    ціле count = 0;
    ціле done = 0;
    для(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        якщо(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            якщо(p == c) field[count] = nan("");
            якщо(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    повернути field;
}

float sum_array(float *a, ціле n)
{
    ціле i;
    float sum = 0;
    для(i = 0; i < n; ++i) sum += a[i];
    повернути sum;
}

float mean_array(float *a, ціле n)
{
    повернути sum_array(a,n)/n;
}

void mean_arrays(float **a, ціле n, ціле els, float *avg)
{
    ціле i;
    ціле j;
    memset(avg, 0, els*sizeof(float));
    для(j = 0; j < n; ++j){
        для(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    для(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, ціле n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, ціле n)
{
    ціле i;
    float sum = 0;
    float mean = mean_array(a, n);
    для(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    повернути variance;
}

ціле constrain_int(ціле a, ціле min, ціле max)
{
    якщо (a < min) повернути min;
    якщо (a > max) повернути max;
    повернути a;
}

float constrain(float min, float max, float a)
{
    якщо (a < min) повернути min;
    якщо (a > max) повернути max;
    повернути a;
}

float dist_array(float *a, float *b, ціле n, ціле sub)
{
    ціле i;
    float sum = 0;
    для(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    повернути sqrt(sum);
}

float mse_array(float *a, ціле n)
{
    ціле i;
    float sum = 0;
    для(i = 0; i < n; ++i) sum += a[i]*a[i];
    повернути sqrt(sum/n);
}

void normalize_array(float *a, ціле n)
{
    ціле i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    для(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, ціле n, float s)
{
    ціле i;
    для(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, ціле n)
{
    ціле i;
    float sum = 0;
    для(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    повернути sqrt(sum);
}

void scale_array(float *a, ціле n, float s)
{
    ціле i;
    для(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ціле sample_array(float *a, ціле n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    ціле i;
    для(i = 0; i < n; ++i){
        r = r - a[i];
        якщо (r <= 0) повернути i;
    }
    повернути n-1;
}

ціле max_int_index(ціле *a, ціле n)
{
    якщо(n <= 0) повернути -1;
    ціле i, max_i = 0;
    ціле max = a[0];
    для(i = 1; i < n; ++i){
        якщо(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    повернути max_i;
}

ціле max_index(float *a, ціле n)
{
    якщо(n <= 0) повернути -1;
    ціле i, max_i = 0;
    float max = a[0];
    для(i = 1; i < n; ++i){
        якщо(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    повернути max_i;
}

ціле int_index(ціле *a, ціле val, ціле n)
{
    ціле i;
    для(i = 0; i < n; ++i){
        якщо(a[i] == val) повернути i;
    }
    повернути -1;
}

ціле rand_int(ціле min, ціле max)
{
    якщо (max < min){
        ціле s = min;
        min = max;
        max = s;
    }
    ціле r = (rand()%(max - min + 1)) + min;
    повернути r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static ціле haveSpare = 0;
    static double rand1, rand2;

    якщо(haveSpare)
    {
        haveSpare = 0;
        повернути sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((double) RAND_MAX);
    якщо(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

    повернути sqrt(rand1) * cos(rand2);
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
    повернути  ((size_t)(rand()&0xff) << 56) | 
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
    якщо(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    повернути ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    якщо(rand()%2) повернути scale;
    повернути 1./scale;
}

float **one_hot_encode(float *a, ціле n, ціле k)
{
    ціле i;
    float **t = calloc(n, sizeof(float*));
    для(i = 0; i < n; ++i){
        t[i] = calloc(k, sizeof(float));
        ціле index = (ціле)a[i];
        t[i][index] = 1;
    }
    повернути t;
}

