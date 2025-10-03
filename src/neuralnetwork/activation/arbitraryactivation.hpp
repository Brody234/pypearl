#ifndef ArbitraryActivation_HPP
#define ArbitraryActivation_HPP

#include "../../matrix/matrix.hpp"
#include <cstdint>


template <typename NumType = float>
struct ActivationLayer {
    /*
     * Types:
     * 0x0: ReLU w/ any arbitrary minimum, uses field relmin
     * 0x1: ReLU w/ 0 as a forced minimum, uses a hardcoded 0 constant to avoid memory
     * 0x2: Softmax
     * 0x3: Leaky ReLUf
     */
    uint8_t type;
    Array<NumType,2>* saved_inputs;
    Array<NumType,2>* dinputs;
    NumType relmin;

    // Combine these to determine what gets sent upwards
    Array<NumType, 2>* outputs;
    bool outputOwnership;

    // Used in Leaky ReLU, 0 otherwise
    NumType alpha;
};


template <typename NumType = float>
Array<NumType, 2>* activationForward(Array<NumType, 2>* inputs, ActivationLayer<NumType>& layer);

template <typename NumType = float>
Array<NumType, 2>* activationBackward(Array<NumType, 2>* dvalues, ActivationLayer<NumType>& layer);


// Warning after a call logits lose access
template <typename NumType = float>
void freeActivationLogits(ActivationLayer<NumType>& layer);

#include "arbitraryactivation.tpp"
#endif
