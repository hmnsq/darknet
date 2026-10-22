#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "detection_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cost_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"

ਖਾਲੀ train_compare(ਅੱਖਰ *cfgfile, ਅੱਖਰ *weightfile)
{
    srand(ਸਮਾਂ(0));
    ਦਸ਼ਮਲਵ avg_loss = -1;
    ਅੱਖਰ *ਅਧਾਰ = basecfg(cfgfile);
    ਅੱਖਰ *backup_directory = "/home/pjreddie/backup/";
    ਲਿਖੋ("%s\n", ਅਧਾਰ);
    ਜਾਲ net = parse_ਜਾਲ_cfg(cfgfile);
    ਜੇ(weightfile){
        ਲੱਦੋ_weights(&net, weightfile);
    }
    ਲਿਖੋ("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_ਦਰ, net.momentum, net.decay);
    ਪੂਰਨ imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    ਅੱਖਰ **paths = (ਅੱਖਰ **)list_ਮੰਜ਼ਿਲ_array(plist);
    ਪੂਰਨ N = plist->ਮਾਪ;
    ਲਿਖੋ("%d\n", N);
    clock_t ਸਮਾਂ;
    pthread_t ਲੱਦੋ_thread;
    ਅੰਕੜੇ train;
    ਅੰਕੜੇ ਬਫਰ;

    ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {0};
    ਦਲੀਲਾਂ.w = net.w;
    ਦਲੀਲਾਂ.h = net.h;
    ਦਲੀਲਾਂ.paths = paths;
    ਦਲੀਲਾਂ.classes = 20;
    ਦਲੀਲਾਂ.n = imgs;
    ਦਲੀਲਾਂ.m = N;
    ਦਲੀਲਾਂ.d = &ਬਫਰ;
    ਦਲੀਲਾਂ.type = COMPARE_ਅੰਕੜੇ;

    ਲੱਦੋ_thread = ਲੱਦੋ_ਅੰਕੜੇ_ਵਿੱਚ_thread(ਦਲੀਲਾਂ);
    ਪੂਰਨ epoch = *net.ਵੇਖਿਆ/N;
    ਪੂਰਨ i = 0;
    ਜਦੋਂ(1){
        ++i;
        ਸਮਾਂ=clock();
        pthread_join(ਲੱਦੋ_thread, 0);
        train = ਬਫਰ;

        ਲੱਦੋ_thread = ਲੱਦੋ_ਅੰਕੜੇ_ਵਿੱਚ_thread(ਦਲੀਲਾਂ);
        ਲਿਖੋ("Loaded: %lf seconds\n", sec(clock()-ਸਮਾਂ));
        ਸਮਾਂ=clock();
        ਦਸ਼ਮਲਵ loss = train_ਜਾਲ(net, train);
        ਜੇ(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        ਲਿਖੋ("%.3f: %f, %f avg, %lf seconds, %ld images\n", (ਦਸ਼ਮਲਵ)*net.ਵੇਖਿਆ/N, loss, avg_loss, sec(clock()-ਸਮਾਂ), *net.ਵੇਖਿਆ);
        free_ਅੰਕੜੇ(train);
        ਜੇ(i%100 == 0){
            ਅੱਖਰ buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,ਅਧਾਰ, epoch, i);
            save_weights(net, buff);
        }
        ਜੇ(*net.ਵੇਖਿਆ/N > epoch){
            epoch = *net.ਵੇਖਿਆ/N;
            i = 0;
            ਅੱਖਰ buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,ਅਧਾਰ, epoch);
            save_weights(net, buff);
            ਜੇ(epoch%22 == 0) net.learning_ਦਰ *= .1;
        }
    }
    pthread_join(ਲੱਦੋ_thread, 0);
    free_ਅੰਕੜੇ(ਬਫਰ);
    free_ਜਾਲ(net);
    free_ptrs((ਖਾਲੀ**)paths, plist->ਮਾਪ);
    free_list(plist);
    ਮੁਕਤ(ਅਧਾਰ);
}

ਖਾਲੀ validate_compare(ਅੱਖਰ *filename, ਅੱਖਰ *weightfile)
{
    ਪੂਰਨ i = 0;
    ਜਾਲ net = parse_ਜਾਲ_cfg(filename);
    ਜੇ(weightfile){
        ਲੱਦੋ_weights(&net, weightfile);
    }
    srand(ਸਮਾਂ(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    ਅੱਖਰ **paths = (ਅੱਖਰ **)list_ਮੰਜ਼ਿਲ_array(plist);
    ਪੂਰਨ N = plist->ਮਾਪ/2;
    free_list(plist);

    clock_t ਸਮਾਂ;
    ਪੂਰਨ correct = 0;
    ਪੂਰਨ ਕੁੱਲ = 0;
    ਪੂਰਨ splits = 10;
    ਪੂਰਨ ਅੰਕ = (i+1)*N/splits - i*N/splits;

    ਅੰਕੜੇ val, ਬਫਰ;

    ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {0};
    ਦਲੀਲਾਂ.w = net.w;
    ਦਲੀਲਾਂ.h = net.h;
    ਦਲੀਲਾਂ.paths = paths;
    ਦਲੀਲਾਂ.classes = 20;
    ਦਲੀਲਾਂ.n = ਅੰਕ;
    ਦਲੀਲਾਂ.m = 0;
    ਦਲੀਲਾਂ.d = &ਬਫਰ;
    ਦਲੀਲਾਂ.type = COMPARE_ਅੰਕੜੇ;

    pthread_t ਲੱਦੋ_thread = ਲੱਦੋ_ਅੰਕੜੇ_ਵਿੱਚ_thread(ਦਲੀਲਾਂ);
    ਲਈ(i = 1; i <= splits; ++i){
        ਸਮਾਂ=clock();

        pthread_join(ਲੱਦੋ_thread, 0);
        val = ਬਫਰ;

        ਅੰਕ = (i+1)*N/splits - i*N/splits;
        ਅੱਖਰ **part = paths+(i*N/splits);
        ਜੇ(i != splits){
            ਦਲੀਲਾਂ.paths = part;
            ਲੱਦੋ_thread = ਲੱਦੋ_ਅੰਕੜੇ_ਵਿੱਚ_thread(ਦਲੀਲਾਂ);
        }
        ਲਿਖੋ("Loaded: %d images in %lf seconds\n", val.X.ਪੰਕਤੀਆਂ, sec(clock()-ਸਮਾਂ));

        ਸਮਾਂ=clock();
        ਮੈਟ੍ਰਿਕਸ pred = ਜਾਲ_predict_ਅੰਕੜੇ(net, val);
        ਪੂਰਨ j,k;
        ਲਈ(j = 0; j < val.y.ਪੰਕਤੀਆਂ; ++j){
            ਲਈ(k = 0; k < 20; ++k){
                ਜੇ(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++ਕੁੱਲ;
                    ਜੇ((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_ਮੈਟ੍ਰਿਕਸ(pred);
        ਲਿਖੋ("%d: Acc: %f, %lf seconds, %d images\n", i, (ਦਸ਼ਮਲਵ)correct/ਕੁੱਲ, sec(clock()-ਸਮਾਂ), val.X.ਪੰਕਤੀਆਂ);
        free_ਅੰਕੜੇ(val);
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਜਾਲ net;
    ਅੱਖਰ *filename;
    ਪੂਰਨ class;
    ਪੂਰਨ classes;
    ਦਸ਼ਮਲਵ elo;
    ਦਸ਼ਮਲਵ *elos;
} sortable_bbox;

ਪੂਰਨ ਕੁੱਲ_compares = 0;
ਪੂਰਨ current_class = 0;

ਪੂਰਨ elo_comparator(ਅਚਲ ਖਾਲੀ*a, ਅਚਲ ਖਾਲੀ *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ਜੇ(box1.elos[current_class] == box2.elos[current_class]) ਵਾਪਸ 0;
    ਜੇ(box1.elos[current_class] >  box2.elos[current_class]) ਵਾਪਸ -1;
    ਵਾਪਸ 1;
}

ਪੂਰਨ bbox_comparator(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ++ਕੁੱਲ_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ਜਾਲ net = box1.net;
    ਪੂਰਨ class   = box1.class;

    image im1 = ਲੱਦੋ_image_color(box1.filename, net.w, net.h);
    image im2 = ਲੱਦੋ_image_color(box2.filename, net.w, net.h);
    ਦਸ਼ਮਲਵ *X  = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net.w*net.h*net.c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(X,                   im1.ਅੰਕੜੇ, im1.w*im1.h*im1.c*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(X+im1.w*im1.h*im1.c, im2.ਅੰਕੜੇ, im2.w*im2.h*im2.c*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਦਸ਼ਮਲਵ *predictions = ਜਾਲ_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    ਮੁਕਤ(X);
    ਜੇ (predictions[class*2] > predictions[class*2+1]){
        ਵਾਪਸ 1;
    }
    ਵਾਪਸ -1;
}

ਖਾਲੀ bbox_update(sortable_bbox *a, sortable_bbox *b, ਪੂਰਨ class, ਪੂਰਨ ਨਤੀਜਾ)
{
    ਪੂਰਨ k = 32;
    ਦਸ਼ਮਲਵ EA = 1./(1+ਘਾਤ(10, (b->elos[class] - a->elos[class])/400.));
    ਦਸ਼ਮਲਵ EB = 1./(1+ਘਾਤ(10, (a->elos[class] - b->elos[class])/400.));
    ਦਸ਼ਮਲਵ SA = ਨਤੀਜਾ ? 1 : 0;
    ਦਸ਼ਮਲਵ SB = ਨਤੀਜਾ ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

ਖਾਲੀ bbox_fight(ਜਾਲ net, sortable_bbox *a, sortable_bbox *b, ਪੂਰਨ classes, ਪੂਰਨ class)
{
    image im1 = ਲੱਦੋ_image_color(a->filename, net.w, net.h);
    image im2 = ਲੱਦੋ_image_color(b->filename, net.w, net.h);
    ਦਸ਼ਮਲਵ *X  = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net.w*net.h*net.c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(X,                   im1.ਅੰਕੜੇ, im1.w*im1.h*im1.c*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(X+im1.w*im1.h*im1.c, im2.ਅੰਕੜੇ, im2.w*im2.h*im2.c*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਦਸ਼ਮਲਵ *predictions = ਜਾਲ_predict(net, X);
    ++ਕੁੱਲ_compares;

    ਪੂਰਨ i;
    ਲਈ(i = 0; i < classes; ++i){
        ਜੇ(class < 0 || class == i){
            ਪੂਰਨ ਨਤੀਜਾ = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, ਨਤੀਜਾ);
        }
    }
    
    free_image(im1);
    free_image(im2);
    ਮੁਕਤ(X);
}

ਖਾਲੀ ਛਾਂਟੋ_Master3000(ਅੱਖਰ *filename, ਅੱਖਰ *weightfile)
{
    ਪੂਰਨ i = 0;
    ਜਾਲ net = parse_ਜਾਲ_cfg(filename);
    ਜੇ(weightfile){
        ਲੱਦੋ_weights(&net, weightfile);
    }
    srand(ਸਮਾਂ(0));
    ਸੈੱਟ_batch_ਜਾਲ(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    ਅੱਖਰ **paths = (ਅੱਖਰ **)list_ਮੰਜ਼ਿਲ_array(plist);
    ਪੂਰਨ N = plist->ਮਾਪ;
    free_list(plist);
    sortable_bbox *boxes = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(N, ਆਕਾਰ(sortable_bbox));
    ਲਿਖੋ("Sorting %d boxes...\n", N);
    ਲਈ(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t ਸਮਾਂ=clock();
    qsort(boxes, N, ਆਕਾਰ(sortable_bbox), bbox_comparator);
    ਲਈ(i = 0; i < N; ++i){
        ਲਿਖੋ("%s\n", boxes[i].filename);
    }
    ਲਿਖੋ("Sorted in %d compares, %f secs\n", ਕੁੱਲ_compares, sec(clock()-ਸਮਾਂ));
}

ਖਾਲੀ BattleRoyaleWithCheese(ਅੱਖਰ *filename, ਅੱਖਰ *weightfile)
{
    ਪੂਰਨ classes = 20;
    ਪੂਰਨ i,j;
    ਜਾਲ net = parse_ਜਾਲ_cfg(filename);
    ਜੇ(weightfile){
        ਲੱਦੋ_weights(&net, weightfile);
    }
    srand(ਸਮਾਂ(0));
    ਸੈੱਟ_batch_ਜਾਲ(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    ਅੱਖਰ **paths = (ਅੱਖਰ **)list_ਮੰਜ਼ਿਲ_array(plist);
    ਪੂਰਨ N = plist->ਮਾਪ;
    ਪੂਰਨ ਕੁੱਲ = N;
    free_list(plist);
    sortable_bbox *boxes = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(N, ਆਕਾਰ(sortable_bbox));
    ਲਿਖੋ("Battling %d boxes...\n", N);
    ਲਈ(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(classes, ਆਕਾਰ(ਦਸ਼ਮਲਵ));;
        ਲਈ(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    ਪੂਰਨ ਗੇੜਾ;
    clock_t ਸਮਾਂ=clock();
    ਲਈ(ਗੇੜਾ = 1; ਗੇੜਾ <= 4; ++ਗੇੜਾ){
        clock_t ਗੇੜਾ_ਸਮਾਂ=clock();
        ਲਿਖੋ("Round: %d\n", ਗੇੜਾ);
        shuffle(boxes, N, ਆਕਾਰ(sortable_bbox));
        ਲਈ(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        ਲਿਖੋ("Round: %f secs, %d remaining\n", sec(clock()-ਗੇੜਾ_ਸਮਾਂ), N);
    }

    ਪੂਰਨ class;

    ਲਈ (class = 0; class < classes; ++class){

        N = ਕੁੱਲ;
        current_class = class;
        qsort(boxes, N, ਆਕਾਰ(sortable_bbox), elo_comparator);
        N /= 2;

        ਲਈ(ਗੇੜਾ = 1; ਗੇੜਾ <= 100; ++ਗੇੜਾ){
            clock_t ਗੇੜਾ_ਸਮਾਂ=clock();
            ਲਿਖੋ("Round: %d\n", ਗੇੜਾ);

            sorta_shuffle(boxes, N, ਆਕਾਰ(sortable_bbox), 10);
            ਲਈ(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, ਆਕਾਰ(sortable_bbox), elo_comparator);
            ਜੇ(ਗੇੜਾ <= 20) N = (N*9/10)/2*2;

            ਲਿਖੋ("Round: %f secs, %d remaining\n", sec(clock()-ਗੇੜਾ_ਸਮਾਂ), N);
        }
        ਅੱਖਰ buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        ਲਈ(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    ਲਿਖੋ("Tournament in %d compares, %f secs\n", ਕੁੱਲ_compares, sec(clock()-ਸਮਾਂ));
}

ਖਾਲੀ ਲੜੀਵਾਰ_compare(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
    ਜੇ(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        ਵਾਪਸ;
    }

    ਅੱਖਰ *cfg = argv[3];
    ਅੱਖਰ *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    ਜੇ(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    ਨਹੀਂਤਾਂ ਜੇ(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    ਨਹੀਂਤਾਂ ਜੇ(0==strcmp(argv[2], "sort")) ਛਾਂਟੋ_Master3000(cfg, weights);
    ਨਹੀਂਤਾਂ ਜੇ(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}
