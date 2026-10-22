#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <sys/time.h>

#અન્તર્ભાવ "utils.h"


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

દ્વિગુણ what_time_is_it_now()
{
    સંરચના timeval time;
    જો (gettimeofday(&time,ખાલી)){
        પાછા 0;
    }
    પાછા (દ્વિગુણ)time.tv_sec + (દ્વિગુણ)time.tv_usec * .000001;
}

પૂર્ણાંક *read_intlist(અક્ષર *gpu_list, પૂર્ણાંક *ngpus, પૂર્ણાંક d)
{
    પૂર્ણાંક *gpus = 0;
    જો(gpu_list){
        પૂર્ણાંક len = strlen(gpu_list);
        *ngpus = 1;
        પૂર્ણાંક i;
        માટે(i = 0; i < len; ++i){
            જો (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, કદ(પૂર્ણાંક));
        માટે(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } નહીંતર {
        gpus = calloc(1, કદ(દશાંશ));
        *gpus = d;
        *ngpus = 1;
    }
    પાછા gpus;
}

પૂર્ણાંક *read_map(અક્ષર *filename)
{
    પૂર્ણાંક n = 0;
    પૂર્ણાંક *map = 0;
    અક્ષર *str;
    FILE *file = fopen(filename, "r");
    જો(!file) file_error(filename);
    જ્યારે((str=fgetl(file))){
        ++n;
        map = realloc(map, n*કદ(પૂર્ણાંક));
        map[n-1] = atoi(str);
    }
    પાછા map;
}

શૂન્ય sorta_shuffle(શૂન્ય *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    માટે(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t અંક = end-start;
        shuffle(arr+(start*size), અંક, size);
    }
}

શૂન્ય shuffle(શૂન્ય *arr, size_t n, size_t size)
{
    size_t i;
    શૂન્ય *swp = calloc(1, size);
    માટે(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

પૂર્ણાંક *random_index_order(પૂર્ણાંક min, પૂર્ણાંક max)
{
    પૂર્ણાંક *inds = calloc(max-min, કદ(પૂર્ણાંક));
    પૂર્ણાંક i;
    માટે(i = min; i < max; ++i){
        inds[i] = i;
    }
    માટે(i = min; i < max-1; ++i){
        પૂર્ણાંક swap = inds[i];
        પૂર્ણાંક index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    પાછા inds;
}

શૂન્ય del_arg(પૂર્ણાંક argc, અક્ષર **argv, પૂર્ણાંક index)
{
    પૂર્ણાંક i;
    માટે(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

પૂર્ણાંક find_arg(પૂર્ણાંક argc, અક્ષર* argv[], અક્ષર *arg)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < argc; ++i) {
        જો(!argv[i]) ચાલુ;
        જો(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            પાછા 1;
        }
    }
    પાછા 0;
}

પૂર્ણાંક find_int_arg(પૂર્ણાંક argc, અક્ષર **argv, અક્ષર *arg, પૂર્ણાંક def)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < argc-1; ++i){
        જો(!argv[i]) ચાલુ;
        જો(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            તોડો;
        }
    }
    પાછા def;
}

દશાંશ find_float_arg(પૂર્ણાંક argc, અક્ષર **argv, અક્ષર *arg, દશાંશ def)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < argc-1; ++i){
        જો(!argv[i]) ચાલુ;
        જો(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            તોડો;
        }
    }
    પાછા def;
}

અક્ષર *find_char_arg(પૂર્ણાંક argc, અક્ષર **argv, અક્ષર *arg, અક્ષર *def)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < argc-1; ++i){
        જો(!argv[i]) ચાલુ;
        જો(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            તોડો;
        }
    }
    પાછા def;
}


અક્ષર *basecfg(અક્ષર *cfgfile)
{
    અક્ષર *c = cfgfile;
    અક્ષર *next;
    જ્યારે((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    જો (next) *next = 0;
    પાછા c;
}

પૂર્ણાંક alphanum_to_int(અક્ષર c)
{
    પાછા (c < 58) ? c - 48 : c-87;
}
અક્ષર int_to_alphanum(પૂર્ણાંક i)
{
    જો (i == 36) પાછા '.';
    પાછા (i < 10) ? i + 48 : i + 87;
}

શૂન્ય pm(પૂર્ણાંક M, પૂર્ણાંક N, દશાંશ *A)
{
    પૂર્ણાંક i,j;
    માટે(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        માટે(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

શૂન્ય find_replace(અક્ષર *str, અક્ષર *orig, અક્ષર *rep, અક્ષર *output)
{
    અક્ષર buffer[4096] = {0};
    અક્ષર *p;

    sprintf(buffer, "%s", str);
    જો(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        પાછા;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

દશાંશ sec(clock_t clocks)
{
    પાછા (દશાંશ)clocks/CLOCKS_PER_SEC;
}

શૂન્ય ટોચ_k(દશાંશ *a, પૂર્ણાંક n, પૂર્ણાંક k, પૂર્ણાંક *index)
{
    પૂર્ણાંક i,j;
    માટે(j = 0; j < k; ++j) index[j] = -1;
    માટે(i = 0; i < n; ++i){
        પૂર્ણાંક curr = i;
        માટે(j = 0; j < k; ++j){
            જો((index[j] < 0) || a[curr] > a[index[j]]){
                પૂર્ણાંક swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

શૂન્ય error(અચલ અક્ષર *s)
{
    perror(s);
    પ્રતિજ્ઞા(0);
    exit(-1);
}

unsigned અક્ષર *read_file(અક્ષર *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned અક્ષર *text = calloc(size+1, કદ(અક્ષર));
    fread(text, 1, size, fp);
    fclose(fp);
    પાછા text;
}

શૂન્ય malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

શૂન્ય file_error(અક્ષર *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(અક્ષર *s, અક્ષર delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_ઉમેરો(l, s);
    માટે(i = 0; i < len; ++i){
        જો(s[i] == delim){
            s[i] = '\0';
            list_ઉમેરો(l, &(s[i+1]));
        }
    }
    પાછા l;
}

શૂન્ય strip(અક્ષર *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    માટે(i = 0; i < len; ++i){
        અક્ષર c = s[i];
        જો(c==' '||c=='\t'||c=='\n') ++offset;
        નહીંતર s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

શૂન્ય strip_char(અક્ષર *s, અક્ષર bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    માટે(i = 0; i < len; ++i){
        અક્ષર c = s[i];
        જો(c==bad) ++offset;
        નહીંતર s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

શૂન્ય free_ptrs(શૂન્ય **ptrs, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i) મુક્ત(ptrs[i]);
    મુક્ત(ptrs);
}

અક્ષર *fgetl(FILE *fp)
{
    જો(feof(fp)) પાછા 0;
    size_t size = 512;
    અક્ષર *લીટી = malloc(size*કદ(અક્ષર));
    જો(!fgets(લીટી, size, fp)){
        મુક્ત(લીટી);
        પાછા 0;
    }

    size_t curr = strlen(લીટી);

    જ્યારે((લીટી[curr-1] != '\n') && !feof(fp)){
        જો(curr == size-1){
            size *= 2;
            લીટી = realloc(લીટી, size*કદ(અક્ષર));
            જો(!લીટી) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        જો(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&લીટી[curr], readsize, fp);
        curr = strlen(લીટી);
    }
    જો(લીટી[curr-1] == '\n') લીટી[curr-1] = '\0';

    પાછા લીટી;
}

પૂર્ણાંક read_int(પૂર્ણાંક fd)
{
    પૂર્ણાંક n = 0;
    પૂર્ણાંક next = પઠ(fd, &n, કદ(પૂર્ણાંક));
    જો(next <= 0) પાછા -1;
    પાછા n;
}

શૂન્ય write_int(પૂર્ણાંક fd, પૂર્ણાંક n)
{
    પૂર્ણાંક next = લિખ(fd, &n, કદ(પૂર્ણાંક));
    જો(next <= 0) error("read failed");
}

પૂર્ણાંક read_all_fail(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes)
{
    size_t n = 0;
    જ્યારે(n < bytes){
        પૂર્ણાંક next = પઠ(fd, buffer + n, bytes-n);
        જો(next <= 0) પાછા 1;
        n += next;
    }
    પાછા 0;
}

પૂર્ણાંક write_all_fail(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes)
{
    size_t n = 0;
    જ્યારે(n < bytes){
        size_t next = લિખ(fd, buffer + n, bytes-n);
        જો(next <= 0) પાછા 1;
        n += next;
    }
    પાછા 0;
}

શૂન્ય read_all(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes)
{
    size_t n = 0;
    જ્યારે(n < bytes){
        પૂર્ણાંક next = પઠ(fd, buffer + n, bytes-n);
        જો(next <= 0) error("read failed");
        n += next;
    }
}

શૂન્ય write_all(પૂર્ણાંક fd, અક્ષર *buffer, size_t bytes)
{
    size_t n = 0;
    જ્યારે(n < bytes){
        size_t next = લિખ(fd, buffer + n, bytes-n);
        જો(next <= 0) error("write failed");
        n += next;
    }
}


અક્ષર *copy_string(અક્ષર *s)
{
    અક્ષર *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    પાછા copy;
}

list *parse_csv_લીટી(અક્ષર *લીટી)
{
    list *l = make_list();
    અક્ષર *c, *p;
    પૂર્ણાંક in = 0;
    માટે(c = લીટી, p = લીટી; *c != '\0'; ++c){
        જો(*c == '"') in = !in;
        નહીંતર જો(*c == ',' && !in){
            *c = '\0';
            list_ઉમેરો(l, copy_string(p));
            p = c+1;
        }
    }
    list_ઉમેરો(l, copy_string(p));
    પાછા l;
}

પૂર્ણાંક ગણતરી_fields(અક્ષર *લીટી)
{
    પૂર્ણાંક ગણતરી = 0;
    પૂર્ણાંક done = 0;
    અક્ષર *c;
    માટે(c = લીટી; !done; ++c){
        done = (*c == '\0');
        જો(*c == ',' || done) ++ગણતરી;
    }
    પાછા ગણતરી;
}

દશાંશ *parse_fields(અક્ષર *લીટી, પૂર્ણાંક n)
{
    દશાંશ *field = calloc(n, કદ(દશાંશ));
    અક્ષર *c, *p, *end;
    પૂર્ણાંક ગણતરી = 0;
    પૂર્ણાંક done = 0;
    માટે(c = લીટી, p = લીટી; !done; ++c){
        done = (*c == '\0');
        જો(*c == ',' || done){
            *c = '\0';
            field[ગણતરી] = strtod(p, &end);
            જો(p == c) field[ગણતરી] = nan("");
            જો(end != c && (end != c-1 || *end != '\r')) field[ગણતરી] = nan(""); //DOS file formats!
            p = c+1;
            ++ગણતરી;
        }
    }
    પાછા field;
}

દશાંશ sum_array(દશાંશ *a, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    દશાંશ sum = 0;
    માટે(i = 0; i < n; ++i) sum += a[i];
    પાછા sum;
}

દશાંશ mean_array(દશાંશ *a, પૂર્ણાંક n)
{
    પાછા sum_array(a,n)/n;
}

શૂન્ય mean_arrays(દશાંશ **a, પૂર્ણાંક n, પૂર્ણાંક els, દશાંશ *avg)
{
    પૂર્ણાંક i;
    પૂર્ણાંક j;
    memset(avg, 0, els*કદ(દશાંશ));
    માટે(j = 0; j < n; ++j){
        માટે(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    માટે(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

શૂન્ય print_statistics(દશાંશ *a, પૂર્ણાંક n)
{
    દશાંશ m = mean_array(a, n);
    દશાંશ v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

દશાંશ variance_array(દશાંશ *a, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    દશાંશ sum = 0;
    દશાંશ mean = mean_array(a, n);
    માટે(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    દશાંશ variance = sum/n;
    પાછા variance;
}

પૂર્ણાંક constrain_int(પૂર્ણાંક a, પૂર્ણાંક min, પૂર્ણાંક max)
{
    જો (a < min) પાછા min;
    જો (a > max) પાછા max;
    પાછા a;
}

દશાંશ constrain(દશાંશ min, દશાંશ max, દશાંશ a)
{
    જો (a < min) પાછા min;
    જો (a > max) પાછા max;
    પાછા a;
}

દશાંશ dist_array(દશાંશ *a, દશાંશ *b, પૂર્ણાંક n, પૂર્ણાંક sub)
{
    પૂર્ણાંક i;
    દશાંશ sum = 0;
    માટે(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    પાછા sqrt(sum);
}

દશાંશ mse_array(દશાંશ *a, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    દશાંશ sum = 0;
    માટે(i = 0; i < n; ++i) sum += a[i]*a[i];
    પાછા sqrt(sum/n);
}

શૂન્ય normalize_array(દશાંશ *a, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    દશાંશ mu = mean_array(a,n);
    દશાંશ sigma = sqrt(variance_array(a,n));
    માટે(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

શૂન્ય translate_array(દશાંશ *a, પૂર્ણાંક n, દશાંશ s)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        a[i] += s;
    }
}

દશાંશ mag_array(દશાંશ *a, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    દશાંશ sum = 0;
    માટે(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    પાછા sqrt(sum);
}

શૂન્ય scale_array(દશાંશ *a, પૂર્ણાંક n, દશાંશ s)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

પૂર્ણાંક sample_array(દશાંશ *a, પૂર્ણાંક n)
{
    દશાંશ sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    દશાંશ r = rand_uniform(0, 1);
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        r = r - a[i];
        જો (r <= 0) પાછા i;
    }
    પાછા n-1;
}

પૂર્ણાંક max_int_index(પૂર્ણાંક *a, પૂર્ણાંક n)
{
    જો(n <= 0) પાછા -1;
    પૂર્ણાંક i, max_i = 0;
    પૂર્ણાંક max = a[0];
    માટે(i = 1; i < n; ++i){
        જો(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    પાછા max_i;
}

પૂર્ણાંક max_index(દશાંશ *a, પૂર્ણાંક n)
{
    જો(n <= 0) પાછા -1;
    પૂર્ણાંક i, max_i = 0;
    દશાંશ max = a[0];
    માટે(i = 1; i < n; ++i){
        જો(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    પાછા max_i;
}

પૂર્ણાંક int_index(પૂર્ણાંક *a, પૂર્ણાંક val, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        જો(a[i] == val) પાછા i;
    }
    પાછા -1;
}

પૂર્ણાંક rand_int(પૂર્ણાંક min, પૂર્ણાંક max)
{
    જો (max < min){
        પૂર્ણાંક s = min;
        min = max;
        max = s;
    }
    પૂર્ણાંક r = (rand()%(max - min + 1)) + min;
    પાછા r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
દશાંશ rand_normal()
{
    સ્થિર પૂર્ણાંક haveSpare = 0;
    સ્થિર દ્વિગુણ rand1, rand2;

    જો(haveSpare)
    {
        haveSpare = 0;
        પાછા sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((દ્વિગુણ) RAND_MAX);
    જો(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((દ્વિગુણ) RAND_MAX)) * TWO_PI;

    પાછા sqrt(rand1) * cos(rand2);
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
    પાછા  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

દશાંશ rand_uniform(દશાંશ min, દશાંશ max)
{
    જો(max < min){
        દશાંશ swap = min;
        min = max;
        max = swap;
    }
    પાછા ((દશાંશ)rand()/RAND_MAX * (max - min)) + min;
}

દશાંશ rand_scale(દશાંશ s)
{
    દશાંશ scale = rand_uniform(1, s);
    જો(rand()%2) પાછા scale;
    પાછા 1./scale;
}

દશાંશ **one_hot_encode(દશાંશ *a, પૂર્ણાંક n, પૂર્ણાંક k)
{
    પૂર્ણાંક i;
    દશાંશ **t = calloc(n, કદ(દશાંશ*));
    માટે(i = 0; i < n; ++i){
        t[i] = calloc(k, કદ(દશાંશ));
        પૂર્ણાંક index = (પૂર્ણાંક)a[i];
        t[i][index] = 1;
    }
    પાછા t;
}

