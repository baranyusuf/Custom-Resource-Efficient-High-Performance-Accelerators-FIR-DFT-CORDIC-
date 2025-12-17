#include <math.h>
#include "dft.h"
#include "coefficients1024.h"
#include <hls_stream.h>

void dft(
    hls::stream<transPkt> &real_sample_stream,
    hls::stream<transPkt> &imag_sample_stream,
    hls::stream<transPkt> &real_op_stream,
    hls::stream<transPkt> &imag_op_stream
)
{
    #pragma HLS INTERFACE axis port=real_sample_stream
    #pragma HLS INTERFACE axis port=imag_sample_stream
    #pragma HLS INTERFACE axis port=real_op_stream
    #pragma HLS INTERFACE axis port=imag_op_stream
    #pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS

    // --- Array Partitioning for parallelism ---
    #pragma HLS ARRAY_PARTITION variable=cos_coefficients_table block factor=16 dim=1
    #pragma HLS ARRAY_PARTITION variable=sin_coefficients_table block factor=16 dim=1

    DTYPE real_sample[SIZE];
    DTYPE imag_sample[SIZE];
    #pragma HLS ARRAY_PARTITION variable=real_sample cyclic factor=16 dim=1
    #pragma HLS ARRAY_PARTITION variable=imag_sample cyclic factor=16 dim=1

    // Read input streams into local arrays
    for (int i = 0; i < SIZE; i++) {
        #pragma HLS PIPELINE II=1
        transPkt r_pkt = real_sample_stream.read();
        transPkt i_pkt = imag_sample_stream.read();
        real_sample[i] = r_pkt.data;
        imag_sample[i] = i_pkt.data;
    }

    // Compute DFT
    for (int i = 0; i < SIZE; i++) {
        DTYPE real_sum = 0;
        DTYPE imag_sum = 0;

        for (int j = 0; j < SIZE; j++) {
            #pragma HLS PIPELINE II=1
            DTYPE c = cos_coefficients_table[(i * j) % SIZE];
            DTYPE s = sin_coefficients_table[(i * j) % SIZE];
            real_sum += real_sample[j] * c - imag_sample[j] * s;
            imag_sum += real_sample[j] * s + imag_sample[j] * c;
        }

        // Write results as AXI packets
        transPkt out_r_pkt, out_i_pkt;
        out_r_pkt.data = real_sum; 
        out_r_pkt.keep = -1; 
        out_r_pkt.strb = -1; 
        out_r_pkt.last = (i == SIZE-1) ? 1 : 0;

        out_i_pkt.data = imag_sum; 
        out_i_pkt.keep = -1; 
        out_i_pkt.strb = -1; 
        out_i_pkt.last = (i == SIZE-1) ? 1 : 0;

        real_op_stream.write(out_r_pkt);
        imag_op_stream.write(out_i_pkt);
    }
}
