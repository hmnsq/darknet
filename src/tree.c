#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, var_na *leaf_list)
{
    list *llist = get_paths(leaf_list);
    var_na **leaves = (var_na **)list_to_array(llist);
    ကိန်းပြည့် n = llist->size;
    ကိန်းပြည့် i,j;
    ကိန်းပြည့် found = 0;
    အတွက်(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        အတွက်(j = 0; j < n; ++j){
            အကယ်၍ (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                bha_nga;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, ကိန်းပြည့် c, ကိန်းပြည့် stride)
{
    float p = 1;
    နေစဉ်(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    ပြန်ပေး p;
}

void hierarchy_predictions(float *predictions, ကိန်းပြည့် n, tree *hier, ကိန်းပြည့် only_leaves, ကိန်းပြည့် stride)
{
    ကိန်းပြည့် j;
    အတွက်(j = 0; j < n; ++j){
        ကိန်းပြည့် parent = hier->parent[j];
        အကယ်၍(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    အကယ်၍(only_leaves){
        အတွက်(j = 0; j < n; ++j){
            အကယ်၍(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

ကိန်းပြည့် hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ကိန်းပြည့် stride)
{
    float p = 1;
    ကိန်းပြည့် group = 0;
    ကိန်းပြည့် i;
    နေစဉ်(1){
        float max = 0;
        ကိန်းပြည့် max_i = 0;

        အတွက်(i = 0; i < hier->group_size[group]; ++i){
            ကိန်းပြည့် index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            အကယ်၍(val > max){
                max_i = index;
                max = val;
            }
        }
        အကယ်၍(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            အကယ်၍(hier->child[max_i] < 0) ပြန်ပေး max_i;
        } မဟုတ်ပါက အကယ်၍ (group == 0){
            ပြန်ပေး max_i;
        } မဟုတ်ပါက {
            ပြန်ပေး hier->parent[hier->group_offset[group]];
        }
    }
    ပြန်ပေး 0;
}

tree *read_tree(var_na *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    var_na *line;
    ကိန်းပြည့် last_parent = -1;
    ကိန်းပြည့် group_size = 0;
    ကိန်းပြည့် groups = 0;
    ကိန်းပြည့် n = 0;
    နေစဉ်((line=fgetl(fp)) != 0){
        var_na *id = calloc(256, parimaa_na(var_na));
        ကိန်းပြည့် parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*parimaa_na(ကိန်းပြည့်));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*parimaa_na(ကိန်းပြည့်));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*parimaa_na(var_na *));
        t.name[n] = id;
        အကယ်၍(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * parimaa_na(ကိန်းပြည့်));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * parimaa_na(ကိန်းပြည့်));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*parimaa_na(ကိန်းပြည့်));
        t.group[n] = groups;
        အကယ်၍ (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * parimaa_na(ကိန်းပြည့်));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * parimaa_na(ကိန်းပြည့်));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, parimaa_na(ကိန်းပြည့်));
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i) t.leaf[i] = 1;
    အတွက်(i = 0; i < n; ++i) အကယ်၍(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, parimaa_na(tree));
    *tree_ptr = t;
    //error(0);
    ပြန်ပေး tree_ptr;
}
