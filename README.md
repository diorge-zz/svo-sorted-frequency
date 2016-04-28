# SVO Sorted Frequency

Frequency counter on a sorted SVO{N} file

### Baseline

This code is based on the [previous näive version](https://github.com/diorge/svo-naive-frequency), but assume the SVO{N} file to be sorted line by line (therefore sorted by S lexicographically).

### Improvements

By knowing when a certain pair won't receive more counts, it's possible to simplify the data structure and not hold all
pairs in memory at the same time, reducing the collision or completely removing the need for the hash table.
