
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

const int norw = 16;        // no. of reserved words �����ֵĸ���
const int txmax = 100;      // length of identifier table ��ʶ����ĳ��ȣ�������
const int al = 10;          // length of identifiers ��ʶ������󳤶�
const int nmax = 14;        // max. no. of digits in numbers ���ֵ���󳤶�
const int amax = 2047;      // maximum address Ѱַ�ռ�
const int levmax = 3;       // maximum depth of block nesting �������Ŀ�Ƕ�ײ���
const int cxmax = 200;      // size of code array ��PCODEĿ��������鳤�ȣ������ɴ�����������������������洢��ô����ָ��
const int lineLength = 82;  // �л���������


/*�����ֱ𣬽��䶨��Ϊö������*/
typedef enum {
	NUL, IDENT, NUMBER, PLUS, MINUS, TIMES, SLASH, ODDSYM, EQL, NEQ, LSS, LEQ, GTR, GEQ, LPAREN,
	RPAREN, COMMA, SEMICOLON, PERIOD, BECOMES, BEGINSYM, ENDSYM, IFSYM, THENSYM, WHILESYM,
	WRITESYM, READSYM, DOSYM, CALLSYM, CONSTSYM, VARSYM, PROCSYM, ELSESYM, REPEATSYM, UNTILSYM
} symbol; // symobl���ͱ�ʶ�˲�ͬ���͵Ĵʻ�

typedef char alfa[al + 1];                                  // alfa�������ڱ�ʶ��
typedef enum { CONSTANT, VARIABLE, PROCEDURE, ARRAY } obj0;    // ���ֱ�ʶ��������
typedef enum { LIT, OPR, LOD, STO, CAL, INT, JMP, JPC } fct;       // functions
typedef set<symbol> symset;

/*һ��Ŀ�����ָ��*/
struct instruction{
	fct f; // function code��ö������
	int l; // level,cann't big than levmax 
	int a; // displacement address,cann't big than amax
};         // ��PCODEָ�����ͣ����������ֶΣ�ָ��f�����l����һ��������a

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

/*��ʶ�����һ����Ŀ*/
typedef struct{
	alfa name;//��ʶ��������
	obj0 kind;//��ʶ�������ͣ��������֣��ֱ�Ϊ��������������������������
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
	alfa  a;                         // �ʷ���������������ʱ������ڷ����Ĵ�
	instruction  code[cxmax + 1];      // destination code array

	alfa  word[norw + 1];              // �����ֱ�
	symbol  wsym[norw + 1];            // �����ֱ���ÿһ�������ֶ�Ӧ��symbol���� 
	symbol ssym[100];                // һЩ���Ŷ�Ӧ��symbol���ͱ�
	char  mnemonic[8][6];            // ��PCODEָ�����Ƿ��� 

	symset  declbegsys, statbegsys, facbegsys; // ������ʼ�����ʽ��ʼ���ʼ���ż���
	Table table[txmax + 1];                    // ���ű�
	FILE*  fin, *fout;

public:

	PL0(char* source, char*destination);                     // ���캯��
	~PL0(){ fclose(fin), fclose(fout); }                       // �������� 	

	void error(int n);                                      // ��������ӡ����λ�úͳ������	
	void getsym();                                          // �ʷ���������ȡһ������ 
	void getch();                                           // ©���ո񣬶�ȡһ���ַ�	
	void gen(fct x, int y, int z);		                    // ����Ŀ����룬������Ŀ�������
	void test(symset s1, symset s2, int n);                   // ���Ե�ǰ���ʷ����Ƿ�Ϸ� 

	void block(int lev, int tx, symset fsys);                 // �ֳ������������� 
	void enter(obj0 k, int &tx, int &dx, int lev);             // �������ֱ�
	int position(alfa id, int tx);                           // ���ұ�ʾ�������ֱ��е�λ�� 
	void constdeclaration(int&tx, int&dx, int lev);           // �������崦��
	void vardeclaration(int&tx, int&dx, int lev);             // ����˵������

	void listcode(int cx0);                                 // �г�Ŀ������嵥
	void statement(symset fsys, int tx, int lev);             // ��䲿�ִ��� 
	void expression(symset fsys, int tx, int lev);            // ���ʽ���� 
	void term(symset fsys, int tx, int lev);                  // ���
	void factor(symset fsys, int tx, int lev);                // ���Ӵ���

	void condition(symset fsys, int tx, int lev);             // ��������	
	void arraydeclaration(int& tx, int& dx, int lev);         // ����˵������
	void interpret();                                       // ��Ŀ�����Ľ���ִ�г��� 
	int base(int l, int b, int s[]);                          // ͨ����̬������������Ļ���ַ  
	void SaveCode();                                        // ������� 

};
#endif
