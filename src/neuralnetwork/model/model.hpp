#ifndef MODELHPP
#define MODELHPP

#include <stdio.h>
#include "../layer/layer.hpp"
#include "../activation/reluactivation.hpp"
#include "../activation/softmaxactivation.hpp"
#include <stdint.h>

typedef u_int8_t byte;
typedef u_int16_t word;
typedef u_int32_t dword32;
typedef u_int64_t qword64;



template <typename NumType = float>
class Model
{
    private:
        bool is64;
        using LayerT = Layer<NumType>;
        using ActT   = BaseActivation<NumType>;


        

    public:
        std::vector<LayerT*> layers;       
        std::vector<ActT*>   activations; 

        Model();
        ~Model();
        void addLayer(Layer<NumType>& layer);
        void addReLU(ActivationReLU<NumType>& relu);
        void addSoftmax(ActivationSoftMax<NumType>& soft);

        void addActivationByByte(byte act);

        Array<NumType, 2> forward(Array<NumType, 2> const& X);
        Array<NumType, 1> forwardRL(Array<NumType, 1> const& X);
        Array<NumType, 1> forwardGA(Array<NumType, 1> const& X);
        void randomize(NumType strength);
        
        // Model saves must have less than 5535 layers.
        void saveModel(const char *path);
        int loadModel(const char *path);

};

#include "model.tpp"

#endif