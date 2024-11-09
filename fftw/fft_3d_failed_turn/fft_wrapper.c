/*
 * This file contains various helper function for FFTW, timing and indexing  
 * 
 * Created by G.P. Brandino, I. Girotto, R. Gebauer
 * Last revision: March 2016
 */ 

#include <complex.h>
#include <fftw3.h>
#include <stdbool.h>
#include <string.h>
#include "utilities.h"
#include <stdlib.h>
/*
* extract a 2D slice inside a 3D data
*/
void fillSlice (double *data, double *slice, int idx, int n2, int n3)
{
  for (int iy = 0; iy < n2; iy++) {
    for (int iz = 0; iz < n3; iz++) {
      int data_index = n3*n2*idx + n3*iy + iz;
      int slice_index = n3*iy + iz;
      slice[slice_index] = data[data_index];
    }
  }
}

/*
* push a 2D slice inside a 3D data
*/
void pushSlice (double *data, double *slice, int idx, int n2, int n3)
{
  for (int iy = 0; iy < n2; iy++) {
    for (int iz = 0; iz < n3; iz++) {
      int data_index = n3*n2*idx + n3*iy + iz;
      int slice_index = n3*iy + iz;
      data[data_index] = slice[slice_index];
    }
  }
}

/*
* extract a 1D line inside a 3D data
*/
void fillLine (double *data, double *line, int idy, int idz, int n1, int n2, int n3)
{
  for (int ix = 0; ix < n1; ix++) {
      int data_index = n3*n2*ix + n3*idy + idz;
      line[ix] = data[data_index];
  }
}

/*
* push a 1D line inside a 3D data
*/
void pushLine (double *data, double *line, int idy, int idz, int n1, int n2, int n3)
{
  for (int ix = 0; ix < n1; ix++) {
      int data_index = n3*n2*ix + n3*idy + idz;
      data[data_index] = line[ix];
  }
}

/*
* extract a 1D cline from ad 3D cdata
*/
void fillCLine (fftw_complex *cdata, fftw_complex *cline, int idy, int idz, int n1, int n2, int n3)
{
  for (int ix = 0; ix < n1; ix++) {
      int data_index = n3*n2*ix + n3*idy + idz;
      cline[ix] = cdata[data_index];
  }
}
/*
* push a 1D cline inside a 3D cdata
*/
void pushCLine (fftw_complex *cdata, fftw_complex *cline, int idy, int idz, int n1, int n2, int n3)
{
  for (int ix = 0; ix < n1; ix++) {
      int data_index = n3*n2*ix + n3*idy + idz;
      cdata[data_index] = cline[ix];
  }
}

/*
* extract a 2D slice inside a 3D data
*/
void fillCSlice (fftw_complex *cdata, fftw_complex *cslice, int idx, int n2, int n3)
{
  for (int iy = 0; iy < n2; iy++) {
    for (int iz = 0; iz < n3; iz++) {
      int data_index = n3*n2*idx + n3*iy + iz;
      int slice_index = n3*iy + iz;
      cslice[slice_index] = cdata[data_index]; 
    }
  }
}

/*
* push a 2D slice inside a 3D data
*/
void pushCSlice (fftw_complex *cdata, fftw_complex *cslice, int idx, int n2, int n3)
{
  for (int iy = 0; iy < n2; iy++) {
    for (int iz = 0; iz < n3; iz++) {
      int data_index = n3*n2*idx + n3*iy + iz;
      int slice_index = n3*iy + iz;
      cdata[data_index] = cslice[slice_index];
    }
  }
}

double seconds(){

  /* 
   * Return the second elapsed since Epoch (00:00:00 UTC, January 1, 1970) 
   *
   */
  struct timeval tmp;
  double sec;

  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;

  return sec;
}

/* 
 * Index linearization is computed following row-major order.
 * For more informtion see FFTW documentation:
 * http://www.fftw.org/doc/Row_002dmajor-Format.html#Row_002dmajor-Format
 *
 */
int index_f ( int i1, int i2, int i3, int n1, int n2, int n3)
{
  return n3*n2*i1 + n3*i2 + i3; 
}


void init_fftw(fftw_handler *fft, int n1, int n2, int n3)
{
  /*
   * Allocation of aligned memory beuffer
   * See also: http://www.fftw.org/doc/Memory-Allocation.html
   *
   */
  fft->fftw_data = (fftw_complex*)fftw_malloc(n1*n2*n3*sizeof(fftw_complex));
  
  
  /*
   * Allocation of FFTW plans for direct and inverse transform 
   * for complex2complex multimentionals data structures 
   * See also: http://www.fftw.org/doc/Complex-Multi_002dDimensional-DFTs.html#Complex-Multi_002dDimensional-DFTs
   *
   */
  fft->fw_plan = fftw_plan_dft_3d(n1, n2, n3, fft->fftw_data, fft->fftw_data, FFTW_FORWARD, FFTW_ESTIMATE);
  fft->bw_plan = fftw_plan_dft_3d(n1, n2, n3, fft->fftw_data, fft->fftw_data, FFTW_BACKWARD, FFTW_ESTIMATE);
  
}

void init_fftw_slice(fftw_handler *fft_1d, fftw_handler *fft_2d, int n1, int n2, int n3) {

  fft_1d->fftw_data = (fftw_complex *)fftw_malloc(n1 * sizeof(fftw_complex));


  fft_1d->fw_plan = fftw_plan_dft_1d(n1, fft_1d->fftw_data, fft_1d->fftw_data,
                                  FFTW_FORWARD, FFTW_ESTIMATE);
  fft_1d->bw_plan = fftw_plan_dft_1d(n1, fft_1d->fftw_data, fft_1d->fftw_data,
                                  FFTW_BACKWARD, FFTW_ESTIMATE);

  fft_2d->fftw_data = (fftw_complex *)fftw_malloc(n2 * n3 * sizeof(fftw_complex));                               

  fft_2d->fw_plan = fftw_plan_dft_2d(n2, n3, fft_2d->fftw_data, fft_2d->fftw_data,
                                  FFTW_FORWARD, FFTW_ESTIMATE);
  fft_2d->bw_plan = fftw_plan_dft_2d(n2, n3, fft_2d->fftw_data, fft_2d->fftw_data,
                                  FFTW_BACKWARD, FFTW_ESTIMATE);               
}

void fft_1d(fftw_handler *fft , int n1, double *data_direct,
            fftw_complex *data_rec, bool direct_to_reciprocal) {
  double fac;
  int i;

  // Now distinguish in which direction the FFT is performed
  if (direct_to_reciprocal) {
    for (i = 0; i < n1 ; i++) {
      fft->fftw_data[i] = data_direct[i] + 0.0 * I;
    }

    fftw_execute_dft(fft->fw_plan, fft->fftw_data, fft->fftw_data);

    //memcpy(data_rec, fft->fftw_data, n1 * sizeof(fftw_complex));
  } else {
    memcpy(fft->fftw_data, data_rec, n1 * sizeof(fftw_complex));

    fftw_execute_dft(fft->bw_plan, fft->fftw_data, fft->fftw_data);

    fac = 1.0 / (n1);

    for (i = 0; i < n1; ++i) {
      data_direct[i] = creal(fft->fftw_data[i]) * fac;
    }
  }
}




void fft_2d(fftw_handler *fft , int n2, int n3, double *data_direct,
            fftw_complex *data_rec, bool direct_to_reciprocal) {
  double fac;
  int i;

  // Now distinguish in which direction the FFT is performed
  if (direct_to_reciprocal) {
    for (i = 0; i < n2 * n3 ; i++) {
      fft->fftw_data[i] = data_direct[i] + 0.0 * I;
    }

    fftw_execute_dft(fft->fw_plan, fft->fftw_data, fft->fftw_data);

    //memcpy(data_rec, fft->fftw_data, n2 * n3 * sizeof(fftw_complex));
  } else {
    memcpy(fft->fftw_data, data_rec, n2 * n3 * sizeof(fftw_complex));

    fftw_execute_dft(fft->bw_plan, fft->fftw_data, fft->fftw_data);

    fac = 1.0 / (n2 * n3 );

    for (i = 0; i < n2 * n3; ++i) {
      data_direct[i] = creal(fft->fftw_data[i]) * fac;
    }
  }
}


void close_fftw(fftw_handler *fft)
{
    fftw_destroy_plan(fft->bw_plan);
    fftw_destroy_plan(fft->fw_plan);
    fftw_free(fft->fftw_data);
}

/* This subroutine uses fftw to calculate 3-dimensional discrete FFTs.
 * The data in direct space is assumed to be real-valued
 * The data in reciprocal space is complex. 
 * direct_to_reciprocal indicates in which direction the FFT is to be calculated
 * 
 * Note that for real data in direct space (like here), we have
 * F(N-j) = conj(F(j)) where F is the array in reciprocal space.
 * Here, we do not make use of this property.
 * Also, we do not use the special (time-saving) routines of FFTW which
 * allow one to save time and memory for such real-to-complex transforms.
 *
 * f: array in direct space
 * F: array in reciprocal space
 * 
 * F(k) = \sum_{l=0}^{N-1} exp(- 2 \pi I k*l/N) f(l)
 * f(l) = 1/N \sum_{k=0}^{N-1} exp(+ 2 \pi I k*l/N) F(k)
 * 
 */

void fft_3d(fftw_handler* fft, fftw_handler* fft_1d_h, fftw_handler* fft_2d_h, int n1, int n2, int n3, double *data_direct, fftw_complex* data_rec, bool direct_to_reciprocal)
{
    double fac;
    int i;
    
    // Now distinguish in which direction the FFT is performed
    if ( direct_to_reciprocal)
      {
	for(i = 0; i < n1*n2*n3; i++)
	  {
	    fft->fftw_data[i]  = data_direct[i] + 0.0 * I;
	  } 
	
	fftw_execute_dft(fft->fw_plan, fft->fftw_data, fft->fftw_data);

	memcpy(data_rec, fft->fftw_data, n1*n2*n3*sizeof(fftw_complex)); 
      }
    else
      {
	memcpy(fft->fftw_data, data_rec, n1*n2*n3*sizeof(fftw_complex));
	  
	fftw_execute_dft(fft->bw_plan, fft->fftw_data, fft->fftw_data);
	
	fac = 1.0 / ( n1 * n2 * n3 );
	
	for( i = 0; i < n1 * n2 * n3; ++i )
	  {
	    data_direct[i] = creal(fft->fftw_data[i])*fac;
	  }
      }
}

void fft_3d_slice(fftw_handler* fft, fftw_handler* fft_1d_h, fftw_handler* fft_2d_h, int n1, int n2, int n3, double *data_direct, fftw_complex* data_rec, bool direct_to_reciprocal)
{
    //double fac;
    //int i;

    double *slice, *line;
    slice = (double*)malloc(n2*n3*sizeof(double));
    line = (double*)malloc(n1*sizeof(double));

    fftw_complex *cslice, *cline;
    cslice = ( fftw_complex* ) fftw_malloc( n2 * n3 * sizeof(fftw_complex) );
    cline = ( fftw_complex* ) fftw_malloc( n1* sizeof(fftw_complex) );

    // Now distinguish in which direction the FFT is performed
    //if ( direct_to_reciprocal) {

        for (int i1 = 0; i1 < n1; i1++) {
             // take a slice of real data
            fillSlice(data_direct,slice,i1,n2,n3);

            fft_2d(fft_2d_h , n2, n3, slice,data_rec, direct_to_reciprocal);

            // fill the 2D FFT structure
            /*
            for(int k = 0; k < n2*n3; k++) {
	              fft_2d_h->fftw_data[k]  = slice[k] + 0.0 * I;
	          }
            */
            // execute 2D fftw
            //fftw_execute_dft(fft_2d_h->fw_plan, fft_2d_h->fftw_data, fft_2d_h->fftw_data);
            // write results in cslice
            for(int k = 0; k < n2*n3; k++) {
	              cslice[k] = fft_2d_h->fftw_data[k];
	          }
            //memcpy(cslice, fft_2d_h->fftw_data, n2*n3*sizeof(fftw_complex));
            // push CSlice
            pushCSlice(data_rec,cslice,i1,n2,n3);
        }
        
        for (int i2 = 0; i2 < n2; i2++) {
          for (int i3 = 0; i3 < n3; i3++) {
              // take a Cline of data
              fillCLine(data_rec,cline,i2,i3,n1,n2,n3);

              //fill the 1D FFT structure
              /*
              for(int i1 = 0; i1 < n1; i1++) {
	                fft_1d_h->fftw_data[i1]  = cline[i1];
	            }
              */

              fft_1d(fft_1d_h , n1, slice,data_rec, direct_to_reciprocal);

              // execute 1D fftw
              fftw_execute_dft(fft_1d_h->fw_plan, fft_1d_h->fftw_data, fft_1d_h->fftw_data);
              memcpy(cline, fft_1d_h->fftw_data, n1*sizeof(fftw_complex));
              pushCLine(data_rec,cline,i2,i3,n1,n2,n3);
          }
        }

    /*
    for cycle

    */
    fftw_free(cslice);
    fftw_free(cline);
    free(line);
    free(slice);

    /*
    } else {
	      memcpy(fft->fftw_data, data_rec, n1*n2*n3*sizeof(fftw_complex));
	  
	      fftw_execute_dft(fft->bw_plan, fft->fftw_data, fft->fftw_data);
	
	      fac = 1.0 / ( n1 * n2 * n3 );
	
	      for( i = 0; i < n1 * n2 * n3; ++i ) {
	        data_direct[i] = creal(fft->fftw_data[i])*fac;
	      }
    }
    */
}

