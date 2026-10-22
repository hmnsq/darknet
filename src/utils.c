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
    如果 (gettimeofday(&time,NULL)){
        返回 0;
    }
    返回 (double)time.tv_sec + (double)time.tv_usec * .000001;
}

整数 *read_intlist(char *gpu_list, 整数 *ngpus, 整数 d)
{
    整数 *gpus = 0;
    如果(gpu_list){
        整数 len = strlen(gpu_list);
        *ngpus = 1;
        整数 i;
        为(i = 0; i < len; ++i){
            如果 (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, sizeof(整数));
        为(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } 否则 {
        gpus = calloc(1, sizeof(float));
        *gpus = d;
        *ngpus = 1;
    }
    返回 gpus;
}

整数 *read_map(char *filename)
{
    整数 n = 0;
    整数 *map = 0;
    char *str;
    FILE *file = fopen(filename, "r");
    如果(!file) file_error(filename);
    当((str=fgetl(file))){
        ++n;
        map = realloc(map, n*sizeof(整数));
        map[n-1] = atoi(str);
    }
    返回 map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    为(i = 0; i < sections; ++i){
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
    为(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

整数 *random_index_order(整数 min, 整数 max)
{
    整数 *inds = calloc(max-min, sizeof(整数));
    整数 i;
    为(i = min; i < max; ++i){
        inds[i] = i;
    }
    为(i = min; i < max-1; ++i){
        整数 swap = inds[i];
        整数 index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    返回 inds;
}

void del_arg(整数 argc, char **argv, 整数 index)
{
    整数 i;
    为(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

整数 find_arg(整数 argc, char* argv[], char *arg)
{
    整数 i;
    为(i = 0; i < argc; ++i) {
        如果(!argv[i]) continue;
        如果(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            返回 1;
        }
    }
    返回 0;
}

整数 find_int_arg(整数 argc, char **argv, char *arg, 整数 def)
{
    整数 i;
    为(i = 0; i < argc-1; ++i){
        如果(!argv[i]) continue;
        如果(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    返回 def;
}

float find_float_arg(整数 argc, char **argv, char *arg, float def)
{
    整数 i;
    为(i = 0; i < argc-1; ++i){
        如果(!argv[i]) continue;
        如果(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    返回 def;
}

char *find_char_arg(整数 argc, char **argv, char *arg, char *def)
{
    整数 i;
    为(i = 0; i < argc-1; ++i){
        如果(!argv[i]) continue;
        如果(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    返回 def;
}


char *basecfg(char *cfgfile)
{
    char *c = cfgfile;
    char *next;
    当((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    如果 (next) *next = 0;
    返回 c;
}

整数 alphanum_to_int(char c)
{
    返回 (c < 58) ? c - 48 : c-87;
}
char int_to_alphanum(整数 i)
{
    如果 (i == 36) 返回 '.';
    返回 (i < 10) ? i + 48 : i + 87;
}

void pm(整数 M, 整数 N, float *A)
{
    整数 i,j;
    为(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        为(j = 0; j < N; ++j){
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
    如果(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        返回;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    返回 (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, 整数 n, 整数 k, 整数 *index)
{
    整数 i,j;
    为(j = 0; j < k; ++j) index[j] = -1;
    为(i = 0; i < n; ++i){
        整数 curr = i;
        为(j = 0; j < k; ++j){
            如果((index[j] < 0) || a[curr] > a[index[j]]){
                整数 swap = curr;
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
    返回 text;
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
    为(i = 0; i < len; ++i){
        如果(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    返回 l;
}

void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    为(i = 0; i < len; ++i){
        char c = s[i];
        如果(c==' '||c=='\t'||c=='\n') ++offset;
        否则 s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    为(i = 0; i < len; ++i){
        char c = s[i];
        如果(c==bad) ++offset;
        否则 s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, 整数 n)
{
    整数 i;
    为(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

char *fgetl(FILE *fp)
{
    如果(feof(fp)) 返回 0;
    size_t size = 512;
    char *line = malloc(size*sizeof(char));
    如果(!fgets(line, size, fp)){
        free(line);
        返回 0;
    }

    size_t curr = strlen(line);

    当((line[curr-1] != '\n') && !feof(fp)){
        如果(curr == size-1){
            size *= 2;
            line = realloc(line, size*sizeof(char));
            如果(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        如果(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    如果(line[curr-1] == '\n') line[curr-1] = '\0';

    返回 line;
}

整数 read_int(整数 fd)
{
    整数 n = 0;
    整数 next = read(fd, &n, sizeof(整数));
    如果(next <= 0) 返回 -1;
    返回 n;
}

void write_int(整数 fd, 整数 n)
{
    整数 next = write(fd, &n, sizeof(整数));
    如果(next <= 0) error("read failed");
}

整数 read_all_fail(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    当(n < bytes){
        整数 next = read(fd, buffer + n, bytes-n);
        如果(next <= 0) 返回 1;
        n += next;
    }
    返回 0;
}

整数 write_all_fail(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    当(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        如果(next <= 0) 返回 1;
        n += next;
    }
    返回 0;
}

void read_all(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    当(n < bytes){
        整数 next = read(fd, buffer + n, bytes-n);
        如果(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    当(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        如果(next <= 0) error("write failed");
        n += next;
    }
}


char *copy_string(char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    返回 copy;
}

list *parse_csv_line(char *line)
{
    list *l = make_list();
    char *c, *p;
    整数 in = 0;
    为(c = line, p = line; *c != '\0'; ++c){
        如果(*c == '"') in = !in;
        否则 如果(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    返回 l;
}

整数 count_fields(char *line)
{
    整数 count = 0;
    整数 done = 0;
    char *c;
    为(c = line; !done; ++c){
        done = (*c == '\0');
        如果(*c == ',' || done) ++count;
    }
    返回 count;
}

float *parse_fields(char *line, 整数 n)
{
    float *field = calloc(n, sizeof(float));
    char *c, *p, *end;
    整数 count = 0;
    整数 done = 0;
    为(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        如果(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            如果(p == c) field[count] = nan("");
            如果(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    返回 field;
}

float sum_array(float *a, 整数 n)
{
    整数 i;
    float sum = 0;
    为(i = 0; i < n; ++i) sum += a[i];
    返回 sum;
}

float mean_array(float *a, 整数 n)
{
    返回 sum_array(a,n)/n;
}

void mean_arrays(float **a, 整数 n, 整数 els, float *avg)
{
    整数 i;
    整数 j;
    memset(avg, 0, els*sizeof(float));
    为(j = 0; j < n; ++j){
        为(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    为(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, 整数 n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, 整数 n)
{
    整数 i;
    float sum = 0;
    float mean = mean_array(a, n);
    为(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    返回 variance;
}

整数 constrain_int(整数 a, 整数 min, 整数 max)
{
    如果 (a < min) 返回 min;
    如果 (a > max) 返回 max;
    返回 a;
}

float constrain(float min, float max, float a)
{
    如果 (a < min) 返回 min;
    如果 (a > max) 返回 max;
    返回 a;
}

float dist_array(float *a, float *b, 整数 n, 整数 sub)
{
    整数 i;
    float sum = 0;
    为(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    返回 sqrt(sum);
}

float mse_array(float *a, 整数 n)
{
    整数 i;
    float sum = 0;
    为(i = 0; i < n; ++i) sum += a[i]*a[i];
    返回 sqrt(sum/n);
}

void normalize_array(float *a, 整数 n)
{
    整数 i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    为(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, 整数 n, float s)
{
    整数 i;
    为(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, 整数 n)
{
    整数 i;
    float sum = 0;
    为(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    返回 sqrt(sum);
}

void scale_array(float *a, 整数 n, float s)
{
    整数 i;
    为(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

整数 sample_array(float *a, 整数 n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    整数 i;
    为(i = 0; i < n; ++i){
        r = r - a[i];
        如果 (r <= 0) 返回 i;
    }
    返回 n-1;
}

整数 max_int_index(整数 *a, 整数 n)
{
    如果(n <= 0) 返回 -1;
    整数 i, max_i = 0;
    整数 max = a[0];
    为(i = 1; i < n; ++i){
        如果(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    返回 max_i;
}

整数 max_index(float *a, 整数 n)
{
    如果(n <= 0) 返回 -1;
    整数 i, max_i = 0;
    float max = a[0];
    为(i = 1; i < n; ++i){
        如果(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    返回 max_i;
}

整数 int_index(整数 *a, 整数 val, 整数 n)
{
    整数 i;
    为(i = 0; i < n; ++i){
        如果(a[i] == val) 返回 i;
    }
    返回 -1;
}

整数 rand_int(整数 min, 整数 max)
{
    如果 (max < min){
        整数 s = min;
        min = max;
        max = s;
    }
    整数 r = (rand()%(max - min + 1)) + min;
    返回 r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static 整数 haveSpare = 0;
    static double rand1, rand2;

    如果(haveSpare)
    {
        haveSpare = 0;
        返回 sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((double) RAND_MAX);
    如果(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

    返回 sqrt(rand1) * cos(rand2);
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
    返回  ((size_t)(rand()&0xff) << 56) | 
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
    如果(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    返回 ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    如果(rand()%2) 返回 scale;
    返回 1./scale;
}

float **one_hot_encode(float *a, 整数 n, 整数 k)
{
    整数 i;
    float **t = calloc(n, sizeof(float*));
    为(i = 0; i < n; ++i){
        t[i] = calloc(k, sizeof(float));
        整数 index = (整数)a[i];
        t[i][index] = 1;
    }
    返回 t;
}

