#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, char *leaf_list)
{
    list *llist = get_paths(leaf_list);
    char **leaves = (char **)list_to_array(llist);
    整数 n = llist->size;
    整数 i,j;
    整数 found = 0;
    为(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        为(j = 0; j < n; ++j){
            如果 (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                break;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, 整数 c, 整数 stride)
{
    float p = 1;
    当(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    返回 p;
}

void hierarchy_predictions(float *predictions, 整数 n, tree *hier, 整数 only_leaves, 整数 stride)
{
    整数 j;
    为(j = 0; j < n; ++j){
        整数 parent = hier->parent[j];
        如果(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    如果(only_leaves){
        为(j = 0; j < n; ++j){
            如果(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

整数 hierarchy_top_prediction(float *predictions, tree *hier, float thresh, 整数 stride)
{
    float p = 1;
    整数 group = 0;
    整数 i;
    当(1){
        float max = 0;
        整数 max_i = 0;

        为(i = 0; i < hier->group_size[group]; ++i){
            整数 index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            如果(val > max){
                max_i = index;
                max = val;
            }
        }
        如果(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            如果(hier->child[max_i] < 0) 返回 max_i;
        } 否则 如果 (group == 0){
            返回 max_i;
        } 否则 {
            返回 hier->parent[hier->group_offset[group]];
        }
    }
    返回 0;
}

tree *read_tree(char *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    char *line;
    整数 last_parent = -1;
    整数 group_size = 0;
    整数 groups = 0;
    整数 n = 0;
    当((line=fgetl(fp)) != 0){
        char *id = calloc(256, sizeof(char));
        整数 parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*sizeof(整数));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*sizeof(整数));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*sizeof(char *));
        t.name[n] = id;
        如果(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * sizeof(整数));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * sizeof(整数));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*sizeof(整数));
        t.group[n] = groups;
        如果 (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * sizeof(整数));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * sizeof(整数));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, sizeof(整数));
    整数 i;
    为(i = 0; i < n; ++i) t.leaf[i] = 1;
    为(i = 0; i < n; ++i) 如果(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, sizeof(tree));
    *tree_ptr = t;
    //error(0);
    返回 tree_ptr;
}
