#pragma once
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include "GLM/glm/mat4x4.hpp"
#include <GLM/glm/gtc/matrix_transform.hpp>
#include "GLM/glm/gtx/norm.hpp"
#include "Shader.h"
#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"
Tri_Mesh *mesh;

float pitch = 0;
float yaw = 0;
float ViewerDistance = 3;
int lastMouseX;
int lastMouseY;
Shader* screenShader = nullptr;
Shader* pickShader = nullptr;
Shader* modelShader = nullptr;
Shader* chosenShader = nullptr;
Shader* patchedShader = nullptr;
unsigned int fbo, fboColor, fboDepth;
unsigned int s1fbo, s1fboColor, s1fboDepth;
unsigned int s2fbo, s2fboColor, s2fboDepth;
unsigned int LquadVAO, LquadVBO;
unsigned int RquadVAO, RquadVBO;
unsigned int pickVAO, pickVBO;
unsigned int modelVAO, modelVBO;
unsigned int chosenVAO, chosenVBO;
unsigned int outsideVAO, outsideVBO;
unsigned int insideVAO, insideVBO;
unsigned int solvedVAO, solvedVBO;
unsigned int patchedVAO, patchedVBO;
glm::mat4 view;
glm::mat4 projection;
/////////////////// screen2 分隔線
Shader* mappingShader = nullptr;
unsigned int mappingVAO, mappingVBO;
unsigned int circleVAO, circleVBO;

unsigned int checkboardTexID;
bool drawPatched = false;
enum Choosemode {
	VERTEX,
	FACE,
}choosemode;
float LquadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 0.0f, -1.0f,  1.0f, 0.0f,
		 0.0f,  1.0f,  1.0f, 1.0f
};
float RquadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		 0.0f,  1.0f,  0.0f, 1.0f,
		 0.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		 0.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};
static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};
Mouse::button Mouse_State = Mouse::ROTATE;

float cubeVertices[] = {
	// positions        
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
namespace OpenMesh_EX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadModelToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openModelDialog;
	private: System::Windows::Forms::SaveFileDialog^  saveModelDialog;
	private: System::Windows::Forms::ToolStripMenuItem^  saveModelToolStripMenuItem;
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
	private: System::Windows::Forms::ToolStripMenuItem^ modeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ faceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ vertexToolStripMenuItem;
	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel1;


	protected:

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->faceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->vertexToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->menuStrip1->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->modeToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(819, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->loadModelToolStripMenuItem,
					this->saveModelToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadModelToolStripMenuItem
			// 
			this->loadModelToolStripMenuItem->Name = L"loadModelToolStripMenuItem";
			this->loadModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->loadModelToolStripMenuItem->Text = L"Load Model";
			this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
			// 
			// saveModelToolStripMenuItem
			// 
			this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
			this->saveModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->saveModelToolStripMenuItem->Text = L"Save Model";
			this->saveModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveModelToolStripMenuItem_Click);
			// 
			// modeToolStripMenuItem
			// 
			this->modeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->faceToolStripMenuItem,
					this->vertexToolStripMenuItem
			});
			this->modeToolStripMenuItem->Name = L"modeToolStripMenuItem";
			this->modeToolStripMenuItem->Size = System::Drawing::Size(54, 20);
			this->modeToolStripMenuItem->Text = L"Mode";
			this->modeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::modeToolStripMenuItem_Click);
			// 
			// faceToolStripMenuItem
			// 
			this->faceToolStripMenuItem->Name = L"faceToolStripMenuItem";
			this->faceToolStripMenuItem->Size = System::Drawing::Size(110, 22);
			this->faceToolStripMenuItem->Text = L"Face";
			this->faceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::faceToolStripMenuItem_Click);
			// 
			// vertexToolStripMenuItem
			// 
			this->vertexToolStripMenuItem->Name = L"vertexToolStripMenuItem";
			this->vertexToolStripMenuItem->Size = System::Drawing::Size(110, 22);
			this->vertexToolStripMenuItem->Text = L"Vertex";
			this->vertexToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::vertexToolStripMenuItem_Click);
			// 
			// openModelDialog
			// 
			this->openModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openModelDialog_FileOk);
			// 
			// saveModelDialog
			// 
			this->saveModelDialog->DefaultExt = L"obj";
			this->saveModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveModelDialog_FileOk);
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->hkoglPanelControl1->Location = System::Drawing::Point(3, 3);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(794, 594);
			this->hkoglPanelControl1->TabIndex = 2;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::hkoglPanelControl1_KeyDown);
			this->hkoglPanelControl1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseClick);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel1->Controls->Add(this->hkoglPanelControl1, 0, 0);
			this->tableLayoutPanel1->Location = System::Drawing::Point(12, 27);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(10);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(800, 600);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(819, 633);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"OpenMesh_EX";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e)
{
	//glewExperimental = true;
	glewInit();
	/// init
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POINT_SMOOTH);
	/// 

	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenShader = new Shader("..\\Shaders\\screen.vert", NULL, NULL, NULL, "..\\Shaders\\screen.frag");
	pickShader = new Shader("..\\Shaders\\pick.vert", NULL, NULL, NULL, "..\\Shaders\\pick.frag");
	modelShader = new Shader("..\\Shaders\\model.vert", NULL, NULL, NULL, "..\\Shaders\\model.frag");
	chosenShader = new Shader("..\\Shaders\\chosen.vert", NULL, NULL, NULL, "..\\Shaders\\chosen.frag");
	mappingShader = new Shader("..\\Shaders\\mapping.vert", NULL, NULL, NULL, "..\\Shaders\\mapping.frag");
	patchedShader = new Shader("..\\Shaders\\patched.vert", NULL, NULL, NULL, "..\\Shaders\\patched.frag");

	checkboardTexID = loadTexture("..\\Images\\checkerboard4.jpg");

	screenShader->Use();
	glUniform1i(glGetUniformLocation(screenShader->Program, "screenTexture"), 0);
	glUseProgram(0);

	
	glGenVertexArrays(1, &LquadVAO);
	glGenBuffers(1, &LquadVBO);
	glBindVertexArray(LquadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, LquadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LquadVertices), &LquadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenVertexArrays(1, &RquadVAO);
	glGenBuffers(1, &RquadVBO);
	glBindVertexArray(RquadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, RquadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RquadVertices), &RquadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &fboColor);
	glBindTexture(GL_TEXTURE_2D, fboColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, hkoglPanelControl1->Width, hkoglPanelControl1->Height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	

	glGenRenderbuffers(1, &fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, hkoglPanelControl1->Width, hkoglPanelControl1->Height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepth); // now actually attach it

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboColor, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	//screen1 fbo
	glGenFramebuffers(1, &s1fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, s1fbo);
	glGenTextures(1, &s1fboColor);
	glBindTexture(GL_TEXTURE_2D, s1fboColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, hkoglPanelControl1->Width, hkoglPanelControl1->Height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glGenRenderbuffers(1, &s1fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, s1fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, hkoglPanelControl1->Width, hkoglPanelControl1->Height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s1fboDepth); // now actually attach it

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, s1fboColor, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


	//screen2 fbo
	glGenFramebuffers(1, &s2fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, s2fbo);
	glGenTextures(1, &s2fboColor);
	glBindTexture(GL_TEXTURE_2D, s2fboColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, hkoglPanelControl1->Width , hkoglPanelControl1->Height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glGenRenderbuffers(1, &s2fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, s2fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, hkoglPanelControl1->Width , hkoglPanelControl1->Height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s2fboDepth); // now actually attach it

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, s2fboColor, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

}
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
	float radius = ViewerDistance;
	float x = cos(glm::radians(yaw)) * radius;
	float z = sin(glm::radians(yaw)) * radius;
	float y = -sin(glm::radians(pitch)) * radius;
	view = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = glm::perspective(90, 1, 0, 2000);
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(10, 10, 10));
	view = view * model;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(pickShader->Program, "view"), 1, false, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pickShader->Program, "projection"), 1, false, &projection[0][0]);
	
	glBindVertexArray(pickVAO);
	if(mesh)
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertexNumber);
	
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, s1fbo);

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(modelShader->Program, "view"), 1, false, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(modelShader->Program, "projection"), 1, false, &projection[0][0]);
	glBindVertexArray(modelVAO);
	if (mesh) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertexNumber);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
		
	
	if (mesh&&mesh->chosenFace.size()>0&&choosemode==Choosemode::FACE) {
		mesh->generateChosenData();
		if (!drawPatched) {
			chosenShader->Use();
			glUniformMatrix4fv(glGetUniformLocation(chosenShader->Program, "view"), 1, false, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(chosenShader->Program, "projection"), 1, false, &projection[0][0]);
			glBindVertexArray(chosenVAO);
			glBindBuffer(GL_ARRAY_BUFFER, chosenVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->chosenFaceData.size(), mesh->chosenFaceData.data());
			glDrawArrays(GL_TRIANGLES, 0, mesh->chosenFace.size() * 3);

			glBindVertexArray(outsideVAO);
			glBindBuffer(GL_ARRAY_BUFFER, outsideVBO);

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->outsideVertexData.size(), mesh->outsideVertexData.data());
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, mesh->outsideVertex.size());
		}
		else {
			patchedShader->Use();
			glUniformMatrix4fv(glGetUniformLocation(patchedShader->Program, "view"), 1, false, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(patchedShader->Program, "projection"), 1, false, &projection[0][0]);
			glBindVertexArray(patchedVAO);
			glBindBuffer(GL_ARRAY_BUFFER, patchedVBO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, checkboardTexID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->patchedModelData.size(), mesh->patchedModelData.data());
			glDrawArrays(GL_TRIANGLES, 0, mesh->chosenFace.size() * 5);
		}


		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, s2fbo);

		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mesh->computeTextureCoordinate();
		mappingShader->Use();
		//glUniformMatrix4fv(glGetUniformLocation(mappingShader->Program, "view"), 1, false, &view[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(mappingShader->Program, "projection"), 1, false, &projection[0][0]);

		glBindVertexArray(insideVAO);
		glBindBuffer(GL_ARRAY_BUFFER, insideVBO);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->insideVertexData.size(), mesh->insideVertexData.data());
		glPointSize(20.0f);
		glDrawArrays(GL_POINTS, 0, mesh->insideVertex.size());

		glDisable(GL_CULL_FACE);
		glBindVertexArray(solvedVAO);
		glBindBuffer(GL_ARRAY_BUFFER, solvedVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->solvedData.size(), mesh->solvedData.data());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, mesh->solvedData.size() / 3);
		glEnable(GL_CULL_FACE);


		glBindVertexArray(mappingVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mappingVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->textureData.size(), mesh->textureData.data());
		glLineWidth(3.0f);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawArrays(GL_TRIANGLES, 0, mesh->textureData.size() / 3);

		glBindVertexArray(circleVAO);
		glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->circleData.size(), mesh->circleData.data());
		glLineWidth(1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, mesh->circleData.size() / 3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		glLineWidth(1.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}else if(mesh && mesh->chosenVertexDraw && choosemode == Choosemode::VERTEX){
		mesh->chosenFace.clear();
		chosenShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(chosenShader->Program, "view"), 1, false, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(chosenShader->Program, "projection"), 1, false, &projection[0][0]);
		glBindVertexArray(chosenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, chosenVBO);
		
		
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*3 , mesh->chosenVertex.data());
		glPointSize(15);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		glDisable(GL_DEPTH_TEST);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader->Use();
		glBindVertexArray(LquadVAO);
		glBindTexture(GL_TEXTURE_2D, s1fboColor);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(RquadVAO);
		glBindTexture(GL_TEXTURE_2D, s2fboColor);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	

	
	glUseProgram(0);

	
}                
private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left ||
		e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		lastMouseX = e->X;
		lastMouseY = e->Y;
	}
}
private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left)
	{
		yaw += (e->X - lastMouseX);
		pitch += (e->Y - lastMouseY);
		if (pitch >= 90) {
			pitch = 89.9;
		}
		else if (pitch <= -90) {
			pitch = -89.9;
		}
		lastMouseX = e->X;
		lastMouseY = e->Y;
		hkoglPanelControl1->Invalidate();
	}

	if (e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		
		hkoglPanelControl1->Invalidate();
	}
}
private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Delta < 0)
	{
		ViewerDistance -= 0.2f;
		hkoglPanelControl1->Invalidate();
	}
	else
	{
		ViewerDistance += 0.2f;
		hkoglPanelControl1->Invalidate();
	}
}
private: System::Void loadModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	openModelDialog->Filter = "Model(*.obj)|*obj";
	openModelDialog->Multiselect = false;
	openModelDialog->ShowDialog();
}
private: System::Void openModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	std::string filename;
	MarshalString(openModelDialog->FileName, filename);

	if (mesh != NULL)
		delete mesh;

	mesh = new Tri_Mesh;

	if (ReadFile(filename, mesh))
		std::cout << filename << std::endl;

	mesh->chosenFace.clear();
	mesh->chosenFaceData.clear();

	mesh->initAvailableData();
	glGenVertexArrays(1, &pickVAO);
	glGenBuffers(1, &pickVBO);
	glBindVertexArray(pickVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pickVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size() + sizeof(int)*mesh->faceData.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->vertexData.size(), mesh->vertexData.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), sizeof(int) * mesh->faceData.size(), mesh->faceData.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, 0, (void*)(sizeof(float)*mesh->vertexData.size()));

	glGenVertexArrays(1, &modelVAO);
	glGenBuffers(1, &modelVBO);
	glBindVertexArray(modelVAO);
	glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), mesh->vertexData.data() , GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	
	glGenVertexArrays(1, &chosenVAO);
	glGenBuffers(1, &chosenVBO);
	glBindVertexArray(chosenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chosenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &solvedVAO);
	glGenBuffers(1, &solvedVBO);
	glBindVertexArray(solvedVAO);
	glBindBuffer(GL_ARRAY_BUFFER, solvedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &outsideVAO);
	glGenBuffers(1, &outsideVBO);
	glBindVertexArray(outsideVAO);
	glBindBuffer(GL_ARRAY_BUFFER, outsideVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &insideVAO);
	glGenBuffers(1, &insideVBO);
	glBindVertexArray(insideVAO);
	glBindBuffer(GL_ARRAY_BUFFER, insideVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &mappingVAO);
	glGenBuffers(1, &mappingVBO);
	glBindVertexArray(mappingVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mappingVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &circleVAO);
	glGenBuffers(1, &circleVBO);
	glBindVertexArray(circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glGenVertexArrays(1, &patchedVAO);
	glGenBuffers(1, &patchedVBO);
	glBindVertexArray(patchedVAO);
	glBindBuffer(GL_ARRAY_BUFFER, patchedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexData.size(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));


	hkoglPanelControl1->Invalidate();
}
private: System::Void saveModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	saveModelDialog->Filter = "Model(*.obj)|*obj";
	saveModelDialog->ShowDialog();
}
private: System::Void saveModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	std::string filename;
	MarshalString(saveModelDialog->FileName, filename);

	if (SaveFile(filename, mesh))
		std::cout << filename << std::endl;

}
private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	

}
private: System::Void hkoglPanelControl1_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		

		int windowX = e->X * 2;
		int windowY = hkoglPanelControl1->Height - e->Y;

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		//std::cout << glGetError() << "\n";

		GLuint faceID = 0;
		
		glReadPixels(windowX, windowY, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &faceID);
		//std::cout << glGetError() << "\n";

		//std::cout << windowX << " " << windowY << " " << faceID << "\n";
		
		

		if (mesh&&faceID>0) {
			if (mesh->chosenFace.find(faceID - 1) != mesh->chosenFace.end())
				mesh->chosenFace.erase(faceID - 1);
			else
				mesh->chosenFace.insert(faceID - 1);
		}

		glReadBuffer(GL_DEPTH_ATTACHMENT);
		float depthValue = 0;
		glReadPixels(windowX, windowY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);

		faceID -= 1;//弄回原本faceID
		

		GLint _viewport[4];
		glGetIntegerv(GL_VIEWPORT, _viewport);

		glm::vec4 viewport(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
		glm::vec3 pos(windowX, windowY, depthValue);
		glm::vec3 wp = glm::unProject(pos, view, projection, viewport);
		
		
		//std::cout << depthValue << "\n";
		//std::cout << wp[0] << " " << wp[1] << " " << wp[2] << "\n";
		if (choosemode == Choosemode::VERTEX) {
			glm::vec3 fv[3];
			fv[0] = glm::vec3(mesh->vertexData[faceID * 9 + 0], mesh->vertexData[faceID * 9 + 1], mesh->vertexData[faceID * 9 + 2]);
			fv[1] = glm::vec3(mesh->vertexData[faceID * 9 + 3], mesh->vertexData[faceID * 9 + 4], mesh->vertexData[faceID * 9 + 5]);
			fv[2] = glm::vec3(mesh->vertexData[faceID * 9 + 6], mesh->vertexData[faceID * 9 + 7], mesh->vertexData[faceID * 9 + 8]);
			glm::vec3 cloest = fv[0];
			int closestDistance = 100000000;
			for (int i = 0; i < 3; ++i) {
				if (glm::length2(fv[i] - wp) < closestDistance) {
					closestDistance = glm::length2(fv[i] - wp);
					cloest = fv[i];
				}
			}
			mesh->chosenVertex.clear();
			mesh->chosenVertex.push_back(cloest[0]);
			mesh->chosenVertex.push_back(cloest[1]);
			mesh->chosenVertex.push_back(cloest[2]);
			mesh->chosenVertexDraw = true;
		}
		
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		hkoglPanelControl1->Invalidate();
	}
}
private: System::Void modeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void faceToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	choosemode = Choosemode::FACE;
}
private: System::Void vertexToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	choosemode = Choosemode::VERTEX;
}

private: System::Void hkoglPanelControl2_Load(System::Object^ sender, System::EventArgs^ e) {
	//glewInit();
	/// init
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POINT_SMOOTH);
	/// 

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//hkoglPanelControl2->Invalidate();
}
private: System::Void hkoglPanelControl2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	
	glm::mat4 S2_view = glm::lookAt(glm::vec3(0, 0, -50), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 S2_projection = glm::perspective(90, 1, 0, 2000);
	glm::mat4 S2_model(1.0f);
	S2_model = glm::scale(S2_model, glm::vec3(1, 1, 1));
	view = view * S2_model;

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	glUseProgram(0);
}
private: System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
	
}
private: System::Void hkoglPanelControl1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
	switch (e->KeyCode)
	{
	case Keys::G:
		if (!drawPatched) {
			mesh->buildPatchedData();
			std::cout << "build\n";
			drawPatched = true;
		}
		else {
			drawPatched = false;
		}
		hkoglPanelControl1->Invalidate();
		break;
	default:
		break;
	}
}
};
}
