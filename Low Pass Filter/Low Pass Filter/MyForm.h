#pragma once
#include "edits.h"

namespace LowPassFilter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	protected:
	private: System::Windows::Forms::Button^ button2;

	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ button3;
	private: System::String^ editImagePath;


	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Label^ label1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(164, 26);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(85, 30);
			this->button1->TabIndex = 0;
			this->button1->Text = L"OpenMP";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(287, 26);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 30);
			this->button2->TabIndex = 1;
			this->button2->Text = L"MPI";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(165, 74);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(700, 660);
			this->pictureBox1->TabIndex = 3;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &MyForm::pictureBox1_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(553, 28);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(117, 30);
			this->button3->TabIndex = 4;
			this->button3->Text = L"Choose image";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(393, 30);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(133, 22);
			this->textBox1->TabIndex = 5;
			this->textBox1->Text = L"Odd Kernel number";
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(728, 33);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(69, 16);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Runtime = ";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlLight;
			this->ClientSize = System::Drawing::Size(1017, 746);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"Low Pass Filter";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {


	}

	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
		openFileDialog1->InitialDirectory = "d:\\";
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

			pictureBox1->Image = Image::FromFile(openFileDialog1->FileName);
			editImagePath = openFileDialog1->FileName;
		}
	}
private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	int start_s, stop_s, TotalTime = 0;
	int ImageWidth = 4, ImageHeight = 4;
	int* imageData = inputImage(&ImageWidth, &ImageHeight, editImagePath);
	// Get the selected item from the TextBox
	String^ selectedItem = textBox1->Text;

	// Convert the selected item to an integer
	int kernelSize = System::Convert::ToInt32(selectedItem);

	start_s = clock();
	applyLowPassFilter_OpenMP(imageData, ImageWidth, ImageHeight, kernelSize);
	stop_s = clock();
	TotalTime = (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
	label1->Text = "Runtime = " + System::Convert::ToString(TotalTime);
	createImage(imageData, ImageWidth, ImageHeight, kernelSize, 0);

	System::String^ imagePath;
	std::string imgPath;

	imgPath = "..//Data//Output//openmp" + std::to_string(kernelSize) + ".png";
	imagePath = marshal_as<System::String^>(imgPath);
	pictureBox1->Image = Image::FromFile(imagePath);
}

private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	int start_s, stop_s, TotalTime = 0;
	int ImageWidth = 4, ImageHeight = 4;
	int* imageData = inputImage(&ImageWidth, &ImageHeight, editImagePath);
	String^ size = textBox1->Text;
	// Convert the selected item to an integer
	int kernelSize = System::Convert::ToInt32(size);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Get the number of processes
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the current process

	if (rank == 0)
	{
		start_s = clock();
	}

	applyLowPassFilter_MPI(imageData, ImageWidth, ImageHeight, kernelSize, world_size, rank);

	if (rank == 0)
	{
		stop_s = clock();
		TotalTime += (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
		createImage(imageData, ImageWidth, ImageHeight, kernelSize, 1);
	}
	label1->Text = "Runtime = " + System::Convert::ToString(TotalTime);
	System::String^ imagePath;
	std::string imgPath;
	imgPath = "..//Data//Output//mpi" + std::to_string(kernelSize) + ".png";
	imagePath = marshal_as<System::String^>(imgPath);
	pictureBox1->Image = Image::FromFile(imagePath);
}
private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	

	
}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}
