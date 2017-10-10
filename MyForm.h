#pragma once
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <msclr\marshal_cppstd.h>

#include <stdlib.h>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

using namespace System;
using namespace System::Windows::Forms;



int cnt = 0;
int c_flag = 0;
int blink = 0;
int face_on = 0;
int close_face = 0;
int eye_work = 0;
clock_t before;
double result;


clock_t startingMoving = 0;
int cycle = 0;

string text = "Gaze the center of Window in 3 seconds";
int fontFace = 1;
double fontScale = 2;
int thickness = 10;
Point textOrg(100, 210);

string hero = "heros\\rocket.png";
int idx = 0;
int p[12] = { 12,600,12,600, 12, 600, 12, 600, 230, 390, 350, 270 };
int q[12] = { 12,440,440,12, 250, 140, 155, 235, 12, 440, 12, 440 };

extern boolean startingVideo = FALSE;
extern boolean startingEx = TRUE;
std::string stream_name;

string console = "";
string face_cascade_name = "haarcascade_frontalface_alt.xml";
string eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Face-eye detection";		//640*480
RNG rng(12345);

void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	//Mat faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	face_on = 0;////////////

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Size size = Size(faces[i].width*0.5, faces[i].height*0.5);
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, size, 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		face_on = 1;////////////////////
	
		//화면으로 부터 기준 30cm로 얼굴을 인식했을 때 반지름을 평균냈을때 140이 나왔다. 얼굴이 가까워지면 반지름의 길이가 늘어나므로 최대 150 보다 커지면 너무 가깝다는 경고 문구를 출력해줌으로써 사용자의 눈을 보호해준다.
		if ((size.width > 150) && (size.height > 150)) { putText(frame, "       It's too close!", textOrg, fontFace, fontScale, Scalar(0, 0, 255)); close_face = 1; }
		else { close_face = 0; }

		
		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		c_flag = 0;
		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
			c_flag = 1;//눈 인식한 걸 의미한다.
			blink = 0;
			
			
		}
		if (c_flag == 0 && blink == 0) {
			cnt += 1;	//눈 감는 횟수
			blink = 1;	//눈 한 번 감으면 cnt가 한 번만 올라가도록한다.
			//printf("%3d\n", cnt);

			result = (double)(clock() - before) / CLOCKS_PER_SEC;
			int result1 = (int)result;
			int result2 = (int)(result * 100) % 100;
			console = to_string(cnt) + "번째\n걸린시간 : " + to_string(result1) + "." + to_string(result2) + "초\n";
			//printf("걸린시간은 %5.2f 입니다.\n", result);
			//console = to_string(cnt) + "번째\n걸린시간 : " + to_string(result) + "초\n";

			before = clock();
			if (result > 5.0) { eye_work = 1; }
		}
		Point save_point = center;
		//if (startingEx == FALSE) { if ((center.x<(save_point.x + 10)) || (center.x<(save_point.x - 10)) || (center.y<(save_point.y + 10)) || (center.y<(save_point.y - 10))) { text = "Please Don't move your head"; } }
		if (startingEx == FALSE) { if ((center.x<340) || (center.x>360) || (center.y<160) || (center.y>320)) { putText(frame, "Please Don't move your head", textOrg, fontFace, fontScale, Scalar(0, 0, 255));; } }
	}



	if (face_on == 0) { putText(frame, " Stop", Point (280,200), fontFace, fontScale, Scalar(0, 0, 255)); }		//얼굴을 인식하지 않을 때 all stop
	
	
	//-- Show what you got
	imshow(window_name, frame);
}




namespace ui_test {



	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm1에 대한 요약입니다.
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::RadioButton^  radioButton5;
	public: System::Windows::Forms::RadioButton^  radioButton6;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;




	protected:

	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다. 
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton5 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton6 = (gcnew System::Windows::Forms::RadioButton());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->panel2->SuspendLayout();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(127, 316);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(158, 28);
			this->textBox1->TabIndex = 0;
			this->textBox1->Text = L"영상제목.확장자";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(313, 316);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 28);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Play";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(8, 12);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(104, 22);
			this->radioButton1->TabIndex = 4;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"iron man";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(8, 40);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(93, 22);
			this->radioButton2->TabIndex = 5;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"Captain";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton2_CheckedChanged);
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(8, 68);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(124, 22);
			this->radioButton3->TabIndex = 6;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"Spider man";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton3_CheckedChanged);
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(8, 96);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(66, 22);
			this->radioButton4->TabIndex = 7;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"Hulk";
			this->radioButton4->UseVisualStyleBackColor = true;
			this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton4_CheckedChanged);
			// 
			// radioButton5
			// 
			this->radioButton5->AutoSize = true;
			this->radioButton5->Location = System::Drawing::Point(8, 124);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(70, 22);
			this->radioButton5->TabIndex = 8;
			this->radioButton5->TabStop = true;
			this->radioButton5->Text = L"Thor";
			this->radioButton5->UseVisualStyleBackColor = true;
			this->radioButton5->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton5_CheckedChanged);
			// 
			// radioButton6
			// 
			this->radioButton6->AutoSize = true;
			this->radioButton6->Location = System::Drawing::Point(8, 224);
			this->radioButton6->Name = L"radioButton6";
			this->radioButton6->Size = System::Drawing::Size(205, 22);
			this->radioButton6->TabIndex = 9;
			this->radioButton6->TabStop = true;
			this->radioButton6->Text = L"i don\'t need exercise";
			this->radioButton6->UseVisualStyleBackColor = true;
			this->radioButton6->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton6_CheckedChanged);
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->radioButton6);
			this->panel2->Controls->Add(this->radioButton5);
			this->panel2->Controls->Add(this->radioButton4);
			this->panel2->Controls->Add(this->radioButton3);
			this->panel2->Controls->Add(this->radioButton2);
			this->panel2->Controls->Add(this->radioButton1);
			this->panel2->Location = System::Drawing::Point(305, 0);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(258, 258);
			this->panel2->TabIndex = 3;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->richTextBox1);
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(258, 258);
			this->panel1->TabIndex = 2;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::panel1_Paint);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(24, 22);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(207, 96);
			this->richTextBox1->TabIndex = 4;
			this->richTextBox1->Text = L"";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(10, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(575, 356);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox1);
			this->Name = L"MyForm";
			this->Text = L"TaserFace";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->panel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}



		void image_overlap(Mat frame)
		{


			text = "Gaze the center of Window";											//눈운동 안내
			if ((idx >= 5) && (idx <7)) { text = "       Check your hero"; }					//눈 운동 이미지 선택 안내
			if ((idx >= 7) && (idx <10)) { text = "Please Don't move your head"; }		//눈운동 주의 사항 경고 문구

			if (idx > 11) { idx = 0; cycle++; }
			if (cycle == 3) { text = "Close your eyes and Rest for 30 seconds"; if (idx >= 3) { eye_work = 0; cycle = 0; idx = 0; } }		//눈운동 두번 반복후 종료
			if ((cycle >= 1) && (cycle <3)) {

				Mat logo = imread(hero);				//눈 운동 이미지 불러옴
				Mat imageROI = frame(Rect(p[idx], q[idx], logo.rows, logo.cols));		//이미지를 오버랩할 영역설정
				Mat mask = imread(hero, 0);		//이미지를 흑백화 하여 불러오기
				logo.copyTo(imageROI, mask);	//영역에 이미지를 원하는 부분만 붙여 넣기
				text = "   Concentrate your Hero!";

			}

			putText(frame, text, textOrg, fontFace, fontScale, Scalar(0, 0, 255));		//Text 
			imshow(window_name, frame);

			double time = (double)(clock() - startingMoving) / CLOCKS_PER_SEC;

			if (time > 1.0) {				//1초마다 눈 운동 image 좌표를 이동 시켜 준다.

				startingMoving = 0;
				idx++;
			}

		}

		void init() {


			CvCapture* capture;
			CvCapture* capture2;
			Mat frame;
			IplImage *frame2;

			//-- 1. Load the cascades
			face_cascade.load("haarcascade_frontalface_alt.xml");
			eyes_cascade.load("haarcascade_eye_tree_eyeglasses.xml");

			//-- 2. Read the video stream
			if (startingVideo == TRUE) {

				capture2 = cvCaptureFromFile(stream_name.c_str());
				capture = cvCaptureFromCAM(0);


				if (capture)
				{

					while (true)
					{
						frame = cvQueryFrame(capture);
						frame2 = cvQueryFrame(capture2);

						//-- 3. Apply the classifier to the frame
						if (!frame.empty())
						{
							detectAndDisplay(frame);
							richTextBox1->Text = gcnew System::String(console.c_str());

							if (eye_work == 1) {							//눈 깜박임 확인 
								if (face_on == 1 && close_face == 0) {		//얼굴 유무 확인, 얼굴이 가까이 있는 지 확인	-> "Stop", "it's too close" 출력
									if (startingMoving == 0) { startingMoving = clock(); }		//타이머	
									if (startingEx == FALSE) {		//운동 희망 여부 확인(라디오버튼6)
										image_overlap(frame);		//눈운동 실행
									}
								}

							}

						}
						else
						{
							break;
						}

						int c = waitKey(10);
						if ((char)c == 'c') { break; }


						if (face_on == 1) { cvShowImage(stream_name.c_str(), frame2); }		//얼굴이 들어오면 영상 실행

					}

				}
			}
			else {
				capture = cvCaptureFromCAM(0);
				if (capture)
				{
					while (true)
					{
						frame = cvQueryFrame(capture);

						if (!frame.empty())
						{
							detectAndDisplay(frame);
							richTextBox1->Text = gcnew System::String(console.c_str());


							if (eye_work == 1) {
								if (face_on == 1 && close_face == 0) {		//얼굴 유무 확인, 얼굴이 가까이 있는 지 확인	-> "Stop", "it's too close" 출력
									if (startingMoving == 0) { startingMoving = clock(); }		//타이머	
									if (startingEx == FALSE) {		//운동 희망 여부 확인(라디오버튼6)
										image_overlap(frame);		//눈운동 실행
									}
								}

							}

						}
						else {break; }

						int c = waitKey(10);
						if ((char)c == 'c') { break; }

					}
				}

			}


		}


#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		stream_name = msclr::interop::marshal_as<std::string>(textBox1->Text);

		startingVideo = TRUE;
		init();
		//destroyAllWindows();
	}
	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		radioButton6->Checked = TRUE;	//라디오 버튼을 TRUE로 설정한다.
		init();
		//destroyAllWindows();
	}
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
		//radioButton6->Checked = FALSE;
		//destroyAllWindows();
		
	}

// 원하는 눈운동 이미지 선택
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	hero = "heros\\ironman.png";
}
private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	
	hero = "heros\\cap.png";
}
private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	hero = "heros\\spidy.png";
}
private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	hero = "heros\\hulk.png";
}
private: System::Void radioButton5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	hero = "heros\\thor.png";
}


//눈운동 희망 여부 선택
private: System::Void radioButton6_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	startingEx = radioButton6->Checked;	//라디오 버튼을 FALSE로 바꾼다
	eye_work = 0;
}

private: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
};
}