//
//  grid2d_mex.cpp
//  ttcr
//
//  Created by Bernard Giroux on 16-01-29.
//  Copyright © 2016 Bernard Giroux. All rights reserved.
//
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <thread>

#include "mex.h"
#include "class_handle.hpp"

#include "Cell.h"
#include "Grid2Drcsp.h"

using namespace std;
using namespace ttcr;

typedef Grid2D<double,uint32_t,sxz<double>> grid;
typedef Grid2Drcsp<double,uint32_t,Cell<double,Node2Dcsp<double,uint32_t>,sxz<double>>> gridiso;
typedef Grid2Drcsp<double,uint32_t,CellElliptical<double,Node2Dcsp<double,uint32_t>,sxz<double>>> gridaniso;
typedef Grid2Drcsp<double,uint32_t,CellTiltedElliptical<double,Node2Dcsp<double,uint32_t>,sxz<double>>> gridtilted;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // Get the command string
    char cmd[64];
    if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
        mexErrMsgTxt("First input should be a command string less than 64 characters long.");

    //  ---------------------------------------------------------------------------
    // New
    if (!strcmp("new", cmd)) {
        // Check parameters
        if (nlhs != 1) {
            mexErrMsgTxt("New: One output expected.");
        }
        if (nrhs > 4) {
            mexErrMsgTxt("New: max 3 input arguments needed.");
        }
        // Return a handle to a new C++ instance


        double        *xmin, *zmin;
        double        *dx, *dz, *nx_d, *nz_d, *nsnx_d, *nsnz_d;
        uint32_t      nx, nz, nsnx, nsnz;
        size_t        nthreads;

        // ------------------------------------------------------
        //	 grid structure
        // ------------------------------------------------------
        if(!mxIsStruct(prhs[1]))
            mexErrMsgTxt("First argument must be a structure.");

        xmin  = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "xmin") ) );
        zmin  = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "zmin") ) );
        dx    = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "dx") ) );
        dz    = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "dz") ) );
        nx_d  = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "nx") ) );
        nz_d  = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "nz") ) );
        nsnx_d  = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "nsnx") ) );
        nsnz_d  = static_cast<double*>( mxGetPr( mxGetField(prhs[1], 0, "nsnz") ) );

        // ------------------------------------------------------
        // type
        // ------------------------------------------------------

        char type[64];
        if (mxGetString(prhs[2], type, sizeof(type)))
            mexErrMsgTxt("Second input should be a string less than 64 characters long.");

        // ------------------------------------------------------
        // number of threads
        // ------------------------------------------------------
        nthreads = 1;
        if ( nrhs>3 ) {
            size_t mrows = mxGetM(prhs[3]);
            size_t ncols = mxGetN(prhs[3]);
            if( !mxIsDouble(prhs[3]) || mxIsComplex(prhs[3]) ||
               !(mrows==1 && ncols==1) ) {
                mexErrMsgTxt("Input must be a noncomplex scalar double.");
            }

            double *dtmp = mxGetPr( prhs[3] );
            nthreads = round( *dtmp );
        }

        nx = uint32_t(round(*nx_d));
        nz = uint32_t(round(*nz_d));
        nsnx = uint32_t(round(*nsnx_d));
        nsnz = uint32_t(round(*nsnz_d));

        if ( strcmp(type, "iso")==0 ) {
            plhs[0] = convertPtr2Mat<gridiso>(new gridiso(nx, nz,
                    *dx, *dz,
                    *xmin, *zmin,
                    nsnx, nsnz,
                    nthreads));
        } else if ( strcmp(type, "elliptical")==0 ) {
            plhs[0] = convertPtr2Mat<gridaniso>(new gridaniso(nx, nz,
                    *dx, *dz,
                    *xmin, *zmin,
                    nsnx, nsnz,
                    nthreads));
        } else if ( strcmp(type, "tilted")==0 ) {
            plhs[0] = convertPtr2Mat<gridtilted>(new gridtilted(nx, nz,
                    *dx, *dz,
                    *xmin, *zmin,
                    nsnx, nsnz,
                    nthreads));
        } else {
            mexErrMsgTxt("Type not defined.");
        }
        return;
    }

    // Check there is a second input, which should be the class instance handle
    if (nrhs < 2)
        mexErrMsgTxt("Second input should be a class instance handle.");

    // ---------------------------------------------------------------------------
    // Delete
    //
    if (!strcmp("delete", cmd)) {
        // Destroy the C++ object
        destroyObject<grid>(prhs[1]);
        // Warn if other commands were ignored
        if (nlhs != 0 || nrhs != 2)
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
        return;
    }

    // Get the class instance pointer from the second input
    grid *grid_instance = convertMat2Ptr<grid>(prhs[1]);

    // Call the various class methods
    // ---------------------------------------------------------------------------
    // setSlowness
    //
    if (!strcmp("setSlowness", cmd)) {
        // Check parameters
        if (nlhs < 0 || nrhs != 3)
            mexErrMsgTxt("setSlowness: Unexpected arguments.");
        // Call the method

        if (!(mxIsDouble(prhs[2]))) {
            mexErrMsgTxt("Slowness must be double precision.");
        }
        double *slowness = static_cast<double*>( mxGetPr(prhs[2]) );
        mwSize number_of_dims = mxGetNumberOfDimensions(prhs[2]);
        if ( number_of_dims != 2 ) {
            mexErrMsgTxt("Slowness must be a vector (nSlowness by 1).");
        }
        const mwSize *dim_array = mxGetDimensions(prhs[2]);
        size_t nSlowness = static_cast<size_t>( dim_array[0] );
        if ( dim_array[1] != 1 ) {
            mexErrMsgTxt("Slowness must be a vector (nSlowness by 1).");
        }

        vector<double> s(nSlowness);
        for ( size_t n=0; n<s.size(); ++n ) s[n] = slowness[n];

        if ( grid_instance->setSlowness(s) == 1 ) {
            mexErrMsgTxt("Slowness values must be defined for each grid node.");
        }

        return;
    }

    //  ---------------------------------------------------------------------------
    // raytrace
    if (!strcmp("raytrace", cmd)) {
        // Check parameters

        if ( nrhs < 5 && nrhs > 7 ) {
            mexErrMsgTxt("raytrace: Unexpected arguments.");
        }
        if (nlhs > 3) {
            mexErrMsgTxt("raytrace has a maximum of three output argument.");
        }
        //
        // Slowness
        //
        if (!(mxIsDouble(prhs[2]))) {
            mexErrMsgTxt("Slowness must be double precision.");
        }
        mwSize number_of_dims = mxGetNumberOfDimensions(prhs[2]);
        if ( number_of_dims != 2 ) {
            mexErrMsgTxt("Slowness must be a vector (nSlowness by 1).");
        }
        const mwSize *dim_array = mxGetDimensions(prhs[2]);
        size_t nSlowness = static_cast<size_t>( dim_array[0] );
        if ( dim_array[1] != 1 ) {
            mexErrMsgTxt("Slowness must be a vector (nSlowness by 1).");
        }
        double *slowness = static_cast<double*>( mxGetPr(prhs[2]) );
        vector<double> s(nSlowness);
        for ( size_t n=0; n<s.size(); ++n ) s[n] = slowness[n];
        if ( grid_instance->setSlowness(s) == 1 ) {
            mexErrMsgTxt("Slowness values must be defined for each grid cell.");
        }

        size_t itx=3;
        size_t irx=4;
        size_t it0=5;

        // check if next arg is xi (anisotropy ratio)
        if (!(mxIsDouble(prhs[3]))) {
            mexErrMsgTxt("Input must be double precision.");
        }
        number_of_dims = mxGetNumberOfDimensions(prhs[3]);
        if ( number_of_dims != 2 ) {
            mexErrMsgTxt("Input must be a vector.");
        }
        const mwSize *dim_array2 = mxGetDimensions(prhs[3]);
        if ( dim_array2[0] == dim_array[0] && dim_array2[1] == dim_array[1] ) {
            // size equal to slowness, we have xi
            double *xi_p = static_cast<double*>( mxGetPr(prhs[3]) );
            vector<double> xi(nSlowness);
            for ( size_t n=0; n<xi.size(); ++n ) xi[n] = xi_p[n];
            if ( grid_instance->setXi(xi) == 1 ) {
                mexErrMsgTxt("Xi values must be defined for each grid cell.");
            }
            itx++;
            irx++;
            it0++;
        }

        // check if next arg is theta (anisotropy angle)
        if (!(mxIsDouble(prhs[4]))) {
            mexErrMsgTxt("Input must be double precision.");
        }
        number_of_dims = mxGetNumberOfDimensions(prhs[4]);
        if ( number_of_dims != 2 ) {
            mexErrMsgTxt("Input must be a vector.");
        }
        dim_array2 = mxGetDimensions(prhs[4]);
        if ( dim_array2[0] == dim_array[0] && dim_array2[1] == dim_array[1] ) {
            // size equal to slowness, we have theta
            double *t_p = static_cast<double*>( mxGetPr(prhs[4]) );
            vector<double> theta(nSlowness);
            for ( size_t n=0; n<theta.size(); ++n ) theta[n] = t_p[n];
            if ( grid_instance->setTiltAngle(theta) == 1 ) {
                mexErrMsgTxt("theta values must be defined for each grid cell.");
            }
            itx++;
            irx++;
            it0++;
        }
        //
        // Tx
        //
        if (!(mxIsDouble(prhs[itx]))) {
            mexErrMsgTxt("Tx must be double precision.");
        }
        number_of_dims = mxGetNumberOfDimensions(prhs[itx]);
        if ( number_of_dims != 2 ){
            mexErrMsgTxt("Tx must be a rank 2 matrix.");
        }
        dim_array = mxGetDimensions(prhs[itx]);
        size_t nTx = static_cast<size_t>( dim_array[0] );
        if ( dim_array[1] != 3 ) {
            mexErrMsgTxt("Tx: matrix nTx by 3.");
        }
        double *Tx = static_cast<double*>( mxGetPr(prhs[itx]) );

        //
        // Rx
        //
        if (!(mxIsDouble(prhs[irx]))) {
            mexErrMsgTxt("Rx must be double precision.");
        }
        number_of_dims = mxGetNumberOfDimensions(prhs[irx]);
        if ( number_of_dims != 2 ){
            mexErrMsgTxt("Rx must be a rank 2 matrix.");
        }
        dim_array = mxGetDimensions(prhs[irx]);
        size_t nRx = static_cast<size_t>( dim_array[0] );
        if ( dim_array[1] != 3 ) {
            mexErrMsgTxt("Rx: matrix nRx by 3.");
        }
        double *Rx = static_cast<double*>( mxGetPr(prhs[irx]) );

        if ( nTx != nRx ) {
            mexErrMsgTxt("nTx should be equal to nRx.");
        }

        //
        // t0
        //
        double *tTx;
        if ( nrhs == it0+1 ) {
            if (!(mxIsDouble(prhs[it0]))) {
                mexErrMsgTxt("t0 must be double precision.");
            }
            number_of_dims = mxGetNumberOfDimensions(prhs[it0]);
            if ( number_of_dims != 2 ){
                mexErrMsgTxt("t0 must be a rank 2 matrix.");
            }
            dim_array = mxGetDimensions(prhs[it0]);
            size_t nT0 = static_cast<size_t>( dim_array[0] );
            if ( dim_array[1] != 1 || nT0 != nTx ) {
                mexErrMsgTxt("t0: matrix nTx by 1.");
            }
            tTx = static_cast<double*>( mxGetPr(prhs[it0]) );
        } else {
            tTx = new double [nTx];
            for ( size_t n=0; n<nTx; ++n ) tTx[n] = 0.0;
        }

        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(nRx, 1, mxREAL);
        double *t_arr = mxGetPr(plhs[0]);


        /* ------------------------------------------------------
         Optional output variables
         ------------------------------------------------------ */

        mxArray **Rays;

        /*
         Looking for redundants Tx pts
         */
        vector<vector<sxz<double>>> vTx;
        vector<vector<double>> t0;
        vector<vector<size_t>> iTx;
        sxz<double> sxz_tmp;
        sxz_tmp.x = Tx[0];
        // y is ignored
        sxz_tmp.z = Tx[2*nTx];
        vTx.push_back( vector<sxz<double> >(1, sxz_tmp) );
        t0.push_back( vector<double>(1, tTx[0]) );
        iTx.push_back( vector<size_t>(1, 0) );  // indices of Rx corresponding to current Tx
        for ( size_t ntx=1; ntx<nTx; ++ntx ) {
            sxz_tmp.x = Tx[ntx];
            sxz_tmp.z = Tx[ntx+2*nTx];
            bool found = false;

            for ( size_t nv=0; nv<vTx.size(); ++nv ) {
                if ( vTx[nv][0].x==sxz_tmp.x &&
                    vTx[nv][0].z==sxz_tmp.z ) {
                    found = true;
                    iTx[nv].push_back( ntx ) ;
                    break;
                }
            }
            if ( !found ) {
                vTx.push_back( vector<sxz<double>>(1, sxz_tmp) );
                t0.push_back( vector<double>(1, tTx[ntx]) );
                iTx.push_back( vector<size_t>(1, ntx) );
            }
        }

        if ( nrhs == it0 ) {
            delete [] tTx;
        }


        /*
         Looping over all non redundant Tx
         */

        vector<sxz<double>> vRx;
        vector<vector<double>> tt( vTx.size() );
        vector<vector<vector<sxz<double>>>> r_data( vTx.size() );
        vector<vector<siv2<double>>> L_data(nTx);
        vector<vector<vector<siv2<double>>>> l_data( vTx.size() );


        if ( grid_instance->getNthreads() == 1  || vTx.size()<=grid_instance->getNthreads() ) {
            for ( size_t nv=0; nv<vTx.size(); ++nv ) {

                vRx.resize( 0 );
                for ( size_t ni=0; ni<iTx[nv].size(); ++ni ) {
                    sxz_tmp.x = Rx[ iTx[nv][ni] ];
                    sxz_tmp.z = Rx[ iTx[nv][ni]+2*nRx ];
                    vRx.push_back( sxz_tmp );
                }

                if ( nlhs == 3 ) {
                    if ( grid_instance->raytrace(vTx[nv], t0[nv], vRx, tt[nv], r_data[nv], l_data[nv]) == 1 ) {
                        mexErrMsgTxt("Problem while raytracing.");
                    }
                } else if ( nlhs == 2 ) {
                    if ( grid_instance->raytrace(vTx[nv], t0[nv], vRx, tt[nv], r_data[nv]) == 1 ) {
                        mexErrMsgTxt("Problem while raytracing.");
                    }
                } else {
                    if ( grid_instance->raytrace(vTx[nv], t0[nv], vRx, tt[nv]) == 1 ) {
                        mexErrMsgTxt("Problem while raytracing.");
                    }
                }
            }
        } else {
            size_t num_threads = grid_instance->getNthreads() < vTx.size() ? grid_instance->getNthreads() : vTx.size();
            vector<size_t> blk_size(num_threads, 0);
            size_t nj = vTx.size();
            while (nj > 0) {
                for ( size_t i=0; i<num_threads; ++i ) {
                    blk_size[i] += 1;
                    nj -= 1;
                    if ( nj == 0 ) {
                        break;
                    }
                }
            }

            vector<thread> threads(num_threads);
            size_t blk_start = 0;
            for ( size_t i=0; i<num_threads; ++i ) {

                size_t blk_end = blk_start + blk_size[i];

                threads[i]=thread( [&grid_instance,&vTx,&tt,&t0,&Rx,&iTx,&nRx,
                                    &nlhs,&r_data,&l_data,blk_start,blk_end,i]{

                    for ( size_t nv=blk_start; nv<blk_end; ++nv ) {

                        sxz<double> sxz_tmp;
                        vector<sxz<double>> vRx;
                        for ( size_t ni=0; ni<iTx[nv].size(); ++ni ) {
                            sxz_tmp.x = Rx[ iTx[nv][ni] ];
                            sxz_tmp.z = Rx[ iTx[nv][ni]+2*nRx ];
                            vRx.push_back( sxz_tmp );
                        }
                        if ( nlhs == 3 ) {
                            if ( grid_instance->raytrace(vTx[nv], t0[nv], vRx, tt[nv], r_data[nv], l_data[nv], i) == 1 ) {
                                mexErrMsgTxt("Problem while raytracing.");
                            }
                        } else if ( nlhs == 2 ) {
                            if ( grid_instance->raytrace(vTx[nv], t0[nv], vRx, tt[nv], r_data[nv], i) == 1 ) {
                                mexErrMsgTxt("Problem while raytracing.");
                            }
                        } else {
                            if ( grid_instance->raytrace(vTx[nv], t0[nv], vRx, tt[nv], i) == 1 ) {
                                mexErrMsgTxt("Problem while raytracing.");
                            }
                        }
                    }
                });

                blk_start = blk_end;
            }

            std::for_each(threads.begin(),threads.end(),
                          std::mem_fn(&std::thread::join));
        }

        for ( size_t nv=0; nv<vTx.size(); ++nv ) {
            for ( size_t ni=0; ni<iTx[nv].size(); ++ni ) {
                t_arr[ iTx[nv][ni] ] = tt[nv][ni];
            }
        }
        if ( nlhs >= 2 ) {
            // 2rd arg: rays.
            plhs[1] = mxCreateCellMatrix(nRx, 1);
            Rays = (mxArray **) mxCalloc(nRx, sizeof(mxArray *));

            for ( size_t nv=0; nv<vTx.size(); ++nv ) {
                for ( size_t ni=0; ni<iTx[nv].size(); ++ni ) {
                    size_t npts = r_data[nv][ni].size();
                    Rays[ iTx[nv][ni] ] = mxCreateDoubleMatrix(npts, 2, mxREAL);
                    double *rays_p = (double*) mxGetData(Rays[ iTx[nv][ni] ]);
                    for ( size_t np=0; np<npts; ++np ) {
                        rays_p[np] = r_data[nv][ni][np].x;
                        rays_p[np+npts] = r_data[nv][ni][np].z;
                    }
                    mxSetCell( plhs[1], iTx[nv][ni], Rays[ iTx[nv][ni] ] );
                }
            }
        }
        if ( nlhs == 3 ) {

            for ( size_t nv=0; nv<vTx.size(); ++nv ) {
                for ( size_t ni=0; ni<iTx[nv].size(); ++ni ) {
                    L_data[ iTx[nv][ni] ] = l_data[nv][ni];
                }
            }
            if (dynamic_cast<gridiso*>(grid_instance)) {

                mwSize nLmax = 0;
                for ( size_t n=0; n<L_data.size(); ++n ) {
                    nLmax += L_data[n].size();
                }
                plhs[2] = mxCreateSparse(nTx, nSlowness, nLmax, mxREAL);
                double *Lval = mxGetPr( plhs[2] );
                mwIndex *irL  = mxGetIr( plhs[2] );
                mwIndex *jcL  = mxGetJc( plhs[2] );

                size_t k = 0;
                for ( size_t j=0; j<nSlowness; ++j ) {
                    jcL[j] = k;
                    for ( size_t i=0; i<nTx; ++i ) {
                        for ( size_t n=0; n<L_data[i].size(); ++n ) {
                            if ( L_data[i][n].i == j ) {
                                irL[k] = i;
                                Lval[k] = L_data[i][n].v;
                                k++;
                            }
                        }
                    }
                }
                jcL[nSlowness] = k;

            } else if (dynamic_cast<gridaniso*>(grid_instance) ||
                    dynamic_cast<gridtilted*>(grid_instance)) {
                mwSize nLmax = 0;
                for ( size_t n=0; n<L_data.size(); ++n ) {
                    nLmax += L_data[n].size();
                }
                nLmax *= 2;
                plhs[2] = mxCreateSparse(nTx, 2*nSlowness, nLmax, mxREAL);
                double *Lval = mxGetPr( plhs[2] );
                mwIndex *irL  = mxGetIr( plhs[2] );
                mwIndex *jcL  = mxGetJc( plhs[2] );

                size_t k = 0;
                for ( size_t j=0; j<nSlowness; ++j ) {
                    jcL[j] = k;
                    for ( size_t i=0; i<nTx; ++i ) {
                        for ( size_t n=0; n<L_data[i].size(); ++n ) {
                            if ( L_data[i][n].i == j ) {
                                irL[k] = i;
                                Lval[k] = L_data[i][n].v;
                                k++;
                            }
                        }
                    }
                }
                for ( size_t j=nSlowness; j<2*nSlowness; ++j ) {
                    jcL[j] = k;
                    for ( size_t i=0; i<nTx; ++i ) {
                        for ( size_t n=0; n<L_data[i].size(); ++n ) {
                            if ( (L_data[i][n].i+nSlowness) == j ) {
                                irL[k] = i;
                                Lval[k] = L_data[i][n].v2;
                                k++;
                            }
                        }
                    }
                }
                jcL[2*nSlowness] = k;
            }
        }
        return;
    }

    if (!strcmp("get_nthreads", cmd)) {
        // Check parameters

        if ( nrhs > 2 ) {
            mexErrMsgTxt("get_nthreads: No arguments needed.");
        }
        if (nlhs > 1) {
            mexErrMsgTxt("get_nthreads: has a maximum of one output argument.");
        }

        size_t nt = grid_instance->getNthreads();
        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double *ntp = mxGetPr(plhs[0]);
        ntp[0] = (double)nt;

        return;
    }

    if (!strcmp("get_xmin", cmd)) {
        // Check parameters

        if ( nrhs > 2 ) {
            mexErrMsgTxt("get_xmin: No arguments needed.");
        }
        if (nlhs > 1) {
            mexErrMsgTxt("get_xmin: has a maximum of one output argument.");
        }

        double nt = grid_instance->getXmin();
        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double *ntp = mxGetPr(plhs[0]);
        ntp[0] = (double)nt;

        return;
    }

    if (!strcmp("get_zmin", cmd)) {
        // Check parameters

        if ( nrhs > 2 ) {
            mexErrMsgTxt("get_zmin: No arguments needed.");
        }
        if (nlhs > 1) {
            mexErrMsgTxt("get_zmin: has a maximum of one output argument.");
        }

        double nt = grid_instance->getZmin();
        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double *ntp = mxGetPr(plhs[0]);
        ntp[0] = (double)nt;

        return;
    }

    if (!strcmp("get_dx", cmd)) {
        // Check parameters

        if ( nrhs > 2 ) {
            mexErrMsgTxt("get_dx: No arguments needed.");
        }
        if (nlhs > 1) {
            mexErrMsgTxt("get_dx: has a maximum of one output argument.");
        }

        double nt = grid_instance->getDx();
        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double *ntp = mxGetPr(plhs[0]);
        ntp[0] = (double)nt;

        return;
    }

    if (!strcmp("get_dz", cmd)) {
        // Check parameters

        if ( nrhs > 2 ) {
            mexErrMsgTxt("get_dz: No arguments needed.");
        }
        if (nlhs > 1) {
            mexErrMsgTxt("get_dz: has a maximum of one output argument.");
        }

        double nt = grid_instance->getDz();
        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double *ntp = mxGetPr(plhs[0]);
        ntp[0] = (double)nt;

        return;
    }

    if (!strcmp("get_nx", cmd)) {
        // Check parameters

        if ( nrhs > 2 ) {
            mexErrMsgTxt("get_nx: No arguments needed.");
        }
        if (nlhs > 1) {
            mexErrMsgTxt("get_nx: has a maximum of one output argument.");
        }

        uint32_t nt = grid_instance->getNcx();
        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double *ntp = mxGetPr(plhs[0]);
        ntp[0] = (double)nt;

        return;
    }

    if (!strcmp("get_nz", cmd)) {
        // Check parameters

        if ( nrhs > 2 ) {
            mexErrMsgTxt("get_nz: No arguments needed.");
        }
        if (nlhs > 1) {
            mexErrMsgTxt("get_nz: has a maximum of one output argument.");
        }

        uint32_t nt = grid_instance->getNcz();
        /* ------------------------------------------------------
         Output variable
         ------------------------------------------------------ */

        plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double *ntp = mxGetPr(plhs[0]);
        ntp[0] = (double)nt;

        return;
    }

    if (!strcmp("get_type", cmd)) {
        if (dynamic_cast<gridiso*>(grid_instance)) {
            plhs[0] = mxCreateString("iso");
        } else if (dynamic_cast<gridaniso*>(grid_instance)) {
            plhs[0] = mxCreateString("elliptical");
        } else if (dynamic_cast<gridtilted*>(grid_instance)) {
            plhs[0] = mxCreateString("tilted");
        } else {
            plhs[0] = mxCreateString("undefined");
        }
        return;
    }


    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
