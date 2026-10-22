#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "detection_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "region_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cost_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "demo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEMO 1

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV

ਸਥਿਰ ਅੱਖਰ **demo_ਨਾਂ;
ਸਥਿਰ image **demo_alphabet;
ਸਥਿਰ ਪੂਰਨ demo_classes;

ਸਥਿਰ ਜਾਲ *net;
ਸਥਿਰ image buff [3];
ਸਥਿਰ image buff_letter[3];
ਸਥਿਰ ਪੂਰਨ buff_ਸੂਚਕਾਂਕ = 0;
ਸਥਿਰ ਖਾਲੀ * cap;
ਸਥਿਰ ਦਸ਼ਮਲਵ fps = 0;
ਸਥਿਰ ਦਸ਼ਮਲਵ demo_thresh = 0;
ਸਥਿਰ ਦਸ਼ਮਲਵ demo_hier = .5;
ਸਥਿਰ ਪੂਰਨ running = 0;

ਸਥਿਰ ਪੂਰਨ demo_ਚੌਖਟਾ = 3;
ਸਥਿਰ ਪੂਰਨ demo_ਸੂਚਕਾਂਕ = 0;
ਸਥਿਰ ਦਸ਼ਮਲਵ **predictions;
ਸਥਿਰ ਦਸ਼ਮਲਵ *avg;
ਸਥਿਰ ਪੂਰਨ demo_ਪੂਰਾ = 0;
ਸਥਿਰ ਪੂਰਨ demo_ਕੁੱਲ = 0;
ਦੁੱਗਣਾ demo_ਸਮਾਂ;

detection *get_ਜਾਲ_boxes(ਜਾਲ *net, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ thresh, ਦਸ਼ਮਲਵ hier, ਪੂਰਨ *map, ਪੂਰਨ relative, ਪੂਰਨ *ਅੰਕ);

ਪੂਰਨ ਮਾਪ_ਜਾਲ(ਜਾਲ *net)
{
    ਪੂਰਨ i;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ਜੇ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ਗਿਣਤੀ_2 += l.outputs;
        }
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਖਾਲੀ remember_ਜਾਲ(ਜਾਲ *net)
{
    ਪੂਰਨ i;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ਜੇ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(predictions[demo_ਸੂਚਕਾਂਕ] + ਗਿਣਤੀ_2, net->layers[i].output, ਆਕਾਰ(ਦਸ਼ਮਲਵ) * l.outputs);
            ਗਿਣਤੀ_2 += l.outputs;
        }
    }
}

detection *avg_predictions(ਜਾਲ *net, ਪੂਰਨ *nboxes)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    fill_cpu(demo_ਕੁੱਲ, 0, avg, 1);
    ਲਈ(j = 0; j < demo_ਚੌਖਟਾ; ++j){
        axpy_cpu(demo_ਕੁੱਲ, 1./demo_ਚੌਖਟਾ, predictions[j], 1, avg, 1);
    }
    ਲਈ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ਜੇ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(l.output, avg + ਗਿਣਤੀ_2, ਆਕਾਰ(ਦਸ਼ਮਲਵ) * l.outputs);
            ਗਿਣਤੀ_2 += l.outputs;
        }
    }
    detection *dets = get_ਜਾਲ_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    ਵਾਪਸ dets;
}

ਖਾਲੀ *detect_ਵਿੱਚ_thread(ਖਾਲੀ *ptr)
{
    running = 1;
    ਦਸ਼ਮਲਵ nms = .4;

    layer l = net->layers[net->n-1];
    ਦਸ਼ਮਲਵ *X = buff_letter[(buff_ਸੂਚਕਾਂਕ+2)%3].ਅੰਕੜੇ;
    ਜਾਲ_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_ਜਾਲ(net);
    detection *dets = 0;
    ਪੂਰਨ nboxes = 0;
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

    ਜੇ (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    ਲਿਖੋ("\033[2J");
    ਲਿਖੋ("\033[1;1H");
    ਲਿਖੋ("\nFPS:%.1f\n",fps);
    ਲਿਖੋ("Objects:\n\n");
    image display = buff[(buff_ਸੂਚਕਾਂਕ+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_ਨਾਂ, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_ਸੂਚਕਾਂਕ = (demo_ਸੂਚਕਾਂਕ + 1)%demo_ਚੌਖਟਾ;
    running = 0;
    ਵਾਪਸ 0;
}

ਖਾਲੀ *fetch_ਵਿੱਚ_thread(ਖਾਲੀ *ptr)
{
    free_image(buff[buff_ਸੂਚਕਾਂਕ]);
    buff[buff_ਸੂਚਕਾਂਕ] = get_image_from_ਧਾਰਾ(cap);
    ਜੇ(buff[buff_ਸੂਚਕਾਂਕ].ਅੰਕੜੇ == 0) {
        demo_ਪੂਰਾ = 1;
        ਵਾਪਸ 0;
    }
    letterbox_image_into(buff[buff_ਸੂਚਕਾਂਕ], net->w, net->h, buff_letter[buff_ਸੂਚਕਾਂਕ]);
    ਵਾਪਸ 0;
}

ਖਾਲੀ *display_ਵਿੱਚ_thread(ਖਾਲੀ *ptr)
{
    ਪੂਰਨ c = show_image(buff[(buff_ਸੂਚਕਾਂਕ + 1)%3], "Demo", 1);
    ਜੇ (c != -1) c = c%256;
    ਜੇ (c == 27) {
        demo_ਪੂਰਾ = 1;
        ਵਾਪਸ 0;
    } ਨਹੀਂਤਾਂ ਜੇ (c == 82) {
        demo_thresh += .02;
    } ਨਹੀਂਤਾਂ ਜੇ (c == 84) {
        demo_thresh -= .02;
        ਜੇ(demo_thresh <= .02) demo_thresh = .02;
    } ਨਹੀਂਤਾਂ ਜੇ (c == 83) {
        demo_hier += .02;
    } ਨਹੀਂਤਾਂ ਜੇ (c == 81) {
        demo_hier -= .02;
        ਜੇ(demo_hier <= .0) demo_hier = .0;
    }
    ਵਾਪਸ 0;
}

ਖਾਲੀ *display_loop(ਖਾਲੀ *ptr)
{
    ਜਦੋਂ(1){
        display_ਵਿੱਚ_thread(0);
    }
}

ਖਾਲੀ *detect_loop(ਖਾਲੀ *ptr)
{
    ਜਦੋਂ(1){
        detect_ਵਿੱਚ_thread(0);
    }
}

ਖਾਲੀ demo(ਅੱਖਰ *cfgfile, ਅੱਖਰ *weightfile, ਦਸ਼ਮਲਵ thresh, ਪੂਰਨ cam_ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ *filename, ਅੱਖਰ **ਨਾਂ, ਪੂਰਨ classes, ਪੂਰਨ delay, ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ avg_ਚੌਖਟੇ, ਦਸ਼ਮਲਵ hier, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ ਚੌਖਟੇ, ਪੂਰਨ fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = ਲੱਦੋ_alphabet();
    demo_ਨਾਂ = ਨਾਂ;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    ਲਿਖੋ("Demo\n");
    net = ਲੱਦੋ_ਜਾਲ(cfgfile, weightfile, 0);
    ਸੈੱਟ_batch_ਜਾਲ(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    ਪੂਰਨ i;
    demo_ਕੁੱਲ = ਮਾਪ_ਜਾਲ(net);
    predictions = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(demo_ਚੌਖਟਾ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    ਲਈ (i = 0; i < demo_ਚੌਖਟਾ; ++i){
        predictions[i] = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(demo_ਕੁੱਲ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }
    avg = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(demo_ਕੁੱਲ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    ਜੇ(filename){
        ਲਿਖੋ("video file: %s\n", filename);
        cap = open_video_ਧਾਰਾ(filename, 0, 0, 0, 0);
    }ਨਹੀਂਤਾਂ{
        cap = open_video_ਧਾਰਾ(0, cam_ਸੂਚਕਾਂਕ, w, h, ਚੌਖਟੇ);
    }

    ਜੇ(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_ਧਾਰਾ(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਜੇ(!ਅਗੇਤਰ){
        make_ਖਿੜਕੀ("Demo", 1352, 1013, fullscreen);
    }

    demo_ਸਮਾਂ = what_ਸਮਾਂ_is_it_now();

    ਜਦੋਂ(!demo_ਪੂਰਾ){
        buff_ਸੂਚਕਾਂਕ = (buff_ਸੂਚਕਾਂਕ + 1) %3;
        ਜੇ(pthread_create(&fetch_thread, 0, fetch_ਵਿੱਚ_thread, 0)) error("Thread creation failed");
        ਜੇ(pthread_create(&detect_thread, 0, detect_ਵਿੱਚ_thread, 0)) error("Thread creation failed");
        ਜੇ(!ਅਗੇਤਰ){
            fps = 1./(what_ਸਮਾਂ_is_it_now() - demo_ਸਮਾਂ);
            demo_ਸਮਾਂ = what_ਸਮਾਂ_is_it_now();
            display_ਵਿੱਚ_thread(0);
        }ਨਹੀਂਤਾਂ{
            ਅੱਖਰ name[256];
            sprintf(name, "%s_%08d", ਅਗੇਤਰ, ਗਿਣਤੀ_2);
            save_image(buff[(buff_ਸੂਚਕਾਂਕ + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++ਗਿਣਤੀ_2;
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
#ਨਹੀਂਤਾਂ
ਖਾਲੀ demo(ਅੱਖਰ *cfgfile, ਅੱਖਰ *weightfile, ਦਸ਼ਮਲਵ thresh, ਪੂਰਨ cam_ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ *filename, ਅੱਖਰ **ਨਾਂ, ਪੂਰਨ classes, ਪੂਰਨ delay, ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ avg, ਦਸ਼ਮਲਵ hier, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ ਚੌਖਟੇ, ਪੂਰਨ fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#ਜੇ_ਅੰਤ

