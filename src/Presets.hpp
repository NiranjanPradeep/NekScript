

#pragma once

struct StackFrameController;

// Deallocators
void StringDeleter		(void *&Data);

// Functions
void Print				(StackFrameController *pSfc);

// Implicit Functions
void _GetString			(StackFrameController *pSfc);
void _GetNumber			(StackFrameController *pSfc);

// Typecasters
void NumberToString		(StackFrameController *pSFc);