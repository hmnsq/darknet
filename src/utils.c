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
    यदि (gettimeofday(&time,रिक्त)){
        प्रत्यागम 0;
    }
    प्रत्यागम (द्विगुण)time.tv_sec + (द्विगुण)time.tv_usec * .000001;
}

पूर्णाङ्क *read_intlist(वर्ण *gpu_list, पूर्णाङ्क *ngpus, पूर्णाङ्क d)
{
    पूर्णाङ्क *gpus = 0;
    यदि(gpu_list){
        पूर्णाङ्क len = strlen(gpu_list);
        *ngpus = 1;
        पूर्णाङ्क i;
        प्रति(i = 0; i < len; ++i){
            यदि (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = calloc(*ngpus, परिमाण(पूर्णाङ्क));
        प्रति(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } अन्यथा {
        gpus = calloc(1, परिमाण(भिन्न));
        *gpus = d;
        *ngpus = 1;
    }
    प्रत्यागम gpus;
}

पूर्णाङ्क *read_map(वर्ण *filename)
{
    पूर्णाङ्क n = 0;
    पूर्णाङ्क *map = 0;
    वर्ण *str;
    FILE *file = fopen(filename, "r");
    यदि(!file) file_error(filename);
    यावत्((str=fgetl(file))){
        ++n;
        map = realloc(map, n*परिमाण(पूर्णाङ्क));
        map[n-1] = atoi(str);
    }
    प्रत्यागम map;
}

शून्य sorta_shuffle(शून्य *arr, size_t n, size_t size, size_t sections)
{
    size_t i;
    प्रति(i = 0; i < sections; ++i){
        size_t start = n*i/sections;
        size_t end = n*(i+1)/sections;
        size_t अंक = end-start;
        shuffle(arr+(start*size), अंक, size);
    }
}

शून्य shuffle(शून्य *arr, size_t n, size_t size)
{
    size_t i;
    शून्य *swp = calloc(1, size);
    प्रति(i = 0; i < n-1; ++i){
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
    प्रति(i = min; i < max; ++i){
        inds[i] = i;
    }
    प्रति(i = min; i < max-1; ++i){
        पूर्णाङ्क swap = inds[i];
        पूर्णाङ्क index = i + rand()%(max-i);
        inds[i] = inds[index];
        inds[index] = swap;
    }
    प्रत्यागम inds;
}

शून्य del_arg(पूर्णाङ्क argc, वर्ण **argv, पूर्णाङ्क index)
{
    पूर्णाङ्क i;
    प्रति(i = index; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

पूर्णाङ्क find_arg(पूर्णाङ्क argc, वर्ण* argv[], वर्ण *arg)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < argc; ++i) {
        यदि(!argv[i]) अनुवर्त;
        यदि(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            प्रत्यागम 1;
        }
    }
    प्रत्यागम 0;
}

पूर्णाङ्क find_int_arg(पूर्णाङ्क argc, वर्ण **argv, वर्ण *arg, पूर्णाङ्क def)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < argc-1; ++i){
        यदि(!argv[i]) अनुवर्त;
        यदि(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            भङ्ग;
        }
    }
    प्रत्यागम def;
}

भिन्न find_float_arg(पूर्णाङ्क argc, वर्ण **argv, वर्ण *arg, भिन्न def)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < argc-1; ++i){
        यदि(!argv[i]) अनुवर्त;
        यदि(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            भङ्ग;
        }
    }
    प्रत्यागम def;
}

वर्ण *find_char_arg(पूर्णाङ्क argc, वर्ण **argv, वर्ण *arg, वर्ण *def)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < argc-1; ++i){
        यदि(!argv[i]) अनुवर्त;
        यदि(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            भङ्ग;
        }
    }
    प्रत्यागम def;
}


वर्ण *basecfg(वर्ण *cfgfile)
{
    वर्ण *c = cfgfile;
    वर्ण *next;
    यावत्((next = strchr(c, '/')))
    {
        c = next+1;
    }
    c = copy_string(c);
    next = strchr(c, '.');
    यदि (next) *next = 0;
    प्रत्यागम c;
}

पूर्णाङ्क alphanum_to_int(वर्ण c)
{
    प्रत्यागम (c < 58) ? c - 48 : c-87;
}
वर्ण int_to_alphanum(पूर्णाङ्क i)
{
    यदि (i == 36) प्रत्यागम '.';
    प्रत्यागम (i < 10) ? i + 48 : i + 87;
}

शून्य pm(पूर्णाङ्क M, पूर्णाङ्क N, भिन्न *A)
{
    पूर्णाङ्क i,j;
    प्रति(i =0 ; i < M; ++i){
        printf("%d ", i+1);
        प्रति(j = 0; j < N; ++j){
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
    यदि(!(p = strstr(buffer, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        प्रत्यागम;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p+strlen(orig));
}

भिन्न sec(clock_t clocks)
{
    प्रत्यागम (भिन्न)clocks/CLOCKS_PER_SEC;
}

शून्य शीर्ष_k(भिन्न *a, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क *index)
{
    पूर्णाङ्क i,j;
    प्रति(j = 0; j < k; ++j) index[j] = -1;
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क curr = i;
        प्रति(j = 0; j < k; ++j){
            यदि((index[j] < 0) || a[curr] > a[index[j]]){
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
    प्रत्यागम text;
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
    list_घाला(l, s);
    प्रति(i = 0; i < len; ++i){
        यदि(s[i] == delim){
            s[i] = '\0';
            list_घाला(l, &(s[i+1]));
        }
    }
    प्रत्यागम l;
}

शून्य strip(वर्ण *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    प्रति(i = 0; i < len; ++i){
        वर्ण c = s[i];
        यदि(c==' '||c=='\t'||c=='\n') ++offset;
        अन्यथा s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

शून्य strip_char(वर्ण *s, वर्ण bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    प्रति(i = 0; i < len; ++i){
        वर्ण c = s[i];
        यदि(c==bad) ++offset;
        अन्यथा s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

शून्य free_ptrs(शून्य **ptrs, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i) मुक्त(ptrs[i]);
    मुक्त(ptrs);
}

वर्ण *fgetl(FILE *fp)
{
    यदि(feof(fp)) प्रत्यागम 0;
    size_t size = 512;
    वर्ण *ओळ = malloc(size*परिमाण(वर्ण));
    यदि(!fgets(ओळ, size, fp)){
        मुक्त(ओळ);
        प्रत्यागम 0;
    }

    size_t curr = strlen(ओळ);

    यावत्((ओळ[curr-1] != '\n') && !feof(fp)){
        यदि(curr == size-1){
            size *= 2;
            ओळ = realloc(ओळ, size*परिमाण(वर्ण));
            यदि(!ओळ) {
                printf("%ld\n", size);
                malloc_error();
            }
        }
        size_t readsize = size-curr;
        यदि(readsize > INT_MAX) readsize = INT_MAX-1;
        fgets(&ओळ[curr], readsize, fp);
        curr = strlen(ओळ);
    }
    यदि(ओळ[curr-1] == '\n') ओळ[curr-1] = '\0';

    प्रत्यागम ओळ;
}

पूर्णाङ्क read_int(पूर्णाङ्क fd)
{
    पूर्णाङ्क n = 0;
    पूर्णाङ्क next = पठ(fd, &n, परिमाण(पूर्णाङ्क));
    यदि(next <= 0) प्रत्यागम -1;
    प्रत्यागम n;
}

शून्य write_int(पूर्णाङ्क fd, पूर्णाङ्क n)
{
    पूर्णाङ्क next = लिख(fd, &n, परिमाण(पूर्णाङ्क));
    यदि(next <= 0) error("read failed");
}

पूर्णाङ्क read_all_fail(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    यावत्(n < bytes){
        पूर्णाङ्क next = पठ(fd, buffer + n, bytes-n);
        यदि(next <= 0) प्रत्यागम 1;
        n += next;
    }
    प्रत्यागम 0;
}

पूर्णाङ्क write_all_fail(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    यावत्(n < bytes){
        size_t next = लिख(fd, buffer + n, bytes-n);
        यदि(next <= 0) प्रत्यागम 1;
        n += next;
    }
    प्रत्यागम 0;
}

शून्य read_all(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    यावत्(n < bytes){
        पूर्णाङ्क next = पठ(fd, buffer + n, bytes-n);
        यदि(next <= 0) error("read failed");
        n += next;
    }
}

शून्य write_all(पूर्णाङ्क fd, वर्ण *buffer, size_t bytes)
{
    size_t n = 0;
    यावत्(n < bytes){
        size_t next = लिख(fd, buffer + n, bytes-n);
        यदि(next <= 0) error("write failed");
        n += next;
    }
}


वर्ण *copy_string(वर्ण *s)
{
    वर्ण *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    प्रत्यागम copy;
}

list *parse_csv_ओळ(वर्ण *ओळ)
{
    list *l = make_list();
    वर्ण *c, *p;
    पूर्णाङ्क in = 0;
    प्रति(c = ओळ, p = ओळ; *c != '\0'; ++c){
        यदि(*c == '"') in = !in;
        अन्यथा यदि(*c == ',' && !in){
            *c = '\0';
            list_घाला(l, copy_string(p));
            p = c+1;
        }
    }
    list_घाला(l, copy_string(p));
    प्रत्यागम l;
}

पूर्णाङ्क मोजणी_fields(वर्ण *ओळ)
{
    पूर्णाङ्क मोजणी = 0;
    पूर्णाङ्क done = 0;
    वर्ण *c;
    प्रति(c = ओळ; !done; ++c){
        done = (*c == '\0');
        यदि(*c == ',' || done) ++मोजणी;
    }
    प्रत्यागम मोजणी;
}

भिन्न *parse_fields(वर्ण *ओळ, पूर्णाङ्क n)
{
    भिन्न *field = calloc(n, परिमाण(भिन्न));
    वर्ण *c, *p, *end;
    पूर्णाङ्क मोजणी = 0;
    पूर्णाङ्क done = 0;
    प्रति(c = ओळ, p = ओळ; !done; ++c){
        done = (*c == '\0');
        यदि(*c == ',' || done){
            *c = '\0';
            field[मोजणी] = strtod(p, &end);
            यदि(p == c) field[मोजणी] = nan("");
            यदि(end != c && (end != c-1 || *end != '\r')) field[मोजणी] = nan(""); //DOS file formats!
            p = c+1;
            ++मोजणी;
        }
    }
    प्रत्यागम field;
}

भिन्न sum_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    प्रति(i = 0; i < n; ++i) sum += a[i];
    प्रत्यागम sum;
}

भिन्न mean_array(भिन्न *a, पूर्णाङ्क n)
{
    प्रत्यागम sum_array(a,n)/n;
}

शून्य mean_arrays(भिन्न **a, पूर्णाङ्क n, पूर्णाङ्क els, भिन्न *avg)
{
    पूर्णाङ्क i;
    पूर्णाङ्क j;
    memset(avg, 0, els*परिमाण(भिन्न));
    प्रति(j = 0; j < n; ++j){
        प्रति(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    प्रति(i = 0; i < els; ++i){
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
    प्रति(i = 0; i < n; ++i) sum += (a[i] - mean)*(a[i]-mean);
    भिन्न variance = sum/n;
    प्रत्यागम variance;
}

पूर्णाङ्क constrain_int(पूर्णाङ्क a, पूर्णाङ्क min, पूर्णाङ्क max)
{
    यदि (a < min) प्रत्यागम min;
    यदि (a > max) प्रत्यागम max;
    प्रत्यागम a;
}

भिन्न constrain(भिन्न min, भिन्न max, भिन्न a)
{
    यदि (a < min) प्रत्यागम min;
    यदि (a > max) प्रत्यागम max;
    प्रत्यागम a;
}

भिन्न dist_array(भिन्न *a, भिन्न *b, पूर्णाङ्क n, पूर्णाङ्क sub)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    प्रति(i = 0; i < n; i += sub) sum += pow(a[i]-b[i], 2);
    प्रत्यागम sqrt(sum);
}

भिन्न mse_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    प्रति(i = 0; i < n; ++i) sum += a[i]*a[i];
    प्रत्यागम sqrt(sum/n);
}

शून्य normalize_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न mu = mean_array(a,n);
    भिन्न sigma = sqrt(variance_array(a,n));
    प्रति(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = sqrt(variance_array(a,n));
}

शून्य translate_array(भिन्न *a, पूर्णाङ्क n, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        a[i] += s;
    }
}

भिन्न mag_array(भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    भिन्न sum = 0;
    प्रति(i = 0; i < n; ++i){
        sum += a[i]*a[i];   
    }
    प्रत्यागम sqrt(sum);
}

शून्य scale_array(भिन्न *a, पूर्णाङ्क n, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

पूर्णाङ्क sample_array(भिन्न *a, पूर्णाङ्क n)
{
    भिन्न sum = sum_array(a, n);
    scale_array(a, n, 1./sum);
    भिन्न r = rand_uniform(0, 1);
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        r = r - a[i];
        यदि (r <= 0) प्रत्यागम i;
    }
    प्रत्यागम n-1;
}

पूर्णाङ्क max_int_index(पूर्णाङ्क *a, पूर्णाङ्क n)
{
    यदि(n <= 0) प्रत्यागम -1;
    पूर्णाङ्क i, max_i = 0;
    पूर्णाङ्क max = a[0];
    प्रति(i = 1; i < n; ++i){
        यदि(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    प्रत्यागम max_i;
}

पूर्णाङ्क max_index(भिन्न *a, पूर्णाङ्क n)
{
    यदि(n <= 0) प्रत्यागम -1;
    पूर्णाङ्क i, max_i = 0;
    भिन्न max = a[0];
    प्रति(i = 1; i < n; ++i){
        यदि(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    प्रत्यागम max_i;
}

पूर्णाङ्क int_index(पूर्णाङ्क *a, पूर्णाङ्क val, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        यदि(a[i] == val) प्रत्यागम i;
    }
    प्रत्यागम -1;
}

पूर्णाङ्क rand_int(पूर्णाङ्क min, पूर्णाङ्क max)
{
    यदि (max < min){
        पूर्णाङ्क s = min;
        min = max;
        max = s;
    }
    पूर्णाङ्क r = (rand()%(max - min + 1)) + min;
    प्रत्यागम r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
भिन्न rand_normal()
{
    स्थिर पूर्णाङ्क haveSpare = 0;
    स्थिर द्विगुण rand1, rand2;

    यदि(haveSpare)
    {
        haveSpare = 0;
        प्रत्यागम sqrt(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = rand() / ((द्विगुण) RAND_MAX);
    यदि(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((द्विगुण) RAND_MAX)) * TWO_PI;

    प्रत्यागम sqrt(rand1) * cos(rand2);
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
    प्रत्यागम  ((size_t)(rand()&0xff) << 56) | 
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
    यदि(max < min){
        भिन्न swap = min;
        min = max;
        max = swap;
    }
    प्रत्यागम ((भिन्न)rand()/RAND_MAX * (max - min)) + min;
}

भिन्न rand_scale(भिन्न s)
{
    भिन्न scale = rand_uniform(1, s);
    यदि(rand()%2) प्रत्यागम scale;
    प्रत्यागम 1./scale;
}

भिन्न **one_hot_encode(भिन्न *a, पूर्णाङ्क n, पूर्णाङ्क k)
{
    पूर्णाङ्क i;
    भिन्न **t = calloc(n, परिमाण(भिन्न*));
    प्रति(i = 0; i < n; ++i){
        t[i] = calloc(k, परिमाण(भिन्न));
        पूर्णाङ्क index = (पूर्णाङ्क)a[i];
        t[i][index] = 1;
    }
    प्रत्यागम t;
}

