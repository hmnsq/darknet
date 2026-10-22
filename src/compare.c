#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "network.h"
#ಅನ್ತರ್ಭಾವ "detection_layer.h"
#ಅನ್ತರ್ಭಾವ "cost_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "parser.h"
#ಅನ್ತರ್ಭಾವ "box.h"

ಶೂನ್ಯ train_compare(ಅಕ್ಷರ *cfgfile, ಅಕ್ಷರ *weightfile)
{
    srand(ಸಮಯ(0));
    ದಶಮಾಂಶ avg_loss = -1;
    ಅಕ್ಷರ *ತಳ = basecfg(cfgfile);
    ಅಕ್ಷರ *backup_directory = "/home/pjreddie/backup/";
    ಬರೆ("%s\n", ತಳ);
    ಜಾಲ net = parse_ಜಾಲ_cfg(cfgfile);
    ಆದರೆ(weightfile){
        ತುಂಬು_weights(&net, weightfile);
    }
    ಬರೆ("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_ದರ, net.momentum, net.decay);
    ಪೂರ್ಣಾಂಕ imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    ಅಕ್ಷರ **paths = (ಅಕ್ಷರ **)list_ಗಮ್ಯ_array(plist);
    ಪೂರ್ಣಾಂಕ N = plist->ಗಾತ್ರ_2;
    ಬರೆ("%d\n", N);
    clock_t ಸಮಯ;
    pthread_t ತುಂಬು_thread;
    ದತ್ತಾಂಶ train;
    ದತ್ತಾಂಶ ಬಫರ್;

    ತುಂಬು_ವಾದಗಳು ವಾದಗಳು = {0};
    ವಾದಗಳು.w = net.w;
    ವಾದಗಳು.h = net.h;
    ವಾದಗಳು.paths = paths;
    ವಾದಗಳು.classes = 20;
    ವಾದಗಳು.n = imgs;
    ವಾದಗಳು.m = N;
    ವಾದಗಳು.d = &ಬಫರ್;
    ವಾದಗಳು.type = COMPARE_ದತ್ತಾಂಶ;

    ತುಂಬು_thread = ತುಂಬು_ದತ್ತಾಂಶ_ಒಳ_thread(ವಾದಗಳು);
    ಪೂರ್ಣಾಂಕ epoch = *net.ನೋಡಿದ/N;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಆಗ(1){
        ++i;
        ಸಮಯ=clock();
        pthread_join(ತುಂಬು_thread, 0);
        train = ಬಫರ್;

        ತುಂಬು_thread = ತುಂಬು_ದತ್ತಾಂಶ_ಒಳ_thread(ವಾದಗಳು);
        ಬರೆ("Loaded: %lf seconds\n", sec(clock()-ಸಮಯ));
        ಸಮಯ=clock();
        ದಶಮಾಂಶ loss = train_ಜಾಲ(net, train);
        ಆದರೆ(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        ಬರೆ("%.3f: %f, %f avg, %lf seconds, %ld images\n", (ದಶಮಾಂಶ)*net.ನೋಡಿದ/N, loss, avg_loss, sec(clock()-ಸಮಯ), *net.ನೋಡಿದ);
        free_ದತ್ತಾಂಶ(train);
        ಆದರೆ(i%100 == 0){
            ಅಕ್ಷರ buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,ತಳ, epoch, i);
            save_weights(net, buff);
        }
        ಆದರೆ(*net.ನೋಡಿದ/N > epoch){
            epoch = *net.ನೋಡಿದ/N;
            i = 0;
            ಅಕ್ಷರ buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,ತಳ, epoch);
            save_weights(net, buff);
            ಆದರೆ(epoch%22 == 0) net.learning_ದರ *= .1;
        }
    }
    pthread_join(ತುಂಬು_thread, 0);
    free_ದತ್ತಾಂಶ(ಬಫರ್);
    free_ಜಾಲ(net);
    free_ptrs((ಶೂನ್ಯ**)paths, plist->ಗಾತ್ರ_2);
    free_list(plist);
    ಬಿಡುಗಡೆ(ತಳ);
}

ಶೂನ್ಯ validate_compare(ಅಕ್ಷರ *filename, ಅಕ್ಷರ *weightfile)
{
    ಪೂರ್ಣಾಂಕ i = 0;
    ಜಾಲ net = parse_ಜಾಲ_cfg(filename);
    ಆದರೆ(weightfile){
        ತುಂಬು_weights(&net, weightfile);
    }
    srand(ಸಮಯ(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    ಅಕ್ಷರ **paths = (ಅಕ್ಷರ **)list_ಗಮ್ಯ_array(plist);
    ಪೂರ್ಣಾಂಕ N = plist->ಗಾತ್ರ_2/2;
    free_list(plist);

    clock_t ಸಮಯ;
    ಪೂರ್ಣಾಂಕ correct = 0;
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = 0;
    ಪೂರ್ಣಾಂಕ splits = 10;
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = (i+1)*N/splits - i*N/splits;

    ದತ್ತಾಂಶ val, ಬಫರ್;

    ತುಂಬು_ವಾದಗಳು ವಾದಗಳು = {0};
    ವಾದಗಳು.w = net.w;
    ವಾದಗಳು.h = net.h;
    ವಾದಗಳು.paths = paths;
    ವಾದಗಳು.classes = 20;
    ವಾದಗಳು.n = ಅಂಕೆ;
    ವಾದಗಳು.m = 0;
    ವಾದಗಳು.d = &ಬಫರ್;
    ವಾದಗಳು.type = COMPARE_ದತ್ತಾಂಶ;

    pthread_t ತುಂಬು_thread = ತುಂಬು_ದತ್ತಾಂಶ_ಒಳ_thread(ವಾದಗಳು);
    ಗಾಗಿ(i = 1; i <= splits; ++i){
        ಸಮಯ=clock();

        pthread_join(ತುಂಬು_thread, 0);
        val = ಬಫರ್;

        ಅಂಕೆ = (i+1)*N/splits - i*N/splits;
        ಅಕ್ಷರ **part = paths+(i*N/splits);
        ಆದರೆ(i != splits){
            ವಾದಗಳು.paths = part;
            ತುಂಬು_thread = ತುಂಬು_ದತ್ತಾಂಶ_ಒಳ_thread(ವಾದಗಳು);
        }
        ಬರೆ("Loaded: %d images in %lf seconds\n", val.X.ಅಡ್ಡಸಾಲುಗಳು, sec(clock()-ಸಮಯ));

        ಸಮಯ=clock();
        ಮಾತೃಕೆ pred = ಜಾಲ_predict_ದತ್ತಾಂಶ(net, val);
        ಪೂರ್ಣಾಂಕ j,k;
        ಗಾಗಿ(j = 0; j < val.y.ಅಡ್ಡಸಾಲುಗಳು; ++j){
            ಗಾಗಿ(k = 0; k < 20; ++k){
                ಆದರೆ(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++ಒಟ್ಟು;
                    ಆದರೆ((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_ಮಾತೃಕೆ(pred);
        ಬರೆ("%d: Acc: %f, %lf seconds, %d images\n", i, (ದಶಮಾಂಶ)correct/ಒಟ್ಟು, sec(clock()-ಸಮಯ), val.X.ಅಡ್ಡಸಾಲುಗಳು);
        free_ದತ್ತಾಂಶ(val);
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಜಾಲ net;
    ಅಕ್ಷರ *filename;
    ಪೂರ್ಣಾಂಕ class;
    ಪೂರ್ಣಾಂಕ classes;
    ದಶಮಾಂಶ elo;
    ದಶಮಾಂಶ *elos;
} sortable_bbox;

ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_compares = 0;
ಪೂರ್ಣಾಂಕ current_class = 0;

ಪೂರ್ಣಾಂಕ elo_comparator(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ಆದರೆ(box1.elos[current_class] == box2.elos[current_class]) ಹಿಂತಿರುಗಿ 0;
    ಆದರೆ(box1.elos[current_class] >  box2.elos[current_class]) ಹಿಂತಿರುಗಿ -1;
    ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ bbox_comparator(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ++ಒಟ್ಟು_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ಜಾಲ net = box1.net;
    ಪೂರ್ಣಾಂಕ class   = box1.class;

    image im1 = ತುಂಬು_image_color(box1.filename, net.w, net.h);
    image im2 = ತುಂಬು_image_color(box2.filename, net.w, net.h);
    ದಶಮಾಂಶ *X  = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net.w*net.h*net.c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಸ್ಮರಣೆ_ನಕಲು(X,                   im1.ದತ್ತಾಂಶ, im1.w*im1.h*im1.c*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಸ್ಮರಣೆ_ನಕಲು(X+im1.w*im1.h*im1.c, im2.ದತ್ತಾಂಶ, im2.w*im2.h*im2.c*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ದಶಮಾಂಶ *predictions = ಜಾಲ_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    ಬಿಡುಗಡೆ(X);
    ಆದರೆ (predictions[class*2] > predictions[class*2+1]){
        ಹಿಂತಿರುಗಿ 1;
    }
    ಹಿಂತಿರುಗಿ -1;
}

ಶೂನ್ಯ bbox_update(sortable_bbox *a, sortable_bbox *b, ಪೂರ್ಣಾಂಕ class, ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ)
{
    ಪೂರ್ಣಾಂಕ k = 32;
    ದಶಮಾಂಶ EA = 1./(1+ಘಾತ(10, (b->elos[class] - a->elos[class])/400.));
    ದಶಮಾಂಶ EB = 1./(1+ಘಾತ(10, (a->elos[class] - b->elos[class])/400.));
    ದಶಮಾಂಶ SA = ಫಲಿತಾಂಶ ? 1 : 0;
    ದಶಮಾಂಶ SB = ಫಲಿತಾಂಶ ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

ಶೂನ್ಯ bbox_fight(ಜಾಲ net, sortable_bbox *a, sortable_bbox *b, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ class)
{
    image im1 = ತುಂಬು_image_color(a->filename, net.w, net.h);
    image im2 = ತುಂಬು_image_color(b->filename, net.w, net.h);
    ದಶಮಾಂಶ *X  = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net.w*net.h*net.c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಸ್ಮರಣೆ_ನಕಲು(X,                   im1.ದತ್ತಾಂಶ, im1.w*im1.h*im1.c*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಸ್ಮರಣೆ_ನಕಲು(X+im1.w*im1.h*im1.c, im2.ದತ್ತಾಂಶ, im2.w*im2.h*im2.c*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ದಶಮಾಂಶ *predictions = ಜಾಲ_predict(net, X);
    ++ಒಟ್ಟು_compares;

    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < classes; ++i){
        ಆದರೆ(class < 0 || class == i){
            ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, ಫಲಿತಾಂಶ);
        }
    }
    
    free_image(im1);
    free_image(im2);
    ಬಿಡುಗಡೆ(X);
}

ಶೂನ್ಯ ವಿಂಗಡಿಸು_Master3000(ಅಕ್ಷರ *filename, ಅಕ್ಷರ *weightfile)
{
    ಪೂರ್ಣಾಂಕ i = 0;
    ಜಾಲ net = parse_ಜಾಲ_cfg(filename);
    ಆದರೆ(weightfile){
        ತುಂಬು_weights(&net, weightfile);
    }
    srand(ಸಮಯ(0));
    ಗಣ_batch_ಜಾಲ(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    ಅಕ್ಷರ **paths = (ಅಕ್ಷರ **)list_ಗಮ್ಯ_array(plist);
    ಪೂರ್ಣಾಂಕ N = plist->ಗಾತ್ರ_2;
    free_list(plist);
    sortable_bbox *boxes = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(N, ಗಾತ್ರ(sortable_bbox));
    ಬರೆ("Sorting %d boxes...\n", N);
    ಗಾಗಿ(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t ಸಮಯ=clock();
    qsort(boxes, N, ಗಾತ್ರ(sortable_bbox), bbox_comparator);
    ಗಾಗಿ(i = 0; i < N; ++i){
        ಬರೆ("%s\n", boxes[i].filename);
    }
    ಬರೆ("Sorted in %d compares, %f secs\n", ಒಟ್ಟು_compares, sec(clock()-ಸಮಯ));
}

ಶೂನ್ಯ BattleRoyaleWithCheese(ಅಕ್ಷರ *filename, ಅಕ್ಷರ *weightfile)
{
    ಪೂರ್ಣಾಂಕ classes = 20;
    ಪೂರ್ಣಾಂಕ i,j;
    ಜಾಲ net = parse_ಜಾಲ_cfg(filename);
    ಆದರೆ(weightfile){
        ತುಂಬು_weights(&net, weightfile);
    }
    srand(ಸಮಯ(0));
    ಗಣ_batch_ಜಾಲ(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    ಅಕ್ಷರ **paths = (ಅಕ್ಷರ **)list_ಗಮ್ಯ_array(plist);
    ಪೂರ್ಣಾಂಕ N = plist->ಗಾತ್ರ_2;
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = N;
    free_list(plist);
    sortable_bbox *boxes = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(N, ಗಾತ್ರ(sortable_bbox));
    ಬರೆ("Battling %d boxes...\n", N);
    ಗಾಗಿ(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(classes, ಗಾತ್ರ(ದಶಮಾಂಶ));;
        ಗಾಗಿ(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    ಪೂರ್ಣಾಂಕ ಸುತ್ತು;
    clock_t ಸಮಯ=clock();
    ಗಾಗಿ(ಸುತ್ತು = 1; ಸುತ್ತು <= 4; ++ಸುತ್ತು){
        clock_t ಸುತ್ತು_ಸಮಯ=clock();
        ಬರೆ("Round: %d\n", ಸುತ್ತು);
        shuffle(boxes, N, ಗಾತ್ರ(sortable_bbox));
        ಗಾಗಿ(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        ಬರೆ("Round: %f secs, %d remaining\n", sec(clock()-ಸುತ್ತು_ಸಮಯ), N);
    }

    ಪೂರ್ಣಾಂಕ class;

    ಗಾಗಿ (class = 0; class < classes; ++class){

        N = ಒಟ್ಟು;
        current_class = class;
        qsort(boxes, N, ಗಾತ್ರ(sortable_bbox), elo_comparator);
        N /= 2;

        ಗಾಗಿ(ಸುತ್ತು = 1; ಸುತ್ತು <= 100; ++ಸುತ್ತು){
            clock_t ಸುತ್ತು_ಸಮಯ=clock();
            ಬರೆ("Round: %d\n", ಸುತ್ತು);

            sorta_shuffle(boxes, N, ಗಾತ್ರ(sortable_bbox), 10);
            ಗಾಗಿ(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, ಗಾತ್ರ(sortable_bbox), elo_comparator);
            ಆದರೆ(ಸುತ್ತು <= 20) N = (N*9/10)/2*2;

            ಬರೆ("Round: %f secs, %d remaining\n", sec(clock()-ಸುತ್ತು_ಸಮಯ), N);
        }
        ಅಕ್ಷರ buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        ಗಾಗಿ(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    ಬರೆ("Tournament in %d compares, %f secs\n", ಒಟ್ಟು_compares, sec(clock()-ಸಮಯ));
}

ಶೂನ್ಯ ಸತತ_compare(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
    ಆದರೆ(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        ಹಿಂತಿರುಗಿ;
    }

    ಅಕ್ಷರ *cfg = argv[3];
    ಅಕ್ಷರ *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    ಆದರೆ(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(0==strcmp(argv[2], "sort")) ವಿಂಗಡಿಸು_Master3000(cfg, weights);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}
