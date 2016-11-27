#include <iostream> 
#include <Windows.h>
#include <thread>
#include <string>
#include <fstream>

#include "../JA_Cpp_Dll/Dll.h"

typedef DWORD(*Foo)(DWORD,DWORD);

int main(int argc, char* argv[]) {
	bool asmDllOn = false, cppDllOn = false; // Zmienne odpowiadajace za wybor wywoania dllki

	unsigned int threadsCount = std::thread::hardware_concurrency(); //iloœæ w¹tków

	std::thread *threadsArray = NULL;; // wskaznik tablicy w¹tków
	int *threadsRange = NULL;;// wskaznik tablicy zakresów
	int arraySize[3];
	int range[2];
	
	std::fstream file;
	std::string sourceFile="", outFile="";
	double **tab[3] = { NULL,NULL,NULL };

	
	

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
		file >> arraySize[0];
		file >> arraySize[1];
		file >> arraySize[2];
		/// INICJALIZACJA TABLIC
		tab[0] = new double *[arraySize[0]];
		tab[1] = new double *[arraySize[1]];
		tab[2]= new double *[arraySize[2]];

		for (int i = 0; i < arraySize[0]; i++) {
			tab[0][i] = new double[arraySize[1]];
			tab[2][i] = new double[arraySize[2]];
		}

		for (int i = 0; i <arraySize[1]; i++) {
			tab[1][i] = new double[arraySize[2]];
		}
		/// WCZYTYWANIE TABLICY A
		std::cout << "\nA array:\n";
		for (int i = 0; i < arraySize[0]; i++) {
			for (int j = 0; j < arraySize[1]; j++) {
				file >> tab[0][i][j];
				std::cout << tab[0][i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";

		/// WCZYTYWANIE TABLICY B
		std::cout << "B array:\n";
		for (int i = 0; i < arraySize[1]; i++) {
			for (int j = 0; j < arraySize[2]; j++) {
				file >> tab[1][i][j];
				std::cout << tab[1][i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";

		for (int i = 0; i < arraySize[0]; i++) {
			for (int j = 0; j < arraySize[2]; j++) {
				tab[2][i][j] = 0;
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
	std::cout << "First matrix is " << arraySize[0] << "x" << arraySize[1] << std::endl;
	std::cout << "Secound matrix is " << arraySize[1] << "x" << arraySize[2] << std::endl;
	std::cout << std::endl;

	threadsArray = new std::thread[threadsCount]; //deklaracja rozmiaru tablicy w¹tków
	threadsRange = new int[threadsCount + 1]; //deklaracja rozmiaru tablicy zakresu watków
	

	



/// INICJALIZACJA PODZIA£U DANNYCH 
	
	for (int i = 0; i < threadsCount + 1; i++) {
		threadsRange[i] = arraySize[0] * i/ threadsCount;
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
				std::cout << (DWORD)(myFunc(5,2)) << std::endl;
			}
			FreeLibrary(lib);
		}
	}
/// Dllka cpp //////////////////////
	if (cppDllOn) {
		for (int i = 0; i < threadsCount; i++)
		{
			range[0] = threadsRange[i];
			range[1] = threadsRange[i + 1];
			threadsArray[i] = std::thread(MyClass::Foo, tab, threadsRange, arraySize);
		}

		for (int i = 0; i < threadsCount; i++) {
			threadsArray[i].join();
		}
		
		std::cout << "C matrix is ready\n";
		for (int i = 0; i < arraySize[0]; i++) {
			for (int j = 0; j < arraySize[2]; j++) {
				std::cout << tab[2][i][j] << " ";
			}
			std::cout << "\n";
		}
	}

/// ZAPISYWANIE DO PLIKU /////////////////////////////
	file.open(outFile, std::ios::out);
	if (file.good()) {
		std::cout << "Saving C matrix to file: " << outFile << std::endl;
		file << "C matrix " << arraySize[0] << "x" << arraySize[2] << std::endl;
		for (int i = 0; i < arraySize[0] ; i++) {
			for (int j = 0; j < arraySize[2] ; j++) {
				file << tab[2][i][j] << " ";
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
	for (int i = 0; i < arraySize[0] ; i++) {
		delete(tab[0][i]);
		tab[0][i] = NULL;
		delete(tab[2][i]);
		tab[2][i] = NULL;
	}
	delete(tab[0]);
	tab[0] = NULL;
	delete(tab[2]);
	tab[2] = NULL;
	for (int i = 0; i < arraySize[1] ; i++) {
		delete(tab[1][i]);
		tab[1][i] = NULL;
	}
	delete(tab[1]);
	tab[1] = NULL;

	system("pause");
	return 0;
}