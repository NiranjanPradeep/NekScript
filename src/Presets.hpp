

#pragma once

struct StackFrameController;

// Deallocators
void StringDeleter		(void *&Data);
void NumberDeleter		(void *&Data);

// Library Functions
void Print				(StackFrameController *pSfc);
void Shreya				(StackFrameController * pSfc);
void AddNumber			(StackFrameController *pSfc);

// Implicit Functions
void _GetString			(StackFrameController *pSfc);
void _GetNumber			(StackFrameController *pSfc);

// Typecasters
void NumberToString		(StackFrameController *pSFc);
void StringToNumber		(StackFrameController *pSFc);

