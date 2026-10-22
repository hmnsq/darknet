#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>

#ಅನ್ತರ್ಭಾವ "utils.h"


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

ದ್ವಿಗುಣ what_time_is_it_now()
{
    ಸಂರಚನಾ timeval time;
    ಯದಿ (gettimeofday(&time,ರಿಕ್ತ)){
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಪ್ರತ್ಯಾಗಮ (ದ್ವಿಗುಣ)time.tv_sec + (ದ್ವಿಗುಣ)time.tv_usec * .000001;
}

ಪೂರ್ಣಾಙ್ಕ *read_intlist(ವರ್ಣ *gpu_list, ಪೂರ್ಣಾಙ್ಕ *ngpus, ಪೂರ್ಣಾಙ್ಕ d)
{
    ಪೂರ್ಣಾಙ್ಕ *gpus = 0;
    ಯದಿ(gpu_list){
        ಪೂರ್ಣಾಙ್ಕ len = strlen(gpu_list);
        *ngpus = 1;
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ(i = 0; i < len; ++i){
            ಯದಿ (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
        ಪ್ರತಿ(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } ಅನ್ಯಥಾ {
        gpus = calloc(1, ಪರಿಮಾಣ(ಭಿನ್ನ));
        *gpus = d;
        *ngpus = 1;
    }
    ಪ್ರತ್ಯಾಗಮ gpus;
}

ಪೂರ್ಣಾಙ್ಕ *read_map(ವರ್ಣ *filename)
{
    ಪೂರ್ಣಾಙ್ಕ n = 0;
    ಪೂರ್ಣಾಙ್ಕ *map = 0;
    ವರ್ಣ *str;
    FILE *file = fopen(filename, "r");
    ಯದಿ(!file) file_error(filename);
    ಯಾವತ್((str=fgetl(file))){
        ++n;
        map = realloc(map, n*ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
        map[n-1] = atoi(str);
    }
    ಪ್ರತ್ಯಾಗಮ map;
}

ಶೂನ್ಯ sorta_shuffle(ಶೂನ್ಯ *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    ಪ್ರತಿ(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t ಅಂಕೆ = end-start;
        shuffle(arr+(start*size), ಅಂಕೆ, size);
    }
}

ಶೂನ್ಯ shuffle(ಶೂನ್ಯ *arr, size_t n, size_t size)
{
    size_t i;
    ಶೂನ್ಯ *swp = calloc(1, size);
    ಪ್ರತಿ(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

ಪೂರ್ಣಾಙ್ಕ *random_index_order(ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max)
{
    ಪೂರ್ಣಾಙ್ಕ *inds = calloc(max-min, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = min; i < max; ++i){
        inds[i] = i;
    }
    ಪ್ರತಿ(i = min; i < max-1; ++i){
        ಪೂರ್ಣಾಙ್ಕ swap = inds[i];
        ಪೂರ್ಣಾಙ್ಕ index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    ಪ್ರತ್ಯಾಗಮ inds;
}

ಶೂನ್ಯ del_arg(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ index)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ಪೂರ್ಣಾಙ್ಕ find_arg(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ* argv[], ವರ್ಣ *arg)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < argc; ++i) {
        ಯದಿ(!argv[i]) ಅನುವರ್ತ;
        ಯದಿ(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ಪ್ರತ್ಯಾಗಮ 1;
        }
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ find_int_arg(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ def)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < argc-1; ++i){
        ಯದಿ(!argv[i]) ಅನುವರ್ತ;
        ಯದಿ(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ಭಙ್ಗ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ def;
}

ಭಿನ್ನ find_float_arg(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, ವರ್ಣ *arg, ಭಿನ್ನ def)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < argc-1; ++i){
        ಯದಿ(!argv[i]) ಅನುವರ್ತ;
        ಯದಿ(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ಭಙ್ಗ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ def;
}

ವರ್ಣ *find_char_arg(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, ವರ್ಣ *arg, ವರ್ಣ *def)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < argc-1; ++i){
        ಯದಿ(!argv[i]) ಅನುವರ್ತ;
        ಯದಿ(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ಭಙ್ಗ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ def;
}


ವರ್ಣ *basecfg(ವರ್ಣ *cfgfile)
{
    ವರ್ಣ *c = cfgfile;
    ವರ್ಣ *next;
    ಯಾವತ್((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    ಯದಿ (next) *next = 0;
    ಪ್ರತ್ಯಾಗಮ c;
}

ಪೂರ್ಣಾಙ್ಕ alphanum_to_int(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ (c < 58) ? c - 48 : c-87;
}
ವರ್ಣ int_to_alphanum(ಪೂರ್ಣಾಙ್ಕ i)
{
    ಯದಿ (i == 36) ಪ್ರತ್ಯಾಗಮ '.';
    ಪ್ರತ್ಯಾಗಮ (i < 10) ? i + 48 : i + 87;
}

ಶೂನ್ಯ pm(ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *A)
{
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        ಪ್ರತಿ(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

ಶೂನ್ಯ find_replace(ವರ್ಣ *str, ವರ್ಣ *orig, ವರ್ಣ *rep, ವರ್ಣ *output)
{
    ವರ್ಣ buffer[4096] = {0};
    ವರ್ಣ *p;

    sprintf(buffer, "%s", str);
    ಯದಿ(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ಪ್ರತ್ಯಾಗಮ;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

ಭಿನ್ನ sec(clock_t clocks)
{
    ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)clocks/CLOCKS_PER_SEC;
}

ಶೂನ್ಯ ತುದಿ_k(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ *index)
{
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(j = 0; j < k; ++j) index[j] = -1;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಙ್ಕ curr = i;
        ಪ್ರತಿ(j = 0; j < k; ++j){
            ಯದಿ((index[j] < 0) || a[curr] > a[index[j]]){
                ಪೂರ್ಣಾಙ್ಕ swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

ಶೂನ್ಯ error(ಧ್ರುವ ವರ್ಣ *s)
{
    perror(s);
    ಪ್ರತಿಜ್ಞಾ(0);
    exit(-1);
}

unsigned ವರ್ಣ *read_file(ವರ್ಣ *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned ವರ್ಣ *text = calloc(size+1, ಪರಿಮಾಣ(ವರ್ಣ));
    fread(text, 1, size, fp);
    fclose(fp);
    ಪ್ರತ್ಯಾಗಮ text;
}

ಶೂನ್ಯ malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

ಶೂನ್ಯ file_error(ವರ್ಣ *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(ವರ್ಣ *s, ವರ್ಣ delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_ಸೇರಿಸು(l, s);
    ಪ್ರತಿ(i = 0; i < len; ++i){
        ಯದಿ(s[i] == delim){
            s[i] = '\0';
            list_ಸೇರಿಸು(l, &(s[i+1]));
        }
    }
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ strip(ವರ್ಣ *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ಪ್ರತಿ(i = 0; i < len; ++i){
        ವರ್ಣ c = s[i];
        ಯದಿ(c==' '||c=='\t'||c=='\n') ++offset;
        ಅನ್ಯಥಾ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

ಶೂನ್ಯ strip_char(ವರ್ಣ *s, ವರ್ಣ bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    ಪ್ರತಿ(i = 0; i < len; ++i){
        ವರ್ಣ c = s[i];
        ಯದಿ(c==bad) ++offset;
        ಅನ್ಯಥಾ s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

ಶೂನ್ಯ free_ptrs(ಶೂನ್ಯ **ptrs, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i) ಮುಕ್ತ(ptrs[i]);
    ಮುಕ್ತ(ptrs);
}

ವರ್ಣ *fgetl(FILE *fp)
{
    ಯದಿ(feof(fp)) ಪ್ರತ್ಯಾಗಮ 0;
    size_t size = 512;
    ವರ್ಣ *ಸಾಲು = malloc(size*ಪರಿಮಾಣ(ವರ್ಣ));
    ಯದಿ(!fgets(ಸಾಲು, size, fp)){
        ಮುಕ್ತ(ಸಾಲು);
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    size_t curr = strlen(ಸಾಲು);

    ಯಾವತ್((ಸಾಲು[curr-1] != '\n') && !feof(fp)){
        ಯದಿ(curr == size-1){
            size *= 2;
            ಸಾಲು = realloc(ಸಾಲು, size*ಪರಿಮಾಣ(ವರ್ಣ));
            ಯದಿ(!ಸಾಲು) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        ಯದಿ(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&ಸಾಲು[curr], readsize, fp);
        curr = strlen(ಸಾಲು);
    }
    ಯದಿ(ಸಾಲು[curr-1] == '\n') ಸಾಲು[curr-1] = '\0';

    ಪ್ರತ್ಯಾಗಮ ಸಾಲು;
}

ಪೂರ್ಣಾಙ್ಕ read_int(ಪೂರ್ಣಾಙ್ಕ fd)
{
    ಪೂರ್ಣಾಙ್ಕ n = 0;
    ಪೂರ್ಣಾಙ್ಕ next = ಪಠ(fd, &n, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    ಯದಿ(next <= 0) ಪ್ರತ್ಯಾಗಮ -1;
    ಪ್ರತ್ಯಾಗಮ n;
}

ಶೂನ್ಯ write_int(ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ next = ಲಿಖ(fd, &n, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    ಯದಿ(next <= 0) error("read failed");
}

ಪೂರ್ಣಾಙ್ಕ read_all_fail(ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ *buffer, size_t bytes)
{
    size_t n = 0;
    ಯಾವತ್(n < bytes){
        ಪೂರ್ಣಾಙ್ಕ next = ಪಠ(fd, buffer + n, bytes-n);
        ಯದಿ(next <= 0) ಪ್ರತ್ಯಾಗಮ 1;
        n += next;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ write_all_fail(ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ *buffer, size_t bytes)
{
    size_t n = 0;
    ಯಾವತ್(n < bytes){
        size_t next = ಲಿಖ(fd, buffer + n, bytes-n);
        ಯದಿ(next <= 0) ಪ್ರತ್ಯಾಗಮ 1;
        n += next;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ read_all(ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ *buffer, size_t bytes)
{
    size_t n = 0;
    ಯಾವತ್(n < bytes){
        ಪೂರ್ಣಾಙ್ಕ next = ಪಠ(fd, buffer + n, bytes-n);
        ಯದಿ(next <= 0) error("read failed");
        n += next;
    }
}

ಶೂನ್ಯ write_all(ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ *buffer, size_t bytes)
{
    size_t n = 0;
    ಯಾವತ್(n < bytes){
        size_t next = ಲಿಖ(fd, buffer + n, bytes-n);
        ಯದಿ(next <= 0) error("write failed");
        n += next;
    }
}


ವರ್ಣ *copy_string(ವರ್ಣ *s)
{
    ವರ್ಣ *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    ಪ್ರತ್ಯಾಗಮ copy;
}

list *parse_csv_ಸಾಲು(ವರ್ಣ *ಸಾಲು)
{
    list *l = make_list();
    ವರ್ಣ *c, *p;
    ಪೂರ್ಣಾಙ್ಕ in = 0;
    ಪ್ರತಿ(c = ಸಾಲು, p = ಸಾಲು; *c != '\0'; ++c){
        ಯದಿ(*c == '"') in = !in;
        ಅನ್ಯಥಾ ಯದಿ(*c == ',' && !in){
            *c = '\0';
            list_ಸೇರಿಸು(l, copy_string(p));
            p = c+1;
        }
    }
    list_ಸೇರಿಸು(l, copy_string(p));
    ಪ್ರತ್ಯಾಗಮ l;
}

ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_fields(ವರ್ಣ *ಸಾಲು)
{
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಙ್ಕ done = 0;
    ವರ್ಣ *c;
    ಪ್ರತಿ(c = ಸಾಲು; !done; ++c){
        done = (*c == '\0');
        ಯದಿ(*c == ',' || done) ++ಎಣಿಕೆ;
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಭಿನ್ನ *parse_fields(ವರ್ಣ *ಸಾಲು, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಭಿನ್ನ *field = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ವರ್ಣ *c, *p, *end;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಙ್ಕ done = 0;
    ಪ್ರತಿ(c = ಸಾಲು, p = ಸಾಲು; !done; ++c){
        done = (*c == '\0');
        ಯದಿ(*c == ',' || done){
            *c = '\0';
            field[ಎಣಿಕೆ] = strtod(p, &end);
            ಯದಿ(p == c) field[ಎಣಿಕೆ] = nan("");
            ಯದಿ(end != c && (end != c-1 || *end != '\r')) field[ಎಣಿಕೆ] = nan(""); //DOS file formats!
            p = c+1;
            ++ಎಣಿಕೆ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ field;
}

ಭಿನ್ನ sum_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i) sum += a[i];
    ಪ್ರತ್ಯಾಗಮ sum;
}

ಭಿನ್ನ mean_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪ್ರತ್ಯಾಗಮ sum_array(a,n)/n;
}

ಶೂನ್ಯ mean_arrays(ಭಿನ್ನ **a, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ els, ಭಿನ್ನ *avg)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ j;
    memset(avg, 0, els*ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತಿ(j = 0; j < n; ++j){
        ಪ್ರತಿ(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    ಪ್ರತಿ(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

ಶೂನ್ಯ print_statistics(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಭಿನ್ನ m = mean_array(a, n);
    ಭಿನ್ನ v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

ಭಿನ್ನ variance_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಭಿನ್ನ mean = mean_array(a, n);
    ಪ್ರತಿ(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    ಭಿನ್ನ variance = sum/n;
    ಪ್ರತ್ಯಾಗಮ variance;
}

ಪೂರ್ಣಾಙ್ಕ constrain_int(ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max)
{
    ಯದಿ (a < min) ಪ್ರತ್ಯಾಗಮ min;
    ಯದಿ (a > max) ಪ್ರತ್ಯಾಗಮ max;
    ಪ್ರತ್ಯಾಗಮ a;
}

ಭಿನ್ನ constrain(ಭಿನ್ನ min, ಭಿನ್ನ max, ಭಿನ್ನ a)
{
    ಯದಿ (a < min) ಪ್ರತ್ಯಾಗಮ min;
    ಯದಿ (a > max) ಪ್ರತ್ಯಾಗಮ max;
    ಪ್ರತ್ಯಾಗಮ a;
}

ಭಿನ್ನ dist_array(ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ sub)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಪ್ರತಿ(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    ಪ್ರತ್ಯಾಗಮ sqrt(sum);
}

ಭಿನ್ನ mse_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i) sum += a[i]*a[i];
    ಪ್ರತ್ಯಾಗಮ sqrt(sum/n);
}

ಶೂನ್ಯ normalize_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ mu = mean_array(a,n);
    ಭಿನ್ನ sigma = sqrt(variance_array(a,n));
    ಪ್ರತಿ(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

ಶೂನ್ಯ translate_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ s)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        a[i] += s;
    }
}

ಭಿನ್ನ mag_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    ಪ್ರತ್ಯಾಗಮ sqrt(sum);
}

ಶೂನ್ಯ scale_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ s)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ಪೂರ್ಣಾಙ್ಕ sample_array(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಭಿನ್ನ sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    ಭಿನ್ನ r = rand_uniform(0, 1);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        r = r - a[i];
        ಯದಿ (r <= 0) ಪ್ರತ್ಯಾಗಮ i;
    }
    ಪ್ರತ್ಯಾಗಮ n-1;
}

ಪೂರ್ಣಾಙ್ಕ max_int_index(ಪೂರ್ಣಾಙ್ಕ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಯದಿ(n <= 0) ಪ್ರತ್ಯಾಗಮ -1;
    ಪೂರ್ಣಾಙ್ಕ i, max_i = 0;
    ಪೂರ್ಣಾಙ್ಕ max = a[0];
    ಪ್ರತಿ(i = 1; i < n; ++i){
        ಯದಿ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ಪ್ರತ್ಯಾಗಮ max_i;
}

ಪೂರ್ಣಾಙ್ಕ max_index(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಯದಿ(n <= 0) ಪ್ರತ್ಯಾಗಮ -1;
    ಪೂರ್ಣಾಙ್ಕ i, max_i = 0;
    ಭಿನ್ನ max = a[0];
    ಪ್ರತಿ(i = 1; i < n; ++i){
        ಯದಿ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ಪ್ರತ್ಯಾಗಮ max_i;
}

ಪೂರ್ಣಾಙ್ಕ int_index(ಪೂರ್ಣಾಙ್ಕ *a, ಪೂರ್ಣಾಙ್ಕ val, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಯದಿ(a[i] == val) ಪ್ರತ್ಯಾಗಮ i;
    }
    ಪ್ರತ್ಯಾಗಮ -1;
}

ಪೂರ್ಣಾಙ್ಕ rand_int(ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max)
{
    ಯದಿ (max < min){
        ಪೂರ್ಣಾಙ್ಕ s = min;
        min = max;
        max = s;
    }
    ಪೂರ್ಣಾಙ್ಕ r = (rand()%(max - min + 1)) + min;
    ಪ್ರತ್ಯಾಗಮ r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
ಭಿನ್ನ rand_normal()
{
    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ haveSpare = 0;
    ಸ್ಥಿರ ದ್ವಿಗುಣ rand1, rand2;

    ಯದಿ(haveSpare)
    {
        haveSpare = 0;
        ಪ್ರತ್ಯಾಗಮ sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((ದ್ವಿಗುಣ) RAND_MAX);
    ಯದಿ(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((ದ್ವಿಗುಣ) RAND_MAX)) * TWO_PI;

    ಪ್ರತ್ಯಾಗಮ sqrt(rand1) * cos(rand2);
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
    ಪ್ರತ್ಯಾಗಮ  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

ಭಿನ್ನ rand_uniform(ಭಿನ್ನ min, ಭಿನ್ನ max)
{
    ಯದಿ(max < min){
        ಭಿನ್ನ swap = min;
        min = max;
        max = swap;
    }
    ಪ್ರತ್ಯಾಗಮ ((ಭಿನ್ನ)rand()/RAND_MAX * (max - min)) + min;
}

ಭಿನ್ನ rand_scale(ಭಿನ್ನ s)
{
    ಭಿನ್ನ scale = rand_uniform(1, s);
    ಯದಿ(rand()%2) ಪ್ರತ್ಯಾಗಮ scale;
    ಪ್ರತ್ಯಾಗಮ 1./scale;
}

ಭಿನ್ನ **one_hot_encode(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ **t = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    ಪ್ರತಿ(i = 0; i < n; ++i){
        t[i] = calloc(k, ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಪೂರ್ಣಾಙ್ಕ index = (ಪೂರ್ಣಾಙ್ಕ)a[i];
        t[i][index] = 1;
    }
    ಪ್ರತ್ಯಾಗಮ t;
}

