#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]

int main() {
	MPI_Init(NULL, NULL); // Initialize MPI
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	LowPassFilter::MyForm form;
	Application::Run(%form);
	MPI_Finalize(); // Finalize MPI
}