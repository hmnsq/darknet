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

#ವ್ಯಾಖ್ಯಾನಿಸು DEMO 1

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV

ಸ್ಥಿರ ಅಕ್ಷರ **demo_ಹೆಸರುಗಳು;
ಸ್ಥಿರ image **demo_alphabet;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ demo_classes;

ಸ್ಥಿರ ಜಾಲ *net;
ಸ್ಥಿರ image buff [3];
ಸ್ಥಿರ image buff_letter[3];
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ buff_ಸೂಚ್ಯಂಕ = 0;
ಸ್ಥಿರ ಶೂನ್ಯ * cap;
ಸ್ಥಿರ ದಶಮಾಂಶ fps = 0;
ಸ್ಥಿರ ದಶಮಾಂಶ demo_thresh = 0;
ಸ್ಥಿರ ದಶಮಾಂಶ demo_hier = .5;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ running = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ demo_ಚೌಕಟ್ಟು = 3;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ demo_ಸೂಚ್ಯಂಕ = 0;
ಸ್ಥಿರ ದಶಮಾಂಶ **predictions;
ಸ್ಥಿರ ದಶಮಾಂಶ *avg;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ demo_ಮುಗಿದ = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ demo_ಒಟ್ಟು = 0;
ದ್ವಿಗುಣ demo_ಸಮಯ;

detection *get_ಜಾಲ_boxes(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ thresh, ದಶಮಾಂಶ hier, ಪೂರ್ಣಾಂಕ *map, ಪೂರ್ಣಾಂಕ relative, ಪೂರ್ಣಾಂಕ *ಅಂಕೆ);

ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_ಜಾಲ(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಆದರೆ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ಎಣಿಕೆ_2 += l.outputs;
        }
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

ಶೂನ್ಯ remember_ಜಾಲ(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಆದರೆ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ಸ್ಮರಣೆ_ನಕಲು(predictions[demo_ಸೂಚ್ಯಂಕ] + ಎಣಿಕೆ_2, net->layers[i].output, ಗಾತ್ರ(ದಶಮಾಂಶ) * l.outputs);
            ಎಣಿಕೆ_2 += l.outputs;
        }
    }
}

detection *avg_predictions(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ *nboxes)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    fill_cpu(demo_ಒಟ್ಟು, 0, avg, 1);
    ಗಾಗಿ(j = 0; j < demo_ಚೌಕಟ್ಟು; ++j){
        axpy_cpu(demo_ಒಟ್ಟು, 1./demo_ಚೌಕಟ್ಟು, predictions[j], 1, avg, 1);
    }
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಆದರೆ(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ಸ್ಮರಣೆ_ನಕಲು(l.output, avg + ಎಣಿಕೆ_2, ಗಾತ್ರ(ದಶಮಾಂಶ) * l.outputs);
            ಎಣಿಕೆ_2 += l.outputs;
        }
    }
    detection *dets = get_ಜಾಲ_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    ಹಿಂತಿರುಗಿ dets;
}

ಶೂನ್ಯ *detect_ಒಳ_thread(ಶೂನ್ಯ *ptr)
{
    running = 1;
    ದಶಮಾಂಶ nms = .4;

    layer l = net->layers[net->n-1];
    ದಶಮಾಂಶ *X = buff_letter[(buff_ಸೂಚ್ಯಂಕ+2)%3].ದತ್ತಾಂಶ;
    ಜಾಲ_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_ಜಾಲ(net);
    detection *dets = 0;
    ಪೂರ್ಣಾಂಕ nboxes = 0;
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

    ಆದರೆ (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    ಬರೆ("\033[2J");
    ಬರೆ("\033[1;1H");
    ಬರೆ("\nFPS:%.1f\n",fps);
    ಬರೆ("Objects:\n\n");
    image display = buff[(buff_ಸೂಚ್ಯಂಕ+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_ಹೆಸರುಗಳು, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_ಸೂಚ್ಯಂಕ = (demo_ಸೂಚ್ಯಂಕ + 1)%demo_ಚೌಕಟ್ಟು;
    running = 0;
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ *fetch_ಒಳ_thread(ಶೂನ್ಯ *ptr)
{
    free_image(buff[buff_ಸೂಚ್ಯಂಕ]);
    buff[buff_ಸೂಚ್ಯಂಕ] = get_image_from_ಹರಿವು(cap);
    ಆದರೆ(buff[buff_ಸೂಚ್ಯಂಕ].ದತ್ತಾಂಶ == 0) {
        demo_ಮುಗಿದ = 1;
        ಹಿಂತಿರುಗಿ 0;
    }
    letterbox_image_into(buff[buff_ಸೂಚ್ಯಂಕ], net->w, net->h, buff_letter[buff_ಸೂಚ್ಯಂಕ]);
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ *display_ಒಳ_thread(ಶೂನ್ಯ *ptr)
{
    ಪೂರ್ಣಾಂಕ c = show_image(buff[(buff_ಸೂಚ್ಯಂಕ + 1)%3], "Demo", 1);
    ಆದರೆ (c != -1) c = c%256;
    ಆದರೆ (c == 27) {
        demo_ಮುಗಿದ = 1;
        ಹಿಂತಿರುಗಿ 0;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == 82) {
        demo_thresh += .02;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == 84) {
        demo_thresh -= .02;
        ಆದರೆ(demo_thresh <= .02) demo_thresh = .02;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == 83) {
        demo_hier += .02;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == 81) {
        demo_hier -= .02;
        ಆದರೆ(demo_hier <= .0) demo_hier = .0;
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ *display_loop(ಶೂನ್ಯ *ptr)
{
    ಆಗ(1){
        display_ಒಳ_thread(0);
    }
}

ಶೂನ್ಯ *detect_loop(ಶೂನ್ಯ *ptr)
{
    ಆಗ(1){
        detect_ಒಳ_thread(0);
    }
}

ಶೂನ್ಯ demo(ಅಕ್ಷರ *cfgfile, ಅಕ್ಷರ *weightfile, ದಶಮಾಂಶ thresh, ಪೂರ್ಣಾಂಕ cam_ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename, ಅಕ್ಷರ **ಹೆಸರುಗಳು, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ delay, ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ avg_ಚೌಕಟ್ಟುಗಳು, ದಶಮಾಂಶ hier, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ ಚೌಕಟ್ಟುಗಳು, ಪೂರ್ಣಾಂಕ fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = ತುಂಬು_alphabet();
    demo_ಹೆಸರುಗಳು = ಹೆಸರುಗಳು;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    ಬರೆ("Demo\n");
    net = ತುಂಬು_ಜಾಲ(cfgfile, weightfile, 0);
    ಗಣ_batch_ಜಾಲ(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    ಪೂರ್ಣಾಂಕ i;
    demo_ಒಟ್ಟು = ಗಾತ್ರ_ಜಾಲ(net);
    predictions = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(demo_ಚೌಕಟ್ಟು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    ಗಾಗಿ (i = 0; i < demo_ಚೌಕಟ್ಟು; ++i){
        predictions[i] = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(demo_ಒಟ್ಟು, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }
    avg = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(demo_ಒಟ್ಟು, ಗಾತ್ರ(ದಶಮಾಂಶ));

    ಆದರೆ(filename){
        ಬರೆ("video file: %s\n", filename);
        cap = open_video_ಹರಿವು(filename, 0, 0, 0, 0);
    }ಇಲ್ಲದಿದ್ದರೆ{
        cap = open_video_ಹರಿವು(0, cam_ಸೂಚ್ಯಂಕ, w, h, ಚೌಕಟ್ಟುಗಳು);
    }

    ಆದರೆ(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_ಹರಿವು(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಆದರೆ(!ಪೂರ್ವಪ್ರತ್ಯಯ){
        make_ಕಿಟಕಿ("Demo", 1352, 1013, fullscreen);
    }

    demo_ಸಮಯ = what_ಸಮಯ_is_it_now();

    ಆಗ(!demo_ಮುಗಿದ){
        buff_ಸೂಚ್ಯಂಕ = (buff_ಸೂಚ್ಯಂಕ + 1) %3;
        ಆದರೆ(pthread_create(&fetch_thread, 0, fetch_ಒಳ_thread, 0)) error("Thread creation failed");
        ಆದರೆ(pthread_create(&detect_thread, 0, detect_ಒಳ_thread, 0)) error("Thread creation failed");
        ಆದರೆ(!ಪೂರ್ವಪ್ರತ್ಯಯ){
            fps = 1./(what_ಸಮಯ_is_it_now() - demo_ಸಮಯ);
            demo_ಸಮಯ = what_ಸಮಯ_is_it_now();
            display_ಒಳ_thread(0);
        }ಇಲ್ಲದಿದ್ದರೆ{
            ಅಕ್ಷರ name[256];
            sprintf(name, "%s_%08d", ಪೂರ್ವಪ್ರತ್ಯಯ, ಎಣಿಕೆ_2);
            save_image(buff[(buff_ಸೂಚ್ಯಂಕ + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++ಎಣಿಕೆ_2;
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
#ಇಲ್ಲದಿದ್ದರೆ
ಶೂನ್ಯ demo(ಅಕ್ಷರ *cfgfile, ಅಕ್ಷರ *weightfile, ದಶಮಾಂಶ thresh, ಪೂರ್ಣಾಂಕ cam_ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename, ಅಕ್ಷರ **ಹೆಸರುಗಳು, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ delay, ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ avg, ದಶಮಾಂಶ hier, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ ಚೌಕಟ್ಟುಗಳು, ಪೂರ್ಣಾಂಕ fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#ಆದರೆ_ಅಂತ್ಯ

