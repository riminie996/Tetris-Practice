#include "Tetris.h"

int Tetris::CalcSendGarbageLines(int clear_line, int ren, bool btb, E_TSPIN_PATTERN tspin, bool perfect)
{
	int garbage_lines = 0;

	//
	// 
	//	Clear Type									Rows Cleared	Garbage Rows Sent
	//	Single										1				0
	//	Double										2				1
	//	Triple										3				2
	//	Tetris										4				4
	//	Mini T-Spin Single(wherever available)		1				0
	//	Mini T-Spin Double(wherever available)		2				1
	//	T-Spin Single								1				2
	//	T-Spin Double								2				4
	//	T-Spin Triple								3				6
	//	Back-to-Back Bonus							0				+1 (MTSS, MTSD; TSS), +2 (TSD; Tetris); +3 (TST)
	//	Perfect clear								1 - 4			10 (+last line clear type if applicable)
	//

	//
	//	0REN +0
	//	1REN +0 
	//	2REN +1 
	//	3REN +1 
	//	4REN +2 
	//	5REN +2 
	//	6REN +3 
	//	7REN +3 
	//	8REN +4 
	//	9REN +4 
	//	10REN +4
	//	11REN +5
	//	12REN +5
	//	13REN +5
	//	14REN +5

	//TƒXƒsƒ“
	if (tspin == E_TSPIN_PATTERN::TSpin)
	{
		garbage_lines += clear_line * 2;
	}
	else if (clear_line >= 4)
	{
		garbage_lines += 4;
	}
	else
	{
		garbage_lines += clear_line - 1;
	}
	//REN
	if (ren >= 11)
		garbage_lines += 5;
	else if (ren >= 8)
		garbage_lines += 4;
	else if (ren >= 6)
		garbage_lines += 3;
	else if (ren >= 4)
		garbage_lines += 2;
	else if (ren >= 2)
		garbage_lines += 1;
	else
		;


	if (btb)
	{
		garbage_lines += 1;
	}
	if (perfect)
	{
		garbage_lines += 10;
	}

	return garbage_lines;
}