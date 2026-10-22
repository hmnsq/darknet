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
    ако (gettimeofday(&time,NULL)){
        врати 0;
    }
    врати (double)time.tv_sec + (double)time.tv_usec * .000001;
}

цео *read_intlist(char *gpu_list, цео *ngpus, цео d)
{
    цео *gpus = 0;
    ако(gpu_list){
        цео len = strlen(gpu_list);
        *ngpus = 1;
        цео i;
        за(i = 0; i < len; ++i){
            ако (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, sizeof(цео));
        за(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } инако {
        gpus = calloc(1, sizeof(float));
        *gpus = d;
        *ngpus = 1;
    }
    врати gpus;
}

цео *read_map(char *filename)
{
    цео n = 0;
    цео *map = 0;
    char *str;
    FILE *file = fopen(filename, "r");
    ако(!file) file_error(filename);
    док((str=fgetl(file))){
        ++n;
        map = realloc(map, n*sizeof(цео));
        map[n-1] = atoi(str);
    }
    врати map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    за(i = 0; i < sections; ++i){
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
    за(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

цео *random_index_order(цео min, цео max)
{
    цео *inds = calloc(max-min, sizeof(цео));
    цео i;
    за(i = min; i < max; ++i){
        inds[i] = i;
    }
    за(i = min; i < max-1; ++i){
        цео swap = inds[i];
        цео index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    врати inds;
}

void del_arg(цео argc, char **argv, цео index)
{
    цео i;
    за(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

цео find_arg(цео argc, char* argv[], char *arg)
{
    цео i;
    за(i = 0; i < argc; ++i) {
        ако(!argv[i]) continue;
        ако(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            врати 1;
        }
    }
    врати 0;
}

цео find_int_arg(цео argc, char **argv, char *arg, цео def)
{
    цео i;
    за(i = 0; i < argc-1; ++i){
        ако(!argv[i]) continue;
        ако(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    врати def;
}

float find_float_arg(цео argc, char **argv, char *arg, float def)
{
    цео i;
    за(i = 0; i < argc-1; ++i){
        ако(!argv[i]) continue;
        ако(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    врати def;
}

char *find_char_arg(цео argc, char **argv, char *arg, char *def)
{
    цео i;
    за(i = 0; i < argc-1; ++i){
        ако(!argv[i]) continue;
        ако(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            break;
        }
    }
    врати def;
}


char *basecfg(char *cfgfile)
{
    char *c = cfgfile;
    char *next;
    док((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    ако (next) *next = 0;
    врати c;
}

цео alphanum_to_int(char c)
{
    врати (c < 58) ? c - 48 : c-87;
}
char int_to_alphanum(цео i)
{
    ако (i == 36) врати '.';
    врати (i < 10) ? i + 48 : i + 87;
}

void pm(цео M, цео N, float *A)
{
    цео i,j;
    за(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        за(j = 0; j < N; ++j){
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
    ако(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        врати;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    врати (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, цео n, цео k, цео *index)
{
    цео i,j;
    за(j = 0; j < k; ++j) index[j] = -1;
    за(i = 0; i < n; ++i){
        цео curr = i;
        за(j = 0; j < k; ++j){
            ако((index[j] < 0) || a[curr] > a[index[j]]){
                цео swap = curr;
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
    врати text;
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
    за(i = 0; i < len; ++i){
        ако(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    врати l;
}

void strip(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    за(i = 0; i < len; ++i){
        char c = s[i];
        ако(c==' '||c=='\t'||c=='\n') ++offset;
        инако s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    за(i = 0; i < len; ++i){
        char c = s[i];
        ако(c==bad) ++offset;
        инако s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, цео n)
{
    цео i;
    за(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

char *fgetl(FILE *fp)
{
    ако(feof(fp)) врати 0;
    size_t size = 512;
    char *line = malloc(size*sizeof(char));
    ако(!fgets(line, size, fp)){
        free(line);
        врати 0;
    }

    size_t curr = strlen(line);

    док((line[curr-1] != '\n') && !feof(fp)){
        ако(curr == size-1){
            size *= 2;
            line = realloc(line, size*sizeof(char));
            ако(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        ако(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    ако(line[curr-1] == '\n') line[curr-1] = '\0';

    врати line;
}

цео read_int(цео fd)
{
    цео n = 0;
    цео next = read(fd, &n, sizeof(цео));
    ако(next <= 0) врати -1;
    врати n;
}

void write_int(цео fd, цео n)
{
    цео next = write(fd, &n, sizeof(цео));
    ако(next <= 0) error("read failed");
}

цео read_all_fail(цео fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    док(n < bytes){
        цео next = read(fd, buffer + n, bytes-n);
        ако(next <= 0) врати 1;
        n += next;
    }
    врати 0;
}

цео write_all_fail(цео fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    док(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ако(next <= 0) врати 1;
        n += next;
    }
    врати 0;
}

void read_all(цео fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    док(n < bytes){
        цео next = read(fd, buffer + n, bytes-n);
        ако(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(цео fd, char *buffer, size_t bytes)
{
    size_t n = 0;
    док(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ако(next <= 0) error("write failed");
        n += next;
    }
}


char *copy_string(char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    врати copy;
}

list *parse_csv_line(char *line)
{
    list *l = make_list();
    char *c, *p;
    цео in = 0;
    за(c = line, p = line; *c != '\0'; ++c){
        ако(*c == '"') in = !in;
        инако ако(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    врати l;
}

цео count_fields(char *line)
{
    цео count = 0;
    цео done = 0;
    char *c;
    за(c = line; !done; ++c){
        done = (*c == '\0');
        ако(*c == ',' || done) ++count;
    }
    врати count;
}

float *parse_fields(char *line, цео n)
{
    float *field = calloc(n, sizeof(float));
    char *c, *p, *end;
    цео count = 0;
    цео done = 0;
    за(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        ако(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            ако(p == c) field[count] = nan("");
            ако(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    врати field;
}

float sum_array(float *a, цео n)
{
    цео i;
    float sum = 0;
    за(i = 0; i < n; ++i) sum += a[i];
    врати sum;
}

float mean_array(float *a, цео n)
{
    врати sum_array(a,n)/n;
}

void mean_arrays(float **a, цео n, цео els, float *avg)
{
    цео i;
    цео j;
    memset(avg, 0, els*sizeof(float));
    за(j = 0; j < n; ++j){
        за(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    за(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, цео n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, цео n)
{
    цео i;
    float sum = 0;
    float mean = mean_array(a, n);
    за(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    врати variance;
}

цео constrain_int(цео a, цео min, цео max)
{
    ако (a < min) врати min;
    ако (a > max) врати max;
    врати a;
}

float constrain(float min, float max, float a)
{
    ако (a < min) врати min;
    ако (a > max) врати max;
    врати a;
}

float dist_array(float *a, float *b, цео n, цео sub)
{
    цео i;
    float sum = 0;
    за(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    врати sqrt(sum);
}

float mse_array(float *a, цео n)
{
    цео i;
    float sum = 0;
    за(i = 0; i < n; ++i) sum += a[i]*a[i];
    врати sqrt(sum/n);
}

void normalize_array(float *a, цео n)
{
    цео i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    за(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, цео n, float s)
{
    цео i;
    за(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, цео n)
{
    цео i;
    float sum = 0;
    за(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    врати sqrt(sum);
}

void scale_array(float *a, цео n, float s)
{
    цео i;
    за(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

цео sample_array(float *a, цео n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    цео i;
    за(i = 0; i < n; ++i){
        r = r - a[i];
        ако (r <= 0) врати i;
    }
    врати n-1;
}

цео max_int_index(цео *a, цео n)
{
    ако(n <= 0) врати -1;
    цео i, max_i = 0;
    цео max = a[0];
    за(i = 1; i < n; ++i){
        ако(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    врати max_i;
}

цео max_index(float *a, цео n)
{
    ако(n <= 0) врати -1;
    цео i, max_i = 0;
    float max = a[0];
    за(i = 1; i < n; ++i){
        ако(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    врати max_i;
}

цео int_index(цео *a, цео val, цео n)
{
    цео i;
    за(i = 0; i < n; ++i){
        ако(a[i] == val) врати i;
    }
    врати -1;
}

цео rand_int(цео min, цео max)
{
    ако (max < min){
        цео s = min;
        min = max;
        max = s;
    }
    цео r = (rand()%(max - min + 1)) + min;
    врати r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static цео haveSpare = 0;
    static double rand1, rand2;

    ако(haveSpare)
    {
        haveSpare = 0;
        врати sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((double) RAND_MAX);
    ако(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

    врати sqrt(rand1) * cos(rand2);
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
    врати  ((size_t)(rand()&0xff) << 56) | 
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
    ако(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    врати ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    ако(rand()%2) врати scale;
    врати 1./scale;
}

float **one_hot_encode(float *a, цео n, цео k)
{
    цео i;
    float **t = calloc(n, sizeof(float*));
    за(i = 0; i < n; ++i){
        t[i] = calloc(k, sizeof(float));
        цео index = (цео)a[i];
        t[i][index] = 1;
    }
    врати t;
}

