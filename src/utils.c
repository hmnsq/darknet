#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश <math.h>
#समावेश <assert.h>
#समावेश <unistd.h>
#समावेश <float.h>
#समावेश <limits.h>
#समावेश <time.h>
#समावेश <sys/time.h>

#समावेश "utils.h"


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

द्विगुन what_time_is_it_now()
{
    काष्ठा timeval समय;
    अगर (gettimeofday(&समय,शून्य)){
        वापस 0;
    }
    वापस (द्विगुन)समय.tv_sec + (द्विगुन)समय.tv_usec * .000001;
}

पूर्णांक *read_intlist(अक्षर *gpu_list, पूर्णांक *ngpus, पूर्णांक d)
{
    पूर्णांक *gpus = 0;
    अगर(gpu_list){
        पूर्णांक len = म_माप(gpu_list);
        *ngpus = 1;
        पूर्णांक i;
        क्रम(i = 0; i < len; ++i){
            अगर (gpu_list[i] == ',') ++*ngpus;
        }
        gpus = सुस्मृति(*ngpus, माप(पूर्णांक));
        क्रम(i = 0; i < *ngpus; ++i){
            gpus[i] = म_से_प(gpu_list);
            gpu_list = म_अक्षर(gpu_list, ',')+1;
        }
    } अन्यथा {
        gpus = सुस्मृति(1, माप(भग्न));
        *gpus = d;
        *ngpus = 1;
    }
    वापस gpus;
}

पूर्णांक *read_map(अक्षर *filename)
{
    पूर्णांक n = 0;
    पूर्णांक *map = 0;
    अक्षर *str;
    खाता *file = ख_खोलो(filename, "r");
    अगर(!file) file_error(filename);
    जबतक((str=fgetl(file))){
        ++n;
        map = पुनः_स्मृति(map, n*माप(पूर्णांक));
        map[n-1] = म_से_प(str);
    }
    वापस map;
}

व्योम sorta_shuffle(व्योम *arr, माप_प्रकार n, माप_प्रकार आकार, माप_प्रकार sections)
{
    माप_प्रकार i;
    क्रम(i = 0; i < sections; ++i){
        माप_प्रकार start = n*i/sections;
        माप_प्रकार end = n*(i+1)/sections;
        माप_प्रकार अंक = end-start;
        shuffle(arr+(start*आकार), अंक, आकार);
    }
}

व्योम shuffle(व्योम *arr, माप_प्रकार n, माप_प्रकार आकार)
{
    माप_प्रकार i;
    व्योम *swp = सुस्मृति(1, आकार);
    क्रम(i = 0; i < n-1; ++i){
        माप_प्रकार j = i + अक्रम()/(अक्रम_उच्च / (n-i)+1);
        स_नकल(swp,          arr+(j*आकार), आकार);
        स_नकल(arr+(j*आकार), arr+(i*आकार), आकार);
        स_नकल(arr+(i*आकार), swp,          आकार);
    }
}

पूर्णांक *random_सूचकांक_order(पूर्णांक min, पूर्णांक max)
{
    पूर्णांक *inds = सुस्मृति(max-min, माप(पूर्णांक));
    पूर्णांक i;
    क्रम(i = min; i < max; ++i){
        inds[i] = i;
    }
    क्रम(i = min; i < max-1; ++i){
        पूर्णांक swap = inds[i];
        पूर्णांक सूचकांक = i + अक्रम()%(max-i);
        inds[i] = inds[सूचकांक];
        inds[सूचकांक] = swap;
    }
    वापस inds;
}

व्योम del_arg(पूर्णांक argc, अक्षर **argv, पूर्णांक सूचकांक)
{
    पूर्णांक i;
    क्रम(i = सूचकांक; i < argc-1; ++i) argv[i] = argv[i+1];
    argv[i] = 0;
}

पूर्णांक find_arg(पूर्णांक argc, अक्षर* argv[], अक्षर *arg)
{
    पूर्णांक i;
    क्रम(i = 0; i < argc; ++i) {
        अगर(!argv[i]) जारी;
        अगर(0==म_भेद(argv[i], arg)) {
            del_arg(argc, argv, i);
            वापस 1;
        }
    }
    वापस 0;
}

पूर्णांक find_int_arg(पूर्णांक argc, अक्षर **argv, अक्षर *arg, पूर्णांक def)
{
    पूर्णांक i;
    क्रम(i = 0; i < argc-1; ++i){
        अगर(!argv[i]) जारी;
        अगर(0==म_भेद(argv[i], arg)){
            def = म_से_प(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            अवरोध;
        }
    }
    वापस def;
}

भग्न find_float_arg(पूर्णांक argc, अक्षर **argv, अक्षर *arg, भग्न def)
{
    पूर्णांक i;
    क्रम(i = 0; i < argc-1; ++i){
        अगर(!argv[i]) जारी;
        अगर(0==म_भेद(argv[i], arg)){
            def = म_से_भ(argv[i+1]);
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            अवरोध;
        }
    }
    वापस def;
}

अक्षर *find_char_arg(पूर्णांक argc, अक्षर **argv, अक्षर *arg, अक्षर *def)
{
    पूर्णांक i;
    क्रम(i = 0; i < argc-1; ++i){
        अगर(!argv[i]) जारी;
        अगर(0==म_भेद(argv[i], arg)){
            def = argv[i+1];
            del_arg(argc, argv, i);
            del_arg(argc, argv, i);
            अवरोध;
        }
    }
    वापस def;
}


अक्षर *basecfg(अक्षर *cfgfile)
{
    अक्षर *c = cfgfile;
    अक्षर *अग्र;
    जबतक((अग्र = म_अक्षर(c, '/')))
    {
        c = अग्र+1;
    }
    c = copy_string(c);
    अग्र = म_अक्षर(c, '.');
    अगर (अग्र) *अग्र = 0;
    वापस c;
}

पूर्णांक alphanum_to_int(अक्षर c)
{
    वापस (c < 58) ? c - 48 : c-87;
}
अक्षर int_to_alphanum(पूर्णांक i)
{
    अगर (i == 36) वापस '.';
    वापस (i < 10) ? i + 48 : i + 87;
}

व्योम pm(पूर्णांक M, पूर्णांक N, भग्न *A)
{
    पूर्णांक i,j;
    क्रम(i =0 ; i < M; ++i){
        म_लिखो("%d ", i+1);
        क्रम(j = 0; j < N; ++j){
            म_लिखो("%2.4f, ", A[i*N+j]);
        }
        म_लिखो("\n");
    }
    म_लिखो("\n");
}

व्योम find_replace(अक्षर *str, अक्षर *orig, अक्षर *rep, अक्षर *output)
{
    अक्षर बफर[4096] = {0};
    अक्षर *p;

    प्र_लिखो(बफर, "%s", str);
    अगर(!(p = म_माला(बफर, orig))){  // Is 'orig' even in 'str'?
        प्र_लिखो(output, "%s", str);
        वापस;
    }

    *p = '\0';

    प्र_लिखो(output, "%s%s%s", बफर, rep, p+म_माप(orig));
}

भग्न sec(घड़ी_प्रकार clocks)
{
    वापस (भग्न)clocks/CLOCKS_PER_SEC;
}

व्योम शीर्ष_k(भग्न *a, पूर्णांक n, पूर्णांक k, पूर्णांक *सूचकांक)
{
    पूर्णांक i,j;
    क्रम(j = 0; j < k; ++j) सूचकांक[j] = -1;
    क्रम(i = 0; i < n; ++i){
        पूर्णांक curr = i;
        क्रम(j = 0; j < k; ++j){
            अगर((सूचकांक[j] < 0) || a[curr] > a[सूचकांक[j]]){
                पूर्णांक swap = curr;
                curr = सूचकांक[j];
                सूचकांक[j] = swap;
            }
        }
    }
}

व्योम error(स्थिर अक्षर *s)
{
    लिखो_त्रुटि(s);
    निश्चित(0);
    निकास(-1);
}

अचिन्हित अक्षर *read_file(अक्षर *filename)
{
    खाता *fp = ख_खोलो(filename, "rb");
    माप_प्रकार आकार;

    ख_जाओ(fp, 0, अंत_से); 
    आकार = ख_बताओ(fp);
    ख_जाओ(fp, 0, शुरू_से); 

    अचिन्हित अक्षर *text = सुस्मृति(आकार+1, माप(अक्षर));
    ख_पढ़ो(text, 1, आकार, fp);
    ख_बंद(fp);
    वापस text;
}

व्योम malloc_error()
{
    ख_लिखो(मानक_त्रुटि, "Malloc error\n");
    निकास(-1);
}

व्योम file_error(अक्षर *s)
{
    ख_लिखो(मानक_त्रुटि, "Couldn't open file: %s\n", s);
    निकास(0);
}

list *split_str(अक्षर *s, अक्षर delim)
{
    माप_प्रकार i;
    माप_प्रकार len = म_माप(s);
    list *l = make_list();
    list_जोड़ो(l, s);
    क्रम(i = 0; i < len; ++i){
        अगर(s[i] == delim){
            s[i] = '\0';
            list_जोड़ो(l, &(s[i+1]));
        }
    }
    वापस l;
}

व्योम strip(अक्षर *s)
{
    माप_प्रकार i;
    माप_प्रकार len = म_माप(s);
    माप_प्रकार offset = 0;
    क्रम(i = 0; i < len; ++i){
        अक्षर c = s[i];
        अगर(c==' '||c=='\t'||c=='\n') ++offset;
        अन्यथा s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

व्योम strip_char(अक्षर *s, अक्षर bad)
{
    माप_प्रकार i;
    माप_प्रकार len = म_माप(s);
    माप_प्रकार offset = 0;
    क्रम(i = 0; i < len; ++i){
        अक्षर c = s[i];
        अगर(c==bad) ++offset;
        अन्यथा s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

व्योम free_ptrs(व्योम **ptrs, पूर्णांक n)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i) मुक्त(ptrs[i]);
    मुक्त(ptrs);
}

अक्षर *fgetl(खाता *fp)
{
    अगर(ख_पूर्ण(fp)) वापस 0;
    माप_प्रकार आकार = 512;
    अक्षर *पंक्ति = दो_स्मृति(आकार*माप(अक्षर));
    अगर(!ख_माला_लो(पंक्ति, आकार, fp)){
        मुक्त(पंक्ति);
        वापस 0;
    }

    माप_प्रकार curr = म_माप(पंक्ति);

    जबतक((पंक्ति[curr-1] != '\n') && !ख_पूर्ण(fp)){
        अगर(curr == आकार-1){
            आकार *= 2;
            पंक्ति = पुनः_स्मृति(पंक्ति, आकार*माप(अक्षर));
            अगर(!पंक्ति) {
                म_लिखो("%ld\n", आकार);
                malloc_error();
            }
        }
        माप_प्रकार readsize = आकार-curr;
        अगर(readsize > पूर्णांक_उच्च) readsize = पूर्णांक_उच्च-1;
        ख_माला_लो(&पंक्ति[curr], readsize, fp);
        curr = म_माप(पंक्ति);
    }
    अगर(पंक्ति[curr-1] == '\n') पंक्ति[curr-1] = '\0';

    वापस पंक्ति;
}

पूर्णांक read_int(पूर्णांक fd)
{
    पूर्णांक n = 0;
    पूर्णांक अग्र = पढ़ो(fd, &n, माप(पूर्णांक));
    अगर(अग्र <= 0) वापस -1;
    वापस n;
}

व्योम write_int(पूर्णांक fd, पूर्णांक n)
{
    पूर्णांक अग्र = ग_लिखो(fd, &n, माप(पूर्णांक));
    अगर(अग्र <= 0) error("read failed");
}

पूर्णांक read_all_fail(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes)
{
    माप_प्रकार n = 0;
    जबतक(n < bytes){
        पूर्णांक अग्र = पढ़ो(fd, बफर + n, bytes-n);
        अगर(अग्र <= 0) वापस 1;
        n += अग्र;
    }
    वापस 0;
}

पूर्णांक write_all_fail(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes)
{
    माप_प्रकार n = 0;
    जबतक(n < bytes){
        माप_प्रकार अग्र = ग_लिखो(fd, बफर + n, bytes-n);
        अगर(अग्र <= 0) वापस 1;
        n += अग्र;
    }
    वापस 0;
}

व्योम read_all(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes)
{
    माप_प्रकार n = 0;
    जबतक(n < bytes){
        पूर्णांक अग्र = पढ़ो(fd, बफर + n, bytes-n);
        अगर(अग्र <= 0) error("read failed");
        n += अग्र;
    }
}

व्योम write_all(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes)
{
    माप_प्रकार n = 0;
    जबतक(n < bytes){
        माप_प्रकार अग्र = ग_लिखो(fd, बफर + n, bytes-n);
        अगर(अग्र <= 0) error("write failed");
        n += अग्र;
    }
}


अक्षर *copy_string(अक्षर *s)
{
    अक्षर *copy = दो_स्मृति(म_माप(s)+1);
    म_नकलन(copy, s, म_माप(s)+1);
    वापस copy;
}

list *parse_csv_पंक्ति(अक्षर *पंक्ति)
{
    list *l = make_list();
    अक्षर *c, *p;
    पूर्णांक in = 0;
    क्रम(c = पंक्ति, p = पंक्ति; *c != '\0'; ++c){
        अगर(*c == '"') in = !in;
        अन्यथा अगर(*c == ',' && !in){
            *c = '\0';
            list_जोड़ो(l, copy_string(p));
            p = c+1;
        }
    }
    list_जोड़ो(l, copy_string(p));
    वापस l;
}

पूर्णांक गिनती_fields(अक्षर *पंक्ति)
{
    पूर्णांक गिनती = 0;
    पूर्णांक done = 0;
    अक्षर *c;
    क्रम(c = पंक्ति; !done; ++c){
        done = (*c == '\0');
        अगर(*c == ',' || done) ++गिनती;
    }
    वापस गिनती;
}

भग्न *parse_fields(अक्षर *पंक्ति, पूर्णांक n)
{
    भग्न *field = सुस्मृति(n, माप(भग्न));
    अक्षर *c, *p, *end;
    पूर्णांक गिनती = 0;
    पूर्णांक done = 0;
    क्रम(c = पंक्ति, p = पंक्ति; !done; ++c){
        done = (*c == '\0');
        अगर(*c == ',' || done){
            *c = '\0';
            field[गिनती] = म_से_भग्न(p, &end);
            अगर(p == c) field[गिनती] = nan("");
            अगर(end != c && (end != c-1 || *end != '\r')) field[गिनती] = nan(""); //DOS file formats!
            p = c+1;
            ++गिनती;
        }
    }
    वापस field;
}

भग्न योग_array(भग्न *a, पूर्णांक n)
{
    पूर्णांक i;
    भग्न योग = 0;
    क्रम(i = 0; i < n; ++i) योग += a[i];
    वापस योग;
}

भग्न mean_array(भग्न *a, पूर्णांक n)
{
    वापस योग_array(a,n)/n;
}

व्योम mean_arrays(भग्न **a, पूर्णांक n, पूर्णांक els, भग्न *avg)
{
    पूर्णांक i;
    पूर्णांक j;
    स_रखो(avg, 0, els*माप(भग्न));
    क्रम(j = 0; j < n; ++j){
        क्रम(i = 0; i < els; ++i){
            avg[i] += a[j][i];
        }
    }
    क्रम(i = 0; i < els; ++i){
        avg[i] /= n;
    }
}

व्योम print_statistics(भग्न *a, पूर्णांक n)
{
    भग्न m = mean_array(a, n);
    भग्न v = variance_array(a, n);
    म_लिखो("MSE: %.6f, Mean: %.6f, Variance: %.6f\n", mse_array(a, n), m, v);
}

भग्न variance_array(भग्न *a, पूर्णांक n)
{
    पूर्णांक i;
    भग्न योग = 0;
    भग्न mean = mean_array(a, n);
    क्रम(i = 0; i < n; ++i) योग += (a[i] - mean)*(a[i]-mean);
    भग्न variance = योग/n;
    वापस variance;
}

पूर्णांक constrain_int(पूर्णांक a, पूर्णांक min, पूर्णांक max)
{
    अगर (a < min) वापस min;
    अगर (a > max) वापस max;
    वापस a;
}

भग्न constrain(भग्न min, भग्न max, भग्न a)
{
    अगर (a < min) वापस min;
    अगर (a > max) वापस max;
    वापस a;
}

भग्न dist_array(भग्न *a, भग्न *b, पूर्णांक n, पूर्णांक sub)
{
    पूर्णांक i;
    भग्न योग = 0;
    क्रम(i = 0; i < n; i += sub) योग += घात(a[i]-b[i], 2);
    वापस वर्ग_मूल(योग);
}

भग्न mse_array(भग्न *a, पूर्णांक n)
{
    पूर्णांक i;
    भग्न योग = 0;
    क्रम(i = 0; i < n; ++i) योग += a[i]*a[i];
    वापस वर्ग_मूल(योग/n);
}

व्योम normalize_array(भग्न *a, पूर्णांक n)
{
    पूर्णांक i;
    भग्न mu = mean_array(a,n);
    भग्न sigma = वर्ग_मूल(variance_array(a,n));
    क्रम(i = 0; i < n; ++i){
        a[i] = (a[i] - mu)/sigma;
    }
    mu = mean_array(a,n);
    sigma = वर्ग_मूल(variance_array(a,n));
}

व्योम translate_array(भग्न *a, पूर्णांक n, भग्न s)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        a[i] += s;
    }
}

भग्न mag_array(भग्न *a, पूर्णांक n)
{
    पूर्णांक i;
    भग्न योग = 0;
    क्रम(i = 0; i < n; ++i){
        योग += a[i]*a[i];   
    }
    वापस वर्ग_मूल(योग);
}

व्योम scale_array(भग्न *a, पूर्णांक n, भग्न s)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        a[i] *= s;
    }
}

पूर्णांक sample_array(भग्न *a, पूर्णांक n)
{
    भग्न योग = योग_array(a, n);
    scale_array(a, n, 1./योग);
    भग्न r = rand_uniform(0, 1);
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        r = r - a[i];
        अगर (r <= 0) वापस i;
    }
    वापस n-1;
}

पूर्णांक max_int_सूचकांक(पूर्णांक *a, पूर्णांक n)
{
    अगर(n <= 0) वापस -1;
    पूर्णांक i, max_i = 0;
    पूर्णांक max = a[0];
    क्रम(i = 1; i < n; ++i){
        अगर(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    वापस max_i;
}

पूर्णांक max_सूचकांक(भग्न *a, पूर्णांक n)
{
    अगर(n <= 0) वापस -1;
    पूर्णांक i, max_i = 0;
    भग्न max = a[0];
    क्रम(i = 1; i < n; ++i){
        अगर(a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    वापस max_i;
}

पूर्णांक int_सूचकांक(पूर्णांक *a, पूर्णांक val, पूर्णांक n)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        अगर(a[i] == val) वापस i;
    }
    वापस -1;
}

पूर्णांक rand_int(पूर्णांक min, पूर्णांक max)
{
    अगर (max < min){
        पूर्णांक s = min;
        min = max;
        max = s;
    }
    पूर्णांक r = (अक्रम()%(max - min + 1)) + min;
    वापस r;
}

// From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
भग्न rand_normal()
{
    अटल पूर्णांक haveSpare = 0;
    अटल द्विगुन rand1, rand2;

    अगर(haveSpare)
    {
        haveSpare = 0;
        वापस वर्ग_मूल(rand1) * sin(rand2);
    }

    haveSpare = 1;

    rand1 = अक्रम() / ((द्विगुन) अक्रम_उच्च);
    अगर(rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (अक्रम() / ((द्विगुन) अक्रम_उच्च)) * TWO_PI;

    वापस वर्ग_मूल(rand1) * cos(rand2);
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

माप_प्रकार rand_आकार_t()
{
    वापस  ((माप_प्रकार)(अक्रम()&0xff) << 56) | 
        ((माप_प्रकार)(अक्रम()&0xff) << 48) |
        ((माप_प्रकार)(अक्रम()&0xff) << 40) |
        ((माप_प्रकार)(अक्रम()&0xff) << 32) |
        ((माप_प्रकार)(अक्रम()&0xff) << 24) |
        ((माप_प्रकार)(अक्रम()&0xff) << 16) |
        ((माप_प्रकार)(अक्रम()&0xff) << 8) |
        ((माप_प्रकार)(अक्रम()&0xff) << 0);
}

भग्न rand_uniform(भग्न min, भग्न max)
{
    अगर(max < min){
        भग्न swap = min;
        min = max;
        max = swap;
    }
    वापस ((भग्न)अक्रम()/अक्रम_उच्च * (max - min)) + min;
}

भग्न rand_scale(भग्न s)
{
    भग्न scale = rand_uniform(1, s);
    अगर(अक्रम()%2) वापस scale;
    वापस 1./scale;
}

भग्न **one_hot_encode(भग्न *a, पूर्णांक n, पूर्णांक k)
{
    पूर्णांक i;
    भग्न **t = सुस्मृति(n, माप(भग्न*));
    क्रम(i = 0; i < n; ++i){
        t[i] = सुस्मृति(k, माप(भग्न));
        पूर्णांक सूचकांक = (पूर्णांक)a[i];
        t[i][सूचकांक] = 1;
    }
    वापस t;
}

