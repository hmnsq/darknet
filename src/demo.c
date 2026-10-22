#ಅನ್ತರ್ಭಾವ "network.h"
#ಅನ್ತರ್ಭಾವ "detection_layer.h"
#ಅನ್ತರ್ಭಾವ "region_layer.h"
#ಅನ್ತರ್ಭಾವ "cost_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "parser.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "demo.h"
#ಅನ್ತರ್ಭಾವ <sys/time.h>

#define DEMO 1

#ifdef OPENCV

ಸ್ಥಿರ ವರ್ಣ **demo_names;
ಸ್ಥಿರ image **demo_alphabet;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ demo_classes;

ಸ್ಥಿರ network *net;
ಸ್ಥಿರ image buff [3];
ಸ್ಥಿರ image buff_letter[3];
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ buff_index = 0;
ಸ್ಥಿರ ಶೂನ್ಯ * cap;
ಸ್ಥಿರ ಭಿನ್ನ fps = 0;
ಸ್ಥಿರ ಭಿನ್ನ demo_thresh = 0;
ಸ್ಥಿರ ಭಿನ್ನ demo_hier = .5;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ running = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ demo_frame = 3;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ demo_index = 0;
ಸ್ಥಿರ ಭಿನ್ನ **predictions;
ಸ್ಥಿರ ಭಿನ್ನ *avg;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ demo_done = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ demo_total = 0;
ದ್ವಿಗುಣ demo_time;

detection *get_network_boxes(network *net, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ thresh, ಭಿನ್ನ hier, ಪೂರ್ಣಾಙ್ಕ *map, ಪೂರ್ಣಾಙ್ಕ relative, ಪೂರ್ಣಾಙ್ಕ *ಅಂಕೆ);

ಪೂರ್ಣಾಙ್ಕ size_network(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಯದಿ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ಎಣಿಕೆ += l.outputs;
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಶೂನ್ಯ remember_network(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಯದಿ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + ಎಣಿಕೆ, net->layers[i].output, ಪರಿಮಾಣ(ಭಿನ್ನ) * l.outputs);
            ಎಣಿಕೆ += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, ಪೂರ್ಣಾಙ್ಕ *nboxes)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    fill_cpu(demo_total, 0, avg, 1);
    ಪ್ರತಿ(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಯದಿ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + ಎಣಿಕೆ, ಪರಿಮಾಣ(ಭಿನ್ನ) * l.outputs);
            ಎಣಿಕೆ += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    ಪ್ರತ್ಯಾಗಮ dets;
}

ಶೂನ್ಯ *detect_in_thread(ಶೂನ್ಯ *ptr)
{
    running = 1;
    ಭಿನ್ನ nms = .4;

    layer l = net->layers[net->n-1];
    ಭಿನ್ನ *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    ಪೂರ್ಣಾಙ್ಕ nboxes = 0;
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

    ಯದಿ (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ *fetch_in_thread(ಶೂನ್ಯ *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    ಯದಿ(buff[buff_index].data == 0) {
        demo_done = 1;
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ *display_in_thread(ಶೂನ್ಯ *ptr)
{
    ಪೂರ್ಣಾಙ್ಕ c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    ಯದಿ (c != -1) c = c%256;
    ಯದಿ (c == 27) {
        demo_done = 1;
        ಪ್ರತ್ಯಾಗಮ 0;
    } ಅನ್ಯಥಾ ಯದಿ (c == 82) {
        demo_thresh += .02;
    } ಅನ್ಯಥಾ ಯದಿ (c == 84) {
        demo_thresh -= .02;
        ಯದಿ(demo_thresh <= .02) demo_thresh = .02;
    } ಅನ್ಯಥಾ ಯದಿ (c == 83) {
        demo_hier += .02;
    } ಅನ್ಯಥಾ ಯದಿ (c == 81) {
        demo_hier -= .02;
        ಯದಿ(demo_hier <= .0) demo_hier = .0;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ *display_loop(ಶೂನ್ಯ *ptr)
{
    ಯಾವತ್(1){
        display_in_thread(0);
    }
}

ಶೂನ್ಯ *detect_loop(ಶೂನ್ಯ *ptr)
{
    ಯಾವತ್(1){
        detect_in_thread(0);
    }
}

ಶೂನ್ಯ demo(ವರ್ಣ *cfgfile, ವರ್ಣ *weightfile, ಭಿನ್ನ thresh, ಪೂರ್ಣಾಙ್ಕ cam_index, ಧ್ರುವ ವರ್ಣ *filename, ವರ್ಣ **names, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ delay, ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ avg_frames, ಭಿನ್ನ hier, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ frames, ಪೂರ್ಣಾಙ್ಕ fullscreen)
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

    ಪೂರ್ಣಾಙ್ಕ i;
    demo_total = size_network(net);
    predictions = calloc(demo_frame, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    ಪ್ರತಿ (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, ಪರಿಮಾಣ(ಭಿನ್ನ));
    }
    avg = calloc(demo_total, ಪರಿಮಾಣ(ಭಿನ್ನ));

    ಯದಿ(filename){
        printf("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }ಅನ್ಯಥಾ{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    ಯದಿ(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಯದಿ(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    ಯಾವತ್(!demo_done){
        buff_index = (buff_index + 1) %3;
        ಯದಿ(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        ಯದಿ(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        ಯದಿ(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }ಅನ್ಯಥಾ{
            ವರ್ಣ name[256];
            sprintf(name, "%s_%08d", prefix, ಎಣಿಕೆ);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++ಎಣಿಕೆ;
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
#ಅನ್ಯಥಾ
ಶೂನ್ಯ demo(ವರ್ಣ *cfgfile, ವರ್ಣ *weightfile, ಭಿನ್ನ thresh, ಪೂರ್ಣಾಙ್ಕ cam_index, ಧ್ರುವ ವರ್ಣ *filename, ವರ್ಣ **names, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ delay, ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ avg, ಭಿನ್ನ hier, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ frames, ಪೂರ್ಣಾಙ್ಕ fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif

