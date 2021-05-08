for (int i = 0; i <SIZE; i++){
	for (int k = 0; k<SIZE; k+=4){
		__m128 v_a = _mm_load_ps(mat_a[i][k]);
		__m128 res = _mm_setzero_ps();
		__m128 vec_c = _mm_setzero_ps();	
			for(int j = 0; j<SIZE; j+=4){
				__m128 v_b = _mm_load_ps(mat_b[k][j]);
				__m128 v_ab = v_a*v_b;
				res = _mm_hadd_ps(v_ab, v_ab); 
				res = _mm_hadd_ps(v_ab, v_ab);	//add twice to get result in 32 bits
				res = _mm_add_ps (v_c, res); //add 32 bits of each
			}
			mat_c[i][k] = _mm_load_ss(res);
	}
}


static float mat_a_int[4][4] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float mat_b_int[4][4] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));


for (int i = 0; i<SIZE; i+=4){
	for (int a = 0; a<SIZE; a+=4){
		mat_a_int[0][0] = _mm_load_ps(&mat_a[i][a]);
		mat_a_int[1][0] = _mm_load_ps(&mat_a[i+1][a]);
		mat_a_int[2][0] = _mm_load_ps(&mat_a[i+2][a]);
		mat_a_int[3][0] = _mm_load_ps(&mat_a[i+3][a]);
		
		__m128 rowb0 = _mm_load_ps(&mat_b[i][a]);
		__m128 rowb1 = _mm_load_ps(&mat_b[i+1][a]);
		__m128 rowb2 = _mm_load_ps(&mat_b[i+2][a]);
		__m128 rowb3 = _mm_load_ps(&mat_b[i+3][a]);

		_MM_TRANSPOSE4_PS (__m128 row0, __m128 row1, __m128 row2, __m128 row3)

		mat_b_int[0][0] = _mm_load_ps(rowb0);
		mat_b_int[1][0] = _mm_load_ps(rowb1);
		mat_b_int[2][0] = _mm_load_ps(rowb2);
		mat_b_int[3][0] = _mm_load_ps(rowb3);
		
		for (int l = 0; l <4; l++){
			__m128 rowa_int = _mm_load_ps(&mat_a_int[l][0]);
			__m128 res_1;
			for (int m = 0; m <4; m++){
			__m128 rowb_int = _mm_load_ps(&mat_b_int[m][0]);
				res_1= rowb_int * rowa_int;
				res_1 = _mm_hadd_ps(res_1, res_1);
				res_1 = _mm_hadd_ps(res_1, res_1);
				_mm_store_ss(&mat_c[i+l][a+m], res_1);
			}
		}
	}
}


static float mat_a_int[4][4] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float mat_b_int[4][4] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));


for (int i = 0; i<SIZE; i+=4){
	for (int a = 0; a<SIZE; a+=4){
		
		__m128 rowb0 = _mm_load_ps(&mat_b[i][a]);
		__m128 rowb1 = _mm_load_ps(&mat_b[i+1][a]);
		__m128 rowb2 = _mm_load_ps(&mat_b[i+2][a]);
		__m128 rowb3 = _mm_load_ps(&mat_b[i+3][a]);

		_MM_TRANSPOSE4_PS (__m128 row0, __m128 row1, __m128 row2, __m128 row3)

		for (int l = 0; l <4; l++){
			__m128 rowa_int = _mm_load_ps(&mat_a[i][a]);
			__m128 res_1;

			res_0 = rowa_int * rowb0;
			res_0 = _mm_hadd_ps(res_0, res_0);
			res_0 = _mm_hadd_ps(res_0, res_0);
			_mm_store_ss(&mat_c[i+l][a+0], res_0);

			res_1 = rowa_int * rowb1;
			res_1 = _mm_hadd_ps(res_1, res_1);
			res_1 = _mm_hadd_ps(res_1, res_1);
			_mm_store_ss(&mat_c[i+l][a+1], res_1);

			res_2 = rowa_int * rowb2;
			res_2 = _mm_hadd_ps(res_2, res_2);
			res_2 = _mm_hadd_ps(res_2, res_2);
			_mm_store_ss(&mat_c[i+l][a+2], res_2);

			res_3 = rowa_int * rowb3;
			res_3 = _mm_hadd_ps(res_3, res_3);
			res_3 = _mm_hadd_ps(res_3, res_3);
			_mm_store_ss(&mat_c[i+l][a+3], res_3);
	
		
		}
	}
}
