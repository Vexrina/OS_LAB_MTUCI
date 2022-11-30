#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <fstream>
#include "process.h"
#include <semaphore>

using namespace std;

void pong();
void pong();
//void Test3(void *);
void SemaphoreTask();

void CriticalSectionSync();
void PrintStrWithSync(LPCWSTR str, CRITICAL_SECTION* cs);
void PrintStrWoSync(LPCWSTR str);
void MutexSync();
void ShowFileMut(HANDLE mutex, UINT* num);
void EditFileMut(HANDLE mutex, UINT* num);
void ThreadingWithEvent(HANDLE ev, LPCWSTR str);
void EventTask();

int main()
{
	int a;
	cout << "take task 1-4:" << endl;
	cin >> a;
	cout << endl;
	switch (a) {
	case 1:
		EventTask();
		break;
	case 2:
		MutexSync();
		break;
	case 3:
		CriticalSectionSync();
		break;
	case 4: 
		SemaphoreTask();
		break;
	default:
		cout << "GoodBye" << endl;
		return 0;
	}

}

// EVENTS

HANDLE hEvent1, hEvent2; /* объявляем  пару глобальных дескрипторов - фактически указателей на любой тип */
int a[5];
HANDLE hThr;
unsigned long uThrID;

void EventTask() {
	hEvent1 = CreateEvent(NULL, FALSE, TRUE, NULL); /* событие создаётся в сигнальном состоянии , с автоматическим сбросом*/
	hEvent2 = CreateEvent(NULL, FALSE, FALSE, NULL); /* событие создаётся в НЕсигнальном состоянии , с автоматическим сбросом */
	hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadingWithEvent, NULL, 0, &uThrID);
	int count = 25;
	while (count>0) // запускаем бесконечный цикл
	{
		WaitForSingleObject(hEvent1, INFINITE); /* эта функция  дожидатся момента,
		когда событие hEvent1 окажется в "сигнальном состоянии" (изначально событие уже в этом состоянии)
		- и только после этого продолжит выполнение программы -
		при этом после продолжения работы hEvent1 будет "сброшено"   так как оно
		создано как "автоматически сбрасываемое (см. примечание )"*/
		Sleep(10); /* это мой дополнение к исходному коду - подождём секунду , а затем выводим строчку , а то всё слишком бчстро)) */
		printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]); // выводим на экран строчку одинакых чисел равных переменной num
		SetEvent(hEvent2); // переводим hEvent2 в сигнальное состояние
		count--;
	}
}

void ThreadingWithEvent(HANDLE ev, LPCWSTR str) {
	int i, num = 0;
	while (1) // запускаем бесконечный цикл
	{
		WaitForSingleObject(hEvent2, INFINITE); /* приостанавливаем выполнение программы до того времени пока hEvent2
				не окажется в сигнальном состоянии (а он при достижении данной "строчки" уже будет прибывать именно в
				сигнальном состоянии)  -вообще в данной программе в обеих функциях
				проверки WaitForSingleObject не остановят выполнение ни на миг ,так как оба объекта-события main
				и hEvent2 будут заблоговременно переводится в сигнальное состояние
				hEvent2- фукции main() , а hEvent1 в функции Thread()   */
		for (i = 0; i < 5; i++) a[i] = num; // присваиваем каждому элементу массива значение = переменной num
		num++; // наращиваем значение num на единицу 
		SetEvent(hEvent1);// переводим hEvent1 в сигнальное состояние
	}
}

// MUTEX
// Changing file
void EditFileMut(HANDLE mutex, UINT* num) {
	// Main cicle of process
	Sleep(10);
	while ((*num) > 0) {
		// Initialize classes of file and str
		locale defaultLocale("");
		wifstream fileRead;
		fileRead.imbue(defaultLocale);
		wofstream fileWrite;
		fileWrite.imbue(defaultLocale);
		wstring wstr;
		Sleep(150);
		// waiting for mutex
		DWORD dwWaitResult = WaitForSingleObject(mutex, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Read str from file
			fileRead.open("text.txt");
			getline(fileRead, wstr, L'\0');
			fileRead.close();

			// Cut str from file
			wstr = wstr.substr(0, wstr.length() - 1);
			*num = wstr.length();

			// Writing cutting str into file
			fileWrite.open("text.txt");
			fileWrite << wstr.c_str();
			fileWrite.close();
		}
		ReleaseMutex(mutex);
		Sleep(200);
	}
}
void ShowFileMut(HANDLE mutex, UINT* num) {
	wifstream fileRead;
	locale defaultLocale("");
	fileRead.imbue(defaultLocale);
	while ((*num)>0){
		DWORD dwWaitResult = WaitForSingleObject(mutex, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			fileRead.open("text.txt");
			wcout << fileRead.rdbuf() << endl;
			fileRead.close();
		}
		ReleaseMutex(mutex);
		Sleep(200);
	}
}
void MutexSync() {
	// Create mutex and str for writing into file
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"MutexSync");
	wstring wstr = L"This string is so hot";

	// Checking to open the application
	DWORD dwWaitResult = WaitForSingleObject(hMutex, 5000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		ReleaseMutex(hMutex);
	}
	else {
		wcout << L"Application is running!\n This application will be closed" << endl;
		system("Pause");
		exit(1);
	}

	// rewriting information in file
	locale defaultLocale("");
	wofstream fileWrite;
	fileWrite.imbue(defaultLocale);
	fileWrite.open("text.txt");
	fileWrite << wstr.c_str();
	fileWrite.close();

	// Create thread for output information from file
	UINT num = wstr.length();
	thread thr1(ShowFileMut, hMutex, &num);
	thread thr2(EditFileMut, hMutex, &num);
	thr1.join();
	thr2.join();
	CloseHandle(hMutex);
	wcout << endl;
}

//CRIT SECTION
void PrintStrWoSync(LPCWSTR str) { // outputs the string character by character
	// without synchronization
	for (int i = 0; i < lstrlen(str); i++)
		wcout << str[i];
	wcout << endl;
}
void PrintStrWithSync(LPCWSTR str, CRITICAL_SECTION* cs) { // outputs the string character by character
	// with sync
	EnterCriticalSection(cs);
	for (int i = 0; i < lstrlen(str); i++)
		wcout << str[i];
	wcout << endl;
	LeaveCriticalSection(cs);
}
void CriticalSectionSync() {
	// Create crit sect and str for output
	LPCRITICAL_SECTION cs = new CRITICAL_SECTION;
	LPCWSTR str1 = L"First test string", str2 = L"Second test string", str3 = L"Third test string";

	// using crit section for synchronization output strings
	wcout << L"Ouputrs strings with sync" << endl;
	InitializeCriticalSection(cs);
	thread thr1(PrintStrWithSync, str1, cs);
	thread thr2(PrintStrWithSync, str2, cs);
	thread thr3(PrintStrWithSync, str3, cs);
	thr1.join();
	thr2.join();
	thr3.join();
	wcout << endl;
	DeleteCriticalSection(cs);
	delete cs;

	// outputs string w/o sync
}

// Semaphore
counting_semaphore<1> signal2Ping(0);       // (1)
counting_semaphore<1> signal2Pong(0);       // (2)

atomic<int> counter{};
constexpr int countlimit = 1'000'000;

void ping() {
	while (counter <= countlimit) {
		signal2Ping.acquire();                  // (5)
		++counter;
		signal2Pong.release();
	}
}

void pong() {
	while (counter < countlimit) {
		signal2Pong.acquire();
		signal2Ping.release();                  // (3)
	}
}

void SemaphoreTask() {
	auto start = chrono::system_clock::now();

	signal2Ping.release();                     // (4)
	thread t1(ping);
	thread t2(pong);

	t1.join();
	t2.join();

	chrono::duration<double> dur = chrono::system_clock::now() - start;
	cout << "Duration: " << dur.count() << " seconds" << '\n';
}