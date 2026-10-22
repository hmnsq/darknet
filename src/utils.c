#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <math.h>
#ضمن <assert.h>
#ضمن <unistd.h>
#ضمن <float.h>
#ضمن <limits.h>
#ضمن <time.h>
#ضمن <sys/time.h>

#ضمن "utils.h"


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

مزدوج what_وخت_is_it_now()
{
    بنية timeval وخت;
    إذا (gettimeofday(&وخت,عدم)){
        أرجع 0;
    }
    أرجع (مزدوج)وخت.tv_sec + (مزدوج)وخت.tv_usec * .000001;
}

صحيح *read_intlist(حرف *gpu_list, صحيح *ngpus, صحيح d)
{
    صحيح *gpus = 0;
    إذا(gpu_list){
        صحيح len = مزي_اوږدوالی(gpu_list);
        *ngpus = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            إذا (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = حافظه_صفر_ورکړه(*ngpus, حجم(صحيح));
        لكل(i = 0; i < *ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } وإلا {
        gpus = حافظه_صفر_ورکړه(1, حجم(عائم));
        *gpus = d;
        *ngpus = 1;
    }
    أرجع gpus;
}

صحيح *read_map(حرف *filename)
{
    صحيح n = 0;
    صحيح *map = 0;
    حرف *str;
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    طالما((str=fgetl(file))){
        ++n;
        map = realloc(map, n*حجم(صحيح));
        map[n-1] = atoi(str);
    }
    أرجع map;
}

فراغ sorta_shuffle(فراغ *arr, size_t n, size_t اندازه, size_t sections)
{
    size_t i;
    لكل(i = 0; i < sections; ++i){
        size_t پیل = n*i/sections;
        size_t پای_ټکی = n*(i+1)/sections;
        size_t عدد = پای_ټکی-پیل;
        shuffle(arr+(پیل*اندازه), عدد, اندازه);
    }
}

فراغ shuffle(فراغ *arr, size_t n, size_t اندازه)
{
    size_t i;
    فراغ *swp = حافظه_صفر_ورکړه(1, اندازه);
    لكل(i = 0; i < n-1; ++i){
        size_t j = i + rand()/(RAND_MAX / (n-i)+1);
        حافظه_نقل(swp,          arr+(j*اندازه), اندازه);
        حافظه_نقل(arr+(j*اندازه), arr+(i*اندازه), اندازه);
        حافظه_نقل(arr+(i*اندازه), swp,          اندازه);
    }
}

صحيح *random_شاخص_ترتیب(صحيح min, صحيح max)
{
    صحيح *inds = حافظه_صفر_ورکړه(max-min, حجم(صحيح));
    صحيح i;
    لكل(i = min; i < max; ++i){
        inds[i] = i;
    }
    لكل(i = min; i < max-1; ++i){
        صحيح بدلون = inds[i];
        صحيح شاخص = i + rand()%(max-i);
        inds[i] = inds[شاخص];
        inds[شاخص] = بدلون;
    }
    أرجع inds;
}

فراغ del_arg(صحيح argc, حرف **argv, صحيح شاخص)
{
    صحيح i;
    لكل(i = شاخص; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

صحيح پیدا_کړه_arg(صحيح argc, حرف* argv[], حرف *arg)
{
    صحيح i;
    لكل(i = 0; i < argc; ++i) {
        إذا(!argv[i]) استمر;
        إذا(0==strcmp(argv[i], arg)) {
            del_arg(argc, argv, i);
            أرجع 1;
        }
    }
    أرجع 0;
}

صحيح پیدا_کړه_int_arg(صحيح argc, حرف **argv, حرف *arg, صحيح def)
{
    صحيح i;
    لكل(i = 0; i < argc-1; ++i){
        إذا(!argv[i]) استمر;
        إذا(0==strcmp(argv[i], arg)){
            def = atoi(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            اكسر;
        }
    }
    أرجع def;
}

عائم پیدا_کړه_float_arg(صحيح argc, حرف **argv, حرف *arg, عائم def)
{
    صحيح i;
    لكل(i = 0; i < argc-1; ++i){
        إذا(!argv[i]) استمر;
        إذا(0==strcmp(argv[i], arg)){
            def = atof(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            اكسر;
        }
    }
    أرجع def;
}

حرف *پیدا_کړه_char_arg(صحيح argc, حرف **argv, حرف *arg, حرف *def)
{
    صحيح i;
    لكل(i = 0; i < argc-1; ++i){
        إذا(!argv[i]) استمر;
        إذا(0==strcmp(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            اكسر;
        }
    }
    أرجع def;
}


حرف *basecfg(حرف *cfgfile)
{
    حرف *c = cfgfile;
    حرف *بل;
    طالما((بل = strchr(c, '/')))
    {
        c = بل+1;
    }
    c = copy_string(c);
    بل = strchr(c, '.');
    إذا (بل) *بل = 0;
    أرجع c;
}

صحيح alphanum_منزل_int(حرف c)
{
    أرجع (c < 58) ? c - 48 : c-87;
}
حرف int_منزل_alphanum(صحيح i)
{
    إذا (i == 36) أرجع '.';
    أرجع (i < 10) ? i + 48 : i + 87;
}

فراغ pm(صحيح M, صحيح N, عائم *A)
{
    صحيح i,j;
    لكل(i =0 ; i < M; ++i){
        چاپ_کړه("%d ", i+1);
        لكل(j = 0; j < N; ++j){
            چاپ_کړه("%2.4f, ", A[i*N+j]);
        }
        چاپ_کړه("\n");
    }
    چاپ_کړه("\n");
}

فراغ پیدا_کړه_replace(حرف *str, حرف *orig, حرف *rep, حرف *output)
{
    حرف بفر[4096] = {0};
    حرف *p;

    sprintf(بفر, "%s", str);
    إذا(!(p = مزي_پلټنه(بفر, orig))){  // Is 'orig' even in 'str'?
        sprintf(output, "%s", str);
        أرجع;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", بفر, rep, p+مزي_اوږدوالی(orig));
}

عائم sec(clock_t clocks)
{
    أرجع (عائم)clocks/CLOCKS_PER_SEC;
}

فراغ سر_k(عائم *a, صحيح n, صحيح k, صحيح *شاخص)
{
    صحيح i,j;
    لكل(j = 0; j < k; ++j) شاخص[j] = -1;
    لكل(i = 0; i < n; ++i){
        صحيح curr = i;
        لكل(j = 0; j < k; ++j){
            إذا((شاخص[j] < 0) || a[curr] > a[شاخص[j]]){
                صحيح بدلون = curr;
                curr = شاخص[j];
                شاخص[j] = بدلون;
            }
        }
    }
}

فراغ error(ثابت حرف *s)
{
    perror(s);
    أكد(0);
    وتل(-1);
}

بینښه حرف *read_file(حرف *filename)
{
    FILE *fp = fopen(filename, "rb");
    size_t اندازه;

    fseek(fp, 0, SEEK_END); 
    اندازه = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    بینښه حرف *متن = حافظه_صفر_ورکړه(اندازه+1, حجم(حرف));
    fread(متن, 1, اندازه, fp);
    fclose(fp);
    أرجع متن;
}

فراغ malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    وتل(-1);
}

فراغ file_error(حرف *s)
{
    fprintf(stderr, "Couldn't open file: %s\n", s);
    وتل(0);
}

list *وېشل_str(حرف *s, حرف delim)
{
    size_t i;
    size_t len = مزي_اوږدوالی(s);
    list *l = make_list();
    list_ورننباسه(l, s);
    لكل(i = 0; i < len; ++i){
        إذا(s[i] == delim){
            s[i] = '\0';
            list_ورننباسه(l, &(s[i+1]));
        }
    }
    أرجع l;
}

فراغ strip(حرف *s)
{
    size_t i;
    size_t len = مزي_اوږدوالی(s);
    size_t دننه_واټن = 0;
    لكل(i = 0; i < len; ++i){
        حرف c = s[i];
        إذا(c==' '||c=='\t'||c=='\n') ++دننه_واټن;
        وإلا s[i-دننه_واټن] = c;
    }
    s[len-دننه_واټن] = '\0';
}

فراغ strip_char(حرف *s, حرف bad)
{
    size_t i;
    size_t len = مزي_اوږدوالی(s);
    size_t دننه_واټن = 0;
    لكل(i = 0; i < len; ++i){
        حرف c = s[i];
        إذا(c==bad) ++دننه_واټن;
        وإلا s[i-دننه_واټن] = c;
    }
    s[len-دننه_واټن] = '\0';
}

فراغ free_ptrs(فراغ **ptrs, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i) حرر(ptrs[i]);
    حرر(ptrs);
}

حرف *fgetl(FILE *fp)
{
    إذا(feof(fp)) أرجع 0;
    size_t اندازه = 512;
    حرف *کرښه = حافظه_ورکړه(اندازه*حجم(حرف));
    إذا(!fgets(کرښه, اندازه, fp)){
        حرر(کرښه);
        أرجع 0;
    }

    size_t curr = مزي_اوږدوالی(کرښه);

    طالما((کرښه[curr-1] != '\n') && !feof(fp)){
        إذا(curr == اندازه-1){
            اندازه *= 2;
            کرښه = realloc(کرښه, اندازه*حجم(حرف));
            إذا(!کرښه) {
                چاپ_کړه("%ld\n", اندازه);
                malloc_error();
            }
        }
        size_t readsize = اندازه-curr;
        إذا(readsize > لوی_عدد) readsize = لوی_عدد-1;
        fgets(&کرښه[curr], readsize, fp);
        curr = مزي_اوږدوالی(کرښه);
    }
    إذا(کرښه[curr-1] == '\n') کرښه[curr-1] = '\0';

    أرجع کرښه;
}

صحيح read_int(صحيح fd)
{
    صحيح n = 0;
    صحيح بل = اقرأ(fd, &n, حجم(صحيح));
    إذا(بل <= 0) أرجع -1;
    أرجع n;
}

فراغ write_int(صحيح fd, صحيح n)
{
    صحيح بل = اكتب(fd, &n, حجم(صحيح));
    إذا(بل <= 0) error("read failed");
}

صحيح read_all_fail(صحيح fd, حرف *بفر, size_t بایټونه)
{
    size_t n = 0;
    طالما(n < بایټونه){
        صحيح بل = اقرأ(fd, بفر + n, بایټونه-n);
        إذا(بل <= 0) أرجع 1;
        n += بل;
    }
    أرجع 0;
}

صحيح write_all_fail(صحيح fd, حرف *بفر, size_t بایټونه)
{
    size_t n = 0;
    طالما(n < بایټونه){
        size_t بل = اكتب(fd, بفر + n, بایټونه-n);
        إذا(بل <= 0) أرجع 1;
        n += بل;
    }
    أرجع 0;
}

فراغ read_all(صحيح fd, حرف *بفر, size_t بایټونه)
{
    size_t n = 0;
    طالما(n < بایټونه){
        صحيح بل = اقرأ(fd, بفر + n, بایټونه-n);
        إذا(بل <= 0) error("read failed");
        n += بل;
    }
}

فراغ write_all(صحيح fd, حرف *بفر, size_t بایټونه)
{
    size_t n = 0;
    طالما(n < بایټونه){
        size_t بل = اكتب(fd, بفر + n, بایټونه-n);
        إذا(بل <= 0) error("write failed");
        n += بل;
    }
}


حرف *copy_string(حرف *s)
{
    حرف *copy = حافظه_ورکړه(مزي_اوږدوالی(s)+1);
    strncpy(copy, s, مزي_اوږدوالی(s)+1);
    أرجع copy;
}

list *parse_csv_کرښه(حرف *کرښه)
{
    list *l = make_list();
    حرف *c, *p;
    صحيح دننه = 0;
    لكل(c = کرښه, p = کرښه; *c != '\0'; ++c){
        إذا(*c == '"') دننه = !دننه;
        وإلا إذا(*c == ',' && !دننه){
            *c = '\0';
            list_ورننباسه(l, copy_string(p));
            p = c+1;
        }
    }
    list_ورننباسه(l, copy_string(p));
    أرجع l;
}

صحيح شمېر_fields(حرف *کرښه)
{
    صحيح شمېر = 0;
    صحيح بشپړ = 0;
    حرف *c;
    لكل(c = کرښه; !بشپړ; ++c){
        بشپړ = (*c == '\0');
        إذا(*c == ',' || بشپړ) ++شمېر;
    }
    أرجع شمېر;
}

عائم *parse_fields(حرف *کرښه, صحيح n)
{
    عائم *ډګر = حافظه_صفر_ورکړه(n, حجم(عائم));
    حرف *c, *p, *پای_ټکی;
    صحيح شمېر = 0;
    صحيح بشپړ = 0;
    لكل(c = کرښه, p = کرښه; !بشپړ; ++c){
        بشپړ = (*c == '\0');
        إذا(*c == ',' || بشپړ){
            *c = '\0';
            ډګر[شمېر] = strtod(p, &پای_ټکی);
            إذا(p == c) ډګر[شمېر] = nan("");
            إذا(پای_ټکی != c && (پای_ټکی != c-1 || *پای_ټکی != '\r')) ډګر[شمېر] = nan(""); //DOS file formats!
            p = c+1;
            ++شمېر;
        }
    }
    أرجع ډګر;
}

عائم مجموعه_array(عائم *a, صحيح n)
{
    صحيح i;
    عائم مجموعه = 0;
    لكل(i = 0; i < n; ++i) مجموعه += a[i];
    أرجع مجموعه;
}

عائم منځنۍ_array(عائم *a, صحيح n)
{
    أرجع مجموعه_array(a,n)/n;
}

فراغ منځنۍ_arrays(عائم **a, صحيح n, صحيح els, عائم *avg)
{
    صحيح i;
    صحيح j;
    memset(avg, 0, els*حجم(عائم));
    لكل(j = 0; j < n; ++j){
        لكل(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    لكل(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

فراغ print_statistics(عائم *a, صحيح n)
{
    عائم m = منځنۍ_array(a, n);
    عائم v = variance_array(a, n);
    چاپ_کړه("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

عائم variance_array(عائم *a, صحيح n)
{
    صحيح i;
    عائم مجموعه = 0;
    عائم منځنۍ = منځنۍ_array(a, n);
    لكل(i = 0; i < n; ++i) مجموعه += (a[i] - منځنۍ)*(a[i]-منځنۍ);
    عائم variance = مجموعه/n;
    أرجع variance;
}

صحيح constrain_int(صحيح a, صحيح min, صحيح max)
{
    إذا (a < min) أرجع min;
    إذا (a > max) أرجع max;
    أرجع a;
}

عائم constrain(عائم min, عائم max, عائم a)
{
    إذا (a < min) أرجع min;
    إذا (a > max) أرجع max;
    أرجع a;
}

عائم dist_array(عائم *a, عائم *b, صحيح n, صحيح sub)
{
    صحيح i;
    عائم مجموعه = 0;
    لكل(i = 0; i < n; i += sub) مجموعه += توان(a[i]-b[i], 2);
    أرجع جذر(مجموعه);
}

عائم mse_array(عائم *a, صحيح n)
{
    صحيح i;
    عائم مجموعه = 0;
    لكل(i = 0; i < n; ++i) مجموعه += a[i]*a[i];
    أرجع جذر(مجموعه/n);
}

فراغ normalize_array(عائم *a, صحيح n)
{
    صحيح i;
    عائم mu = منځنۍ_array(a,n);
    عائم sigma = جذر(variance_array(a,n));
    لكل(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = منځنۍ_array(a,n);
    sigma = جذر(variance_array(a,n));
}

فراغ translate_array(عائم *a, صحيح n, عائم s)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        a[i] += s;
    }
}

عائم mag_array(عائم *a, صحيح n)
{
    صحيح i;
    عائم مجموعه = 0;
    لكل(i = 0; i < n; ++i){
        مجموعه += a[i]*a[i];   
    }
    أرجع جذر(مجموعه);
}

فراغ scale_array(عائم *a, صحيح n, عائم s)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

صحيح sample_array(عائم *a, صحيح n)
{
    عائم مجموعه = مجموعه_array(a, n);
    scale_array(a, n, 1./مجموعه);
    عائم r = rand_یوشان(0, 1);
    صحيح i;
    لكل(i = 0; i < n; ++i){
        r = r - a[i];
        إذا (r <= 0) أرجع i;
    }
    أرجع n-1;
}

صحيح max_int_شاخص(صحيح *a, صحيح n)
{
    إذا(n <= 0) أرجع -1;
    صحيح i, max_i = 0;
    صحيح max = a[0];
    لكل(i = 1; i < n; ++i){
        إذا(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    أرجع max_i;
}

صحيح max_شاخص(عائم *a, صحيح n)
{
    إذا(n <= 0) أرجع -1;
    صحيح i, max_i = 0;
    عائم max = a[0];
    لكل(i = 1; i < n; ++i){
        إذا(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    أرجع max_i;
}

صحيح int_شاخص(صحيح *a, صحيح val, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        إذا(a[i] == val) أرجع i;
    }
    أرجع -1;
}

صحيح rand_int(صحيح min, صحيح max)
{
    إذا (max < min){
        صحيح s = min;
        min = max;
        max = s;
    }
    صحيح r = (rand()%(max - min + 1)) + min;
    أرجع r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
عائم rand_نورمال()
{
    ساكن صحيح have_اضافي = 0;
    ساكن مزدوج rand1, rand2;

    إذا(have_اضافي)
    {
        have_اضافي = 0;
        أرجع جذر(rand1) * ساین(rand2);
    }

    have_اضافي = 1;

    rand1 = rand() / ((مزدوج) RAND_MAX);
    إذا(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * لوګاریتم(rand1);
    rand2 = (rand() / ((مزدوج) RAND_MAX)) * TWO_PI;

    أرجع جذر(rand1) * کوساین(rand2);
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

size_t rand_اندازه_t()
{
    أرجع  ((size_t)(rand()&0xff) << 56) | 
        ((size_t)(rand()&0xff) << 48) |
        ((size_t)(rand()&0xff) << 40) |
        ((size_t)(rand()&0xff) << 32) |
        ((size_t)(rand()&0xff) << 24) |
        ((size_t)(rand()&0xff) << 16) |
        ((size_t)(rand()&0xff) << 8) |
        ((size_t)(rand()&0xff) << 0);
}

عائم rand_یوشان(عائم min, عائم max)
{
    إذا(max < min){
        عائم بدلون = min;
        min = max;
        max = بدلون;
    }
    أرجع ((عائم)rand()/RAND_MAX * (max - min)) + min;
}

عائم rand_scale(عائم s)
{
    عائم scale = rand_یوشان(1, s);
    إذا(rand()%2) أرجع scale;
    أرجع 1./scale;
}

عائم **one_hot_encode(عائم *a, صحيح n, صحيح k)
{
    صحيح i;
    عائم **t = حافظه_صفر_ورکړه(n, حجم(عائم*));
    لكل(i = 0; i < n; ++i){
        t[i] = حافظه_صفر_ورکړه(k, حجم(عائم));
        صحيح شاخص = (صحيح)a[i];
        t[i][شاخص] = 1;
    }
    أرجع t;
}

