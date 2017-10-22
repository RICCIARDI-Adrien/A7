/** @file AES.c
 * @see AES.h for description.
 * @author Adrien RICCIARDI
 */
#include <AES.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** This value is fixed in the algorithm. */
#define AES_STATE_ROWS_COUNT 4
/** The AES standard "Nb" parameter. */
#define AES_STATE_COLUMNS_COUNT 4
/** The AES standard "Nk" parameter. */
#define AES_CIPHER_KEY_DOUBLE_WORDS_COUNT 8
/** The AES standard "Nr" parameter. */
#define AES_ROUNDS_COUNT 14

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The S-box. */
static unsigned char AES_Substitution_Box[16][16] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0xF8, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Substitute bytes from the input buffer using the S-box. This is the SubBytes() step of the specification.
 * @param Pointer_Input_Buffer The data to substitute. TODO state
 * @param Pointer_Output_Buffer On output, contain the substituted data. TODO state
 */
static void AESStepSubstituteBytes(unsigned char *Pointer_Input_Buffer, unsigned char *Pointer_Output_Buffer)
{
	unsigned char Substitution_Box_Row, Substitution_Box_Column, i, Byte;
	
	for (i = 0; i < AES_STATE_ROWS_COUNT * AES_STATE_COLUMNS_COUNT; i++)
	{
		// Cache input byte value
		Byte = *Pointer_Input_Buffer;
		
		// Extract S-Box coordinates
		Substitution_Box_Row = Byte >> 4;
		Substitution_Box_Column = Byte & 0x0F;
		
		// Substitute the byte
		*Pointer_Output_Buffer = AES_Substitution_Box[Substitution_Box_Row][Substitution_Box_Column];
		
		Pointer_Input_Buffer++;
		Pointer_Output_Buffer++;
	}
}

/** Rotate row 1 to one position on the left, row 2 to two positions on the left and row 3 to three positions on the left.
 * @param Pointer_Input_State The state to shift.
 * @param Pointer_Output_State On output, contain the shifted state.
 */
static void AESStepShiftRows(unsigned char Pointer_Input_State[][AES_STATE_COLUMNS_COUNT], unsigned char Pointer_Output_State[][AES_STATE_COLUMNS_COUNT])
{
	// Row 0 is left untouched (row 0 copy could be avoided to gain some speed but it is safer to execute it because Pointer_Output_State count contain everything)
	Pointer_Output_State[0][0] = Pointer_Input_State[0][0];
	Pointer_Output_State[0][1] = Pointer_Input_State[0][1];
	Pointer_Output_State[0][2] = Pointer_Input_State[0][2];
	Pointer_Output_State[0][3] = Pointer_Input_State[0][3];
	
	// Shift row 1
	Pointer_Output_State[1][0] = Pointer_Input_State[1][1];
	Pointer_Output_State[1][2] = Pointer_Input_State[1][3];
	Pointer_Output_State[1][1] = Pointer_Input_State[1][2];
	Pointer_Output_State[1][3] = Pointer_Input_State[1][0];
	
	// Shift row 2
	Pointer_Output_State[2][0] = Pointer_Input_State[2][2];
	Pointer_Output_State[2][1] = Pointer_Input_State[2][3];
	Pointer_Output_State[2][2] = Pointer_Input_State[2][0];
	Pointer_Output_State[2][3] = Pointer_Input_State[2][1];
	
	// Shift row 3
	Pointer_Output_State[3][0] = Pointer_Input_State[3][3];
	Pointer_Output_State[3][1] = Pointer_Input_State[3][0];
	Pointer_Output_State[3][2] = Pointer_Input_State[3][1];
	Pointer_Output_State[3][3] = Pointer_Input_State[3][2];
}

/** Execute the MixColumns() AES step.
 * @param Pointer_Input_State The state to mix columns.
 * @param Pointer_Output_State On output, contains mixed columns state.
 */
static void AESStepMixColumns(unsigned char Pointer_Input_State[][AES_STATE_COLUMNS_COUNT], unsigned char Pointer_Output_State[][AES_STATE_COLUMNS_COUNT])
{
	unsigned char i;
	
	for (i = 0; i < AES_STATE_COLUMNS_COUNT; i++)
	{
		// Compute column values
		Pointer_Output_State[0][i] = (2 * Pointer_Input_State[0][i]) ^ (3 * Pointer_Input_State[1][i]) ^ Pointer_Input_State[2][i] ^ Pointer_Input_State[3][i];
		Pointer_Output_State[1][i] = Pointer_Input_State[0][i] ^ (2 * Pointer_Input_State[1][i]) ^ (3 * Pointer_Input_State[2][i]) ^ Pointer_Input_State[3][i];
		Pointer_Output_State[2][i] = Pointer_Input_State[0][i] ^ Pointer_Input_State[1][i] ^ (2 * Pointer_Input_State[2][i]) ^ (3 * Pointer_Input_State[3][i]);
		Pointer_Output_State[3][i] = (3 * Pointer_Input_State[0][i]) ^ Pointer_Input_State[1][i] ^ Pointer_Input_State[2][i] ^ (2 * Pointer_Input_State[3][i]);
	}
}
