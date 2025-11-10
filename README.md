# AdaptClean
Removes residual adapter sequences from the ends of single-end or paired-end unmerged FASTQ reads
that may remain after trimming with tools like fastp. The program targets only previously untrimmed
reads (i.e., reads with full sequencing cycle length), scans the bases at the 3' end for adapter
sequences, and trims them even if only a single base matches the adapter.

## Requirements
- A C++ compiler with C++17 support
- zlib library

## Installation

Run the following commands:

```bash
git clone https://github.com/bilalbioinfo/AdaptClean.git
cd AdaptClean
make
```
By default `make` will compile the program and install the `AdaptClean` binary to `/bin`.

## Usage

```
AdaptClean <input.fastq[.gz]> <output.fastq[.gz]> <sequencing_cycle_length> <adapter_sequence>
```

Example:

```bash
AdaptClean reads.fastq.gz reads.trimmed.fastq.gz 75 AGATC
```

Notes:
- Input is treated as gzipped only if the filename ends with `.gz`.
- Trimming is applied only to reads whose length equals the given sequencing cycle.


Contact: Bilal Sharif <bilal.bioinfo@gmail.com>

