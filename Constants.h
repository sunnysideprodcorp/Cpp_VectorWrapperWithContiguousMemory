#ifndef CONSTANTS_POLICY_H
#define CONSTANTS_POLICY_H


// These should be tuned to your use case
const double MIN_MEMORY_FRACTION = .5;

// The maximum length of an individual Small Vector
// This has been declared for future use, but no individual 
// size-limiting is enforced at present
const int MAX_SIZE = 10;

// The maximum number of SmallVectors to be stored in a single memory block
// This also is not enforced at present
const int NUM_ELEMENTS = 4;

// These variables set the max size of an individual SmallVector and
// the max size an individual memory block, assuming SmallVectors will hold ints
// This should be adjusted to suit your application
constexpr int MAX_INDIV = sizeof(int)*MAX_SIZE;
constexpr int MAX_BLOCK = NUM_ELEMENTS*MAX_INDIV;


#endif  // CONSTANTS_POLICY_H
