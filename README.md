# multithread_randomizer
<b>Produces random integer (0-255) based on thread scheduling.</b>
<br>
<br>
This program creates a number of peer threads.<br>
Each thread is responsible for one bit in a (global) 8-bit integer.<br>
Every time a thread is scheduled by the kernel, it flips that specific bit.<br>
<br>
Example: Every time thread number 4 is active, it will flip bit number 4.<br>
<br>
The program uses a a mutex a lock and a condition variable to ensure<br>
that one thread can not make more than one bit-flip in a row, without letting<br>
at least one other tread make a bit-flip.<br>
<br>
After 200 operations the threads terminate, and the number is printed to standard output.<br>
<br>
It's an 8-bit number, so there are 8 bits that are flipped.<br>
But the program creates 9 peer threads.<br>
The 9'th thread doesn't flip any bit, but it's presence makes sure, that there are not <br>
always an even number (200) bit-flips.<br>
