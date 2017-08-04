# rvd_gen

Generate a designer TAL effector RVD sequence from a given nucleotide sequence

VERSION 1.0 Release - 2017-Aug-03

### copyright and license

This program is copyright (c) 2017, Stephen Cohen (stephen.cohen@colostate.edu).
LICENSE.txt contains the terms of the license.

### about

This program generates an optimal TAL effector RVD sequence given a nucleotide
sequence. It uses the following RVDs for DNA-binding, utilizing the modules
from the Cermak et al. (Nucleic Acids Res. 2011;39(12):e82) kit:

    A -> NI (or NN)
    C -> HD
    G -> NK (or NN)
    T -> NG

In the case of sequences that would lead to stretches of 5 or more weak RVDs,
the program uses 'NN' for binding to 'A' or 'G' to break up the weak stretches,
based on recommendations from Streubel et al. (Nat Biotechnol. 2012;30:593-5).

### installation

Clone this distribution and type make at the command line. The binary file will
be called rvd_gen.

### usage

The tool generates RVD sequences given a nucleotide sequence. This sequence
can be a command line argument, e.g.:

~/rvd_gen> ./rvd_gen ATCGGTACTGAATGTA

or after running the program with no arguments. To force usage of the RVD 'NN',
the nucleotide symbol 'R' may be used. Lowercase and capitol letters are both
acceptable input. RVDs for unknown bases will appear as '??'.
