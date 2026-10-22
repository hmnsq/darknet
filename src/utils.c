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
    만약 (gettimeofday(&time,NULL)){
        반환 0;
    }
    반환 (double)time.tv_sec + (double)time.tv_usec * .000001;
}

정수 *read_intlist(char *gpu_list, 정수 *ngpus, 정수 d)
{
    정수 *gpus = 0;
    만약(gpu_list){
        정수 len = strlen(gpu_list);
        *ngpus = 1;
        정수 i;
        위해(i = 0; i < len; ++i){
            만약 (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, sizeof(정수));
        위해(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } 아니면 {
        gpus = calloc(1, sizeof(float));
        *gpus = d;
        *ngpus = 1;
    }
    반환 gpus;
}

정수 *read_map(char *filename)
{
    정수 n = 0;
    정수 *map = 0;
    char *str;
    FILE *file = fopen(filename, "r");
    만약(!file) file_error(filename);
    동안((str=fgetl(file))){
        ++n;
        map = realloc(map, n*sizeof(정수));
        map[n-1] = atoi(str);
    }
    반환 map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    위해(i = 0; i < sections; ++i){
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
    위해(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

정수 *random_index_order(정수 min, 정수 max)
{
    정수 *inds = calloc(max-min, sizeof(정수));
    정수 i;
    위해(i = min; i < max; ++i){
        inds[i] = i;
    }
    위해(i = min; i < max-1; ++i){
        정수 swap = inds[i];
        정수 index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    반환 inds;
}

void del_arg(정수 argc, char **argv, 정수 index)
{
    정수 i;
    위해(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

정수 find_arg(정수 argc, char* argv[], char *arg)
{
    정수 i;
    위해(i = 0; i < argc; ++i) {
        만약(!argv[i]) continue;
        만약(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            반환 1;
        }
    }
    반환 0;
}

정수 find_int_arg(정수 argc, char **argv, char *arg, 정수 def)
{
    정수 i;
    위해(i = 0; i < argc-1; ++i){
        만약(!argv[i]) continue;
        만약(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    반환 def;
}

float find_float_arg(정수 argc, char **argv, char *arg, float def)
{
    정수 i;
    위해(i = 0; i < argc-1; ++i){
        만약(!argv[i]) continue;
        만약(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    반환 def;
}

char *find_char_arg(정수 argc, char **argv, char *arg, char *def)
{
    정수 i;
    위해(i = 0; i < argc-1; ++i){
        만약(!argv[i]) continue;
        만약(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    반환 def;
}


char *basecfg(char *cfgfile)
{
    char *c = cfgfile;
    char *next;
    동안((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    만약 (next) *next = 0;
    반환 c;
}

정수 alphanum_to_int(char c)
{
    반환 (c < 58) ? c - 48 : c-87;
}
char int_to_alphanum(정수 i)
{
    만약 (i == 36) 반환 '.';
    반환 (i < 10) ? i + 48 : i + 87;
}

void pm(정수 M, 정수 N, float *A)
{
    정수 i,j;
    위해(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        위해(j = 0; j < N; ++j){
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
    만약(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        반환;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    반환 (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, 정수 n, 정수 k, 정수 *index)
{
    정수 i,j;
    위해(j = 0; j < k; ++j) index[j] = -1;
    위해(i = 0; i < n; ++i){
        정수 curr = i;
        위해(j = 0; j < k; ++j){
            만약((index[j] < 0) || a[curr] > a[index[j]]){
                정수 swap = curr;
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
    반환 text;
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
    위해(i = 0; i < len; ++i){
        만약(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    반환 l;
}

void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    위해(i = 0; i < len; ++i){
        char c = s[i];
        만약(c==' '||c=='\t'||c=='\n') ++offset;
        아니면 s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    위해(i = 0; i < len; ++i){
        char c = s[i];
        만약(c==bad) ++offset;
        아니면 s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, 정수 n)
{
    정수 i;
    위해(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

char *fgetl(FILE *fp)
{
    만약(feof(fp)) 반환 0;
    size_t size = 512;
    char *line = malloc(size*sizeof(char));
    만약(!fgets(line, size, fp)){
        free(line);
        반환 0;
    }

    size_t curr = strlen(line);

    동안((line[curr-1] != '\n') && !feof(fp)){
        만약(curr == size-1){
            size *= 2;
            line = realloc(line, size*sizeof(char));
            만약(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        만약(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    만약(line[curr-1] == '\n') line[curr-1] = '\0';

    반환 line;
}

정수 read_int(정수 fd)
{
    정수 n = 0;
    정수 next = read(fd, &n, sizeof(정수));
    만약(next <= 0) 반환 -1;
    반환 n;
}

void write_int(정수 fd, 정수 n)
{
    정수 next = write(fd, &n, sizeof(정수));
    만약(next <= 0) error("read failed");
}

정수 read_all_fail(정수 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    동안(n < bytes){
        정수 next = read(fd, buffer + n, bytes-n);
        만약(next <= 0) 반환 1;
        n += next;
    }
    반환 0;
}

정수 write_all_fail(정수 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    동안(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        만약(next <= 0) 반환 1;
        n += next;
    }
    반환 0;
}

void read_all(정수 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    동안(n < bytes){
        정수 next = read(fd, buffer + n, bytes-n);
        만약(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(정수 fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    동안(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        만약(next <= 0) error("write failed");
        n += next;
    }
}


char *copy_string(char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    반환 copy;
}

list *parse_csv_line(char *line)
{
    list *l = make_list();
    char *c, *p;
    정수 in = 0;
    위해(c = line, p = line; *c != '\0'; ++c){
        만약(*c == '"') in = !in;
        아니면 만약(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    반환 l;
}

정수 count_fields(char *line)
{
    정수 count = 0;
    정수 done = 0;
    char *c;
    위해(c = line; !done; ++c){
        done = (*c == '\0');
        만약(*c == ',' || done) ++count;
    }
    반환 count;
}

float *parse_fields(char *line, 정수 n)
{
    float *field = calloc(n, sizeof(float));
    char *c, *p, *end;
    정수 count = 0;
    정수 done = 0;
    위해(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        만약(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            만약(p == c) field[count] = nan("");
            만약(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    반환 field;
}

float sum_array(float *a, 정수 n)
{
    정수 i;
    float sum = 0;
    위해(i = 0; i < n; ++i) sum += a[i];
    반환 sum;
}

float mean_array(float *a, 정수 n)
{
    반환 sum_array(a,n)/n;
}

void mean_arrays(float **a, 정수 n, 정수 els, float *avg)
{
    정수 i;
    정수 j;
    memset(avg, 0, els*sizeof(float));
    위해(j = 0; j < n; ++j){
        위해(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    위해(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, 정수 n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, 정수 n)
{
    정수 i;
    float sum = 0;
    float mean = mean_array(a, n);
    위해(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    반환 variance;
}

정수 constrain_int(정수 a, 정수 min, 정수 max)
{
    만약 (a < min) 반환 min;
    만약 (a > max) 반환 max;
    반환 a;
}

float constrain(float min, float max, float a)
{
    만약 (a < min) 반환 min;
    만약 (a > max) 반환 max;
    반환 a;
}

float dist_array(float *a, float *b, 정수 n, 정수 sub)
{
    정수 i;
    float sum = 0;
    위해(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    반환 sqrt(sum);
}

float mse_array(float *a, 정수 n)
{
    정수 i;
    float sum = 0;
    위해(i = 0; i < n; ++i) sum += a[i]*a[i];
    반환 sqrt(sum/n);
}

void normalize_array(float *a, 정수 n)
{
    정수 i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    위해(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, 정수 n, float s)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, 정수 n)
{
    정수 i;
    float sum = 0;
    위해(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    반환 sqrt(sum);
}

void scale_array(float *a, 정수 n, float s)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

정수 sample_array(float *a, 정수 n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    정수 i;
    위해(i = 0; i < n; ++i){
        r = r - a[i];
        만약 (r <= 0) 반환 i;
    }
    반환 n-1;
}

정수 max_int_index(정수 *a, 정수 n)
{
    만약(n <= 0) 반환 -1;
    정수 i, max_i = 0;
    정수 max = a[0];
    위해(i = 1; i < n; ++i){
        만약(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    반환 max_i;
}

정수 max_index(float *a, 정수 n)
{
    만약(n <= 0) 반환 -1;
    정수 i, max_i = 0;
    float max = a[0];
    위해(i = 1; i < n; ++i){
        만약(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    반환 max_i;
}

정수 int_index(정수 *a, 정수 val, 정수 n)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        만약(a[i] == val) 반환 i;
    }
    반환 -1;
}

정수 rand_int(정수 min, 정수 max)
{
    만약 (max < min){
        정수 s = min;
        min = max;
        max = s;
    }
    정수 r = (rand()%(max - min + 1)) + min;
    반환 r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static 정수 haveSpare = 0;
    static double rand1, rand2;

    만약(haveSpare)
    {
        haveSpare = 0;
        반환 sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((double) RAND_MAX);
    만약(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

    반환 sqrt(rand1) * cos(rand2);
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
    반환  ((size_t)(rand()&0xff) << 56) | 
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
    만약(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    반환 ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    만약(rand()%2) 반환 scale;
    반환 1./scale;
}

float **one_hot_encode(float *a, 정수 n, 정수 k)
{
    정수 i;
    float **t = calloc(n, sizeof(float*));
    위해(i = 0; i < n; ++i){
        t[i] = calloc(k, sizeof(float));
        정수 index = (정수)a[i];
        t[i][index] = 1;
    }
    반환 t;
}

