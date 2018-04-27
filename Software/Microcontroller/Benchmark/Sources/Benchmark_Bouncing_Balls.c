/** @file Benchmark_Bouncing_Balls.c
 * @see Benchmark.h for description.
 * @author Adrien RICCIARDI
 */
#include <Benchmark.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Ball sprite width in pixels. */
#define BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_WIDTH 8
/** Ball sprite height in pixels. */
#define BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_HEIGHT 8

/** How many pixels a ball moves in each direction on every frame. */
#define BENCHMARK_BOUNCING_BALLS_BALL_SPEED 2

/** How many balls to handle. */
#define BENCHMARK_BOUNCING_BALLS_COUNT 512

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A ball coordinates and moving speeds. */
typedef struct
{
	signed char X; //!< Horizontal coordinate (a signed char is enough to store all values because the ball will never reach a value greater than 127, due to speed and sprite width).
	signed char Y; //!< Vertical coordinate.
	signed char Vertical_Speed; //!< Vertical moving speed (a positive value goes downward, a negative value goes upward).
	signed char Horizontal_Speed; //!< Horizontal moving speed (a positive value goes rightward, a negative value goes leftward).
} TBenchmarkBouncingBallsBall;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The sprite representing the ball. */
static const unsigned char Benchmark_Bouncing_Balls_Sprite_Ball[] = { 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E };

/** All balls. */
static TBenchmarkBouncingBallsBall Benchmark_Bouncing_Balls[BENCHMARK_BOUNCING_BALLS_COUNT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void BenchmarkBouncingBalls(void)
{
	unsigned short i;
	TBenchmarkBouncingBallsBall *Pointer_Ball;
	
	// Initialize all balls
	for (i = 0; i < BENCHMARK_BOUNCING_BALLS_COUNT; i++)
	{
		// Set random coordinates
		Benchmark_Bouncing_Balls[i].X = SystemRandomGetNumber() % (SYSTEM_DISPLAY_WIDTH - BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_WIDTH);
		Benchmark_Bouncing_Balls[i].Y = SystemRandomGetNumber() % (SYSTEM_DISPLAY_HEIGHT - BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_HEIGHT);
		
		// Set moving speeds
		Benchmark_Bouncing_Balls[i].Vertical_Speed = BENCHMARK_BOUNCING_BALLS_BALL_SPEED;
		Benchmark_Bouncing_Balls[i].Horizontal_Speed = BENCHMARK_BOUNCING_BALLS_BALL_SPEED;
	}
	
	while (1)
	{
		SystemDisplayStartFrameTimer();
		SystemDisplayClearFrameBuffer();
		
		// Handle ball moves
		for (i = 0; i < BENCHMARK_BOUNCING_BALLS_COUNT; i++)
		{
			// Cache ball access
			Pointer_Ball = &Benchmark_Bouncing_Balls[i];
			
			// Handle horizontal bounces
			if (Pointer_Ball->X >= SYSTEM_DISPLAY_WIDTH - BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_WIDTH)
			{
				Pointer_Ball->X = SYSTEM_DISPLAY_WIDTH - BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_WIDTH;
				Pointer_Ball->Vertical_Speed = -BENCHMARK_BOUNCING_BALLS_BALL_SPEED;
			}
			else if (Pointer_Ball->X <= 0)
			{
				Pointer_Ball->X = 0;
				Pointer_Ball->Vertical_Speed = BENCHMARK_BOUNCING_BALLS_BALL_SPEED;
			}
			
			// Handle vertical bounces
			if (Pointer_Ball->Y >= SYSTEM_DISPLAY_HEIGHT - BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_HEIGHT)
			{
				Pointer_Ball->Y = SYSTEM_DISPLAY_HEIGHT - BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_HEIGHT;
				Pointer_Ball->Horizontal_Speed = -BENCHMARK_BOUNCING_BALLS_BALL_SPEED;
			}
			else if (Pointer_Ball->Y <= 0)
			{
				Pointer_Ball->Y = 0;
				Pointer_Ball->Horizontal_Speed = BENCHMARK_BOUNCING_BALLS_BALL_SPEED;
			}
			
			// Display the ball
			SystemDisplayRenderSprite(Pointer_Ball->X, Pointer_Ball->Y, Benchmark_Bouncing_Balls_Sprite_Ball, BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_WIDTH, BENCHMARK_BOUNCING_BALLS_SPRITE_BALL_HEIGHT);
			
			// Update ball moves
			Pointer_Ball->X += Pointer_Ball->Vertical_Speed;
			Pointer_Ball->Y += Pointer_Ball->Horizontal_Speed;
		}
		
		SystemDisplayRenderFrameBuffer();
		SystemDisplayWaitForFrameTimerEnd();
	}
}
