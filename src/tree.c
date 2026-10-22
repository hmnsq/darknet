#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, char *leaf_list)
{
    list *llist = get_paths(leaf_list);
    char **leaves = (char **)list_to_array(llist);
    số_nguyên n = llist->size;
    số_nguyên i,j;
    số_nguyên found = 0;
    cho(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        cho(j = 0; j < n; ++j){
            nếu (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                break;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, số_nguyên c, số_nguyên stride)
{
    float p = 1;
    trong_khi(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    trả_về p;
}

void hierarchy_predictions(float *predictions, số_nguyên n, tree *hier, số_nguyên only_leaves, số_nguyên stride)
{
    số_nguyên j;
    cho(j = 0; j < n; ++j){
        số_nguyên parent = hier->parent[j];
        nếu(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    nếu(only_leaves){
        cho(j = 0; j < n; ++j){
            nếu(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

số_nguyên hierarchy_top_prediction(float *predictions, tree *hier, float thresh, số_nguyên stride)
{
    float p = 1;
    số_nguyên group = 0;
    số_nguyên i;
    trong_khi(1){
        float max = 0;
        số_nguyên max_i = 0;

        cho(i = 0; i < hier->group_size[group]; ++i){
            số_nguyên index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            nếu(val > max){
                max_i = index;
                max = val;
            }
        }
        nếu(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            nếu(hier->child[max_i] < 0) trả_về max_i;
        } không_thì nếu (group == 0){
            trả_về max_i;
        } không_thì {
            trả_về hier->parent[hier->group_offset[group]];
        }
    }
    trả_về 0;
}

tree *read_tree(char *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    char *line;
    số_nguyên last_parent = -1;
    số_nguyên group_size = 0;
    số_nguyên groups = 0;
    số_nguyên n = 0;
    trong_khi((line=fgetl(fp)) != 0){
        char *id = calloc(256, sizeof(char));
        số_nguyên parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*sizeof(số_nguyên));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*sizeof(số_nguyên));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*sizeof(char *));
        t.name[n] = id;
        nếu(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * sizeof(số_nguyên));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * sizeof(số_nguyên));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*sizeof(số_nguyên));
        t.group[n] = groups;
        nếu (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * sizeof(số_nguyên));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * sizeof(số_nguyên));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, sizeof(số_nguyên));
    số_nguyên i;
    cho(i = 0; i < n; ++i) t.leaf[i] = 1;
    cho(i = 0; i < n; ++i) nếu(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, sizeof(tree));
    *tree_ptr = t;
    //error(0);
    trả_về tree_ptr;
}
