
// MFCProject20210513Doc.cpp: CMFCProject20210513Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCProject20210513.h"
#endif

#include "MFCProject20210513Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCProject20210513Doc

IMPLEMENT_DYNCREATE(CMFCProject20210513Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCProject20210513Doc, CDocument)
END_MESSAGE_MAP()


// CMFCProject20210513Doc 생성/소멸

CMFCProject20210513Doc::CMFCProject20210513Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMFCProject20210513Doc::~CMFCProject20210513Doc()
{
}

BOOL CMFCProject20210513Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMFCProject20210513Doc serialization

void CMFCProject20210513Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMFCProject20210513Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CMFCProject20210513Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCProject20210513Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCProject20210513Doc 진단

#ifdef _DEBUG
void CMFCProject20210513Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCProject20210513Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCProject20210513Doc 명령


BOOL CMFCProject20210513Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 구현 코드 추가.
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	//CFile에서 처리
	CFile hFile;        //파일을 읽어들이면 이 클래스 형식으로 저장.
	       //이미지의 높이를 저장할 변수
	       //이미지의 가로길이를 저장할 변수

	hFile.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);    //파일 열기
	hFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));                          //dibHf에 파일헤더를 읽어들인다.
	//이 파일이 BMP파일인지 검사 
	if (dibHf.bfType != 0x4D42)
	{
		AfxMessageBox(L"Not BMP file!!");                                        //프로젝트 생성시 유니코드를 사용하게 할 경우
		return FALSE;                                                                      //L을 붙여준다
	}
	hFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));                             //영상정보의 header를 읽기
	if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24)                                //8,24비트가 아닐경우
	{
		AfxMessageBox(L"Gray/True Color Possible!!");
		return FALSE;
	}
	if (dibHi.biBitCount == 8)
		//8비트의 경우 팔레트를 생성해 주어야 한다. 총 256가지 색이므로 그 길이만큼 읽어들인다
		hFile.Read(palRGB, sizeof(RGBQUAD) * 256);

	//메모리 할당
	if (dibHi.biBitCount == 8)
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);    //이미지의 크기는 파일 총 길이에서, 두 헤드와 팔레트의 사이즈를 
	}                                                                       //제외한다.
	else if (dibHi.biBitCount == 24) //컬러영상
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	}

	//이미지를 저장, 출력할 배열생성.
	m_InImg = new unsigned char[ImgSize];
	m_OutImg = new unsigned char[ImgSize];
	hFile.Read(m_InImg, ImgSize);
	hFile.Close();
	re_dibHf = dibHf;
	re_dibHi = dibHi;
	//이미지의 길이정보
	height = dibHi.biHeight;
	width = dibHi.biWidth;
	int a = 0;
	a = dibHi.biSize;
	a = dibHi.biXPelsPerMeter;
	a = dibHi.biYPelsPerMeter;

	return TRUE;
}


BOOL CMFCProject20210513Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 구현 코드 추가.
	//정보저장
	CFile File; // 파일 객체 선언
	CFileDialog SaveDlg(FALSE, L"bmp", NULL, OFN_HIDEREADONLY);
	// raw 파일을 다른 이름으로 저장하기를 위한 대화상자 객체 선언

	if (SaveDlg.DoModal() == IDOK) {
		// DoModal 멤버 함수에서 저장하기 수행
		File.Open(SaveDlg.GetPathName(), CFile::modeCreate |
			CFile::modeWrite);
		// 파일 열기
		File.Write(&re_dibHf, sizeof(BITMAPFILEHEADER));
		File.Write(&re_dibHi, sizeof(BITMAPINFOHEADER));
		if (re_dibHi.biBitCount == 8)
			File.Write(palRGB, sizeof(RGBQUAD) * 256);
		File.Write(m_OutImg, re_dibHi.biSizeImage);
		File.Close(); // 파일 닫기
	}

	return TRUE;

}


void CMFCProject20210513Doc::OnGrayscale()
{
	// TODO: 여기에 구현 코드 추가.
	re_height = height;
	re_width = width;
	m_OutImg = new unsigned char[ImgSize];
	re_dibHf = dibHf;
	re_dibHi = dibHi;
	
	int i, sum;
	for (i = 0; i < ImgSize; i += 3) {
		sum = m_InImg[i];
		sum += m_InImg[i + 1];
		sum += m_InImg[i + 2];
		m_OutImg[i] = m_OutImg[i + 1] = m_OutImg[i + 2] = sum / 3;
	}
}


void CMFCProject20210513Doc::OnDownsampling()
{
	// TODO: 여기에 구현 코드 추가.
	re_height = height/2 ;
	// 축소 영상의 세로 길이를 계산
	re_width = width/2 ;
	// 축소 영상의 가로 길이를 계산
	re_ImSize = re_height * re_width * 3;
	re_dibHf = dibHf;
	re_dibHi = dibHi;
	re_dibHi.biHeight = re_height;
	re_dibHi.biWidth = re_width;
	re_dibHi.biSizeImage = re_ImSize;

	
	// 축소 영상의 크기를 계산
	int i, j, k;

	m_OutImg = new unsigned char[re_ImSize];
	// 축소 영상을 위한 메모리 할당

	for (i = 0; i < re_height; i++) {
		for (j = 0; j < re_width; j++) {
			for (k = 0; k < 3; k++) {
				m_OutImg[(i * re_width * 3) + (j * 3) + k] = m_InImg[(i * width * 3 * 2) + (j * 3 * 2) + k];
			}
		}
	}
}

#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif

#include <Python.h>

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG 1
#endif
void CMFCProject20210513Doc::OnBackground()
{
	// TODO: 여기에 구현 코드 추가.
	CFile hFile;
	CFileDialog OpenDlg(TRUE);
	int i=0;
	unsigned char* temp=NULL, * masktemp=NULL, maskvalue=NULL;
	re_dibHf = dibHf;
	re_dibHi = dibHi;
	re_height = height;
	re_width = width;
	

	m_OutImg = new unsigned char[ImgSize];

	AfxMessageBox(L"합성할 영상을 입력하시오");
	if (OpenDlg.DoModal() == IDOK) { // 합성할 영상을 입력
		hFile.Open(OpenDlg.GetPathName(), CFile::modeRead | CFile::typeBinary);    //파일 열기
		hFile.Read(&temp_dibHf, sizeof(BITMAPFILEHEADER));                          //dibHf에 파일헤더를 읽어들인다.
		//이 파일이 BMP파일인지 검사 
		if (temp_dibHf.bfType != 0x4D42)
		{
			AfxMessageBox(L"Not BMP file!!");                                        //프로젝트 생성시 유니코드를 사용하게 할 경우
			return;                                                                      //L을 붙여준다
		}
		hFile.Read(&temp_dibHi, sizeof(BITMAPINFOHEADER));                             //영상정보의 header를 읽기
		if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24)                                //8,24비트가 아닐경우
		{
			AfxMessageBox(L"Gray/True Color Possible!!");
			return;
		}
		if (dibHi.biBitCount != temp_dibHi.biBitCount || dibHi.biHeight != temp_dibHi.biHeight || dibHi.biWidth != temp_dibHi.biWidth) {
			//8비트의 경우 팔레트를 생성해 주어야 한다. 총 256가지 색이므로 그 길이만큼 읽어들인다
			AfxMessageBox(L"파일 형식이 다릅니다.");
			return;
		}
		else {
			temp = new unsigned char[ImgSize];
			hFile.Read(temp, ImgSize);
		}
		hFile.Close();
	}
	//AfxMessageBox(L"입력 영상의 마스크 영상을 입력하시오");
	//if (OpenDlg.DoModal() == IDOK) { // 합성할 영상을 입력
	//	hFile.Open(OpenDlg.GetPathName(), CFile::modeRead | CFile::typeBinary);    //파일 열기
	//	hFile.Read(&temp_dibHf, sizeof(BITMAPFILEHEADER));                          //dibHf에 파일헤더를 읽어들인다.
	//	//이 파일이 BMP파일인지 검사 
	//	if (temp_dibHf.bfType != 0x4D42)
	//	{
	//		AfxMessageBox(L"Not BMP file!!");                                        //프로젝트 생성시 유니코드를 사용하게 할 경우
	//		return;                                                                      //L을 붙여준다
	//	}
	//	hFile.Read(&temp_dibHi, sizeof(BITMAPINFOHEADER));                             //영상정보의 header를 읽기
	//	if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24)                                //8,24비트가 아닐경우
	//	{
	//		AfxMessageBox(L"Gray/True Color Possible!!");
	//		return;
	//	}
	//	if (dibHi.biBitCount != temp_dibHi.biBitCount || dibHi.biHeight != temp_dibHi.biHeight || dibHi.biWidth != temp_dibHi.biWidth) {
	//		//8비트의 경우 팔레트를 생성해 주어야 한다. 총 256가지 색이므로 그 길이만큼 읽어들인다
	//		AfxMessageBox(L"파일 형식이 다릅니다.");
	//		return;
	//	}
	//	else {
	//		masktemp = new unsigned char[ImgSize];
	//		hFile.Read(masktemp, ImgSize);
	//	}
	//	hFile.Close();
	//}
	masktemp = new unsigned char[ImgSize];
	masktemp = MakeMask(m_InImg);
	for (i < 0; i < ImgSize; i++) {
		maskvalue = 255 - masktemp[i];
		m_OutImg[i] = (m_InImg[i] & masktemp[i]) | (temp[i] & maskvalue);
	}
}


unsigned char* CMFCProject20210513Doc::MakeMask(unsigned char* m_InImg)
{
	PyObject* pName, * pModule, * pFunc, * pValue, * pArg, * ptemp, * objectsRepresentation;
	unsigned char* masktemp = NULL;
	Py_Initialize();
	pName = PyUnicode_FromString("masking");
	pModule = PyImport_Import(pName);
	pFunc = PyObject_GetAttrString(pModule, "maskimg");
	pArg = Py_BuildValue("s", m_InImg); // int형 인자 4개를 전달한다.
	pValue = PyObject_CallObject(pFunc, pArg); // pFunc에 매개변수를 전달해서 실행한다.

	int count = (int)PyList_Size(pValue); // count 변수에 함수에서 반환된 배열의 size를 전달한다.
	//vector<int> arr(count); // 배열을 저장할 vector 생성
	masktemp = new unsigned char[ImgSize];
	const char* return_val;

	for (int i = 0; i < count; i++)
	{
		ptemp = PyList_GetItem(pValue, i);
		objectsRepresentation = PyObject_Repr(ptemp);
		PyObject* str = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8", "~E~");
		return_val = PyBytes_AsString(str);
		masktemp[i] = (unsigned char)strtod(return_val, NULL);
	}

	Py_Finalize();
	return masktemp;
}

