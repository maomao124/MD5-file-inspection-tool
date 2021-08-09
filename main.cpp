#include<iostream>
#include<stdio.h>
#include<string.h>
#include <Windows.h>
#include<time.h>
#include <graphics.h>
#include <conio.h>
#include<algorithm>
#include<stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <ddraw.h>
#include<iomanip>
#include <Psapi.h>
#include<vector>
#include<io.h>
#include <direct.h>
#include <sapi.h>                                             // 语音头文件
#include <sphelper.h>                                     // 语音识别头文件
#include <thread>                                           //多线程
                                                                        //vs2019自定义控制台代码模板 by mao
                                                                        //作者QQ1296193245
using namespace std;
#pragma warning(disable : 4996)
#pragma comment (lib,"ddraw.lib")
#pragma comment(lib,"sapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma intrinsic(__rdtsc)
unsigned __int64 t1, t2, t3, t4;
double run_time;                                                   //微秒级算法时间计时器
LARGE_INTEGER time_start;	                                //开始时间
LARGE_INTEGER time_over;	                                //结束时间
double dqFreq;		                                               //计时器频率
LARGE_INTEGER f;	                                               //计时器频率
double tt;
long double totaltime;

inline void runstart()
{
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	t1 = __rdtsc();
	QueryPerformanceCounter(&time_start);	       //计时开始
}
inline void runend()
{
	QueryPerformanceCounter(&time_over);	        //计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	t2 = __rdtsc();
	t3 = t2 - t1;
}
void ShowProcessMemoryUsageInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
	float memoryUsage_K = pmc.WorkingSetSize / 1024.0;
	std::cout << std::fixed << setprecision(0) << "程序内存使用率：" << memoryUsage_K << " KB = " << setprecision(2) << memoryUsage_M << " MB" << std::endl;
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
{
	__int64 a = static_cast<long long>(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	__int64 b = static_cast<long long>(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return   (b - a);
}
void rundisplay()
{
	cout << endl;
	cout << "-------------------------------------------------------" << endl;
	if (t3 < 10000)
	{
		cout << "CPU脉冲数：" << (t3) << endl;
	}
	else if (t3 < 100000000 && t3 >= 10000)
	{
		tt = t3;
		cout << "CPU脉冲数：" << (tt) / 10000 << "万" << endl;
	}
	else if (t3 > 100000000000)
	{
		t4 = t3;
		t4 = t3 / 10000000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10 << "亿" << endl;
	}
	else
	{
		t4 = t3;
		t4 = t3 / 10000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10000 << "亿" << endl;
	}
	if (run_time > 10000)
	{
		cout << "算法运行时间：" << run_time / 1000 << "秒" << endl;
	}
	else if (run_time < 1)
	{
		cout << "算法运行时间：" << run_time * 1000 << "微秒" << endl;
	}
	else
	{
		cout << "算法运行时间：" << run_time << "毫秒" << endl;
	}
	totaltime = clock();
	cout << "程序运行时间：" << totaltime / 1000 << "秒" << endl;
	if (run_time > 0.4)
	{
		double frequency = t3 / run_time / 1000;
		cout << "CPU频率：" << frequency << "MHZ" << endl;
	}
	ShowProcessMemoryUsageInfo();
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)tchData, &dwSize);
		if (lRet == ERROR_SUCCESS)
		{
			cout << "CPU 名字：";
			wcout << tchData << endl;
		}
		else
		{
			cout << "CPU 名字：";
			wcout << L"未知" << endl;
		}
	}
	else
	{
		cout << "CPU 名字：";
		wcout << L"未知" << endl;
	}
	RegCloseKey(hKey);
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	double totalphy = statex.ullTotalPhys / 1024 / 1024;
	totalphy = totalphy / 1024;
	cout << "总物理内存：    " << setprecision(4) << left << setw(7) << totalphy << " GB" << endl;
	double totalvir = statex.ullTotalPageFile / 1024 / 1024;
	totalvir = totalvir / 1024;
	cout << "总虚拟内存：    " << setw(7) << totalvir - totalphy << " GB" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	//   光标位置   
	COORD     cursorPos;
	//   标准输出句柄   
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	double freephy = statex.ullAvailPhys / 1024 / 1024;
	freephy = freephy / 1024;
	double usephy = (totalphy - freephy) * 100.0 / totalphy;
	cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
	double freetotal = statex.ullAvailPageFile / 1024 / 1024;
	freetotal = freetotal / 1024;
	cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
	double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
	cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
	double usevir = freevir * 100 / (totalvir - totalphy);
	cout << "虚拟内存使用率：";
	printf("%.2f%%", usevir);
	cout << endl;
	cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << 0.00 << "%" << endl;
	cout << "CPU空闲率：    " << setw(6) << 100.0<< "%" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "按任意键退出程序" << endl;
	while (!_kbhit())
	{
		GlobalMemoryStatusEx(&statex);
		WaitForSingleObject(hEvent, 1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);
		float cpu = (kernel + user - idle) * 100.0 / (kernel + user);
		float cpuidle = (idle) * 100.0 / (kernel + user);
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 0;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "内存使用率：" << statex.dwMemoryLoad << "%" << endl;
		double freephy = statex.ullAvailPhys / 1024 / 1024;
		freephy = freephy / 1024;
		double usephy = (totalphy - freephy) * 100.0 / totalphy;
		cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 1;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freetotal = statex.ullAvailPageFile / 1024 / 1024;
		freetotal = freetotal / 1024;
		cout << "总空闲内存：    " << setprecision(3) << freetotal <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 2;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
		cout << "虚拟内存使用量：" << setprecision(3) << freevir <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 3;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double usevir = freevir * 100 / (totalvir - totalphy);
		cout << "虚拟内存使用率：";
		printf("%.2f%%", usevir);
		cout << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 4;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << cpu << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 5;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU空闲率：    " << setw(6) << cpuidle << "%"<<endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 6;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "-------------------------------------------------------" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 7;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "按任意键退出程序" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	cout << endl;
	
}

/*   ****************************************
void FpsTime(const time_t* t0, time_t* t1, const time_t* t2)// 图形界面计算显示FPS和运行时间
{
	if (!t0 || !t1 || !t2)
		return;
	static int FPS = 0;
	static int fps = 0;
	static wstring fpsTime;
	if (*t2 - *t1 == 1)
	{
		FPS = ++fps;
	}
	fpsTime = L"FPS: ";
	fpsTime += to_wstring(FPS);
	fpsTime += L" 运行时间: ";
	fpsTime += to_wstring(*t2 - *t0);
	fpsTime += L"s";
	settextcolor(LIGHTMAGENTA);
	outtextxy(0, 0, fpsTime.c_str());

	if (*t2 - *t1 == 1)
	{
		fps = 0;
		*t1 = time(nullptr);
	}
	else
	{
		fps++;
	}
}
*/    //*************************************
/*bool cmp(const int a, const int b)
{
	return a > b;
}*/
/*    ***************************************
bool ChangeDisSize(int cx, int cy)	                                  // 分辨率设置为 cx × cy，退出程序时自动还原
{
	LPDIRECTDRAW DXW;
	HRESULT ddrval;
	ddrval = DirectDrawCreate(NULL, &DXW, NULL);
	if (DD_OK != DD_OK) return FALSE;	                         // 创建DirectDraw对象失败返回
	ddrval = DXW->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	if (DD_OK != DD_OK) return FALSE;	                         // 取得窗口模式失败返回
	ddrval = DXW->SetDisplayMode(cx, cy, 16);
	if (DD_OK != DD_OK) return FALSE;	                          // 设置显示模式失败返回
}
*/
//**************************************************************//************************非主函数

#define MAX_PATH 500


	/* Type define */
typedef unsigned char byte;
typedef unsigned long ulong;

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD5
{
public:
	MD5();
	MD5(const void* input, size_t length);
	MD5(const string& str);
	MD5(ifstream& in);
	void update(const void* input, size_t length);
	void update(const string& str);
	void update(ifstream& in);
	const byte* digest();
	string toString();
	void reset();
private:
	void update(const byte* input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const ulong* input, byte* output, size_t length);
	void decode(const byte* input, ulong* output, size_t length);
	string bytesToHexString(const byte* input, size_t length);

	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator=(const MD5&);
private:
	ulong _state[4];	/* state (ABCD) */
	ulong _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte _buffer[64];	/* input buffer */
	byte _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	static const size_t BUFFER_SIZE = 1024;
};


/* Constants for MD5Transform routine. */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}


const byte MD5::PADDING[64] = { 0x80 };
const char MD5::HEX[16] =
{
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f'
};

/* Default construct. */
MD5::MD5() 
{
	reset();
}

/* Construct a MD5 object with a input buffer. */
MD5::MD5(const void* input, size_t length) 
{
	reset();
	update(input, length);
}

/* Construct a MD5 object with a string. */
MD5::MD5(const string& str)
{
	reset();
	update(str);
}

/* Construct a MD5 object with a file. */
MD5::MD5(ifstream& in)
{
	reset();
	update(in);
}

/* Return the message-digest */
const byte* MD5::digest()
{
	if (!_finished) 
	{
		_finished = true;
		final();
	}
	return _digest;
}

/* Reset the calculate state */
void MD5::reset() 
{

	_finished = false;
	/* reset number of bits. */
	_count[0] = _count[1] = 0;
	/* Load magic initialization constants. */
	_state[0] = 0x67452301;
	_state[1] = 0xefcdab89;
	_state[2] = 0x98badcfe;
	_state[3] = 0x10325476;
}

/* Updating the context with a input buffer. */
void MD5::update(const void* input, size_t length) 
{
	update((const byte*)input, length);
}

/* Updating the context with a string. */
void MD5::update(const string& str)
{
	update((const byte*)str.c_str(), str.length());
}

/* Updating the context with a file. */
void MD5::update(ifstream& in)
{

	if (!in)
		return;

	std::streamsize length;
	char buffer[BUFFER_SIZE];
	while (!in.eof())
	{
		in.read(buffer, BUFFER_SIZE);
		length = in.gcount();
		if (length > 0)
			update(buffer, length);
	}
	in.close();
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void MD5::update(const byte* input, size_t length) 
{

	ulong i, index, partLen;

	_finished = false;

	/* Compute number of bytes mod 64 */
	index = (ulong)((_count[0] >> 3) & 0x3f);

	/* update number of bits */
	if ((_count[0] += ((ulong)length << 3)) < ((ulong)length << 3))
		_count[1]++;
	_count[1] += ((ulong)length >> 29);

	partLen = 64 - index;

	/* transform as many times as possible. */
	if (length >= partLen)
	{

		memcpy(&_buffer[index], input, partLen);
		transform(_buffer);

		for (i = partLen; i + 63 < length; i += 64)
			transform(&input[i]);
		index = 0;

	}
	else
	{
		i = 0;
	}

	/* Buffer remaining input */
	memcpy(&_buffer[index], &input[i], length - i);
}

/* MD5 finalization. Ends an MD5 message-_digest operation, writing the
the message _digest and zeroizing the context.
*/
void MD5::final()
{

	byte bits[8];
	ulong oldState[4];
	ulong oldCount[2];
	ulong index, padLen;

	/* Save current state and count. */
	memcpy(oldState, _state, 16);
	memcpy(oldCount, _count, 8);

	/* Save number of bits */
	encode(_count, bits, 8);

	/* Pad out to 56 mod 64. */
	index = (ulong)((_count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	update(PADDING, padLen);

	/* Append length (before padding) */
	update(bits, 8);

	/* Store state in digest */
	encode(_state, _digest, 16);

	/* Restore current state and count. */
	memcpy(_state, oldState, 16);
	memcpy(_count, oldCount, 8);
}

/* MD5 basic transformation. Transforms _state based on block. */
void MD5::transform(const byte block[64]) 
{

	ulong a = _state[0], b = _state[1], c = _state[2], d = _state[3], x[16];

	decode(block, x, 64);

	/* Round 1 */
	FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
	FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
	FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
	GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
	HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
	II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

	_state[0] += a;
	_state[1] += b;
	_state[2] += c;
	_state[3] += d;
}

/* Encodes input (ulong) into output (byte). Assumes length is
a multiple of 4.
*/
void MD5::encode(const ulong* input, byte* output, size_t length)
{

	for (size_t i = 0, j = 0; j < length; i++, j += 4) 
	{
		output[j] = (byte)(input[i] & 0xff);
		output[j + 1] = (byte)((input[i] >> 8) & 0xff);
		output[j + 2] = (byte)((input[i] >> 16) & 0xff);
		output[j + 3] = (byte)((input[i] >> 24) & 0xff);
	}
}

/* Decodes input (byte) into output (ulong). Assumes length is
a multiple of 4.
*/
void MD5::decode(const byte* input, ulong* output, size_t length) 
{

	for (size_t i = 0, j = 0; j < length; i++, j += 4) 
	{
		output[i] = ((ulong)input[j]) | (((ulong)input[j + 1]) << 8) |
			(((ulong)input[j + 2]) << 16) | (((ulong)input[j + 3]) << 24);
	}
}

/* Convert byte array to hex string. */
string MD5::bytesToHexString(const byte* input, size_t length) 
{
	string str;
	str.reserve(length << 1);
	for (size_t i = 0; i < length; i++)
	{
		int t = input[i];
		int a = t / 16;
		int b = t % 16;
		str.append(1, HEX[a]);
		str.append(1, HEX[b]);
	}
	return str;
}

/* Convert digest to string value */
string MD5::toString()
{
	return bytesToHexString(digest(), 16);
}

void PrintMD5(const string& str, MD5& md5)
{
	cout << "MD5(\"" << str << "\") = " << md5.toString() << endl;
}

string FileDigest(const string& file)
{

	ifstream in(file.c_str(), ios::binary);
	if (!in)
		return "";

	MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) 
	{
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0)
			md5.update(buffer, length);
	}
	in.close();
	return md5.toString();
}


void getFiles(string path, string path2, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p, p2;
	if ((hFile = _findfirst(p.assign(path).append(path2).append("\\*").c_str(), &fileinfo)) != -1)// _findfirst(p.assign(path).append(path2).append("\\*").c_str(), &fileinfo)) != -1
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getFiles(p.assign(path).append("\\"), p2.assign(fileinfo.name).append("\\"), files);
				}
			}
			else
			{
				files.push_back(p.assign(path2).append(fileinfo.name));  //这一行可以给出相对路径
				//files.push_back(p.assign(fileinfo.name) );  //这一行可以给出文件名 
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


void getFilesAll(string path, vector<string>& files)
{
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					getFilesAll(p.assign(path).append("\\\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

string solve(string& s1, string& s2)
{
	if (s2.size() == 0)
		return s1;
	for (int i = 0;i < (int)(s1.size() - s2.size() + 1);)
	{
		if (s1.substr(i, s2.size()) == s2)
		{
			s1.erase(i, s2.size());
		}
		else
			i++;
	}
	return s1;
}

void speak()
{
	ISpVoice* pVoice = NULL;                                            //创建一个ISpVoice的空指针
	if (FAILED(::CoInitialize(NULL)))                                     //初始化COM编程环境
	{
		cout << "初始化COM环境失败！" << endl;
		return ;
	}
	//获取ISpVoice接口对象：
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (SUCCEEDED(hr))                                                      //如果获取接口对象成功
	{
		char* szStr = (char*)malloc(200);
		sprintf(szStr, "感谢您使用本程序，祝您身体健康，万事如意");
		WCHAR wszClassName[200];
		memset(wszClassName, 0, sizeof(wszClassName));
		MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
			sizeof(wszClassName) / sizeof(wszClassName[0]));
		hr = pVoice->Speak(wszClassName, 0, NULL);         //朗读

		pVoice->Release();                                                 //释放分配资源
		pVoice = NULL;                                                     //防止野指针
	}
	::CoUninitialize();                                                       //退出COM编程环境
}

//**************************************************************//************************

int main()
{
	/*   *************************************
	constexpr int MIN = 1;                            //随机数
	constexpr int MAX = 100;
	constexpr int numm = 10;
	 random_device rd;
	default_random_engine eng(rd());
	uniform_int_distribution<int> distr(MIN, MAX);

	for (int n = 0; n < numm; ++n)
	{
		cout << distr(eng) << ",";
	}
	*/   //***********************************
	/*  //************************************
	SYSTEMTIME sys;                                    //获取系统时间
	GetLocalTime(&sys);
	cout << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "      ";
	cout<< setfill('0') << setw(2) << sys.wHour << ":" << setfill('0') << setw(2) << sys.wMinute << ":" << setfill('0') << setw(2) << sys.wSecond << endl;
	*/   //***********************************
	/*   *************************************                                             //全屏图形窗口模板
	int rw, rh;
	rw = GetSystemMetrics(SM_CXSCREEN);                                          // 屏幕宽度 像素
	rh = GetSystemMetrics(SM_CYSCREEN);                                          // 屏幕高度 像素
	HWND hwnd = initgraph(rw, rh);                                                    // 初始化绘图窗口并获取窗口句柄
	setbkcolor(GREEN);
	cleardevice();
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);               // 获取窗口信息
																										 // 设置窗口信息 最大化 取消标题栏及边框
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);           // 修改窗口样式
	SetWindowPos(hwnd, HWND_TOP, 0, 0, rw, rh, 0);
	*/   //***********************************
	/*      ***********************************
	initgraph(1600, 900);
	setbkcolor(WHITE);
	cleardevice();
	*/  //***********************************
	/*    ***********************************                                             //后台方式运行，Unicode字符集改成多字节字符集
	HWND hwnd; if (hwnd = ::FindWindow("ConsoleWindowClass", NULL)) //找到控制台句柄
	{
		::ShowWindow(hwnd, SW_HIDE); //隐藏控制台窗口
	}
                                                                                                    //unicode字符集下：
	const char* sss = "ConsoleWindowClass";
	WCHAR wszName[200];
	memset(wszName, 0, sizeof(wszName));
	MultiByteToWideChar(CP_ACP, 0, sss, strlen(sss) + 1, wszName,
		sizeof(wszName) / sizeof(wszName[0]));
	HWND hwnd; if (hwnd = ::FindWindow(wszName, NULL)) //找到控制台句柄
	{
		::ShowWindow(hwnd, SW_HIDE); //隐藏控制台窗口
	}
	*/  //**********************************

	//*************************************************************************

	
	cout << "                               欢迎使用文件检验工具" << endl;
	loop:
	cout << "***************************************菜单*************************************" << endl;
	cout << "1.检验多文件的MD5值" << endl;
	cout << "2.计算相对路径下所有文件的MD5值，并把数据写入MD5.txt文件" << endl;
	cout << "3.计算当个文件的MD5值（文件路径从键盘读入）" << endl;
	cout << "4.退出" << endl;
	cout << "********************************************************************************" << endl;
	cout << "请选择：";
	char c;
	while (1)
	{
		c = _getch();
		if (c == '1' || c == '2' || c == '3' || c == '4')
		{
			cout << c;
			break;
		}
	}
	cout << endl;
	if (c == '2')
	{
		HWND wnd = GetHWnd();
		cout << "\a";
		int ret = MessageBox(NULL, TEXT("如果相对路径下存在MD5,txt文件，继续执行将会覆盖掉原来的数据！\n\t\t是否继续执行？"), TEXT("文件覆盖警告！！！"), MB_YESNO | MB_ICONQUESTION);
		if (ret != IDYES)
			goto loop;
	}

	//*************************************************************************
	runstart();
	//*************************计时开始***************************************
	
	if (c == '1')
	{
		int error1 = 0;               //文件里MD5值为null的数量
		string errorpath1[500];
		int error2 = 0;                //找不到文件路径，或者文件被占用，无法计算MD5值
		string errorpath2[500];
		int error3 = 0;                //MD5值不一致，文件损坏
		string errorpath3[5000];
		ifstream in1("MD5.txt", ios::in);
		if (!in1)
		{
			cout << "没有找到MD5.txt文件！！！" << endl;
			system("pause");
			goto loop;
		}
		int size = 0;
		in1 >> size;
		if (size==0||size<0||size>50000)
		{
			cout << "文件数据异常" << endl;
			system("pause");
			goto loop;
		}
		string* filemd5 = new string [size];
		string* path = new string[size];
		for (int i = 0; i < size; i++)
		{
			in1 >> path[i];
		}
		for (int i = 0; i < size; i++)
		{
			in1 >> filemd5[i];
			if (filemd5[i] == "NULL")
			{
				errorpath1[error1] = path[i];
				error1++;
			}
		}
		in1.close();
		string* md5 = new string[size];
		for (int i = 0; i < size; i++)
		{
			cout << "路径：" << path[i] << endl;
			cout << "文件中的MD5值：" << filemd5[i] << endl;
			md5[i] = FileDigest(path[i]);
			cout << "计算出的MD5值：" << md5[i] << endl;
			if (md5[i] != filemd5[i] && md5[i] != "")
			{
				cout << "错误3" << endl << endl;
				errorpath3[error3] = path[i];
				error3++;
			}
			else if (md5[i] == "")
			{
				cout << "错误3" << endl << endl;
				errorpath2[error2] = path[i];
				error2++;
			}
			else
				cout << "正确" << endl << endl;
		}
		if (error1 != 0 || error2 != 0 || error3 != 0)
		{ 
			cout << "**********************************错误结果**********************************" << endl;
			cout << "\a";
			cout << "                       错误1：文件里MD5值为null，原因：生成MD5.txt文件时文件被占用" << endl;
			cout << "                                                         错误数量：" << error1 << endl;
			cout << "错误文件的路径：" << endl;
			for (int i = 0;i < error1;i++)
			{
				cout << errorpath1[i] << endl;
			}
			cout << endl;
			cout << "                       错误2：找不到文件路径，或者文件被占用，故而无法计算MD5值" << endl;
			cout << "                                                         错误数量：" << error2 << endl;
			cout << "错误文件的路径：" << endl;
			for (int i = 0;i < error2;i++)
			{
				cout << errorpath2[i] << endl;
			}
			cout << endl;
			cout << "                       错误3：MD5值不一致，文件损坏，或者文件已被修改" << endl;
			cout << "                                                         错误数量：" << error3 << endl;
			cout << "错误文件的路径：" << endl;
			for (int i = 0;i < error3;i++)
			{
				cout << errorpath3[i] << endl;
			}
			cout << endl;
		}
		cout << "**********************************************************************" << endl;
		cout << "                            最终结果：" << endl;
		cout << "                            全部文件：" << size << "个" << endl;
		cout << "                            文件损坏：" << error3 << "个" << endl;
		cout << "                            其它错误：" << error1 + error2 << "个" << endl;
		cout <<endl<< "任务完成" << endl;
		delete[]md5;
		delete[]filemd5;
		delete[]path;
		//system("pause");
	}


	else if (c == '2')
	{
		cout << "加载中，请等待......." << endl;
		vector<string> files;
		char   buffer[MAX_PATH];
		getcwd(buffer, MAX_PATH);
		string filePath;
		filePath.assign(buffer).append("\\");
		////获取该路径下的所有文件  
		getFilesAll(filePath, files);
		int size = files.size();
		int s = 0;
		filePath.append("\\\\");

		for (int i = 0;i < size;i++)
		{
			solve(files[i], filePath);
			if (files[i] == "MD5值检验工具.exe")
			{
				s++;

			}
			else if (files[i] == "._cache_MD5值检验工具.exe")
			{
				s++;

			}
			else
			{
				cout << files[i].c_str() << endl;
				//out << files[i].c_str() << endl;
			}
		}
		cout << "正在把所有路径写入文件MD5.txt中......." << endl;
		cout << "在此期间请不要关闭程序，以免文件损坏" << endl;
		ofstream out("MD5.txt", ios::out);
		out << size - s << endl;
		for (int i = 0;i < size;i++)
		{
			solve(files[i], filePath);
			if (files[i] == "MD5值检验工具.exe")
			{
				//s++;

			}
			else if (files[i] == "._cache_MD5值检验工具.exe")
			{
				//s++;

			}
			else
			{
				//cout << files[i].c_str() << endl;
				out << files[i].c_str() << endl;
			}
		}
		out.close();
		cout << "写入完成" << endl;
		cout << "一共" << size - s << "个文件" << endl;
		size = size - s;
		string* paths = new string[size];
		string* fileMD5 = new string[size];
		int filesize = 0;
		cout << "正在读入文件，读取期间请不要关闭程序，以免文件损坏" << endl;
		ifstream in("MD5.txt", ios::in);
		in >> filesize;
		if (filesize != size)
		{
			cout << "文件总数异常，将已文件的数据为基准" << endl;
			cout << "文件中的数据：" << filesize << "，" << "程序里的数据：" << size << endl;
			system("pause");
		}
		for (int i = 0;i < filesize;i++)
		{
			in >> paths[i];
		}
		cout << "读入完成" << endl;
		in.close();
		cout << "正在计算MD5值......." << endl;
		int error = 0;
		for (int i = 0;i < filesize;i++)
		{
			fileMD5[i] = FileDigest(paths[i]);
			if (fileMD5[i] == "")
			{
				fileMD5[i] = "NULL";
				cout << "\a";
				error = error + 1;
			}
			cout << "第" << i + 1 << "个：" << fileMD5[i] << "    " << paths[i] << endl;
		}
		cout << "计算完成" << endl;
		ofstream out2("MD5.txt", ios::app);
		cout << "开始把MD5值写入文件.........." << endl;
		for (int i = 0; i < filesize; i++)
		{
			out2 << fileMD5[i] << endl;
		}
		out2.close();
		cout << "写入完成" << endl;
		if (error != 0)
		{
			cout <<"                       " << error << "个文件没有正确计算出MD5值！！！！！" << endl;
		}
		cout << "任务完成" << endl << endl;
		delete[]paths;
		delete[]fileMD5;
		//system("pause");
	}
	else if (c == '3')
	{
	cout << "请输入文件路径(注意要使用两个\\，而不是一个\\)：" << endl;
	string s2;
	char* s;
	s = (char*)malloc(100);
	//cin >> s;
	gets_s(s, 100);
	cout << "路径：" << s << endl;
	cout << "请稍等......" << endl;
	string s1 = FileDigest(s);
	if (s1.length() == 0)
	{
		cout << "文件不存在！！！" << endl;
		system("pause");
		goto loop;
	}
	cout << "文件的MD5值：" << endl;
	cout << s1 << endl << endl;
	goto loop;
    }

	//*************************计时结束***************************************

	runend();
	system("pause");
	thread t(speak);
	t.detach();

	/*   ************************************                            //微软TTS语音模板
	ISpVoice* pVoice = NULL;                                            //创建一个ISpVoice的空指针
	if (FAILED(::CoInitialize(NULL)))                                     //初始化COM编程环境
	{
		cout << "初始化COM环境失败！" << endl;
		return 0;
	}
	                                                                                   //获取ISpVoice接口对象：
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (SUCCEEDED(hr))                                                      //如果获取接口对象成功
	{
		char* szStr = (char*)malloc(200);
		sprintf(szStr, "你好，世界。");
		WCHAR wszClassName[200];
		memset(wszClassName, 0, sizeof(wszClassName));
		MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
			sizeof(wszClassName) / sizeof(wszClassName[0]));
		hr = pVoice->Speak(wszClassName, 0, NULL);         //朗读
		
		pVoice->Release();                                                 //释放分配资源
		pVoice = NULL;                                                     //防止野指针
	}
	::CoUninitialize();                                                       //退出COM编程环境
	*/  //*********************************
	/*   ************************************                                  //自定义控制台输出位置
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };                                           //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	COORD     cursorPos;                                                       //标准输出句柄
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorPos.X = 0;
	cursorPos.Y = csbi.dwCursorPosition.Y + 1;
	SetConsoleCursorPosition(hStdout, cursorPos);
	cout << "第1行" << endl;
	*/  //*********************************
	/*   **********************************                          //多线程模板
	void task()
    {
	//多线程代码
    }
	thread t(task);
	t.detach();
	*/   //*******************************
	rundisplay();                                                                   //计时结果显示
	_getch();
	//closegraph();
	// system("pause");
	//Sleep(10000);
	return 0;
}
