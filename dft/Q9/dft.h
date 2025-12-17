#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef float DTYPE;
#define SIZE 1024  /* SIZE OF DFT */
typedef hls::axis<DTYPE, 0, 0, 0> transPkt;
// Use streaming interface for demo
void dft(
    hls::stream<transPkt> &real_sample_stream,
    hls::stream<transPkt> &imag_sample_stream,
    hls::stream<transPkt> &real_op_stream,
    hls::stream<transPkt> &imag_op_stream
);
