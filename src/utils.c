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
    ပေႃး (gettimeofday(&time,NULL)){
        ပွၵ်ႈ 0;
    }
    ပွၵ်ႈ (dviguu_na)time.tv_sec + (dviguu_na)time.tv_usec * .000001;
}

puur_naa_nka *read_intlist(var_na *gpu_list, puur_naa_nka *ngpus, puur_naa_nka d)
{
    puur_naa_nka *gpus = 0;
    ပေႃး(gpu_list){
        puur_naa_nka len = strlen(gpu_list);
        *ngpus = 1;
        puur_naa_nka i;
        for(i = 0; i < len; ++i){
            ပေႃး (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, parimaa_na(puur_naa_nka));
        for(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } ပေႃးဢမ်ႇ {
        gpus = calloc(1, parimaa_na(float));
        *gpus = d;
        *ngpus = 1;
    }
    ပွၵ်ႈ gpus;
}

puur_naa_nka *read_map(var_na *filename)
{
    puur_naa_nka n = 0;
    puur_naa_nka *map = 0;
    var_na *str;
    FILE *file = fopen(filename, "r");
    ပေႃး(!file) file_error(filename);
    ၶၢဝ်း((str=fgetl(file))){
        ++n;
        map = realloc(map, n*parimaa_na(puur_naa_nka));
        map[n-1] = atoi(str);
    }
    ပွၵ်ႈ map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    for(i = 0; i < sections; ++i){
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
    for(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

puur_naa_nka *random_index_order(puur_naa_nka min, puur_naa_nka max)
{
    puur_naa_nka *inds = calloc(max-min, parimaa_na(puur_naa_nka));
    puur_naa_nka i;
    for(i = min; i < max; ++i){
        inds[i] = i;
    }
    for(i = min; i < max-1; ++i){
        puur_naa_nka swap = inds[i];
        puur_naa_nka index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ပွၵ်ႈ inds;
}

void del_arg(puur_naa_nka argc, var_na **argv, puur_naa_nka index)
{
    puur_naa_nka i;
    for(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

puur_naa_nka find_arg(puur_naa_nka argc, var_na* argv[], var_na *arg)
{
    puur_naa_nka i;
    for(i = 0; i < argc; ++i) {
        ပေႃး(!argv[i]) continue;
        ပေႃး(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ပွၵ်ႈ 1;
        }
    }
    ပွၵ်ႈ 0;
}

puur_naa_nka find_int_arg(puur_naa_nka argc, var_na **argv, var_na *arg, puur_naa_nka def)
{
    puur_naa_nka i;
    for(i = 0; i < argc-1; ++i){
        ပေႃး(!argv[i]) continue;
        ပေႃး(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ပွၵ်ႈ def;
}

float find_float_arg(puur_naa_nka argc, var_na **argv, var_na *arg, float def)
{
    puur_naa_nka i;
    for(i = 0; i < argc-1; ++i){
        ပေႃး(!argv[i]) continue;
        ပေႃး(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ပွၵ်ႈ def;
}

var_na *find_char_arg(puur_naa_nka argc, var_na **argv, var_na *arg, var_na *def)
{
    puur_naa_nka i;
    for(i = 0; i < argc-1; ++i){
        ပေႃး(!argv[i]) continue;
        ပေႃး(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ပွၵ်ႈ def;
}


var_na *basecfg(var_na *cfgfile)
{
    var_na *c = cfgfile;
    var_na *next;
    ၶၢဝ်း((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    ပေႃး (next) *next = 0;
    ပွၵ်ႈ c;
}

puur_naa_nka alphanum_to_int(var_na c)
{
    ပွၵ်ႈ (c < 58) ? c - 48 : c-87;
}
var_na int_to_alphanum(puur_naa_nka i)
{
    ပေႃး (i == 36) ပွၵ်ႈ '.';
    ပွၵ်ႈ (i < 10) ? i + 48 : i + 87;
}

void pm(puur_naa_nka M, puur_naa_nka N, float *A)
{
    puur_naa_nka i,j;
    for(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        for(j = 0; j < N; ++j){
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
    ပေႃး(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ပွၵ်ႈ;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    ပွၵ်ႈ (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, puur_naa_nka n, puur_naa_nka k, puur_naa_nka *index)
{
    puur_naa_nka i,j;
    for(j = 0; j < k; ++j) index[j] = -1;
    for(i = 0; i < n; ++i){
        puur_naa_nka curr = i;
        for(j = 0; j < k; ++j){
            ပေႃး((index[j] < 0) || a[curr] > a[index[j]]){
                puur_naa_nka swap = curr;
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
    ပွၵ်ႈ text;
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
    for(i = 0; i < len; ++i){
        ပေႃး(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    ပွၵ်ႈ l;
}

void strip(var_na *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    for(i = 0; i < len; ++i){
        var_na c = s[i];
        ပေႃး(c==' '||c=='\t'||c=='\n') ++offset;
        ပေႃးဢမ်ႇ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(var_na *s, var_na bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    for(i = 0; i < len; ++i){
        var_na c = s[i];
        ပေႃး(c==bad) ++offset;
        ပေႃးဢမ်ႇ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, puur_naa_nka n)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

var_na *fgetl(FILE *fp)
{
    ပေႃး(feof(fp)) ပွၵ်ႈ 0;
    size_t size = 512;
    var_na *line = malloc(size*parimaa_na(var_na));
    ပေႃး(!fgets(line, size, fp)){
        free(line);
        ပွၵ်ႈ 0;
    }

    size_t curr = strlen(line);

    ၶၢဝ်း((line[curr-1] != '\n') && !feof(fp)){
        ပေႃး(curr == size-1){
            size *= 2;
            line = realloc(line, size*parimaa_na(var_na));
            ပေႃး(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        ပေႃး(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    ပေႃး(line[curr-1] == '\n') line[curr-1] = '\0';

    ပွၵ်ႈ line;
}

puur_naa_nka read_int(puur_naa_nka fd)
{
    puur_naa_nka n = 0;
    puur_naa_nka next = read(fd, &n, parimaa_na(puur_naa_nka));
    ပေႃး(next <= 0) ပွၵ်ႈ -1;
    ပွၵ်ႈ n;
}

void write_int(puur_naa_nka fd, puur_naa_nka n)
{
    puur_naa_nka next = write(fd, &n, parimaa_na(puur_naa_nka));
    ပေႃး(next <= 0) error("read failed");
}

puur_naa_nka read_all_fail(puur_naa_nka fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ၶၢဝ်း(n < bytes){
        puur_naa_nka next = read(fd, buffer + n, bytes-n);
        ပေႃး(next <= 0) ပွၵ်ႈ 1;
        n += next;
    }
    ပွၵ်ႈ 0;
}

puur_naa_nka write_all_fail(puur_naa_nka fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ၶၢဝ်း(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ပေႃး(next <= 0) ပွၵ်ႈ 1;
        n += next;
    }
    ပွၵ်ႈ 0;
}

void read_all(puur_naa_nka fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ၶၢဝ်း(n < bytes){
        puur_naa_nka next = read(fd, buffer + n, bytes-n);
        ပေႃး(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(puur_naa_nka fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ၶၢဝ်း(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ပေႃး(next <= 0) error("write failed");
        n += next;
    }
}


var_na *copy_string(var_na *s)
{
    var_na *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ပွၵ်ႈ copy;
}

list *parse_csv_line(var_na *line)
{
    list *l = make_list();
    var_na *c, *p;
    puur_naa_nka in = 0;
    for(c = line, p = line; *c != '\0'; ++c){
        ပေႃး(*c == '"') in = !in;
        ပေႃးဢမ်ႇ ပေႃး(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    ပွၵ်ႈ l;
}

puur_naa_nka count_fields(var_na *line)
{
    puur_naa_nka count = 0;
    puur_naa_nka done = 0;
    var_na *c;
    for(c = line; !done; ++c){
        done = (*c == '\0');
        ပေႃး(*c == ',' || done) ++count;
    }
    ပွၵ်ႈ count;
}

float *parse_fields(var_na *line, puur_naa_nka n)
{
    float *field = calloc(n, parimaa_na(float));
    var_na *c, *p, *end;
    puur_naa_nka count = 0;
    puur_naa_nka done = 0;
    for(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        ပေႃး(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            ပေႃး(p == c) field[count] = nan("");
            ပေႃး(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    ပွၵ်ႈ field;
}

float sum_array(float *a, puur_naa_nka n)
{
    puur_naa_nka i;
    float sum = 0;
    for(i = 0; i < n; ++i) sum += a[i];
    ပွၵ်ႈ sum;
}

float mean_array(float *a, puur_naa_nka n)
{
    ပွၵ်ႈ sum_array(a,n)/n;
}

void mean_arrays(float **a, puur_naa_nka n, puur_naa_nka els, float *avg)
{
    puur_naa_nka i;
    puur_naa_nka j;
    memset(avg, 0, els*parimaa_na(float));
    for(j = 0; j < n; ++j){
        for(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    for(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, puur_naa_nka n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, puur_naa_nka n)
{
    puur_naa_nka i;
    float sum = 0;
    float mean = mean_array(a, n);
    for(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    ပွၵ်ႈ variance;
}

puur_naa_nka constrain_int(puur_naa_nka a, puur_naa_nka min, puur_naa_nka max)
{
    ပေႃး (a < min) ပွၵ်ႈ min;
    ပေႃး (a > max) ပွၵ်ႈ max;
    ပွၵ်ႈ a;
}

float constrain(float min, float max, float a)
{
    ပေႃး (a < min) ပွၵ်ႈ min;
    ပေႃး (a > max) ပွၵ်ႈ max;
    ပွၵ်ႈ a;
}

float dist_array(float *a, float *b, puur_naa_nka n, puur_naa_nka sub)
{
    puur_naa_nka i;
    float sum = 0;
    for(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ပွၵ်ႈ sqrt(sum);
}

float mse_array(float *a, puur_naa_nka n)
{
    puur_naa_nka i;
    float sum = 0;
    for(i = 0; i < n; ++i) sum += a[i]*a[i];
    ပွၵ်ႈ sqrt(sum/n);
}

void normalize_array(float *a, puur_naa_nka n)
{
    puur_naa_nka i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    for(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, puur_naa_nka n, float s)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, puur_naa_nka n)
{
    puur_naa_nka i;
    float sum = 0;
    for(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ပွၵ်ႈ sqrt(sum);
}

void scale_array(float *a, puur_naa_nka n, float s)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

puur_naa_nka sample_array(float *a, puur_naa_nka n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        r = r - a[i];
        ပေႃး (r <= 0) ပွၵ်ႈ i;
    }
    ပွၵ်ႈ n-1;
}

puur_naa_nka max_int_index(puur_naa_nka *a, puur_naa_nka n)
{
    ပေႃး(n <= 0) ပွၵ်ႈ -1;
    puur_naa_nka i, max_i = 0;
    puur_naa_nka max = a[0];
    for(i = 1; i < n; ++i){
        ပေႃး(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ပွၵ်ႈ max_i;
}

puur_naa_nka max_index(float *a, puur_naa_nka n)
{
    ပေႃး(n <= 0) ပွၵ်ႈ -1;
    puur_naa_nka i, max_i = 0;
    float max = a[0];
    for(i = 1; i < n; ++i){
        ပေႃး(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ပွၵ်ႈ max_i;
}

puur_naa_nka int_index(puur_naa_nka *a, puur_naa_nka val, puur_naa_nka n)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        ပေႃး(a[i] == val) ပွၵ်ႈ i;
    }
    ပွၵ်ႈ -1;
}

puur_naa_nka rand_int(puur_naa_nka min, puur_naa_nka max)
{
    ပေႃး (max < min){
        puur_naa_nka s = min;
        min = max;
        max = s;
    }
    puur_naa_nka r = (rand()%(max - min + 1)) + min;
    ပွၵ်ႈ r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static puur_naa_nka haveSpare = 0;
    static dviguu_na rand1, rand2;

    ပေႃး(haveSpare)
    {
        haveSpare = 0;
        ပွၵ်ႈ sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((dviguu_na) RAND_MAX);
    ပေႃး(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((dviguu_na) RAND_MAX)) * TWO_PI;

    ပွၵ်ႈ sqrt(rand1) * cos(rand2);
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
    ပွၵ်ႈ  ((size_t)(rand()&0xff) << 56) | 
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
    ပေႃး(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    ပွၵ်ႈ ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    ပေႃး(rand()%2) ပွၵ်ႈ scale;
    ပွၵ်ႈ 1./scale;
}

float **one_hot_encode(float *a, puur_naa_nka n, puur_naa_nka k)
{
    puur_naa_nka i;
    float **t = calloc(n, parimaa_na(float*));
    for(i = 0; i < n; ++i){
        t[i] = calloc(k, parimaa_na(float));
        puur_naa_nka index = (puur_naa_nka)a[i];
        t[i][index] = 1;
    }
    ပွၵ်ႈ t;
}

