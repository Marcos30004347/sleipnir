int get_block_start_index(int i, int j, int columns, int block_height, int block_width) {
	int block_line = i/block_height;
	int block_column = j/block_width;
	return block_line*(columns/block_width)*(block_height * block_width) + block_column * (block_height * block_width);
}

int get_index(int i, int j, int columns, int block_height, int block_width) {
	int y = i%block_height;
	int x = j%block_width;
	return get_block_start_index(i,j, columns, block_height, block_width) + (y*block_width + x);
}

__kernel void matrix_mul(
	const int block_height,
	const int block_width,
	const int lines,
	const int columns,
	const int A_columns,
	const int B_lines,
  	const __global float* A,
	const __global float* B,
  	__global float* C
) {
	const int i = get_global_id(0);
	const int j = get_global_id(1);

	// C[get_block_start_index(i, j, columns, block_height, block_width)] = 1;//get_global_id(0)*lines + get_global_id(1);
	// C[get_index(i*block_height, j*block_width, columns, block_height, block_width)] = get_index(i*block_height, j*block_width, columns, block_height, block_width)/(block_height*block_width);
	for(int k=0; k<A_columns/block_width; k++) {
	
		for(int y=0; y<block_height; y++) {
			for(int x=0; x<block_width; x++) {
				for(int q=0; q<block_width; q++) {
					// printf("	c[%i %i] += a[%i %i] * b[%i %i]\n",
					// 	(block_height*i)%lines +  y, (block_width*j)%columns + x,
					// 	(block_height*i)%lines + y, (block_width*k)%A_columns + (x+q)%block_width,
					// 	(block_height*k)%B_lines + (y+q)%block_height, (block_width*j)%columns + x
					// );

					C[
						get_index((block_height*i)%lines + y, (block_width*j)%columns + x, columns, block_height, block_width)
					] += A[
						get_index((block_height*i)%lines + y, (block_width*k)%A_columns + (x+q)%block_width, A_columns, block_height, block_width)
					] * B[
						get_index((block_height*k)%B_lines + (x+q)%block_width, (block_width*j)%columns + x, columns, block_height, block_width)
					];
				}
			}
		}
	}
	
	// C[get_index(i, j, columns, block_height, block_width)] = j;

	// for(int k=0; k < max(A_columns/block_width, B_lines/block_height); k++) {
	// 	for(int y=0; y<block_height; y++) {
	// 		for(int x=0; x<block_width; x++) {
	// 			for(int q=0; q < max(block_height, block_width); q++) {
	// 				int Cl = i+y;
	// 				int Cc = j+x;
	// 				int Al = i+y;
	// 				int Ac = ((i+k*block_width)%A_columns + x + q) % A_columns;
	// 				int Bl = ((i+k*block_height)%B_lines + x + q) % B_lines;
	// 				int Bc = j + x;
					
	// 				C[get_index(Cl, Cc, block_height, block_width)] += 
	// 					A[A.get_index(Al, Ac, block_height, block_width)] * B[get_index(Bl, Bc, block_height, block_width)];
	// 			}
	// 		}
	// 	}
	// }

}