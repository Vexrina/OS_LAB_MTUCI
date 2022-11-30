#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <iomanip>

using namespace std;

void take_task(int task);
void ShowModulesInfo();
void GetModuleInfo();
void GetModuleInfo(LPCWSTR moduleName);
void GetModuleInfo(HMODULE moduleHandle);
void ModuleInfoToConsole(LPCWSTR moduleFilePath, HMODULE& moduleHandle);
void ShowCurrentProccessInfo();
void ShowProcessEntry(PROCESSENTRY32& entry);
void ShowThreadEntry(THREADENTRY32& entry);
void ShowModuleEntry(MODULEENTRY32& entry);
void ShowThreads();
void ShowModules();
void ShowProcesses();
void task3();

int main()
{
	setlocale(LC_ALL, "Russian");
	int a;
	cout << "Take a quest!\n1 - Information about modules\n2 - Information about current process\n"
		<< "3 - All processes, threads, modules\n"
		<< "4 - List of drivers\nElse - exit" << endl;
	cin >> a;
	while (a < 5 && a>0) {
		take_task(a);
		cout << endl;
		cout << "Take a quest!\n1 - Information about modules\n2 - Information about current process\n"
			<< "3 - All processes, threads, modules\n"
			<< "4 - List of drivers\nElse - exit" << endl;
		cin >> a;
	}

	system("pause");
	return 0;
}

// first task
void ModuleInfoToConsole(LPCWSTR moduleFilePath, HMODULE& moduleHandle) {
	wstring moduleFileName = wstring(moduleFilePath);
	moduleFileName = moduleFileName.substr(moduleFileName.find_last_of(L"/\\") + 1);
	wprintf(L"Name of module: ");  wprintf(L"%s", moduleFileName.c_str());
	wprintf(L"\nFull name of module: ");  wprintf(L"%s", moduleFilePath);
	wprintf(L"\nDescriptor adress of module: "); wprintf(L"%u", moduleHandle);
	wprintf(L"\n");
}

void GetModuleInfo() {
	WCHAR filePath[MAX_PATH];
	GetModuleFileName(NULL, filePath, MAX_PATH);
	HMODULE moduleHandle = GetModuleHandle(NULL);
	ModuleInfoToConsole(filePath, moduleHandle);
}

void GetModuleInfo(LPCWSTR moduleName) {
	WCHAR filePath[MAX_PATH];
	HMODULE moduleHandle = GetModuleHandle(moduleName);
	GetModuleFileName(moduleHandle, filePath, MAX_PATH);
	ModuleInfoToConsole(filePath, moduleHandle);
}

void ShowModulesInfo() {
	LPCWSTR module2Name = L"ntdll.dll";
	HMODULE module3Handle = GetModuleHandle(L"kernel32.dll");
	wcout << L"Infromation about current program:" << endl;
	GetModuleInfo();
	wcout << L"\nInformation about module of it name:" << endl;
	GetModuleInfo(module2Name);
	wcout << L"\nInformation about module of it descriptor:" << endl;
	GetModuleInfo(module3Handle);
	cout << endl;
}

void GetModuleInfo(HMODULE moduleHandle) {
	WCHAR filePath[MAX_PATH];
	GetModuleFileName(moduleHandle, filePath, MAX_PATH);
	ModuleInfoToConsole(filePath, moduleHandle);
}

// second task
void ShowCurrentProccessInfo() {
	DWORD curProcID = GetCurrentProcessId(); // current pid

	HANDLE curProcHandle = GetCurrentProcess(); // Pseudodescriptor of current pid
	HANDLE copyCurProcHandle;

	DuplicateHandle(curProcHandle, curProcHandle, curProcHandle, &copyCurProcHandle, PROCESS_ALL_ACCESS, FALSE, curProcID); // real descriptor

	HANDLE openCurProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, curProcID); // new descriptor

	wcout << L"Current PID: " << curProcID
		<< endl << L"Pseudodescriptor of current process: " << curProcHandle
		<< endl << L"Real descriptor of current process: " << copyCurProcHandle
		<< endl << L"New descriptor of current process: " << openCurProcHandle
		<< endl;

	CloseHandle(copyCurProcHandle);
	CloseHandle(openCurProcHandle);
}
// third task

int i = 0;
void ShowProcessEntry(PROCESSENTRY32& entry) {
	cout
		<< "PID" << entry.th32ProcessID << endl// содержится идентификационный номер процесса.
		<< "DefaultHeapID" << entry.th32DefaultHeapID << endl// предназначено для хранения идентификатора {ID} для кучи процесса, действующей по умолчанию.
		<< "ModuleID" << entry.th32ModuleID << endl// идентифицирует модуль, связанный с процессом.
		<< "ParentPID" << entry.th32ParentProcessID << endl// идентифицирует родительский процесс для данного процесса.
		<< "cntUsage" << entry.cntUsage << endl/* В поле cntUsage хранится значение счетчика ссылок процесса.Когда это значение станет
																 равным нулю, операционная система выгрузит процесс. */
		<< "cntThreads" << entry.cntThreads << endl// По значению поля cntThreads можно судить о том, сколько потоков начало выполняться в данном процессе.
		<< "Base prio" << entry.pcPriClassBase << endl// В поле pcPriClassBase хранится базовый приоритет процесса.
		<< "szExeFile" << entry.szExeFile << endl/* В поле szExeFile содержится строка с ограничивающим нуль-символом, которая представляет
														   собой путь и имя файла ЕХЕ - программы или драйвера, связанного с данным процессом. */
		<< endl;
}

void ShowThreadEntry(THREADENTRY32& entry) {
	cout
		<< "ThreadID:\t\t\t" << entry.th32ThreadID << endl// Поле th32ThreadID представляет собой идентификационный номер потока
		<< "OwnerPID:\t\t\t" << entry.th32OwnerProcessID << endl// В поле th320wnerProcessID содержится идентификатор (ID) процесса, которому принадлежит данный поток.
		<< "cntUsage:\t\t\t" << entry.cntUsage << endl/* В поле cntUsage содержится счетчик ссылок данного потока. При обнулении этого счетчика
											поток выгружается операционной системой. */
		<< "BasePrio:\t\t\t" << entry.tpBasePri << endl
		/*"dwFlags" Поле tpBasePri представляет собой базовый класс приоритета потока. Это значение одинаково
		для всех потоков данного процесса.Возможные значения этого поля обычно лежат в диапазоне от 4 до 24.
		Описания этих значений приведены в следующей таблице.
		Значение Описание
		4 Ожидающий
		8 Нормальный
		13 Высокий
		24 Реальное время
		*/
		<< "DeltaPrio:\t\t\t" << entry.tpDeltaPri << endl
		/* Поле tpDeltaPri представляет собой дельта-приоритет (разницу), определяющий величину
		отличия реального приоритета от значения tpBasePri. Это число со знаком, которое в сочетании с
		базовым классом приоритета отображает общий приоритет потока. Константы, определенные для всех
		возможных значений дельта-приоритета, перечислены в следующей таблице.
		Константа Значение
		THREAD_PRIORITY_IDLE		  -15
		THREAD_PRIORITY_LOWEST		   -2
		THREAD PRIORITYBELOW NORMAL    -1
		THREAD_PRIORITY_NORMAL		    0
		THREAD_PRIORITY_ABOVE_NORHAL    1
		THREAD_PRIORITY_H IGHEST	    2
		THREAD_PRIORITY_TIME_CRITICAL  15
		*/
		<< "dwFlags:\t\t\t" << entry.dwFlags << endl
		<< endl;
}

void ShowModuleEntry(MODULEENTRY32& entry) {
	cout
		<< "ModuleID:\t\t\t" << entry.th32ModuleID << endl// Поле th32ModuleID представляет собой идентификатор модуля
		<< "PID:\t\t\t" << entry.th32ProcessID << endl// Поле th32processID содержит идентификатор (ID) опрашиваемого процесса.
		<< "GlblCntUsage:\t\t\t" << entry.GlblcntUsage << endl// Поле GlblcntUsage содержит глобальный счетчик ссылок данного модуля.
		<< "ProcCntUsage:\t\t\t" << entry.ProccntUsage << endl// Поле ProccntUsage содержит счетчик ссылок модуля в контексте процесса-владельца.
		<< "modBaseAddr:\t\t\t" << entry.modBaseAddr << endl// Поле modBaseAddr представляет собой базовый адрес модуля в памяти.
		<< "modBaseSize:\t\t\t" << entry.modBaseSize << endl// Поле modBaseSize определяет размер (в байтах) модуля в памяти.
		<< "hModule:\t\t\t" << entry.hModule << endl// В поле hHodule содержится дескриптор модуля.
		<< "szModule:\t\t\t" << entry.szModule << endl// имя модуля
		<< "szExePath:\t\t\t" << entry.szExePath << endl// Поле szExepath предназначено для хранения строки с ограничивающим нуль-символом, содержащей полный путь модуля.
		<< endl;
}

void ShowThreads() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // Включает в снимок список потоков Win32
	cout << "\nSnapshot ID = " << hSnapshot;
	THREADENTRY32 entry;
	entry.dwSize = sizeof(THREADENTRY32);
	Thread32First(hSnapshot, &entry);
	cout << endl << "\n ********* Information about threads *********" << endl;
	do
	{
		ShowThreadEntry(entry);
	} while (Thread32Next(hSnapshot, &entry) && i < 14);
	CloseHandle(hSnapshot);
	cout << endl << "\t\t********* Information about threads *********" << endl;
}

void ShowModules() {
	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	MODULEENTRY32 meModuleEntry;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	cout << "\nSnapshot ID = " << hSnapshot;
	cout << "\n\n********* Information about modules *********" << endl;
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}
	meModuleEntry.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &meModuleEntry);
	do {
		ShowModuleEntry(meModuleEntry);
		if (i == 10) {
			system("pause");
			i = 0;
		}
		else i++;
	} while (Module32Next(hSnapshot, &meModuleEntry));
	CloseHandle(hSnapshot);
	cout << endl << "\t\t********* Information about modules is ended *********" << endl;
}

void ShowProcesses() {
	PROCESSENTRY32 peProcessEntry;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	cout << "\nSnapshot ID = " << hSnapshot;
	cout << endl << "\t\t\t********* Information about processes *********" << endl;
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}
	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);
	do {
		//cout << endl << "********* Process *********\n" << endl;
		ShowProcessEntry(peProcessEntry);
	} while (Process32Next(hSnapshot, &peProcessEntry));
	CloseHandle(hSnapshot);
	cout << endl << "\t\t********* Information about processes is ended *********" << endl;
}

void task3() {
	ShowProcesses();
	system("pause");
	ShowThreads();
	system("pause");
	ShowModules();
}

void take_task(int task) {
	switch (task) {
	case 1:
		ShowModulesInfo();
		break;
	case 2:
		ShowCurrentProccessInfo();
		break;
	case 3:
		task3();
		break;
	case 4:
		system("driverquery");
		break;
	}
}
