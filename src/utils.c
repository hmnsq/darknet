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
    もし (gettimeofday(&time,NULL)){
        戻る 0;
    }
    戻る (double)time.tv_sec + (double)time.tv_usec * .000001;
}

整数 *read_intlist(char *gpu_list, 整数 *ngpus, 整数 d)
{
    整数 *gpus = 0;
    もし(gpu_list){
        整数 len = strlen(gpu_list);
        *ngpus = 1;
        整数 i;
        ために(i = 0; i < len; ++i){
            もし (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, sizeof(整数));
        ために(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } 否则 {
        gpus = calloc(1, sizeof(float));
        *gpus = d;
        *ngpus = 1;
    }
    戻る gpus;
}

整数 *read_map(char *filename)
{
    整数 n = 0;
    整数 *map = 0;
    char *str;
    FILE *file = fopen(filename, "r");
    もし(!file) file_error(filename);
    ながら((str=fgetl(file))){
        ++n;
        map = realloc(map, n*sizeof(整数));
        map[n-1] = atoi(str);
    }
    戻る map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    ために(i = 0; i < sections; ++i){
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
    ために(i = 0; i < n-1; ++i){
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
    ために(i = min; i < max; ++i){
        inds[i] = i;
    }
    ために(i = min; i < max-1; ++i){
        整数 swap = inds[i];
        整数 index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    戻る inds;
}

void del_arg(整数 argc, char **argv, 整数 index)
{
    整数 i;
    ために(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

整数 find_arg(整数 argc, char* argv[], char *arg)
{
    整数 i;
    ために(i = 0; i < argc; ++i) {
        もし(!argv[i]) continue;
        もし(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            戻る 1;
        }
    }
    戻る 0;
}

整数 find_int_arg(整数 argc, char **argv, char *arg, 整数 def)
{
    整数 i;
    ために(i = 0; i < argc-1; ++i){
        もし(!argv[i]) continue;
        もし(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    戻る def;
}

float find_float_arg(整数 argc, char **argv, char *arg, float def)
{
    整数 i;
    ために(i = 0; i < argc-1; ++i){
        もし(!argv[i]) continue;
        もし(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    戻る def;
}

char *find_char_arg(整数 argc, char **argv, char *arg, char *def)
{
    整数 i;
    ために(i = 0; i < argc-1; ++i){
        もし(!argv[i]) continue;
        もし(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    戻る def;
}


char *basecfg(char *cfgfile)
{
    char *c = cfgfile;
    char *next;
    ながら((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    もし (next) *next = 0;
    戻る c;
}

整数 alphanum_to_int(char c)
{
    戻る (c < 58) ? c - 48 : c-87;
}
char int_to_alphanum(整数 i)
{
    もし (i == 36) 戻る '.';
    戻る (i < 10) ? i + 48 : i + 87;
}

void pm(整数 M, 整数 N, float *A)
{
    整数 i,j;
    ために(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        ために(j = 0; j < N; ++j){
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
    もし(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        戻る;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    戻る (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, 整数 n, 整数 k, 整数 *index)
{
    整数 i,j;
    ために(j = 0; j < k; ++j) index[j] = -1;
    ために(i = 0; i < n; ++i){
        整数 curr = i;
        ために(j = 0; j < k; ++j){
            もし((index[j] < 0) || a[curr] > a[index[j]]){
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
    戻る text;
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
    ために(i = 0; i < len; ++i){
        もし(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    戻る l;
}

void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ために(i = 0; i < len; ++i){
        char c = s[i];
        もし(c==' '||c=='\t'||c=='\n') ++offset;
        否则 s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ために(i = 0; i < len; ++i){
        char c = s[i];
        もし(c==bad) ++offset;
        否则 s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, 整数 n)
{
    整数 i;
    ために(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

char *fgetl(FILE *fp)
{
    もし(feof(fp)) 戻る 0;
    size_t size = 512;
    char *line = malloc(size*sizeof(char));
    もし(!fgets(line, size, fp)){
        free(line);
        戻る 0;
    }

    size_t curr = strlen(line);

    ながら((line[curr-1] != '\n') && !feof(fp)){
        もし(curr == size-1){
            size *= 2;
            line = realloc(line, size*sizeof(char));
            もし(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        もし(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    もし(line[curr-1] == '\n') line[curr-1] = '\0';

    戻る line;
}

整数 read_int(整数 fd)
{
    整数 n = 0;
    整数 next = read(fd, &n, sizeof(整数));
    もし(next <= 0) 戻る -1;
    戻る n;
}

void write_int(整数 fd, 整数 n)
{
    整数 next = write(fd, &n, sizeof(整数));
    もし(next <= 0) error("read failed");
}

整数 read_all_fail(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    ながら(n < bytes){
        整数 next = read(fd, buffer + n, bytes-n);
        もし(next <= 0) 戻る 1;
        n += next;
    }
    戻る 0;
}

整数 write_all_fail(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    ながら(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        もし(next <= 0) 戻る 1;
        n += next;
    }
    戻る 0;
}

void read_all(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    ながら(n < bytes){
        整数 next = read(fd, buffer + n, bytes-n);
        もし(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(整数 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    ながら(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        もし(next <= 0) error("write failed");
        n += next;
    }
}


char *copy_string(char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    戻る copy;
}

list *parse_csv_line(char *line)
{
    list *l = make_list();
    char *c, *p;
    整数 in = 0;
    ために(c = line, p = line; *c != '\0'; ++c){
        もし(*c == '"') in = !in;
        否则 もし(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    戻る l;
}

整数 count_fields(char *line)
{
    整数 count = 0;
    整数 done = 0;
    char *c;
    ために(c = line; !done; ++c){
        done = (*c == '\0');
        もし(*c == ',' || done) ++count;
    }
    戻る count;
}

float *parse_fields(char *line, 整数 n)
{
    float *field = calloc(n, sizeof(float));
    char *c, *p, *end;
    整数 count = 0;
    整数 done = 0;
    ために(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        もし(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            もし(p == c) field[count] = nan("");
            もし(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    戻る field;
}

float sum_array(float *a, 整数 n)
{
    整数 i;
    float sum = 0;
    ために(i = 0; i < n; ++i) sum += a[i];
    戻る sum;
}

float mean_array(float *a, 整数 n)
{
    戻る sum_array(a,n)/n;
}

void mean_arrays(float **a, 整数 n, 整数 els, float *avg)
{
    整数 i;
    整数 j;
    memset(avg, 0, els*sizeof(float));
    ために(j = 0; j < n; ++j){
        ために(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    ために(i = 0; i < els; ++i){
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
    ために(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    戻る variance;
}

整数 constrain_int(整数 a, 整数 min, 整数 max)
{
    もし (a < min) 戻る min;
    もし (a > max) 戻る max;
    戻る a;
}

float constrain(float min, float max, float a)
{
    もし (a < min) 戻る min;
    もし (a > max) 戻る max;
    戻る a;
}

float dist_array(float *a, float *b, 整数 n, 整数 sub)
{
    整数 i;
    float sum = 0;
    ために(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    戻る sqrt(sum);
}

float mse_array(float *a, 整数 n)
{
    整数 i;
    float sum = 0;
    ために(i = 0; i < n; ++i) sum += a[i]*a[i];
    戻る sqrt(sum/n);
}

void normalize_array(float *a, 整数 n)
{
    整数 i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    ために(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, 整数 n, float s)
{
    整数 i;
    ために(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, 整数 n)
{
    整数 i;
    float sum = 0;
    ために(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    戻る sqrt(sum);
}

void scale_array(float *a, 整数 n, float s)
{
    整数 i;
    ために(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

整数 sample_array(float *a, 整数 n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    整数 i;
    ために(i = 0; i < n; ++i){
        r = r - a[i];
        もし (r <= 0) 戻る i;
    }
    戻る n-1;
}

整数 max_int_index(整数 *a, 整数 n)
{
    もし(n <= 0) 戻る -1;
    整数 i, max_i = 0;
    整数 max = a[0];
    ために(i = 1; i < n; ++i){
        もし(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    戻る max_i;
}

整数 max_index(float *a, 整数 n)
{
    もし(n <= 0) 戻る -1;
    整数 i, max_i = 0;
    float max = a[0];
    ために(i = 1; i < n; ++i){
        もし(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    戻る max_i;
}

整数 int_index(整数 *a, 整数 val, 整数 n)
{
    整数 i;
    ために(i = 0; i < n; ++i){
        もし(a[i] == val) 戻る i;
    }
    戻る -1;
}

整数 rand_int(整数 min, 整数 max)
{
    もし (max < min){
        整数 s = min;
        min = max;
        max = s;
    }
    整数 r = (rand()%(max - min + 1)) + min;
    戻る r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static 整数 haveSpare = 0;
    static double rand1, rand2;

    もし(haveSpare)
    {
        haveSpare = 0;
        戻る sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((double) RAND_MAX);
    もし(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

    戻る sqrt(rand1) * cos(rand2);
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
    戻る  ((size_t)(rand()&0xff) << 56) | 
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
    もし(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    戻る ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    もし(rand()%2) 戻る scale;
    戻る 1./scale;
}

float **one_hot_encode(float *a, 整数 n, 整数 k)
{
    整数 i;
    float **t = calloc(n, sizeof(float*));
    ために(i = 0; i < n; ++i){
        t[i] = calloc(k, sizeof(float));
        整数 index = (整数)a[i];
        t[i][index] = 1;
    }
    戻る t;
}

