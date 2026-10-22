#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option_list.h"
#include "utils.h"

list *read_data_cfg(char *filename)
{
    FILE *file = fopen(filename, "r");
    if(file == 0) file_error(filename);
    char *लिन्या;
    int nu = 0;
    list *options = make_list();
    while((लिन्या=fgetl(file)) != 0){
        ++ nu;
        strip(लिन्या);
        switch(लिन्या[0]){
            case '\0':
            case '#':
            case ';':
                free(लिन्या);
                break;
            default:
                if(!read_option(लिन्या, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, लिन्या);
                    free(लिन्या);
                }
                break;
        }
    }
    fclose(file);
    return options;
}

metadata get_metadata(char *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    char *name_list = option_find_str(options, "names", 0);
    if(!name_list) name_list = option_find_str(options, "labels", 0);
    if(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } else {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    return m;
}

int read_option(char *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    char *val = 0;
    for(i = 0; i < len; ++i){
        if(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            break;
        }
    }
    if(i == len-1) return 0;
    char *सुसि = s;
    option_इसिंगित(options, सुसि, val);
    return 1;
}

void option_इसिंगित(list *l, char *सुसि, char *val)
{
    kvp *p = malloc(sizeof(kvp));
    p->सुसि = सुसि;
    p->val = val;
    p->used = 0;
    list_इसिंगित(l, p);
}

void option_unused(list *l)
{
    बुहोल *n = l->front;
    while(n){
        kvp *p = (kvp *)n->val;
        if(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->सुसि, p->val);
        }
        n = n->next;
    }
}

char *option_find(list *l, char *सुसि)
{
    बुहोल *n = l->front;
    while(n){
        kvp *p = (kvp *)n->val;
        if(strcmp(p->सुसि, सुसि) == 0){
            p->used = 1;
            return p->val;
        }
        n = n->next;
    }
    return 0;
}
char *option_find_str(list *l, char *सुसि, char *def)
{
    char *v = option_find(l, सुसि);
    if(v) return v;
    if(def) fprintf(stderr, "%s: Using default '%s'\n", सुसि, def);
    return def;
}

int option_find_int(list *l, char *सुसि, int def)
{
    char *v = option_find(l, सुसि);
    if(v) return atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", सुसि, def);
    return def;
}

int option_find_int_quiet(list *l, char *सुसि, int def)
{
    char *v = option_find(l, सुसि);
    if(v) return atoi(v);
    return def;
}

float option_find_float_quiet(list *l, char *सुसि, float def)
{
    char *v = option_find(l, सुसि);
    if(v) return atof(v);
    return def;
}

float option_find_float(list *l, char *सुसि, float def)
{
    char *v = option_find(l, सुसि);
    if(v) return atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", सुसि, def);
    return def;
}
