# How PyPearl Works
This file serves as a discussion both of the higher level layout of PyPearl to make reverse engineering easier, and more importantly a discussion of cool things I did while making PyPearl.

## Activation Functions
Activation functions were originally formed as a complex abstract class structure. This was inefficient and it had some other poorly optimized components. After getting addicted to Casey Muratori (look him up), I decided to convert the entire activation system to 4 functions and a struct. There is an unsigned 8 bit integer that represents what the activations decision actually is and branching logic to pick the math within a forward and backward function. There is a function to clear logits, and a function to update parameters like PReLU. This simplified structure removes any under the hood classes. To put these classes into Python they are all completely identical, except the struct gets initialized slightly differently, creating different activation functions when ran. This also isn't a 1:1 mapping. There are cases, like linear activations where a linear function without a b has a separate branch than a linear with a m and a b, and a linear with m = 1, b = 0 is different from both of these. It actualy isn't even a function. Consider the case where a Leaky ReLU is given alpha = 0.0. This is an unnecessary inefficiency with multiplication and it can be treated as a ReLU. So you can have cases where two objects of different classes are identical under the hood, and two objects of the same class can be as different as different classes under the hood. Read arbitrary activations file to learn about this.

## Loss
To be upgraded, kinda bad rn.

## Layers
To be upgraded, kinda bad rn.

## Models
To be upgraded, kinda bad rn.