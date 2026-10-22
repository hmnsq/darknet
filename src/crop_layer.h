#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક crop_height, પૂર્ણાંક crop_width, પૂર્ણાંક flip, દશાંશ angle, દશાંશ saturation, દશાંશ exposure);
શૂન્ય forward_crop_layer(અચલ crop_layer l, network net);
શૂન્ય resize_crop_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h);

#ifdef GPU
શૂન્ય forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

