# What is it?
This is a library that works with the stack. It can create stack (stack_construct), add elements to it (stack_push), remove elements from the end (stack_pop), cleare the stack (stack_clear), get last element (stack_back()), delete stack (stack_destruct()).

# How do I start this?
Clone the stack.cpp and stack.h to a folder. Write at the beginning of the main.cpp #include "stack.h". Run from command line. There are 2 options for the development of events:
1. If you want to view the help, write "-h"
2. If you want to work with stack, write "-s"
(tests are not done in this work)

# Does it have vulnerabilities?
In a way, yes. Firstly, canary protection made (you can enable it by uncommenting #define IF_CANARY_PROTECT(code) code in stack.h). Also I did stack hashing (you can enable it by uncommenting #define IF_HASH_PROTECT(code) code in stack.h). Remember of uncomment #define IF_DEBUG(code) code (it will enable diagnostic of stack).

# Your stack doens't work, I broke it!
Yes, it could be happen. At least one error - function get_hash doesn't protect (any person can change hash of the stack and come out a winner!). In principle, I can make a full copy of stack, but copies also need to be protected somehow :)
