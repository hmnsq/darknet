#అన్తర్భావ "network.h"
#అన్తర్భావ "detection_layer.h"
#అన్తర్భావ "region_layer.h"
#అన్తర్భావ "cost_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "parser.h"
#అన్తర్భావ "box.h"
#అన్తర్భావ "image.h"
#అన్తర్భావ "demo.h"
#అన్తర్భావ <sys/time.h>

#నిర్వచించు DEMO 1

#నిర్వచితమైతే OPENCV

స్థిర అక్షరం **demo_పేర్లు;
స్థిర image **demo_alphabet;
స్థిర పూర్ణాంకం demo_classes;

స్థిర జాలం *net;
స్థిర image buff [3];
స్థిర image buff_letter[3];
స్థిర పూర్ణాంకం buff_సూచీ = 0;
స్థిర శూన్య * cap;
స్థిర దశాంశం fps = 0;
స్థిర దశాంశం demo_thresh = 0;
స్థిర దశాంశం demo_hier = .5;
స్థిర పూర్ణాంకం running = 0;

స్థిర పూర్ణాంకం demo_చట్రం = 3;
స్థిర పూర్ణాంకం demo_సూచీ = 0;
స్థిర దశాంశం **predictions;
స్థిర దశాంశం *avg;
స్థిర పూర్ణాంకం demo_పూర్తి = 0;
స్థిర పూర్ణాంకం demo_మొత్తం = 0;
ద్విగుణ demo_సమయం;

detection *get_జాలం_boxes(జాలం *net, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం thresh, దశాంశం hier, పూర్ణాంకం *map, పూర్ణాంకం relative, పూర్ణాంకం *అంకె);

పూర్ణాంకం కొలత_జాలం(జాలం *net)
{
    పూర్ణాంకం i;
    పూర్ణాంకం లెక్క = 0;
    కొరకు(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ఒకవేళ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            లెక్క += l.outputs;
        }
    }
    తిరిగి లెక్క;
}

శూన్య remember_జాలం(జాలం *net)
{
    పూర్ణాంకం i;
    పూర్ణాంకం లెక్క = 0;
    కొరకు(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ఒకవేళ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            జ్ఞాపకం_నకలు(predictions[demo_సూచీ] + లెక్క, net->layers[i].output, పరిమాణం(దశాంశం) * l.outputs);
            లెక్క += l.outputs;
        }
    }
}

detection *avg_predictions(జాలం *net, పూర్ణాంకం *nboxes)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం లెక్క = 0;
    fill_cpu(demo_మొత్తం, 0, avg, 1);
    కొరకు(j = 0; j < demo_చట్రం; ++j){
        axpy_cpu(demo_మొత్తం, 1./demo_చట్రం, predictions[j], 1, avg, 1);
    }
    కొరకు(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ఒకవేళ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            జ్ఞాపకం_నకలు(l.output, avg + లెక్క, పరిమాణం(దశాంశం) * l.outputs);
            లెక్క += l.outputs;
        }
    }
    detection *dets = get_జాలం_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    తిరిగి dets;
}

శూన్య *detect_లోపల_thread(శూన్య *ptr)
{
    running = 1;
    దశాంశం nms = .4;

    layer l = net->layers[net->n-1];
    దశాంశం *X = buff_letter[(buff_సూచీ+2)%3].దత్తాంశం;
    జాలం_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_జాలం(net);
    detection *dets = 0;
    పూర్ణాంకం nboxes = 0;
    dets = avg_predictions(net, &nboxes);


    /*
       int i,j;
       box zero = {0};
       int classes = l.classes;
       for(i = 0; i < demo_detections; ++i){
       avg[i].objectness = 0;
       avg[i].bbox = zero;
       memset(avg[i].prob, 0, classes*sizeof(float));
       for(j = 0; j < demo_frame; ++j){
       axpy_cpu(classes, 1./demo_frame, dets[j][i].prob, 1, avg[i].prob, 1);
       avg[i].objectness += dets[j][i].objectness * 1./demo_frame;
       avg[i].bbox.x += dets[j][i].bbox.x * 1./demo_frame;
       avg[i].bbox.y += dets[j][i].bbox.y * 1./demo_frame;
       avg[i].bbox.w += dets[j][i].bbox.w * 1./demo_frame;
       avg[i].bbox.h += dets[j][i].bbox.h * 1./demo_frame;
       }
    //copy_cpu(classes, dets[0][i].prob, 1, avg[i].prob, 1);
    //avg[i].objectness = dets[0][i].objectness;
    }
     */

    ఒకవేళ (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    రాయి("\033[2J");
    రాయి("\033[1;1H");
    రాయి("\nFPS:%.1f\n",fps);
    రాయి("Objects:\n\n");
    image display = buff[(buff_సూచీ+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_పేర్లు, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_సూచీ = (demo_సూచీ + 1)%demo_చట్రం;
    running = 0;
    తిరిగి 0;
}

శూన్య *fetch_లోపల_thread(శూన్య *ptr)
{
    free_image(buff[buff_సూచీ]);
    buff[buff_సూచీ] = get_image_from_ప్రవాహం(cap);
    ఒకవేళ(buff[buff_సూచీ].దత్తాంశం == 0) {
        demo_పూర్తి = 1;
        తిరిగి 0;
    }
    letterbox_image_into(buff[buff_సూచీ], net->w, net->h, buff_letter[buff_సూచీ]);
    తిరిగి 0;
}

శూన్య *display_లోపల_thread(శూన్య *ptr)
{
    పూర్ణాంకం c = show_image(buff[(buff_సూచీ + 1)%3], "Demo", 1);
    ఒకవేళ (c != -1) c = c%256;
    ఒకవేళ (c == 27) {
        demo_పూర్తి = 1;
        తిరిగి 0;
    } లేకపోతే ఒకవేళ (c == 82) {
        demo_thresh += .02;
    } లేకపోతే ఒకవేళ (c == 84) {
        demo_thresh -= .02;
        ఒకవేళ(demo_thresh <= .02) demo_thresh = .02;
    } లేకపోతే ఒకవేళ (c == 83) {
        demo_hier += .02;
    } లేకపోతే ఒకవేళ (c == 81) {
        demo_hier -= .02;
        ఒకవేళ(demo_hier <= .0) demo_hier = .0;
    }
    తిరిగి 0;
}

శూన్య *display_loop(శూన్య *ptr)
{
    అయితే(1){
        display_లోపల_thread(0);
    }
}

శూన్య *detect_loop(శూన్య *ptr)
{
    అయితే(1){
        detect_లోపల_thread(0);
    }
}

శూన్య demo(అక్షరం *cfgfile, అక్షరం *weightfile, దశాంశం thresh, పూర్ణాంకం cam_సూచీ, స్థిరరాశి అక్షరం *filename, అక్షరం **పేర్లు, పూర్ణాంకం classes, పూర్ణాంకం delay, అక్షరం *ఉపసర్గ, పూర్ణాంకం avg_చట్రాలు, దశాంశం hier, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం చట్రాలు, పూర్ణాంకం fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = ఎక్కించు_alphabet();
    demo_పేర్లు = పేర్లు;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    రాయి("Demo\n");
    net = ఎక్కించు_జాలం(cfgfile, weightfile, 0);
    సమితి_batch_జాలం(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    పూర్ణాంకం i;
    demo_మొత్తం = కొలత_జాలం(net);
    predictions = సున్నా_జ్ఞాపకం_ఇవ్వు(demo_చట్రం, పరిమాణం(దశాంశం*));
    కొరకు (i = 0; i < demo_చట్రం; ++i){
        predictions[i] = సున్నా_జ్ఞాపకం_ఇవ్వు(demo_మొత్తం, పరిమాణం(దశాంశం));
    }
    avg = సున్నా_జ్ఞాపకం_ఇవ్వు(demo_మొత్తం, పరిమాణం(దశాంశం));

    ఒకవేళ(filename){
        రాయి("video file: %s\n", filename);
        cap = open_video_ప్రవాహం(filename, 0, 0, 0, 0);
    }లేకపోతే{
        cap = open_video_ప్రవాహం(0, cam_సూచీ, w, h, చట్రాలు);
    }

    ఒకవేళ(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_ప్రవాహం(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    పూర్ణాంకం లెక్క = 0;
    ఒకవేళ(!ఉపసర్గ){
        make_కిటికీ("Demo", 1352, 1013, fullscreen);
    }

    demo_సమయం = what_సమయం_is_it_now();

    అయితే(!demo_పూర్తి){
        buff_సూచీ = (buff_సూచీ + 1) %3;
        ఒకవేళ(pthread_create(&fetch_thread, 0, fetch_లోపల_thread, 0)) error("Thread creation failed");
        ఒకవేళ(pthread_create(&detect_thread, 0, detect_లోపల_thread, 0)) error("Thread creation failed");
        ఒకవేళ(!ఉపసర్గ){
            fps = 1./(what_సమయం_is_it_now() - demo_సమయం);
            demo_సమయం = what_సమయం_is_it_now();
            display_లోపల_thread(0);
        }లేకపోతే{
            అక్షరం name[256];
            sprintf(name, "%s_%08d", ఉపసర్గ, లెక్క);
            save_image(buff[(buff_సూచీ + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++లెక్క;
    }
}

/*
   void demo_compare(char *cfg1, char *weight1, char *cfg2, char *weight2, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg_frames, float hier, int w, int h, int frames, int fullscreen)
   {
   demo_frame = avg_frames;
   predictions = calloc(demo_frame, sizeof(float*));
   image **alphabet = load_alphabet();
   demo_names = names;
   demo_alphabet = alphabet;
   demo_classes = classes;
   demo_thresh = thresh;
   demo_hier = hier;
   printf("Demo\n");
   net = load_network(cfg1, weight1, 0);
   set_batch_network(net, 1);
   pthread_t detect_thread;
   pthread_t fetch_thread;

   srand(2222222);

   if(filename){
   printf("video file: %s\n", filename);
   cap = cvCaptureFromFile(filename);
   }else{
   cap = cvCaptureFromCAM(cam_index);

   if(w){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH, w);
   }
   if(h){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT, h);
   }
   if(frames){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FPS, frames);
   }
   }

   if(!cap) error("Couldn't connect to webcam.\n");

   layer l = net->layers[net->n-1];
   demo_detections = l.n*l.w*l.h;
   int j;

   avg = (float *) calloc(l.outputs, sizeof(float));
   for(j = 0; j < demo_frame; ++j) predictions[j] = (float *) calloc(l.outputs, sizeof(float));

   boxes = (box *)calloc(l.w*l.h*l.n, sizeof(box));
   probs = (float **)calloc(l.w*l.h*l.n, sizeof(float *));
   for(j = 0; j < l.w*l.h*l.n; ++j) probs[j] = (float *)calloc(l.classes+1, sizeof(float));

   buff[0] = get_image_from_stream(cap);
   buff[1] = copy_image(buff[0]);
   buff[2] = copy_image(buff[0]);
   buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
   buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
   buff_letter[2] = letterbox_image(buff[0], net->w, net->h);
   ipl = cvCreateImage(cvSize(buff[0].w,buff[0].h), IPL_DEPTH_8U, buff[0].c);

   int count = 0;
   if(!prefix){
   cvNamedWindow("Demo", CV_WINDOW_NORMAL); 
   if(fullscreen){
   cvSetWindowProperty("Demo", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
   } else {
   cvMoveWindow("Demo", 0, 0);
   cvResizeWindow("Demo", 1352, 1013);
   }
   }

   demo_time = what_time_is_it_now();

   while(!demo_done){
buff_index = (buff_index + 1) %3;
if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
if(!prefix){
    fps = 1./(what_time_is_it_now() - demo_time);
    demo_time = what_time_is_it_now();
    display_in_thread(0);
}else{
    char name[256];
    sprintf(name, "%s_%08d", prefix, count);
    save_image(buff[(buff_index + 1)%3], name);
}
pthread_join(fetch_thread, 0);
pthread_join(detect_thread, 0);
++count;
}
}
*/
#లేకపోతే
శూన్య demo(అక్షరం *cfgfile, అక్షరం *weightfile, దశాంశం thresh, పూర్ణాంకం cam_సూచీ, స్థిరరాశి అక్షరం *filename, అక్షరం **పేర్లు, పూర్ణాంకం classes, పూర్ణాంకం delay, అక్షరం *ఉపసర్గ, పూర్ణాంకం avg, దశాంశం hier, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం చట్రాలు, పూర్ణాంకం fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#అయితే_ముగింపు

