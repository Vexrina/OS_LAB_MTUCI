#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>


using namespace std;
volatile int counter = 0;

CRITICAL_SECTION critical;
HANDLE mutex;
HANDLE event;
HANDLE semaphore;
int Counter = 0;


int isPrime(int n) {
	for (int i = 2; i < (int)(sqrt((float)n) + 1.0); i++) {
		if (n % i == 0) return 0;
	}
	return 1;
}
void addCount(int increment)
{
	WaitForSingleObject(semaphore, INFINITE);
	Counter += increment;
	ReleaseSemaphore(semaphore, 1, 0);
}

unsigned int _stdcall mythreadforCS(void*);
void CSSync();

unsigned int _stdcall mythreadforMutex(void*);
void MutexSync();

unsigned int _stdcall mythreadforEventA(void*);
unsigned int _stdcall mythreadforEventB(void*);
void EventSync();

unsigned int _stdcall mythreadforSemaphore(void*);
void SemaphoreSync();

int main()
{
	int a;
	cout << "take task 1-4:" << endl;
	cin >> a;
	cout << endl;
	while (a > 0 && a < 5) {
		switch (a) {
		case 1:
			CSSync();
			break;
		case 2:
			MutexSync();
			break;
		case 3:
			EventSync();
			break;
		case 4:
			SemaphoreSync();
			break;
		default:
			cout << "GoodBye" << endl;
			return 0;
		}
		cout << "take task 1-4:" << endl;
		cin >> a;
	}
	return 0;
}

unsigned int _stdcall mythreadforCS(void*) {
	string s;
	while (counter < 25) {
		EnterCriticalSection(&critical);
		int number = counter++;
		LeaveCriticalSection(&critical);
		s = "No";
		if (isPrime(number)) s = "Yes";
		const char str[] = "Thread %d value = %d is prime = %s\n";
		cout << "Thread " << GetCurrentThreadId() << " value = " << number << " is prime? " << s << endl;//printf(str, GetCurrentThreadId(), number, s);
	}
	return 0;
}
void CSSync() {
	HANDLE CSSA, CSSB;

	InitializeCriticalSection(&critical);

	CSSA = (HANDLE)_beginthreadex(0, 0, &mythreadforCS, (void*)0, 0, 0);
	CSSB = (HANDLE)_beginthreadex(0, 0, &mythreadforCS, (void*)0, 0, 0);

	WaitForSingleObject(CSSA, INFINITE);
	WaitForSingleObject(CSSB, INFINITE);

	CloseHandle(CSSA);
	CloseHandle(CSSB);
	DeleteCriticalSection(&critical);
}

unsigned int _stdcall mythreadforMutex(void*) {
	string s;
	while (counter < 25) {
		WaitForSingleObject(mutex, INFINITE);
		int number = counter++;
		ReleaseMutex(mutex);
		s = "No";
		if (isPrime(number)) s = "Yes";
		const char str[] = "Thread %d value = %d is prime = %s\n";
		cout << "Thread " << GetCurrentThreadId() << " value = " << number << " is prime? " << s << endl;
	}
	return 0;
}
void MutexSync() {
	HANDLE MA, MB;

	mutex = CreateMutex(0, 0, 0);

	MA = (HANDLE)_beginthreadex(0, 0, &mythreadforMutex, (void*)0, 0, 0);
	MB = (HANDLE)_beginthreadex(0, 0, &mythreadforMutex, (void*)1, 0, 0);

	WaitForSingleObject(MA, INFINITE);
	WaitForSingleObject(MB, INFINITE);

	CloseHandle(MA);
	CloseHandle(MB);

	CloseHandle(mutex);
}

unsigned int _stdcall mythreadforEventA(void*) {
	WaitForSingleObject(event, INFINITE);
	printf("Thread A finished\n");
	return 0;
}
unsigned int _stdcall mythreadforEventB(void*) {
	printf("Thread B finished\n");

	/* A call to SetEvent() places the event into the signaled state.
	This allows threads waiting on the event
	using WaitForSingleObject() to be released. */
	SetEvent(event);

	return 0;
}
void EventSync() {
	HANDLE EA, EB;

	/*This object requires manual reset
	and is created in the unsignaled state */
	event = CreateEvent(0, 0, 0, 0);

	EA = (HANDLE)_beginthreadex(0, 0, &mythreadforEventA, (void*)0, 0, 0);
	EB = (HANDLE)_beginthreadex(0, 0, &mythreadforEventB, (void*)1, 0, 0);

	WaitForSingleObject(EA, INFINITE);
	WaitForSingleObject(EB, INFINITE);

	CloseHandle(EA);
	CloseHandle(EB);

	CloseHandle(event);

	getchar();
}

unsigned int _stdcall mythreadforSemaphore(void*) {
	for (int i = 0; i < 50; i++) addCount(2);
	return 0;
}
void SemaphoreSync() {
	HANDLE SA, SB;

	semaphore = CreateSemaphore(0, 1, 1, 0);

	SA = (HANDLE)_beginthreadex(0, 0, mythreadforSemaphore, (void*)0, 0, 0);
	SB = (HANDLE)_beginthreadex(0, 0, mythreadforSemaphore, (void*)0, 0, 0);

	WaitForSingleObject(SA, INFINITE);
	WaitForSingleObject(SB, INFINITE);

	CloseHandle(SA);
	CloseHandle(SB);

	printf("counter = %d\n", counter);

	CloseHandle(semaphore);
}