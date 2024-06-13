#include "App/pv_pulse.h"

#include <stdint.h>
#include <string.h>

uint8_t adc_to_digital( uint16_t sample, uint16_t mean) {
    if( sample >= mean ) return 1;
    return 0;
}

int get_pulse( uint16_t* adc_buf, uint16_t arr_size) {

    uint16_t samples[arr_size];
    // __disable_irq();
    memcpy(&samples, adc_buf, arr_size*2);
    // __enable_irq();

    // Calculate pulse

    // Prescaler ADC_GET_CLOCK_PRESCALER
    // TODO: calculate period
    float period = 1;


    // Calculate mean value
    uint16_t min = 0;
    uint16_t max = 0;
    for( uint16_t i=0; i < arr_size; i++ ) {
        // printf("%d %d\n", i, samples[i]);
		if( samples[i] > max)
            max = i;
        if(  samples[i] < min)
            min = i;
    }

    uint16_t mean = (max-min) / 2;
	// printf("samples end\n max: %d min %d mean: %d\n", max, min, mean);

    // Calculate samples in period
    uint8_t prev = adc_to_digital(samples[0],mean);
    uint8_t edge_count = 0;
    uint16_t sample_count = 0;
    uint16_t samples_per_period[255] = {0};
    uint16_t samples_per_period_i = 0;

    for( uint16_t i=0; i < arr_size; i++ ) {
        if( prev != adc_to_digital(samples[i], mean) ) {
            edge_count++;
            if( edge_count > 1 ) {
                samples_per_period[samples_per_period_i] = sample_count;
                if(samples_per_period_i < 254)
                {
                	samples_per_period_i++;
                }
                sample_count =0;
            }
        } else {

        }
        sample_count++;
    }
	// printf("period 1: %d\n", samples_per_period[0]);


    uint16_t samples_per_period_sum = 0;

    for( uint16_t i=0; i<(sizeof(samples_per_period)/sizeof(samples_per_period[0])); i++) {
        if( i < samples_per_period_i ) {
			samples_per_period_sum+=samples_per_period[i];
		} else {
			samples_per_period[i]=0;
		}
		// printf(" %ld ",samples_per_period[i] );
    }

	// printf("Done %d %d\n", samples_per_period_sum, samples_per_period_i);
	uint16_t samples_per_period_mean = samples_per_period_sum / samples_per_period_i;
    // printf("\n%d", samples_per_period_mean);
    return samples_per_period_mean * period;

}