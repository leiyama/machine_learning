How to run:

make the makefile;

After make that makefile, copy the generated file “smoother” to file “data”, then you can run it in “data”;

run it using(./smoother scene_bunny.txt 600 600 0.0512) for example.

Method to build F operator matrix:

1. first we initiate the (i - 1, i - 1)th item of the matrix with 1, therefore, we build the sparseMatrix representation of the Identity Matrix;

2. then when we do the loop to iterate all vertices adjacent to v_i, we use:
    F.coeffRef(i - 1, i - 1) -= -cot_j;
    F.insert( i - 1, j - 1 ) = -cot_j;
   to add up the Laplacian operator, 
   where cot_j = h * (cot_aj + cot_bj) / (2 * A) 
3. finished!

