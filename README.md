# multithread_randomizer

Produces a random integer (0-255) based on thread scheduling.

## Build and run

```bash
make
make run
```

## Requirements

- GCC (or another compatible C compiler)
- pthread library
- A Unix-like operating system

## Program logic

This program creates a number of threads.<br>
Each thread is responsible for one bit in a 8-bit integer, and <br>
every time a thread is scheduled by the kernel, it flips that specific bit.<br>
<br>
_Example_:<br>
If the 8-bit integer at some point is 00000000 and thread number 4 becomes<br>
active, it will flip bit number 4 and change the integer to 00001000<br>
<br>
The program uses a mutex a lock and a condition variable to ensure<br>
that one thread can not make more than one bit-flip in a row, without letting<br>
at least one other thread make a bit-flip.<br>
<br>
After 200 operations the threads terminate, and the final (random) number<br>is printed to standard output.<br>
<br>
It's an 8-bit number, so there are 8 bits that are flipped.<br>
But the program creates 9 peer threads. The 9'th thread doesn't flip any bit,<br>
but it's presence is necessary to make sure, that there are not always an<br>
even number (200) bit-flips.
