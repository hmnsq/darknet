#ਅਨ੍ਤਰ੍ਭਾਵ "detection_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "softmax_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

detection_layer make_detection_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ n, ਪੂਰਨ side, ਪੂਰਨ classes, ਪੂਰਨ coords, ਪੂਰਨ rescore)
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
    ਪ੍ਰਤਿਜ੍ਞਾ(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ਜੇ_ਅੰਤ

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    ਵਾਪਸ l;
}

ਖਾਲੀ forward_detection_layer(ਅਚਲ detection_layer l, ਜਾਲ net)
{
    ਪੂਰਨ locations = l.side*l.side;
    ਪੂਰਨ i,j;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(l.output, net.ਨਿਵੇਸ਼, l.outputs*l.batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    ਪੂਰਨ b;
    ਜੇ (l.softmax){
        ਲਈ(b = 0; b < l.batch; ++b){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.inputs;
            ਲਈ (i = 0; i < locations; ++i) {
                ਪੂਰਨ ਵਿੱਥ = i*l.classes;
                softmax(l.output + ਸੂਚਕਾਂਕ + ਵਿੱਥ, l.classes, 1, 1,
                        l.output + ਸੂਚਕਾਂਕ + ਵਿੱਥ);
            }
        }
    }
    ਜੇ(net.train){
        ਦਸ਼ਮਲਵ avg_iou = 0;
        ਦਸ਼ਮਲਵ avg_cat = 0;
        ਦਸ਼ਮਲਵ avg_allcat = 0;
        ਦਸ਼ਮਲਵ avg_obj = 0;
        ਦਸ਼ਮਲਵ avg_anyobj = 0;
        ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
        *(l.ਲਾਗਤ_2) = 0;
        ਪੂਰਨ ਮਾਪ = l.inputs * l.batch;
        memset(l.delta, 0, ਮਾਪ * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਲਈ (b = 0; b < l.batch; ++b){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.inputs;
            ਲਈ (i = 0; i < locations; ++i) {
                ਪੂਰਨ truth_ਸੂਚਕਾਂਕ = (b*locations + i)*(1+l.coords+l.classes);
                ਪੂਰਨ is_obj = net.truth[truth_ਸੂਚਕਾਂਕ];
                ਲਈ (j = 0; j < l.n; ++j) {
                    ਪੂਰਨ p_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ + locations*l.classes + i*l.n + j;
                    l.delta[p_ਸੂਚਕਾਂਕ] = l.noobject_scale*(0 - l.output[p_ਸੂਚਕਾਂਕ]);
                    *(l.ਲਾਗਤ_2) += l.noobject_scale*ਘਾਤ(l.output[p_ਸੂਚਕਾਂਕ], 2);
                    avg_anyobj += l.output[p_ਸੂਚਕਾਂਕ];
                }

                ਪੂਰਨ ਉੱਤਮ_ਸੂਚਕਾਂਕ = -1;
                ਦਸ਼ਮਲਵ ਉੱਤਮ_iou = 0;
                ਦਸ਼ਮਲਵ ਉੱਤਮ_rmse = 20;

                ਜੇ (!is_obj){
                    ਜਾਰੀ;
                }

                ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ + i*l.classes;
                ਲਈ(j = 0; j < l.classes; ++j) {
                    l.delta[class_ਸੂਚਕਾਂਕ+j] = l.class_scale * (net.truth[truth_ਸੂਚਕਾਂਕ+1+j] - l.output[class_ਸੂਚਕਾਂਕ+j]);
                    *(l.ਲਾਗਤ_2) += l.class_scale * ਘਾਤ(net.truth[truth_ਸੂਚਕਾਂਕ+1+j] - l.output[class_ਸੂਚਕਾਂਕ+j], 2);
                    ਜੇ(net.truth[truth_ਸੂਚਕਾਂਕ + 1 + j]) avg_cat += l.output[class_ਸੂਚਕਾਂਕ+j];
                    avg_allcat += l.output[class_ਸੂਚਕਾਂਕ+j];
                }

                box truth = float_ਮੰਜ਼ਿਲ_box(net.truth + truth_ਸੂਚਕਾਂਕ + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                ਲਈ(j = 0; j < l.n; ++j){
                    ਪੂਰਨ box_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box ਬਾਹਰਲਾ = float_ਮੰਜ਼ਿਲ_box(l.output + box_ਸੂਚਕਾਂਕ, 1);
                    ਬਾਹਰਲਾ.x /= l.side;
                    ਬਾਹਰਲਾ.y /= l.side;

                    ਜੇ (l.ਵਰਗਮੂਲ){
                        ਬਾਹਰਲਾ.w = ਬਾਹਰਲਾ.w*ਬਾਹਰਲਾ.w;
                        ਬਾਹਰਲਾ.h = ਬਾਹਰਲਾ.h*ਬਾਹਰਲਾ.h;
                    }

                    ਦਸ਼ਮਲਵ iou  = box_iou(ਬਾਹਰਲਾ, truth);
                    //iou = 0;
                    ਦਸ਼ਮਲਵ rmse = box_rmse(ਬਾਹਰਲਾ, truth);
                    ਜੇ(ਉੱਤਮ_iou > 0 || iou > 0){
                        ਜੇ(iou > ਉੱਤਮ_iou){
                            ਉੱਤਮ_iou = iou;
                            ਉੱਤਮ_ਸੂਚਕਾਂਕ = j;
                        }
                    }ਨਹੀਂਤਾਂ{
                        ਜੇ(rmse < ਉੱਤਮ_rmse){
                            ਉੱਤਮ_rmse = rmse;
                            ਉੱਤਮ_ਸੂਚਕਾਂਕ = j;
                        }
                    }
                }

                ਜੇ(l.forced){
                    ਜੇ(truth.w*truth.h < .1){
                        ਉੱਤਮ_ਸੂਚਕਾਂਕ = 1;
                    }ਨਹੀਂਤਾਂ{
                        ਉੱਤਮ_ਸੂਚਕਾਂਕ = 0;
                    }
                }
                ਜੇ(l.random && *(net.ਵੇਖਿਆ) < 64000){
                    ਉੱਤਮ_ਸੂਚਕਾਂਕ = rand()%l.n;
                }

                ਪੂਰਨ box_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ + locations*(l.classes + l.n) + (i*l.n + ਉੱਤਮ_ਸੂਚਕਾਂਕ) * l.coords;
                ਪੂਰਨ tbox_ਸੂਚਕਾਂਕ = truth_ਸੂਚਕਾਂਕ + 1 + l.classes;

                box ਬਾਹਰਲਾ = float_ਮੰਜ਼ਿਲ_box(l.output + box_ਸੂਚਕਾਂਕ, 1);
                ਬਾਹਰਲਾ.x /= l.side;
                ਬਾਹਰਲਾ.y /= l.side;
                ਜੇ (l.ਵਰਗਮੂਲ) {
                    ਬਾਹਰਲਾ.w = ਬਾਹਰਲਾ.w*ਬਾਹਰਲਾ.w;
                    ਬਾਹਰਲਾ.h = ਬਾਹਰਲਾ.h*ਬਾਹਰਲਾ.h;
                }
                ਦਸ਼ਮਲਵ iou  = box_iou(ਬਾਹਰਲਾ, truth);

                //printf("%d,", best_index);
                ਪੂਰਨ p_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ + locations*l.classes + i*l.n + ਉੱਤਮ_ਸੂਚਕਾਂਕ;
                *(l.ਲਾਗਤ_2) -= l.noobject_scale * ਘਾਤ(l.output[p_ਸੂਚਕਾਂਕ], 2);
                *(l.ਲਾਗਤ_2) += l.object_scale * ਘਾਤ(1-l.output[p_ਸੂਚਕਾਂਕ], 2);
                avg_obj += l.output[p_ਸੂਚਕਾਂਕ];
                l.delta[p_ਸੂਚਕਾਂਕ] = l.object_scale * (1.-l.output[p_ਸੂਚਕਾਂਕ]);

                ਜੇ(l.rescore){
                    l.delta[p_ਸੂਚਕਾਂਕ] = l.object_scale * (iou - l.output[p_ਸੂਚਕਾਂਕ]);
                }

                l.delta[box_ਸੂਚਕਾਂਕ+0] = l.coord_scale*(net.truth[tbox_ਸੂਚਕਾਂਕ + 0] - l.output[box_ਸੂਚਕਾਂਕ + 0]);
                l.delta[box_ਸੂਚਕਾਂਕ+1] = l.coord_scale*(net.truth[tbox_ਸੂਚਕਾਂਕ + 1] - l.output[box_ਸੂਚਕਾਂਕ + 1]);
                l.delta[box_ਸੂਚਕਾਂਕ+2] = l.coord_scale*(net.truth[tbox_ਸੂਚਕਾਂਕ + 2] - l.output[box_ਸੂਚਕਾਂਕ + 2]);
                l.delta[box_ਸੂਚਕਾਂਕ+3] = l.coord_scale*(net.truth[tbox_ਸੂਚਕਾਂਕ + 3] - l.output[box_ਸੂਚਕਾਂਕ + 3]);
                ਜੇ(l.ਵਰਗਮੂਲ){
                    l.delta[box_ਸੂਚਕਾਂਕ+2] = l.coord_scale*(ਵਰਗਮੂਲ(net.truth[tbox_ਸੂਚਕਾਂਕ + 2]) - l.output[box_ਸੂਚਕਾਂਕ + 2]);
                    l.delta[box_ਸੂਚਕਾਂਕ+3] = l.coord_scale*(ਵਰਗਮੂਲ(net.truth[tbox_ਸੂਚਕਾਂਕ + 3]) - l.output[box_ਸੂਚਕਾਂਕ + 3]);
                }

                *(l.ਲਾਗਤ_2) += ਘਾਤ(1-iou, 2);
                avg_iou += iou;
                ++ਗਿਣਤੀ_2;
            }
        }

        ਜੇ(0){
            ਦਸ਼ਮਲਵ *costs = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*locations*l.n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
            ਲਈ (b = 0; b < l.batch; ++b) {
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.inputs;
                ਲਈ (i = 0; i < locations; ++i) {
                    ਲਈ (j = 0; j < l.n; ++j) {
                        ਪੂਰਨ p_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_ਸੂਚਕਾਂਕ]*l.delta[p_ਸੂਚਕਾਂਕ];
                    }
                }
            }
            ਪੂਰਨ indexes[100];
            ਸਿਖਰ_k(costs, l.batch*locations*l.n, 100, indexes);
            ਦਸ਼ਮਲਵ cutoff = costs[indexes[99]];
            ਲਈ (b = 0; b < l.batch; ++b) {
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.inputs;
                ਲਈ (i = 0; i < locations; ++i) {
                    ਲਈ (j = 0; j < l.n; ++j) {
                        ਪੂਰਨ p_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ + locations*l.classes + i*l.n + j;
                        ਜੇ (l.delta[p_ਸੂਚਕਾਂਕ]*l.delta[p_ਸੂਚਕਾਂਕ] < cutoff) l.delta[p_ਸੂਚਕਾਂਕ] = 0;
                    }
                }
            }
            ਮੁਕਤ(costs);
        }


        *(l.ਲਾਗਤ_2) = ਘਾਤ(mag_array(l.delta, l.outputs * l.batch), 2);


        ਲਿਖੋ("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/ਗਿਣਤੀ_2, avg_cat/ਗਿਣਤੀ_2, avg_allcat/(ਗਿਣਤੀ_2*l.classes), avg_obj/ਗਿਣਤੀ_2, avg_anyobj/(l.batch*locations*l.n), ਗਿਣਤੀ_2);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

ਖਾਲੀ backward_detection_layer(ਅਚਲ detection_layer l, ਜਾਲ net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ਖਾਲੀ get_detection_detections(layer l, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ thresh, detection *dets)
{
    ਪੂਰਨ i,j,n;
    ਦਸ਼ਮਲਵ *predictions = l.output;
    //int per_cell = 5*num+classes;
    ਲਈ (i = 0; i < l.side*l.side; ++i){
        ਪੂਰਨ ਪੰਕਤੀ = i / l.side;
        ਪੂਰਨ ਖਾਨਾ = i % l.side;
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = i*l.n + n;
            ਪੂਰਨ p_ਸੂਚਕਾਂਕ = l.side*l.side*l.classes + i*l.n + n;
            ਦਸ਼ਮਲਵ scale = predictions[p_ਸੂਚਕਾਂਕ];
            ਪੂਰਨ box_ਸੂਚਕਾਂਕ = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_ਸੂਚਕਾਂਕ + 0] + ਖਾਨਾ) / l.side * w;
            b.y = (predictions[box_ਸੂਚਕਾਂਕ + 1] + ਪੰਕਤੀ) / l.side * h;
            b.w = ਘਾਤ(predictions[box_ਸੂਚਕਾਂਕ + 2], (l.ਵਰਗਮੂਲ?2:1)) * w;
            b.h = ਘਾਤ(predictions[box_ਸੂਚਕਾਂਕ + 3], (l.ਵਰਗਮੂਲ?2:1)) * h;
            dets[ਸੂਚਕਾਂਕ].bbox = b;
            dets[ਸੂਚਕਾਂਕ].objectness = scale;
            ਲਈ(j = 0; j < l.classes; ++j){
                ਪੂਰਨ class_ਸੂਚਕਾਂਕ = i*l.classes;
                ਦਸ਼ਮਲਵ prob = scale*predictions[class_ਸੂਚਕਾਂਕ+j];
                dets[ਸੂਚਕਾਂਕ].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_detection_layer_gpu(ਅਚਲ detection_layer l, ਜਾਲ net)
{
    ਜੇ(!net.train){
        copy_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
        ਵਾਪਸ;
    }

    cuda_pull_array(net.ਨਿਵੇਸ਼_gpu, net.ਨਿਵੇਸ਼, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ਧੱਕੋ_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ਧੱਕੋ_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ਖਾਲੀ backward_detection_layer_gpu(detection_layer l, ਜਾਲ net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ

