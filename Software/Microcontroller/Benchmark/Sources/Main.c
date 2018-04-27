/** @file Main.c
 * Allow to measure computer performances in a reproducible way.
 * @author Adrien RICCIARDI
 */
#include <Benchmark.h>
#include <Strings.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Display a menu and wait for the user to choose one item.
 * @return The chosen item (starting from 0, so item #1 will return 0).
 */
static inline unsigned char MainShowMenu(void)
{
	unsigned char Key;
	
	// Display menu
	SystemDisplayRenderTextString(STRINGS_MENU_TITLE);
	SystemDisplayRenderTextString(STRINGS_MENU_FIRST_CHOICE);
	SystemDisplayRenderTextString(STRINGS_MENU_SECOND_CHOICE);
	SystemDisplayRenderFrameBuffer();
	
	// Wait for the user to press a valid key
	do
	{
		Key = SystemKeyboardReadCharacter();
	} while ((Key < '1') || (Key > '2'));
	
	return Key - '1';
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	SystemInitialize();
	
	// Start the selected benchmark
	switch (MainShowMenu())
	{
		case 0:
			BenchmarkBouncingBalls();
			break;
			
		default:
			BenchmarkFloatingPointArithmetic();
			break;
	}
	
	SystemExitProgram();
}
