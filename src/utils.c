#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"


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

ਦੁੱਗਣਾ what_ਸਮਾਂ_is_it_now()
{
    ਬਣਤਰ timeval ਸਮਾਂ;
    ਜੇ (gettimeofday(&ਸਮਾਂ,ਨਿਰਾ)){
        ਵਾਪਸ 0;
    }
    ਵਾਪਸ (ਦੁੱਗਣਾ)ਸਮਾਂ.tv_sec + (ਦੁੱਗਣਾ)ਸਮਾਂ.tv_usec * .000001;
}

ਪੂਰਨ *read_intlist(ਅੱਖਰ *gpu_list, ਪੂਰਨ *ngpus, ਪੂਰਨ d)
{
    ਪੂਰਨ *gpus = 0;
    ਜੇ(gpu_list){
        ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(gpu_list);
        *ngpus = 1;
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < len; ++i){
            ਜੇ (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(*ngpus, ਆਕਾਰ(ਪੂਰਨ));
        ਲਈ(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } ਨਹੀਂਤਾਂ {
        gpus = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        *gpus = d;
        *ngpus = 1;
    }
    ਵਾਪਸ gpus;
}

ਪੂਰਨ *read_map(ਅੱਖਰ *filename)
{
    ਪੂਰਨ n = 0;
    ਪੂਰਨ *map = 0;
    ਅੱਖਰ *str;
    FILE *file = fopen(filename, "r");
    ਜੇ(!file) file_error(filename);
    ਜਦੋਂ((str=fgetl(file))){
        ++n;
        map = realloc(map, n*ਆਕਾਰ(ਪੂਰਨ));
        map[n-1] = atoi(str);
    }
    ਵਾਪਸ map;
}

ਖਾਲੀ sorta_shuffle(ਖਾਲੀ *arr, size_t n, size_t ਮਾਪ, size_t sections)
{
    size_t i;
    ਲਈ(i = 0; i < sections; ++i){
        size_t ਸ਼ੁਰੂ = n*i/sections;
        size_t ਸਿਰਾ = n*(i+1)/sections;
        size_t ਅੰਕ = ਸਿਰਾ-ਸ਼ੁਰੂ;
        shuffle(arr+(ਸ਼ੁਰੂ*ਮਾਪ), ਅੰਕ, ਮਾਪ);
    }
}

ਖਾਲੀ shuffle(ਖਾਲੀ *arr, size_t n, size_t ਮਾਪ)
{
    size_t i;
    ਖਾਲੀ *swp = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਮਾਪ);
    ਲਈ(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(swp,          arr+(j*ਮਾਪ), ਮਾਪ);
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(arr+(j*ਮਾਪ), arr+(i*ਮਾਪ), ਮਾਪ);
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(arr+(i*ਮਾਪ), swp,          ਮਾਪ);
    }
}

ਪੂਰਨ *random_ਸੂਚਕਾਂਕ_ਕ੍ਰਮ(ਪੂਰਨ min, ਪੂਰਨ max)
{
    ਪੂਰਨ *inds = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(max-min, ਆਕਾਰ(ਪੂਰਨ));
    ਪੂਰਨ i;
    ਲਈ(i = min; i < max; ++i){
        inds[i] = i;
    }
    ਲਈ(i = min; i < max-1; ++i){
        ਪੂਰਨ ਅਦਲਾ_ਬਦਲੀ = inds[i];
        ਪੂਰਨ ਸੂਚਕਾਂਕ = i + rand()%(max-i);
        inds[i] = inds[ਸੂਚਕਾਂਕ];
        inds[ਸੂਚਕਾਂਕ] = ਅਦਲਾ_ਬਦਲੀ;
    }
    ਵਾਪਸ inds;
}

ਖਾਲੀ del_arg(ਪੂਰਨ argc, ਅੱਖਰ **argv, ਪੂਰਨ ਸੂਚਕਾਂਕ)
{
    ਪੂਰਨ i;
    ਲਈ(i = ਸੂਚਕਾਂਕ; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

ਪੂਰਨ ਲੱਭੋ_arg(ਪੂਰਨ argc, ਅੱਖਰ* argv[], ਅੱਖਰ *arg)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < argc; ++i) {
        ਜੇ(!argv[i]) ਜਾਰੀ;
        ਜੇ(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            ਵਾਪਸ 1;
        }
    }
    ਵਾਪਸ 0;
}

ਪੂਰਨ ਲੱਭੋ_int_arg(ਪੂਰਨ argc, ਅੱਖਰ **argv, ਅੱਖਰ *arg, ਪੂਰਨ def)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < argc-1; ++i){
        ਜੇ(!argv[i]) ਜਾਰੀ;
        ਜੇ(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ਰੋਕੋ;
        }
    }
    ਵਾਪਸ def;
}

ਦਸ਼ਮਲਵ ਲੱਭੋ_float_arg(ਪੂਰਨ argc, ਅੱਖਰ **argv, ਅੱਖਰ *arg, ਦਸ਼ਮਲਵ def)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < argc-1; ++i){
        ਜੇ(!argv[i]) ਜਾਰੀ;
        ਜੇ(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ਰੋਕੋ;
        }
    }
    ਵਾਪਸ def;
}

ਅੱਖਰ *ਲੱਭੋ_char_arg(ਪੂਰਨ argc, ਅੱਖਰ **argv, ਅੱਖਰ *arg, ਅੱਖਰ *def)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < argc-1; ++i){
        ਜੇ(!argv[i]) ਜਾਰੀ;
        ਜੇ(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            ਰੋਕੋ;
        }
    }
    ਵਾਪਸ def;
}


ਅੱਖਰ *basecfg(ਅੱਖਰ *cfgfile)
{
    ਅੱਖਰ *c = cfgfile;
    ਅੱਖਰ *ਅਗਲਾ;
    ਜਦੋਂ((ਅਗਲਾ = strchr(c, '/')))
    {
        c = ਅਗਲਾ+1;
    }
    c = copy_string(c);
    ਅਗਲਾ = strchr(c, '.');
    ਜੇ (ਅਗਲਾ) *ਅਗਲਾ = 0;
    ਵਾਪਸ c;
}

ਪੂਰਨ alphanum_ਮੰਜ਼ਿਲ_int(ਅੱਖਰ c)
{
    ਵਾਪਸ (c < 58) ? c - 48 : c-87;
}
ਅੱਖਰ int_ਮੰਜ਼ਿਲ_alphanum(ਪੂਰਨ i)
{
    ਜੇ (i == 36) ਵਾਪਸ '.';
    ਵਾਪਸ (i < 10) ? i + 48 : i + 87;
}

ਖਾਲੀ pm(ਪੂਰਨ M, ਪੂਰਨ N, ਦਸ਼ਮਲਵ *A)
{
    ਪੂਰਨ i,j;
    ਲਈ(i =0 ; i < M; ++i){
        ਲਿਖੋ("%d ", i+1);
        ਲਈ(j = 0; j < N; ++j){
            ਲਿਖੋ("%2.4f, ", A[i*N+j]);
        }
        ਲਿਖੋ("\n");
    }
    ਲਿਖੋ("\n");
}

ਖਾਲੀ ਲੱਭੋ_replace(ਅੱਖਰ *str, ਅੱਖਰ *orig, ਅੱਖਰ *rep, ਅੱਖਰ *output)
{
    ਅੱਖਰ ਬਫਰ[4096] = {0};
    ਅੱਖਰ *p;

    sprintf(ਬਫਰ, "%s", str);
    ਜੇ(!(p = ਲੜੀ_ਖੋਜ(ਬਫਰ, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        ਵਾਪਸ;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", ਬਫਰ, rep, p+ਲੜੀ_ਲੰਬਾਈ(orig));
}

ਦਸ਼ਮਲਵ sec(clock_t clocks)
{
    ਵਾਪਸ (ਦਸ਼ਮਲਵ)clocks/CLOCKS_PER_SEC;
}

ਖਾਲੀ ਸਿਖਰ_k(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ *ਸੂਚਕਾਂਕ)
{
    ਪੂਰਨ i,j;
    ਲਈ(j = 0; j < k; ++j) ਸੂਚਕਾਂਕ[j] = -1;
    ਲਈ(i = 0; i < n; ++i){
        ਪੂਰਨ curr = i;
        ਲਈ(j = 0; j < k; ++j){
            ਜੇ((ਸੂਚਕਾਂਕ[j] < 0) || a[curr] > a[ਸੂਚਕਾਂਕ[j]]){
                ਪੂਰਨ ਅਦਲਾ_ਬਦਲੀ = curr;
                curr = ਸੂਚਕਾਂਕ[j];
                ਸੂਚਕਾਂਕ[j] = ਅਦਲਾ_ਬਦਲੀ;
            }
        }
    }
}

ਖਾਲੀ error(ਅਚਲ ਅੱਖਰ *s)
{
    perror(s);
    ਪ੍ਰਤਿਜ੍ਞਾ(0);
    ਬਾਹਰ_ਨਿਕਲੋ(-1);
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *read_file(ਅੱਖਰ *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t ਮਾਪ;

    fseek(fp, 0, SEEK_END); 
    ਮਾਪ = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਪਾਠ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ+1, ਆਕਾਰ(ਅੱਖਰ));
    fread(ਪਾਠ, 1, ਮਾਪ, fp);
    fclose(fp);
    ਵਾਪਸ ਪਾਠ;
}

ਖਾਲੀ malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    ਬਾਹਰ_ਨਿਕਲੋ(-1);
}

ਖਾਲੀ file_error(ਅੱਖਰ *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    ਬਾਹਰ_ਨਿਕਲੋ(0);
}

list *ਵੰਡੋ_str(ਅੱਖਰ *s, ਅੱਖਰ delim)
{
    size_t i;
    size_t len = ਲੜੀ_ਲੰਬਾਈ(s);
    list *l = make_list();
    list_ਪਾਓ(l, s);
    ਲਈ(i = 0; i < len; ++i){
        ਜੇ(s[i] == delim){
            s[i] = '\0';
            list_ਪਾਓ(l, &(s[i+1]));
        }
    }
    ਵਾਪਸ l;
}

ਖਾਲੀ strip(ਅੱਖਰ *s)
{
    size_t i;
    size_t len = ਲੜੀ_ਲੰਬਾਈ(s);
    size_t ਵਿੱਥ = 0;
    ਲਈ(i = 0; i < len; ++i){
        ਅੱਖਰ c = s[i];
        ਜੇ(c==' '||c=='\t'||c=='\n') ++ਵਿੱਥ;
        ਨਹੀਂਤਾਂ s[i-ਵਿੱਥ] = c;
    }
    s[len-ਵਿੱਥ] = '\0';
}

ਖਾਲੀ strip_char(ਅੱਖਰ *s, ਅੱਖਰ bad)
{
    size_t i;
    size_t len = ਲੜੀ_ਲੰਬਾਈ(s);
    size_t ਵਿੱਥ = 0;
    ਲਈ(i = 0; i < len; ++i){
        ਅੱਖਰ c = s[i];
        ਜੇ(c==bad) ++ਵਿੱਥ;
        ਨਹੀਂਤਾਂ s[i-ਵਿੱਥ] = c;
    }
    s[len-ਵਿੱਥ] = '\0';
}

ਖਾਲੀ free_ptrs(ਖਾਲੀ **ptrs, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i) ਮੁਕਤ(ptrs[i]);
    ਮੁਕਤ(ptrs);
}

ਅੱਖਰ *fgetl(FILE *fp)
{
    ਜੇ(feof(fp)) ਵਾਪਸ 0;
    size_t ਮਾਪ = 512;
    ਅੱਖਰ *ਸਤਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ*ਆਕਾਰ(ਅੱਖਰ));
    ਜੇ(!fgets(ਸਤਰ, ਮਾਪ, fp)){
        ਮੁਕਤ(ਸਤਰ);
        ਵਾਪਸ 0;
    }

    size_t curr = ਲੜੀ_ਲੰਬਾਈ(ਸਤਰ);

    ਜਦੋਂ((ਸਤਰ[curr-1] != '\n') && !feof(fp)){
        ਜੇ(curr == ਮਾਪ-1){
            ਮਾਪ *= 2;
            ਸਤਰ = realloc(ਸਤਰ, ਮਾਪ*ਆਕਾਰ(ਅੱਖਰ));
            ਜੇ(!ਸਤਰ) {
                ਲਿਖੋ("%ld\n", ਮਾਪ);
                malloc_error();
            }
        }
        size_t readsize = ਮਾਪ-curr;
        ਜੇ(readsize > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) readsize = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ-1;
        fgets(&ਸਤਰ[curr], readsize, fp);
        curr = ਲੜੀ_ਲੰਬਾਈ(ਸਤਰ);
    }
    ਜੇ(ਸਤਰ[curr-1] == '\n') ਸਤਰ[curr-1] = '\0';

    ਵਾਪਸ ਸਤਰ;
}

ਪੂਰਨ read_int(ਪੂਰਨ fd)
{
    ਪੂਰਨ n = 0;
    ਪੂਰਨ ਅਗਲਾ = ਪਠ(fd, &n, ਆਕਾਰ(ਪੂਰਨ));
    ਜੇ(ਅਗਲਾ <= 0) ਵਾਪਸ -1;
    ਵਾਪਸ n;
}

ਖਾਲੀ write_int(ਪੂਰਨ fd, ਪੂਰਨ n)
{
    ਪੂਰਨ ਅਗਲਾ = ਲਿਖ(fd, &n, ਆਕਾਰ(ਪੂਰਨ));
    ਜੇ(ਅਗਲਾ <= 0) error("read failed");
}

ਪੂਰਨ read_all_fail(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ)
{
    size_t n = 0;
    ਜਦੋਂ(n < ਬਾਈਟਾਂ){
        ਪੂਰਨ ਅਗਲਾ = ਪਠ(fd, ਬਫਰ + n, ਬਾਈਟਾਂ-n);
        ਜੇ(ਅਗਲਾ <= 0) ਵਾਪਸ 1;
        n += ਅਗਲਾ;
    }
    ਵਾਪਸ 0;
}

ਪੂਰਨ write_all_fail(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ)
{
    size_t n = 0;
    ਜਦੋਂ(n < ਬਾਈਟਾਂ){
        size_t ਅਗਲਾ = ਲਿਖ(fd, ਬਫਰ + n, ਬਾਈਟਾਂ-n);
        ਜੇ(ਅਗਲਾ <= 0) ਵਾਪਸ 1;
        n += ਅਗਲਾ;
    }
    ਵਾਪਸ 0;
}

ਖਾਲੀ read_all(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ)
{
    size_t n = 0;
    ਜਦੋਂ(n < ਬਾਈਟਾਂ){
        ਪੂਰਨ ਅਗਲਾ = ਪਠ(fd, ਬਫਰ + n, ਬਾਈਟਾਂ-n);
        ਜੇ(ਅਗਲਾ <= 0) error("read failed");
        n += ਅਗਲਾ;
    }
}

ਖਾਲੀ write_all(ਪੂਰਨ fd, ਅੱਖਰ *ਬਫਰ, size_t ਬਾਈਟਾਂ)
{
    size_t n = 0;
    ਜਦੋਂ(n < ਬਾਈਟਾਂ){
        size_t ਅਗਲਾ = ਲਿਖ(fd, ਬਫਰ + n, ਬਾਈਟਾਂ-n);
        ਜੇ(ਅਗਲਾ <= 0) error("write failed");
        n += ਅਗਲਾ;
    }
}


ਅੱਖਰ *copy_string(ਅੱਖਰ *s)
{
    ਅੱਖਰ *copy = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੜੀ_ਲੰਬਾਈ(s)+1);
    strncpy(copy, s, ਲੜੀ_ਲੰਬਾਈ(s)+1);
    ਵਾਪਸ copy;
}

list *parse_csv_ਸਤਰ(ਅੱਖਰ *ਸਤਰ)
{
    list *l = make_list();
    ਅੱਖਰ *c, *p;
    ਪੂਰਨ ਵਿੱਚ = 0;
    ਲਈ(c = ਸਤਰ, p = ਸਤਰ; *c != '\0'; ++c){
        ਜੇ(*c == '"') ਵਿੱਚ = !ਵਿੱਚ;
        ਨਹੀਂਤਾਂ ਜੇ(*c == ',' && !ਵਿੱਚ){
            *c = '\0';
            list_ਪਾਓ(l, copy_string(p));
            p = c+1;
        }
    }
    list_ਪਾਓ(l, copy_string(p));
    ਵਾਪਸ l;
}

ਪੂਰਨ ਗਿਣਤੀ_fields(ਅੱਖਰ *ਸਤਰ)
{
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਪੂਰਨ ਪੂਰਾ = 0;
    ਅੱਖਰ *c;
    ਲਈ(c = ਸਤਰ; !ਪੂਰਾ; ++c){
        ਪੂਰਾ = (*c == '\0');
        ਜੇ(*c == ',' || ਪੂਰਾ) ++ਗਿਣਤੀ_2;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਦਸ਼ਮਲਵ *parse_fields(ਅੱਖਰ *ਸਤਰ, ਪੂਰਨ n)
{
    ਦਸ਼ਮਲਵ *ਖੇਤਰ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਅੱਖਰ *c, *p, *ਸਿਰਾ;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਪੂਰਨ ਪੂਰਾ = 0;
    ਲਈ(c = ਸਤਰ, p = ਸਤਰ; !ਪੂਰਾ; ++c){
        ਪੂਰਾ = (*c == '\0');
        ਜੇ(*c == ',' || ਪੂਰਾ){
            *c = '\0';
            ਖੇਤਰ[ਗਿਣਤੀ_2] = strtod(p, &ਸਿਰਾ);
            ਜੇ(p == c) ਖੇਤਰ[ਗਿਣਤੀ_2] = nan("");
            ਜੇ(ਸਿਰਾ != c && (ਸਿਰਾ != c-1 || *ਸਿਰਾ != '\r')) ਖੇਤਰ[ਗਿਣਤੀ_2] = nan(""); //DOS file formats!
            p = c+1;
            ++ਗਿਣਤੀ_2;
        }
    }
    ਵਾਪਸ ਖੇਤਰ;
}

ਦਸ਼ਮਲਵ ਜੋੜ_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਲਈ(i = 0; i < n; ++i) ਜੋੜ += a[i];
    ਵਾਪਸ ਜੋੜ;
}

ਦਸ਼ਮਲਵ ਔਸਤ_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਵਾਪਸ ਜੋੜ_array(a,n)/n;
}

ਖਾਲੀ ਔਸਤ_arrays(ਦਸ਼ਮਲਵ **a, ਪੂਰਨ n, ਪੂਰਨ els, ਦਸ਼ਮਲਵ *avg)
{
    ਪੂਰਨ i;
    ਪੂਰਨ j;
    memset(avg, 0, els*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਲਈ(j = 0; j < n; ++j){
        ਲਈ(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    ਲਈ(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

ਖਾਲੀ print_statistics(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਦਸ਼ਮਲਵ m = ਔਸਤ_array(a, n);
    ਦਸ਼ਮਲਵ v = variance_array(a, n);
    ਲਿਖੋ("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

ਦਸ਼ਮਲਵ variance_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਦਸ਼ਮਲਵ ਔਸਤ = ਔਸਤ_array(a, n);
    ਲਈ(i = 0; i < n; ++i) ਜੋੜ += (a[i] - ਔਸਤ)*(a[i]-ਔਸਤ);
    ਦਸ਼ਮਲਵ variance = ਜੋੜ/n;
    ਵਾਪਸ variance;
}

ਪੂਰਨ constrain_int(ਪੂਰਨ a, ਪੂਰਨ min, ਪੂਰਨ max)
{
    ਜੇ (a < min) ਵਾਪਸ min;
    ਜੇ (a > max) ਵਾਪਸ max;
    ਵਾਪਸ a;
}

ਦਸ਼ਮਲਵ constrain(ਦਸ਼ਮਲਵ min, ਦਸ਼ਮਲਵ max, ਦਸ਼ਮਲਵ a)
{
    ਜੇ (a < min) ਵਾਪਸ min;
    ਜੇ (a > max) ਵਾਪਸ max;
    ਵਾਪਸ a;
}

ਦਸ਼ਮਲਵ dist_array(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਪੂਰਨ n, ਪੂਰਨ sub)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਲਈ(i = 0; i < n; i += sub) ਜੋੜ += ਘਾਤ(a[i]-b[i], 2);
    ਵਾਪਸ ਵਰਗਮੂਲ(ਜੋੜ);
}

ਦਸ਼ਮਲਵ mse_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਲਈ(i = 0; i < n; ++i) ਜੋੜ += a[i]*a[i];
    ਵਾਪਸ ਵਰਗਮੂਲ(ਜੋੜ/n);
}

ਖਾਲੀ normalize_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ mu = ਔਸਤ_array(a,n);
    ਦਸ਼ਮਲਵ sigma = ਵਰਗਮੂਲ(variance_array(a,n));
    ਲਈ(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = ਔਸਤ_array(a,n);
    sigma = ਵਰਗਮੂਲ(variance_array(a,n));
}

ਖਾਲੀ translate_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n, ਦਸ਼ਮਲਵ s)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        a[i] += s;
    }
}

ਦਸ਼ਮਲਵ mag_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਲਈ(i = 0; i < n; ++i){
        ਜੋੜ += a[i]*a[i];   
    }
    ਵਾਪਸ ਵਰਗਮੂਲ(ਜੋੜ);
}

ਖਾਲੀ scale_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n, ਦਸ਼ਮਲਵ s)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

ਪੂਰਨ sample_array(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਦਸ਼ਮਲਵ ਜੋੜ = ਜੋੜ_array(a, n);
    scale_array(a, n, 1./ਜੋੜ);
    ਦਸ਼ਮਲਵ r = rand_ਇਕਸਾਰ(0, 1);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        r = r - a[i];
        ਜੇ (r <= 0) ਵਾਪਸ i;
    }
    ਵਾਪਸ n-1;
}

ਪੂਰਨ max_int_ਸੂਚਕਾਂਕ(ਪੂਰਨ *a, ਪੂਰਨ n)
{
    ਜੇ(n <= 0) ਵਾਪਸ -1;
    ਪੂਰਨ i, max_i = 0;
    ਪੂਰਨ max = a[0];
    ਲਈ(i = 1; i < n; ++i){
        ਜੇ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ਵਾਪਸ max_i;
}

ਪੂਰਨ max_ਸੂਚਕਾਂਕ(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਜੇ(n <= 0) ਵਾਪਸ -1;
    ਪੂਰਨ i, max_i = 0;
    ਦਸ਼ਮਲਵ max = a[0];
    ਲਈ(i = 1; i < n; ++i){
        ਜੇ(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    ਵਾਪਸ max_i;
}

ਪੂਰਨ int_ਸੂਚਕਾਂਕ(ਪੂਰਨ *a, ਪੂਰਨ val, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਜੇ(a[i] == val) ਵਾਪਸ i;
    }
    ਵਾਪਸ -1;
}

ਪੂਰਨ rand_int(ਪੂਰਨ min, ਪੂਰਨ max)
{
    ਜੇ (max < min){
        ਪੂਰਨ s = min;
        min = max;
        max = s;
    }
    ਪੂਰਨ r = (rand()%(max - min + 1)) + min;
    ਵਾਪਸ r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
ਦਸ਼ਮਲਵ rand_ਸਾਧਾਰਨ()
{
    ਸਥਿਰ ਪੂਰਨ have_ਵਾਧੂ = 0;
    ਸਥਿਰ ਦੁੱਗਣਾ rand1, rand2;

    ਜੇ(have_ਵਾਧੂ)
    {
        have_ਵਾਧੂ = 0;
        ਵਾਪਸ ਵਰਗਮੂਲ(rand1) * ਜੀਵਾ(rand2);
    }

    have_ਵਾਧੂ = 1;

    rand1 = rand() / ((ਦੁੱਗਣਾ) RAND_MAX);
    ਜੇ(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * ਲਘੂਗਣਕ(rand1);
    rand2 = (rand() / ((ਦੁੱਗਣਾ) RAND_MAX)) * TWO_PI;

    ਵਾਪਸ ਵਰਗਮੂਲ(rand1) * ਕੋਜੀਵਾ(rand2);
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

size_t rand_ਮਾਪ_t()
{
    ਵਾਪਸ  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

ਦਸ਼ਮਲਵ rand_ਇਕਸਾਰ(ਦਸ਼ਮਲਵ min, ਦਸ਼ਮਲਵ max)
{
    ਜੇ(max < min){
        ਦਸ਼ਮਲਵ ਅਦਲਾ_ਬਦਲੀ = min;
        min = max;
        max = ਅਦਲਾ_ਬਦਲੀ;
    }
    ਵਾਪਸ ((ਦਸ਼ਮਲਵ)rand()/RAND_MAX * (max - min)) + min;
}

ਦਸ਼ਮਲਵ rand_scale(ਦਸ਼ਮਲਵ s)
{
    ਦਸ਼ਮਲਵ scale = rand_ਇਕਸਾਰ(1, s);
    ਜੇ(rand()%2) ਵਾਪਸ scale;
    ਵਾਪਸ 1./scale;
}

ਦਸ਼ਮਲਵ **one_hot_encode(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n, ਪੂਰਨ k)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ **t = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    ਲਈ(i = 0; i < n; ++i){
        t[i] = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(k, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਪੂਰਨ ਸੂਚਕਾਂਕ = (ਪੂਰਨ)a[i];
        t[i][ਸੂਚਕਾਂਕ] = 1;
    }
    ਵਾਪਸ t;
}

