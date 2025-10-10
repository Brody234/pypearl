# How To Use PyPearl
This document focuses on the Python implementation of PyPearl, not on how PyPearl actually works. Be warned what is exposed at the surface level can be misleading.

# Activation Functions
Activation functions ending in 64 currently work for 64 bit values. As of now, activation functions with no size ending do the same thing  and will be deprecated soon.
- ReLU(minimum): minimum defaults to 0
- LeakyReLU(minimum, alpha): minimum defaults to 0, alpha defaults to 0
- 