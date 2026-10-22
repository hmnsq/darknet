#ضمن "detection_layer.h"
#ضمن "activations.h"
#ضمن "softmax_layer.h"
#ضمن "blas.h"
#ضمن "box.h"
#ضمن "cuda.h"
#ضمن "utils.h"

#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <string.h>
#ضمن <stdlib.h>

detection_layer make_detection_layer(صحيح batch, صحيح inputs, صحيح n, صحيح side, صحيح classes, صحيح coords, صحيح rescore)
{
    detection_layer l = {0};
    l.type = DETECTION;

    l.n = n;
    l.batch = batch;
    l.inputs = inputs;
    l.classes = classes;
    l.coords = coords;
    l.rescore = rescore;
    l.side = side;
    l.w = side;
    l.h = side;
    أكد(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.لاڳت_2 = صفر_يادگيري_ڏيو(1, حجم(عائم));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = صفر_يادگيري_ڏيو(batch*l.outputs, حجم(عائم));
    l.delta = صفر_يادگيري_ڏيو(batch*l.outputs, حجم(عائم));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#جيڪڏهن_پورو

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    أرجع l;
}

فراغ forward_detection_layer(ثابت detection_layer l, ڄار net)
{
    صحيح locations = l.side*l.side;
    صحيح i,j;
    يادگيري_نقل(l.output, net.داخلا, l.outputs*l.batch*حجم(عائم));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    صحيح b;
    إذا (l.softmax){
        لكل(b = 0; b < l.batch; ++b){
            صحيح اشاريو = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح وٿي = i*l.classes;
                softmax(l.output + اشاريو + وٿي, l.classes, 1, 1,
                        l.output + اشاريو + وٿي);
            }
        }
    }
    إذا(net.train){
        عائم avg_iou = 0;
        عائم avg_cat = 0;
        عائم avg_allcat = 0;
        عائم avg_obj = 0;
        عائم avg_anyobj = 0;
        صحيح ڳڻپ = 0;
        *(l.لاڳت_2) = 0;
        صحيح سائيز = l.inputs * l.batch;
        memset(l.delta, 0, سائيز * حجم(عائم));
        لكل (b = 0; b < l.batch; ++b){
            صحيح اشاريو = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح truth_اشاريو = (b*locations + i)*(1+l.coords+l.classes);
                صحيح is_obj = net.truth[truth_اشاريو];
                لكل (j = 0; j < l.n; ++j) {
                    صحيح p_اشاريو = اشاريو + locations*l.classes + i*l.n + j;
                    l.delta[p_اشاريو] = l.noobject_scale*(0 - l.output[p_اشاريو]);
                    *(l.لاڳت_2) += l.noobject_scale*قوت(l.output[p_اشاريو], 2);
                    avg_anyobj += l.output[p_اشاريو];
                }

                صحيح بهترين_اشاريو = -1;
                عائم بهترين_iou = 0;
                عائم بهترين_rmse = 20;

                إذا (!is_obj){
                    استمر;
                }

                صحيح class_اشاريو = اشاريو + i*l.classes;
                لكل(j = 0; j < l.classes; ++j) {
                    l.delta[class_اشاريو+j] = l.class_scale * (net.truth[truth_اشاريو+1+j] - l.output[class_اشاريو+j]);
                    *(l.لاڳت_2) += l.class_scale * قوت(net.truth[truth_اشاريو+1+j] - l.output[class_اشاريو+j], 2);
                    إذا(net.truth[truth_اشاريو + 1 + j]) avg_cat += l.output[class_اشاريو+j];
                    avg_allcat += l.output[class_اشاريو+j];
                }

                box truth = float_منزل_box(net.truth + truth_اشاريو + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                لكل(j = 0; j < l.n; ++j){
                    صحيح box_اشاريو = اشاريو + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box ٻاهريون = float_منزل_box(l.output + box_اشاريو, 1);
                    ٻاهريون.x /= l.side;
                    ٻاهريون.y /= l.side;

                    إذا (l.جذر){
                        ٻاهريون.w = ٻاهريون.w*ٻاهريون.w;
                        ٻاهريون.h = ٻاهريون.h*ٻاهريون.h;
                    }

                    عائم iou  = box_iou(ٻاهريون, truth);
                    //iou = 0;
                    عائم rmse = box_rmse(ٻاهريون, truth);
                    إذا(بهترين_iou > 0 || iou > 0){
                        إذا(iou > بهترين_iou){
                            بهترين_iou = iou;
                            بهترين_اشاريو = j;
                        }
                    }وإلا{
                        إذا(rmse < بهترين_rmse){
                            بهترين_rmse = rmse;
                            بهترين_اشاريو = j;
                        }
                    }
                }

                إذا(l.forced){
                    إذا(truth.w*truth.h < .1){
                        بهترين_اشاريو = 1;
                    }وإلا{
                        بهترين_اشاريو = 0;
                    }
                }
                إذا(l.random && *(net.ڏٺل) < 64000){
                    بهترين_اشاريو = rand()%l.n;
                }

                صحيح box_اشاريو = اشاريو + locations*(l.classes + l.n) + (i*l.n + بهترين_اشاريو) * l.coords;
                صحيح tbox_اشاريو = truth_اشاريو + 1 + l.classes;

                box ٻاهريون = float_منزل_box(l.output + box_اشاريو, 1);
                ٻاهريون.x /= l.side;
                ٻاهريون.y /= l.side;
                إذا (l.جذر) {
                    ٻاهريون.w = ٻاهريون.w*ٻاهريون.w;
                    ٻاهريون.h = ٻاهريون.h*ٻاهريون.h;
                }
                عائم iou  = box_iou(ٻاهريون, truth);

                //printf("%d,", best_index);
                صحيح p_اشاريو = اشاريو + locations*l.classes + i*l.n + بهترين_اشاريو;
                *(l.لاڳت_2) -= l.noobject_scale * قوت(l.output[p_اشاريو], 2);
                *(l.لاڳت_2) += l.object_scale * قوت(1-l.output[p_اشاريو], 2);
                avg_obj += l.output[p_اشاريو];
                l.delta[p_اشاريو] = l.object_scale * (1.-l.output[p_اشاريو]);

                إذا(l.rescore){
                    l.delta[p_اشاريو] = l.object_scale * (iou - l.output[p_اشاريو]);
                }

                l.delta[box_اشاريو+0] = l.coord_scale*(net.truth[tbox_اشاريو + 0] - l.output[box_اشاريو + 0]);
                l.delta[box_اشاريو+1] = l.coord_scale*(net.truth[tbox_اشاريو + 1] - l.output[box_اشاريو + 1]);
                l.delta[box_اشاريو+2] = l.coord_scale*(net.truth[tbox_اشاريو + 2] - l.output[box_اشاريو + 2]);
                l.delta[box_اشاريو+3] = l.coord_scale*(net.truth[tbox_اشاريو + 3] - l.output[box_اشاريو + 3]);
                إذا(l.جذر){
                    l.delta[box_اشاريو+2] = l.coord_scale*(جذر(net.truth[tbox_اشاريو + 2]) - l.output[box_اشاريو + 2]);
                    l.delta[box_اشاريو+3] = l.coord_scale*(جذر(net.truth[tbox_اشاريو + 3]) - l.output[box_اشاريو + 3]);
                }

                *(l.لاڳت_2) += قوت(1-iou, 2);
                avg_iou += iou;
                ++ڳڻپ;
            }
        }

        إذا(0){
            عائم *costs = صفر_يادگيري_ڏيو(l.batch*locations*l.n, حجم(عائم));
            لكل (b = 0; b < l.batch; ++b) {
                صحيح اشاريو = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_اشاريو = اشاريو + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_اشاريو]*l.delta[p_اشاريو];
                    }
                }
            }
            صحيح indexes[100];
            چوٽي_k(costs, l.batch*locations*l.n, 100, indexes);
            عائم cutoff = costs[indexes[99]];
            لكل (b = 0; b < l.batch; ++b) {
                صحيح اشاريو = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_اشاريو = اشاريو + locations*l.classes + i*l.n + j;
                        إذا (l.delta[p_اشاريو]*l.delta[p_اشاريو] < cutoff) l.delta[p_اشاريو] = 0;
                    }
                }
            }
            حرر(costs);
        }


        *(l.لاڳت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);


        لکو("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/ڳڻپ, avg_cat/ڳڻپ, avg_allcat/(ڳڻپ*l.classes), avg_obj/ڳڻپ, avg_anyobj/(l.batch*locations*l.n), ڳڻپ);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

فراغ backward_detection_layer(ثابت detection_layer l, ڄار net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

فراغ get_detection_detections(layer l, صحيح w, صحيح h, عائم thresh, detection *dets)
{
    صحيح i,j,n;
    عائم *predictions = l.output;
    //int per_cell = 5*num+classes;
    لكل (i = 0; i < l.side*l.side; ++i){
        صحيح صف = i / l.side;
        صحيح کانو = i % l.side;
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاريو = i*l.n + n;
            صحيح p_اشاريو = l.side*l.side*l.classes + i*l.n + n;
            عائم scale = predictions[p_اشاريو];
            صحيح box_اشاريو = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_اشاريو + 0] + کانو) / l.side * w;
            b.y = (predictions[box_اشاريو + 1] + صف) / l.side * h;
            b.w = قوت(predictions[box_اشاريو + 2], (l.جذر?2:1)) * w;
            b.h = قوت(predictions[box_اشاريو + 3], (l.جذر?2:1)) * h;
            dets[اشاريو].bbox = b;
            dets[اشاريو].objectness = scale;
            لكل(j = 0; j < l.classes; ++j){
                صحيح class_اشاريو = i*l.classes;
                عائم prob = scale*predictions[class_اشاريو+j];
                dets[اشاريو].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ forward_detection_layer_gpu(ثابت detection_layer l, ڄار net)
{
    إذا(!net.train){
        copy_gpu(l.batch*l.inputs, net.داخلا_gpu, 1, l.output_gpu, 1);
        أرجع;
    }

    cuda_pull_array(net.داخلا_gpu, net.داخلا, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ڌڪو_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ڌڪو_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ backward_detection_layer_gpu(detection_layer l, ڄار net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جيڪڏهن_پورو

