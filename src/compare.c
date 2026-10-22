#شامل <stdio.h>

#شامل "network.h"
#شامل "detection_layer.h"
#شامل "cost_layer.h"
#شامل "utils.h"
#شامل "parser.h"
#شامل "box.h"

خالی train_compare(حرف *cfgfile, حرف *weightfile)
{
    srand(وقت(0));
    اعشاری avg_loss = -1;
    حرف *بنیاد = basecfg(cfgfile);
    حرف *backup_directory = "/home/pjreddie/backup/";
    لکھو("%s\n", بنیاد);
    جال net = parse_جال_cfg(cfgfile);
    اگر(weightfile){
        بھرو_weights(&net, weightfile);
    }
    لکھو("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_شرح, net.momentum, net.decay);
    صحیح imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحیح N = plist->سائز;
    لکھو("%d\n", N);
    clock_t وقت;
    pthread_t بھرو_thread;
    ڈیٹا train;
    ڈیٹا بفر;

    بھرو_دلائل دلائل = {0};
    دلائل.w = net.w;
    دلائل.h = net.h;
    دلائل.paths = paths;
    دلائل.classes = 20;
    دلائل.n = imgs;
    دلائل.m = N;
    دلائل.d = &بفر;
    دلائل.type = COMPARE_ڈیٹا;

    بھرو_thread = بھرو_ڈیٹا_اندر_thread(دلائل);
    صحیح epoch = *net.نظر_آیا/N;
    صحیح i = 0;
    جب(1){
        ++i;
        وقت=clock();
        pthread_join(بھرو_thread, 0);
        train = بفر;

        بھرو_thread = بھرو_ڈیٹا_اندر_thread(دلائل);
        لکھو("Loaded: %lf seconds\n", sec(clock()-وقت));
        وقت=clock();
        اعشاری loss = train_جال(net, train);
        اگر(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        لکھو("%.3f: %f, %f avg, %lf seconds, %ld images\n", (اعشاری)*net.نظر_آیا/N, loss, avg_loss, sec(clock()-وقت), *net.نظر_آیا);
        free_ڈیٹا(train);
        اگر(i%100 == 0){
            حرف buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,بنیاد, epoch, i);
            save_weights(net, buff);
        }
        اگر(*net.نظر_آیا/N > epoch){
            epoch = *net.نظر_آیا/N;
            i = 0;
            حرف buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,بنیاد, epoch);
            save_weights(net, buff);
            اگر(epoch%22 == 0) net.learning_شرح *= .1;
        }
    }
    pthread_join(بھرو_thread, 0);
    free_ڈیٹا(بفر);
    free_جال(net);
    free_ptrs((خالی**)paths, plist->سائز);
    free_list(plist);
    آزاد(بنیاد);
}

خالی validate_compare(حرف *filename, حرف *weightfile)
{
    صحیح i = 0;
    جال net = parse_جال_cfg(filename);
    اگر(weightfile){
        بھرو_weights(&net, weightfile);
    }
    srand(وقت(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحیح N = plist->سائز/2;
    free_list(plist);

    clock_t وقت;
    صحیح correct = 0;
    صحیح کل = 0;
    صحیح splits = 10;
    صحیح عدد = (i+1)*N/splits - i*N/splits;

    ڈیٹا val, بفر;

    بھرو_دلائل دلائل = {0};
    دلائل.w = net.w;
    دلائل.h = net.h;
    دلائل.paths = paths;
    دلائل.classes = 20;
    دلائل.n = عدد;
    دلائل.m = 0;
    دلائل.d = &بفر;
    دلائل.type = COMPARE_ڈیٹا;

    pthread_t بھرو_thread = بھرو_ڈیٹا_اندر_thread(دلائل);
    برائے(i = 1; i <= splits; ++i){
        وقت=clock();

        pthread_join(بھرو_thread, 0);
        val = بفر;

        عدد = (i+1)*N/splits - i*N/splits;
        حرف **part = paths+(i*N/splits);
        اگر(i != splits){
            دلائل.paths = part;
            بھرو_thread = بھرو_ڈیٹا_اندر_thread(دلائل);
        }
        لکھو("Loaded: %d images in %lf seconds\n", val.X.صفیں, sec(clock()-وقت));

        وقت=clock();
        میٹرکس pred = جال_predict_ڈیٹا(net, val);
        صحیح j,k;
        برائے(j = 0; j < val.y.صفیں; ++j){
            برائے(k = 0; k < 20; ++k){
                اگر(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++کل;
                    اگر((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_میٹرکس(pred);
        لکھو("%d: Acc: %f, %lf seconds, %d images\n", i, (اعشاری)correct/کل, sec(clock()-وقت), val.X.صفیں);
        free_ڈیٹا(val);
    }
}

قسمتعریف ساخت {
    جال net;
    حرف *filename;
    صحیح class;
    صحیح classes;
    اعشاری elo;
    اعشاری *elos;
} sortable_bbox;

صحیح کل_compares = 0;
صحیح current_class = 0;

صحیح elo_comparator(ثابت خالی*a, ثابت خالی *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    اگر(box1.elos[current_class] == box2.elos[current_class]) واپس 0;
    اگر(box1.elos[current_class] >  box2.elos[current_class]) واپس -1;
    واپس 1;
}

صحیح bbox_comparator(ثابت خالی *a, ثابت خالی *b)
{
    ++کل_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    جال net = box1.net;
    صحیح class   = box1.class;

    image im1 = بھرو_image_color(box1.filename, net.w, net.h);
    image im2 = بھرو_image_color(box2.filename, net.w, net.h);
    اعشاری *X  = صفر_حافظہ_دو(net.w*net.h*net.c, حجم(اعشاری));
    حافظہ_نقل(X,                   im1.ڈیٹا, im1.w*im1.h*im1.c*حجم(اعشاری));
    حافظہ_نقل(X+im1.w*im1.h*im1.c, im2.ڈیٹا, im2.w*im2.h*im2.c*حجم(اعشاری));
    اعشاری *predictions = جال_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    آزاد(X);
    اگر (predictions[class*2] > predictions[class*2+1]){
        واپس 1;
    }
    واپس -1;
}

خالی bbox_update(sortable_bbox *a, sortable_bbox *b, صحیح class, صحیح نتیجہ)
{
    صحیح k = 32;
    اعشاری EA = 1./(1+قوت(10, (b->elos[class] - a->elos[class])/400.));
    اعشاری EB = 1./(1+قوت(10, (a->elos[class] - b->elos[class])/400.));
    اعشاری SA = نتیجہ ? 1 : 0;
    اعشاری SB = نتیجہ ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

خالی bbox_fight(جال net, sortable_bbox *a, sortable_bbox *b, صحیح classes, صحیح class)
{
    image im1 = بھرو_image_color(a->filename, net.w, net.h);
    image im2 = بھرو_image_color(b->filename, net.w, net.h);
    اعشاری *X  = صفر_حافظہ_دو(net.w*net.h*net.c, حجم(اعشاری));
    حافظہ_نقل(X,                   im1.ڈیٹا, im1.w*im1.h*im1.c*حجم(اعشاری));
    حافظہ_نقل(X+im1.w*im1.h*im1.c, im2.ڈیٹا, im2.w*im2.h*im2.c*حجم(اعشاری));
    اعشاری *predictions = جال_predict(net, X);
    ++کل_compares;

    صحیح i;
    برائے(i = 0; i < classes; ++i){
        اگر(class < 0 || class == i){
            صحیح نتیجہ = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, نتیجہ);
        }
    }
    
    free_image(im1);
    free_image(im2);
    آزاد(X);
}

خالی ترتیب_دو_Master3000(حرف *filename, حرف *weightfile)
{
    صحیح i = 0;
    جال net = parse_جال_cfg(filename);
    اگر(weightfile){
        بھرو_weights(&net, weightfile);
    }
    srand(وقت(0));
    سیٹ_batch_جال(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحیح N = plist->سائز;
    free_list(plist);
    sortable_bbox *boxes = صفر_حافظہ_دو(N, حجم(sortable_bbox));
    لکھو("Sorting %d boxes...\n", N);
    برائے(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t وقت=clock();
    qsort(boxes, N, حجم(sortable_bbox), bbox_comparator);
    برائے(i = 0; i < N; ++i){
        لکھو("%s\n", boxes[i].filename);
    }
    لکھو("Sorted in %d compares, %f secs\n", کل_compares, sec(clock()-وقت));
}

خالی BattleRoyaleWithCheese(حرف *filename, حرف *weightfile)
{
    صحیح classes = 20;
    صحیح i,j;
    جال net = parse_جال_cfg(filename);
    اگر(weightfile){
        بھرو_weights(&net, weightfile);
    }
    srand(وقت(0));
    سیٹ_batch_جال(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحیح N = plist->سائز;
    صحیح کل = N;
    free_list(plist);
    sortable_bbox *boxes = صفر_حافظہ_دو(N, حجم(sortable_bbox));
    لکھو("Battling %d boxes...\n", N);
    برائے(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = صفر_حافظہ_دو(classes, حجم(اعشاری));;
        برائے(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    صحیح باری;
    clock_t وقت=clock();
    برائے(باری = 1; باری <= 4; ++باری){
        clock_t باری_وقت=clock();
        لکھو("Round: %d\n", باری);
        shuffle(boxes, N, حجم(sortable_bbox));
        برائے(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        لکھو("Round: %f secs, %d remaining\n", sec(clock()-باری_وقت), N);
    }

    صحیح class;

    برائے (class = 0; class < classes; ++class){

        N = کل;
        current_class = class;
        qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
        N /= 2;

        برائے(باری = 1; باری <= 100; ++باری){
            clock_t باری_وقت=clock();
            لکھو("Round: %d\n", باری);

            sorta_shuffle(boxes, N, حجم(sortable_bbox), 10);
            برائے(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
            اگر(باری <= 20) N = (N*9/10)/2*2;

            لکھو("Round: %f secs, %d remaining\n", sec(clock()-باری_وقت), N);
        }
        حرف buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        برائے(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    لکھو("Tournament in %d compares, %f secs\n", کل_compares, sec(clock()-وقت));
}

خالی تسلسل_compare(صحیح argc, حرف **argv)
{
    اگر(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        واپس;
    }

    حرف *cfg = argv[3];
    حرف *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    اگر(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    ورنہ اگر(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    ورنہ اگر(0==strcmp(argv[2], "sort")) ترتیب_دو_Master3000(cfg, weights);
    ورنہ اگر(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}
