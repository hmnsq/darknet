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
    ຖ້າ (gettimeofday(&time,NULL)){
        ສົ່ງຄືນ 0;
    }
    ສົ່ງຄືນ (dviguu_na)time.tv_sec + (dviguu_na)time.tv_usec * .000001;
}

ຈຳນວນເຕັມ *read_intlist(var_na *gpu_list, ຈຳນວນເຕັມ *ngpus, ຈຳນວນເຕັມ d)
{
    ຈຳນວນເຕັມ *gpus = 0;
    ຖ້າ(gpu_list){
        ຈຳນວນເຕັມ len = strlen(gpu_list);
        *ngpus = 1;
        ຈຳນວນເຕັມ i;
        ສຳລັບ(i = 0; i < len; ++i){
            ຖ້າ (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, parimaa_na(ຈຳນວນເຕັມ));
        ສຳລັບ(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } ບໍ່ດັ່ງນັ້ນ {
        gpus = calloc(1, parimaa_na(float));
        *gpus = d;
        *ngpus = 1;
    }
    ສົ່ງຄືນ gpus;
}

ຈຳນວນເຕັມ *read_map(var_na *filename)
{
    ຈຳນວນເຕັມ n = 0;
    ຈຳນວນເຕັມ *map = 0;
    var_na *str;
    FILE *file = fopen(filename, "r");
    ຖ້າ(!file) file_error(filename);
    ໃນຂະນະທີ່((str=fgetl(file))){
        ++n;
        map = realloc(map, n*parimaa_na(ຈຳນວນເຕັມ));
        map[n-1] = atoi(str);
    }
    ສົ່ງຄືນ map;
}

void sorta_shuffle(void *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    ສຳລັບ(i = 0; i < sections; ++i){
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
    ສຳລັບ(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

ຈຳນວນເຕັມ *random_index_order(ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max)
{
    ຈຳນວນເຕັມ *inds = calloc(max-min, parimaa_na(ຈຳນວນເຕັມ));
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = min; i < max; ++i){
        inds[i] = i;
    }
    ສຳລັບ(i = min; i < max-1; ++i){
        ຈຳນວນເຕັມ swap = inds[i];
        ຈຳນວນເຕັມ index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ສົ່ງຄືນ inds;
}

void del_arg(ຈຳນວນເຕັມ argc, var_na **argv, ຈຳນວນເຕັມ index)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ຈຳນວນເຕັມ find_arg(ຈຳນວນເຕັມ argc, var_na* argv[], var_na *arg)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < argc; ++i) {
        ຖ້າ(!argv[i]) continue;
        ຖ້າ(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ສົ່ງຄືນ 1;
        }
    }
    ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ find_int_arg(ຈຳນວນເຕັມ argc, var_na **argv, var_na *arg, ຈຳນວນເຕັມ def)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < argc-1; ++i){
        ຖ້າ(!argv[i]) continue;
        ຖ້າ(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ສົ່ງຄືນ def;
}

float find_float_arg(ຈຳນວນເຕັມ argc, var_na **argv, var_na *arg, float def)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < argc-1; ++i){
        ຖ້າ(!argv[i]) continue;
        ຖ້າ(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ສົ່ງຄືນ def;
}

var_na *find_char_arg(ຈຳນວນເຕັມ argc, var_na **argv, var_na *arg, var_na *def)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < argc-1; ++i){
        ຖ້າ(!argv[i]) continue;
        ຖ້າ(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            bha_nga;
        }
    }
    ສົ່ງຄືນ def;
}


var_na *basecfg(var_na *cfgfile)
{
    var_na *c = cfgfile;
    var_na *next;
    ໃນຂະນະທີ່((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    ຖ້າ (next) *next = 0;
    ສົ່ງຄືນ c;
}

ຈຳນວນເຕັມ alphanum_to_int(var_na c)
{
    ສົ່ງຄືນ (c < 58) ? c - 48 : c-87;
}
var_na int_to_alphanum(ຈຳນວນເຕັມ i)
{
    ຖ້າ (i == 36) ສົ່ງຄືນ '.';
    ສົ່ງຄືນ (i < 10) ? i + 48 : i + 87;
}

void pm(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, float *A)
{
    ຈຳນວນເຕັມ i,j;
    ສຳລັບ(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        ສຳລັບ(j = 0; j < N; ++j){
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
    ຖ້າ(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ສົ່ງຄືນ;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

float sec(clock_t clocks)
{
    ສົ່ງຄືນ (float)clocks/CLOCKS_PER_SEC;
}

void top_k(float *a, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ *index)
{
    ຈຳນວນເຕັມ i,j;
    ສຳລັບ(j = 0; j < k; ++j) index[j] = -1;
    ສຳລັບ(i = 0; i < n; ++i){
        ຈຳນວນເຕັມ curr = i;
        ສຳລັບ(j = 0; j < k; ++j){
            ຖ້າ((index[j] < 0) || a[curr] > a[index[j]]){
                ຈຳນວນເຕັມ swap = curr;
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
    ສົ່ງຄືນ text;
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
    ສຳລັບ(i = 0; i < len; ++i){
        ຖ້າ(s[i] == delim){
            s[i] = '\0';
            list_insert(l, &(s[i+1]));
        }
    }
    ສົ່ງຄືນ l;
}

void strip(var_na *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ສຳລັບ(i = 0; i < len; ++i){
        var_na c = s[i];
        ຖ້າ(c==' '||c=='\t'||c=='\n') ++offset;
        ບໍ່ດັ່ງນັ້ນ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_char(var_na *s, var_na bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ສຳລັບ(i = 0; i < len; ++i){
        var_na c = s[i];
        ຖ້າ(c==bad) ++offset;
        ບໍ່ດັ່ງນັ້ນ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void free_ptrs(void **ptrs, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i) free(ptrs[i]);
    free(ptrs);
}

var_na *fgetl(FILE *fp)
{
    ຖ້າ(feof(fp)) ສົ່ງຄືນ 0;
    size_t size = 512;
    var_na *line = malloc(size*parimaa_na(var_na));
    ຖ້າ(!fgets(line, size, fp)){
        free(line);
        ສົ່ງຄືນ 0;
    }

    size_t curr = strlen(line);

    ໃນຂະນະທີ່((line[curr-1] != '\n') && !feof(fp)){
        ຖ້າ(curr == size-1){
            size *= 2;
            line = realloc(line, size*parimaa_na(var_na));
            ຖ້າ(!line) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        ຖ້າ(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&line[curr], readsize, fp);
        curr = strlen(line);
    }
    ຖ້າ(line[curr-1] == '\n') line[curr-1] = '\0';

    ສົ່ງຄືນ line;
}

ຈຳນວນເຕັມ read_int(ຈຳນວນເຕັມ fd)
{
    ຈຳນວນເຕັມ n = 0;
    ຈຳນວນເຕັມ next = read(fd, &n, parimaa_na(ຈຳນວນເຕັມ));
    ຖ້າ(next <= 0) ສົ່ງຄືນ -1;
    ສົ່ງຄືນ n;
}

void write_int(ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ next = write(fd, &n, parimaa_na(ຈຳນວນເຕັມ));
    ຖ້າ(next <= 0) error("read failed");
}

ຈຳນວນເຕັມ read_all_fail(ຈຳນວນເຕັມ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ໃນຂະນະທີ່(n < bytes){
        ຈຳນວນເຕັມ next = read(fd, buffer + n, bytes-n);
        ຖ້າ(next <= 0) ສົ່ງຄືນ 1;
        n += next;
    }
    ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ write_all_fail(ຈຳນວນເຕັມ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ໃນຂະນະທີ່(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ຖ້າ(next <= 0) ສົ່ງຄືນ 1;
        n += next;
    }
    ສົ່ງຄືນ 0;
}

void read_all(ຈຳນວນເຕັມ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ໃນຂະນະທີ່(n < bytes){
        ຈຳນວນເຕັມ next = read(fd, buffer + n, bytes-n);
        ຖ້າ(next <= 0) error("read failed");
        n += next;
    }
}

void write_all(ຈຳນວນເຕັມ fd, var_na *buffer, size_t bytes)
{
    size_t n = 0;
    ໃນຂະນະທີ່(n < bytes){
        size_t next = write(fd, buffer + n, bytes-n);
        ຖ້າ(next <= 0) error("write failed");
        n += next;
    }
}


var_na *copy_string(var_na *s)
{
    var_na *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ສົ່ງຄືນ copy;
}

list *parse_csv_line(var_na *line)
{
    list *l = make_list();
    var_na *c, *p;
    ຈຳນວນເຕັມ in = 0;
    ສຳລັບ(c = line, p = line; *c != '\0'; ++c){
        ຖ້າ(*c == '"') in = !in;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ(*c == ',' && !in){
            *c = '\0';
            list_insert(l, copy_string(p));
            p = c+1;
        }
    }
    list_insert(l, copy_string(p));
    ສົ່ງຄືນ l;
}

ຈຳນວນເຕັມ count_fields(var_na *line)
{
    ຈຳນວນເຕັມ count = 0;
    ຈຳນວນເຕັມ done = 0;
    var_na *c;
    ສຳລັບ(c = line; !done; ++c){
        done = (*c == '\0');
        ຖ້າ(*c == ',' || done) ++count;
    }
    ສົ່ງຄືນ count;
}

float *parse_fields(var_na *line, ຈຳນວນເຕັມ n)
{
    float *field = calloc(n, parimaa_na(float));
    var_na *c, *p, *end;
    ຈຳນວນເຕັມ count = 0;
    ຈຳນວນເຕັມ done = 0;
    ສຳລັບ(c = line, p = line; !done; ++c){
        done = (*c == '\0');
        ຖ້າ(*c == ',' || done){
            *c = '\0';
            field[count] = strtod(p, &end);
            ຖ້າ(p == c) field[count] = nan("");
            ຖ້າ(end != c && (end != c-1 || *end != '\r')) field[count] = nan(""); //DOS file formats!
            p = c+1;
            ++count;
        }
    }
    ສົ່ງຄືນ field;
}

float sum_array(float *a, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    float sum = 0;
    ສຳລັບ(i = 0; i < n; ++i) sum += a[i];
    ສົ່ງຄືນ sum;
}

float mean_array(float *a, ຈຳນວນເຕັມ n)
{
    ສົ່ງຄືນ sum_array(a,n)/n;
}

void mean_arrays(float **a, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ els, float *avg)
{
    ຈຳນວນເຕັມ i;
    ຈຳນວນເຕັມ j;
    memset(avg, 0, els*parimaa_na(float));
    ສຳລັບ(j = 0; j < n; ++j){
        ສຳລັບ(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    ສຳລັບ(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

void print_statistics(float *a, ຈຳນວນເຕັມ n)
{
    float m = mean_array(a, n);
    float v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

float variance_array(float *a, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    float sum = 0;
    float mean = mean_array(a, n);
    ສຳລັບ(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    float variance = sum/n;
    ສົ່ງຄືນ variance;
}

ຈຳນວນເຕັມ constrain_int(ຈຳນວນເຕັມ a, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max)
{
    ຖ້າ (a < min) ສົ່ງຄືນ min;
    ຖ້າ (a > max) ສົ່ງຄືນ max;
    ສົ່ງຄືນ a;
}

float constrain(float min, float max, float a)
{
    ຖ້າ (a < min) ສົ່ງຄືນ min;
    ຖ້າ (a > max) ສົ່ງຄືນ max;
    ສົ່ງຄືນ a;
}

float dist_array(float *a, float *b, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ sub)
{
    ຈຳນວນເຕັມ i;
    float sum = 0;
    ສຳລັບ(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ສົ່ງຄືນ sqrt(sum);
}

float mse_array(float *a, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    float sum = 0;
    ສຳລັບ(i = 0; i < n; ++i) sum += a[i]*a[i];
    ສົ່ງຄືນ sqrt(sum/n);
}

void normalize_array(float *a, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    float mu = mean_array(a,n);
    float sigma = sqrt(variance_array(a,n));
    ສຳລັບ(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

void translate_array(float *a, ຈຳນວນເຕັມ n, float s)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        a[i] += s;
    }
}

float mag_array(float *a, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    float sum = 0;
    ສຳລັບ(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ສົ່ງຄືນ sqrt(sum);
}

void scale_array(float *a, ຈຳນວນເຕັມ n, float s)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ຈຳນວນເຕັມ sample_array(float *a, ຈຳນວນເຕັມ n)
{
    float sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    float r = rand_uniform(0, 1);
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        r = r - a[i];
        ຖ້າ (r <= 0) ສົ່ງຄືນ i;
    }
    ສົ່ງຄືນ n-1;
}

ຈຳນວນເຕັມ max_int_index(ຈຳນວນເຕັມ *a, ຈຳນວນເຕັມ n)
{
    ຖ້າ(n <= 0) ສົ່ງຄືນ -1;
    ຈຳນວນເຕັມ i, max_i = 0;
    ຈຳນວນເຕັມ max = a[0];
    ສຳລັບ(i = 1; i < n; ++i){
        ຖ້າ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ສົ່ງຄືນ max_i;
}

ຈຳນວນເຕັມ max_index(float *a, ຈຳນວນເຕັມ n)
{
    ຖ້າ(n <= 0) ສົ່ງຄືນ -1;
    ຈຳນວນເຕັມ i, max_i = 0;
    float max = a[0];
    ສຳລັບ(i = 1; i < n; ++i){
        ຖ້າ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ສົ່ງຄືນ max_i;
}

ຈຳນວນເຕັມ int_index(ຈຳນວນເຕັມ *a, ຈຳນວນເຕັມ val, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        ຖ້າ(a[i] == val) ສົ່ງຄືນ i;
    }
    ສົ່ງຄືນ -1;
}

ຈຳນວນເຕັມ rand_int(ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max)
{
    ຖ້າ (max < min){
        ຈຳນວນເຕັມ s = min;
        min = max;
        max = s;
    }
    ຈຳນວນເຕັມ r = (rand()%(max - min + 1)) + min;
    ສົ່ງຄືນ r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
float rand_normal()
{
    static ຈຳນວນເຕັມ haveSpare = 0;
    static dviguu_na rand1, rand2;

    ຖ້າ(haveSpare)
    {
        haveSpare = 0;
        ສົ່ງຄືນ sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((dviguu_na) RAND_MAX);
    ຖ້າ(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((dviguu_na) RAND_MAX)) * TWO_PI;

    ສົ່ງຄືນ sqrt(rand1) * cos(rand2);
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
    ສົ່ງຄືນ  ((size_t)(rand()&0xff) << 56) | 
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
    ຖ້າ(max < min){
        float swap = min;
        min = max;
        max = swap;
    }
    ສົ່ງຄືນ ((float)rand()/RAND_MAX * (max - min)) + min;
}

float rand_scale(float s)
{
    float scale = rand_uniform(1, s);
    ຖ້າ(rand()%2) ສົ່ງຄືນ scale;
    ສົ່ງຄືນ 1./scale;
}

float **one_hot_encode(float *a, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ k)
{
    ຈຳນວນເຕັມ i;
    float **t = calloc(n, parimaa_na(float*));
    ສຳລັບ(i = 0; i < n; ++i){
        t[i] = calloc(k, parimaa_na(float));
        ຈຳນວນເຕັມ index = (ຈຳນວນເຕັມ)a[i];
        t[i][index] = 1;
    }
    ສົ່ງຄືນ t;
}

