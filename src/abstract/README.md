# Abstract Algebra

This sublibrary has nothing to do with machine learning. It is in PyPearl 1 because I want to use nd arrays, 2 because I want to follow through with it. It will likely go on my resume as a different project and by all accounts is it's own thing. Idk maybe I'll tie it back to ML eventually. I don't know how that would work but I kinda want to make it. Like a dihedral classifier.

I will briefly explain groups here to make it clear what I am talking about as I am not good at abstract algebra and want any math mistakes I make to be seen in the docs, but if you are also not good at abstract algebra, please refer to other resources.

While + is conventionally reserved for abelian groups, I want to save * for multiplications in rings and fields, so binary operations between 2 group elements are always + and @ is for group actions. I figured it would be simpler for users, and easier to implement complex functions if everything was by default + on it's base group operation.

## Z/nZ

Z/nZ is the equivalence classes under modular arithmetic of the integers, divided by the integers times a scalar n. For example, if n = 20, you get 0..19, with addition defined such that 0+0 = 0, 1+1=2, 10+10=0, 11+10=1. In other words, any two numbers that equal each other modulo 20 are equal in this example.

Elements in this group can be created with ZNZ(rep=rep, n=n). n is the value in Z/nZ, the order of the group. rep is any long, negative, positive, greater than n, less than n, any number. In the constructor, it is converted to an equivalence class representative between 0..n-1.

The operators +, += are defined as the group action, integer addition within the equivalence classes. Again, representatives are reduced to 0..n-1 after the addition. This is abelian.

## Dihedral

The dihedral group is the group of symmetries over a regular n gon. Elements in the dihedral group can all be represented in the normal form r^k * s^m. r represents a 2pi/n degree rotation, s represents a reflection. The group operation is defined such that (r^k * s^m) + (r^k' * s^m') = r^k*((-1)^m r^k')*s^m *s^m'.

Dihedral elements are always stored as 1 long for k, and one for m. Elements are created with Dihedral(r=r, n=n, s=s). n is 1 half the order of the group, the number of sides on the ngon. r is the rotations that this element represents, s is the reflection. This element is stored in r^k s^m form, where r would actually be k, the power of r, and s would actually be m. All operations including initialization reduce s to {0,1} and k to a number in {0..n-1} with the modular operation. s and r can be initialized to anything, however, the constructor cannot read complex strings like rsrsrsr^2r^3. If you have an arbitrary element you want to input, it must first be reduced to r^k s^m. This can be done either manually, or by creating a bunch of dihedral elements and then summing them.

The operators +, += are defined as the group action, a+b described above. Again, everything is reduced to the simplified form described above. This is not abelian.

## Symmetric

The symmetric group is the group of all bijections on a set of size n. Elements in this group can be thought of as products of disjoint cycles, products of transpostions, or new sequences. For this description, elements take the form of the new sequence they create, where each kth element, is the index it sends a sets kth element to. Because this is a computer, it is far more computationally efficient to store this on the sequence {0..n-1} rather than the standard mathematical {1..n}.

Elements are created with Symmetric(ordering = ordering, n = n). Both arguments are optional; however, one must be passed. If ordering is passed, n is ignored. Ordering is expected to be an ndarray from this library. It must have nd=1, and it will be treated as a bijection. Each index must be an element from 0 to n-1, and all elements in this range must be filled exactly once. Bijection checking code is functionally order G = order H, f: G->H is surjective \therefore bijection. If ordering is not selected, the identity element in S_n (where n is a positive long passed), is returned. 

The operators +, += are defined as the group action a+b. Function a is composed with b, to create a bijection equivalent to a(b(x)), in both a+b and a+=b. This is not abelian.

## Dihedral Tensor

Code found in dihedral_matrix.hpp. This is stored as a tensor of dihedral elements, but can be thought of as isomorphic to a dihedral vector with the same number of components. This can be thought of as isomorphic to D_n \times ... \times D_n as a direct product, with number of components (\Pi dims[i]) many D_n terms, n is constant.

Elements are created with DihedralTensor(r=r, s=s, n=n). This is the same as Dihedral, but r and s are now ndarrays of the same shape. Otherwise, everything is exactly the same. This is just an increased efficiency version of direct products as I want to play around with Dihedral based ML models that appeared to me in a dream.

The operators +, += are defined as the group action described above. It is not abelian.

## Direct Product

Code found in directproduct.hpp. This is a group that allows the creation of an arbitrary direct product between any number of groups, allowing for the creation of massive groups.

Elements are constructed with DirectProduct((tuple of group objects)).

## Rules Of Memory and Access

The following defines behavior, both for the understanding of users and the direction of contributors. These rules are forced by the library, so users don't need to keep track of them, but they are good to understand the reasoning for various seemingly annoying behaviors. Group elements are not to be altered directly, in the sense that they will not recieve absolute assignments to their raw data. All assignments, changes, etc. must go through a function that forces the change to obey the rules of the group. For example, no one can ever set dihedral. s = 34, as s is either 0 or 1. For the sake of optimization, redudant assignments are assumed to not exist. Therefore, s = 34, or s^34, which is the same as saying s^0 may be treated as s^1 as s!= 0.