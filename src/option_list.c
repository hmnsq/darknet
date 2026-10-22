#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option_list.h"
#include "utils.h"

list *read_data_cfg(char *filename)
{
    FILE *file = fopen(filename, "r");
    만약(file == 0) file_error(filename);
    char *line;
    정수 nu = 0;
    list *options = make_list();
    동안((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        switch(line[0]){
            case '\0':
            case '#':
            case ';':
                free(line);
                break;
            default:
                만약(!read_option(line, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }
    fclose(file);
    반환 options;
}

metadata get_metadata(char *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    char *name_list = option_find_str(options, "names", 0);
    만약(!name_list) name_list = option_find_str(options, "labels", 0);
    만약(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } 아니면 {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    반환 m;
}

정수 read_option(char *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    char *val = 0;
    위해(i = 0; i < len; ++i){
        만약(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            break;
        }
    }
    만약(i == len-1) 반환 0;
    char *key = s;
    option_insert(options, key, val);
    반환 1;
}

void option_insert(list *l, char *key, char *val)
{
    kvp *p = malloc(sizeof(kvp));
    p->key = key;
    p->val = val;
    p->used = 0;
    list_insert(l, p);
}

void option_unused(list *l)
{
    node *n = l->front;
    동안(n){
        kvp *p = (kvp *)n->val;
        만약(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->key, p->val);
        }
        n = n->next;
    }
}

char *option_find(list *l, char *key)
{
    node *n = l->front;
    동안(n){
        kvp *p = (kvp *)n->val;
        만약(strcmp(p->key, key) == 0){
            p->used = 1;
            반환 p->val;
        }
        n = n->next;
    }
    반환 0;
}
char *option_find_str(list *l, char *key, char *def)
{
    char *v = option_find(l, key);
    만약(v) 반환 v;
    만약(def) fprintf(stderr, "%s: Using default '%s'\n", key, def);
    반환 def;
}

정수 option_find_int(list *l, char *key, 정수 def)
{
    char *v = option_find(l, key);
    만약(v) 반환 atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", key, def);
    반환 def;
}

정수 option_find_int_quiet(list *l, char *key, 정수 def)
{
    char *v = option_find(l, key);
    만약(v) 반환 atoi(v);
    반환 def;
}

float option_find_float_quiet(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    만약(v) 반환 atof(v);
    반환 def;
}

float option_find_float(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    만약(v) 반환 atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", key, def);
    반환 def;
}
