/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Benjamin Gilstorf / Bikku Byju Varughese
* Professors: Paulo Sousa
************************************************************


/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152   Compilers, Lab Section:012
* Assignment: A12.
* Date: Feb 04 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(atys_intg size, atys_intg increment, atys_intg mode) {
	BufferPointer readerPointer;
	if (size < 0 || size > READER_MAX_SIZE)
	{
		return NULL;
	}
	if (size == 0) {
		size = READER_DEFAULT_SIZE;
		increment = READER_DEFAULT_INCREMENT;
	}


	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return NULL;
	readerPointer->content = (atys_string)malloc(size);

	for (int i = 0; i < NCHAR; i++)
		readerPointer->histogram[i] = 0;
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;

	if (readerPointer->content)
		readerPointer->content[0] = READER_TERMINATOR;
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, atys_char ch) {
	atys_intg newSize = 0;

	if (readerPointer == NULL || ch < 0 || ch >= NCHAR) {
		return NULL; // Return NULL if the readerPointer is invalid or character is out of range
	}

	// Check if the buffer has space for the new character
	if ((readerPointer->position.wrte + 1) * sizeof(atys_char) > readerPointer->size) {
		// Buffer is full; need to reallocate
		switch (readerPointer->mode) {
		case MODE_FIXED:
			// Buffer is fixed size and cannot expand, return NULL
			return NULL;
		case MODE_ADDIT:
			// Additive mode; expand buffer by a fixed increment
			newSize = readerPointer->size + readerPointer->increment;
			break;
		case MODE_MULTI:
			// Multiplicative mode; expand buffer by multiplying by a fixed increment
			newSize = readerPointer->size * readerPointer->increment; // Adjust as necessary
			break;
		default:
			// Unknown mode, return NULL
			return NULL;
		}

		// Allocate new buffer with the calculated size
		atys_string newBuffer = (atys_string)realloc(readerPointer->content, newSize);
		if (newBuffer == NULL) {
			// Memory allocation failed, return NULL
			return NULL;
		}

		// Update buffer properties
		readerPointer->content = newBuffer;
		readerPointer->size = newSize;
		// Set relocation flag here if you have one
	}

	// Add the character to the buffer and increment the write position
	readerPointer->content[readerPointer->position.wrte++] = ch;
	// Update the histogram for the added character
	readerPointer->histogram[ch]++;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerClear(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		return ATYS_FALSE;
	}

	readerPointer->position.wrte = readerPointer->position.mark = readerPointer->position.read = 0;
	readerPointer->flags = READER_DEFAULT_FLAG;

	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}
	readerPointer->numReaderErrors = 0;

	return ATYS_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerFree(BufferPointer const readerPointer) {
	if (readerPointer == NULL) {
		return ATYS_FALSE;
	}
	if (readerPointer->size == READER_MAX_SIZE) {
		return ATYS_TRUE;
	}
	//De-allocate the reader content
	if (readerPointer->content != NULL) {
		free(readerPointer->content);

		readerPointer->content = NULL;
	}
	//De-allocate the reader pointer
	if (readerPointer != NULL) free(readerPointer);
	return ATYS_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerIsFull(BufferPointer const readerPointer) {
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return ATYS_TRUE; // Error condition
	}

	if (readerPointer->position.wrte >= readerPointer->size) {
		return ATYS_TRUE; // Buffer is full
	}


	return ATYS_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerIsEmpty(BufferPointer const readerPointer) {
	if (readerPointer == NULL) {
		return ATYS_FALSE;
	}

	if (readerPointer->position.wrte == 0 || (readerPointer->flags & FLAG_EMP)) {
		return ATYS_TRUE;
	}

	return ATYS_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerSetMark(BufferPointer const readerPointer, atys_intg mark) {

	if (readerPointer == NULL) {
		return ATYS_FALSE; // Cannot set mark on a NULL buffer
	}

	// Check if the mark is within the bounds of the buffer
	if (mark < 0 || mark > readerPointer->position.wrte) {
		return ATYS_FALSE; // Mark is out of bounds
	}

	readerPointer->position.mark = mark;
	return ATYS_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerPrint(BufferPointer const readerPointer) {
	atys_intg cont = 0;
	atys_char c;

	c = readerGetChar(readerPointer);

	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerLoad(BufferPointer const readerPointer, FILE* const fileDescriptor) {
	atys_intg size = 0;
	atys_char c;

	c = (atys_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerRecover(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		return ATYS_FALSE; // Operation failed due to invalid buffer pointer
	}
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	return ATYS_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerRetract(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		return ATYS_FALSE; // Cannot retract a NULL buffer
	}
	if (readerPointer->position.read > 0) {
		// Decrement the read position by one
		readerPointer->position.read--;

		return ATYS_FALSE; // Successfully retracted the read position
	}

	return ATYS_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_boln readerRestore(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		return ATYS_FALSE; // Cannot perform operation on a NULL buffer
	}

	// Check for boundary condition: Ensure mark position is within valid range
	// Assuming mark should not exceed the current write position
	if (readerPointer->position.mark < 0 || readerPointer->position.mark > readerPointer->position.wrte) {
		return ATYS_FALSE; // Mark position is out of bounds
	}

	readerPointer->position.read = readerPointer->position.mark;
	return ATYS_TRUE;
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_char readerGetChar(BufferPointer const readerPointer) {

	if (readerPointer == NULL || readerPointer->content == NULL) {
		// Cannot proceed if the buffer or its content does not exist
		return READER_TERMINATOR;
	}
	if (readerPointer->position.wrte > 0)
		return readerPointer->content[readerPointer->position.read++];
	return READER_TERMINATOR;
	if (readerPointer->position.read == readerPointer->position.wrte) {
		// Reset EOB flag if necessary
		readerPointer->flags &= ~FLAG_END; // Assuming FLAG_END indicates End Of Buffer

		// Return the character at the current read position and increment it for the next read
		return readerPointer->content[readerPointer->position.read++];
	}
	else {
		// Set EOB flag to indicate the end of the buffer has been reached
		readerPointer->flags |= FLAG_END; // Set the End Of Buffer flag

		// Return the terminator when read position is at or beyond the write position
		return READER_TERMINATOR;

	}
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_string readerGetContent(BufferPointer const readerPointer, atys_intg pos) {

	if (readerPointer == NULL || readerPointer->content == NULL) {
		// If the buffer pointer or its content does not exist, return NULL.
		return NULL;
	}

	// Check boundary conditions: Ensure pos is within the bounds of the buffer's content
	if (pos < 0 || pos >= readerPointer->position.wrte) {
		// If pos is out of bounds, return NULL to indicate an error or invalid request.
		return NULL;
	}
	return (readerPointer->content + pos);
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerGetPosRead(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {

		return READER_ERROR; // Ensure READER_ERROR is defined appropriately in your system
	}

	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerGetPosWrte(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		return READER_ERROR; // Ensure READER_ERROR is defined as an error indication.
	}
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerGetPosMark(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		return READER_ERROR; // Assuming READER_ERROR is defined as an error indication.
	}
	return readerPointer->position.mark;;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerGetSize(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {

		return READER_ERROR; // Ensure READER_ERROR or a similar mechanism is defined appropriately.
	}

	return readerPointer->size;

}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerGetInc(BufferPointer const readerPointer) {
	if (readerPointer == NULL) {
		// If the buffer pointer does not exist, return an error code or invalid value.
		// Assuming READER_ERROR is defined in your system to represent such cases.
		return READER_ERROR; // Make sure READER_ERROR is defined appropriately.
	}

	// Return the increment value of the buffer.
	return readerPointer->increment;


}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerGetMode(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		return READER_ERROR; // Assuming READER_ERROR is defined as an error indication.
	}
	return readerPointer->mode;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_byte readerGetFlags(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {

		return 0; // Placeholder for error or invalid state in flags, adjust as necessary.
	}
	return readerPointer->flags;
}



/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_void readerPrintStat(BufferPointer const readerPointer) {

	if (readerPointer == NULL) {
		printf("Error: Cannot print statistics for a NULL buffer.\n");
		return; // Exit the function if the readerPointer is NULL
	}

	// Check if the histogram has been initialized
	if (readerPointer->histogram == NULL) {
		printf("Error: Histogram has not been initialized.\n");
		return; // Exit the function if the histogram is NULL
	}

	// Print the histogram for each character
	printf("Character Statistics:\n");
	for (int i = 0; i < NCHAR; ++i) {
		// Only print the characters that have appeared at least once
		if (readerPointer->histogram[i] > 0) {
			// Print the character (as a letter or as a control character code) and its frequency
			if (isprint(i)) {
				printf("'%c' : %d\n", i, readerPointer->histogram[i]);
			}
			else {
				printf("'0x%X' : %d\n", i, readerPointer->histogram[i]);
			}
		}
	}
}
/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
atys_intg readerNumErrors(BufferPointer const readerPointer) {
	if (readerPointer == NULL) {
		// If readerPointer is NULL, return an error code or 0 if you do not have a defined error code
		return READER_ERROR; // Assuming READER_ERROR is defined as an error code
	}
	return 0;
}