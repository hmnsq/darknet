#అన్తర్భావ "box.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <math.h>
#అన్తర్భావ <stdlib.h>

పూర్ణాంకం nms_comparator(స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    దశాంశం diff = 0;
    ఒకవేళ(b.క్రమీకరించు_class >= 0){
        diff = a.prob[b.క్రమీకరించు_class] - b.prob[b.క్రమీకరించు_class];
    } లేకపోతే {
        diff = a.objectness - b.objectness;
    }
    ఒకవేళ(diff < 0) తిరిగి 1;
    లేకపోతే ఒకవేళ(diff > 0) తిరిగి -1;
    తిరిగి 0;
}

శూన్య do_nms_obj(detection *dets, పూర్ణాంకం మొత్తం, పూర్ణాంకం classes, దశాంశం thresh)
{
    పూర్ణాంకం i, j, k;
    k = మొత్తం-1;
    కొరకు(i = 0; i <= k; ++i){
        ఒకవేళ(dets[i].objectness == 0){
            detection మార్పిడి = dets[i];
            dets[i] = dets[k];
            dets[k] = మార్పిడి;
            --k;
            --i;
        }
    }
    మొత్తం = k+1;

    కొరకు(i = 0; i < మొత్తం; ++i){
        dets[i].క్రమీకరించు_class = -1;
    }

    qsort(dets, మొత్తం, పరిమాణం(detection), nms_comparator);
    కొరకు(i = 0; i < మొత్తం; ++i){
        ఒకవేళ(dets[i].objectness == 0) కొనసాగు;
        box a = dets[i].bbox;
        కొరకు(j = i+1; j < మొత్తం; ++j){
            ఒకవేళ(dets[j].objectness == 0) కొనసాగు;
            box b = dets[j].bbox;
            ఒకవేళ (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                కొరకు(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


శూన్య do_nms_క్రమీకరించు(detection *dets, పూర్ణాంకం మొత్తం, పూర్ణాంకం classes, దశాంశం thresh)
{
    పూర్ణాంకం i, j, k;
    k = మొత్తం-1;
    కొరకు(i = 0; i <= k; ++i){
        ఒకవేళ(dets[i].objectness == 0){
            detection మార్పిడి = dets[i];
            dets[i] = dets[k];
            dets[k] = మార్పిడి;
            --k;
            --i;
        }
    }
    మొత్తం = k+1;

    కొరకు(k = 0; k < classes; ++k){
        కొరకు(i = 0; i < మొత్తం; ++i){
            dets[i].క్రమీకరించు_class = k;
        }
        qsort(dets, మొత్తం, పరిమాణం(detection), nms_comparator);
        కొరకు(i = 0; i < మొత్తం; ++i){
            ఒకవేళ(dets[i].prob[k] == 0) కొనసాగు;
            box a = dets[i].bbox;
            కొరకు(j = i+1; j < మొత్తం; ++j){
                box b = dets[j].bbox;
                ఒకవేళ (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_గమ్యం_box(దశాంశం *f, పూర్ణాంకం stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    తిరిగి b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    దశాంశం l1 = a.x - a.w/2;
    దశాంశం l2 = b.x - b.w/2;
    ఒకవేళ (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    దశాంశం r1 = a.x + a.w/2;
    దశాంశం r2 = b.x + b.w/2;
    ఒకవేళ(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    ఒకవేళ (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    ఒకవేళ (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    దశాంశం t1 = a.y - a.h/2;
    దశాంశం t2 = b.y - b.h/2;
    ఒకవేళ (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    దశాంశం b1 = a.y + a.h/2;
    దశాంశం b2 = b.y + b.h/2;
    ఒకవేళ(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    ఒకవేళ (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    ఒకవేళ (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    తిరిగి d;
}

దశాంశం overlap(దశాంశం x1, దశాంశం w1, దశాంశం x2, దశాంశం w2)
{
    దశాంశం l1 = x1 - w1/2;
    దశాంశం l2 = x2 - w2/2;
    దశాంశం ఎడమ = l1 > l2 ? l1 : l2;
    దశాంశం r1 = x1 + w1/2;
    దశాంశం r2 = x2 + w2/2;
    దశాంశం కుడి = r1 < r2 ? r1 : r2;
    తిరిగి కుడి - ఎడమ;
}

దశాంశం box_intersection(box a, box b)
{
    దశాంశం w = overlap(a.x, a.w, b.x, b.w);
    దశాంశం h = overlap(a.y, a.h, b.y, b.h);
    ఒకవేళ(w < 0 || h < 0) తిరిగి 0;
    దశాంశం area = w*h;
    తిరిగి area;
}

దశాంశం box_union(box a, box b)
{
    దశాంశం i = box_intersection(a, b);
    దశాంశం u = a.w*a.h + b.w*b.h - i;
    తిరిగి u;
}

దశాంశం box_iou(box a, box b)
{
    తిరిగి box_intersection(a, b)/box_union(a, b);
}

దశాంశం box_rmse(box a, box b)
{
    తిరిగి వర్గమూలం(ఘాతం(a.x-b.x, 2) + 
                ఘాతం(a.y-b.y, 2) + 
                ఘాతం(a.w-b.w, 2) + 
                ఘాతం(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    దశాంశం w = overlap(a.x, a.w, b.x, b.w);
    దశాంశం h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    తిరిగి di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    తిరిగి du;
}


శూన్య test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    రాయి("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    దశాంశం inter =  box_union(a, b);
    దశాంశం xinter = box_union(dxa, b);
    దశాంశం yinter = box_union(dya, b);
    దశాంశం winter = box_union(dwa, b);
    దశాంశం hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    రాయి("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
శూన్య test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    రాయి("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    దశాంశం inter =  box_intersection(a, b);
    దశాంశం xinter = box_intersection(dxa, b);
    దశాంశం yinter = box_intersection(dya, b);
    దశాంశం winter = box_intersection(dwa, b);
    దశాంశం hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    రాయి("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

శూన్య test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    దశాంశం iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    రాయి("%f\n", iou);
    dbox d = diou(a, b);
    రాయి("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    దశాంశం xiou = box_iou(dxa, b);
    దశాంశం yiou = box_iou(dya, b);
    దశాంశం wiou = box_iou(dwa, b);
    దశాంశం hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    రాయి("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    దశాంశం u = box_union(a,b);
    దశాంశం i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    ఒకవేళ(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        తిరిగి dd;
    }

    dd.dx = 2*ఘాతం((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*ఘాతం((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*ఘాతం((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*ఘాతం((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    తిరిగి dd;
}


శూన్య do_nms(box *boxes, దశాంశం **probs, పూర్ణాంకం మొత్తం, పూర్ణాంకం classes, దశాంశం thresh)
{
    పూర్ణాంకం i, j, k;
    కొరకు(i = 0; i < మొత్తం; ++i){
        పూర్ణాంకం any = 0;
        కొరకు(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        ఒకవేళ(!any) {
            కొనసాగు;
        }
        కొరకు(j = i+1; j < మొత్తం; ++j){
            ఒకవేళ (box_iou(boxes[i], boxes[j]) > thresh){
                కొరకు(k = 0; k < classes; ++k){
                    ఒకవేళ (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    లేకపోతే probs[j][k] = 0;
                }
            }
        }
    }
}

box encode_box(box b, box anchor)
{
    box encode;
    encode.x = (b.x - anchor.x) / anchor.w;
    encode.y = (b.y - anchor.y) / anchor.h;
    encode.w = log2(b.w / anchor.w);
    encode.h = log2(b.h / anchor.h);
    తిరిగి encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = ఘాతం(2., b.w) * anchor.w;
    decode.h = ఘాతం(2., b.h) * anchor.h;
    తిరిగి decode;
}
