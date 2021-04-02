#include "f32.hpp"
#include "types.hpp"
#include <iostream>
#include <math.h>

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

    inline const i32 get_index(i32 i, i32 j) const {
			i32 y = i%block_height;
			i32 x = j%block_width;
			return get_block_start_index(i,j) + (y*block_width + x);
    }

    inline const i32 get_block_start_index(i32 i, i32 j) const {
			i32 block_line = i/block_height;
			i32 block_column = j/block_width;
			return (block_height * block_width) * (min(columns/block_width, lines/block_height) * max(block_line, block_column) + min(block_line, block_column));
    }

    static matrix<lines, columns> identity() {
			matrix<lines, columns> ident = matrix<lines, columns>();
			for(i32 i=0; i < min(lines,columns); i++)
				ident.data[ident.get_index(i,i)] = 1.f;
			return ident;
    }

    // template<i32 mat_l, i32 mat_c>
    // matrix<lines, mat_c, block_height, block_width> mul(matrix<mat_l, mat_c, block_height, block_width>& b) {
    //     static_assert(mat_l == columns, "Matrix A lines needs to be equal Matrix B colums\n");
        
    //     matrix<lines, mat_c, block_height, block_width> res;

    //     for(i32 i=0; i<mat_l; i++) {
    //         for(i32 j=0; j<columns; j++) {
    //             // std::cout << "process[" << i <<", " << j<<"]" << std::endl;
    //             for(i32 k=0; k<max(columns, mat_l); k++) {
    //                 // std::cout << "columns[" << i << ", " << j << "]" <<" + a["<<i << ", " << i+k << "] * b[" << i+k << ", " << j << "]" << std::endl;
    //                 res.data[res.get_index(i, j)] = res.get(i, j) + this->get(i, (i+k) % max(columns, mat_l)) * b.get((i+k)%max(columns, mat_l), j);
    //             }
    
    //             // std::cout << std::endl;

    //         }
    //     }
    //     return res;
    // }



    // template<i32 mat_l, i32 mat_c>
    // matrix<columns, mat_l, block_height, block_width> simd_mul(matrix<mat_l, mat_c, block_height, block_width>& b) {
    //     // columns[i, j] = dot(a[i,:],b[:,j])
    //     static_assert(mat_l == columns, "Matrix A lines needs to be equal Matrix B colums\n");
        
    //     matrix<columns, mat_l, block_height, block_width> res;

    //     for(i32 i=0; i<lines; i++) {
    //         for(i32 j=0; j<columns; j++) {
    //             for(i32 k=0; k<lines/4; k++) {

    //                 f32x4 al = f32x4_new(
    //                     this->data[this->get_index(i,(i+0)%lines)],
    //                     this->data[this->get_index(i,(i+1)%lines)],
    //                     this->data[this->get_index(i,(i+2)%lines)],
    //                     this->data[this->get_index(i,(i+3)%lines)]
    //                 );

    //                 f32x4 bc = f32x4_new(
    //                     b.data[this->get_index((i+0)%lines,j)],
    //                     b.data[this->get_index((i+1)%lines,j)],
    //                     b.data[this->get_index((i+2)%lines,j)],
    //                     b.data[this->get_index((i+3)%lines,j)]
    //                 );

    //                 res.data[res.get_index(i, j)] = f32x4_dot(al,bc).vec[0];
    //             }
    //         }
    //     }
    //     return res;
    // } 

    // template<i32 mat_l, i32 mat_c>
    // matrix<lines, mat_c, block_height, block_width> block_mul(matrix<mat_l, mat_c, block_height, block_width>& b) {
    //     static_assert(mat_l == columns, "Matrix A lines needs to be equal Matrix B colums\n");
        
    //     matrix<lines, mat_c, block_height, block_width> res;

    //     for(i32 i=0; i<mat_l; i+=block_height) {
    //         for(i32 j=0; j<columns; j+=block_width) {
    //             // maybe should go from o to max(lines/block_height,columns/block_width)
    //             for(i32 k=0; k < max(columns/block_width, mat_l/block_height); k++) {

    //                 // i32 Cstart = get_block_start_index(i, j);
    //                 // i32 Astart = get_block_start_index((j + k*block_height)%lines, i);
    //                 // i32 Bstart = b.get_block_start_index(i, (j + k*block_height)%lines);

    //                 // printf("Cs = [%i, %i]\n", i, j);
    //                 // printf("As = [%i, %i]\n", (i + k*block_height)%columns, i);
    //                 // printf("Bs = [%i, %i]\n\n", i, (i + k*block_height )%mat_l );

    //                 // Multiply submatrices
    //                 for(i32 y=0; y<block_height; y++) {
    //                     for(i32 x=0; x<block_width; x++) {
    //                         // maybe should go from o to max(lines/block_height,columns/block_width)
    //                         for(i32 q=0; q < max(block_height, block_width); q++) {
    //                             // Above the 'i' in the may also be ((i+k*block_height)%lines
    //                             i32 ln = i+y;
    //                             i32 cl = j+x;
                            
    //                             i32 Al = i+y;
    //                             i32 Ac = ((i+k*block_height)%columns + x + q) % columns;
    
    //                             i32 Bl = ((i+k*block_height)%mat_l + x + q) % mat_l;
    //                             i32 Bc = j + x;

    //                             res.data[res.get_index(ln, cl)] += 
    //                                 this->data[this->get_index(Al, Ac)] * b.data[b.get_index(Bl, Bc)];
                                
    //                             // printf("columns[%i, %i] += a[%i, %i]*b[%i, %i]\n", 
    //                             //     ln, cl,
    //                             //     Al, Ac,
    //                             //     Bl, Bc
    //                             // );

    //                         }
    //                     }
    //                 }
    //             }
    //             // printf("\n");

    //         }
    //     }
            

    //     return res;
    // }

     

    // This function multiply matrices using cached submatrices
    // so it can have better performance due to less cache misses
    template<i32 Amat_c, i32 Bmat_l>
    static matrix<lines, columns> block_mul(const matrix<lines, Amat_c>& a, const matrix<Bmat_l, columns>& b) {
        static_assert(Amat_c == Bmat_l, "Matrix A lines needs to be equal Matrix B colums\n");
        
        matrix<lines, columns> res = matrix<lines, columns>();

        for(i32 i=0; i<Bmat_l; i+=block_height) {
					for(i32 j=0; j<Amat_c; j+=block_width) {
						for(i32 k=0; k < max(Amat_c/block_width, Bmat_l/block_height); k++) {
							// multiply blocks submatrices
							for(i32 y=0; y<block_height; y++) {
								for(i32 x=0; x<block_width; x++) {
									for(i32 q=0; q < max(block_height, block_width); q++) {
										i32 Cl = i+y; // C matric line
										i32 Cc = j+x; // C matric column
										i32 Al = i+y; // A matric line
										i32 Ac = ((i+k*block_width)%Amat_c + x + q) % Amat_c; // A matric column
										i32 Bl = ((i+k*block_height)%Bmat_l + x + q) % Bmat_l; // B matric line
										i32 Bc = j + x; // B matric column
										res.data[res.get_index(Cl, Cc)] += 
												a.data[a.get_index(Al, Ac)] * b.data[b.get_index(Bl, Bc)];
									}
								}
							}
						}
					}
				}	
        return res;
    }

		// Fox matrix multiplication
    template<i32 Amat_c, i32 Bmat_l>
    static matrix<lines, columns> mul(const matrix<lines, Amat_c>& a, const matrix<Bmat_l, columns>& b) {
        static_assert(Amat_c == Bmat_l, "Matrix A lines needs to be equal Matrix B colums\n");
        
        matrix<lines, columns> res = matrix<lines, columns>();

        for(i32 i=0; i<Bmat_l; i++) {
					for(i32 j=0; j<Amat_c; j++) {
						// Code above can be splited into pij processes/threads for
						// parallel matrix multiplication.
						for(i32 k=0; k<max(Amat_c, Bmat_l); k++) {
							res.data[res.get_index(i, j)] = res.get(i, j) + a.get(i, (i+k) % max(Amat_c, Bmat_l)) * b.get((i+k)%max(Amat_c, Bmat_l), j);
						}
					}
        }
        return res;
    }
};

