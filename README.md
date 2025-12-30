# PyPearl

PyPearl is a library that is designed for a variety of data science and mathematics applications. The primary function is machine learning, particularly neural networks. The library also supports arbitrary length, dimension and datatype ndarrays with support functions, and abstract algebra. PyPearl places an emphasis on efficiency and optimization, especially on computationally intense tasks like neural network training.

## About

My name is Brody and as of now I am the only maintainer of PyPearl. I plan to eventually make it into a community effort, but for now, I am using this library as a way to reinforce machine learning and math principles for my own learning purposes.

PyPearl is built out of a series of "sub libraries", each of which could be their own small library. I will describe these at a high level here, refer to the docs folder in the source code for deeper explanations.

Source code: https://github.com/Brody234/pypearl
PyPi: https://pypi.org/project/pypearl/ 

## ndarray

This can be found in the matrix folder. The ndarray is an object that is used for almost everything in this library. It is meant to mimick as much of numpy's functionality as possible. It is built on a char array to make byte size building blocks, allocating one flat block of memory, and then a shape and strides array reinterprets this into an ndarray. Views can be created without copying the data. Features that are in numpy (or that aren't) are implemented as I need them, though the goal is to make something that is stronger and more efficient.

## Neural Networks

This section supports neural network models. It is the my favorite part of the library. Currently supports models with the following architectures for gradient descent.

Layers:
- Dense

Activation Functions:
- ReLU
- Softmax
- Leaky ReLU
- Linear (mx+b)
- Sigmoid
- Step
- Reverse ReLU

Loss Functions:
- CCE

Optimizers:
- Gradient Descent

## Abstract Algebra

This section of the library is devoted to abstract algebra. For someone who does not know abstract algebra, think about it like this. Imagine you took a regular polygon of n dimensions. You can rotate it n different ways and flip it. What if each of those symmetries was a number? If that doesn't make sense, I'm not great at it either, so I'd suggest learning it elsewhere. Note that this is not super useful for ML or DS, but helps you think a lot.

This sublibrarys gives access to the following groups:
- Dihedral Group: The symmetries of an arbitrary n gon.
- Z/nZ Group: The equivalence classes of the integers divided by a factor.
- Symmetric: The group of bijections over a fixed length set.
- Dihedral Tensor: Dihedral groups of a fixed n, put into a tensor under addition. Basically just a direct product over a bunch of dihedral groups.
- Direct Product: Any arbitrary direct product between any other groups in this library.

## PyPearl Status

PyPearl is currently in development and not ready for industry use.

PyPearl can handle most basic neural network use cases, and is somewhat well optimized for small models. In small cases (2 hidden layers, 60 neurons, 900 samples) it out performs PyTorch due to it's nature as a thinner wrapper compared to PyTorch's 60% Python.

This description was last edited December 29, 2025.