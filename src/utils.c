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
    အကယ်၍ (gettimeofday(&time,NULL)){
        ပြန်ပေး 0;
    }
    ပြန်ပေး (dviguu_na)time.tv_sec + (dviguu_na)time.tv_usec * .000001;
}

ကိန်းပြည့် *read_intlist(var_na *gpu_list, ကိန်းပြည့် *ngpus, ကိန်းပြည့် d)
{
    ကိန်းပြည့် *gpus = 0;
    အကယ်၍(gpu_list){
        ကိန်းပြည့် len = strlen(gpu_list);
        *ngpus = 1;
        ကိန်းပြည့် i;
        အတွက်(i = 0; i < len; ++i){
            အကယ်၍ (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, parimaa_na(ကိန်းပြည့်));
        အတွက်(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } မဟုတ်ပါက {
        gpus = calloc(1, parimaa_na(float));
        *gpus = d;
        *ngpus = 1;
    }
    ပြန်ပေး gpus;
}

ကိန်းပြည့် *read_map(var_na *filename)
{
    ကိန်းပြည့် n = 0;
    ကိန်းပြည့် *map = 0;
    var_na *str;
    FILE *file = fopen(filename, "r");
    အကယ်၍(!file) file_error(filename);
    နေစဉ်((str=fgetl(file))){
        ++n;
        map = realloc(map, n*parimaa_na(ကိန်းပြည့်));
        map[n-1] = atoi(str);
    }
    ပြန်ပေး map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    အတွက်(i = 0; i < sections; ++i){
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
    အတွက်(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

ကိန်းပြည့် *random_index_order(ကိန်းပြည့် min, ကိန်းပြည့် max)
{
    ကိန်းပြည့် *inds = calloc(max-min, parimaa_na(ကိန်းပြည့်));
    ကိန်းပြည့် i;
    အတွက်(i = min; i < max; ++i){
        inds[i] = i;
    }
    အတွက်(i = min; i < max-1; ++i){
        ကိန်းပြည့် swap = inds[i];
        ကိန်းပြည့် index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ပြန်ပေး inds;
}

void del_arg(ကိန်းပြည့် argc, var_na **argv, ကိန်းပြည့် index)
{
    ကိန်းပြည့် i;
    အတွက်(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ကိန်းပြည့် find_arg(ကိန်းပြည့် argc, var_na* argv[], var_na *arg)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < argc; ++i) {
        အကယ်၍(!argv[i]) continue;
        အကယ်၍(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ပြန်ပေး 1;
        }
    }
    ပြန်ပေး 0;
}

ကိန်းပြည့် find_int_arg(ကိန်းပြည့် argc, var_na **argv, var_na *arg, ကိန်းပြည့် def)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < argc-1; ++i){
        အကယ်၍(!argv[i]) continue;
        အကယ်၍(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ပြန်ပေး def;
}

float find_float_arg(ကိန်းပြည့် argc, var_na **argv, var_na *arg, float def)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < argc-1; ++i){
        အကယ်၍(!argv[i]) continue;
        အကယ်၍(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ပြန်ပေး def;
}

var_na *find_char_arg(ကိန်းပြည့် argc, var_na **argv, var_na *arg, var_na *def)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < argc-1; ++i){
        အကယ်၍(!argv[i]) continue;
        အကယ်၍(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ပြန်ပေး def;
}


var_na *basecfg(var_na *cfgfile)
{
    var_na *c = cfgfile;
    var_na *next;
    နေစဉ်((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    အကယ်၍ (next) *next = 0;
    ပြန်ပေး c;
}

ကိန်းပြည့် alphanum_to_int(var_na c)
{
    ပြန်ပေး (c < 58) ? c - 48 : c-87;
}
var_na int_to_alphanum(ကိန်းပြည့် i)
{
    အကယ်၍ (i == 36) ပြန်ပေး '.';
    ပြန်ပေး (i < 10) ? i + 48 : i + 87;
}

void pm(ကိန်းပြည့် M, ကိန်းပြည့် N, float *A)
{
    ကိန်းပြည့် i,j;
    အတွက်(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        အတွက်(j = 0; j < N; ++j){
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
    အကယ်၍(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ပြန်ပေး;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    ပြန်ပေး (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, ကိန်းပြည့် n, ကိန်းပြည့် k, ကိန်းပြည့် *index)
{
    ကိန်းပြည့် i,j;
    အတွက်(j = 0; j < k; ++j) index[j] = -1;
    အတွက်(i = 0; i < n; ++i){
        ကိန်းပြည့် curr = i;
        အတွက်(j = 0; j < k; ++j){
            အကယ်၍((index[j] < 0) || a[curr] > a[index[j]]){
                ကိန်းပြည့် swap = curr;
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
    ပြန်ပေး text;
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
    အတွက်(i = 0; i < len; ++i){
        အကယ်၍(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    ပြန်ပေး l;
}

void strip(var_na *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    အတွက်(i = 0; i < len; ++i){
        var_na c = s[i];
        အကယ်၍(c==' '||c=='\t'||c=='\n') ++offset;
        မဟုတ်ပါက s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(var_na *s, var_na bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    အတွက်(i = 0; i < len; ++i){
        var_na c = s[i];
        အကယ်၍(c==bad) ++offset;
        မဟုတ်ပါက s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

var_na *fgetl(FILE *fp)
{
    အကယ်၍(feof(fp)) ပြန်ပေး 0;
    size_t size = 512;
    var_na *line = malloc(size*parimaa_na(var_na));
    အကယ်၍(!fgets(line, size, fp)){
        free(line);
        ပြန်ပေး 0;
    }

    size_t curr = strlen(line);

    နေစဉ်((line[curr-1] != '\n') && !feof(fp)){
        အကယ်၍(curr == size-1){
            size *= 2;
            line = realloc(line, size*parimaa_na(var_na));
            အကယ်၍(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        အကယ်၍(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    အကယ်၍(line[curr-1] == '\n') line[curr-1] = '\0';

    ပြန်ပေး line;
}

ကိန်းပြည့် read_int(ကိန်းပြည့် fd)
{
    ကိန်းပြည့် n = 0;
    ကိန်းပြည့် next = read(fd, &n, parimaa_na(ကိန်းပြည့်));
    အကယ်၍(next <= 0) ပြန်ပေး -1;
    ပြန်ပေး n;
}

void write_int(ကိန်းပြည့် fd, ကိန်းပြည့် n)
{
    ကိန်းပြည့် next = write(fd, &n, parimaa_na(ကိန်းပြည့်));
    အကယ်၍(next <= 0) error("read failed");
}

ကိန်းပြည့် read_all_fail(ကိန်းပြည့် fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    နေစဉ်(n < bytes){
        ကိန်းပြည့် next = read(fd, buffer + n, bytes-n);
        အကယ်၍(next <= 0) ပြန်ပေး 1;
        n += next;
    }
    ပြန်ပေး 0;
}

ကိန်းပြည့် write_all_fail(ကိန်းပြည့် fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    နေစဉ်(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        အကယ်၍(next <= 0) ပြန်ပေး 1;
        n += next;
    }
    ပြန်ပေး 0;
}

void read_all(ကိန်းပြည့် fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    နေစဉ်(n < bytes){
        ကိန်းပြည့် next = read(fd, buffer + n, bytes-n);
        အကယ်၍(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(ကိန်းပြည့် fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    နေစဉ်(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        အကယ်၍(next <= 0) error("write failed");
        n += next;
    }
}


var_na *copy_string(var_na *s)
{
    var_na *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ပြန်ပေး copy;
}

list *parse_csv_line(var_na *line)
{
    list *l = make_list();
    var_na *c, *p;
    ကိန်းပြည့် in = 0;
    အတွက်(c = line, p = line; *c != '\0'; ++c){
        အကယ်၍(*c == '"') in = !in;
        မဟုတ်ပါက အကယ်၍(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    ပြန်ပေး l;
}

ကိန်းပြည့် count_fields(var_na *line)
{
    ကိန်းပြည့် count = 0;
    ကိန်းပြည့် done = 0;
    var_na *c;
    အတွက်(c = line; !done; ++c){
        done = (*c == '\0');
        အကယ်၍(*c == ',' || done) ++count;
    }
    ပြန်ပေး count;
}

float *parse_fields(var_na *line, ကိန်းပြည့် n)
{
    float *field = calloc(n, parimaa_na(float));
    var_na *c, *p, *end;
    ကိန်းပြည့် count = 0;
    ကိန်းပြည့် done = 0;
    အတွက်(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        အကယ်၍(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            အကယ်၍(p == c) field[count] = nan("");
            အကယ်၍(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    ပြန်ပေး field;
}

float sum_array(float *a, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    float sum = 0;
    အတွက်(i = 0; i < n; ++i) sum += a[i];
    ပြန်ပေး sum;
}

float mean_array(float *a, ကိန်းပြည့် n)
{
    ပြန်ပေး sum_array(a,n)/n;
}

void mean_arrays(float **a, ကိန်းပြည့် n, ကိန်းပြည့် els, float *avg)
{
    ကိန်းပြည့် i;
    ကိန်းပြည့် j;
    memset(avg, 0, els*parimaa_na(float));
    အတွက်(j = 0; j < n; ++j){
        အတွက်(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    အတွက်(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, ကိန်းပြည့် n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    float sum = 0;
    float mean = mean_array(a, n);
    အတွက်(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    ပြန်ပေး variance;
}

ကိန်းပြည့် constrain_int(ကိန်းပြည့် a, ကိန်းပြည့် min, ကိန်းပြည့် max)
{
    အကယ်၍ (a < min) ပြန်ပေး min;
    အကယ်၍ (a > max) ပြန်ပေး max;
    ပြန်ပေး a;
}

float constrain(float min, float max, float a)
{
    အကယ်၍ (a < min) ပြန်ပေး min;
    အကယ်၍ (a > max) ပြန်ပေး max;
    ပြန်ပေး a;
}

float dist_array(float *a, float *b, ကိန်းပြည့် n, ကိန်းပြည့် sub)
{
    ကိန်းပြည့် i;
    float sum = 0;
    အတွက်(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ပြန်ပေး sqrt(sum);
}

float mse_array(float *a, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    float sum = 0;
    အတွက်(i = 0; i < n; ++i) sum += a[i]*a[i];
    ပြန်ပေး sqrt(sum/n);
}

void normalize_array(float *a, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    အတွက်(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, ကိန်းပြည့် n, float s)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    float sum = 0;
    အတွက်(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ပြန်ပေး sqrt(sum);
}

void scale_array(float *a, ကိန်းပြည့် n, float s)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ကိန်းပြည့် sample_array(float *a, ကိန်းပြည့် n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        r = r - a[i];
        အကယ်၍ (r <= 0) ပြန်ပေး i;
    }
    ပြန်ပေး n-1;
}

ကိန်းပြည့် max_int_index(ကိန်းပြည့် *a, ကိန်းပြည့် n)
{
    အကယ်၍(n <= 0) ပြန်ပေး -1;
    ကိန်းပြည့် i, max_i = 0;
    ကိန်းပြည့် max = a[0];
    အတွက်(i = 1; i < n; ++i){
        အကယ်၍(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ပြန်ပေး max_i;
}

ကိန်းပြည့် max_index(float *a, ကိန်းပြည့် n)
{
    အကယ်၍(n <= 0) ပြန်ပေး -1;
    ကိန်းပြည့် i, max_i = 0;
    float max = a[0];
    အတွက်(i = 1; i < n; ++i){
        အကယ်၍(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ပြန်ပေး max_i;
}

ကိန်းပြည့် int_index(ကိန်းပြည့် *a, ကိန်းပြည့် val, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        အကယ်၍(a[i] == val) ပြန်ပေး i;
    }
    ပြန်ပေး -1;
}

ကိန်းပြည့် rand_int(ကိန်းပြည့် min, ကိန်းပြည့် max)
{
    အကယ်၍ (max < min){
        ကိန်းပြည့် s = min;
        min = max;
        max = s;
    }
    ကိန်းပြည့် r = (rand()%(max - min + 1)) + min;
    ပြန်ပေး r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static ကိန်းပြည့် haveSpare = 0;
    static dviguu_na rand1, rand2;

    အကယ်၍(haveSpare)
    {
        haveSpare = 0;
        ပြန်ပေး sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((dviguu_na) RAND_MAX);
    အကယ်၍(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((dviguu_na) RAND_MAX)) * TWO_PI;

    ပြန်ပေး sqrt(rand1) * cos(rand2);
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
    ပြန်ပေး  ((size_t)(rand()&0xff) << 56) | 
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
    အကယ်၍(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    ပြန်ပေး ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    အကယ်၍(rand()%2) ပြန်ပေး scale;
    ပြန်ပေး 1./scale;
}

float **one_hot_encode(float *a, ကိန်းပြည့် n, ကိန်းပြည့် k)
{
    ကိန်းပြည့် i;
    float **t = calloc(n, parimaa_na(float*));
    အတွက်(i = 0; i < n; ++i){
        t[i] = calloc(k, parimaa_na(float));
        ကိန်းပြည့် index = (ကိန်းပြည့်)a[i];
        t[i][index] = 1;
    }
    ပြန်ပေး t;
}

