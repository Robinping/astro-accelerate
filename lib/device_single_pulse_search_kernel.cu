// Added by Karel Adamek 

#ifndef SINGLE_PULSE_SEARCH_KERNEL_H_
#define SINGLE_PULSE_SEARCH_KERNEL_H_

#include <cuda.h>
#include <cuda_runtime.h>
#include "AstroAccelerate/params.h"

__device__ __constant__ float c_sqrt_taps[PD_MAXTAPS+1];

<<<<<<< HEAD
__global__ void PD_SEARCH_GPU(float const* __restrict__ d_input, float *d_output, float *d_output_taps, float *d_MSD, int maxTaps, int nTimesamples) {
=======
__global__ void PD_SEARCH_GPU(float const* __restrict__ d_input, float *d_output, float *d_output_taps, float *d_MSD, int maxTaps, int nTimesamples)
{
>>>>>>> fe80b9c735d1c898047cbb64bcf8da05cd6a21da
	extern __shared__ float s_input[]; //dynamically allocated memory for now
	
	int f, gpos, spos, itemp;
	float res_SNR[PD_NWINDOWS], SNR, FIR_value, ftemp, signal_mean, signal_sd;
	int res_Taps[PD_NWINDOWS];
<<<<<<< HEAD
	signal_mean=d_MSD[0];
	signal_sd=d_MSD[1];
	
	//----------------------------------------------
	//----> Reading data
	gpos=blockIdx.y*nTimesamples + blockIdx.x*PD_NTHREADS*PD_NWINDOWS;
	spos=threadIdx.x;
	for(f=0; f<PD_NWINDOWS; f++){
		s_input[spos]=__ldg(&d_input[gpos + spos]);
=======
	signal_mean = d_MSD[0];
	signal_sd = d_MSD[1];
	
	//----------------------------------------------
	//----> Reading data
	gpos = blockIdx.y*nTimesamples + blockIdx.x*PD_NTHREADS*PD_NWINDOWS;
	spos = threadIdx.x;
	for(f=0; f<PD_NWINDOWS; f++){
		s_input[spos] = __ldg(&d_input[gpos + spos]);
>>>>>>> fe80b9c735d1c898047cbb64bcf8da05cd6a21da
		spos = spos + blockDim.x;
	}
	
	itemp=PD_NTHREADS*PD_NWINDOWS+maxTaps-1;
<<<<<<< HEAD
	while(spos<itemp){
		s_input[spos]=__ldg(&d_input[gpos + spos]);
=======
	while(spos<itemp)
	{
		s_input[spos] = __ldg(&d_input[gpos + spos]);
>>>>>>> fe80b9c735d1c898047cbb64bcf8da05cd6a21da
		spos = spos + blockDim.x;
	}
	
	//----> SNR for nTaps=1
<<<<<<< HEAD
	spos=threadIdx.x;
	for(f=0; f<PD_NWINDOWS; f++){
		res_SNR[f]=(s_input[spos]-signal_mean)/signal_sd;
		res_Taps[f]=1;
=======
	spos = threadIdx.x;
	for(f=0; f<PD_NWINDOWS; f++)
	{
		res_SNR[f]  = (s_input[spos]-signal_mean)/signal_sd;
		res_Taps[f] = 1;
>>>>>>> fe80b9c735d1c898047cbb64bcf8da05cd6a21da
		spos=spos + blockDim.x;
	}
	
	__syncthreads();
	
	//----------------------------------------------
	//----> FIR calculation loop
<<<<<<< HEAD
	for(f=2; f<=maxTaps; f++){
		ftemp=c_sqrt_taps[f]*signal_sd;
		for(int i=0; i<PD_NWINDOWS; i++){
			spos=threadIdx.x + i*blockDim.x;
			FIR_value=0;
			for(int t=0; t<f; t++){
				FIR_value+=s_input[spos + t];
			}
			SNR=(FIR_value-f*signal_mean)/(ftemp);		
			if(SNR>res_SNR[i]) {
				res_SNR[i]=SNR;
				res_Taps[i]=f;
=======
	for(f=2; f<=maxTaps; f++)
	{
		ftemp = c_sqrt_taps[f]*signal_sd;
		for(int i=0; i<PD_NWINDOWS; i++)
		{
			spos = threadIdx.x + i*blockDim.x;
			FIR_value = 0;
			for(int t=0; t<f; t++)
			    FIR_value+=s_input[spos + t];
			SNR = (FIR_value-f*signal_mean)/(ftemp);		
			if(SNR>res_SNR[i])
			{
				res_SNR[i] = SNR;
				res_Taps[i] = f;
>>>>>>> fe80b9c735d1c898047cbb64bcf8da05cd6a21da
			}
		}
	}

	//----------------------------------------------
	//---- Writing data
<<<<<<< HEAD
	gpos=blockIdx.y*nTimesamples + blockIdx.x*PD_NTHREADS*PD_NWINDOWS;
	spos=threadIdx.x;
	for(int i=0; i<PD_NWINDOWS; i++){
		d_output[gpos + spos]=res_SNR[i];
		d_output_taps[gpos + spos]=res_Taps[i];
		spos = spos + blockDim.x;
	}
}



#endif

=======
	gpos = blockIdx.y*nTimesamples + blockIdx.x*PD_NTHREADS*PD_NWINDOWS;
	spos = threadIdx.x;
	for(int i=0; i<PD_NWINDOWS; i++)
	{
		d_output[gpos + spos] = res_SNR[i];
		d_output_taps[gpos + spos] = res_Taps[i];
		spos = spos + blockDim.x;
	}
}
#endif
>>>>>>> fe80b9c735d1c898047cbb64bcf8da05cd6a21da
