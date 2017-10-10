#include "MyForm.h"

#include "MyForm.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>


/*using namespace std;
using namespace cv;*/
using namespace System;
using namespace System::Windows::Forms;
[STAThread]


int main()
{

	
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ui_test::MyForm form;


	Application::Run(%form);
	destroyAllWindows();

	return 0;

}