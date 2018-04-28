/** @file Benchmark_Random_Generator.c
 * @see Benchmark.h for description.
 * @author Adrien RICCIARDI
 */
#include <Benchmark.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void BenchmarkRandomNumbersGenerator(void)
{
	const unsigned char Sprite_Pixel = 0x01;
	unsigned char X_Coordinate, Y_Coordinate;
	
	SystemDisplayClearFrameBuffer();
	
	while (1)
	{
		// Randomly generate a pixel coordinate
		X_Coordinate = SystemRandomGetNumber() % SYSTEM_DISPLAY_WIDTH;
		Y_Coordinate = SystemRandomGetNumber() % SYSTEM_DISPLAY_HEIGHT;
		
		// Display the pixel
		SystemDisplayRenderSprite(X_Coordinate, Y_Coordinate, &Sprite_Pixel, 1, 1);
		SystemDisplayRenderFrameBuffer();
	}
}
