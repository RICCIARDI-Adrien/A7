/** @file Main.c
 * The classic pong game. It can be played by two players sharing the same keyboard.
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

#define BALL_SPEED 2

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The sprite representing the ball. */
static const unsigned char Sprite_Ball[] = { 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E };
//static const unsigned char Sprite_Ball[] = { 0xC0, 0x30, 0x2C, 0x23, 0x23, 0x2C, 0x30, 0xC0 };

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	signed short Ball_X = 0, Ball_Y = 0, Ball_Vertical_Speed = BALL_SPEED, Ball_Horizontal_Speed = BALL_SPEED;
	
	SystemInitialize();
	
	while (1)
	{
		SystemDisplayBeginRendering();
		
		SystemDisplayRenderSprite(Ball_X, Ball_Y, Sprite_Ball, SPRITE_BALL_WIDTH, SPRITE_BALL_HEIGHT);
		
		SystemDisplayEndRendering();
		__delay_ms(66);
		//__delay_ms(16);
				
		Ball_X += Ball_Vertical_Speed;
		Ball_Y += Ball_Horizontal_Speed;
		
		if (Ball_X > SYSTEM_DISPLAY_WIDTH - SPRITE_BALL_WIDTH)
		{
			Ball_X = SYSTEM_DISPLAY_WIDTH - SPRITE_BALL_WIDTH;
			Ball_Vertical_Speed = -BALL_SPEED;
		}
		else if (Ball_X < 0)
		{
			Ball_X = 0;
			Ball_Vertical_Speed = BALL_SPEED;
		}
		
		if (Ball_Y > SYSTEM_DISPLAY_HEIGHT - SPRITE_BALL_HEIGHT)
		{
			Ball_Y = SYSTEM_DISPLAY_HEIGHT - SPRITE_BALL_HEIGHT;
			Ball_Horizontal_Speed = -BALL_SPEED;
		}
		else if (Ball_Y < 0)
		{
			Ball_Y = 0;
			Ball_Horizontal_Speed = BALL_SPEED;
		}
	}
}
