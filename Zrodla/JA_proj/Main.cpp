#include <iostream> 
#include <Windows.h>
#include <thread>
#include <string>
#include <fstream>

#include "../JA_Cpp_Dll/Dll.h"

typedef DWORD(*Foo)();

int main(int argc, char* argv[]) {
	bool asmDllOn = false, cppDllOn = false; // Zmienne odpowiadajace za wybor wywoania dllki

	unsigned int threadsCount = std::thread::hardware_concurrency(); //iloœæ w¹tków

	std::thread *threadsArray = NULL;; // wskaznik tablicy w¹tków
	int *threadsRange = NULL;;// wskaznik tablicy zakresów
	int x = 0, y = 0, z = 0;
	
	std::fstream file;
	std::string sourceFile="", outFile="";
	
	double **tabA = NULL;
	double **tabB = NULL;
	double **tabC = NULL;
	
	

/// OBS£UGA ARGUMENTOW /////////////////////////
	for (int i = 0; i < argc; i++) {
		std::cout << argv[i];
		// ASM arg
		if ((std::string)argv[i] == "-asm") {
			asmDllOn = true; cppDllOn = false;
		}
		//CPP arg
		else if ((std::string)argv[i] == "-cpp") {
			asmDllOn = false; cppDllOn = true;
		}
		// THREAD arg
		else if ((std::string)argv[i] == "-th") {
			if (i + 1 < argc) {
				i++;
				std::cout << " : " << argv[i];
				if (atoi(argv[i]) > 0) threadsCount = atoi(argv[i]);
			}
		}
		// IN FILE arg
		else if ((std::string)argv[i] == "-inf") {
			if (i + 1 < argc) {
				i++;
				std::cout << " : " << argv[i];
				sourceFile = argv[i];
			}
		}
		// OUT FILE arg
		else if ((std::string)argv[i] == "-outf") {
			if (i + 1 < argc) {
				i++;
				std::cout << " : " << argv[i];
				outFile = argv[i];
			}
		}
		std::cout << std::endl;
	}
	
/// WCZYTYWANIE DANYCH Z PLIKU /////////////////////////////////
	file.open(sourceFile, std::ios::in);
	if (file.good()) {
		std::cout << "Takeing A and C matrix from  file: " << sourceFile << std::endl;
		/// POBRANIE WYMIARÓW 
		file >> x;
		file >> y;
		file >> z;
		/// INICJALIZACJA TABLIC
		tabA = new double *[x];
		tabB = new double *[y];
		tabC = new double *[x];

		for (int i = 0; i < x; i++) {
			tabA[i] = new double[y];
			tabC[i] = new double[z];
		}

		for (int i = 0; i <y; i++) {
			tabB[i] = new double[z];
		}
		/// WCZYTYWANIE TABLICY A
		std::cout << "\nA array:\n";
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				file >> tabA[i][j];
				std::cout << tabA[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";

		/// WCZYTYWANIE TABLICY B
		std::cout << "B array:\n";
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < z; j++) {
				file >> tabB[i][j];
				std::cout << tabB[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < z; j++) {
				tabC[i][j] = 0;
			}
		}


	}
	else {
		std::cout << "Source file ERROR\n";
	}
	file.close();
/// INICJALIZACJA ZMIENNYCH ////////////////////////////////////
	std::cout << std::endl;
	std::cout << "Threads count:" << threadsCount << std::endl;
	std::cout << "First matrix is " << x << "x" << y << std::endl;
	std::cout << "Secound matrix is " << y << "x" << z << std::endl;
	std::cout << std::endl;

	threadsArray = new std::thread[threadsCount]; //deklaracja rozmiaru tablicy w¹tków
	threadsRange = new int[threadsCount + 1]; //deklaracja rozmiaru tablicy zakresu watków
	

	



/// INICJALIZACJA PODZIA£U DANNYCH 
	
	for (int i = 0; i < threadsCount + 1; i++) {
		threadsRange[i] = x * i/ threadsCount;
	}

	std::cout << "threadsRange: \n";
	for (int i = 0; i < threadsCount + 1; i++) {
		std::cout << "nr:" << i << " " << threadsRange[i] << std::endl;
		//std::system("pause");
	}
	std::cout << "\n";
	//std::system("pause");
	
/// DLLka asemblerowa /////////////////////////
	if (asmDllOn) {
		Foo myFunc;
		HMODULE lib;

		if ((lib = LoadLibrary(L"JA_Asm_Dll.dll")) != NULL)
		{
			myFunc = (Foo)GetProcAddress(lib, "Foo");
			if (myFunc != NULL)
			{
				std::cout << (DWORD)(myFunc()) << std::endl;
			}
			FreeLibrary(lib);
		}
	}
/// Dllka cpp //////////////////////
	if (cppDllOn) {
		for (int i = 0; i < threadsCount; i++)
		{
			threadsArray[i] = std::thread(MyClass::Foo, tabA, tabB, tabC, threadsRange[i], threadsRange[i + 1], x, y, z);
		}
		//t[i] = std::thread(call_from_thread, i, tab);
		//MyClass::Foo( tabA[0], tabB[0], tabC[0], 0, 2, x, y, z);
		for (int i = 0; i < threadsCount; i++)
		{
		//	threadsArray[i].join();
		}

		
			
		for (int i = 0; i < threadsCount; i++) {
			threadsArray[i].join();
		}
		
		std::cout << "C matrix is ready\n";
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < z; j++) {
				std::cout << tabC[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

/// ZAPISYWANIE DO PLIKU /////////////////////////////
	file.open(outFile, std::ios::out);
	if (file.good()) {
		std::cout << "Saving C matrix to file: " << outFile << std::endl;
		file << "C matrix " << x << "x" << z << std::endl;
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < z; j++) {
				file << tabC[i][j] << " ";
			}
			file << "\n";
		}
	}
	else {
		std::cout << "Out File ERROR\n";
	}
	file.close();
/// ZWALNIANIE PAMIECI ///////////////////////////////
	for (int i = 0; i < threadsCount; i++) {
		
	}
	//delete(threadsArray);
	//threadsArray = NULL;
	delete(threadsRange);
	threadsRange = NULL;
	for (int i = 0; i < x; i++) {
		delete(tabA[i]);
		tabA[i] = NULL;
		delete(tabC[i]);
		tabC[i] = NULL;
	}
	delete(tabA);
	tabA = NULL;
	delete(tabC);
	tabC = NULL;
	for (int i = 0; i < y; i++) {
		delete(tabB[i]);
		tabB[i] = NULL;
	}
	delete(tabB);
	tabB = NULL;

	system("pause");
	return 0;
}