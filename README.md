# multithread_randomizer
Produces random integer (0-255) based on thread scheduling

This program creates a number of threads.
Each thread is responsible for one bit in an 8-bit integer.
Every time a thread is scheduled by the kernel, it flips its bit.

Example: Every time thread number 4 is active, it will flip bit number 4.

The program uses a a mutex a lock and a condition variable to ensure
that one thread can not make more than one bit-flip in a row, without letting
at least one other tread make a bit flip.

After 200 operations the threads terminate, and the number is printed to standard output.

It's an 8-bit number, so there are 8 bits that are flipped.
But the program creates 9 peer threads.
The 9'th thread doesn't flip any bit, but it's presence makes sure, that there are not 
always an even number (200) bit-flips.
