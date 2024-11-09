/* Assignement:
 * Here you have to modify the includes, the array sizes and the fftw calls, to
 * use the fftw-mpi
 *
 * Regarding the fftw calls. here is the substitution
 * fftw_plan_dft_3d -> fftw_mpi_plan_dft_3d
 * ftw_execute_dft  > fftw_mpi_execute_dft
 * use fftw_mpi_local_size_3d for local size of the arrays
 *
 * Created by G.P. Brandino, I. Girotto, R. Gebauer
 * Last revision: March 2016
 *
 */

#include <complex.h>
#include <fftw3.h>
#include "utilities.h"
#include <stdbool.h>
#include <string.h>
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




double seconds() {
  /* Return the second elapsed since Epoch (00:00:00 UTC, January 1, 1970) */
  struct timeval tmp;
  double sec;
  gettimeofday(&tmp, (struct timezone *)0);
  sec = tmp.tv_sec + ((double)tmp.tv_usec) / 1000000.0;
  return sec;
}

/*
 *  Index linearization is computed following row-major order.
 *  For more informtion see FFTW documentation:
 *  http://www.fftw.org/doc/Row_002dmajor-Format.html#Row_002dmajor-Format
 *
 */
int index_f(int i1, int i2, int i3, int n1, int n2, int n3) {

  return n3 * n2 * i1 + n3 * i2 + i3;
}

void init_fftw_slice(fftw_dist_handler *fft_1d, fftw_dist_handler *fft_2d ,int n1, int n2, int n3, MPI_Comm comm) {

  int npes, mype;
  int buffer_size = 0;

  fft_1d->mpi_comm = comm;
  fft_2d->mpi_comm = comm;

  /*
   *  Allocate a distributed grid for complex FFT using aligned memory
   * allocation See details here:
   *  http://www.fftw.org/fftw3_doc/Allocating-aligned-memory-in-Fortran.html#Allocating-aligned-memory-in-Fortran
   *  HINT: initialize all global and local dimensions. Consider the first
   * dimension being multiple of the number of processes
   *
   */

  MPI_Comm_size(comm, &npes);
  MPI_Comm_rank(comm, &mype);

  if (((n1 % npes) || (n2 % npes)) && !mype) {

    fprintf(stdout, "\nN1 dimension must be multiple of the number of "
                    "processes. The program will be aborted...\n\n");
    MPI_Abort(comm, 1);
  }

  /* set the field of the structure */
  fft_1d->n1 = n1;
  fft_1d->n2 = n2;
  fft_1d->n3 = n3;
  fft_1d->local_n1 = n1/npes;
  fft_1d->local_n2 = n2/npes;
  fft_1d->local_n1_offset = (mype*n1)/npes;
  fft_1d->global_size_grid = n1*n2*n3;
  fft_1d->local_size_grid = (fft_1d->local_n1)*n2*n3;

  fft_1d->fftw_data = (fftw_complex *)fftw_malloc(n1 * sizeof(fftw_complex));

  fft_1d->fw_plan = fftw_plan_dft_1d(n1, fft_1d->fftw_data, fft_1d->fftw_data,
                                  FFTW_FORWARD, FFTW_ESTIMATE);
  fft_1d->bw_plan = fftw_plan_dft_1d(n1, fft_1d->fftw_data, fft_1d->fftw_data,
                                  FFTW_BACKWARD, FFTW_ESTIMATE);

  fft_2d->n1 = n1;
  fft_2d->n2 = n2;
  fft_2d->n3 = n3;
  fft_2d->local_n1 =n1/npes;
  fft_2d->local_n2 =n2/npes;
  fft_2d->local_n1_offset = (mype*n1)/npes;
  fft_2d->global_size_grid = n1*n2*n3;
  fft_2d->local_size_grid = (fft_1d->local_n1)*n2*n3;

  fft_2d->fftw_data = (fftw_complex *)fftw_malloc(n2 * n3 * sizeof(fftw_complex));

  fft_2d->fw_plan = fftw_plan_dft_2d(n2, n3, fft_2d->fftw_data, fft_2d->fftw_data,
                                  FFTW_FORWARD, FFTW_ESTIMATE);
  fft_2d->bw_plan = fftw_plan_dft_2d(n2, n3, fft_2d->fftw_data, fft_2d->fftw_data,
                                  FFTW_BACKWARD, FFTW_ESTIMATE);

}


void close_fftw(fftw_dist_handler *fft) {
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

void fft_3d(fftw_dist_handler* fft_2d_h, fftw_dist_handler* fft_1d_h,
        double *data_direct, fftw_complex *data_rec,bool direct_to_reciprocal) {

  double fac;
  int index, start_index, end_index, index_buf, i2_loc, i_1_loc;
  int n2 = fft->n2, n3 = fft->n3, n1 = fft->n1, npes, block_dim, nblock;

  fftw_complex *cslice, *cline;
  cslice = ( fftw_complex* ) fftw_malloc( n2 * n3 * sizeof(fftw_complex) );
  cline = ( fftw_complex* ) fftw_malloc( n1* sizeof(fftw_complex) );

  /* Allocate buffers to send and receive data */
  MPI_Comm_size(fft->mpi_comm, &npes);


  // Now distinguish in which direction the FFT is performed
  if (direct_to_reciprocal) {
    for (int i1 = 0; i1 < n1; i1++) {
    // take a slice of real data
    fillSlice(data_direct,slice,i1,n2,n3);
    // fill the 2D FFT structure
    for(int k = 0; k < n2*n3; k++) {
	        fft_2d_h->fftw_data[k]  = slice[k] + 0.0 * I;
	  }
    // execute 2D fftw
    fftw_execute_dft(fft_2d_h->fw_plan, fft_2d_h->fftw_data, fft_2d_h->fftw_data);
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

              for(int i1 = 0; i1 < n1; i1++) {
	                fft_1d_h->fftw_data[i1]  = cline[i1];
	            }
              // execute 1D fftw
              fftw_execute_dft(fft_1d_h->fw_plan, fft_1d_h->fftw_data, fft_1d_h->fftw_data);
              memcpy(cline, fft_1d_h->fftw_data, n1*sizeof(fftw_complex));
              pushCLine(data_rec,cline,i2,i3,n1,n2,n3);
          }
        }


        /*
         * Reorder the different data blocks to be contigous in memory.
         * The new distribution will allow to use the Alltoall function
         *
         */

        // Perform an Alltoall communication

        /*  among i1 dimension */
        for (i3 = 0; i3 < n3; i3++) {
          for (i2 = 0; i2 < /*block_dim*/; i2++) {
            for (i1 = 0; i1 < n1; i1++) {

              // Fill the missing part
            }
          }
        }

        // Perform an Alltoall communication

        /*
         * Reoder the different data blocks to be consistent with the initial
         * distribution.
         *
         */
      } else {

        /* Implement the reverse transform */
      }

}