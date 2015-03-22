### This is a header-only library which provides a C++ STL-like implementation of a tree data structure ###

The design encourages simplicity of use and extensive use of STL algorithms and streams.

This is a header only library, so in order to use it the only requirement is to include the header file in your source file:
```
#include <tree.hpp>
```

Two main classes are provided:
  * **`std::tree`**: the tree container class.
  * **`std::node`**: the node structure that contains the actual data.