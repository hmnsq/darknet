#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, var_na *leaf_list)
{
    list *llist = get_paths(leaf_list);
    var_na **leaves = (var_na **)list_to_array(llist);
    ចំនួនគត់ n = llist->size;
    ចំនួនគត់ i,j;
    ចំនួនគត់ found = 0;
    សម្រាប់(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        សម្រាប់(j = 0; j < n; ++j){
            ប្រសិនបើ (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                bha_nga;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, ចំនួនគត់ c, ចំនួនគត់ stride)
{
    float p = 1;
    ខណៈពេល(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    ត្រឡប់ p;
}

void hierarchy_predictions(float *predictions, ចំនួនគត់ n, tree *hier, ចំនួនគត់ only_leaves, ចំនួនគត់ stride)
{
    ចំនួនគត់ j;
    សម្រាប់(j = 0; j < n; ++j){
        ចំនួនគត់ parent = hier->parent[j];
        ប្រសិនបើ(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    ប្រសិនបើ(only_leaves){
        សម្រាប់(j = 0; j < n; ++j){
            ប្រសិនបើ(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

ចំនួនគត់ hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ចំនួនគត់ stride)
{
    float p = 1;
    ចំនួនគត់ group = 0;
    ចំនួនគត់ i;
    ខណៈពេល(1){
        float max = 0;
        ចំនួនគត់ max_i = 0;

        សម្រាប់(i = 0; i < hier->group_size[group]; ++i){
            ចំនួនគត់ index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            ប្រសិនបើ(val > max){
                max_i = index;
                max = val;
            }
        }
        ប្រសិនបើ(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            ប្រសិនបើ(hier->child[max_i] < 0) ត្រឡប់ max_i;
        } បើមិនដូច្នេះ ប្រសិនបើ (group == 0){
            ត្រឡប់ max_i;
        } បើមិនដូច្នេះ {
            ត្រឡប់ hier->parent[hier->group_offset[group]];
        }
    }
    ត្រឡប់ 0;
}

tree *read_tree(var_na *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    var_na *line;
    ចំនួនគត់ last_parent = -1;
    ចំនួនគត់ group_size = 0;
    ចំនួនគត់ groups = 0;
    ចំនួនគត់ n = 0;
    ខណៈពេល((line=fgetl(fp)) != 0){
        var_na *id = calloc(256, parimaa_na(var_na));
        ចំនួនគត់ parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*parimaa_na(ចំនួនគត់));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*parimaa_na(ចំនួនគត់));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*parimaa_na(var_na *));
        t.name[n] = id;
        ប្រសិនបើ(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * parimaa_na(ចំនួនគត់));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * parimaa_na(ចំនួនគត់));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*parimaa_na(ចំនួនគត់));
        t.group[n] = groups;
        ប្រសិនបើ (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * parimaa_na(ចំនួនគត់));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * parimaa_na(ចំនួនគត់));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, parimaa_na(ចំនួនគត់));
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i) t.leaf[i] = 1;
    សម្រាប់(i = 0; i < n; ++i) ប្រសិនបើ(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, parimaa_na(tree));
    *tree_ptr = t;
    //error(0);
    ត្រឡប់ tree_ptr;
}
