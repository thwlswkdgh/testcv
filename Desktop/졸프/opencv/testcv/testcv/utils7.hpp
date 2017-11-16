#include  "opencv.hpp"

using  namespace  cv;
using  namespace  std;

const  char  * faceCascadeFilename	= "haarcascades/haarcascade_frontalface_alt2.xml";					// fast Harr 얼굴 검출기
const  char  * eyeCascadeFilename	= "haarcascades/haarcascade_eye.xml"; // 기본 눈 검출기

// 분류기 읽기 
void  load_classifier(CascadeClassifier &cascade , string filename  ) {
	try {  
		string  opencv_classifier_folder = "C:/opencv/opencv/sources/data/" ;		// OpenCV 분류기 파일 폴더
		cascade.load(opencv_classifier_folder + filename );
	} catch (Exception e) {}
	if ( cascade.empty() ) {
		cerr << "에러 : [" << filename << "] 파일을 읽을 수 없습니다.!" << endl;
		exit(-1);
	}
	cout << "[" << filename << "] 읽기 완료." << endl;
}

	// 카메라 초기화
VideoCapture   init_camera( int width , int height , int  cameraNumber = 0 )
{
	VideoCapture   capture ;
	try {   
		capture.open(cameraNumber);
	} catch ( Exception e ) {}
	if ( !capture.isOpened() ) {
		cerr << "에러 : 카메라를 열수 없습니다." << endl;
		exit(1);
	}

	capture.set(CV_CAP_PROP_FRAME_WIDTH , width );
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, height );

	cout << "카메라 준비 완료 ( 카메라 번호 : " << cameraNumber << " )" << endl;	

	return  capture ; 
}

// 카메라 프레임 읽기 및 좌우반전 
Mat  get_videoframe( VideoCapture  capture ){
	Mat  frame ; 
	capture.read( frame );				// 카메라 영상 받기
	if( frame.empty() ) {
		cerr << "ERROR: 카메라를 열수 없습니다." << endl;
		exit(1);
	}
	flip(frame, frame, 1);				// 영상 좌우 뒤집기- 출력 영상을 거울처럼 보기위해
	return frame;
}


//얼굴 및 눈 표시
void  draw_face_eyes(Mat  & frame, Rect rects[] , Point obj_pts[] )
{
	//-----------------------------------
	// 얼굴 크기 및 타원 그리기
	Size   face_size (rects[0].width/2 ,  rects[0].height/2+ 30) ;
	ellipse(frame, obj_pts[0], face_size, 0, 0, 360, CV_RGB(255,255,0) , 3 );
	
	//-----------------------------------
	// 눈 그리기
	circle (frame,  rects[0].tl()+ obj_pts[1] , rects[1].width/2 , CV_RGB(0,255,255) ,2 );
	circle (frame,  rects[0].tl()+ obj_pts[2] , rects[2].width/2 , CV_RGB(0,255,255) ,2 );
}

//글자 쓰기 - 그림자 추가
void putText(Mat &frame, string msg, Point pt, double scale , Scalar color)
{
	putText( frame , msg , pt+Point(1,1) , FONT_HERSHEY_COMPLEX, 0.6f, Scalar(50,50,50) );		
	putText( frame , msg , pt			 , FONT_HERSHEY_COMPLEX, 0.6f, color );		
}


// 객체 검출 
void detectLargestObject ( const Mat &img, CascadeClassifier &cascade, 
			Rect  &largestObject,  int  scaledWidth = 320)
{
	Mat norm_img , equl_img ;
	vector<Rect> objects;						// 검출 객체 사각형들

	//---------------------------------------------
	// 크기 축소 – 속도 향상 위해 
	float  scale			= img.cols / (float)scaledWidth;		// 너비 비율 설정
	int   scaledHeight	= cvRound(img.rows / scale);			// 축소 높이
	if  ( img.cols > scaledWidth)		
				resize(img,  norm_img, Size(scaledWidth, scaledHeight));
	else		
				norm_img = img;

	//---------------------------------------------
	// 평활화 및 얼굴 검출
	equalizeHist(norm_img, equl_img);
	cascade.detectMultiScale(equl_img, objects);

	//---------------------------------------------
	// 검출 사각형 크기 복원
	if (img.cols > scaledWidth) {
		for (int i = 0; i < (int)objects.size(); i++ ) {
			objects[i].x	= cvRound(objects[i].x * scale);
			objects[i].y	= cvRound(objects[i].y * scale);
			objects[i].width	= cvRound(objects[i].width * scale);
			objects[i].height	= cvRound(objects[i].height * scale);
		}
	}

	//---------------------------------------------
	// 예외 처리 
	for (int i = 0; i < (int)objects.size(); i++ ) {
		if (objects[i].x < 0)            objects[i].x = 0;
		if (objects[i].y < 0)            objects[i].y = 0;
		if (objects[i].x + objects[i].width > img.cols)		objects[i].x = img.cols - objects[i].width;
		if (objects[i].y + objects[i].height > img.rows)	objects[i].y = img.rows - objects[i].height;
	}

	//---------------------------------------------
	// 가장 큰 객체만 반환
	if (objects.size() > 0) 	largestObject = (Rect)objects.at(0);
	else								largestObject = Rect(-1,-1,-1,-1);
}

// 눈 검출 사각형에서 중심점 찾기
Point  find_center( Rect  rect )
{
	Point  pt = Point(-1, -1);											// 중심점 초기화
	if ( rect.width > 0) {				
		pt = rect.tl() + Point( rect.width/2, rect.height/2);
	}
	return  pt ;
}

// 객체 검출 –얼굴, 눈
Mat  detect_object(Mat srcImg, Rect * rects, Point * points ,
	CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade)
{
	//-------------------------------------------
	// 컬러 영상이면 명암도 영상 변경
	Mat  gray ;
	if (srcImg.channels() == 3)	cvtColor(srcImg, gray, CV_BGR2GRAY);
	else								gray = srcImg;

	//---------------------------------------------
	// 얼굴과 눈 정보 초기화
	for (int i = 0 ; i < 3; i++) {
		rects[i]= Rect(-1,-1,-1,-1);			// 얼굴 눈 사각형
		points[i] = Point(-1,-1);				// 얼굴 눈 좌표
	}

	//---------------------------------------------
	// 얼굴 검출
	detectLargestObject(gray, faceCascade, rects[0]);			// 가장 큰 사각형 하나 검출

	if (rects[0].width > 0)											// 얼굴이 검출되면
	{
		Mat face = gray(rects[0]);						// 얼굴 영상

		//-----------------------------------------
		// 왼쪽 얼굴, 오른쪽 얼굴 분리
		Size sub_size( cvRound(face.cols * 0.30f), cvRound(face.rows * 0.30f) );
		Point margin_left ( cvRound(face.cols * 0.16f) , cvRound(face.rows * 0.26f) ) ; 
		Point margin_right( cvRound(face.cols * 0.60f) , cvRound(face.rows * 0.26f) ) ; 

		Mat left_face   = face( Rect( margin_left , sub_size ));
		Mat right_Face  = face( Rect( margin_right, sub_size ));

		//-----------------------------------------
		//눈 검출
		detectLargestObject(left_face  , eyeCascade, rects[1] );
		detectLargestObject(right_Face , eyeCascade, rects[2] );

		rects[1] += margin_left ;							//  여백만큼 이동
		rects[2] += margin_right ;

		//-----------------------------------------
		// 얼굴, 눈 중심점
		points[0] = find_center( rects[0] );
		points[1] = find_center( rects[1] );
		points[2] = find_center( rects[2] );

		return  face ;
	}
	return Mat();
}