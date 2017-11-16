
#include  "utils7.hpp"					// 얼굴 검출을 위한 함수 모음


//--------------------------------------------------
// 전역 변수
Rect  btn0 , btn1 ;						// 버튼을 위한 사각형
bool  isSaved   = false ;				// 버튼 클릭시 제어 변수
bool  isChanged = false ; 

//--------------------------------------------------
// 마우스 이벤트
void onMouse( int event, int x, int y, int flags , void*  )
{
	if ( event ==  CV_EVENT_LBUTTONDOWN) 
	{
		if ( btn0.contains(Point(x,y)) )  isSaved	= !isSaved ;		// save 버튼 클릭시 토글 반전
		if ( btn1.contains(Point(x,y)) )  isChanged = true;				// change 버튼 클릭시 
	}
}

void  main()
{

	//----------------------------------------------
	// 마우스 이벤트 콜백함수 등록
	namedWindow("얼굴검출");
	setMouseCallback( "얼굴검출", onMouse ) ; 

	//----------------------------------------------
	// 변수 설정
	int  h_cnt = 0 , f_cnt = -1;
	string  image_url = "../../image/chap7/3/"  ; 
	string  msg ,fn ; 
	btn0 = Rect( Point(10, 10 ) , Size(140,40) );
	btn1 = Rect( Point(10, 50 ) , Size(140,40) );

	//----------------------------------------------
	// 검출기 읽기
	CascadeClassifier  faceCascade , eyeCascade;
	load_classifier( faceCascade, faceCascadeFilename);
	load_classifier( eyeCascade , eyeCascadeFilename);

	//---------------------------------------------------------------------
	// 카메라 초기화
	//---------------------------------------------------------------------
	VideoCapture  capture = init_camera ( 640 ,480 );

	//---------------------------------------------------------------------
	// 실시간 얼굴 검출
	//---------------------------------------------------------------------
	while (1){
		Mat  frame = get_videoframe( capture ) ;			// 카메라 영상 입력
		Mat  copy 	= frame.clone();								// 저장을 위한 복사본
		//--------------------------------------------
		// 버튼 만들기
		Mat  tmp = frame(btn0) ;
		tmp += (isSaved)? Scalar(50,50,50):Scalar(0,0,0) ;
		tmp = frame(btn1) ;
		tmp += (isChanged)? Scalar(50,50,50):Scalar(0,0,0) ;

		putText  ( frame ,"save face" , btn0.tl() + Point(5, 26) , 0.6f, Scalar(0,255,0));		
		rectangle( frame ,  btn0 , Scalar(100,100,100)  );
		putText  ( frame ,"change face" , btn1.tl() + Point(5, 26) , 0.6f, Scalar(0,255,0));		
		rectangle( frame ,  btn1 , Scalar(100,100,100)  );

		//--------------------------------------------
		// change 버튼 토글 - 다음 사람 저장 준비
		if (isChanged)  { 
			h_cnt++;	f_cnt = -1;
			isChanged = false ;
			isSaved   = false ;
		}

		//---------------------------------------------
		// 얼굴과 눈 검출
		Rect   face_rects[3];
		Point  obj_pts[3];
		Mat  detectedFace = detect_object( frame,  face_rects, obj_pts, faceCascade,  eyeCascade  );	
		//---------------------------------------------
		// 얼굴 및 눈 영역 표시
		if (obj_pts[1].x >0 && obj_pts[2].x > 0 )			// 눈 영역이 검출되면
		{				
			draw_face_eyes(frame, face_rects , obj_pts );
		
			//----------------------------------------
			// save 버튼 토글 - 영상 저장 
			if ( isSaved ) {
				fn  = format( "face_%02d_%02d.png", h_cnt , ++f_cnt );
				imwrite( image_url+ fn , copy );
			}
		}

		//---------------------------------------------
		// 저장 정보 표시
		if ( f_cnt>=0)
			putText( frame , fn + " is saved." , Point(20, 460) , 0.6f, Scalar(0,255,0));		

		imshow("얼굴검출" , frame);

		if ( waitKey(100) == 27)   break;    // 실행 중지 키(Esc키) 설정
	}

}
