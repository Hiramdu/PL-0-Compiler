
//#include "stdafx.h"
#include <stdio.h>
#include <set>
#include <string>
#include <iostream>
#include <iostream>
#include <vector>

#ifndef WIRTH_ZYC_
#define WIRTH_ZYC_
using namespace std;

const int norw = 16;        // no. of reserved words 保留字的个数
const int txmax = 100;      // length of identifier table 标识符表的长度（容量）
const int al = 10;          // length of identifiers 标识符的最大长度
const int nmax = 14;        // max. no. of digits in numbers 数字的最大长度
const int amax = 2047;      // maximum address 寻址空间
const int levmax = 3;       // maximum depth of block nesting 最大允许的块嵌套层数
const int cxmax = 200;      // size of code array 类PCODE目标代码数组长度（可容纳代码行数），即最多可以允许存储这么多汇编指令
const int lineLength = 82;  // 行缓冲区长度


/*单词种别，将其定义为枚举类型*/
typedef enum {
	NUL, IDENT, NUMBER, PLUS, MINUS, TIMES, SLASH, ODDSYM, EQL, NEQ, LSS, LEQ, GTR, GEQ, LPAREN,
	RPAREN, COMMA, SEMICOLON, PERIOD, BECOMES, BEGINSYM, ENDSYM, IFSYM, THENSYM, WHILESYM,
	WRITESYM, READSYM, DOSYM, CALLSYM, CONSTSYM, VARSYM, PROCSYM, ELSESYM, REPEATSYM, UNTILSYM
} symbol; // symobl类型标识了不同类型的词汇

typedef char alfa[al + 1];                                  // alfa类型用于标识符
typedef enum { CONSTANT, VARIABLE, PROCEDURE, ARRAY } obj0;    // 四种标识符的类型
typedef enum { LIT, OPR, LOD, STO, CAL, INT, JMP, JPC } fct;       // functions
typedef set<symbol> symset;

/*一条目标代码指令*/
struct instruction{
	fct f; // function code，枚举类型
	int l; // level,cann't big than levmax 
	int a; // displacement address,cann't big than amax
};         // 类PCODE指令类型，包含三个字段：指令f、层差l和另一个操作数a

/*******************************************
*	lit 0,a: load constant a               *
*	opr 0,a: execute operation a           *
*	lod l,a: load variable l,a             *
*	sto l,a: store variable l,a            *
*	cal l,a: call procedure a at level l   *
*	int 0,a: increment t-register by a     *
*	jmp 0,a: jump to a                     *
*	jpc 0,a: jump conditional to a         *
*******************************************/

/*标识符表的一个条目*/
typedef struct{
	alfa name;//标识符的名字
	obj0 kind;//标识符的类型，共有四种，分别为常量，变量，过程名与数组名
	union {
		struct{ int level, adr, size; }inOther;
		int val;
	}other;
} Table;

class PL0
{

protected:

	bool listswitch, sourceEnd;
	char ch;                         // last character read
	symbol  sym;                     // last symbol read
	alfa  id;                        // last identifier read
	int  num;                        // last number read

	int  cc;                         // character count
	int  ll;                         // line length
	int  kk, err;
	int  cx;                         // code allocation index
	int  codeNo;                     // code line no.

	static string errStr[];          // error string 
	char  line[lineLength];          // code line
	vector<string> errorString;      // error array
	alfa  a;                         // 词法分析器中用于临时存放正在分析的词
	instruction  code[cxmax + 1];      // destination code array

	alfa  word[norw + 1];              // 保留字表
	symbol  wsym[norw + 1];            // 保留字表中每一个保留字对应的symbol类型 
	symbol ssym[100];                // 一些符号对应的symbol类型表
	char  mnemonic[8][6];            // 类PCODE指令助记符表 

	symset  declbegsys, statbegsys, facbegsys; // 声明开始、表达式开始和项开始符号集合
	Table table[txmax + 1];                    // 符号表
	FILE*  fin, *fout;

public:

	PL0(char* source, char*destination);                     // 构造函数
	~PL0(){ fclose(fin), fclose(fout); }                       // 析构函数 	

	void error(int n);                                      // 出错处理，打印出错位置和出错代码	
	void getsym();                                          // 词法分析，读取一个单词 
	void getch();                                           // 漏掉空格，读取一个字符	
	void gen(fct x, int y, int z);		                    // 生成目标代码，并送入目标程序区
	void test(symset s1, symset s2, int n);                   // 测试当前单词符号是否合法 

	void block(int lev, int tx, symset fsys);                 // 分程序分析处理过程 
	void enter(obj0 k, int &tx, int &dx, int lev);             // 登入名字表
	int position(alfa id, int tx);                           // 查找标示符在名字表中的位置 
	void constdeclaration(int&tx, int&dx, int lev);           // 常量定义处理
	void vardeclaration(int&tx, int&dx, int lev);             // 变量说明处理

	void listcode(int cx0);                                 // 列出目标代码清单
	void statement(symset fsys, int tx, int lev);             // 语句部分处理 
	void expression(symset fsys, int tx, int lev);            // 表达式处理 
	void term(symset fsys, int tx, int lev);                  // 项处理
	void factor(symset fsys, int tx, int lev);                // 因子处理

	void condition(symset fsys, int tx, int lev);             // 条件处理	
	void arraydeclaration(int& tx, int& dx, int lev);         // 数组说明处理
	void interpret();                                       // 对目标代码的解释执行程序 
	int base(int l, int b, int s[]);                          // 通过静态链求出数据区的基地址  
	void SaveCode();                                        // 保存代码 

};
#endif
