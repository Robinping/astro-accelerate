#include "AstroAccelerateTest.h"
#include "../../AstroAccelerateParameters.h"
#include "../../DedispersionStrategy.h"
#include "../../DedispersionStrategyFile.h"
#include "../../DmTime.h"
#include "../../AstroAccelerate.h"

#include <vector>

namespace astroaccelerate {
namespace test {

AstroAccelerateTest::AstroAccelerateTest()
    : ::testing::Test()
{
}

AstroAccelerateTest::~AstroAccelerateTest()
{
}

void AstroAccelerateTest::SetUp()
{
}

void AstroAccelerateTest::TearDown()
{
}

class TestParams : public AstroAccelerateParameters<TestParams> {};

// test AstroAccelerate class
TEST_F(AstroAccelerateTest, AstroAccelerate_call)
{
	// Following is ok for ska_karel.txt
	//char* filename = my_argv[1] + strlen(my_argv[1]) - 13;
	//if(strcmp(filename, "ska_karel.txt") == 0)
	//{
		// Internal code variables
		// File pointers
		FILE *fp = NULL;
		// Counters and flags
		int range = 0;
		int nb_selected_dm = 0;
		int enable_debug = 0;
		int enable_analysis = 0;
		int enable_acceleration = 0;
		int enable_periodicity = 0;
		int output_dmt = 0;
		int enable_zero_dm = 0;
		int enable_zero_dm_with_outliers = 0;
		int enable_rfi = 0;
		int enable_fdas_custom_fft = 0;
		int enable_fdas_inbin = 0;
		int enable_fdas_norm = 0;
		int enable_output_ffdot_plan = 0;
		int enable_output_fdas_list = 0;
		int analysis_debug = 0;
	    int *inBin = NULL;
		int *outBin = NULL;
		int multi_file = 1;
		int candidate_algorithm=0;
		int failsafe = 0;
		// Memory sizes and pointers
		float *user_dm_low = NULL;
		float *user_dm_high = NULL;
		float *user_dm_step = NULL;
		float *selected_dm_low = NULL;
		float *selected_dm_high = NULL;
		// Telescope parameters
		int nchans = 0;
		int nsamp = 0;
		int nbits = 0;
		int nsamples = 0;
		int nifs = 0;
		int nboots = -1;
		int ntrial_bins;
		int navdms = 1;
		int nsearch = 3;
		float aggression = 2.5;
		float narrow = 0.001f;
		float wide = 0.1f;
		float tstart = 0.0f;
		float tsamp = 0.0f;
		float fch1 = 0.0f;
		float foff = 0.0f;
		// Analysis variables
		float power = 2.0f;
		float sigma_cutoff = 6.0f;
		float sigma_constant = 4.0f;
		float max_boxcar_width_in_sec = 0.5f;

		// Users desired de-dispersion strategy. Pick up user defined values from the CLI.
		get_user_input(&fp, my_argc, my_argv, &multi_file, &enable_debug, &enable_analysis,
		    &enable_periodicity, &enable_acceleration, &enable_output_ffdot_plan,
		    &enable_output_fdas_list, &output_dmt, &enable_zero_dm,
		    &enable_zero_dm_with_outliers, &enable_rfi, &enable_fdas_custom_fft,
		    &enable_fdas_inbin, &enable_fdas_norm, &nboots, &ntrial_bins, &navdms,
		    &narrow, &wide, &aggression, &nsearch, &inBin, &outBin, &power, &sigma_cutoff,
		    &sigma_constant, &max_boxcar_width_in_sec, &range, &user_dm_low, &user_dm_high,
		    &user_dm_step, &candidate_algorithm, &selected_dm_low, &selected_dm_high, &nb_selected_dm, &analysis_debug, &failsafe);
		// Reads telescope parameters from the header of the input file and then counts the number of samples in the input data file.
		get_file_data(&fp, &nchans, &nsamples, &nsamp, &nifs, &nbits, &tsamp, &tstart,
		&fch1, &foff);


		//std::vector<float> bin_frequencies(nchans, 0.0);


		// dedispersion
		//DedispersionStrategy dedispersion_strategy;
		//DedispersionStrategyFile(&fp, my_argc, my_argv, dedispersion_strategy, gpu_memory);
		/*
		DedispersionStrategy dedispersion_strategy
									 (user_dm_low
									 ,user_dm_high
									 ,user_dm_step
									 ,inBin
									 ,outBin
									 ,gpu_memory
									 ,power
									 ,range
									 ,nchans
									 ,nsamples
									 ,nsamp
									 ,nifs
									 ,nbits
									 ,tsamp
									 ,tstart
									 ,sigma_cutoff
									 ,sigma_constant
									 ,max_boxcar_width_in_sec
									 ,narrow
									 ,wide
									 ,nboots
									 ,navdms
									 ,ntrial_bins
									 ,nsearch
									 ,aggression
                                     ,bin_frequencies);
        }
        */

		// Initialise the GPU.
		int device_id = 0;
		size_t gpu_memory = 0;
		cudaSetDevice(device_id);
		size_t mem_free, total;
		cudaMemGetInfo(&mem_free, &total);
		gpu_memory = ( mem_free );

		DedispersionStrategy dedispersion_strategy
											 (user_dm_low
											 ,user_dm_high
											 ,user_dm_step
											 ,inBin
											 ,outBin
											 ,gpu_memory
											 ,power
											 ,range
											 ,nchans
											 ,nsamples
											 ,nsamp
											 ,nifs
											 ,nbits
											 ,tsamp
											 ,tstart
											 ,fch1
											 ,foff
											 ,sigma_cutoff
											 ,sigma_constant
											 ,max_boxcar_width_in_sec
											 ,narrow
											 ,wide
											 ,nboots
											 ,navdms
											 ,ntrial_bins
											 ,nsearch ,aggression);



		// input buffer
		unsigned short *input_buffer = nullptr;
		size_t inputsize = 0;
		allocate_memory_cpu_input(dedispersion_strategy.get_nsamp(), dedispersion_strategy.get_nchans(), &input_buffer,&inputsize);
		get_recorded_data(&fp, dedispersion_strategy.get_nsamp(), dedispersion_strategy.get_nchans(), dedispersion_strategy.get_nbits(),
						  &input_buffer, &inputsize);

		printf("\nAA is starting\n");
		// dedispersed data
		DmTime<float> output_buffer(dedispersion_strategy);
		// output of sps - assume it's a quarter of the output size
		std::vector<float> output_sps;

		astroaccelerate::AstroAccelerate<TestParams> astroaccelerate(dedispersion_strategy);
		astroaccelerate.run_dedispersion_sps(device_id
						,input_buffer
						,output_buffer
						,output_sps
						);
		//*/
		fclose(fp);
		free(inBin);
		free(outBin);
		free(user_dm_low);
		free(user_dm_high);
		free(user_dm_step);
		free(input_buffer);
	}
//}

} // namespace test
} // namespace astroaccelerate

