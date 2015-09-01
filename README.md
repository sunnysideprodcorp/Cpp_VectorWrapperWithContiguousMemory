The SmallVector class and its associated factory method (VectorMaker.h), allocation-related classes (VectorClasses.h), and memory management policies (ArenaMemMgmtPolicies.h) were created to provide an easy-to-use std::vector container that stores vectors in contiguous memory chunks. The purpose of SmallVector is to enhance performance in applications where contiguity of values stored in std::vector will enhance performance by reducing look-up times.

To use SmallVector users must specify

1. The typename T of the values to be stored in the std::vectors
2. How large the memory blocks should be
3. The 'identity' of the memory manager for these vectors

This last parameter is a template parameter designated int ID whose sole function is to different SmallVector classes that would otherwise overlap. For example, if users have 2 different categories of std::vector<int>'s that they would like to move to SmallVectors, where each category should be contiguous but not contiguous with members of the other category, they can ensure these SmallVectors have different memory locations by giving the SmallVector a different int ID. Even if all other template parameters are identical, this will create separate memory blocks. 