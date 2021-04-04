#include "f32.hpp"
#include "types.hpp"
#include <iostream>
#include <math.h>

#include "cl.hpp"

i32 max(i32 a, i32 b) {
  i32 diff = a - b;
  i32 dsgn = diff >> 31;
  return a - (diff & dsgn);
}

i32 min(i32 a, i32 b) {
  i32 diff = a - b;
  i32 dsgn = diff >> 31;
  return b + (diff & dsgn);
}

// 'block_height' neews to be a divisor of 'lines' and 'block_width' needs to be a
// divisor of 'columns', so the matrix can be splited into 
// (lines/block_height)*(columns/block_width) submatrices.
template<i32 lines, i32 columns, i32 block_height = 2, i32 block_width = 2>
class matrix {
public:
	// Data is stored into a block major fashion, what this means is that 
	// the first 'block_height*block_width' elements of the array correspont
	// to a submatrix of size 'block_height*block_width', both blocks and 
	// submatrices are stored into a row major fashion.
	// ex:
	// [1, 2, 3,  4]
	// [5, 6, 7,  8]
	// [9,10, 11,12]
	// [13,14,15,16]
	// using 2x2 blocks will be stored as:
	// [1, 2, 5,  6,
	//  3, 4, 7,  8,
	//  9,10, 13,14,
	//  11,12,15,16]
	f32* data;

	matrix() {
		this->data = new f32[lines*columns];
		// memset was given unpredictable results, probably due to
		// memory allignment 
		for(i32 i=0; i<lines*columns;i++)
			this->data[i] = 0.f;
	}

	matrix(f32* matrix_data) {
			/*
			* Matrix is stored in a block major fashior for optimizing 
			* cache locality on operations. So the content of the matrix_data
			* will be stored as continuously small matrix of size (block_height, block_width)

			*/
			this->data = new f32[lines*columns];

			i32 idx = 0;

			for(i32 block_line=0; block_line<lines; block_line+=block_height) {
				for(i32 block_column=0; block_column<columns; block_column+=block_width) {
					for(i32 y=0; y<block_height; y++) {
						for(i32 x=0; x<block_width; x++) {
							this->data[idx++] = matrix_data[(block_line + y) * columns + block_column + x];
						}
					}
				}
			}   
	}

	const f32 get(i32 i, i32 j) const {
		return this->data[get_index(i,j)];
	}

	inline const void print() {
		for(int i=0; i<lines; i++) {
			std::cout << "[";
			for(int j=0; j<columns; j++) {
				std::cout << this->get(i,j);
				if(j != columns - 1)
					std::cout	<< ", "; 
			}
			std::cout << "]" << std::endl;
    }
    std::cout <<std::endl;
	}

	inline const i32 get_index(i32 i, i32 j) const {
		i32 y = i%block_height;
		i32 x = j%block_width;
		return get_block_start_index(i,j) + (y*block_width + x);
	}

	inline const i32 get_block_start_index(i32 i, i32 j) const {
		i32 block_line = i/block_height;
		i32 block_column = j/block_width;
		
		/**
		 * [a,b,c,d] [e,f,g,h] 
		 * [i,j,k,l] [m,n,o,p]
		 */

		return block_line*(columns/block_width)*(block_height * block_width) + block_column *(block_height * block_width);
	}

	static matrix<lines, columns> identity() {
		matrix<lines, columns> ident = matrix<lines, columns>();
		for(i32 i=0; i < min(lines,columns); i++)
			ident.data[ident.get_index(i,i)] = 1.f;
		return ident;
	}

	// // This function multiply matrices using cached submatrices
	// // so it can have better performance due to less cache misses
	// template<i32 A_columns, i32 B_lines>
	// static matrix<lines, columns> block_mul(const matrix<lines, A_columns>& A, const matrix<B_lines, columns>& B) {
		
	// 	static_assert(A_columns == B_lines, "Matrix A lines needs to be equal Matrix B colums\n");
		
	// 	matrix<lines, columns> C = matrix<lines, columns>();

	// 	for(i32 i=0; i<B_lines; i+=block_height) {
	// 		for(i32 j=0; j<A_columns; j+=block_width) {
	// 				// multiply blocks submatrices
	// 					for(i32 k=0; k < max(A_columns/block_width, B_lines/block_height); k++) {
	// 			for(i32 y=0; y<block_height; y++) {
	// 				for(i32 x=0; x<block_width; x++) {
	// 						for(i32 q=0; q < max(block_height, block_width); q++) {
	// 							i32 Cl = i+y; // C matric line
	// 							i32 Cc = j+x; // C matric column
	// 							i32 Al = i+y; // A matric line
	// 							i32 Ac = ((i+k*block_width)%A_columns + x + q) % A_columns; // A matric column
	// 							i32 Bl = ((i+k*block_height)%B_lines + x + q) % B_lines; // B matric line
	// 							i32 Bc = j + x; // B matric column
	// 							printf("C[%i %i] += ", Cl, Cc);
	// 							printf(" A[%i %i] + ", Al, Bc);
	// 							printf("B[%i %i]\n", Bl, Bc);
	// 							C.data[C.get_index(Cl, Cc)] += 
	// 								A.data[A.get_index(Al, Ac)] * B.data[B.get_index(Bl, Bc)];
	// 						}
	// 					}
	// 				}
	// 			}
	// 			printf("\n");
	// 		}
	// 	}	
	// 	return C;
	// }

	// Fox matrix multiplication
	template<i32 A_columns, i32 B_lines>
	static matrix<lines, columns> mul(const matrix<lines, A_columns>& A, const matrix<B_lines, columns>& B) {
		
		static_assert(A_columns == B_lines, "Matrix A lines needs to be equal Matrix B colums\n");
		
		matrix<lines, columns> C = matrix<lines, columns>();
		for(i32 i=0; i<B_lines; i++) {
			for(i32 j=0; j<A_columns; j++) {
				// Code above can be splited into pij processes/threads for
				// parallel matrix multiplication.
				for(i32 k=0; k<max(A_columns, B_lines); k++) {
					C.data[C.get_index(i, j)] = C.get(i, j) + A.get(i, (i+k) % max(A_columns, B_lines)) * B.get((i+k)%max(A_columns, B_lines), j);
				}
			}
		}
		return C;
	}


	template<i32 A_columns, i32 B_lines>
	static matrix<lines, columns> gpu_mul(const matrix<lines, A_columns>& A, const matrix<B_lines, columns>& B) {
		static_assert(A_columns == B_lines, "Matrix A lines needs to be equal Matrix B colums\n");
		matrix<lines, columns> C = matrix<lines, columns>();
		// each gpu core will multiply a block submatrix
		mul_matrix(A.data,B.data,C.data,block_width,block_height,lines,columns,A_columns,B_lines);
	
		return C;
	}




	template<i32 A_columns, i32 B_lines>
	static matrix<lines, columns> block_mul(const matrix<lines, A_columns>& A, const matrix<B_lines, columns>& B) {
		// block_width of A needs to be equal to block_height of B
		static_assert(A_columns == B_lines, "Matrix A lines needs to be equal Matrix B colums\n");
		
		matrix<lines, columns> C = matrix<lines, columns>();

		// Iterate over C blocks
		for(i32 i=0; i<lines/block_height; i++) {
			for(i32 j=0; j<columns/block_width; j++) {
				//working C(i,j) block
				// C(i,j) = A(i,:)*B(:,j)
				for(i32 k=0; k<A_columns/block_width; k++) {
				// for(i32 k=0; k<columns/block_width; k++) {
					// k'th A block line
					// k'th B block column

					// printf("C[%i %i] += A[%i %i] X B[%i %i]\n",
					// 	(block_height*i)%lines, (block_width*j)%columns,
					// 	(block_height*i)%lines, (block_width*k)%A_columns,
					// 	(block_height*k)%B_lines, (block_width*j)%columns
					// );
					// Multiply block matrix
					for(i32 y=0; y<block_height; y++) {
						for(i32 x=0; x<block_width; x++) {
							for(i32 q=0; q<block_width; q++) {
								// printf("	c[%i %i] += a[%i %i] * b[%i %i]\n",
								// 	(block_height*i)%lines +  y, (block_width*j)%columns + x,
								// 	(block_height*i)%lines + y, (block_width*k)%A_columns + (x+q)%block_width,
								// 	(block_height*k)%B_lines + (y+q)%block_height, (block_width*j)%columns + x
								// );

								C.data[
									C.get_index((block_height*i)%lines + y, (block_width*j)%columns + x)
								] += A.data[
									// OBS: block_width of A needs to be equal to block_height of B
									A.get_index((block_height*i)%lines + y, (block_width*k)%A_columns + (x+q)%block_width)
								] * B.data[
									// OBS: block_width of A needs to be equal to block_height of B
									B.get_index((block_height*k)%B_lines + (x+q)%block_width, (block_width*j)%columns + x)
								];
							}
						}
					}
				}
			}
		}	
		return C;
	}


};

