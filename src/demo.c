#ضمن "network.h"
#ضمن "detection_layer.h"
#ضمن "region_layer.h"
#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "parser.h"
#ضمن "box.h"
#ضمن "image.h"
#ضمن "demo.h"
#ضمن <sys/time.h>

#تعریف_کړه DEMO 1

#که_تعریف_شوی OPENCV

ساكن حرف **demo_نومونه;
ساكن image **demo_alphabet;
ساكن صحيح demo_classes;

ساكن شبکه *net;
ساكن image buff [3];
ساكن image buff_letter[3];
ساكن صحيح buff_شاخص = 0;
ساكن فراغ * cap;
ساكن عائم fps = 0;
ساكن عائم demo_thresh = 0;
ساكن عائم demo_hier = .5;
ساكن صحيح running = 0;

ساكن صحيح demo_چوکاټ = 3;
ساكن صحيح demo_شاخص = 0;
ساكن عائم **predictions;
ساكن عائم *avg;
ساكن صحيح demo_بشپړ = 0;
ساكن صحيح demo_ټول = 0;
مزدوج demo_وخت;

detection *get_شبکه_boxes(شبکه *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, صحيح *عدد);

صحيح اندازه_شبکه(شبکه *net)
{
    صحيح i;
    صحيح شمېر = 0;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        إذا(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            شمېر += l.outputs;
        }
    }
    أرجع شمېر;
}

فراغ remember_شبکه(شبکه *net)
{
    صحيح i;
    صحيح شمېر = 0;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        إذا(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            حافظه_نقل(predictions[demo_شاخص] + شمېر, net->layers[i].output, حجم(عائم) * l.outputs);
            شمېر += l.outputs;
        }
    }
}

detection *avg_predictions(شبکه *net, صحيح *nboxes)
{
    صحيح i, j;
    صحيح شمېر = 0;
    fill_cpu(demo_ټول, 0, avg, 1);
    لكل(j = 0; j < demo_چوکاټ; ++j){
        axpy_cpu(demo_ټول, 1./demo_چوکاټ, predictions[j], 1, avg, 1);
    }
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        إذا(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            حافظه_نقل(l.output, avg + شمېر, حجم(عائم) * l.outputs);
            شمېر += l.outputs;
        }
    }
    detection *dets = get_شبکه_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    أرجع dets;
}

فراغ *detect_دننه_thread(فراغ *ptr)
{
    running = 1;
    عائم nms = .4;

    layer l = net->layers[net->n-1];
    عائم *X = buff_letter[(buff_شاخص+2)%3].معلومات;
    شبکه_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_شبکه(net);
    detection *dets = 0;
    صحيح nboxes = 0;
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

    إذا (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    چاپ_کړه("\033[2J");
    چاپ_کړه("\033[1;1H");
    چاپ_کړه("\nFPS:%.1f\n",fps);
    چاپ_کړه("Objects:\n\n");
    image display = buff[(buff_شاخص+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_نومونه, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_شاخص = (demo_شاخص + 1)%demo_چوکاټ;
    running = 0;
    أرجع 0;
}

فراغ *fetch_دننه_thread(فراغ *ptr)
{
    free_image(buff[buff_شاخص]);
    buff[buff_شاخص] = get_image_from_بهیر(cap);
    إذا(buff[buff_شاخص].معلومات == 0) {
        demo_بشپړ = 1;
        أرجع 0;
    }
    letterbox_image_into(buff[buff_شاخص], net->w, net->h, buff_letter[buff_شاخص]);
    أرجع 0;
}

فراغ *display_دننه_thread(فراغ *ptr)
{
    صحيح c = show_image(buff[(buff_شاخص + 1)%3], "Demo", 1);
    إذا (c != -1) c = c%256;
    إذا (c == 27) {
        demo_بشپړ = 1;
        أرجع 0;
    } وإلا إذا (c == 82) {
        demo_thresh += .02;
    } وإلا إذا (c == 84) {
        demo_thresh -= .02;
        إذا(demo_thresh <= .02) demo_thresh = .02;
    } وإلا إذا (c == 83) {
        demo_hier += .02;
    } وإلا إذا (c == 81) {
        demo_hier -= .02;
        إذا(demo_hier <= .0) demo_hier = .0;
    }
    أرجع 0;
}

فراغ *display_loop(فراغ *ptr)
{
    طالما(1){
        display_دننه_thread(0);
    }
}

فراغ *detect_loop(فراغ *ptr)
{
    طالما(1){
        detect_دننه_thread(0);
    }
}

فراغ demo(حرف *cfgfile, حرف *weightfile, عائم thresh, صحيح cam_شاخص, ثابت حرف *filename, حرف **نومونه, صحيح classes, صحيح delay, حرف *مختاړی, صحيح avg_چوکاټونه, عائم hier, صحيح w, صحيح h, صحيح چوکاټونه, صحيح fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = بارول_alphabet();
    demo_نومونه = نومونه;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    چاپ_کړه("Demo\n");
    net = بارول_شبکه(cfgfile, weightfile, 0);
    سېټ_batch_شبکه(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    صحيح i;
    demo_ټول = اندازه_شبکه(net);
    predictions = حافظه_صفر_ورکړه(demo_چوکاټ, حجم(عائم*));
    لكل (i = 0; i < demo_چوکاټ; ++i){
        predictions[i] = حافظه_صفر_ورکړه(demo_ټول, حجم(عائم));
    }
    avg = حافظه_صفر_ورکړه(demo_ټول, حجم(عائم));

    إذا(filename){
        چاپ_کړه("video file: %s\n", filename);
        cap = open_video_بهیر(filename, 0, 0, 0, 0);
    }وإلا{
        cap = open_video_بهیر(0, cam_شاخص, w, h, چوکاټونه);
    }

    إذا(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_بهیر(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    صحيح شمېر = 0;
    إذا(!مختاړی){
        make_کړکۍ("Demo", 1352, 1013, fullscreen);
    }

    demo_وخت = what_وخت_is_it_now();

    طالما(!demo_بشپړ){
        buff_شاخص = (buff_شاخص + 1) %3;
        إذا(pthread_create(&fetch_thread, 0, fetch_دننه_thread, 0)) error("Thread creation failed");
        إذا(pthread_create(&detect_thread, 0, detect_دننه_thread, 0)) error("Thread creation failed");
        إذا(!مختاړی){
            fps = 1./(what_وخت_is_it_now() - demo_وخت);
            demo_وخت = what_وخت_is_it_now();
            display_دننه_thread(0);
        }وإلا{
            حرف name[256];
            sprintf(name, "%s_%08d", مختاړی, شمېر);
            save_image(buff[(buff_شاخص + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++شمېر;
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
#وإلا
فراغ demo(حرف *cfgfile, حرف *weightfile, عائم thresh, صحيح cam_شاخص, ثابت حرف *filename, حرف **نومونه, صحيح classes, صحيح delay, حرف *مختاړی, صحيح avg, عائم hier, صحيح w, صحيح h, صحيح چوکاټونه, صحيح fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#د_که_پای

