/** @file Main.c
 * Space invaders are back... But this time they are rushing at you ! How many time will you survive without colliding an Invader ?
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Gather sprite pixels data and dimensions. */ // TODO put in system library
typedef struct
{
	unsigned char Width; //!< Sprite width in pixels.
	unsigned char Height; //!< Sprite height in pixels.
	const unsigned char *Pointer_Pixels_Data; //<! The picture to display.
} TSprite;

/** All available sprites index. */
typedef enum
{
	SPRITE_ID_INVADER_0,
	SPRITE_ID_INVADER_1,
	SPRITE_ID_INVADER_2,
	SPRITE_ID_INVADER_3,
	SPRITE_ID_INVADER_4,
	SPRITE_ID_INVADER_5,
	SPRITE_IDS_COUNT
} TSpriteID;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The first invader sprite. */
static const unsigned char Sprite_Invader_0[] = { 0x1C, 0x5E, 0xFE, 0xB6, 0x37, 0x5F, 0x5F, 0x37, 0xB6, 0xFE, 0x5E, 0x1C };
/** The second invader sprite. */
static const unsigned char Sprite_Invader_1[] = { 0x9C, 0x9E, 0x5E, 0x76, 0x37, 0x5F, 0x5F, 0x37, 0x76, 0x5E, 0x9E, 0x9C };
/** The third invader sprite. */
static const unsigned char Sprite_Invader_2[] = { 0x70, 0x18, 0x7D, 0xB6, 0xBC, 0x3C, 0xBC, 0xB6, 0x7D, 0x18, 0x70 };
/** The fourth invader sprite. */
static const unsigned char Sprite_Invader_3[] = { 0x1E, 0xB8, 0x7D, 0x36, 0x3C, 0x3C, 0x3C, 0x36, 0x7D, 0xB8, 0x1E };
/** The fifth invader sprite. */
static const unsigned char Sprite_Invader_4[] = { 0x58, 0xBC, 0x16, 0x3F, 0x3F, 0x16, 0xBC, 0x58 };
/** The sixth invader sprite. */
static const unsigned char Sprite_Invader_5[] = { 0x98, 0x5C, 0xB6, 0x5F, 0x5F, 0xB6, 0x5C, 0x98 };

/** All available sprites. */
static TSprite Sprites[SPRITE_IDS_COUNT] =
{
	{
		12,
		8,
		Sprite_Invader_0
	},
	{
		12,
		8,
		Sprite_Invader_1
	},
	{
		11,
		8,
		Sprite_Invader_2
	},
	{
		11,
		8,
		Sprite_Invader_3
	},
	{
		8,
		8,
		Sprite_Invader_4
	},
	{
		8,
		8,
		Sprite_Invader_5
	}
};

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	SystemInitialize();
	
	while (1)
	{
		SystemDisplayClear();
		
		SystemDisplayRenderSprite(0, 0, Sprites[SPRITE_ID_INVADER_0].Pointer_Pixels_Data, Sprites[SPRITE_ID_INVADER_0].Width, Sprites[SPRITE_ID_INVADER_0].Height);
		SystemDisplayRenderSprite(16, 0, Sprites[SPRITE_ID_INVADER_1].Pointer_Pixels_Data, Sprites[SPRITE_ID_INVADER_1].Width, Sprites[SPRITE_ID_INVADER_1].Height);
		SystemDisplayRenderSprite(32, 0, Sprites[SPRITE_ID_INVADER_2].Pointer_Pixels_Data, Sprites[SPRITE_ID_INVADER_2].Width, Sprites[SPRITE_ID_INVADER_2].Height);
		SystemDisplayRenderSprite(48, 0, Sprites[SPRITE_ID_INVADER_3].Pointer_Pixels_Data, Sprites[SPRITE_ID_INVADER_3].Width, Sprites[SPRITE_ID_INVADER_3].Height);
		SystemDisplayRenderSprite(64, 0, Sprites[SPRITE_ID_INVADER_4].Pointer_Pixels_Data, Sprites[SPRITE_ID_INVADER_4].Width, Sprites[SPRITE_ID_INVADER_4].Height);
		SystemDisplayRenderSprite(80, 0, Sprites[SPRITE_ID_INVADER_5].Pointer_Pixels_Data, Sprites[SPRITE_ID_INVADER_5].Width, Sprites[SPRITE_ID_INVADER_5].Height);
		
		SystemDisplayUpdate();
		
		while (1);
	}
}
