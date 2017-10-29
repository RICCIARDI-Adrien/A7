/** @file AES.c
 * @see AES.h for description.
 * @author Adrien RICCIARDI
 */
#include <AES.h>
#include <string.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** This value is fixed in the algorithm. */
#define AES_STATE_ROWS_COUNT 4
/** The AES standard "Nb" parameter. */
#define AES_STATE_COLUMNS_COUNT 4
/** The AES standard "Nk" parameter. */
#define AES_CIPHER_KEY_WORDS_COUNT 8
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
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

/** AES Rcon array. Only the constants used in AES-256 were kept (see https://en.wikipedia.org/wiki/Rijndael_key_schedule).
 * @note Values are expanded to 32-bit yet to improve speed.
 */
static unsigned long AES_Round_Constants[] =
{
	0x8d000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000
};

/** The key schedule, computed from the provided key. */
static unsigned long AES_Key_Schedule[AES_STATE_COLUMNS_COUNT * (AES_ROUNDS_COUNT + 1)];

/** This buffer is used by cipher and decipher functions to share data between successive update() function calls. */
static unsigned char AES_Internal_Block_Buffer[AES_STATE_ROWS_COUNT][AES_STATE_COLUMNS_COUNT];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Substitute bytes from the input buffer using the S-box. This is the SubBytes() step of the specification.
 * @param Pointer_Input_Buffer The data to substitute.
 * @param Pointer_Output_Buffer On output, contain the substituted data.
 */
static void AESStepSubstituteBytes(unsigned char Pointer_Input_Buffer[][AES_STATE_COLUMNS_COUNT], unsigned char Pointer_Output_Buffer[][AES_STATE_COLUMNS_COUNT])
{
	unsigned char Substitution_Box_Row, Substitution_Box_Column, i, Byte, *Pointer_Input_Buffer_Bytes, *Pointer_Output_Buffer_Bytes;
	
	// One-dimension arrays allow a faster and simpler code in this case
	Pointer_Input_Buffer_Bytes = (unsigned char *) Pointer_Input_Buffer;
	Pointer_Output_Buffer_Bytes = (unsigned char *) Pointer_Output_Buffer;
	
	for (i = 0; i < AES_STATE_ROWS_COUNT * AES_STATE_COLUMNS_COUNT; i++)
	{
		// Cache input byte value
		Byte = *Pointer_Input_Buffer_Bytes;
		
		// Extract S-Box coordinates
		Substitution_Box_Row = Byte >> 4;
		Substitution_Box_Column = Byte & 0x0F;
		
		// Substitute the byte
		*Pointer_Output_Buffer_Bytes = AES_Substitution_Box[Substitution_Box_Row][Substitution_Box_Column];
		
		Pointer_Input_Buffer_Bytes++;
		Pointer_Output_Buffer_Bytes++;
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
	Pointer_Output_State[1][1] = Pointer_Input_State[1][2];
	Pointer_Output_State[1][2] = Pointer_Input_State[1][3];
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

/** Multiply the provided value by 2 in Rijndael's Galois field.
 * @param Value The value to multiply.
 * @return The multiplied value.
 */
static inline unsigned char AESMixColumnMultiplyByTwo(unsigned char Value)
{
	unsigned char Shifted_Value;
	
	// Following magic comes from http://www.angelfire.com/biz7/atleast/mix_columns.pdf.
	Shifted_Value = Value << 1;
	if (Value & 0x80) Shifted_Value ^= 0x1B;
	
	return Shifted_Value;
}

/** Multiply the provided value by 3 in Rijndael's Galois field.
 * @param Value The value to multiply.
 * @return The multiplied value.
 */
static inline unsigned char AESMixColumnMultiplyByThree(unsigned char Value)
{
	// Following magic comes from https://crypto.stackexchange.com/questions/2402/how-to-solve-mixcolumns.
	return AESMixColumnMultiplyByTwo(Value) ^ Value;
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
		Pointer_Output_State[0][i] = AESMixColumnMultiplyByTwo(Pointer_Input_State[0][i]) ^ AESMixColumnMultiplyByThree(Pointer_Input_State[1][i]) ^ Pointer_Input_State[2][i] ^ Pointer_Input_State[3][i];
		Pointer_Output_State[1][i] = Pointer_Input_State[0][i] ^ AESMixColumnMultiplyByTwo(Pointer_Input_State[1][i]) ^ AESMixColumnMultiplyByThree(Pointer_Input_State[2][i]) ^ Pointer_Input_State[3][i];
		Pointer_Output_State[2][i] = Pointer_Input_State[0][i] ^ Pointer_Input_State[1][i] ^ AESMixColumnMultiplyByTwo(Pointer_Input_State[2][i]) ^ AESMixColumnMultiplyByThree(Pointer_Input_State[3][i]);
		Pointer_Output_State[3][i] = AESMixColumnMultiplyByThree(Pointer_Input_State[0][i]) ^ Pointer_Input_State[1][i] ^ Pointer_Input_State[2][i] ^ AESMixColumnMultiplyByTwo(Pointer_Input_State[3][i]);
	}
}

/** Rotate a word bytes according to AES key expansion.
 * @param Word The word to rotate.
 * @return The rotated word.
 * @note This function is optimized for PIC18 cores that can't shift more than one bit at a time, but can handle two pointers at a time.
 */
static inline unsigned long AESKeyExpansionRotateWord(unsigned long Word)
{
	unsigned long Rotated_Word;
	unsigned char *Pointer_Word_Bytes = (unsigned char *) &Word, *Pointer_Rotated_Word_Bytes = (unsigned char *) &Rotated_Word;
	
	Pointer_Rotated_Word_Bytes[0] = Pointer_Word_Bytes[3];
	Pointer_Rotated_Word_Bytes[1] = Pointer_Word_Bytes[0];
	Pointer_Rotated_Word_Bytes[2] = Pointer_Word_Bytes[1];
	Pointer_Rotated_Word_Bytes[3] = Pointer_Word_Bytes[2];
	
	return Rotated_Word;
}

/** Substitute all word byte values with corresponding S-Box ones.
 * @param Word The word to substitute.
 * @return The substituted word.
 */
static inline unsigned long AESKeyExpansionSubstituteWord(unsigned long Word)
{
	unsigned long Substituted_Word;
	unsigned char i, *Pointer_Word_Bytes = (unsigned char *) &Word, *Pointer_Substituted_Word_Bytes = (unsigned char *) &Substituted_Word, Byte, Substitution_Box_Row, Substitution_Box_Column;
	
	for (i = 0; i < 4; i++)
	{
		// Cache word next byte
		Byte = *Pointer_Word_Bytes;
		
		// Extract S-Box coordinates
		Substitution_Box_Row = Byte >> 4;
		Substitution_Box_Column = Byte & 0x0F;
		
		// Substitute the byte
		*Pointer_Substituted_Word_Bytes = AES_Substitution_Box[Substitution_Box_Row][Substitution_Box_Column];
		
		Pointer_Word_Bytes++;
		Pointer_Substituted_Word_Bytes++;
	}
	
	return Substituted_Word;
}

/** Expand the provided 256-bit key.
 * @param Pointer_AES_Key The 32-byte AES key.
 * @note This is the specification key expansion algorithm with little improvements because only AES-256 is supported.
 */
static inline void AESKeyExpansion(unsigned char *Pointer_AES_Key)
{
	unsigned char i, j, Index_Remainder;
	unsigned long Temp;
	
	// Fill the key schedule first words with the key
	for (i = 0; i < AES_CIPHER_KEY_WORDS_COUNT; i++)
	{
		// Convert 4 bytes into a word (loop is manually unrolled to make sure to save execution time)
		AES_Key_Schedule[i] = Pointer_AES_Key[0];
		AES_Key_Schedule[i] = (AES_Key_Schedule[i] << 8) | Pointer_AES_Key[1];
		AES_Key_Schedule[i] = (AES_Key_Schedule[i] << 8) | Pointer_AES_Key[2];
		AES_Key_Schedule[i] = (AES_Key_Schedule[i] << 8) | Pointer_AES_Key[3];
		Pointer_AES_Key += 4;
	}
	
	// Expand the key to generate all remaining needed values
	for (i = AES_CIPHER_KEY_WORDS_COUNT; i < AES_STATE_COLUMNS_COUNT * (AES_ROUNDS_COUNT + 1); i++)
	{
		Temp = AES_Key_Schedule[i - 1];
		
		// Cache modulo operation result as it is slow to compute
		Index_Remainder = i % AES_CIPHER_KEY_WORDS_COUNT;
		if (Index_Remainder == 0) Temp = AESKeyExpansionSubstituteWord(AESKeyExpansionRotateWord(Temp)) ^ AES_Round_Constants[i / AES_CIPHER_KEY_WORDS_COUNT];
		else if (Index_Remainder == 4) Temp = AESKeyExpansionSubstituteWord(Temp);
		
		AES_Key_Schedule[i] = AES_Key_Schedule[i - AES_CIPHER_KEY_WORDS_COUNT] ^ Temp;
	}
}

static void AESStepAddRoundKey(unsigned char First_Key_Schedule_Index, unsigned char Pointer_Input_State[][AES_STATE_COLUMNS_COUNT], unsigned char Pointer_Output_State[][AES_STATE_COLUMNS_COUNT])
{
	unsigned char *Pointer_Key_Schedule_Bytes, i;
	
	for (i = 0; i < AES_STATE_COLUMNS_COUNT; i++)
	{
		// Get byte access to the next key schedule
		Pointer_Key_Schedule_Bytes = (unsigned char *) &AES_Key_Schedule[First_Key_Schedule_Index];
		First_Key_Schedule_Index++;
		
		// Do the XOR operation on the current column (don't forget that 32-bit key schedule are stored in little endian order)
		Pointer_Output_State[0][i] = Pointer_Input_State[0][i] ^ Pointer_Key_Schedule_Bytes[3];
		Pointer_Output_State[1][i] = Pointer_Input_State[1][i] ^ Pointer_Key_Schedule_Bytes[2];
		Pointer_Output_State[2][i] = Pointer_Input_State[2][i] ^ Pointer_Key_Schedule_Bytes[1];
		Pointer_Output_State[3][i] = Pointer_Input_State[3][i] ^ Pointer_Key_Schedule_Bytes[0];
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void AES256CTRInitialize(unsigned char *Pointer_Key)
{
	// Compute key schedule
	AESKeyExpansion(Pointer_Key);
}

void AES256CTRUpdate(unsigned char *Pointer_Input_Buffer, unsigned char *Pointer_Output_Buffer)
{
	unsigned char (*Pointer_Buffer_1)[AES_STATE_COLUMNS_COUNT], (*Pointer_Buffer_2)[AES_STATE_COLUMNS_COUNT], Row, Column, i;
	
	Pointer_Buffer_1 = (unsigned char (*)[AES_STATE_COLUMNS_COUNT]) Pointer_Input_Buffer;
	Pointer_Buffer_2 = (unsigned char (*)[AES_STATE_COLUMNS_COUNT]) Pointer_Output_Buffer;
	
	// Reorganize input data in columns
	for (Column = 0; Column < AES_STATE_COLUMNS_COUNT; Column++)
	{
		for (Row = 0; Row < AES_STATE_ROWS_COUNT; Row++)
		{
			Pointer_Buffer_2[Row][Column] = *Pointer_Input_Buffer;
			Pointer_Input_Buffer++;
		}
	}
	
	// Initial key schedule
	AESStepAddRoundKey(0, Pointer_Buffer_2, Pointer_Buffer_1);
	
	// Execute all rounds but least one
	for (i = 1; i < AES_ROUNDS_COUNT; i++)
	{
		AESStepSubstituteBytes(Pointer_Buffer_1, Pointer_Buffer_2);
		AESStepShiftRows(Pointer_Buffer_2, Pointer_Buffer_1);
		AESStepMixColumns(Pointer_Buffer_1, Pointer_Buffer_2);
		AESStepAddRoundKey(i * AES_STATE_COLUMNS_COUNT, Pointer_Buffer_2, Pointer_Buffer_1);
	}
	
	// Special last round
	AESStepSubstituteBytes(Pointer_Buffer_1, Pointer_Buffer_2);
	AESStepShiftRows(Pointer_Buffer_2, Pointer_Buffer_1);
	AESStepAddRoundKey(AES_ROUNDS_COUNT * AES_STATE_COLUMNS_COUNT, Pointer_Buffer_1, Pointer_Buffer_2);
	
	// Reorganize output data in columns
	for (Column = 0; Column < AES_STATE_COLUMNS_COUNT; Column++)
	{
		for (Row = 0; Row < AES_STATE_ROWS_COUNT; Row++)
		{
			Pointer_Buffer_1[Row][Column] = *Pointer_Output_Buffer;
			Pointer_Output_Buffer++;
		}
	}
	memcpy(Pointer_Buffer_2, Pointer_Buffer_1, AES_BLOCK_SIZE);
}

void AES256CBCInitialize(unsigned char *Pointer_Key)
{
	unsigned char Row, Column;
	
	// Compute key schedule
	AESKeyExpansion(Pointer_Key);
	
	// Generate a random data block to use as the initial chaining seed (this way there is no need to provide an initialization vector)
	for (Row = 0; Row < AES_STATE_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < AES_STATE_COLUMNS_COUNT; Column++) AES_Internal_Block_Buffer[Row][Column] = SystemRandomGetNumber();
	}
}

void AES256CBCUpdate(unsigned char *Pointer_Buffer)
{
	unsigned char (*Pointer_Buffer_1)[AES_STATE_COLUMNS_COUNT], *Pointer_Internal_Block_Buffer, i, Row, Column;
	
	Pointer_Buffer_1 = (unsigned char (*)[AES_STATE_COLUMNS_COUNT]) Pointer_Buffer;
	Pointer_Internal_Block_Buffer = (unsigned char *) AES_Internal_Block_Buffer;
	
	// XOR input data with previous block data
	for (Row = 0; Row < AES_STATE_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < AES_STATE_COLUMNS_COUNT; Column++) Pointer_Buffer_1[Row][Column] ^= AES_Internal_Block_Buffer[Row][Column];
	}
	
	// Reorganize input data in columns
	for (Column = 0; Column < AES_STATE_COLUMNS_COUNT; Column++)
	{
		for (Row = 0; Row < AES_STATE_ROWS_COUNT; Row++)
		{
			AES_Internal_Block_Buffer[Row][Column] = *Pointer_Buffer;
			Pointer_Buffer++;
		}
	}
	
	// Initial key schedule
	AESStepAddRoundKey(0, AES_Internal_Block_Buffer, Pointer_Buffer_1);
	
	// Execute all rounds but least one
	for (i = 1; i < AES_ROUNDS_COUNT; i++)
	{
		AESStepSubstituteBytes(Pointer_Buffer_1, AES_Internal_Block_Buffer);
		AESStepShiftRows(AES_Internal_Block_Buffer, Pointer_Buffer_1);
		AESStepMixColumns(Pointer_Buffer_1, AES_Internal_Block_Buffer);
		AESStepAddRoundKey(i * AES_STATE_COLUMNS_COUNT, AES_Internal_Block_Buffer, Pointer_Buffer_1);
	}
	
	// Special last round
	AESStepSubstituteBytes(Pointer_Buffer_1, AES_Internal_Block_Buffer);
	AESStepShiftRows(AES_Internal_Block_Buffer, Pointer_Buffer_1);
	AESStepAddRoundKey(AES_ROUNDS_COUNT * AES_STATE_COLUMNS_COUNT, Pointer_Buffer_1, AES_Internal_Block_Buffer);
	
	// Reorganize output data in columns
	for (Column = 0; Column < AES_STATE_COLUMNS_COUNT; Column++)
	{
		for (Row = 0; Row < AES_STATE_ROWS_COUNT; Row++)
		{
			Pointer_Buffer_1[Row][Column] = *Pointer_Internal_Block_Buffer;
			Pointer_Internal_Block_Buffer++;
		}
	}
	
	// Keep encrypted block in the internal buffer to allow XORing it with the next block
	memcpy(AES_Internal_Block_Buffer, Pointer_Buffer_1, AES_BLOCK_SIZE);
}
