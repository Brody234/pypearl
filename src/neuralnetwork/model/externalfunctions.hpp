#ifndef MODELEXTERNALHPP
#define MODELEXTERNALHPP
#include "model.hpp"
#include "../layer/layer.hpp"

template <typename NumType>
void breedModels(Model<NumType> model1, Model<NumType> model2);

template <typename NumType>
Model<NumType>* copyModel(Model<NumType>& models);

#include "externalfunctions.tpp"
#endif