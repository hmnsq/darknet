#അന്തര്ഭാവ "network.h"
#അന്തര്ഭാവ "detection_layer.h"
#അന്തര്ഭാവ "region_layer.h"
#അന്തര്ഭാവ "cost_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "parser.h"
#അന്തര്ഭാവ "box.h"
#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "demo.h"
#അന്തര്ഭാവ <sys/time.h>

#define DEMO 1

#ifdef OPENCV

സ്ഥിതം അക്ഷരം **demo_names;
സ്ഥിതം image **demo_alphabet;
സ്ഥിതം പൂർണ്ണസംഖ്യ demo_classes;

സ്ഥിതം network *net;
സ്ഥിതം image buff [3];
സ്ഥിതം image buff_letter[3];
സ്ഥിതം പൂർണ്ണസംഖ്യ buff_index = 0;
സ്ഥിതം ശൂന്യം * cap;
സ്ഥിതം ദശാംശം fps = 0;
സ്ഥിതം ദശാംശം demo_thresh = 0;
സ്ഥിതം ദശാംശം demo_hier = .5;
സ്ഥിതം പൂർണ്ണസംഖ്യ running = 0;

സ്ഥിതം പൂർണ്ണസംഖ്യ demo_frame = 3;
സ്ഥിതം പൂർണ്ണസംഖ്യ demo_index = 0;
സ്ഥിതം ദശാംശം **predictions;
സ്ഥിതം ദശാംശം *avg;
സ്ഥിതം പൂർണ്ണസംഖ്യ demo_done = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ demo_total = 0;
ഇരട്ട demo_time;

detection *get_network_boxes(network *net, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം thresh, ദശാംശം hier, പൂർണ്ണസംഖ്യ *map, പൂർണ്ണസംഖ്യ relative, പൂർണ്ണസംഖ്യ *അക്കം);

പൂർണ്ണസംഖ്യ size_network(network *net)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        എങ്കിൽ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            എണ്ണം += l.outputs;
        }
    }
    തിരികെ എണ്ണം;
}

ശൂന്യം remember_network(network *net)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        എങ്കിൽ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + എണ്ണം, net->layers[i].output, വലുപ്പം(ദശാംശം) * l.outputs);
            എണ്ണം += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, പൂർണ്ണസംഖ്യ *nboxes)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    fill_cpu(demo_total, 0, avg, 1);
    വേണ്ടി(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    വേണ്ടി(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        എങ്കിൽ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + എണ്ണം, വലുപ്പം(ദശാംശം) * l.outputs);
            എണ്ണം += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    തിരികെ dets;
}

ശൂന്യം *detect_in_thread(ശൂന്യം *ptr)
{
    running = 1;
    ദശാംശം nms = .4;

    layer l = net->layers[net->n-1];
    ദശാംശം *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    പൂർണ്ണസംഖ്യ nboxes = 0;
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

    എങ്കിൽ (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    തിരികെ 0;
}

ശൂന്യം *fetch_in_thread(ശൂന്യം *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    എങ്കിൽ(buff[buff_index].data == 0) {
        demo_done = 1;
        തിരികെ 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    തിരികെ 0;
}

ശൂന്യം *display_in_thread(ശൂന്യം *ptr)
{
    പൂർണ്ണസംഖ്യ c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    എങ്കിൽ (c != -1) c = c%256;
    എങ്കിൽ (c == 27) {
        demo_done = 1;
        തിരികെ 0;
    } അല്ലെങ്കിൽ എങ്കിൽ (c == 82) {
        demo_thresh += .02;
    } അല്ലെങ്കിൽ എങ്കിൽ (c == 84) {
        demo_thresh -= .02;
        എങ്കിൽ(demo_thresh <= .02) demo_thresh = .02;
    } അല്ലെങ്കിൽ എങ്കിൽ (c == 83) {
        demo_hier += .02;
    } അല്ലെങ്കിൽ എങ്കിൽ (c == 81) {
        demo_hier -= .02;
        എങ്കിൽ(demo_hier <= .0) demo_hier = .0;
    }
    തിരികെ 0;
}

ശൂന്യം *display_loop(ശൂന്യം *ptr)
{
    ആകുമ്പോൾ(1){
        display_in_thread(0);
    }
}

ശൂന്യം *detect_loop(ശൂന്യം *ptr)
{
    ആകുമ്പോൾ(1){
        detect_in_thread(0);
    }
}

ശൂന്യം demo(അക്ഷരം *cfgfile, അക്ഷരം *weightfile, ദശാംശം thresh, പൂർണ്ണസംഖ്യ cam_index, സ്ഥിരം അക്ഷരം *filename, അക്ഷരം **names, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ delay, അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ avg_frames, ദശാംശം hier, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ frames, പൂർണ്ണസംഖ്യ fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = load_alphabet();
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    printf("Demo\n");
    net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    പൂർണ്ണസംഖ്യ i;
    demo_total = size_network(net);
    predictions = calloc(demo_frame, വലുപ്പം(ദശാംശം*));
    വേണ്ടി (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, വലുപ്പം(ദശാംശം));
    }
    avg = calloc(demo_total, വലുപ്പം(ദശാംശം));

    എങ്കിൽ(filename){
        printf("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }അല്ലെങ്കിൽ{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    എങ്കിൽ(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    എങ്കിൽ(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    ആകുമ്പോൾ(!demo_done){
        buff_index = (buff_index + 1) %3;
        എങ്കിൽ(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        എങ്കിൽ(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        എങ്കിൽ(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }അല്ലെങ്കിൽ{
            അക്ഷരം name[256];
            sprintf(name, "%s_%08d", prefix, എണ്ണം);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++എണ്ണം;
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
#അല്ലെങ്കിൽ
ശൂന്യം demo(അക്ഷരം *cfgfile, അക്ഷരം *weightfile, ദശാംശം thresh, പൂർണ്ണസംഖ്യ cam_index, സ്ഥിരം അക്ഷരം *filename, അക്ഷരം **names, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ delay, അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ avg, ദശാംശം hier, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ frames, പൂർണ്ണസംഖ്യ fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif

