/* 
 * This code calcutes the solution of the diffusion equaution in 3D, using time forward 
 * discretization for the time derivative, and using discrete fourier transform to calculate
 * spatial derivatives. 
 * 
 * Created by G.P. Brandino, I. Girotto, R. Gebauer
 * Last revision: March 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include "utilities.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))


void print3DMatrixLinear(double *matrix, int n1, int n2, int n3) {
    // Iterate over each slice along X (each column of the matrix)
    for (int ix = 0; ix < n1; ix++) {
        printf("Slice for X = %d:\n", ix + 1);  // Print the slice based on the X index
        for (int iz = 0; iz < n3; iz++) {
            for (int iy = 0; iy < n2; iy++) {
                // Calculate the index for the 1D array
                int index = n3*n2*ix + n3*iy + iz;
                printf("%.4f ", matrix[index]);
            }
            printf("\n");  // Newline for next row
        }
        printf("\n");  // Newline after each slice
    }
}
void print2DMatrixLinear(double *matrix, int X, int Y) {
    // Iterate over each row (X) and each column (Y)
    printf(" slice = \n");
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            // Calculate the index for the 1D array in row-major order
            int index = x * Y + y;
            printf("%.2f ", matrix[index]);  // Print the value (formatted as double)
        }
        printf("\n");  // Newline after each row
    }
}

void print1DVector(double *vector, int size) {
    // Iterate through the 1D vector and print each element
    printf(" line = \n");
    for (int i = 0; i < size; i++) {
        printf("%.2f \n", vector[i]);  // Print the element with two decimal places
    }
    printf("\n");  // Newline after the vector is printed
}





int main(){

    // Dimensions of the system
    double L1 = 10., L2 = 10., L3 = 20.;
    // Grid size
    //int n1 = 5, n2 = 5, n3 = 5;
    //int n1 = 10, n2 = 10, n3 = 10;
    int n1 = 48, n2 = 48, n3 = 96;
    // time step for time integration
    double dt = 2.e-3; 
    // number of time steps
    int nstep = 101; 
    // Radius of diffusion channel
    double rad_diff = 0.7;
    // Radius of starting concentration
    double rad_conc = 0.6;
    double start, end;
  
    double *diffusivity, *conc, *dconc, *aux1, *aux2, *data, *slice, *line;
    double *rData, *iData;
    double *testData;

    diffusivity = (double*)malloc(n1*n2*n3*sizeof(double));
    conc = (double*)malloc(n1*n2*n3*sizeof(double));
    dconc = (double*)malloc(n1*n2*n3*sizeof(double));
    aux1 = (double*)malloc(n1*n2*n3*sizeof(double));
    aux2 = (double*)malloc(n1*n2*n3*sizeof(double));
    data = (double*)malloc(n1*n2*n3*sizeof(double));
    slice = (double*)malloc(n2*n3*sizeof(double));
    line = (double*)malloc(n1*sizeof(double));

    rData = (double*)malloc(n1*n2*n3*sizeof(double));
    iData = (double*)malloc(n1*n2*n3*sizeof(double));
    testData = (double*)malloc(n1*n2*n3*sizeof(double));

    fftw_complex *cdata, *cdata_alternate, *cslice, *cline;
    cdata = ( fftw_complex* ) fftw_malloc( n1*n2 * n3 * sizeof(fftw_complex) );
    cdata_alternate = ( fftw_complex* ) fftw_malloc( n1*n2 * n3 * sizeof(fftw_complex) );
    cslice = ( fftw_complex* ) fftw_malloc( n2 * n3 * sizeof(fftw_complex) );
    cline = ( fftw_complex* ) fftw_malloc( n1* sizeof(fftw_complex) );
   
    int i1, i2, i3, ipol, istep, index;
  
    double f1conc, f2conc, f3conc, f1diff, f2diff, f3diff, fac, ss;
    double x1, x2 , x3, rr, r2mean;

    fftw_handler fft_h, fft_1d_h, fft_2d_h;
    init_fftw(&fft_h, n1, n2, n3);
    init_fftw_slice(&fft_1d_h, &fft_2d_h, n1, n2, n3);

    for (int k = 0; k<n1*n2*n3 ;k++) {
      data[k] = 50;
      cdata[k] = 0.0 + 0.0*I;
      cdata_alternate[k] = 0.0 + 0.0*I;
    }

    //print3DMatrixLinear(data,n1,n2,n3);


    fft_3d(&fft_h, &fft_1d_h, &fft_2d_h, n1, n2, n3, data, cdata, true);
    fft_3d_slice(&fft_h, &fft_1d_h, &fft_2d_h, n1, n2, n3, data, cdata_alternate, true);

    for (int k = 0; k<n1*n2*n3 ;k++) {
      rData[k] = creal(cdata[k]);
      iData[k] = cimag(cdata[k]);
    }

    //print3DMatrixLinear(rData,n1,n2,n3);
    //print3DMatrixLinear(iData,n1,n2,n3);

    for (int k = 0; k<n1*n2*n3 ;k++) {
      rData[k] = creal(cdata_alternate[k]);
      iData[k] = cimag(cdata_alternate[k]);
    }

    //print3DMatrixLinear(rData,n1,n2,n3);
    //print3DMatrixLinear(iData,n1,n2,n3);

    fft_3d(&fft_h, &fft_1d_h, &fft_2d_h, n1, n2, n3, data, cdata, false);
   

    for (int k = 0; k<n1*n2*n3 ;k++) {
      rData[k] = data[k];
    }

    //print3DMatrixLinear(rData,n1,n2,n3);
    //print3DMatrixLinear(iData,n1,n2,n3);


    fft_3d_slice(&fft_h, &fft_1d_h, &fft_2d_h, n1, n2, n3, testData, cdata_alternate, false);
    for (int k = 0; k<n1*n2*n3 ;k++) {
      rData[k] = testData[k];
    }

    //print3DMatrixLinear(rData,n1,n2,n3);
    //print3DMatrixLinear(iData,n1,n2,n3);


 // 
 // Define the diffusivity inside the system and 
 // the starting concentration
 //
 // ss is to integrate (and normalize) the concentration
 // 

    
    ss = 0.0;

    for (i3 = 0; i3 < n3; ++i3)
      {  
	x3=L3*((double)i3)/n3;
	f3diff = exp( -pow((x3-0.5*L3)/rad_diff,2));
	f3conc = exp( -pow((x3-0.5*L3)/rad_conc,2));
	
	
        for (i2 = 0; i2 < n2; ++i2)
	  {
            x2=L2*((double)i2)/n2;
            f2diff = exp( -pow((x2-0.5*L2)/rad_diff,2));
            f2conc = exp( -pow((x2-0.5*L2)/rad_conc,2));
	    
	
	    for (i1 = 0; i1 < n1; ++i1)
	      {
		x1=L1*((double)i1)/n1;
		f1diff = exp( -pow((x1-0.5*L1)/rad_diff,2));
		f1conc = exp( -pow((x1-0.5*L1)/rad_conc,2));
		
		index = index_f(i1, i2, i3, n1, n2, n3);
		diffusivity[index]  = MAX( f1diff * f2diff, f2diff * f3diff);
		conc[index] = f1conc * f2conc * f3conc;
		ss += conc[index]; 
		
	      }   
	  }
      }
    
    plot_data_2d("diffusivity", n1, n2, n3, 1, diffusivity);
    plot_data_2d("diffusivity", n1, n2, n3, 2, diffusivity);
    plot_data_2d("diffusivity", n1, n2, n3, 3, diffusivity);
    
    //print3DMatrixLinear(conc,n1,n2,n3);
    //print3DMatrixLinear(diffusivity,n1,n2,n3);
    for (int k = 0; k<n1*n2*n3 ;k++) {
      data[k] = 0;
      cdata[k] = 0.0 + 0.0*I;
      cdata_alternate[k] = 0.0 + 0.0*I;
    }

    fft_3d(&fft_h, &fft_1d_h, &fft_2d_h, n1, n2, n3, diffusivity, cdata, true);
    fft_3d_slice(&fft_h, &fft_1d_h, &fft_2d_h, n1, n2, n3, diffusivity, cdata_alternate, true);

    for (int k = 0; k<n1*n2*n3 ;k++) {
      rData[k] = creal(cdata[k]);
      iData[k] = cimag(cdata_alternate[k]);
    }
    //printf("fourier transform 3D \n");
    //print3DMatrixLinear(iData,n1,n2,n3);
    
    for (int k = 0; k<n1*n2*n3 ;k++) {
      rData[k] = creal(cdata_alternate[k]);
      iData[k] = cimag(cdata_alternate[k]);
    }
    //printf("fourier transform 2D slices + 1D lines \n");
    //print3DMatrixLinear(iData,n1,n2,n3);

    //print3DMatrixLinear(rData,n1,n2,n3);
    //print3DMatrixLinear(iData,n1,n2,n3);

    
    
    fac= L1*L2*L3/(n1*n2*n3);
  // Now normalize the concentration
    ss = 1.0/(ss*fac);
    for (i1=0; i1< n1*n2*n3; ++i1)
      conc[i1]*=ss;
      
   // initialize the fftw system 

   
 
    start = seconds();
    for (istep = 1; istep <= nstep; ++istep) {
        for (i1=0; i1< n1*n2*n3; ++i1) {
	          dconc[i1] = 0.0; // set dconc to 0
        }
        for (ipol =1; ipol<=3; ++ipol ) {
            // compute derivative of conc and store them in aux1
	          derivative(&fft_h,&fft_1d_h, &fft_2d_h, n1, n2, n3, L1, L2, L3, ipol, conc, aux1);
	          for (i1=0; i1< n1*n2*n3; ++i1) {
                aux1[i1] *= diffusivity[i1]; // mult aux1 by diffusivity
	          }
            // compute derivative of aux1 and store them them in aux2
	          derivative(&fft_h,&fft_1d_h, &fft_2d_h, n1, n2, n3, L1, L2, L3, ipol, aux1, aux2);
            // summing up contributions from the three spatial directions
            for (i1=0; i1< n1*n2*n3; ++i1) {
	              dconc[i1] += aux2[i1]; // mult dconc by aux2
            }
	      } 
        for (i1=0; i1< n1*n2*n3; ++i1) {
	          conc[i1] += dt*dconc[i1];
        }

        /*
        if (istep == 1 )
        {
          //printf("concentration at first iteration \n");
          //print3DMatrixLinear(conc,n1,n2,n3);
        }
        */
        if (istep%30 == 1) {
            // Check the normalization of conc
            ss = 0.;
            r2mean = 0.;
            for (i3 = 0; i3 < n3; ++i3) {
                x3=L3*((double)i3)/n3 - 0.5*L3;

                for (i2 = 0; i2 < n2; ++i2) {
                    x2=L2*((double)i2)/n2 - 0.5*L2;

                    for (i1 = 0; i1 < n1; ++i1) {
			                  x1=L1*((double)i1)/n1 - 0.5*L1;
			                  rr = pow( x1, 2)  + pow( x2, 2) + pow( x3, 2);
			                  index = index_f(i1, i2, i3, n1, n2, n3); 
			                  ss += conc[index]; 
			                  r2mean += conc[index]*rr;
		                }   
		            }
	          }
            ss *= fac;
            r2mean *= fac;
            end = seconds();
            printf(" %d %17.15f %17.15f Elapsed time per iteration %f \n", istep, r2mean, ss, (end-start)/istep);

            plot_data_2d("concentration", n1, n2, n3, 2, conc);
            plot_data_1d("1d_conc", n1, n2, n3, 3, conc);
	      }
	
    } 
    
    
    close_fftw(&fft_h);
    close_fftw(&fft_1d_h);
    close_fftw(&fft_2d_h);
    free(diffusivity);
    free(conc);
    free(dconc);
    free(aux1);
    free(aux2);
    free(rData);
    free(iData);
    free(testData);


    return 0;
} 
