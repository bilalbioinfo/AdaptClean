#include <iostream>
#include <fstream>
#include <string>
#include <zlib.h>
#include <sstream>
#include <vector>
#include <cstring>
using namespace std;

/*
* adapter_cleanup.cpp
*
* Author: Bilal Sharif
* Email: bilal.bioinfo@gmail.com
*
* Description: This program removes the leftover adapter sequences from the end of reads after trimming
* using other programs like fastp for single-end FASTQ files. It checks for the presence of adapter sequences
* at the end of reads of a specified cycle length and trims them off if found.
*/


// Function to find the longest match of adapter at the end of the read
int adapter_match_length(const string &read, const string &adapter) {
    int max_match = 0;
    int read_len = read.size();
    int adapter_len = adapter.size();
    for (int i = 1; i <= adapter_len; ++i) {
        if (read.compare(read_len - i, i, adapter, 0, i) == 0) {
            max_match = i;
        }
    }
    return max_match;
}

// Function to read next FASTQ record from gzipped file
bool read_fastq_gz(gzFile fp, string &header, string &seq, string &plus, string &qual) {
    char buffer[262144]; // 256KB buffer
    if (gzgets(fp, buffer, sizeof(buffer)) == Z_NULL) return false;
    header = string(buffer);
    if (gzgets(fp, buffer, sizeof(buffer)) == Z_NULL) return false;
    seq = string(buffer);
    if (gzgets(fp, buffer, sizeof(buffer)) == Z_NULL) return false;
    plus = string(buffer);
    if (gzgets(fp, buffer, sizeof(buffer)) == Z_NULL) return false;
    qual = string(buffer);

    // Remove trailing newline only if it exists
    if (header.back() == '\n') header.pop_back();
    if (seq.back() == '\n') seq.pop_back();
    if (plus.back() == '\n') plus.pop_back();
    if (qual.back() == '\n') qual.pop_back();
    return true;
}

// Function to read next FASTQ record from plain file
bool read_fastq_plain(ifstream &in, string &h, string &s, string &p, string &q) {
    if (!getline(in, h)) return false;
    getline(in, s);
    getline(in, p);
    getline(in, q);
    return true;
}

// Write FASTQ record to gzipped file
void write_fastq_gz(gzFile fp, const string &h, const string &s, const string &p, const string &q) {
    gzprintf(fp, "%s\n%s\n%s\n%s\n", h.c_str(), s.c_str(), p.c_str(), q.c_str());
}

// Write FASTQ record to plain file
void write_fastq_plain(ofstream &out, const string &h, const string &s, const string &p, const string &q) {
    out << h << "\n" << s << "\n" << p << "\n" << q << "\n";
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <input.fastq[.gz]> <output.fastq[.gz]> <sequencing_cycle> <adapter_seq>\n";
        return 1;
    }

    string in_file = argv[1];
    string out_file = argv[2];
    int cycle_len = stoi(argv[3]);
    string adapter = argv[4];

    bool input_gz = in_file.substr(in_file.size() - 3) == ".gz";

    if (input_gz) {
        gzFile in_fp = gzopen(in_file.c_str(), "rb");
        if (!in_fp) {
            cerr << "Error: Cannot open input file " << in_file << "\n";
            return 1;
        }

        gzFile out_fp = gzopen(out_file.c_str(), "wb");
        if (!out_fp) {
            cerr << "Error: Cannot open output file " << out_file << "\n";
            gzclose(in_fp);
            return 1;
        }

        string h, s, p, q;
        while (read_fastq_gz(in_fp, h, s, p, q)) {
            if ((int)s.size() == cycle_len) {
                int match = adapter_match_length(s, adapter);
                if (match > 0) {
                    s = s.substr(0, s.size() - match);
                    q = q.substr(0, q.size() - match);
                }
            }
            write_fastq_gz(out_fp, h, s, p, q);
        }
        gzclose(in_fp);
        gzclose(out_fp);

    } else {
        ifstream in_plain(in_file);
        if (!in_plain.is_open()) {
            cerr << "Error: Cannot open input file " << in_file << "\n";
            return 1;
        }

        ofstream out_plain(out_file);
        if (!out_plain.is_open()) {
            cerr << "Error: Cannot open output file " << out_file << "\n";
            in_plain.close();
            return 1;
        }

        string h, s, p, q;
        while (read_fastq_plain(in_plain, h, s, p, q)) {
            if ((int)s.size() == cycle_len) {
                int match = adapter_match_length(s, adapter);
                if (match > 0) {
                    s = s.substr(0, s.size() - match);
                    q = q.substr(0, q.size() - match);
                }
            }
            write_fastq_plain(out_plain, h, s, p, q);
        }
        in_plain.close();
        out_plain.close();
    }
    return 0;
}
