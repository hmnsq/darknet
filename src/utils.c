#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/time.h>

#അന്തര്ഭാവ "utils.h"


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

ഇരട്ട what_time_is_it_now()
{
    ഘടന timeval time;
    എങ്കിൽ (gettimeofday(&time,ശൂന്യ)){
        തിരികെ 0;
    }
    തിരികെ (ഇരട്ട)time.tv_sec + (ഇരട്ട)time.tv_usec * .000001;
}

പൂർണ്ണസംഖ്യ *read_intlist(അക്ഷരം *gpu_list, പൂർണ്ണസംഖ്യ *ngpus, പൂർണ്ണസംഖ്യ d)
{
    പൂർണ്ണസംഖ്യ *gpus = 0;
    എങ്കിൽ(gpu_list){
        പൂർണ്ണസംഖ്യ len = strlen(gpu_list);
        *ngpus = 1;
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < len; ++i){
            എങ്കിൽ (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, വലുപ്പം(പൂർണ്ണസംഖ്യ));
        വേണ്ടി(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } അല്ലെങ്കിൽ {
        gpus = calloc(1, വലുപ്പം(ദശാംശം));
        *gpus = d;
        *ngpus = 1;
    }
    തിരികെ gpus;
}

പൂർണ്ണസംഖ്യ *read_map(അക്ഷരം *filename)
{
    പൂർണ്ണസംഖ്യ n = 0;
    പൂർണ്ണസംഖ്യ *map = 0;
    അക്ഷരം *str;
    FILE *file = fopen(filename, "r");
    എങ്കിൽ(!file) file_error(filename);
    ആകുമ്പോൾ((str=fgetl(file))){
        ++n;
        map = realloc(map, n*വലുപ്പം(പൂർണ്ണസംഖ്യ));
        map[n-1] = atoi(str);
    }
    തിരികെ map;
}

ശൂന്യം sorta_shuffle(ശൂന്യം *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    വേണ്ടി(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t അക്കം = end-start;
        shuffle(arr+(start*size), അക്കം, size);
    }
}

ശൂന്യം shuffle(ശൂന്യം *arr, size_t n, size_t size)
{
    size_t i;
    ശൂന്യം *swp = calloc(1, size);
    വേണ്ടി(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

പൂർണ്ണസംഖ്യ *random_index_order(പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max)
{
    പൂർണ്ണസംഖ്യ *inds = calloc(max-min, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = min; i < max; ++i){
        inds[i] = i;
    }
    വേണ്ടി(i = min; i < max-1; ++i){
        പൂർണ്ണസംഖ്യ swap = inds[i];
        പൂർണ്ണസംഖ്യ index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    തിരികെ inds;
}

ശൂന്യം del_arg(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, പൂർണ്ണസംഖ്യ index)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

പൂർണ്ണസംഖ്യ find_arg(പൂർണ്ണസംഖ്യ argc, അക്ഷരം* argv[], അക്ഷരം *arg)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < argc; ++i) {
        എങ്കിൽ(!argv[i]) തുടരുക;
        എങ്കിൽ(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            തിരികെ 1;
        }
    }
    തിരികെ 0;
}

പൂർണ്ണസംഖ്യ find_int_arg(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, അക്ഷരം *arg, പൂർണ്ണസംഖ്യ def)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < argc-1; ++i){
        എങ്കിൽ(!argv[i]) തുടരുക;
        എങ്കിൽ(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            നിർത്തുക;
        }
    }
    തിരികെ def;
}

ദശാംശം find_float_arg(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, അക്ഷരം *arg, ദശാംശം def)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < argc-1; ++i){
        എങ്കിൽ(!argv[i]) തുടരുക;
        എങ്കിൽ(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            നിർത്തുക;
        }
    }
    തിരികെ def;
}

അക്ഷരം *find_char_arg(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, അക്ഷരം *arg, അക്ഷരം *def)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < argc-1; ++i){
        എങ്കിൽ(!argv[i]) തുടരുക;
        എങ്കിൽ(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            നിർത്തുക;
        }
    }
    തിരികെ def;
}


അക്ഷരം *basecfg(അക്ഷരം *cfgfile)
{
    അക്ഷരം *c = cfgfile;
    അക്ഷരം *next;
    ആകുമ്പോൾ((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    എങ്കിൽ (next) *next = 0;
    തിരികെ c;
}

പൂർണ്ണസംഖ്യ alphanum_to_int(അക്ഷരം c)
{
    തിരികെ (c < 58) ? c - 48 : c-87;
}
അക്ഷരം int_to_alphanum(പൂർണ്ണസംഖ്യ i)
{
    എങ്കിൽ (i == 36) തിരികെ '.';
    തിരികെ (i < 10) ? i + 48 : i + 87;
}

ശൂന്യം pm(പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, ദശാംശം *A)
{
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        വേണ്ടി(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

ശൂന്യം find_replace(അക്ഷരം *str, അക്ഷരം *orig, അക്ഷരം *rep, അക്ഷരം *output)
{
    അക്ഷരം buffer[4096] = {0};
    അക്ഷരം *p;

    sprintf(buffer, "%s", str);
    എങ്കിൽ(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        തിരികെ;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

ദശാംശം sec(clock_t clocks)
{
    തിരികെ (ദശാംശം)clocks/CLOCKS_PER_SEC;
}

ശൂന്യം മുകൾ_k(ദശാംശം *a, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ *index)
{
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(j = 0; j < k; ++j) index[j] = -1;
    വേണ്ടി(i = 0; i < n; ++i){
        പൂർണ്ണസംഖ്യ curr = i;
        വേണ്ടി(j = 0; j < k; ++j){
            എങ്കിൽ((index[j] < 0) || a[curr] > a[index[j]]){
                പൂർണ്ണസംഖ്യ swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

ശൂന്യം error(സ്ഥിരം അക്ഷരം *s)
{
    perror(s);
    പ്രതിജ്ഞാ(0);
    exit(-1);
}

unsigned അക്ഷരം *read_file(അക്ഷരം *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned അക്ഷരം *text = calloc(size+1, വലുപ്പം(അക്ഷരം));
    fread(text, 1, size, fp);
    fclose(fp);
    തിരികെ text;
}

ശൂന്യം malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

ശൂന്യം file_error(അക്ഷരം *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(അക്ഷരം *s, അക്ഷരം delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_തിരുകുക(l, s);
    വേണ്ടി(i = 0; i < len; ++i){
        എങ്കിൽ(s[i] == delim){
            s[i] = '\0';
            list_തിരുകുക(l, &(s[i+1]));
        }
    }
    തിരികെ l;
}

ശൂന്യം strip(അക്ഷരം *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    വേണ്ടി(i = 0; i < len; ++i){
        അക്ഷരം c = s[i];
        എങ്കിൽ(c==' '||c=='\t'||c=='\n') ++offset;
        അല്ലെങ്കിൽ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

ശൂന്യം strip_char(അക്ഷരം *s, അക്ഷരം bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    വേണ്ടി(i = 0; i < len; ++i){
        അക്ഷരം c = s[i];
        എങ്കിൽ(c==bad) ++offset;
        അല്ലെങ്കിൽ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

ശൂന്യം free_ptrs(ശൂന്യം **ptrs, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i) സ്വതന്ത്രം(ptrs[i]);
    സ്വതന്ത്രം(ptrs);
}

അക്ഷരം *fgetl(FILE *fp)
{
    എങ്കിൽ(feof(fp)) തിരികെ 0;
    size_t size = 512;
    അക്ഷരം *വരി = malloc(size*വലുപ്പം(അക്ഷരം));
    എങ്കിൽ(!fgets(വരി, size, fp)){
        സ്വതന്ത്രം(വരി);
        തിരികെ 0;
    }

    size_t curr = strlen(വരി);

    ആകുമ്പോൾ((വരി[curr-1] != '\n') && !feof(fp)){
        എങ്കിൽ(curr == size-1){
            size *= 2;
            വരി = realloc(വരി, size*വലുപ്പം(അക്ഷരം));
            എങ്കിൽ(!വരി) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        എങ്കിൽ(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&വരി[curr], readsize, fp);
        curr = strlen(വരി);
    }
    എങ്കിൽ(വരി[curr-1] == '\n') വരി[curr-1] = '\0';

    തിരികെ വരി;
}

പൂർണ്ണസംഖ്യ read_int(പൂർണ്ണസംഖ്യ fd)
{
    പൂർണ്ണസംഖ്യ n = 0;
    പൂർണ്ണസംഖ്യ next = പഠ(fd, &n, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    എങ്കിൽ(next <= 0) തിരികെ -1;
    തിരികെ n;
}

ശൂന്യം write_int(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ next = ലിഖ(fd, &n, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    എങ്കിൽ(next <= 0) error("read failed");
}

പൂർണ്ണസംഖ്യ read_all_fail(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes)
{
    size_t n = 0;
    ആകുമ്പോൾ(n < bytes){
        പൂർണ്ണസംഖ്യ next = പഠ(fd, buffer + n, bytes-n);
        എങ്കിൽ(next <= 0) തിരികെ 1;
        n += next;
    }
    തിരികെ 0;
}

പൂർണ്ണസംഖ്യ write_all_fail(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes)
{
    size_t n = 0;
    ആകുമ്പോൾ(n < bytes){
        size_t next = ലിഖ(fd, buffer + n, bytes-n);
        എങ്കിൽ(next <= 0) തിരികെ 1;
        n += next;
    }
    തിരികെ 0;
}

ശൂന്യം read_all(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes)
{
    size_t n = 0;
    ആകുമ്പോൾ(n < bytes){
        പൂർണ്ണസംഖ്യ next = പഠ(fd, buffer + n, bytes-n);
        എങ്കിൽ(next <= 0) error("read failed");
        n += next;
    }
}

ശൂന്യം write_all(പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buffer, size_t bytes)
{
    size_t n = 0;
    ആകുമ്പോൾ(n < bytes){
        size_t next = ലിഖ(fd, buffer + n, bytes-n);
        എങ്കിൽ(next <= 0) error("write failed");
        n += next;
    }
}


അക്ഷരം *copy_string(അക്ഷരം *s)
{
    അക്ഷരം *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    തിരികെ copy;
}

list *parse_csv_വരി(അക്ഷരം *വരി)
{
    list *l = make_list();
    അക്ഷരം *c, *p;
    പൂർണ്ണസംഖ്യ in = 0;
    വേണ്ടി(c = വരി, p = വരി; *c != '\0'; ++c){
        എങ്കിൽ(*c == '"') in = !in;
        അല്ലെങ്കിൽ എങ്കിൽ(*c == ',' && !in){
            *c = '\0';
            list_തിരുകുക(l, copy_string(p));
            p = c+1;
        }
    }
    list_തിരുകുക(l, copy_string(p));
    തിരികെ l;
}

പൂർണ്ണസംഖ്യ എണ്ണം_fields(അക്ഷരം *വരി)
{
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    പൂർണ്ണസംഖ്യ done = 0;
    അക്ഷരം *c;
    വേണ്ടി(c = വരി; !done; ++c){
        done = (*c == '\0');
        എങ്കിൽ(*c == ',' || done) ++എണ്ണം;
    }
    തിരികെ എണ്ണം;
}

ദശാംശം *parse_fields(അക്ഷരം *വരി, പൂർണ്ണസംഖ്യ n)
{
    ദശാംശം *field = calloc(n, വലുപ്പം(ദശാംശം));
    അക്ഷരം *c, *p, *end;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    പൂർണ്ണസംഖ്യ done = 0;
    വേണ്ടി(c = വരി, p = വരി; !done; ++c){
        done = (*c == '\0');
        എങ്കിൽ(*c == ',' || done){
            *c = '\0';
            field[എണ്ണം] = strtod(p, &end);
            എങ്കിൽ(p == c) field[എണ്ണം] = nan("");
            എങ്കിൽ(end != c && (end != c-1 || *end != '\r')) field[എണ്ണം] = nan(""); //DOS file formats!
            p = c+1;
            ++എണ്ണം;
        }
    }
    തിരികെ field;
}

ദശാംശം sum_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    വേണ്ടി(i = 0; i < n; ++i) sum += a[i];
    തിരികെ sum;
}

ദശാംശം mean_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    തിരികെ sum_array(a,n)/n;
}

ശൂന്യം mean_arrays(ദശാംശം **a, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ els, ദശാംശം *avg)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ j;
    memset(avg, 0, els*വലുപ്പം(ദശാംശം));
    വേണ്ടി(j = 0; j < n; ++j){
        വേണ്ടി(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    വേണ്ടി(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

ശൂന്യം print_statistics(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    ദശാംശം m = mean_array(a, n);
    ദശാംശം v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

ദശാംശം variance_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    ദശാംശം mean = mean_array(a, n);
    വേണ്ടി(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    ദശാംശം variance = sum/n;
    തിരികെ variance;
}

പൂർണ്ണസംഖ്യ constrain_int(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max)
{
    എങ്കിൽ (a < min) തിരികെ min;
    എങ്കിൽ (a > max) തിരികെ max;
    തിരികെ a;
}

ദശാംശം constrain(ദശാംശം min, ദശാംശം max, ദശാംശം a)
{
    എങ്കിൽ (a < min) തിരികെ min;
    എങ്കിൽ (a > max) തിരികെ max;
    തിരികെ a;
}

ദശാംശം dist_array(ദശാംശം *a, ദശാംശം *b, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ sub)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    വേണ്ടി(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    തിരികെ sqrt(sum);
}

ദശാംശം mse_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    വേണ്ടി(i = 0; i < n; ++i) sum += a[i]*a[i];
    തിരികെ sqrt(sum/n);
}

ശൂന്യം normalize_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം mu = mean_array(a,n);
    ദശാംശം sigma = sqrt(variance_array(a,n));
    വേണ്ടി(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

ശൂന്യം translate_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n, ദശാംശം s)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        a[i] += s;
    }
}

ദശാംശം mag_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    വേണ്ടി(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    തിരികെ sqrt(sum);
}

ശൂന്യം scale_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n, ദശാംശം s)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

പൂർണ്ണസംഖ്യ sample_array(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    ദശാംശം sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    ദശാംശം r = rand_uniform(0, 1);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        r = r - a[i];
        എങ്കിൽ (r <= 0) തിരികെ i;
    }
    തിരികെ n-1;
}

പൂർണ്ണസംഖ്യ max_int_index(പൂർണ്ണസംഖ്യ *a, പൂർണ്ണസംഖ്യ n)
{
    എങ്കിൽ(n <= 0) തിരികെ -1;
    പൂർണ്ണസംഖ്യ i, max_i = 0;
    പൂർണ്ണസംഖ്യ max = a[0];
    വേണ്ടി(i = 1; i < n; ++i){
        എങ്കിൽ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    തിരികെ max_i;
}

പൂർണ്ണസംഖ്യ max_index(ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    എങ്കിൽ(n <= 0) തിരികെ -1;
    പൂർണ്ണസംഖ്യ i, max_i = 0;
    ദശാംശം max = a[0];
    വേണ്ടി(i = 1; i < n; ++i){
        എങ്കിൽ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    തിരികെ max_i;
}

പൂർണ്ണസംഖ്യ int_index(പൂർണ്ണസംഖ്യ *a, പൂർണ്ണസംഖ്യ val, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        എങ്കിൽ(a[i] == val) തിരികെ i;
    }
    തിരികെ -1;
}

പൂർണ്ണസംഖ്യ rand_int(പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max)
{
    എങ്കിൽ (max < min){
        പൂർണ്ണസംഖ്യ s = min;
        min = max;
        max = s;
    }
    പൂർണ്ണസംഖ്യ r = (rand()%(max - min + 1)) + min;
    തിരികെ r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
ദശാംശം rand_normal()
{
    സ്ഥിതം പൂർണ്ണസംഖ്യ haveSpare = 0;
    സ്ഥിതം ഇരട്ട rand1, rand2;

    എങ്കിൽ(haveSpare)
    {
        haveSpare = 0;
        തിരികെ sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((ഇരട്ട) RAND_MAX);
    എങ്കിൽ(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((ഇരട്ട) RAND_MAX)) * TWO_PI;

    തിരികെ sqrt(rand1) * cos(rand2);
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
    തിരികെ  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

ദശാംശം rand_uniform(ദശാംശം min, ദശാംശം max)
{
    എങ്കിൽ(max < min){
        ദശാംശം swap = min;
        min = max;
        max = swap;
    }
    തിരികെ ((ദശാംശം)rand()/RAND_MAX * (max - min)) + min;
}

ദശാംശം rand_scale(ദശാംശം s)
{
    ദശാംശം scale = rand_uniform(1, s);
    എങ്കിൽ(rand()%2) തിരികെ scale;
    തിരികെ 1./scale;
}

ദശാംശം **one_hot_encode(ദശാംശം *a, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം **t = calloc(n, വലുപ്പം(ദശാംശം*));
    വേണ്ടി(i = 0; i < n; ++i){
        t[i] = calloc(k, വലുപ്പം(ദശാംശം));
        പൂർണ്ണസംഖ്യ index = (പൂർണ്ണസംഖ്യ)a[i];
        t[i][index] = 1;
    }
    തിരികെ t;
}

