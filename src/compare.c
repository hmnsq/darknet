#అన్తర్భావ <stdio.h>

#అన్తర్భావ "network.h"
#అన్తర్భావ "detection_layer.h"
#అన్తర్భావ "cost_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "parser.h"
#అన్తర్భావ "box.h"

శూన్య train_compare(అక్షరం *cfgfile, అక్షరం *weightfile)
{
    srand(సమయం(0));
    దశాంశం avg_loss = -1;
    అక్షరం *పునాది = basecfg(cfgfile);
    అక్షరం *backup_directory = "/home/pjreddie/backup/";
    రాయి("%s\n", పునాది);
    జాలం net = parse_జాలం_cfg(cfgfile);
    ఒకవేళ(weightfile){
        ఎక్కించు_weights(&net, weightfile);
    }
    రాయి("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_రేటు, net.momentum, net.decay);
    పూర్ణాంకం imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    అక్షరం **paths = (అక్షరం **)list_గమ్యం_array(plist);
    పూర్ణాంకం N = plist->కొలత;
    రాయి("%d\n", N);
    clock_t సమయం;
    pthread_t ఎక్కించు_thread;
    దత్తాంశం train;
    దత్తాంశం బఫరు;

    ఎక్కించు_వాదనలు వాదనలు = {0};
    వాదనలు.w = net.w;
    వాదనలు.h = net.h;
    వాదనలు.paths = paths;
    వాదనలు.classes = 20;
    వాదనలు.n = imgs;
    వాదనలు.m = N;
    వాదనలు.d = &బఫరు;
    వాదనలు.type = COMPARE_దత్తాంశం;

    ఎక్కించు_thread = ఎక్కించు_దత్తాంశం_లోపల_thread(వాదనలు);
    పూర్ణాంకం epoch = *net.చూసిన/N;
    పూర్ణాంకం i = 0;
    అయితే(1){
        ++i;
        సమయం=clock();
        pthread_join(ఎక్కించు_thread, 0);
        train = బఫరు;

        ఎక్కించు_thread = ఎక్కించు_దత్తాంశం_లోపల_thread(వాదనలు);
        రాయి("Loaded: %lf seconds\n", sec(clock()-సమయం));
        సమయం=clock();
        దశాంశం loss = train_జాలం(net, train);
        ఒకవేళ(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        రాయి("%.3f: %f, %f avg, %lf seconds, %ld images\n", (దశాంశం)*net.చూసిన/N, loss, avg_loss, sec(clock()-సమయం), *net.చూసిన);
        free_దత్తాంశం(train);
        ఒకవేళ(i%100 == 0){
            అక్షరం buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,పునాది, epoch, i);
            save_weights(net, buff);
        }
        ఒకవేళ(*net.చూసిన/N > epoch){
            epoch = *net.చూసిన/N;
            i = 0;
            అక్షరం buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,పునాది, epoch);
            save_weights(net, buff);
            ఒకవేళ(epoch%22 == 0) net.learning_రేటు *= .1;
        }
    }
    pthread_join(ఎక్కించు_thread, 0);
    free_దత్తాంశం(బఫరు);
    free_జాలం(net);
    free_ptrs((శూన్య**)paths, plist->కొలత);
    free_list(plist);
    విడుదల(పునాది);
}

శూన్య validate_compare(అక్షరం *filename, అక్షరం *weightfile)
{
    పూర్ణాంకం i = 0;
    జాలం net = parse_జాలం_cfg(filename);
    ఒకవేళ(weightfile){
        ఎక్కించు_weights(&net, weightfile);
    }
    srand(సమయం(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    అక్షరం **paths = (అక్షరం **)list_గమ్యం_array(plist);
    పూర్ణాంకం N = plist->కొలత/2;
    free_list(plist);

    clock_t సమయం;
    పూర్ణాంకం correct = 0;
    పూర్ణాంకం మొత్తం = 0;
    పూర్ణాంకం splits = 10;
    పూర్ణాంకం అంకె = (i+1)*N/splits - i*N/splits;

    దత్తాంశం val, బఫరు;

    ఎక్కించు_వాదనలు వాదనలు = {0};
    వాదనలు.w = net.w;
    వాదనలు.h = net.h;
    వాదనలు.paths = paths;
    వాదనలు.classes = 20;
    వాదనలు.n = అంకె;
    వాదనలు.m = 0;
    వాదనలు.d = &బఫరు;
    వాదనలు.type = COMPARE_దత్తాంశం;

    pthread_t ఎక్కించు_thread = ఎక్కించు_దత్తాంశం_లోపల_thread(వాదనలు);
    కొరకు(i = 1; i <= splits; ++i){
        సమయం=clock();

        pthread_join(ఎక్కించు_thread, 0);
        val = బఫరు;

        అంకె = (i+1)*N/splits - i*N/splits;
        అక్షరం **part = paths+(i*N/splits);
        ఒకవేళ(i != splits){
            వాదనలు.paths = part;
            ఎక్కించు_thread = ఎక్కించు_దత్తాంశం_లోపల_thread(వాదనలు);
        }
        రాయి("Loaded: %d images in %lf seconds\n", val.X.అడ్డువరుసలు, sec(clock()-సమయం));

        సమయం=clock();
        మాతృక pred = జాలం_predict_దత్తాంశం(net, val);
        పూర్ణాంకం j,k;
        కొరకు(j = 0; j < val.y.అడ్డువరుసలు; ++j){
            కొరకు(k = 0; k < 20; ++k){
                ఒకవేళ(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++మొత్తం;
                    ఒకవేళ((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_మాతృక(pred);
        రాయి("%d: Acc: %f, %lf seconds, %d images\n", i, (దశాంశం)correct/మొత్తం, sec(clock()-సమయం), val.X.అడ్డువరుసలు);
        free_దత్తాంశం(val);
    }
}

రకనిర్వచనం నిర్మాణం {
    జాలం net;
    అక్షరం *filename;
    పూర్ణాంకం class;
    పూర్ణాంకం classes;
    దశాంశం elo;
    దశాంశం *elos;
} sortable_bbox;

పూర్ణాంకం మొత్తం_compares = 0;
పూర్ణాంకం current_class = 0;

పూర్ణాంకం elo_comparator(స్థిరరాశి శూన్య*a, స్థిరరాశి శూన్య *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ఒకవేళ(box1.elos[current_class] == box2.elos[current_class]) తిరిగి 0;
    ఒకవేళ(box1.elos[current_class] >  box2.elos[current_class]) తిరిగి -1;
    తిరిగి 1;
}

పూర్ణాంకం bbox_comparator(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    ++మొత్తం_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    జాలం net = box1.net;
    పూర్ణాంకం class   = box1.class;

    image im1 = ఎక్కించు_image_color(box1.filename, net.w, net.h);
    image im2 = ఎక్కించు_image_color(box2.filename, net.w, net.h);
    దశాంశం *X  = సున్నా_జ్ఞాపకం_ఇవ్వు(net.w*net.h*net.c, పరిమాణం(దశాంశం));
    జ్ఞాపకం_నకలు(X,                   im1.దత్తాంశం, im1.w*im1.h*im1.c*పరిమాణం(దశాంశం));
    జ్ఞాపకం_నకలు(X+im1.w*im1.h*im1.c, im2.దత్తాంశం, im2.w*im2.h*im2.c*పరిమాణం(దశాంశం));
    దశాంశం *predictions = జాలం_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    విడుదల(X);
    ఒకవేళ (predictions[class*2] > predictions[class*2+1]){
        తిరిగి 1;
    }
    తిరిగి -1;
}

శూన్య bbox_update(sortable_bbox *a, sortable_bbox *b, పూర్ణాంకం class, పూర్ణాంకం ఫలితం)
{
    పూర్ణాంకం k = 32;
    దశాంశం EA = 1./(1+ఘాతం(10, (b->elos[class] - a->elos[class])/400.));
    దశాంశం EB = 1./(1+ఘాతం(10, (a->elos[class] - b->elos[class])/400.));
    దశాంశం SA = ఫలితం ? 1 : 0;
    దశాంశం SB = ఫలితం ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

శూన్య bbox_fight(జాలం net, sortable_bbox *a, sortable_bbox *b, పూర్ణాంకం classes, పూర్ణాంకం class)
{
    image im1 = ఎక్కించు_image_color(a->filename, net.w, net.h);
    image im2 = ఎక్కించు_image_color(b->filename, net.w, net.h);
    దశాంశం *X  = సున్నా_జ్ఞాపకం_ఇవ్వు(net.w*net.h*net.c, పరిమాణం(దశాంశం));
    జ్ఞాపకం_నకలు(X,                   im1.దత్తాంశం, im1.w*im1.h*im1.c*పరిమాణం(దశాంశం));
    జ్ఞాపకం_నకలు(X+im1.w*im1.h*im1.c, im2.దత్తాంశం, im2.w*im2.h*im2.c*పరిమాణం(దశాంశం));
    దశాంశం *predictions = జాలం_predict(net, X);
    ++మొత్తం_compares;

    పూర్ణాంకం i;
    కొరకు(i = 0; i < classes; ++i){
        ఒకవేళ(class < 0 || class == i){
            పూర్ణాంకం ఫలితం = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, ఫలితం);
        }
    }
    
    free_image(im1);
    free_image(im2);
    విడుదల(X);
}

శూన్య క్రమీకరించు_Master3000(అక్షరం *filename, అక్షరం *weightfile)
{
    పూర్ణాంకం i = 0;
    జాలం net = parse_జాలం_cfg(filename);
    ఒకవేళ(weightfile){
        ఎక్కించు_weights(&net, weightfile);
    }
    srand(సమయం(0));
    సమితి_batch_జాలం(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    అక్షరం **paths = (అక్షరం **)list_గమ్యం_array(plist);
    పూర్ణాంకం N = plist->కొలత;
    free_list(plist);
    sortable_bbox *boxes = సున్నా_జ్ఞాపకం_ఇవ్వు(N, పరిమాణం(sortable_bbox));
    రాయి("Sorting %d boxes...\n", N);
    కొరకు(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t సమయం=clock();
    qsort(boxes, N, పరిమాణం(sortable_bbox), bbox_comparator);
    కొరకు(i = 0; i < N; ++i){
        రాయి("%s\n", boxes[i].filename);
    }
    రాయి("Sorted in %d compares, %f secs\n", మొత్తం_compares, sec(clock()-సమయం));
}

శూన్య BattleRoyaleWithCheese(అక్షరం *filename, అక్షరం *weightfile)
{
    పూర్ణాంకం classes = 20;
    పూర్ణాంకం i,j;
    జాలం net = parse_జాలం_cfg(filename);
    ఒకవేళ(weightfile){
        ఎక్కించు_weights(&net, weightfile);
    }
    srand(సమయం(0));
    సమితి_batch_జాలం(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    అక్షరం **paths = (అక్షరం **)list_గమ్యం_array(plist);
    పూర్ణాంకం N = plist->కొలత;
    పూర్ణాంకం మొత్తం = N;
    free_list(plist);
    sortable_bbox *boxes = సున్నా_జ్ఞాపకం_ఇవ్వు(N, పరిమాణం(sortable_bbox));
    రాయి("Battling %d boxes...\n", N);
    కొరకు(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = సున్నా_జ్ఞాపకం_ఇవ్వు(classes, పరిమాణం(దశాంశం));;
        కొరకు(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    పూర్ణాంకం విడత;
    clock_t సమయం=clock();
    కొరకు(విడత = 1; విడత <= 4; ++విడత){
        clock_t విడత_సమయం=clock();
        రాయి("Round: %d\n", విడత);
        shuffle(boxes, N, పరిమాణం(sortable_bbox));
        కొరకు(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        రాయి("Round: %f secs, %d remaining\n", sec(clock()-విడత_సమయం), N);
    }

    పూర్ణాంకం class;

    కొరకు (class = 0; class < classes; ++class){

        N = మొత్తం;
        current_class = class;
        qsort(boxes, N, పరిమాణం(sortable_bbox), elo_comparator);
        N /= 2;

        కొరకు(విడత = 1; విడత <= 100; ++విడత){
            clock_t విడత_సమయం=clock();
            రాయి("Round: %d\n", విడత);

            sorta_shuffle(boxes, N, పరిమాణం(sortable_bbox), 10);
            కొరకు(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, పరిమాణం(sortable_bbox), elo_comparator);
            ఒకవేళ(విడత <= 20) N = (N*9/10)/2*2;

            రాయి("Round: %f secs, %d remaining\n", sec(clock()-విడత_సమయం), N);
        }
        అక్షరం buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        కొరకు(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    రాయి("Tournament in %d compares, %f secs\n", మొత్తం_compares, sec(clock()-సమయం));
}

శూన్య నిరంతరం_compare(పూర్ణాంకం argc, అక్షరం **argv)
{
    ఒకవేళ(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        తిరిగి;
    }

    అక్షరం *cfg = argv[3];
    అక్షరం *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    ఒకవేళ(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    లేకపోతే ఒకవేళ(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    లేకపోతే ఒకవేళ(0==strcmp(argv[2], "sort")) క్రమీకరించు_Master3000(cfg, weights);
    లేకపోతే ఒకవేళ(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}
