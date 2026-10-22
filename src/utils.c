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
    ถ้า (gettimeofday(&time,NULL)){
        คืน 0;
    }
    คืน (dviguu_na)time.tv_sec + (dviguu_na)time.tv_usec * .000001;
}

จำนวนเต็ม *read_intlist(var_na *gpu_list, จำนวนเต็ม *ngpus, จำนวนเต็ม d)
{
    จำนวนเต็ม *gpus = 0;
    ถ้า(gpu_list){
        จำนวนเต็ม len = strlen(gpu_list);
        *ngpus = 1;
        จำนวนเต็ม i;
        สำหรับ(i = 0; i < len; ++i){
            ถ้า (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, parimaa_na(จำนวนเต็ม));
        สำหรับ(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } มิฉะนั้น {
        gpus = calloc(1, parimaa_na(float));
        *gpus = d;
        *ngpus = 1;
    }
    คืน gpus;
}

จำนวนเต็ม *read_map(var_na *filename)
{
    จำนวนเต็ม n = 0;
    จำนวนเต็ม *map = 0;
    var_na *str;
    FILE *file = fopen(filename, "r");
    ถ้า(!file) file_error(filename);
    ขณะ((str=fgetl(file))){
        ++n;
        map = realloc(map, n*parimaa_na(จำนวนเต็ม));
        map[n-1] = atoi(str);
    }
    คืน map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    สำหรับ(i = 0; i < sections; ++i){
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
    สำหรับ(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

จำนวนเต็ม *random_index_order(จำนวนเต็ม min, จำนวนเต็ม max)
{
    จำนวนเต็ม *inds = calloc(max-min, parimaa_na(จำนวนเต็ม));
    จำนวนเต็ม i;
    สำหรับ(i = min; i < max; ++i){
        inds[i] = i;
    }
    สำหรับ(i = min; i < max-1; ++i){
        จำนวนเต็ม swap = inds[i];
        จำนวนเต็ม index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    คืน inds;
}

void del_arg(จำนวนเต็ม argc, var_na **argv, จำนวนเต็ม index)
{
    จำนวนเต็ม i;
    สำหรับ(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

จำนวนเต็ม find_arg(จำนวนเต็ม argc, var_na* argv[], var_na *arg)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < argc; ++i) {
        ถ้า(!argv[i]) continue;
        ถ้า(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            คืน 1;
        }
    }
    คืน 0;
}

จำนวนเต็ม find_int_arg(จำนวนเต็ม argc, var_na **argv, var_na *arg, จำนวนเต็ม def)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < argc-1; ++i){
        ถ้า(!argv[i]) continue;
        ถ้า(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    คืน def;
}

float find_float_arg(จำนวนเต็ม argc, var_na **argv, var_na *arg, float def)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < argc-1; ++i){
        ถ้า(!argv[i]) continue;
        ถ้า(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    คืน def;
}

var_na *find_char_arg(จำนวนเต็ม argc, var_na **argv, var_na *arg, var_na *def)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < argc-1; ++i){
        ถ้า(!argv[i]) continue;
        ถ้า(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    คืน def;
}


var_na *basecfg(var_na *cfgfile)
{
    var_na *c = cfgfile;
    var_na *next;
    ขณะ((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    ถ้า (next) *next = 0;
    คืน c;
}

จำนวนเต็ม alphanum_to_int(var_na c)
{
    คืน (c < 58) ? c - 48 : c-87;
}
var_na int_to_alphanum(จำนวนเต็ม i)
{
    ถ้า (i == 36) คืน '.';
    คืน (i < 10) ? i + 48 : i + 87;
}

void pm(จำนวนเต็ม M, จำนวนเต็ม N, float *A)
{
    จำนวนเต็ม i,j;
    สำหรับ(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        สำหรับ(j = 0; j < N; ++j){
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
    ถ้า(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        คืน;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    คืน (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, จำนวนเต็ม n, จำนวนเต็ม k, จำนวนเต็ม *index)
{
    จำนวนเต็ม i,j;
    สำหรับ(j = 0; j < k; ++j) index[j] = -1;
    สำหรับ(i = 0; i < n; ++i){
        จำนวนเต็ม curr = i;
        สำหรับ(j = 0; j < k; ++j){
            ถ้า((index[j] < 0) || a[curr] > a[index[j]]){
                จำนวนเต็ม swap = curr;
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
    คืน text;
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
    สำหรับ(i = 0; i < len; ++i){
        ถ้า(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    คืน l;
}

void strip(var_na *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    สำหรับ(i = 0; i < len; ++i){
        var_na c = s[i];
        ถ้า(c==' '||c=='\t'||c=='\n') ++offset;
        มิฉะนั้น s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(var_na *s, var_na bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    สำหรับ(i = 0; i < len; ++i){
        var_na c = s[i];
        ถ้า(c==bad) ++offset;
        มิฉะนั้น s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

var_na *fgetl(FILE *fp)
{
    ถ้า(feof(fp)) คืน 0;
    size_t size = 512;
    var_na *line = malloc(size*parimaa_na(var_na));
    ถ้า(!fgets(line, size, fp)){
        free(line);
        คืน 0;
    }

    size_t curr = strlen(line);

    ขณะ((line[curr-1] != '\n') && !feof(fp)){
        ถ้า(curr == size-1){
            size *= 2;
            line = realloc(line, size*parimaa_na(var_na));
            ถ้า(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        ถ้า(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    ถ้า(line[curr-1] == '\n') line[curr-1] = '\0';

    คืน line;
}

จำนวนเต็ม read_int(จำนวนเต็ม fd)
{
    จำนวนเต็ม n = 0;
    จำนวนเต็ม next = read(fd, &n, parimaa_na(จำนวนเต็ม));
    ถ้า(next <= 0) คืน -1;
    คืน n;
}

void write_int(จำนวนเต็ม fd, จำนวนเต็ม n)
{
    จำนวนเต็ม next = write(fd, &n, parimaa_na(จำนวนเต็ม));
    ถ้า(next <= 0) error("read failed");
}

จำนวนเต็ม read_all_fail(จำนวนเต็ม fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ขณะ(n < bytes){
        จำนวนเต็ม next = read(fd, buffer + n, bytes-n);
        ถ้า(next <= 0) คืน 1;
        n += next;
    }
    คืน 0;
}

จำนวนเต็ม write_all_fail(จำนวนเต็ม fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ขณะ(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ถ้า(next <= 0) คืน 1;
        n += next;
    }
    คืน 0;
}

void read_all(จำนวนเต็ม fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ขณะ(n < bytes){
        จำนวนเต็ม next = read(fd, buffer + n, bytes-n);
        ถ้า(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(จำนวนเต็ม fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ขณะ(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ถ้า(next <= 0) error("write failed");
        n += next;
    }
}


var_na *copy_string(var_na *s)
{
    var_na *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    คืน copy;
}

list *parse_csv_line(var_na *line)
{
    list *l = make_list();
    var_na *c, *p;
    จำนวนเต็ม in = 0;
    สำหรับ(c = line, p = line; *c != '\0'; ++c){
        ถ้า(*c == '"') in = !in;
        มิฉะนั้น ถ้า(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    คืน l;
}

จำนวนเต็ม count_fields(var_na *line)
{
    จำนวนเต็ม count = 0;
    จำนวนเต็ม done = 0;
    var_na *c;
    สำหรับ(c = line; !done; ++c){
        done = (*c == '\0');
        ถ้า(*c == ',' || done) ++count;
    }
    คืน count;
}

float *parse_fields(var_na *line, จำนวนเต็ม n)
{
    float *field = calloc(n, parimaa_na(float));
    var_na *c, *p, *end;
    จำนวนเต็ม count = 0;
    จำนวนเต็ม done = 0;
    สำหรับ(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        ถ้า(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            ถ้า(p == c) field[count] = nan("");
            ถ้า(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    คืน field;
}

float sum_array(float *a, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    float sum = 0;
    สำหรับ(i = 0; i < n; ++i) sum += a[i];
    คืน sum;
}

float mean_array(float *a, จำนวนเต็ม n)
{
    คืน sum_array(a,n)/n;
}

void mean_arrays(float **a, จำนวนเต็ม n, จำนวนเต็ม els, float *avg)
{
    จำนวนเต็ม i;
    จำนวนเต็ม j;
    memset(avg, 0, els*parimaa_na(float));
    สำหรับ(j = 0; j < n; ++j){
        สำหรับ(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    สำหรับ(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, จำนวนเต็ม n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    float sum = 0;
    float mean = mean_array(a, n);
    สำหรับ(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    คืน variance;
}

จำนวนเต็ม constrain_int(จำนวนเต็ม a, จำนวนเต็ม min, จำนวนเต็ม max)
{
    ถ้า (a < min) คืน min;
    ถ้า (a > max) คืน max;
    คืน a;
}

float constrain(float min, float max, float a)
{
    ถ้า (a < min) คืน min;
    ถ้า (a > max) คืน max;
    คืน a;
}

float dist_array(float *a, float *b, จำนวนเต็ม n, จำนวนเต็ม sub)
{
    จำนวนเต็ม i;
    float sum = 0;
    สำหรับ(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    คืน sqrt(sum);
}

float mse_array(float *a, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    float sum = 0;
    สำหรับ(i = 0; i < n; ++i) sum += a[i]*a[i];
    คืน sqrt(sum/n);
}

void normalize_array(float *a, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    สำหรับ(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, จำนวนเต็ม n, float s)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    float sum = 0;
    สำหรับ(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    คืน sqrt(sum);
}

void scale_array(float *a, จำนวนเต็ม n, float s)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

จำนวนเต็ม sample_array(float *a, จำนวนเต็ม n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        r = r - a[i];
        ถ้า (r <= 0) คืน i;
    }
    คืน n-1;
}

จำนวนเต็ม max_int_index(จำนวนเต็ม *a, จำนวนเต็ม n)
{
    ถ้า(n <= 0) คืน -1;
    จำนวนเต็ม i, max_i = 0;
    จำนวนเต็ม max = a[0];
    สำหรับ(i = 1; i < n; ++i){
        ถ้า(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    คืน max_i;
}

จำนวนเต็ม max_index(float *a, จำนวนเต็ม n)
{
    ถ้า(n <= 0) คืน -1;
    จำนวนเต็ม i, max_i = 0;
    float max = a[0];
    สำหรับ(i = 1; i < n; ++i){
        ถ้า(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    คืน max_i;
}

จำนวนเต็ม int_index(จำนวนเต็ม *a, จำนวนเต็ม val, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        ถ้า(a[i] == val) คืน i;
    }
    คืน -1;
}

จำนวนเต็ม rand_int(จำนวนเต็ม min, จำนวนเต็ม max)
{
    ถ้า (max < min){
        จำนวนเต็ม s = min;
        min = max;
        max = s;
    }
    จำนวนเต็ม r = (rand()%(max - min + 1)) + min;
    คืน r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static จำนวนเต็ม haveSpare = 0;
    static dviguu_na rand1, rand2;

    ถ้า(haveSpare)
    {
        haveSpare = 0;
        คืน sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((dviguu_na) RAND_MAX);
    ถ้า(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((dviguu_na) RAND_MAX)) * TWO_PI;

    คืน sqrt(rand1) * cos(rand2);
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
    คืน  ((size_t)(rand()&0xff) << 56) | 
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
    ถ้า(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    คืน ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    ถ้า(rand()%2) คืน scale;
    คืน 1./scale;
}

float **one_hot_encode(float *a, จำนวนเต็ม n, จำนวนเต็ม k)
{
    จำนวนเต็ม i;
    float **t = calloc(n, parimaa_na(float*));
    สำหรับ(i = 0; i < n; ++i){
        t[i] = calloc(k, parimaa_na(float));
        จำนวนเต็ม index = (จำนวนเต็ม)a[i];
        t[i][index] = 1;
    }
    คืน t;
}

