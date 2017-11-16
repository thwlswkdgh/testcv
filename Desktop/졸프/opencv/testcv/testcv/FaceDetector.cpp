
#include  "utils7.hpp"					// �� ������ ���� �Լ� ����


//--------------------------------------------------
// ���� ����
Rect  btn0 , btn1 ;						// ��ư�� ���� �簢��
bool  isSaved   = false ;				// ��ư Ŭ���� ���� ����
bool  isChanged = false ; 

//--------------------------------------------------
// ���콺 �̺�Ʈ
void onMouse( int event, int x, int y, int flags , void*  )
{
	if ( event ==  CV_EVENT_LBUTTONDOWN) 
	{
		if ( btn0.contains(Point(x,y)) )  isSaved	= !isSaved ;		// save ��ư Ŭ���� ��� ����
		if ( btn1.contains(Point(x,y)) )  isChanged = true;				// change ��ư Ŭ���� 
	}
}

void  main()
{

	//----------------------------------------------
	// ���콺 �̺�Ʈ �ݹ��Լ� ���
	namedWindow("�󱼰���");
	setMouseCallback( "�󱼰���", onMouse ) ; 

	//----------------------------------------------
	// ���� ����
	int  h_cnt = 0 , f_cnt = -1;
	string  image_url = "../../image/chap7/3/"  ; 
	string  msg ,fn ; 
	btn0 = Rect( Point(10, 10 ) , Size(140,40) );
	btn1 = Rect( Point(10, 50 ) , Size(140,40) );

	//----------------------------------------------
	// ����� �б�
	CascadeClassifier  faceCascade , eyeCascade;
	load_classifier( faceCascade, faceCascadeFilename);
	load_classifier( eyeCascade , eyeCascadeFilename);

	//---------------------------------------------------------------------
	// ī�޶� �ʱ�ȭ
	//---------------------------------------------------------------------
	VideoCapture  capture = init_camera ( 640 ,480 );

	//---------------------------------------------------------------------
	// �ǽð� �� ����
	//---------------------------------------------------------------------
	while (1){
		Mat  frame = get_videoframe( capture ) ;			// ī�޶� ���� �Է�
		Mat  copy 	= frame.clone();								// ������ ���� ���纻
		//--------------------------------------------
		// ��ư �����
		Mat  tmp = frame(btn0) ;
		tmp += (isSaved)? Scalar(50,50,50):Scalar(0,0,0) ;
		tmp = frame(btn1) ;
		tmp += (isChanged)? Scalar(50,50,50):Scalar(0,0,0) ;

		putText  ( frame ,"save face" , btn0.tl() + Point(5, 26) , 0.6f, Scalar(0,255,0));		
		rectangle( frame ,  btn0 , Scalar(100,100,100)  );
		putText  ( frame ,"change face" , btn1.tl() + Point(5, 26) , 0.6f, Scalar(0,255,0));		
		rectangle( frame ,  btn1 , Scalar(100,100,100)  );

		//--------------------------------------------
		// change ��ư ��� - ���� ��� ���� �غ�
		if (isChanged)  { 
			h_cnt++;	f_cnt = -1;
			isChanged = false ;
			isSaved   = false ;
		}

		//---------------------------------------------
		// �󱼰� �� ����
		Rect   face_rects[3];
		Point  obj_pts[3];
		Mat  detectedFace = detect_object( frame,  face_rects, obj_pts, faceCascade,  eyeCascade  );	
		//---------------------------------------------
		// �� �� �� ���� ǥ��
		if (obj_pts[1].x >0 && obj_pts[2].x > 0 )			// �� ������ ����Ǹ�
		{				
			draw_face_eyes(frame, face_rects , obj_pts );
		
			//----------------------------------------
			// save ��ư ��� - ���� ���� 
			if ( isSaved ) {
				fn  = format( "face_%02d_%02d.png", h_cnt , ++f_cnt );
				imwrite( image_url+ fn , copy );
			}
		}

		//---------------------------------------------
		// ���� ���� ǥ��
		if ( f_cnt>=0)
			putText( frame , fn + " is saved." , Point(20, 460) , 0.6f, Scalar(0,255,0));		

		imshow("�󱼰���" , frame);

		if ( waitKey(100) == 27)   break;    // ���� ���� Ű(EscŰ) ����
	}

}
