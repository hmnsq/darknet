#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, var_na *leaf_list)
{
    list *llist = get_paths(leaf_list);
    var_na **leaves = (var_na **)list_to_array(llist);
    จำนวนเต็ม n = llist->size;
    จำนวนเต็ม i,j;
    จำนวนเต็ม found = 0;
    สำหรับ(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        สำหรับ(j = 0; j < n; ++j){
            ถ้า (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                bha_nga;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, จำนวนเต็ม c, จำนวนเต็ม stride)
{
    float p = 1;
    ขณะ(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    คืน p;
}

void hierarchy_predictions(float *predictions, จำนวนเต็ม n, tree *hier, จำนวนเต็ม only_leaves, จำนวนเต็ม stride)
{
    จำนวนเต็ม j;
    สำหรับ(j = 0; j < n; ++j){
        จำนวนเต็ม parent = hier->parent[j];
        ถ้า(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    ถ้า(only_leaves){
        สำหรับ(j = 0; j < n; ++j){
            ถ้า(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

จำนวนเต็ม hierarchy_top_prediction(float *predictions, tree *hier, float thresh, จำนวนเต็ม stride)
{
    float p = 1;
    จำนวนเต็ม group = 0;
    จำนวนเต็ม i;
    ขณะ(1){
        float max = 0;
        จำนวนเต็ม max_i = 0;

        สำหรับ(i = 0; i < hier->group_size[group]; ++i){
            จำนวนเต็ม index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            ถ้า(val > max){
                max_i = index;
                max = val;
            }
        }
        ถ้า(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            ถ้า(hier->child[max_i] < 0) คืน max_i;
        } มิฉะนั้น ถ้า (group == 0){
            คืน max_i;
        } มิฉะนั้น {
            คืน hier->parent[hier->group_offset[group]];
        }
    }
    คืน 0;
}

tree *read_tree(var_na *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    var_na *line;
    จำนวนเต็ม last_parent = -1;
    จำนวนเต็ม group_size = 0;
    จำนวนเต็ม groups = 0;
    จำนวนเต็ม n = 0;
    ขณะ((line=fgetl(fp)) != 0){
        var_na *id = calloc(256, parimaa_na(var_na));
        จำนวนเต็ม parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*parimaa_na(จำนวนเต็ม));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*parimaa_na(จำนวนเต็ม));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*parimaa_na(var_na *));
        t.name[n] = id;
        ถ้า(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * parimaa_na(จำนวนเต็ม));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * parimaa_na(จำนวนเต็ม));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*parimaa_na(จำนวนเต็ม));
        t.group[n] = groups;
        ถ้า (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * parimaa_na(จำนวนเต็ม));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * parimaa_na(จำนวนเต็ม));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, parimaa_na(จำนวนเต็ม));
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i) t.leaf[i] = 1;
    สำหรับ(i = 0; i < n; ++i) ถ้า(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, parimaa_na(tree));
    *tree_ptr = t;
    //error(0);
    คืน tree_ptr;
}
