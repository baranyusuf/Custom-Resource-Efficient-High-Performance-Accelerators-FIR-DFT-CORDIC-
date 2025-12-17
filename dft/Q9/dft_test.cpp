#include <stdio.h>
#include <math.h>
#include <hls_stream.h>
#include "dft.h"

struct Rmse {
    int num_sq;
    float sum_sq;
    float error;

    Rmse() { num_sq = 0; sum_sq = 0; error = 0; }

    float add_value(float d_n) {
        num_sq++;
        sum_sq += (d_n * d_n);
        error = sqrtf(sum_sq / num_sq);
        return error;
    }
};

Rmse rmse_R, rmse_I;

int main() {
    int index;
    float gold_R, gold_I;

    hls::stream<transPkt> real_input_stream, imag_input_stream;
    hls::stream<transPkt> real_output_stream, imag_output_stream;

    FILE *fp = fopen("out.gold.dat", "r");
    if (!fp) {
        printf("ERROR: Could not open out.gold.dat\n");
        return 1;
    }

    // Write input samples to AXI streams
    for (int i = 0; i < SIZE; i++) {
        transPkt r_pkt, i_pkt;
        r_pkt.data = i; i_pkt.data = 0.0;
        r_pkt.keep = i_pkt.keep = -1;
        r_pkt.strb = i_pkt.strb = -1;
        r_pkt.last = i_pkt.last = (i == SIZE-1) ? 1 : 0;
        real_input_stream.write(r_pkt);
        imag_input_stream.write(i_pkt);
    }

    // Call DFT
    dft(real_input_stream, imag_input_stream, real_output_stream, imag_output_stream);

    // Compare outputs with golden reference
    for (int i = 0; i < SIZE; i++) {
        if (fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I) != 3) {
            printf("ERROR: Invalid format in golden file.\n");
            fclose(fp);
            return 1;
        }

        transPkt out_r_pkt = real_output_stream.read();
        transPkt out_i_pkt = imag_output_stream.read();
        rmse_R.add_value(out_r_pkt.data - gold_R);
        rmse_I.add_value(out_i_pkt.data - gold_I);
    }
    fclose(fp);

    printf("----------------------------------------------\n");
    printf("   RMSE(R)           RMSE(I)\n");
    printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
    printf("----------------------------------------------\n");

    if (rmse_R.error > 0.1 || rmse_I.error > 0.1) {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
        fprintf(stdout, "*******************************************\n");
        return 1;
    } else {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "PASS: The output matches the golden output!\n");
        fprintf(stdout, "*******************************************\n");
        return 0;
    }
}