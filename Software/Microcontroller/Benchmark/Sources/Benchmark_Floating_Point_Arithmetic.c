/** @file Benchmark_Floating_Point_Arithmetic.c
 * @see Benchmark.h for description.
 * @author Adrien RICCIARDI
 */
#include <Benchmark.h>
#include <math.h>
#include <stdio.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void BenchmarkFloatingPointArithmetic(void)
{
	double Value = 0, Result;
	unsigned char String_Result[64]; // Should be big enough, some margin must be taken as there is no snprinf() function available
	
	// Clear display and return display to home
	SystemDisplayClear();
	SystemDisplayClearFrameBuffer();
	SystemDisplaySetTextCursor(0, 0);
	
	while (1)
	{
		Result = sqrt(Value);
		
		// Display value and corresponding result
		sprintf(String_Result, "%.0f = %.4f\n", Value, Result);
		SystemDisplayRenderTextString(String_Result);
		SystemDisplayRenderFrameBuffer();
		
		Value++;
	}
}
