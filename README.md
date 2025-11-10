# AdaptClean
Removes residual adapter sequences from the ends of single-end or paired-end unmerged FASTQ reads 
that may remain after trimming with tools like fastp. The program targets only previously untrimmed 
reads (i.e., reads with full sequencing cycle length), scans the bases at the 3' end for adapter 
sequences, and trims them even if only a single base matches the adapter.
