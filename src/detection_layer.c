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
    l.لاگت_2 = صفر_حافظہ_دیو(1, حجم(عائم));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));
    l.delta = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#جے_تعریف_شدہ GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#جے_انت

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    أرجع l;
}

فراغ forward_detection_layer(ثابت detection_layer l, جال net)
{
    صحيح locations = l.side*l.side;
    صحيح i,j;
    حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(عائم));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    صحيح b;
    إذا (l.softmax){
        لكل(b = 0; b < l.batch; ++b){
            صحيح اشاریہ = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح وتھ = i*l.classes;
                softmax(l.output + اشاریہ + وتھ, l.classes, 1, 1,
                        l.output + اشاریہ + وتھ);
            }
        }
    }
    إذا(net.train){
        عائم avg_iou = 0;
        عائم avg_cat = 0;
        عائم avg_allcat = 0;
        عائم avg_obj = 0;
        عائم avg_anyobj = 0;
        صحيح گنتی = 0;
        *(l.لاگت_2) = 0;
        صحيح ناپ = l.inputs * l.batch;
        memset(l.delta, 0, ناپ * حجم(عائم));
        لكل (b = 0; b < l.batch; ++b){
            صحيح اشاریہ = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح truth_اشاریہ = (b*locations + i)*(1+l.coords+l.classes);
                صحيح is_obj = net.truth[truth_اشاریہ];
                لكل (j = 0; j < l.n; ++j) {
                    صحيح p_اشاریہ = اشاریہ + locations*l.classes + i*l.n + j;
                    l.delta[p_اشاریہ] = l.noobject_scale*(0 - l.output[p_اشاریہ]);
                    *(l.لاگت_2) += l.noobject_scale*قوت(l.output[p_اشاریہ], 2);
                    avg_anyobj += l.output[p_اشاریہ];
                }

                صحيح بہترین_اشاریہ = -1;
                عائم بہترین_iou = 0;
                عائم بہترین_rmse = 20;

                إذا (!is_obj){
                    استمر;
                }

                صحيح class_اشاریہ = اشاریہ + i*l.classes;
                لكل(j = 0; j < l.classes; ++j) {
                    l.delta[class_اشاریہ+j] = l.class_scale * (net.truth[truth_اشاریہ+1+j] - l.output[class_اشاریہ+j]);
                    *(l.لاگت_2) += l.class_scale * قوت(net.truth[truth_اشاریہ+1+j] - l.output[class_اشاریہ+j], 2);
                    إذا(net.truth[truth_اشاریہ + 1 + j]) avg_cat += l.output[class_اشاریہ+j];
                    avg_allcat += l.output[class_اشاریہ+j];
                }

                box truth = float_منزل_box(net.truth + truth_اشاریہ + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                لكل(j = 0; j < l.n; ++j){
                    صحيح box_اشاریہ = اشاریہ + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box باہرلا = float_منزل_box(l.output + box_اشاریہ, 1);
                    باہرلا.x /= l.side;
                    باہرلا.y /= l.side;

                    إذا (l.جذر){
                        باہرلا.w = باہرلا.w*باہرلا.w;
                        باہرلا.h = باہرلا.h*باہرلا.h;
                    }

                    عائم iou  = box_iou(باہرلا, truth);
                    //iou = 0;
                    عائم rmse = box_rmse(باہرلا, truth);
                    إذا(بہترین_iou > 0 || iou > 0){
                        إذا(iou > بہترین_iou){
                            بہترین_iou = iou;
                            بہترین_اشاریہ = j;
                        }
                    }وإلا{
                        إذا(rmse < بہترین_rmse){
                            بہترین_rmse = rmse;
                            بہترین_اشاریہ = j;
                        }
                    }
                }

                إذا(l.forced){
                    إذا(truth.w*truth.h < .1){
                        بہترین_اشاریہ = 1;
                    }وإلا{
                        بہترین_اشاریہ = 0;
                    }
                }
                إذا(l.random && *(net.تکیا) < 64000){
                    بہترین_اشاریہ = rand()%l.n;
                }

                صحيح box_اشاریہ = اشاریہ + locations*(l.classes + l.n) + (i*l.n + بہترین_اشاریہ) * l.coords;
                صحيح tbox_اشاریہ = truth_اشاریہ + 1 + l.classes;

                box باہرلا = float_منزل_box(l.output + box_اشاریہ, 1);
                باہرلا.x /= l.side;
                باہرلا.y /= l.side;
                إذا (l.جذر) {
                    باہرلا.w = باہرلا.w*باہرلا.w;
                    باہرلا.h = باہرلا.h*باہرلا.h;
                }
                عائم iou  = box_iou(باہرلا, truth);

                //printf("%d,", best_index);
                صحيح p_اشاریہ = اشاریہ + locations*l.classes + i*l.n + بہترین_اشاریہ;
                *(l.لاگت_2) -= l.noobject_scale * قوت(l.output[p_اشاریہ], 2);
                *(l.لاگت_2) += l.object_scale * قوت(1-l.output[p_اشاریہ], 2);
                avg_obj += l.output[p_اشاریہ];
                l.delta[p_اشاریہ] = l.object_scale * (1.-l.output[p_اشاریہ]);

                إذا(l.rescore){
                    l.delta[p_اشاریہ] = l.object_scale * (iou - l.output[p_اشاریہ]);
                }

                l.delta[box_اشاریہ+0] = l.coord_scale*(net.truth[tbox_اشاریہ + 0] - l.output[box_اشاریہ + 0]);
                l.delta[box_اشاریہ+1] = l.coord_scale*(net.truth[tbox_اشاریہ + 1] - l.output[box_اشاریہ + 1]);
                l.delta[box_اشاریہ+2] = l.coord_scale*(net.truth[tbox_اشاریہ + 2] - l.output[box_اشاریہ + 2]);
                l.delta[box_اشاریہ+3] = l.coord_scale*(net.truth[tbox_اشاریہ + 3] - l.output[box_اشاریہ + 3]);
                إذا(l.جذر){
                    l.delta[box_اشاریہ+2] = l.coord_scale*(جذر(net.truth[tbox_اشاریہ + 2]) - l.output[box_اشاریہ + 2]);
                    l.delta[box_اشاریہ+3] = l.coord_scale*(جذر(net.truth[tbox_اشاریہ + 3]) - l.output[box_اشاریہ + 3]);
                }

                *(l.لاگت_2) += قوت(1-iou, 2);
                avg_iou += iou;
                ++گنتی;
            }
        }

        إذا(0){
            عائم *costs = صفر_حافظہ_دیو(l.batch*locations*l.n, حجم(عائم));
            لكل (b = 0; b < l.batch; ++b) {
                صحيح اشاریہ = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_اشاریہ = اشاریہ + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_اشاریہ]*l.delta[p_اشاریہ];
                    }
                }
            }
            صحيح indexes[100];
            چوٹی_k(costs, l.batch*locations*l.n, 100, indexes);
            عائم cutoff = costs[indexes[99]];
            لكل (b = 0; b < l.batch; ++b) {
                صحيح اشاریہ = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_اشاریہ = اشاریہ + locations*l.classes + i*l.n + j;
                        إذا (l.delta[p_اشاریہ]*l.delta[p_اشاریہ] < cutoff) l.delta[p_اشاریہ] = 0;
                    }
                }
            }
            حرر(costs);
        }


        *(l.لاگت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);


        لکھو("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/گنتی, avg_cat/گنتی, avg_allcat/(گنتی*l.classes), avg_obj/گنتی, avg_anyobj/(l.batch*locations*l.n), گنتی);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

فراغ backward_detection_layer(ثابت detection_layer l, جال net)
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
        صحيح خانہ = i % l.side;
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاریہ = i*l.n + n;
            صحيح p_اشاریہ = l.side*l.side*l.classes + i*l.n + n;
            عائم scale = predictions[p_اشاریہ];
            صحيح box_اشاریہ = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_اشاریہ + 0] + خانہ) / l.side * w;
            b.y = (predictions[box_اشاریہ + 1] + صف) / l.side * h;
            b.w = قوت(predictions[box_اشاریہ + 2], (l.جذر?2:1)) * w;
            b.h = قوت(predictions[box_اشاریہ + 3], (l.جذر?2:1)) * h;
            dets[اشاریہ].bbox = b;
            dets[اشاریہ].objectness = scale;
            لكل(j = 0; j < l.classes; ++j){
                صحيح class_اشاریہ = i*l.classes;
                عائم prob = scale*predictions[class_اشاریہ+j];
                dets[اشاریہ].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#جے_تعریف_شدہ GPU

فراغ forward_detection_layer_gpu(ثابت detection_layer l, جال net)
{
    إذا(!net.train){
        copy_gpu(l.batch*l.inputs, net.داخلہ_gpu, 1, l.output_gpu, 1);
        أرجع;
    }

    cuda_pull_array(net.داخلہ_gpu, net.داخلہ, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ڈالو_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ backward_detection_layer_gpu(detection_layer l, جال net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جے_انت

