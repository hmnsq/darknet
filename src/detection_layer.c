#അന്തര്ഭാവ "detection_layer.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "softmax_layer.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "box.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>

detection_layer make_detection_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ side, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ coords, പൂർണ്ണസംഖ്യ rescore)
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
    പ്രതിജ്ഞാ(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = calloc(1, വലുപ്പം(ദശാംശം));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));
    l.delta = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#ifdef GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    തിരികെ l;
}

ശൂന്യം forward_detection_layer(സ്ഥിരം detection_layer l, network net)
{
    പൂർണ്ണസംഖ്യ locations = l.side*l.side;
    പൂർണ്ണസംഖ്യ i,j;
    memcpy(l.output, net.നിവേശം, l.outputs*l.batch*വലുപ്പം(ദശാംശം));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    പൂർണ്ണസംഖ്യ b;
    എങ്കിൽ (l.softmax){
        വേണ്ടി(b = 0; b < l.batch; ++b){
            പൂർണ്ണസംഖ്യ index = b*l.inputs;
            വേണ്ടി (i = 0; i < locations; ++i) {
                പൂർണ്ണസംഖ്യ offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    എങ്കിൽ(net.train){
        ദശാംശം avg_iou = 0;
        ദശാംശം avg_cat = 0;
        ദശാംശം avg_allcat = 0;
        ദശാംശം avg_obj = 0;
        ദശാംശം avg_anyobj = 0;
        പൂർണ്ണസംഖ്യ എണ്ണം = 0;
        *(l.cost) = 0;
        പൂർണ്ണസംഖ്യ size = l.inputs * l.batch;
        memset(l.delta, 0, size * വലുപ്പം(ദശാംശം));
        വേണ്ടി (b = 0; b < l.batch; ++b){
            പൂർണ്ണസംഖ്യ index = b*l.inputs;
            വേണ്ടി (i = 0; i < locations; ++i) {
                പൂർണ്ണസംഖ്യ truth_index = (b*locations + i)*(1+l.coords+l.classes);
                പൂർണ്ണസംഖ്യ is_obj = net.truth[truth_index];
                വേണ്ടി (j = 0; j < l.n; ++j) {
                    പൂർണ്ണസംഖ്യ p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                പൂർണ്ണസംഖ്യ best_index = -1;
                ദശാംശം best_iou = 0;
                ദശാംശം best_rmse = 20;

                എങ്കിൽ (!is_obj){
                    തുടരുക;
                }

                പൂർണ്ണസംഖ്യ class_index = index + i*l.classes;
                വേണ്ടി(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    എങ്കിൽ(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                വേണ്ടി(j = 0; j < l.n; ++j){
                    പൂർണ്ണസംഖ്യ box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    എങ്കിൽ (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    ദശാംശം iou  = box_iou(out, truth);
                    //iou = 0;
                    ദശാംശം rmse = box_rmse(out, truth);
                    എങ്കിൽ(best_iou > 0 || iou > 0){
                        എങ്കിൽ(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }അല്ലെങ്കിൽ{
                        എങ്കിൽ(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                എങ്കിൽ(l.forced){
                    എങ്കിൽ(truth.w*truth.h < .1){
                        best_index = 1;
                    }അല്ലെങ്കിൽ{
                        best_index = 0;
                    }
                }
                എങ്കിൽ(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                പൂർണ്ണസംഖ്യ box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                പൂർണ്ണസംഖ്യ tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                എങ്കിൽ (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                ദശാംശം iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                പൂർണ്ണസംഖ്യ p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                എങ്കിൽ(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                എങ്കിൽ(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++എണ്ണം;
            }
        }

        എങ്കിൽ(0){
            ദശാംശം *costs = calloc(l.batch*locations*l.n, വലുപ്പം(ദശാംശം));
            വേണ്ടി (b = 0; b < l.batch; ++b) {
                പൂർണ്ണസംഖ്യ index = b*l.inputs;
                വേണ്ടി (i = 0; i < locations; ++i) {
                    വേണ്ടി (j = 0; j < l.n; ++j) {
                        പൂർണ്ണസംഖ്യ p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            പൂർണ്ണസംഖ്യ indexes[100];
            മുകൾ_k(costs, l.batch*locations*l.n, 100, indexes);
            ദശാംശം cutoff = costs[indexes[99]];
            വേണ്ടി (b = 0; b < l.batch; ++b) {
                പൂർണ്ണസംഖ്യ index = b*l.inputs;
                വേണ്ടി (i = 0; i < locations; ++i) {
                    വേണ്ടി (j = 0; j < l.n; ++j) {
                        പൂർണ്ണസംഖ്യ p_index = index + locations*l.classes + i*l.n + j;
                        എങ്കിൽ (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            സ്വതന്ത്രം(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/എണ്ണം, avg_cat/എണ്ണം, avg_allcat/(എണ്ണം*l.classes), avg_obj/എണ്ണം, avg_anyobj/(l.batch*locations*l.n), എണ്ണം);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

ശൂന്യം backward_detection_layer(സ്ഥിരം detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ശൂന്യം get_detection_detections(layer l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം thresh, detection *dets)
{
    പൂർണ്ണസംഖ്യ i,j,n;
    ദശാംശം *predictions = l.output;
    //int per_cell = 5*num+classes;
    വേണ്ടി (i = 0; i < l.side*l.side; ++i){
        പൂർണ്ണസംഖ്യ row = i / l.side;
        പൂർണ്ണസംഖ്യ col = i % l.side;
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ index = i*l.n + n;
            പൂർണ്ണസംഖ്യ p_index = l.side*l.side*l.classes + i*l.n + n;
            ദശാംശം scale = predictions[p_index];
            പൂർണ്ണസംഖ്യ box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            വേണ്ടി(j = 0; j < l.classes; ++j){
                പൂർണ്ണസംഖ്യ class_index = i*l.classes;
                ദശാംശം prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

ശൂന്യം forward_detection_layer_gpu(സ്ഥിരം detection_layer l, network net)
{
    എങ്കിൽ(!net.train){
        copy_gpu(l.batch*l.inputs, net.നിവേശം_gpu, 1, l.output_gpu, 1);
        തിരികെ;
    }

    cuda_pull_array(net.നിവേശം_gpu, net.നിവേശം, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_തള്ളുക_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_തള്ളുക_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ശൂന്യം backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

