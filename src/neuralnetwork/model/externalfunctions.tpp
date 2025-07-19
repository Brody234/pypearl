#ifndef MODELEXTERNAL
#define MODELEXTERNAL

#include "externalfunctions.hpp"

template <typename NumType>
void breedModels(Model<NumType> model1, Model<NumType> model2){
    
}

template <typename NumType>
Model<NumType>* copyModel(Model<NumType>& model){
    Model<NumType> *cmodel = new Model<NumType>();
    size_t max = model.layers.size();
    for(size_t i = 0; i < max; i++){
        Layer<NumType>* layer = new Layer<NumType>(model.layers[i]->weights.shape[0], model.layers[i]->weights.shape[1], false);
        layer->deepcopy(model.layers[i]);
        cmodel->addLayer((*layer));
    }
    size_t maxa = model.activations.size();
    for(size_t i = 0; i < maxa; i++){
        cmodel->addActivationByByte(model.activations[i]->type);
    }

    return cmodel;
}


#endif