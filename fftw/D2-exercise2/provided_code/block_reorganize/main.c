#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reorganize_blocks(double *source, double *dest, int n1, int n2, int n3, int p) {
    int block_size_n2 = n2 / p;  // Block size along n2 in source
    int block_size_n1 = n1 / p;  // Block size along n1 in dest
    
    for (int block = 0; block < p; block++) {
        for (int i1 = 0; i1 < block_size_n1; i1++) {
            for (int i2 = 0; i2 < block_size_n2; i2++) {
                for (int i3 = 0; i3 < n3; i3++) {
                    // Source index (in row-major order)
                    int src_i1 = i1 + block * block_size_n1;
                    int src_i2 = i2;
                    int src_index = src_i1 * (n2 * n3) + src_i2 * n3 + i3;

                    // Destination index
                    int dest_i1 = i1;
                    int dest_i2 = i2 + block * block_size_n2;
                    int dest_index = dest_i1 * (block_size_n2 * n3) + dest_i2 * n3 + i3;

                    dest[dest_index] = source[src_index];
                }
            }
        }
    }
}

int main() {
    int n1 = 6, n2 = 4, n3 = 3, p = 2;
    int total_size = n1 * n2 * n3;

    int block_size_n2 = n2 / p;

    double *source = malloc(total_size * sizeof(double));
    double *dest = malloc(total_size * sizeof(double));

    // Initialize source array for demonstration
    for (int i = 0; i < total_size; i++) {
        source[i] = i + 1;
    }

    // Perform the block reorganization
    reorganize_blocks(source, dest, n1, n2, n3, p);

    // Print destination buffer for verification
    printf("Destination buffer:\n");
    for (int i = 0; i < total_size; i++) {
        printf("%.1f ", dest[i]);
        if ((i + 1) % n3 == 0) printf("\n");
        if ((i + 1) % (n3 * block_size_n2) == 0) printf("\n");
    }

    free(source);
    free(dest);
    return 0;
}

