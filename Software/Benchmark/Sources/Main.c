/** @file Main.c
 * Allow to measure computer performances in a reproducible way.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Ball sprite width in pixels. */
#define SPRITE_BALL_WIDTH 8
/** Ball sprite height in pixels. */
#define SPRITE_BALL_HEIGHT 8

/** How many pixels a ball moves in each direction on every frame. */
#define BALL_SPEED 2

/** How many balls to handle. */
#define BALLS_COUNT 512

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
} TBall;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The sprite representing the ball. */
static const unsigned char Sprite_Ball[] = { 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E };

/** All balls. */
static TBall Balls[BALLS_COUNT];

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned short i;
	TBall *Pointer_Ball;
	
	SystemInitialize();
	
	// Initialize all balls
	for (i = 0; i < BALLS_COUNT; i++)
	{
		// Set random coordinates
		Balls[i].X = SystemRandomGetNumber() % (SYSTEM_DISPLAY_WIDTH - SPRITE_BALL_WIDTH);
		Balls[i].Y = SystemRandomGetNumber() % (SYSTEM_DISPLAY_HEIGHT - SPRITE_BALL_HEIGHT);
		
		// Set moving speeds
		Balls[i].Vertical_Speed = BALL_SPEED;
		Balls[i].Horizontal_Speed = BALL_SPEED;
	}
	
	while (1)
	{
		SystemDisplayStartFrameTimer();
		SystemDisplayClearFrameBuffer();
		
		// Handle ball moves
		for (i = 0; i < BALLS_COUNT; i++)
		{
			// Cache ball access
			Pointer_Ball = &Balls[i];
			
			// Handle horizontal bounces
			if (Pointer_Ball->X >= SYSTEM_DISPLAY_WIDTH - SPRITE_BALL_WIDTH)
			{
				Pointer_Ball->X = SYSTEM_DISPLAY_WIDTH - SPRITE_BALL_WIDTH;
				Pointer_Ball->Vertical_Speed = -BALL_SPEED;
			}
			else if (Pointer_Ball->X <= 0)
			{
				Pointer_Ball->X = 0;
				Pointer_Ball->Vertical_Speed = BALL_SPEED;
			}
			
			// Handle vertical bounces
			if (Pointer_Ball->Y >= SYSTEM_DISPLAY_HEIGHT - SPRITE_BALL_HEIGHT)
			{
				Pointer_Ball->Y = SYSTEM_DISPLAY_HEIGHT - SPRITE_BALL_HEIGHT;
				Pointer_Ball->Horizontal_Speed = -BALL_SPEED;
			}
			else if (Pointer_Ball->Y <= 0)
			{
				Pointer_Ball->Y = 0;
				Pointer_Ball->Horizontal_Speed = BALL_SPEED;
			}
			
			// Display the ball
			SystemDisplayRenderSprite(Pointer_Ball->X, Pointer_Ball->Y, Sprite_Ball, SPRITE_BALL_WIDTH, SPRITE_BALL_HEIGHT);
			
			// Update ball moves
			Pointer_Ball->X += Pointer_Ball->Vertical_Speed;
			Pointer_Ball->Y += Pointer_Ball->Horizontal_Speed;
		}
		
		SystemDisplayRenderFrameBuffer();
		SystemDisplayWaitForFrameTimerEnd();
	}
}
