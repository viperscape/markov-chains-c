# markov chains, name generator example in C

## build instructions

```$ gcc -std=c99 chain.c main.c -o name_gen```

## example output

```
$ ./name_gen
Colissofa
```

## other info

The name builder works off of the prefix and its length (currently 3), so changing the prefix will build different results. The prefix is generated from an existing randomly chosen word and starts from a random position in the word.

