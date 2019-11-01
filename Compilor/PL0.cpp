//#include "stdafx.h"
#include "pl0.h"

#pragma warning(disable : 4996)  
// ¥ÌŒÛ◊÷∑˚¥Æ ˝◊È
string PL0::errStr[] = { " ", "error 0001:  常数说明中“=”写成“：=”",
    "error 0002:  常数说明中的“=”后应为数字",
    "error 0003:  常数说明中的标识符后应是“=”",
    "error 0004:  const,var,procedure后应为标识符",
    "error 0005:  漏掉了‘,’或‘;’",
    "error 0006:  过程说明后的符号不正确（应是语句开始符或过程开始符）",
    "error 0007:  应是语句开始符",
    "error 0008:  过程体内语句部分的后跟符不正确",
    "error 0009:  程序皆为丢了句号‘.’",
    "error 0010:  语句之间漏了‘;’",
    "error 0011:  标识符没说明",
    "error 0012:  赋值语句中，赋值号左部标识符属性应是变量",
    "error 0013:  赋值语句左部标识符应是赋值号:=",
    "error 0014:  call后应为标识符",
    "error 0015:  call后标识符属性应为过程",
    "error 0016:  条件语句中丢了then",
    "error 0017:  丢了end或;",
    "error 0018:  while型循环语句中丢了do",
    "error 0019:  语句后的标识符不正确",
    "error 0020:  应为关系运算符",
    "error 0021:  表达式内标识符属性不能是过程",
    "error 0022:  表达式中漏掉了右括号‘)’",
    "error 0023:  因子后的非法符号",
    "error 0024:  表达式开始符不能是此符号",
    "error 0025:  文件在不该结束的地方结束了",
    "error 0026:  结束符出现在不该结束的地方",
    "error 0027:  ", "error 0028:  ", "error 0029:  ", "error 0030:  ",
    "error 0031:  数越界",
    "error 0032:  read语句括号中标识符不是变量",
    "error 0033:  else附近错误",
    "error 0034:  repeat附近错误" };



// PL0ππ‘Ï∫Ø ˝
PL0::PL0(const char* source, const char*destination)
{
	listswitch = true, sourceEnd = false;
	/*±£¡Ù◊÷µƒ≥ı ºªØ£¨π≤”–16∏ˆ±£¡Ù◊÷£¨◊¢“‚ ◊◊÷ƒ∏µƒÀ≥–Ú*/
	strcpy(word[1], "begin");                 
	strcpy(word[2], "call");
	strcpy(word[3], "const");
	strcpy(word[4], "do");
	strcpy(word[5], "else");
	strcpy(word[6], "end");
	strcpy(word[7], "if");
	strcpy(word[8], "odd");
	strcpy(word[9], "procedure");
	strcpy(word[10], "read");
	strcpy(word[11], "repeat");
	strcpy(word[12], "then");
	strcpy(word[13], "until");
	strcpy(word[14], "var");
	strcpy(word[15], "while");
	strcpy(word[16], "write");

	/*√ø∏ˆπÿº¸◊÷∂‘”¶µƒµ•¥ ÷÷±£¨“ª¥ “ª÷÷*/
	wsym[1] = BEGINSYM;   wsym[2] = CALLSYM;       // ≥ı ºªØ±£¡Ù◊÷±Ì÷–√ø“ª∏ˆ±£¡Ù◊÷∂‘”¶µƒsymbol¿‡–Õ 
	wsym[3] = CONSTSYM;   wsym[4] = DOSYM;
	wsym[5] = ELSESYM;    wsym[6] = ENDSYM;
	wsym[7] = IFSYM;      wsym[8] = ODDSYM;
	wsym[9] = PROCSYM;    wsym[10] = READSYM;
	wsym[11] = REPEATSYM; wsym[12] = THENSYM;
	wsym[13] = UNTILSYM;  wsym[14] = VARSYM;
	wsym[15] = WHILESYM;  wsym[16] = WRITESYM;


	memset(code, 0, sizeof(code));//¥Ê¥¢ƒø±Í¥˙¬Î£¨◊Ó¥ÛŒ™200Ãı
	memset(ssym, 0, 100 * sizeof(symbol));//µ•¥ ÷÷±±Ì£¨◊Ó¥ÛŒ™100
	memset(table, 0, sizeof(table));//±Í ∂∑˚±Ì£¨◊Ó≥§Œ™100Ãı£¨“≤æÕ «Àµ≥Ã–Ú÷–≤ª‘ –Ì∂®“Â≥¨π˝100∏ˆ±Í ∂∑˚
	memset(line, 0, sizeof(line));//––ª∫≥Â«¯£°

	/*≤ø∑÷‘ÀÀ„∑˚∫≈”ÎΩÁ∑˚∂‘”¶µƒµ•¥ ÷÷±£¨’‚—˘◊ˆ «Œ™¡Àºı…Ÿµ•¥ πÈ¿‡µƒ≈–∂œ£¨’‚–©œ‡πÿ◊÷∑˚÷±Ω”Õ®π˝“‘œ¬ ˝◊È∏≥÷µ∏¯sym*/
	ssym['+'] = PLUS;                            // ≥ı ºªØ“ª–©∑˚∫≈∂‘”¶µƒsymbol¿‡–Õ±Ì
	ssym['-'] = MINUS;
	ssym['*'] = TIMES;
	ssym['/'] = SLASH;
	ssym['('] = LPAREN;
	ssym[')'] = RPAREN;
	ssym['='] = EQL;
	ssym[','] = COMMA;
	ssym['.'] = PERIOD;
	ssym['#'] = NEQ;
	ssym['<'] = LSS;
	ssym['>'] = GTR;
	ssym[';'] = SEMICOLON;

	/*π≤”–8∏ˆ÷˙º«∑˚£¨“≤æÕ «8∏ˆƒø±Í¥˙¬Îµƒπ¶ƒ‹÷∏¡Ó*/
	strcpy(mnemonic[LIT], " lit ");             // ≥ı ºªØ¿‡PCODE÷∏¡Ó÷˙º«∑˚±Ì  
	strcpy(mnemonic[OPR], " opr ");
	strcpy(mnemonic[LOD], " lod ");
	strcpy(mnemonic[STO], " sto ");
	strcpy(mnemonic[CAL], " cal ");
	strcpy(mnemonic[INT], " int ");
	strcpy(mnemonic[JMP], " jmp ");
	strcpy(mnemonic[JPC], " jpc ");

	/*œ¬√Ê∂ºŒ™set<symbol>¿‡–Õ£¨ «Ω´≤ªÕ¨”√Õæµƒ±Í ∂∑˚Ω¯––∑÷¿‡*/
	declbegsys.insert(CONSTSYM), declbegsys.insert(VARSYM), declbegsys.insert(PROCSYM); // ≥ı ºªØ…˘√˜ø™ º∑˚∫≈ºØ∫œ£¨∑÷±Œ™≥£¡ø£¨±‰¡ø”Îπ˝≥Ã√˚  
	statbegsys.insert(BEGINSYM), statbegsys.insert(CALLSYM), statbegsys.insert(IFSYM), statbegsys.insert(WHILESYM); // ≥ı ºªØ±Ì¥Ô Ωø™ º∑˚∫≈ºØ∫œ
	facbegsys.insert(IDENT), facbegsys.insert(NUMBER), facbegsys.insert(LPAREN); // ≥ı ºªØœÓø™ º∑˚∫≈ºØ∫œ

	err = 0;
	cc = 0;                          // ––ª∫≥Â«¯÷∏’Î
	cx = 0;                          // ¥˙¬Î∑÷≈‰÷∏’Î£¨¥˙¬Î…˙≥…ƒ£øÈ◊‹‘⁄cxÀ˘÷∏Œª÷√…˙≥…–¬µƒ¥˙¬Î                       
	ll = 0;                          // ––ª∫≥Â«¯≥§∂»
	ch = ' ';                        // last character read   
	kk = al;                         // “˝»Î¥À±‰¡ø «≥ˆ”⁄≥Ã–Ú–‘ƒ‹øº¬«,al «±Í ∂∑˚µƒ◊Ó¥Û≥§∂»
	codeNo = 0;                       // code line no.π≤∂¡¡À∂‡…Ÿ––ª∫≥Â«¯
	fin = fopen(source, "r");
	fout = fopen(destination, "w");
}



// ≥ˆ¥Ì¥¶¿Ì£¨¥Ú”°≥ˆ¥ÌŒª÷√∫Õ≥ˆ¥Ì¥˙¬Î
void PL0::error(int n)
{
	char s[10];
	sprintf(s, "µ⁄ %d ––:", codeNo);
	errorString.push_back(s + errStr[n]);
	err = err + 1;//error count
}//error end

// ¥ ∑®∑÷Œˆ£¨∂¡»°“ª∏ˆµ•¥  
void PL0::getsym()
{
	if (sourceEnd)
		return;
	int i, j, k;
	while (ch == ' ' || ch == 9)
		getch();    // cls space and tab£¨∂¡»°◊÷∑˚µΩch÷–
	if (isalpha(ch)) // id or reserved word
	{
		k = 0;//index of word
		memset(a, 0, al + 1);//a¡Ÿ ±¥Ê∑≈±ª∑÷Œˆµƒ¥ 
		//∂¡»°“ª∏ˆÕÍ’˚µƒµ•¥ ,≥§∂»◊Ó≥§Œ™10
		do{
			if (k < al)// ºÏ≤‚“ª∏ˆµ•¥ ≥§∂»
			{
				a[k] = ch;
				k = k + 1;
			}
			getch();
			if (sourceEnd)
				return;
		} while (isalpha(ch) || isdigit(ch));//k◊Ó¥ÛŒ™10£¨◊÷∑˚¥Æ◊Ó≥§Œ™a[0]~a[9]
		if (k >= kk)//kk = 10,kk÷∏œÚ◊÷∑˚¥ÆµƒΩ·Œ≤¥¶£¨’‚∏ˆµÿ∑Ω”¶∏√∑≈'\0'
			kk = k;
		else
		{
			do{
				a[kk] = ' ';
				kk = kk - 1;
			} while (kk > k);
		}//kk“™∫Õkœ‡µ»
		strcpy(id, a);//idŒ™◊Ó∫Û“ª∏ˆ±Í ∂∑˚£¨’‚—˘id÷–¥Ê∑≈¥À ±∂¡»°µƒµ•¥ 
		i = 1;
		j = norw;
		// ≈–∂œ «∑Ò «πÿº¸◊÷£®∂˛∑÷À—À˜£©
		do{
			k = (i + j) / 2;
			if (strcmp(id, word[k]) <= 0)
				j = k - 1;
			if (strcmp(id, word[k]) >= 0)
				i = k + 1;
		} while (i <= j);
		if (i - 1 > j)
			sym = wsym[k];
		else
			sym = IDENT;
	}
	else if (isdigit(ch))  // number
	{
		k = 0;
		num = 0;
		sym = NUMBER;
		do{
			num = 10 * num + ch - '0';
			k = k + 1;
			getch();
		} while (isdigit(ch));
		if (k > nmax)
			error(30);
	}
	else if (ch == ':')
	{
		getch();
		if (ch == '=')//“™÷™µ¿œ¬“ª∏ˆ◊÷∑˚ «À≠
		{
			sym = BECOMES;//∏≥÷µ≤Ÿ◊˜
			getch();
		}
		else
			sym = NUL;
	}
	else if (ch == '<')   // extra stuff added to support <=
	{
		getch();
		if (ch == '=')
		{
			sym = LEQ;
			getch();
		}
		else
			sym = LSS;
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			sym = GEQ;
			getch();
		}
		else
			sym = GTR;
	}
	else                // end of extra stuff
	{
		sym = ssym[ch];  // ∆‰À¸∑˚∫≈µƒ∏≥÷µ
		getch();

	}
	int ttt=sym;
	char *XXX[100] = { "NUL", "IDENT", "NUMBER", "PLUS", "MINUS", "TIMES", "SLASH", "ODDSYM", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "LPAREN",
		"RPAREN", "COMMA", "SEMICOLON", "PERIOD", "BECOMES", "BEGINSYM", "ENDSYM", "IFSYM", "THENSYM", "WHILESYM",
		"WRITESYM", "READSYM", "DOSYM", "CALLSYM", "CONSTSYM", "VARSYM", "PROCSYM", "ELSESYM", "REPEATSYM", "UNTILSYM" };
	cout << XXX[ttt]; 
	cout << '\n';

	



}

// ¥”ª∫≥Â«¯∂¡»°“ª∏ˆ◊÷∑˚£¨≤¢∑≈‘⁄ch÷–	
void PL0::getch()
{
	if (cc == ll)//cc «––ª∫≥Â«¯÷∏’Î£¨ll «––ª∫≥Â«¯≥§∂»
	{
		if (feof(fin))//ºÏ—ÈŒƒº˛ «∑ÒΩ· ¯
		{
			if (sym != PERIOD)
				error(25);//Œƒº˛‘⁄≤ª∏√Ω· ¯µƒµÿ∑ΩΩ· ¯¡À
			sourceEnd = true;
			return;
		}
		cc = 0;
		fgets(line, lineLength, fin);
		codeNo++;
		ll = strlen(line);//ª∫≥Â«¯µƒ≥§∂»
		if (line[ll - 1] == 10) ll--;
	}
	ch = line[cc];
	cc = cc + 1;
	cout<< ch;
	cout << '\n';
}


// …˙≥…ƒø±Í¥˙¬Î£¨≤¢ÀÕ»Îƒø±Í≥Ã–Ú«¯
void PL0::gen(fct x, int y, int z)
{
	if (cx > cxmax)//÷∏œÚ¥Ê∑≈ƒø±Í¥˙¬Îµƒ÷∏’Î
	{
		cout << "Program too long\n";
		return;
	}
	code[cx].f = x;
	code[cx].l = y;//≤„
	code[cx].a = z;
	cx = cx + 1;
}//gen end

// ≤‚ ‘µ±«∞µ•¥ ∑˚∫≈ «∑Ò∫œ∑® 
void PL0::test(symset s1, symset s2, int n)
{
	if (sourceEnd)        return;
	if (s1.find(sym) == s1.end())
	{
		error(n);//error 6 π˝≥ÃÀµ√˜∫Ûµƒ∑˚∫≈≤ª’˝»∑£®”¶ «”Ôæ‰ø™ º∑˚ªÚπ˝≥Ãø™ º∑˚£©
		symset::iterator it;
		for (it = s2.begin(); it != s2.end(); it++)
			s1.insert(*it);//s1=s1+s2
		while (s1.find(sym) == s1.end())
			getsym();
	}
}//test end


// ∑÷≥Ã–Ú∑÷Œˆ¥¶¿Ìπ˝≥Ã £¨ÕÍ≥…À˘”–”Ô∑®∑÷Œˆ∫Õ¥˙¬Î…˙≥…µƒπ§◊˜
void PL0::block(int lev, int tx, symset fsys)//‘⁄÷˜≥Ã–Ú÷–levŒ™0£¨txŒ™table±ÌµƒÀ˜“˝
{
	if (sourceEnd)
		return;
	int dx;  // data allocation index
	int tx0; // initial table index
	int cx0; // initial code index
	dx = 3;   // ±‰¡øµƒ∏ˆ ˝£ø√ø∏ˆπ˝≥Ãµƒœ‡∂‘∆ ºŒª÷√‘⁄block÷–µƒ≥ı÷µ
	tx0 = tx; // ±Ì÷∏’Î
	table[tx].other.inOther.adr = cx;//cxæÕ «ƒø±Í¥˙¬Îµƒ÷∏’Î
	gen(JMP, 0, 0);//π¶ƒ‹£¨µÿ÷∑£¨≤„ ˝
	if (lev>levmax)
		error(32);
	do{
		if (sym == CONSTSYM)        // ±£¡Ù◊÷£¨¥¶¿Ì≥£¡ø…˘√˜
		{
			getsym();//‘Ÿ¥ŒªÒ»°µ•¥ 
			do{
				constdeclaration(tx, dx, lev);//≥£¡ø∂®“Â∑÷Œˆπ˝≥Ã£®table±ÌµƒÀ˜“˝£¨µÿ÷∑∆´“∆£¨≤„¥Œ£©
				while (sym == COMMA)//¡¨–¯…˘√˜
				{
					getsym();
					constdeclaration(tx, dx, lev);
				}
				if (sym == SEMICOLON)//;
					getsym();
				else
					error(5);
			} while (sym == IDENT);//Œ™ ≤√¥“™”√while—≠ª∑£¨Œ™¡À¥¶¿Ì“ª–©¥ÌŒÛ«Èøˆ£¨±»»Á…Ÿº”¡À£¨ªÚ’ﬂ£ªµ´ «≤ª÷¡”⁄»√’˚∏ˆ±‡“Î≥Ã–Ú÷’÷π
		}
		if (sym == VARSYM)            // ¥¶¿Ì±‰¡ø…˘√˜
		{
			getsym();
			do{
				vardeclaration(tx, dx, lev);//¥¶¿Ì±‰¡ø…˘√˜£¨µ«»Îtable
				while (sym == COMMA){
					getsym();
					vardeclaration(tx, dx, lev);
				}
				if (sym == SEMICOLON)
					getsym();
				else
					error(5);
			} while (sym == IDENT);
		}
		while (sym == PROCSYM)         // ¥¶¿Ìπ˝≥Ãµƒ…˘√˜
		{
			getsym();
			if (sym == IDENT)
			{
				enter(PROCEDURE, tx, dx, lev);
				getsym();
			}
			else
				error(4);//const,var,procedure∫Û”¶Œ™±Í ∂∑˚",
			if (sym == SEMICOLON)
				getsym();
			else
				error(5);//"error 0005:  ¬©µÙ¡À°Æ,°ØªÚ°Æ;°Ø"
			symset tmp = fsys;//±‰¡ø…˘√˜ªÚ’ﬂ±Ì¥Ô Ω…˘√˜µƒ∑˚∫≈
			tmp.insert(SEMICOLON);
			block(lev + 1, tx, tmp);//Ω¯»Î◊”π˝≥Ã
			if (sym == SEMICOLON){//’‚ «‘ı√¥ªÿ ¬£ø£ø£ø¥”◊”≥Ã–Ú∑µªÿµƒ«Èøˆ£ø
				getsym();
				symset tmp = statbegsys;
				for (int i = IDENT; i <= PROCSYM; i++)
					tmp.insert((symbol)i);
				test(tmp, fsys, 6);
			}
			else
				error(5);// ¬©µÙ¡À°Æ,°ØªÚ°Æ;°Ø",
		}
		symset tmp = statbegsys;
		tmp.insert(IDENT);
		test(tmp, declbegsys, 7);//”¶∏√Œ™”Ôæ‰ø™ º∏¥
	} while (declbegsys.find(sym) != declbegsys.end());//÷ª“™symªπ «…˘√˜”Ôæ‰£¨ƒ«√¥æÕºÃ–¯’‚∏ˆπ˝≥Ã

	code[table[tx0].other.inOther.adr].a = cx;
	table[tx0].other.inOther.adr = cx; // start adr of code£¨∂‘”⁄π˝≥Ã√˚µƒADR”Ú£¨ «‘⁄π˝≥ÃÃÂµƒƒø±Í¥˙¬Î…˙≥…∫Û∑µÃÓπ˝≥ÃÃÂµƒ»Îø⁄µÿ÷∑°£
	table[tx0].other.inOther.size = dx;

	cx0 = cx;
	gen(INT, 0, dx);//Œ™÷˜≥Ã–Ú‘⁄‘À––’ª÷–ø™±Ÿ ˝æ›«¯°£
	symset tmp = statbegsys;
	for (int i = SEMICOLON; i <= ENDSYM; i++)
		tmp.insert((symbol)i);
	statement(tmp, tx, lev);
	gen(OPR, 0, 0); // return
	symset s2;
	test(fsys, s2, 8);//π˝≥ÃÃÂƒ⁄”Ôæ‰≤ø∑÷µƒ∫Û∏˙∑˚≤ª’˝»∑
	listcode(cx0);//Ω´±æ¥Œblock…˙≥…µƒƒø±Í¥˙¬Î¥Ú”°≥ˆ¿¥
}// block end

// µ«»Î√˚◊÷±Ì£¨÷˜“™ «Àƒ÷÷¿‡–Õ
void PL0::enter(obj0 k, int &tx, int &dx, int lev)
{
	tx = tx + 1;
	strcpy(table[tx].name, id);
	table[tx].kind = k;//¿‡–Õ
	switch (k)//∂‘”⁄≤ªÕ¨µƒ√˚◊÷µƒ¿‡–ÕΩ¯––≤ªÕ¨µƒ¥¶¿Ì
	{
	case CONSTANT:
		if (num>amax)//∂¡≥ˆµƒ ˝÷µ
		{
			error(31);
			num = 0;
		}
		table[tx].other.val = num;
		break;
	case VARIABLE:
		table[tx].other.inOther.level = lev;
		table[tx].other.inOther.adr = dx;
		dx++;
		break;
	case PROCEDURE:
		table[tx].other.inOther.level = lev;
		break;
	case ARRAY:
		table[tx].other.inOther.size = lev;
		break;
	}
}//enter end


// ≤È’“±Í æ∑˚‘⁄√˚◊÷±Ì÷–µƒŒª÷√ 
int PL0::position(alfa id, int tx)//find identifier id in table
{
	int i;
	strcpy(table[0].name, id);
	i = tx;
	while (strcmp(table[i].name, id) != 0)i--;
	return i;
}//position end

// ≥£¡ø∂®“Â¥¶¿Ì£®table±ÌµƒÀ˜“˝£¨µÿ÷∑∆´“∆£¨≤„¥Œ£©
void PL0::constdeclaration(int&tx, int&dx, int lev)
{
	if (sym == IDENT)//≥£¡ø∫Û∂®“Âµƒ±Í ∂∑˚
	{
		getsym();
		if (sym >= EQL&&sym <= BECOMES)
		{
			if (sym == BECOMES)//≥£ ˝Àµ√˜÷–°∞ = °±–¥≥…£∫ =,‘⁄pl/0÷–£¨≥£¡øµƒ∏≥÷µ π”√=
				error(1);
			getsym();
			if (sym == NUMBER)
			{
				enter(CONSTANT, tx, dx, lev);//Ω´∏√≥£¡ø≤Â»Îtable±Ì÷–
				getsym();
			}
			else
				error(2);
		}
		else
			error(3);
	}
	else
		error(4);
}// constdeclaration end

// ±‰¡øÀµ√˜¥¶¿Ì
void PL0::vardeclaration(int&tx, int&dx, int lev)
{
	if (sym == IDENT)
	{
		enter(VARIABLE, tx, dx, lev);
		getsym();
	}
	else
		error(4);
}//vardeclaration end

//  ˝◊ÈÀµ√˜¥¶¿Ì
void PL0::arraydeclaration(int&tx, int&dx, int lev)
{
	int upscript = 0, downscript = 0;
	getsym();
	if (sym == NUMBER || sym == CONSTSYM)
	{
		if (num == 0)
		{
			upscript = num;
			getsym();
		}
		else
			error(32);
	}
	if (sym == COMMA)
		getsym();
	else
		error(32);
	if (sym == NUMBER || sym == CONSTSYM)
	{
		downscript = num;
		getsym();
	}
	if (sym != RPAREN)
		error(32);
	else
	{
		enter(ARRAY, tx, dx, downscript + 1);
		getsym();
	}
}

// ¡–≥ˆƒø±Í¥˙¬Î«Âµ•
void PL0::listcode(int cx0)//list code generated for this block
{
	int i;
	if (listswitch)
	for (i = cx0; i<cx; i++)
		cout << " " << i << "  " << mnemonic[code[i].f]//÷˙º«∑˚
		<< "  " << code[i].l << "  " << code[i].a << endl;
}// listcode end

// ”Ôæ‰≤ø∑÷¥¶¿Ì 
void PL0::statement(symset fsys, int tx, int lev)
{
	if (sourceEnd)
		return;
	int i, cx1, cx2;
	if (sym == IDENT)//∂‘∏≥÷µ”Ôæ‰µƒ¥¶¿Ì
	{
		i = position(id, tx);
		if (i == 0)
			error(11);
		else if (table[i].kind != VARIABLE)
		{
			error(12);
			i = 0;
		}
		getsym();
		if (sym == BECOMES)
			getsym();
		else
			error(13);
		expression(fsys, tx, lev);
		if (sym != SEMICOLON)
			error(10);
		if (i != 0)
			gen(STO, lev - table[i].other.inOther.level, table[i].other.inOther.adr);
	}
	else if (sym == READSYM)//∂¡≤Ÿ◊˜µƒ¥¶¿Ì
	{
		getsym();
		if (sym != LPAREN)
			error(34);
		else
		do{
			getsym();
			if (sym == IDENT)
				i = position(id, tx);
			else
				i = 0;
			if (i == 0)
				error(35);
			else
			{
				gen(OPR, 0, 16);//opr≤Ÿ◊˜ «…∂,∂¡»°“ª∏ˆ±‰¡ø÷µ£¨≤¢Ω´∆‰∑≈‘⁄’ª∂•
				gen(STO, lev - table[i].other.inOther.level, table[i].other.inOther.adr);//Ω´’ª∂•µƒ ˝æ›ÀÕµΩµ•‘™±‰¡ø÷–
			}
			getsym();
		} while (sym == COMMA);//readø…≤Ÿ◊˜∂‡∏ˆ ˝
		if (sym != RPAREN)//”“¿®∫≈
		{
			error(33);
			while (fsys.find(sym) != fsys.end())  getsym();
		}
		else
			getsym();
	}
	else if (sym == WRITESYM)
	{
		getsym();
		if (sym == LPAREN)
		{
			do{
				getsym();
				symset tmp = fsys;
				for (int t = RPAREN; t <= COMMA; t++)
					tmp.insert((symbol)t);
				expression(tmp, tx, lev);
				gen(OPR, 0, 14);
			} while (sym == COMMA);
			if (sym != RPAREN)
				error(33);
			else
				getsym();
		}
		gen(OPR, 0, 15);
	}
	else if (sym == CALLSYM)//µ˜”√◊”≥Ã–Ú
	{
		getsym();
		if (sym != IDENT)
			error(14);
		else
		{
			i = position(id, tx);
			if (i == 0)
				error(11);
			else if (table[i].kind = PROCEDURE)
				gen(CAL, lev - table[i].other.inOther.level, table[i].other.inOther.adr);
			else
				error(15);
			getsym();
		}
	}
	else if (sym == IFSYM)
	{
		getsym();
		symset tmp = fsys;
		for (int i = THENSYM; i <= DOSYM; i++)
			tmp.insert((symbol)i);
		condition(tmp, tx, lev);
		if (sym == THENSYM)
			getsym();
		else
			error(16);
		cx1 = cx;
		gen(JPC, 0, 0);
		tmp.insert(ELSESYM);
		statement(tmp, tx, lev);
		getsym();
		code[cx1].a = cx;

		if (sym == ELSESYM)
		{
			getsym();
			cx2 = cx;
			gen(JMP, 0, 0);
			code[cx1].a = cx;
			statement(fsys, tx, lev);
			code[cx2].a = cx;
		}
	}
	else if (sym == BEGINSYM)
	{
		getsym();
		symset tmp = fsys;
		for (int i = SEMICOLON; i <= ENDSYM; i++)
			tmp.insert((symbol)i);
		statement(tmp, tx, lev);
		tmp = statbegsys;
		tmp.insert(SEMICOLON);
		while (tmp.find(sym) != tmp.end())
		{
			if (sourceEnd)return;
			if (sym == SEMICOLON || sym == ENDSYM)
				getsym();
			else if (sym = PERIOD)
			{
				error(26);
				getsym();
			}
			else
				error(10);
			tmp = fsys;
			for (i = SEMICOLON; i <= ENDSYM; i++)
				tmp.insert((symbol)i);
			if (sourceEnd)return;
			if (sym == ENDSYM)
				break;
			statement(tmp, tx, lev);
		}
		if (sym == ENDSYM)
			getsym();
		else if (!sourceEnd)
			error(17);
	}
	else if (sym == WHILESYM)
	{
		cx1 = cx;                 // º«œ¬µ±«∞¥˙¬Î∑÷≈‰Œª÷√£¨’‚ «while—≠ª∑µƒø™ ºŒª÷√
		getsym();
		symset tmp = fsys;
		tmp.insert(DOSYM);
		condition(tmp, tx, lev);
		cx2 = cx;                // º«œ¬µ±«∞¥˙¬Î∑÷≈‰Œª÷√£¨’‚ «whileµƒdo÷–µƒ”Ôæ‰µƒø™ ºŒª÷√
		gen(JPC, 0, 0);
		if (sym == DOSYM)
			getsym();
		else
			error(18);
		statement(fsys, tx, lev);
		gen(JMP, 0, cx1);
		code[cx2].a = cx;
	}
	else if (sym == REPEATSYM)
	{
		symset temp1, temp2;
		temp1 = fsys, temp1.insert(SEMICOLON), temp1.insert(UNTILSYM);
		cx1 = cx;
		getsym();
		statement(temp1, tx, lev);
		temp2 = statbegsys;
		temp2.insert(SEMICOLON);
		while (temp2.find(sym) != temp2.end())
		{
			if (sym == SEMICOLON)
				getsym();
			else
				error(34);
			statement(temp1, tx, lev);
		}
		if (sym == UNTILSYM)
		{
			getsym();
			condition(fsys, tx, lev);
			gen(JPC, 0, cx1);
		}
		else
			error(34);
	}
	symset setT;
	test(fsys, setT, 19);
}//statement end


// ±Ì¥Ô Ω¥¶¿Ì 
void PL0::expression(symset fsys, int tx, int lev)
{
	symbol addop;
	symset tmp = fsys;
	for (int t = PLUS; t <= MINUS; t++)
		tmp.insert((symbol)t);
	if (sym >= PLUS&&sym <= MINUS)
	{
		addop = sym;
		getsym();
		term(tmp, tx, lev);
		if (addop == MINUS)
			gen(OPR, 0, 1);
	}
	else
		term(tmp, tx, lev);
	while (sym >= PLUS&&sym <= MINUS)
	{
		addop = sym;
		getsym();
		term(tmp, tx, lev);
		if (addop == PLUS)
			gen(OPR, 0, 2);
		else
			gen(OPR, 0, 3);
	}
}// expression end

// œÓ¥¶¿Ì
void PL0::term(symset fsys, int tx, int lev)
{
	if (sourceEnd)
		return;
	symbol  mulop;
	symset tmp = fsys;
	for (int t = TIMES; t <= SLASH; t++)
		tmp.insert((symbol)t);
	factor(tmp, tx, lev);
	while (sym >= TIMES && sym <= SLASH)
	{
		mulop = sym;
		getsym();
		factor(tmp, tx, lev);
		if (mulop == TIMES)
			gen(OPR, 0, 4);
		else
			gen(OPR, 0, 5);
	}
}// term end

// “Ú◊”¥¶¿Ì
void PL0::factor(symset fsys, int tx, int lev)
{
	int i;
	test(facbegsys, fsys, 24);
	while (facbegsys.find(sym) != facbegsys.end())
	{
		if (sym == IDENT)
		{
			i = position(id, tx);
			if (i == 0)
				error(11);
			else
				switch (table[i].kind)
			{
				case CONSTANT:
					gen(LIT, 0, table[i].other.val);
					break;
				case VARIABLE:
					gen(LOD, lev - table[i].other.inOther.level, table[i].other.inOther.adr);
					break;
				case PROCEDURE:
					error(21);
					break;
			}
			getsym();
		}
		else  if (sym == NUMBER)
		{
			if (num>amax)
			{
				error(31);
				num = 0;
			}
			gen(LIT, 0, num);
			getsym();
		}
		else if (sym == LPAREN)
		{
			getsym();
			symset tmp = fsys;
			tmp.insert(RPAREN);
			expression(tmp, tx, lev);
			if (sym == RPAREN)
				getsym();
			else
				error(22);
		}
		test(fsys, facbegsys, 23);
	}
}//factor end

// Ãıº˛¥¶¿Ì	
void PL0::condition(symset fsys, int tx, int lev)
{
	symbol relop;
	symset tmp = fsys;
	tmp.insert(EQL), tmp.insert(NEQ), tmp.insert(LSS), tmp.insert(LEQ), tmp.insert(GTR), tmp.insert(GEQ);
	if (sym == ODDSYM)
	{
		getsym();
		expression(fsys, tx, lev);
		gen(OPR, 0, 6);
	}
	else
	{
		expression(tmp, tx, lev);
		if (tmp.find(sym) == tmp.end())
			error(20);
		else
		{
			relop = sym;
			getsym();
			expression(fsys, tx, lev);
			switch (relop)
			{
			case EQL: gen(OPR, 0, 8);
				break;
			case NEQ: gen(OPR, 0, 9);
				break;
			case LSS: gen(OPR, 0, 10);
				break;
			case GEQ: gen(OPR, 0, 11);
				break;
			case GTR: gen(OPR, 0, 12);
				break;
			case LEQ: gen(OPR, 0, 13);
				break;
			}
		}
	}
}//condition end

// ∂‘ƒø±Í¥˙¬ÎµƒΩ‚ Õ÷¥––≥Ã–Ú 
void PL0::interpret()
{
	int err1 = errorString.size();
	if (err1>0)
	{
		cout << "¥Ê‘⁄%d∏ˆ¥ÌŒÛ:" << err1 << endl;
		for (int i = 0; i<err1; i++)
			cout << errorString[i] << endl;
		//return;
	}
	const int stacksize = 500;
	int  p = 0, b = 1, t = 0;//program-,base-,topstack-registers
	instruction  i;// instruction register
	int  s[stacksize + 1] = { 0 };//  datastore
	cout << " Start PL/0\n";
	do{
		i = code[p];
		p = p + 1;
		switch (i.f)
		{
		case LIT:
			t = t + 1;
			s[t] = i.a;
			break;
		case OPR:
			switch (i.a) //operator
			{
			case 0:// return
				t = b - 1;
				p = s[t + 3];
				b = s[t + 2];
				break;
			case 1:
				s[t] = -s[t];
				break;
			case 2:
				t = t - 1;
				s[t] = s[t] + s[t + 1];
				break;
			case 3:
				t = t - 1;
				s[t] = s[t] - s[t + 1];
				break;
			case 4:
				t = t - 1;
				s[t] = s[t] * s[t + 1];
				break;
			case 5:
				t = t - 1;
				s[t] = s[t] / s[t + 1];
				break;
			case 6:
				if (s[t] % 2)
					s[t] = 1;
				else
					s[t] = 0;
				break;
			case 8:
				t = t - 1;
				if (s[t] == s[t + 1])
					s[t] = 1;
				else
					s[t] = 0;
				break;
			case 9:
				t = t - 1;
				if (s[t] == s[t + 1])
					s[t] = 0;
				else
					s[t] = 1;
				break;
			case 10:
				t = t - 1;
				if (s[t]<s[t + 1])
					s[t] = 1;
				else
					s[t] = 0;
				break;
			case 11:
				t = t - 1;
				if (s[t] >= s[t + 1])
					s[t] = 1;
				else
					s[t] = 0;
				break;
			case 12:
				t = t - 1;
				if (s[t]>s[t + 1])
					s[t] = 1;
				else
					s[t] = 0;
				break;
			case 13:
				t = t - 1;
				if (s[t] <= s[t + 1])
					s[t] = 1;
				else
					s[t] = 0;
				break;
			case 14:
				cout << "    " << s[t];
				t = t - 1;
				break;
			case 15:
				cout << endl;
				break;
			case 16:
				t = t + 1;
				cout << "?";
				s[t] = 0;
				cin >> s[t];
				break;
			};
			break;
		case LOD:
			t = t + 1;
			s[t] = s[base(i.l, b, s) + i.a];
			break;
		case STO:
			s[base(i.l, b, s) + i.a] = s[t];
			t = t - 1;
			break;
		case CAL:// generate new block mark
			s[t + 1] = base(i.l, b, s);
			s[t + 2] = b;
			s[t + 3] = p;
			b = t + 1;
			p = i.a;
			break;
		case INT:
			t = t + i.a;
			break;
		case JMP:
			p = i.a;
			break;
		case JPC:
			if (s[t] == 0)
				p = i.a;
			t = t - 1;
			break;
		}//switch end 
	} while (p != 0);
	cout << " End PL/0\n";
} // interpret end

// Õ®π˝æ≤Ã¨¡¥«Û≥ˆ ˝æ›«¯µƒª˘µÿ÷∑  
int PL0::base(int l, int b, int s[])
{
	int b1;
	b1 = b;//find base l levels down
	while (l>0){
		b1 = s[b1];
		l = l - 1;
	}
	return  b1;
}


// ±£¥Ê¥˙¬Î 
void PL0::SaveCode()
{
	if (fout)
	for (int i = 0; i<cx; i++)
		fprintf(fout, "%d  %s %d %d\n ", i, mnemonic[code[i].f], code[i].l, code[i].a);
}
