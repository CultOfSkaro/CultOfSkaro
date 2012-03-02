#pragma once

using namespace HeliosUsbWrapper;

namespace RobotCameraViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  txtDebug;
	protected: 

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
			this->txtDebug = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// txtDebug
			// 
			this->txtDebug->Location = System::Drawing::Point(314, 12);
			this->txtDebug->Multiline = true;
			this->txtDebug->Name = L"txtDebug";
			this->txtDebug->ReadOnly = true;
			this->txtDebug->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtDebug->Size = System::Drawing::Size(303, 422);
			this->txtDebug->TabIndex = 0;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(629, 461);
			this->Controls->Add(this->txtDebug);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			this->txtDebug->AppendText("YAY!\r\n");
			if(HeliosUsb::USBOpenHeliosW((HWND)this->Handle.ToPointer())) {
				this->txtDebug->AppendText("Connected to Helios USB!\r\n");
			} else {
				this->txtDebug->AppendText("Faild to connect to Helios USB!\r\n");
			}
		}
	};
}

