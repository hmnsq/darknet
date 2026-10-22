#include "network.h"
#include "detection_layer.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "image.h"
#include "demo.h"
#include <sys/time.h>

#define DEMO 1

#ifdef OPENCV

static var_na **demo_names;
static image **demo_alphabet;
static ཧྲིལ demo_classes;

static network *net;
static image buff [3];
static image buff_letter[3];
static ཧྲིལ buff_index = 0;
static void * cap;
static float fps = 0;
static float demo_thresh = 0;
static float demo_hier = .5;
static ཧྲིལ running = 0;

static ཧྲིལ demo_frame = 3;
static ཧྲིལ demo_index = 0;
static float **predictions;
static float *avg;
static ཧྲིལ demo_done = 0;
static ཧྲིལ demo_total = 0;
dviguu_na demo_time;

detection *get_network_boxes(network *net, ཧྲིལ w, ཧྲིལ h, float thresh, float hier, ཧྲིལ *map, ཧྲིལ relative, ཧྲིལ *num);

ཧྲིལ size_network(network *net)
{
    ཧྲིལ i;
    ཧྲིལ count = 0;
    དོན་ལུ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        གལ་སྲིད(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            count += l.outputs;
        }
    }
    ལོག count;
}

void remember_network(network *net)
{
    ཧྲིལ i;
    ཧྲིལ count = 0;
    དོན་ལུ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        གལ་སྲིད(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + count, net->layers[i].output, parimaa_na(float) * l.outputs);
            count += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, ཧྲིལ *nboxes)
{
    ཧྲིལ i, j;
    ཧྲིལ count = 0;
    fill_cpu(demo_total, 0, avg, 1);
    དོན་ལུ(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    དོན་ལུ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        གལ་སྲིད(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + count, parimaa_na(float) * l.outputs);
            count += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    ལོག dets;
}

void *detect_in_thread(void *ptr)
{
    running = 1;
    float nms = .4;

    layer l = net->layers[net->n-1];
    float *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    ཧྲིལ nboxes = 0;
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

    གལ་སྲིད (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    ལོག 0;
}

void *fetch_in_thread(void *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    གལ་སྲིད(buff[buff_index].data == 0) {
        demo_done = 1;
        ལོག 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    ལོག 0;
}

void *display_in_thread(void *ptr)
{
    ཧྲིལ c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    གལ་སྲིད (c != -1) c = c%256;
    གལ་སྲིད (c == 27) {
        demo_done = 1;
        ལོག 0;
    } དེ་མིན གལ་སྲིད (c == 82) {
        demo_thresh += .02;
    } དེ་མིན གལ་སྲིད (c == 84) {
        demo_thresh -= .02;
        གལ་སྲིད(demo_thresh <= .02) demo_thresh = .02;
    } དེ་མིན གལ་སྲིད (c == 83) {
        demo_hier += .02;
    } དེ་མིན གལ་སྲིད (c == 81) {
        demo_hier -= .02;
        གལ་སྲིད(demo_hier <= .0) demo_hier = .0;
    }
    ལོག 0;
}

void *display_loop(void *ptr)
{
    སྐབས(1){
        display_in_thread(0);
    }
}

void *detect_loop(void *ptr)
{
    སྐབས(1){
        detect_in_thread(0);
    }
}

void demo(var_na *cfgfile, var_na *weightfile, float thresh, ཧྲིལ cam_index, const var_na *filename, var_na **names, ཧྲིལ classes, ཧྲིལ delay, var_na *prefix, ཧྲིལ avg_frames, float hier, ཧྲིལ w, ཧྲིལ h, ཧྲིལ frames, ཧྲིལ fullscreen)
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

    ཧྲིལ i;
    demo_total = size_network(net);
    predictions = calloc(demo_frame, parimaa_na(float*));
    དོན་ལུ (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, parimaa_na(float));
    }
    avg = calloc(demo_total, parimaa_na(float));

    གལ་སྲིད(filename){
        printf("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }དེ་མིན{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    གལ་སྲིད(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    ཧྲིལ count = 0;
    གལ་སྲིད(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    སྐབས(!demo_done){
        buff_index = (buff_index + 1) %3;
        གལ་སྲིད(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        གལ་སྲིད(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        གལ་སྲིད(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }དེ་མིན{
            var_na name[256];
            sprintf(name, "%s_%08d", prefix, count);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++count;
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
#དེ་མིན
void demo(var_na *cfgfile, var_na *weightfile, float thresh, ཧྲིལ cam_index, const var_na *filename, var_na **names, ཧྲིལ classes, ཧྲིལ delay, var_na *prefix, ཧྲིལ avg, float hier, ཧྲིལ w, ཧྲིལ h, ཧྲིལ frames, ཧྲིལ fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif

