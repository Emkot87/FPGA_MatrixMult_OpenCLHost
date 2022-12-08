
#define lm 4
#define ln 4
#define lp 4

#define m  1<<lm
#define n  1<<ln
#define p  1<<lp

extern "C" {
void vadd(const int* A, const int* B, int* out){

#pragma HLS INTERFACE m_axi port = A offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = B offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = A bundle = control
#pragma HLS INTERFACE s_axilite port = B bundle = control
#pragma HLS INTERFACE s_axilite port = out bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control



	const int loop = m;

	int A_bram[n][m];
	int B_bram[m][p];

#pragma HLS ARRAY_PARTITION variable=A_bram cyclic factor 8 dim=2
#pragma HLS ARRAY_PARTITION variable=B_bram cyclic factor 8 dim=1



	for(int i = 0; i < n; i++){
#pragma HLS loop_tripcount min=loop max=loop
			for(int j = 0; j < m; j++){
#pragma HLS loop_tripcount min=loop max=loop
#pragma HLS PIPELINE II=1
				A_bram[i][j] = A[i * (n) + j];
			}
		}


	for(int i = 0; i < m; i++){
		#pragma HLS loop_tripcount min=loop max=loop
			for(int j = 0; j < p; j++){
			#pragma HLS loop_tripcount min=loop max=loop

				#pragma HLS PIPELINE II=1
				B_bram[i][j] = B[i * (m) + j];
			}
		}



	for(int i = 0; i < n; i++){
		#pragma HLS loop_tripcount min=loop max=loop
			for(int j = 0; j < p; j++){
			#pragma HLS loop_tripcount min=loop max=loop

				#pragma HLS PIPELINE II=1
				int result = 0;
				for(int k = 0 ; k < m ; k++){
					result += A_bram[i][k] * B_bram[k][j];
				}
				out[i * (n) + j] = result;
			}
		}





}
}


//void MATRIX_MUL(ap_uint<8> A[n][m], ap_uint<8> B[m][p], int out[n][p]){
//
//	const int loop = m;
//
//	for(int i = 0; i < n; i++){
//		#pragma HLS loop_tripcount min=loop max=loop
//			for(int j = 0; j < p; j++){
//			#pragma HLS loop_tripcount min=loop max=loop
//
//				int result = 0;
//				for(int k = 0 ; k < m ; k++){
//					result += A[i][k] * B[k][j];
//				}
//				out[i][j] = result;
//			}
//		}
//
//}

