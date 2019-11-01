// Compilor.cpp : ∂®“Âøÿ÷∆Ã®”¶”√≥Ã–Úµƒ»Îø⁄µ„°£
//
#pragma warning(disable : 4996)  
//#include "stdafx.h"
#include "pl0.h"
#define _CRT_SECURE_NO_DEPRECATE
int main()
{
	PL0 cp("/Users/apple/Downloads/Compilor/testPas2.txt", "/Users/apple/Downloads/Compilor/nasm.txt");
	symset fsys;
	fsys.insert(PERIOD);
	fsys.insert(CONSTSYM), fsys.insert(VARSYM), fsys.insert(PROCSYM);//≥ı ºªØ…˘√˜
	fsys.insert(BEGINSYM), fsys.insert(CALLSYM), fsys.insert(IFSYM), fsys.insert(WHILESYM);//≥ı ºªØ±Ì¥Ô Ω
	cp.getsym();             // ¥ ∑®∑÷Œˆ£¨∑÷Œˆ“ª∏ˆ¥ £¨µ√µΩµƒ¥ µƒ¿‡–Õ∑≈‘⁄sym÷–£¨»ÙŒ™±Í ∂∑˚£¨‘ÚΩ´µ•¥ ∑≈‘⁄id÷–£¨»ÙŒ™ ˝÷µ£¨‘ÚΩ´ ˝÷µ∑≈‘⁄num÷–
	cp.block(0, 0, fsys);      // ∑÷≥Ã–Ú∑÷Œˆ¥¶¿Ìπ¶ƒ‹
	cp.SaveCode();           // ±£¥Ê¥˙¬Î
	cp.interpret();          // ∂‘ƒø±Í¥˙¬ÎµƒΩ‚ Õ÷¥––≥Ã–Ú 
	return 0;
}

