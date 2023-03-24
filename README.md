# rustyweightbench
 A no-fills, quick C util to test system write performance. Don't stare directly at it. 

 ## Usage

To use the script, change to the `src`directory.

Then compile it with the following command:

`gcc -o rwb rwb.c`

Then run it with the write size and number of writes to test as arguments:

`./rwb <write_size> <num_writes>`

For example:

`./rwb 4096 1000`

This would write 1000 blocks of 4096 bytes each to disk and output the time taken, system time taken, min load, max load, and I/O stats.












