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
    nếu (gettimeofday(&time,NULL)){
        trả_về 0;
    }
    trả_về (double)time.tv_sec + (double)time.tv_usec * .000001;
}

số_nguyên *read_intlist(char *gpu_list, số_nguyên *ngpus, số_nguyên d)
{
    số_nguyên *gpus = 0;
    nếu(gpu_list){
        số_nguyên len = strlen(gpu_list);
        *ngpus = 1;
        số_nguyên i;
        cho(i = 0; i < len; ++i){
            nếu (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, sizeof(số_nguyên));
        cho(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } không_thì {
        gpus = calloc(1, sizeof(float));
        *gpus = d;
        *ngpus = 1;
    }
    trả_về gpus;
}

số_nguyên *read_map(char *filename)
{
    số_nguyên n = 0;
    số_nguyên *map = 0;
    char *str;
    FILE *file = fopen(filename, "r");
    nếu(!file) file_error(filename);
    trong_khi((str=fgetl(file))){
        ++n;
        map = realloc(map, n*sizeof(số_nguyên));
        map[n-1] = atoi(str);
    }
    trả_về map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    cho(i = 0; i < sections; ++i){
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
    cho(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

số_nguyên *random_index_order(số_nguyên min, số_nguyên max)
{
    số_nguyên *inds = calloc(max-min, sizeof(số_nguyên));
    số_nguyên i;
    cho(i = min; i < max; ++i){
        inds[i] = i;
    }
    cho(i = min; i < max-1; ++i){
        số_nguyên swap = inds[i];
        số_nguyên index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    trả_về inds;
}

void del_arg(số_nguyên argc, char **argv, số_nguyên index)
{
    số_nguyên i;
    cho(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

số_nguyên find_arg(số_nguyên argc, char* argv[], char *arg)
{
    số_nguyên i;
    cho(i = 0; i < argc; ++i) {
        nếu(!argv[i]) continue;
        nếu(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            trả_về 1;
        }
    }
    trả_về 0;
}

số_nguyên find_int_arg(số_nguyên argc, char **argv, char *arg, số_nguyên def)
{
    số_nguyên i;
    cho(i = 0; i < argc-1; ++i){
        nếu(!argv[i]) continue;
        nếu(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    trả_về def;
}

float find_float_arg(số_nguyên argc, char **argv, char *arg, float def)
{
    số_nguyên i;
    cho(i = 0; i < argc-1; ++i){
        nếu(!argv[i]) continue;
        nếu(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    trả_về def;
}

char *find_char_arg(số_nguyên argc, char **argv, char *arg, char *def)
{
    số_nguyên i;
    cho(i = 0; i < argc-1; ++i){
        nếu(!argv[i]) continue;
        nếu(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    trả_về def;
}


char *basecfg(char *cfgfile)
{
    char *c = cfgfile;
    char *next;
    trong_khi((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    nếu (next) *next = 0;
    trả_về c;
}

số_nguyên alphanum_to_int(char c)
{
    trả_về (c < 58) ? c - 48 : c-87;
}
char int_to_alphanum(số_nguyên i)
{
    nếu (i == 36) trả_về '.';
    trả_về (i < 10) ? i + 48 : i + 87;
}

void pm(số_nguyên M, số_nguyên N, float *A)
{
    số_nguyên i,j;
    cho(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        cho(j = 0; j < N; ++j){
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
    nếu(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        trả_về;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    trả_về (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, số_nguyên n, số_nguyên k, số_nguyên *index)
{
    số_nguyên i,j;
    cho(j = 0; j < k; ++j) index[j] = -1;
    cho(i = 0; i < n; ++i){
        số_nguyên curr = i;
        cho(j = 0; j < k; ++j){
            nếu((index[j] < 0) || a[curr] > a[index[j]]){
                số_nguyên swap = curr;
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
    trả_về text;
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
    cho(i = 0; i < len; ++i){
        nếu(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    trả_về l;
}

void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    cho(i = 0; i < len; ++i){
        char c = s[i];
        nếu(c==' '||c=='\t'||c=='\n') ++offset;
        không_thì s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    cho(i = 0; i < len; ++i){
        char c = s[i];
        nếu(c==bad) ++offset;
        không_thì s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, số_nguyên n)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

char *fgetl(FILE *fp)
{
    nếu(feof(fp)) trả_về 0;
    size_t size = 512;
    char *line = malloc(size*sizeof(char));
    nếu(!fgets(line, size, fp)){
        free(line);
        trả_về 0;
    }

    size_t curr = strlen(line);

    trong_khi((line[curr-1] != '\n') && !feof(fp)){
        nếu(curr == size-1){
            size *= 2;
            line = realloc(line, size*sizeof(char));
            nếu(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        nếu(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    nếu(line[curr-1] == '\n') line[curr-1] = '\0';

    trả_về line;
}

số_nguyên read_int(số_nguyên fd)
{
    số_nguyên n = 0;
    số_nguyên next = read(fd, &n, sizeof(số_nguyên));
    nếu(next <= 0) trả_về -1;
    trả_về n;
}

void write_int(số_nguyên fd, số_nguyên n)
{
    số_nguyên next = write(fd, &n, sizeof(số_nguyên));
    nếu(next <= 0) error("read failed");
}

số_nguyên read_all_fail(số_nguyên fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    trong_khi(n < bytes){
        số_nguyên next = read(fd, buffer + n, bytes-n);
        nếu(next <= 0) trả_về 1;
        n += next;
    }
    trả_về 0;
}

số_nguyên write_all_fail(số_nguyên fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    trong_khi(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        nếu(next <= 0) trả_về 1;
        n += next;
    }
    trả_về 0;
}

void read_all(số_nguyên fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    trong_khi(n < bytes){
        số_nguyên next = read(fd, buffer + n, bytes-n);
        nếu(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(số_nguyên fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    trong_khi(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        nếu(next <= 0) error("write failed");
        n += next;
    }
}


char *copy_string(char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    trả_về copy;
}

list *parse_csv_line(char *line)
{
    list *l = make_list();
    char *c, *p;
    số_nguyên in = 0;
    cho(c = line, p = line; *c != '\0'; ++c){
        nếu(*c == '"') in = !in;
        không_thì nếu(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    trả_về l;
}

số_nguyên count_fields(char *line)
{
    số_nguyên count = 0;
    số_nguyên done = 0;
    char *c;
    cho(c = line; !done; ++c){
        done = (*c == '\0');
        nếu(*c == ',' || done) ++count;
    }
    trả_về count;
}

float *parse_fields(char *line, số_nguyên n)
{
    float *field = calloc(n, sizeof(float));
    char *c, *p, *end;
    số_nguyên count = 0;
    số_nguyên done = 0;
    cho(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        nếu(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            nếu(p == c) field[count] = nan("");
            nếu(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    trả_về field;
}

float sum_array(float *a, số_nguyên n)
{
    số_nguyên i;
    float sum = 0;
    cho(i = 0; i < n; ++i) sum += a[i];
    trả_về sum;
}

float mean_array(float *a, số_nguyên n)
{
    trả_về sum_array(a,n)/n;
}

void mean_arrays(float **a, số_nguyên n, số_nguyên els, float *avg)
{
    số_nguyên i;
    số_nguyên j;
    memset(avg, 0, els*sizeof(float));
    cho(j = 0; j < n; ++j){
        cho(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    cho(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, số_nguyên n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, số_nguyên n)
{
    số_nguyên i;
    float sum = 0;
    float mean = mean_array(a, n);
    cho(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    trả_về variance;
}

số_nguyên constrain_int(số_nguyên a, số_nguyên min, số_nguyên max)
{
    nếu (a < min) trả_về min;
    nếu (a > max) trả_về max;
    trả_về a;
}

float constrain(float min, float max, float a)
{
    nếu (a < min) trả_về min;
    nếu (a > max) trả_về max;
    trả_về a;
}

float dist_array(float *a, float *b, số_nguyên n, số_nguyên sub)
{
    số_nguyên i;
    float sum = 0;
    cho(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    trả_về sqrt(sum);
}

float mse_array(float *a, số_nguyên n)
{
    số_nguyên i;
    float sum = 0;
    cho(i = 0; i < n; ++i) sum += a[i]*a[i];
    trả_về sqrt(sum/n);
}

void normalize_array(float *a, số_nguyên n)
{
    số_nguyên i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    cho(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, số_nguyên n, float s)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, số_nguyên n)
{
    số_nguyên i;
    float sum = 0;
    cho(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    trả_về sqrt(sum);
}

void scale_array(float *a, số_nguyên n, float s)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

số_nguyên sample_array(float *a, số_nguyên n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        r = r - a[i];
        nếu (r <= 0) trả_về i;
    }
    trả_về n-1;
}

số_nguyên max_int_index(số_nguyên *a, số_nguyên n)
{
    nếu(n <= 0) trả_về -1;
    số_nguyên i, max_i = 0;
    số_nguyên max = a[0];
    cho(i = 1; i < n; ++i){
        nếu(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    trả_về max_i;
}

số_nguyên max_index(float *a, số_nguyên n)
{
    nếu(n <= 0) trả_về -1;
    số_nguyên i, max_i = 0;
    float max = a[0];
    cho(i = 1; i < n; ++i){
        nếu(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    trả_về max_i;
}

số_nguyên int_index(số_nguyên *a, số_nguyên val, số_nguyên n)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        nếu(a[i] == val) trả_về i;
    }
    trả_về -1;
}

số_nguyên rand_int(số_nguyên min, số_nguyên max)
{
    nếu (max < min){
        số_nguyên s = min;
        min = max;
        max = s;
    }
    số_nguyên r = (rand()%(max - min + 1)) + min;
    trả_về r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static số_nguyên haveSpare = 0;
    static double rand1, rand2;

    nếu(haveSpare)
    {
        haveSpare = 0;
        trả_về sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((double) RAND_MAX);
    nếu(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

    trả_về sqrt(rand1) * cos(rand2);
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
    trả_về  ((size_t)(rand()&0xff) << 56) | 
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
    nếu(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    trả_về ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    nếu(rand()%2) trả_về scale;
    trả_về 1./scale;
}

float **one_hot_encode(float *a, số_nguyên n, số_nguyên k)
{
    số_nguyên i;
    float **t = calloc(n, sizeof(float*));
    cho(i = 0; i < n; ++i){
        t[i] = calloc(k, sizeof(float));
        số_nguyên index = (số_nguyên)a[i];
        t[i][index] = 1;
    }
    trả_về t;
}

