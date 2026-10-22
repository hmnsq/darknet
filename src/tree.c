#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, var_na *leaf_list)
{
    list *llist = get_paths(leaf_list);
    var_na **leaves = (var_na **)list_to_array(llist);
    ຈຳນວນເຕັມ n = llist->size;
    ຈຳນວນເຕັມ i,j;
    ຈຳນວນເຕັມ found = 0;
    ສຳລັບ(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        ສຳລັບ(j = 0; j < n; ++j){
            ຖ້າ (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                bha_nga;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ stride)
{
    float p = 1;
    ໃນຂະນະທີ່(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    ສົ່ງຄືນ p;
}

void hierarchy_predictions(float *predictions, ຈຳນວນເຕັມ n, tree *hier, ຈຳນວນເຕັມ only_leaves, ຈຳນວນເຕັມ stride)
{
    ຈຳນວນເຕັມ j;
    ສຳລັບ(j = 0; j < n; ++j){
        ຈຳນວນເຕັມ parent = hier->parent[j];
        ຖ້າ(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    ຖ້າ(only_leaves){
        ສຳລັບ(j = 0; j < n; ++j){
            ຖ້າ(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

ຈຳນວນເຕັມ hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ຈຳນວນເຕັມ stride)
{
    float p = 1;
    ຈຳນວນເຕັມ group = 0;
    ຈຳນວນເຕັມ i;
    ໃນຂະນະທີ່(1){
        float max = 0;
        ຈຳນວນເຕັມ max_i = 0;

        ສຳລັບ(i = 0; i < hier->group_size[group]; ++i){
            ຈຳນວນເຕັມ index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            ຖ້າ(val > max){
                max_i = index;
                max = val;
            }
        }
        ຖ້າ(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            ຖ້າ(hier->child[max_i] < 0) ສົ່ງຄືນ max_i;
        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (group == 0){
            ສົ່ງຄືນ max_i;
        } ບໍ່ດັ່ງນັ້ນ {
            ສົ່ງຄືນ hier->parent[hier->group_offset[group]];
        }
    }
    ສົ່ງຄືນ 0;
}

tree *read_tree(var_na *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    var_na *line;
    ຈຳນວນເຕັມ last_parent = -1;
    ຈຳນວນເຕັມ group_size = 0;
    ຈຳນວນເຕັມ groups = 0;
    ຈຳນວນເຕັມ n = 0;
    ໃນຂະນະທີ່((line=fgetl(fp)) != 0){
        var_na *id = calloc(256, parimaa_na(var_na));
        ຈຳນວນເຕັມ parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*parimaa_na(ຈຳນວນເຕັມ));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*parimaa_na(ຈຳນວນເຕັມ));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*parimaa_na(var_na *));
        t.name[n] = id;
        ຖ້າ(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * parimaa_na(ຈຳນວນເຕັມ));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * parimaa_na(ຈຳນວນເຕັມ));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*parimaa_na(ຈຳນວນເຕັມ));
        t.group[n] = groups;
        ຖ້າ (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * parimaa_na(ຈຳນວນເຕັມ));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * parimaa_na(ຈຳນວນເຕັມ));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, parimaa_na(ຈຳນວນເຕັມ));
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i) t.leaf[i] = 1;
    ສຳລັບ(i = 0; i < n; ++i) ຖ້າ(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, parimaa_na(tree));
    *tree_ptr = t;
    //error(0);
    ສົ່ງຄືນ tree_ptr;
}
