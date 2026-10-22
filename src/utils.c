#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <float.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/time.h>

#अन्तर्भाव "utils.h"


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

द्विगुण what_time_is_it_now()
{
    संरचना timeval time;
    सचे (gettimeofday(&time,रिक्त)){
        पटिनिवत्त 0;
    }
    पटिनिवत्त (द्विगुण)time.tv_sec + (द्विगुण)time.tv_usec * .000001;
}

पूर्णाङ्क *read_intlist(वर्ण *gpu_list, पूर्णाङ्क *ngpus, पूर्णाङ्क d)
{
    पूर्णाङ्क *gpus = 0;
    सचे(gpu_list){
        पूर्णाङ्क len = strlen(gpu_list);
        *ngpus = 1;
        पूर्णाङ्क i;
        अत्थाय(i = 0; i < len; ++i){
            सचे (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, परिमाण(पूर्णाङ्क));
        अत्थाय(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } अञ्ञथा {
        gpus = calloc(1, परिमाण(भिन्न));
        *gpus = d;
        *ngpus = 1;
    }
    पटिनिवत्त gpus;
}

पूर्णाङ्क *read_map(वर्ण *filename)
{
    पूर्णाङ्क n = 0;
    पूर्णाङ्क *map = 0;
    वर्ण *str;
    FILE *file = fopen(filename, "r");
    सचे(!file) file_error(filename);
    याव((str=fgetl(file))){
        ++n;
        map = realloc(map, n*परिमाण(पूर्णाङ्क));
        map[n-1] = atoi(str);
    }
    पटिनिवत्त map;
}

शून्य sorta_shuffle(शून्य *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    अत्थाय(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t अङ्कः = end-start;
        shuffle(arr+(start*size), अङ्कः, size);
    }
}

शून्य shuffle(शून्य *arr, size_t n, size_t size)
{
    size_t i;
    शून्य *swp = calloc(1, size);
    अत्थाय(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        memcpy(swp,          arr+(j*size), size);
        memcpy(arr+(j*size), arr+(i*size), size);
        memcpy(arr+(i*size), swp,          size);
    }
}

पूर्णाङ्क *random_index_order(पूर्णाङ्क min, पूर्णाङ्क max)
{
    पूर्णाङ्क *inds = calloc(max-min, परिमाण(पूर्णाङ्क));
    पूर्णाङ्क i;
    अत्थाय(i = min; i < max; ++i){
        inds[i] = i;
    }
    अत्थाय(i = min; i < max-1; ++i){
        पूर्णाङ्क swap = inds[i];
        पूर्णाङ्क index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    पटिनिवत्त inds;
}

शून्य del_arg(पूर्णाङ्क argc, वर्ण **argv, पूर्णाङ्क index)
{
    पूर्णाङ्क i;
    अत्थाय(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

पूर्णाङ्क find_arg(पूर्णाङ्क argc, वर्ण* argv[], वर्ण *arg)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < argc; ++i) {
        सचे(!argv[i]) अनुवर्त;
        सचे(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            पटिनिवत्त 1;
        }
    }
    पटिनिवत्त 0;
}

पूर्णाङ्क find_int_arg(पूर्णाङ्क argc, वर्ण **argv, वर्ण *arg, पूर्णाङ्क def)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < argc-1; ++i){
        सचे(!argv[i]) अनुवर्त;
        सचे(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            भङ्ग;
        }
    }
    पटिनिवत्त def;
}

भिन्न find_float_arg(पूर्णाङ्क argc, वर्ण **argv, वर्ण *arg, भिन्न def)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < argc-1; ++i){
        सचे(!argv[i]) अनुवर्त;
        सचे(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            भङ्ग;
        }
    }
    पटिनिवत्त def;
}

वर्ण *find_char_arg(पूर्णाङ्क argc, वर्ण **argv, वर्ण *arg, वर्ण *def)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < argc-1; ++i){
        सचे(!argv[i]) अनुवर्त;
        सचे(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            भङ्ग;
        }
    }
    पटिनिवत्त def;
}


वर्ण *basecfg(वर्ण *cfgfile)
{
    वर्ण *c = cfgfile;
    वर्ण *next;
    याव((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    सचे (next) *next = 0;
    पटिनिवत्त c;
}

पूर्णाङ्क alphanum_to_int(वर्ण c)
{
    पटिनिवत्त (c < 58) ? c - 48 : c-87;
}
वर्ण int_to_alphanum(पूर्णाङ्क i)
{
    सचे (i == 36) पटिनिवत्त '.';
    पटिनिवत्त (i < 10) ? i + 48 : i + 87;
}

शून्य pm(पूर्णाङ्क M, पूर्णाङ्क N, भिन्न *A)
{
    पूर्णाङ्क i,j;
    अत्थाय(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        अत्थाय(j = 0; j < N; ++j){
            printf("%2.4f, ", A[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");
}

शून्य find_replace(वर्ण *str, वर्ण *orig, वर्ण *rep, वर्ण *output)
{
    वर्ण buffer[4096] = {0};
    वर्ण *p;

    sprintf(buffer, "%s", str);
    सचे(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        पटिनिवत्त;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

भिन्न sec(clock_t clocks)
{
    पटिनिवत्त (भिन्न)clocks/CLOCKS_PER_SEC;
}

शून्य शिखरम्_k(भिन्न *a, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क *index)
{
    पूर्णाङ्क i,j;
    अत्थाय(j = 0; j < k; ++j) index[j] = -1;
    अत्थाय(i = 0; i < n; ++i){
        पूर्णाङ्क curr = i;
        अत्थाय(j = 0; j < k; ++j){
            सचे((index[j] < 0) || a[curr] > a[index[j]]){
                पूर्णाङ्क swap = curr;
                curr = index[j];
                index[j] = swap;
            }
        }
    }
}

शून्य error(ध्रुव वर्ण *s)
{
    perror(s);
    प्रतिज्ञा(0);
    exit(-1);
}

unsigned वर्ण *read_file(वर्ण *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t size;

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    unsigned वर्ण *text = calloc(size+1, परिमाण(वर्ण));
    fread(text, 1, size, fp);
    fclose(fp);
    पटिनिवत्त text;
}

शून्य malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

शून्य file_error(वर्ण *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    exit(0);
}

list *split_str(वर्ण *s, वर्ण delim)
{
    size_t i;
    size_t len = strlen(s);
    list *l = make_list();
    list_निवेशय(l, s);
    अत्थाय(i = 0; i < len; ++i){
        सचे(s[i] == delim){
            s[i] = '\0';
            list_निवेशय(l, &(s[i+1]));
        }
    }
    पटिनिवत्त l;
}

शून्य strip(वर्ण *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    अत्थाय(i = 0; i < len; ++i){
        वर्ण c = s[i];
        सचे(c==' '||c=='\t'||c=='\n') ++offset;
        अञ्ञथा s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

शून्य strip_char(वर्ण *s, वर्ण bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    अत्थाय(i = 0; i < len; ++i){
        वर्ण c = s[i];
        सचे(c==bad) ++offset;
        अञ्ञथा s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

शून्य free_ptrs(शून्य **ptrs, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i) मुक्त(ptrs[i]);
    मुक्त(ptrs);
}

वर्ण *fgetl(FILE *fp)
{
    सचे(feof(fp)) पटिनिवत्त 0;
    size_t size = 512;
    वर्ण *पङ्क्तिः = malloc(size*परिमाण(वर्ण));
    सचे(!fgets(पङ्क्तिः, size, fp)){
        मुक्त(पङ्क्तिः);
        पटिनिवत्त 0;
    }

    size_t curr = strlen(पङ्क्तिः);

    याव((पङ्क्तिः[curr-1] != '\n') && !feof(fp)){
        सचे(curr == size-1){
            size *= 2;
            पङ्क्तिः = realloc(पङ्क्तिः, size*परिमाण(वर्ण));
            सचे(!पङ्क्तिः) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        सचे(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&पङ्क्तिः[curr], readsize, fp);
        curr = strlen(पङ्क्तिः);
    }
    सचे(पङ्क्तिः[curr-1] == '\n') पङ्क्तिः[curr-1] = '\0';

    पटिनिवत्त पङ्क्तिः;
}

पूर्णाङ्क read_int(पूर्णाङ्क fd)
{
    पूर्णाङ्क n = 0;
    पूर्णाङ्क next = पठ(fd, &n, परिमाण(पूर्णाङ्क));
    सचे(next <= 0) पटिनिवत्त -1;
    पटिनिवत्त n;
}

शून्य write_int(पूर्णाङ्क fd, पूर्णाङ्क n)
{
    पूर्णाङ्क next = लिख(fd, &n, परिमाण(पूर्णाङ्क));
    सचे(next <= 0) error("read failed");
}

पूर्णाङ्क read_all_fail(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    याव(n < bytes){
        पूर्णाङ्क next = पठ(fd, buffer + n, bytes-n);
        सचे(next <= 0) पटिनिवत्त 1;
        n += next;
    }
    पटिनिवत्त 0;
}

पूर्णाङ्क write_all_fail(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    याव(n < bytes){
        size_t next = लिख(fd, buffer + n, bytes-n);
        सचे(next <= 0) पटिनिवत्त 1;
        n += next;
    }
    पटिनिवत्त 0;
}

शून्य read_all(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    याव(n < bytes){
        पूर्णाङ्क next = पठ(fd, buffer + n, bytes-n);
        सचे(next <= 0) error("read failed");
        n += next;
    }
}

शून्य write_all(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    याव(n < bytes){
        size_t next = लिख(fd, buffer + n, bytes-n);
        सचे(next <= 0) error("write failed");
        n += next;
    }
}


वर्ण *copy_string(वर्ण *s)
{
    वर्ण *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    पटिनिवत्त copy;
}

list *parse_csv_पङ्क्तिः(वर्ण *पङ्क्तिः)
{
    list *l = make_list();
    वर्ण *c, *p;
    पूर्णाङ्क in = 0;
    अत्थाय(c = पङ्क्तिः, p = पङ्क्तिः; *c != '\0'; ++c){
        सचे(*c == '"') in = !in;
        अञ्ञथा सचे(*c == ',' && !in){
            *c = '\0';
            list_निवेशय(l, copy_string(p));
            p = c+1;
        }
    }
    list_निवेशय(l, copy_string(p));
    पटिनिवत्त l;
}

पूर्णाङ्क गणनम्_fields(वर्ण *पङ्क्तिः)
{
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क done = 0;
    वर्ण *c;
    अत्थाय(c = पङ्क्तिः; !done; ++c){
        done = (*c == '\0');
        सचे(*c == ',' || done) ++गणनम्;
    }
    पटिनिवत्त गणनम्;
}

भिन्न *parse_fields(वर्ण *पङ्क्तिः, पूर्णाङ्क n)
{
    भिन्न *field = calloc(n, परिमाण(भिन्न));
    वर्ण *c, *p, *end;
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क done = 0;
    अत्थाय(c = पङ्क्तिः, p = पङ्क्तिः; !done; ++c){
        done = (*c == '\0');
        सचे(*c == ',' || done){
            *c = '\0';
            field[गणनम्] = strtod(p, &end);
            सचे(p == c) field[गणनम्] = nan("");
            सचे(end != c && (end != c-1 || *end != '\r')) field[गणनम्] = nan(""); //DOS file formats!
            p = c+1;
            ++गणनम्;
        }
    }
    पटिनिवत्त field;
}

भिन्न sum_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    अत्थाय(i = 0; i < n; ++i) sum += a[i];
    पटिनिवत्त sum;
}

भिन्न mean_array(भिन्न *a, पूर्णाङ्क n)
{
    पटिनिवत्त sum_array(a,n)/n;
}

शून्य mean_arrays(भिन्न **a, पूर्णाङ्क n, पूर्णाङ्क els, भिन्न *avg)
{
    पूर्णाङ्क i;
    पूर्णाङ्क j;
    memset(avg, 0, els*परिमाण(भिन्न));
    अत्थाय(j = 0; j < n; ++j){
        अत्थाय(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    अत्थाय(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

शून्य print_statistics(भिन्न *a, पूर्णाङ्क n)
{
    भिन्न m = mean_array(a, n);
    भिन्न v = variance_array(a, n);
    printf("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

भिन्न variance_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    भिन्न mean = mean_array(a, n);
    अत्थाय(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    भिन्न variance = sum/n;
    पटिनिवत्त variance;
}

पूर्णाङ्क constrain_int(पूर्णाङ्क a, पूर्णाङ्क min, पूर्णाङ्क max)
{
    सचे (a < min) पटिनिवत्त min;
    सचे (a > max) पटिनिवत्त max;
    पटिनिवत्त a;
}

भिन्न constrain(भिन्न min, भिन्न max, भिन्न a)
{
    सचे (a < min) पटिनिवत्त min;
    सचे (a > max) पटिनिवत्त max;
    पटिनिवत्त a;
}

भिन्न dist_array(भिन्न *a, भिन्न *b, पूर्णाङ्क n, पूर्णाङ्क sub)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    अत्थाय(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    पटिनिवत्त sqrt(sum);
}

भिन्न mse_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    अत्थाय(i = 0; i < n; ++i) sum += a[i]*a[i];
    पटिनिवत्त sqrt(sum/n);
}

शून्य normalize_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न mu = mean_array(a,n);
    भिन्न sigma = sqrt(variance_array(a,n));
    अत्थाय(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

शून्य translate_array(भिन्न *a, पूर्णाङ्क n, भिन्न s)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i){
        a[i] += s;
    }
}

भिन्न mag_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    अत्थाय(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    पटिनिवत्त sqrt(sum);
}

शून्य scale_array(भिन्न *a, पूर्णाङ्क n, भिन्न s)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

पूर्णाङ्क sample_array(भिन्न *a, पूर्णाङ्क n)
{
    भिन्न sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    भिन्न r = rand_uniform(0, 1);
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i){
        r = r - a[i];
        सचे (r <= 0) पटिनिवत्त i;
    }
    पटिनिवत्त n-1;
}

पूर्णाङ्क max_int_index(पूर्णाङ्क *a, पूर्णाङ्क n)
{
    सचे(n <= 0) पटिनिवत्त -1;
    पूर्णाङ्क i, max_i = 0;
    पूर्णाङ्क max = a[0];
    अत्थाय(i = 1; i < n; ++i){
        सचे(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    पटिनिवत्त max_i;
}

पूर्णाङ्क max_index(भिन्न *a, पूर्णाङ्क n)
{
    सचे(n <= 0) पटिनिवत्त -1;
    पूर्णाङ्क i, max_i = 0;
    भिन्न max = a[0];
    अत्थाय(i = 1; i < n; ++i){
        सचे(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    पटिनिवत्त max_i;
}

पूर्णाङ्क int_index(पूर्णाङ्क *a, पूर्णाङ्क val, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i){
        सचे(a[i] == val) पटिनिवत्त i;
    }
    पटिनिवत्त -1;
}

पूर्णाङ्क rand_int(पूर्णाङ्क min, पूर्णाङ्क max)
{
    सचे (max < min){
        पूर्णाङ्क s = min;
        min = max;
        max = s;
    }
    पूर्णाङ्क r = (rand()%(max - min + 1)) + min;
    पटिनिवत्त r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
भिन्न rand_normal()
{
    स्थिर पूर्णाङ्क haveSpare = 0;
    स्थिर द्विगुण rand1, rand2;

    सचे(haveSpare)
    {
        haveSpare = 0;
        पटिनिवत्त sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((द्विगुण) RAND_MAX);
    सचे(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((द्विगुण) RAND_MAX)) * TWO_PI;

    पटिनिवत्त sqrt(rand1) * cos(rand2);
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
    पटिनिवत्त  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

भिन्न rand_uniform(भिन्न min, भिन्न max)
{
    सचे(max < min){
        भिन्न swap = min;
        min = max;
        max = swap;
    }
    पटिनिवत्त ((भिन्न)rand()/RAND_MAX * (max - min)) + min;
}

भिन्न rand_scale(भिन्न s)
{
    भिन्न scale = rand_uniform(1, s);
    सचे(rand()%2) पटिनिवत्त scale;
    पटिनिवत्त 1./scale;
}

भिन्न **one_hot_encode(भिन्न *a, पूर्णाङ्क n, पूर्णाङ्क k)
{
    पूर्णाङ्क i;
    भिन्न **t = calloc(n, परिमाण(भिन्न*));
    अत्थाय(i = 0; i < n; ++i){
        t[i] = calloc(k, परिमाण(भिन्न));
        पूर्णाङ्क index = (पूर्णाङ्क)a[i];
        t[i][index] = 1;
    }
    पटिनिवत्त t;
}

