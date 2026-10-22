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
    если (gettimeofday(&time,NULL)){
        вернуть 0;
    }
    вернуть (double)time.tv_sec + (double)time.tv_usec * .000001;
}

целое *read_intlist(char *gpu_list, целое *ngpus, целое d)
{
    целое *gpus = 0;
    если(gpu_list){
        целое len = strlen(gpu_list);
        *ngpus = 1;
        целое i;
        для(i = 0; i < len; ++i){
            если (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, sizeof(целое));
        для(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } иначе {
        gpus = calloc(1, sizeof(float));
        *gpus = d;
        *ngpus = 1;
    }
    вернуть gpus;
}

целое *read_map(char *filename)
{
    целое n = 0;
    целое *map = 0;
    char *str;
    FILE *file = fopen(filename, "r");
    если(!file) file_error(filename);
    пока((str=fgetl(file))){
        ++n;
        map = realloc(map, n*sizeof(целое));
        map[n-1] = atoi(str);
    }
    вернуть map;
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

целое *random_index_order(целое min, целое max)
{
    целое *inds = calloc(max-min, sizeof(целое));
    целое i;
    для(i = min; i < max; ++i){
        inds[i] = i;
    }
    для(i = min; i < max-1; ++i){
        целое swap = inds[i];
        целое index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    вернуть inds;
}

void del_arg(целое argc, char **argv, целое index)
{
    целое i;
    для(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

целое find_arg(целое argc, char* argv[], char *arg)
{
    целое i;
    для(i = 0; i < argc; ++i) {
        если(!argv[i]) continue;
        если(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            вернуть 1;
        }
    }
    вернуть 0;
}

целое find_int_arg(целое argc, char **argv, char *arg, целое def)
{
    целое i;
    для(i = 0; i < argc-1; ++i){
        если(!argv[i]) continue;
        если(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    вернуть def;
}

float find_float_arg(целое argc, char **argv, char *arg, float def)
{
    целое i;
    для(i = 0; i < argc-1; ++i){
        если(!argv[i]) continue;
        если(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    вернуть def;
}

char *find_char_arg(целое argc, char **argv, char *arg, char *def)
{
    целое i;
    для(i = 0; i < argc-1; ++i){
        если(!argv[i]) continue;
        если(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    вернуть def;
}


char *basecfg(char *cfgfile)
{
    char *c = cfgfile;
    char *next;
    пока((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    если (next) *next = 0;
    вернуть c;
}

целое alphanum_to_int(char c)
{
    вернуть (c < 58) ? c - 48 : c-87;
}
char int_to_alphanum(целое i)
{
    если (i == 36) вернуть '.';
    вернуть (i < 10) ? i + 48 : i + 87;
}

void pm(целое M, целое N, float *A)
{
    целое i,j;
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
    если(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        вернуть;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    вернуть (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, целое n, целое k, целое *index)
{
    целое i,j;
    для(j = 0; j < k; ++j) index[j] = -1;
    для(i = 0; i < n; ++i){
        целое curr = i;
        для(j = 0; j < k; ++j){
            если((index[j] < 0) || a[curr] > a[index[j]]){
                целое swap = curr;
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
    вернуть text;
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
        если(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    вернуть l;
}

void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    для(i = 0; i < len; ++i){
        char c = s[i];
        если(c==' '||c=='\t'||c=='\n') ++offset;
        иначе s[i-offset] = c;
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
        если(c==bad) ++offset;
        иначе s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, целое n)
{
    целое i;
    для(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

char *fgetl(FILE *fp)
{
    если(feof(fp)) вернуть 0;
    size_t size = 512;
    char *line = malloc(size*sizeof(char));
    если(!fgets(line, size, fp)){
        free(line);
        вернуть 0;
    }

    size_t curr = strlen(line);

    пока((line[curr-1] != '\n') && !feof(fp)){
        если(curr == size-1){
            size *= 2;
            line = realloc(line, size*sizeof(char));
            если(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        если(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    если(line[curr-1] == '\n') line[curr-1] = '\0';

    вернуть line;
}

целое read_int(целое fd)
{
    целое n = 0;
    целое next = read(fd, &n, sizeof(целое));
    если(next <= 0) вернуть -1;
    вернуть n;
}

void write_int(целое fd, целое n)
{
    целое next = write(fd, &n, sizeof(целое));
    если(next <= 0) error("read failed");
}

целое read_all_fail(целое fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    пока(n < bytes){
        целое next = read(fd, buffer + n, bytes-n);
        если(next <= 0) вернуть 1;
        n += next;
    }
    вернуть 0;
}

целое write_all_fail(целое fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    пока(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        если(next <= 0) вернуть 1;
        n += next;
    }
    вернуть 0;
}

void read_all(целое fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    пока(n < bytes){
        целое next = read(fd, buffer + n, bytes-n);
        если(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(целое fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    пока(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        если(next <= 0) error("write failed");
        n += next;
    }
}


char *copy_string(char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    вернуть copy;
}

list *parse_csv_line(char *line)
{
    list *l = make_list();
    char *c, *p;
    целое in = 0;
    для(c = line, p = line; *c != '\0'; ++c){
        если(*c == '"') in = !in;
        иначе если(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    вернуть l;
}

целое count_fields(char *line)
{
    целое count = 0;
    целое done = 0;
    char *c;
    для(c = line; !done; ++c){
        done = (*c == '\0');
        если(*c == ',' || done) ++count;
    }
    вернуть count;
}

float *parse_fields(char *line, целое n)
{
    float *field = calloc(n, sizeof(float));
    char *c, *p, *end;
    целое count = 0;
    целое done = 0;
    для(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        если(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            если(p == c) field[count] = nan("");
            если(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    вернуть field;
}

float sum_array(float *a, целое n)
{
    целое i;
    float sum = 0;
    для(i = 0; i < n; ++i) sum += a[i];
    вернуть sum;
}

float mean_array(float *a, целое n)
{
    вернуть sum_array(a,n)/n;
}

void mean_arrays(float **a, целое n, целое els, float *avg)
{
    целое i;
    целое j;
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

void print_statistics(float *a, целое n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, целое n)
{
    целое i;
    float sum = 0;
    float mean = mean_array(a, n);
    для(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    вернуть variance;
}

целое constrain_int(целое a, целое min, целое max)
{
    если (a < min) вернуть min;
    если (a > max) вернуть max;
    вернуть a;
}

float constrain(float min, float max, float a)
{
    если (a < min) вернуть min;
    если (a > max) вернуть max;
    вернуть a;
}

float dist_array(float *a, float *b, целое n, целое sub)
{
    целое i;
    float sum = 0;
    для(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    вернуть sqrt(sum);
}

float mse_array(float *a, целое n)
{
    целое i;
    float sum = 0;
    для(i = 0; i < n; ++i) sum += a[i]*a[i];
    вернуть sqrt(sum/n);
}

void normalize_array(float *a, целое n)
{
    целое i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    для(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, целое n, float s)
{
    целое i;
    для(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, целое n)
{
    целое i;
    float sum = 0;
    для(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    вернуть sqrt(sum);
}

void scale_array(float *a, целое n, float s)
{
    целое i;
    для(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

целое sample_array(float *a, целое n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    целое i;
    для(i = 0; i < n; ++i){
        r = r - a[i];
        если (r <= 0) вернуть i;
    }
    вернуть n-1;
}

целое max_int_index(целое *a, целое n)
{
    если(n <= 0) вернуть -1;
    целое i, max_i = 0;
    целое max = a[0];
    для(i = 1; i < n; ++i){
        если(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    вернуть max_i;
}

целое max_index(float *a, целое n)
{
    если(n <= 0) вернуть -1;
    целое i, max_i = 0;
    float max = a[0];
    для(i = 1; i < n; ++i){
        если(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    вернуть max_i;
}

целое int_index(целое *a, целое val, целое n)
{
    целое i;
    для(i = 0; i < n; ++i){
        если(a[i] == val) вернуть i;
    }
    вернуть -1;
}

целое rand_int(целое min, целое max)
{
    если (max < min){
        целое s = min;
        min = max;
        max = s;
    }
    целое r = (rand()%(max - min + 1)) + min;
    вернуть r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static целое haveSpare = 0;
    static double rand1, rand2;

    если(haveSpare)
    {
        haveSpare = 0;
        вернуть sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((double) RAND_MAX);
    если(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

    вернуть sqrt(rand1) * cos(rand2);
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
    вернуть  ((size_t)(rand()&0xff) << 56) | 
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
    если(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    вернуть ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    если(rand()%2) вернуть scale;
    вернуть 1./scale;
}

float **one_hot_encode(float *a, целое n, целое k)
{
    целое i;
    float **t = calloc(n, sizeof(float*));
    для(i = 0; i < n; ++i){
        t[i] = calloc(k, sizeof(float));
        целое index = (целое)a[i];
        t[i][index] = 1;
    }
    вернуть t;
}

