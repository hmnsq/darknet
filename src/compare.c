#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ "network.h"
#ଅନ୍ତର୍ଭାଵ "detection_layer.h"
#ଅନ୍ତର୍ଭାଵ "cost_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"
#ଅନ୍ତର୍ଭାଵ "box.h"

ଶୂନ୍ୟ train_compare(ଅକ୍ଷର *cfgfile, ଅକ୍ଷର *weightfile)
{
    srand(time(0));
    ଦଶମିକ avg_loss = -1;
    ଅକ୍ଷର *base = basecfg(cfgfile);
    ଅକ୍ଷର *backup_directory = "/home/pjreddie/backup/";
    printf("%s\n", base);
    network net = parse_network_cfg(cfgfile);
    ଯଦି(weightfile){
        load_weights(&net, weightfile);
    }
    printf("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_rate, net.momentum, net.decay);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    ଅକ୍ଷର **paths = (ଅକ୍ଷର **)list_to_array(plist);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = plist->size;
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା epoch = *net.seen/N;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ଯେତେବେଳେ(1){
        ++i;
        time=clock();
        pthread_join(load_thread, 0);
        train = buffer;

        load_thread = load_data_in_thread(args);
        printf("Loaded: %lf seconds\n", sec(clock()-time));
        time=clock();
        ଦଶମିକ loss = train_network(net, train);
        ଯଦି(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        printf("%.3f: %f, %f avg, %lf seconds, %ld images\n", (ଦଶମିକ)*net.seen/N, loss, avg_loss, sec(clock()-time), *net.seen);
        free_data(train);
        ଯଦି(i%100 == 0){
            ଅକ୍ଷର buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,base, epoch, i);
            save_weights(net, buff);
        }
        ଯଦି(*net.seen/N > epoch){
            epoch = *net.seen/N;
            i = 0;
            ଅକ୍ଷର buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,base, epoch);
            save_weights(net, buff);
            ଯଦି(epoch%22 == 0) net.learning_rate *= .1;
        }
    }
    pthread_join(load_thread, 0);
    free_data(buffer);
    free_network(net);
    free_ptrs((ଶୂନ୍ୟ**)paths, plist->size);
    free_list(plist);
    ମୁକ୍ତ(base);
}

ଶୂନ୍ୟ validate_compare(ଅକ୍ଷର *filename, ଅକ୍ଷର *weightfile)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    network net = parse_network_cfg(filename);
    ଯଦି(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    ଅକ୍ଷର **paths = (ଅକ୍ଷର **)list_to_array(plist);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = plist->size/2;
    free_list(plist);

    clock_t time;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା correct = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା splits = 10;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = (i+1)*N/splits - i*N/splits;

    data val, buffer;

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    args.classes = 20;
    args.n = ଅଙ୍କ;
    args.m = 0;
    args.d = &buffer;
    args.type = COMPARE_DATA;

    pthread_t load_thread = load_data_in_thread(args);
    ପାଇଁ(i = 1; i <= splits; ++i){
        time=clock();

        pthread_join(load_thread, 0);
        val = buffer;

        ଅଙ୍କ = (i+1)*N/splits - i*N/splits;
        ଅକ୍ଷର **part = paths+(i*N/splits);
        ଯଦି(i != splits){
            args.paths = part;
            load_thread = load_data_in_thread(args);
        }
        printf("Loaded: %d images in %lf seconds\n", val.X.rows, sec(clock()-time));

        time=clock();
        matrix pred = network_predict_data(net, val);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j,k;
        ପାଇଁ(j = 0; j < val.y.rows; ++j){
            ପାଇଁ(k = 0; k < 20; ++k){
                ଯଦି(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++total;
                    ଯଦି((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_matrix(pred);
        printf("%d: Acc: %f, %lf seconds, %d images\n", i, (ଦଶମିକ)correct/total, sec(clock()-time), val.X.rows);
        free_data(val);
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    network net;
    ଅକ୍ଷର *filename;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes;
    ଦଶମିକ elo;
    ଦଶମିକ *elos;
} sortable_bbox;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_compares = 0;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_class = 0;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା elo_comparator(ଅଚଳ ଶୂନ୍ୟ*a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ଯଦି(box1.elos[current_class] == box2.elos[current_class]) ଫେରନ୍ତୁ 0;
    ଯଦି(box1.elos[current_class] >  box2.elos[current_class]) ଫେରନ୍ତୁ -1;
    ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା bbox_comparator(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ++total_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    network net = box1.net;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class   = box1.class;

    image im1 = load_image_color(box1.filename, net.w, net.h);
    image im2 = load_image_color(box2.filename, net.w, net.h);
    ଦଶମିକ *X  = calloc(net.w*net.h*net.c, ଆକାର(ଦଶମିକ));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*ଆକାର(ଦଶମିକ));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*ଆକାର(ଦଶମିକ));
    ଦଶମିକ *predictions = network_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    ମୁକ୍ତ(X);
    ଯଦି (predictions[class*2] > predictions[class*2+1]){
        ଫେରନ୍ତୁ 1;
    }
    ଫେରନ୍ତୁ -1;
}

ଶୂନ୍ୟ bbox_update(sortable_bbox *a, sortable_bbox *b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା class, ପୂର୍ଣ୍ଣସଂଖ୍ୟା result)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 32;
    ଦଶମିକ EA = 1./(1+pow(10, (b->elos[class] - a->elos[class])/400.));
    ଦଶମିକ EB = 1./(1+pow(10, (a->elos[class] - b->elos[class])/400.));
    ଦଶମିକ SA = result ? 1 : 0;
    ଦଶମିକ SB = result ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

ଶୂନ୍ୟ bbox_fight(network net, sortable_bbox *a, sortable_bbox *b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା class)
{
    image im1 = load_image_color(a->filename, net.w, net.h);
    image im2 = load_image_color(b->filename, net.w, net.h);
    ଦଶମିକ *X  = calloc(net.w*net.h*net.c, ଆକାର(ଦଶମିକ));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*ଆକାର(ଦଶମିକ));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*ଆକାର(ଦଶମିକ));
    ଦଶମିକ *predictions = network_predict(net, X);
    ++total_compares;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < classes; ++i){
        ଯଦି(class < 0 || class == i){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, result);
        }
    }
    
    free_image(im1);
    free_image(im2);
    ମୁକ୍ତ(X);
}

ଶୂନ୍ୟ SortMaster3000(ଅକ୍ଷର *filename, ଅକ୍ଷର *weightfile)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    network net = parse_network_cfg(filename);
    ଯଦି(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    ଅକ୍ଷର **paths = (ଅକ୍ଷର **)list_to_array(plist);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = plist->size;
    free_list(plist);
    sortable_bbox *boxes = calloc(N, ଆକାର(sortable_bbox));
    printf("Sorting %d boxes...\n", N);
    ପାଇଁ(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t time=clock();
    qsort(boxes, N, ଆକାର(sortable_bbox), bbox_comparator);
    ପାଇଁ(i = 0; i < N; ++i){
        printf("%s\n", boxes[i].filename);
    }
    printf("Sorted in %d compares, %f secs\n", total_compares, sec(clock()-time));
}

ଶୂନ୍ୟ BattleRoyaleWithCheese(ଅକ୍ଷର *filename, ଅକ୍ଷର *weightfile)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes = 20;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    network net = parse_network_cfg(filename);
    ଯଦି(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    ଅକ୍ଷର **paths = (ଅକ୍ଷର **)list_to_array(plist);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = plist->size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = N;
    free_list(plist);
    sortable_bbox *boxes = calloc(N, ଆକାର(sortable_bbox));
    printf("Battling %d boxes...\n", N);
    ପାଇଁ(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = calloc(classes, ଆକାର(ଦଶମିକ));;
        ପାଇଁ(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା round;
    clock_t time=clock();
    ପାଇଁ(round = 1; round <= 4; ++round){
        clock_t round_time=clock();
        printf("Round: %d\n", round);
        shuffle(boxes, N, ଆକାର(sortable_bbox));
        ପାଇଁ(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class;

    ପାଇଁ (class = 0; class < classes; ++class){

        N = total;
        current_class = class;
        qsort(boxes, N, ଆକାର(sortable_bbox), elo_comparator);
        N /= 2;

        ପାଇଁ(round = 1; round <= 100; ++round){
            clock_t round_time=clock();
            printf("Round: %d\n", round);

            sorta_shuffle(boxes, N, ଆକାର(sortable_bbox), 10);
            ପାଇଁ(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, ଆକାର(sortable_bbox), elo_comparator);
            ଯଦି(round <= 20) N = (N*9/10)/2*2;

            printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
        }
        ଅକ୍ଷର buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        ପାଇଁ(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    printf("Tournament in %d compares, %f secs\n", total_compares, sec(clock()-time));
}

ଶୂନ୍ୟ run_compare(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
    ଯଦି(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        ଫେରନ୍ତୁ;
    }

    ଅକ୍ଷର *cfg = argv[3];
    ଅକ୍ଷର *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    ଯଦି(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    ଅନ୍ୟଥା ଯଦି(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    ଅନ୍ୟଥା ଯଦି(0==strcmp(argv[2], "sort")) SortMaster3000(cfg, weights);
    ଅନ୍ୟଥା ଯଦି(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}
