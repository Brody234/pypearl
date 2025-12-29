# Abstract Algebra

This sublibrary has nothing to do with machine learning. It is in PyPearl 1 because I want to use nd arrays, 2 because I want to follow through with it. It will likely go on my resume as a different project and by all accounts is it's own thing. Idk maybe I'll tie it back to ML eventually. I don't know how that would work but I kinda want to make it. Like a dihedral classifier.

## Rules Of Usage

The following defines behavior, both for the understanding of users and the direction of contributors. These rules are forced by the library, so users don't need to keep track of them, but they are good to understand the reasoning for various seemingly annoying behaviors. Group elements are not to be altered directly, in the sense that they will not recieve absolute assignments to their raw data. All assignments, changes, etc. must go through a function that forces the change to obey the rules of the group. For example, no one can ever set dihedral. s = 34, as s is either 0 or 1. For the sake of optimization, redudant assignments are assumed to not exist. Therefore, s = 34, or s^34, which is the same as saying s^0 may be treated as s^1 as s!= 0.