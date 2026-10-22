#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, char *leaf_list)
{
    list *llist = get_paths(leaf_list);
    char **leaves = (char **)list_to_array(llist);
    정수 n = llist->size;
    정수 i,j;
    정수 found = 0;
    위해(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        위해(j = 0; j < n; ++j){
            만약 (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                break;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, 정수 c, 정수 stride)
{
    float p = 1;
    동안(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    반환 p;
}

void hierarchy_predictions(float *predictions, 정수 n, tree *hier, 정수 only_leaves, 정수 stride)
{
    정수 j;
    위해(j = 0; j < n; ++j){
        정수 parent = hier->parent[j];
        만약(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    만약(only_leaves){
        위해(j = 0; j < n; ++j){
            만약(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

정수 hierarchy_top_prediction(float *predictions, tree *hier, float thresh, 정수 stride)
{
    float p = 1;
    정수 group = 0;
    정수 i;
    동안(1){
        float max = 0;
        정수 max_i = 0;

        위해(i = 0; i < hier->group_size[group]; ++i){
            정수 index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            만약(val > max){
                max_i = index;
                max = val;
            }
        }
        만약(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            만약(hier->child[max_i] < 0) 반환 max_i;
        } 아니면 만약 (group == 0){
            반환 max_i;
        } 아니면 {
            반환 hier->parent[hier->group_offset[group]];
        }
    }
    반환 0;
}

tree *read_tree(char *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    char *line;
    정수 last_parent = -1;
    정수 group_size = 0;
    정수 groups = 0;
    정수 n = 0;
    동안((line=fgetl(fp)) != 0){
        char *id = calloc(256, sizeof(char));
        정수 parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*sizeof(정수));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*sizeof(정수));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*sizeof(char *));
        t.name[n] = id;
        만약(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * sizeof(정수));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * sizeof(정수));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*sizeof(정수));
        t.group[n] = groups;
        만약 (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * sizeof(정수));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * sizeof(정수));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, sizeof(정수));
    정수 i;
    위해(i = 0; i < n; ++i) t.leaf[i] = 1;
    위해(i = 0; i < n; ++i) 만약(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, sizeof(tree));
    *tree_ptr = t;
    //error(0);
    반환 tree_ptr;
}
