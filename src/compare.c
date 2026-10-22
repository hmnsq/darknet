#ضمن <stdio.h>

#ضمن "network.h"
#ضمن "detection_layer.h"
#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "parser.h"
#ضمن "box.h"

فراغ train_compare(حرف *cfgfile, حرف *weightfile)
{
    srand(time(0));
    عائم avg_loss = -1;
    حرف *base = basecfg(cfgfile);
    حرف *backup_directory = "/home/pjreddie/backup/";
    printf("%s\n", base);
    network net = parse_network_cfg(cfgfile);
    ܐܢ(weightfile){
        load_weights(&net, weightfile);
    }
    printf("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_rate, net.momentum, net.decay);
    صحيح imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    حرف **paths = (حرف **)list_to_array(plist);
    صحيح N = plist->size;
    printf("%d\n", N);
    clock_t time;
    pthread_t load_thread;
    data train;
    data buffer;

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    args.classes = 20;
    args.n = imgs;
    args.m = N;
    args.d = &buffer;
    args.type = COMPARE_DATA;

    load_thread = load_data_in_thread(args);
    صحيح epoch = *net.seen/N;
    صحيح i = 0;
    ܟܡܐ(1){
        ++i;
        time=clock();
        pthread_join(load_thread, 0);
        train = buffer;

        load_thread = load_data_in_thread(args);
        printf("Loaded: %lf seconds\n", sec(clock()-time));
        time=clock();
        عائم loss = train_network(net, train);
        ܐܢ(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        printf("%.3f: %f, %f avg, %lf seconds, %ld images\n", (عائم)*net.seen/N, loss, avg_loss, sec(clock()-time), *net.seen);
        free_data(train);
        ܐܢ(i%100 == 0){
            حرف buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,base, epoch, i);
            save_weights(net, buff);
        }
        ܐܢ(*net.seen/N > epoch){
            epoch = *net.seen/N;
            i = 0;
            حرف buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,base, epoch);
            save_weights(net, buff);
            ܐܢ(epoch%22 == 0) net.learning_rate *= .1;
        }
    }
    pthread_join(load_thread, 0);
    free_data(buffer);
    free_network(net);
    free_ptrs((فراغ**)paths, plist->size);
    free_list(plist);
    حرر(base);
}

فراغ validate_compare(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    network net = parse_network_cfg(filename);
    ܐܢ(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_to_array(plist);
    صحيح N = plist->size/2;
    free_list(plist);

    clock_t time;
    صحيح correct = 0;
    صحيح total = 0;
    صحيح splits = 10;
    صحيح num = (i+1)*N/splits - i*N/splits;

    data val, buffer;

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    args.classes = 20;
    args.n = num;
    args.m = 0;
    args.d = &buffer;
    args.type = COMPARE_DATA;

    pthread_t load_thread = load_data_in_thread(args);
    لكل(i = 1; i <= splits; ++i){
        time=clock();

        pthread_join(load_thread, 0);
        val = buffer;

        num = (i+1)*N/splits - i*N/splits;
        حرف **part = paths+(i*N/splits);
        ܐܢ(i != splits){
            args.paths = part;
            load_thread = load_data_in_thread(args);
        }
        printf("Loaded: %d images in %lf seconds\n", val.X.rows, sec(clock()-time));

        time=clock();
        matrix pred = network_predict_data(net, val);
        صحيح j,k;
        لكل(j = 0; j < val.y.rows; ++j){
            لكل(k = 0; k < 20; ++k){
                ܐܢ(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++total;
                    ܐܢ((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_matrix(pred);
        printf("%d: Acc: %f, %lf seconds, %d images\n", i, (عائم)correct/total, sec(clock()-time), val.X.rows);
        free_data(val);
    }
}

تعريفنوع بنية {
    network net;
    حرف *filename;
    صحيح class;
    صحيح classes;
    عائم elo;
    عائم *elos;
} sortable_bbox;

صحيح total_compares = 0;
صحيح current_class = 0;

صحيح elo_comparator(ثابت فراغ*a, ثابت فراغ *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ܐܢ(box1.elos[current_class] == box2.elos[current_class]) ܦܢܐ 0;
    ܐܢ(box1.elos[current_class] >  box2.elos[current_class]) ܦܢܐ -1;
    ܦܢܐ 1;
}

صحيح bbox_comparator(ثابت فراغ *a, ثابت فراغ *b)
{
    ++total_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    network net = box1.net;
    صحيح class   = box1.class;

    image im1 = load_image_color(box1.filename, net.w, net.h);
    image im2 = load_image_color(box2.filename, net.w, net.h);
    عائم *X  = calloc(net.w*net.h*net.c, حجم(عائم));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*حجم(عائم));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = network_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    حرر(X);
    ܐܢ (predictions[class*2] > predictions[class*2+1]){
        ܦܢܐ 1;
    }
    ܦܢܐ -1;
}

فراغ bbox_update(sortable_bbox *a, sortable_bbox *b, صحيح class, صحيح result)
{
    صحيح k = 32;
    عائم EA = 1./(1+pow(10, (b->elos[class] - a->elos[class])/400.));
    عائم EB = 1./(1+pow(10, (a->elos[class] - b->elos[class])/400.));
    عائم SA = result ? 1 : 0;
    عائم SB = result ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

فراغ bbox_fight(network net, sortable_bbox *a, sortable_bbox *b, صحيح classes, صحيح class)
{
    image im1 = load_image_color(a->filename, net.w, net.h);
    image im2 = load_image_color(b->filename, net.w, net.h);
    عائم *X  = calloc(net.w*net.h*net.c, حجم(عائم));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*حجم(عائم));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = network_predict(net, X);
    ++total_compares;

    صحيح i;
    لكل(i = 0; i < classes; ++i){
        ܐܢ(class < 0 || class == i){
            صحيح result = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, result);
        }
    }
    
    free_image(im1);
    free_image(im2);
    حرر(X);
}

فراغ SortMaster3000(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    network net = parse_network_cfg(filename);
    ܐܢ(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_to_array(plist);
    صحيح N = plist->size;
    free_list(plist);
    sortable_bbox *boxes = calloc(N, حجم(sortable_bbox));
    printf("Sorting %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t time=clock();
    qsort(boxes, N, حجم(sortable_bbox), bbox_comparator);
    لكل(i = 0; i < N; ++i){
        printf("%s\n", boxes[i].filename);
    }
    printf("Sorted in %d compares, %f secs\n", total_compares, sec(clock()-time));
}

فراغ BattleRoyaleWithCheese(حرف *filename, حرف *weightfile)
{
    صحيح classes = 20;
    صحيح i,j;
    network net = parse_network_cfg(filename);
    ܐܢ(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_to_array(plist);
    صحيح N = plist->size;
    صحيح total = N;
    free_list(plist);
    sortable_bbox *boxes = calloc(N, حجم(sortable_bbox));
    printf("Battling %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = calloc(classes, حجم(عائم));;
        لكل(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    صحيح round;
    clock_t time=clock();
    لكل(round = 1; round <= 4; ++round){
        clock_t round_time=clock();
        printf("Round: %d\n", round);
        shuffle(boxes, N, حجم(sortable_bbox));
        لكل(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
    }

    صحيح class;

    لكل (class = 0; class < classes; ++class){

        N = total;
        current_class = class;
        qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
        N /= 2;

        لكل(round = 1; round <= 100; ++round){
            clock_t round_time=clock();
            printf("Round: %d\n", round);

            sorta_shuffle(boxes, N, حجم(sortable_bbox), 10);
            لكل(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
            ܐܢ(round <= 20) N = (N*9/10)/2*2;

            printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
        }
        حرف buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        لكل(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    printf("Tournament in %d compares, %f secs\n", total_compares, sec(clock()-time));
}

فراغ run_compare(صحيح argc, حرف **argv)
{
    ܐܢ(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        ܦܢܐ;
    }

    حرف *cfg = argv[3];
    حرف *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    ܐܢ(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    ܐܚܪܢܐ ܐܢ(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    ܐܚܪܢܐ ܐܢ(0==strcmp(argv[2], "sort")) SortMaster3000(cfg, weights);
    ܐܚܪܢܐ ܐܢ(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}
