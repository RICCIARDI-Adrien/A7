/** @file Main.c
 * Allow to measure computer performances in a reproducible way.
 * @author Adrien RICCIARDI
 */
#include <Benchmark.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	SystemInitialize();
	
	BenchmarkBouncingBalls();
}
