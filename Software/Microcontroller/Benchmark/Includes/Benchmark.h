/** @file Benchmark.h
 * Gather all available benchmarks.
 * @author Adrien RICCIARDI
 */
#ifndef H_BENCHMARK_H
#define H_BENCHMARK_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display a huge amount of balls bouncing around display corners. */
void BenchmarkBouncingBalls(void);

/** Extract the square root of integer numbers and display the results. */
void BenchmarkFloatingPointArithmetic(void);

/** Check if the random numbers generator can generate all numbers of a specific set. */
void BenchmarkRandomNumbersGenerator(void);

#endif
