
// MFCProject20210513View.cpp: CMFCProject20210513View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCProject20210513.h"
#endif

#include "MFCProject20210513Doc.h"
#include "MFCProject20210513View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCProject20210513View

IMPLEMENT_DYNCREATE(CMFCProject20210513View, CView)

BEGIN_MESSAGE_MAP(CMFCProject20210513View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCProject20210513View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_GRAYSCALE, &CMFCProject20210513View::OnGrayscale)
	ON_COMMAND(ID_DOWNSAMPLING, &CMFCProject20210513View::OnDownsampling)
	ON_COMMAND(ID_BACKGROUND, &CMFCProject20210513View::OnBackground)
END_MESSAGE_MAP()

// CMFCProject20210513View 생성/소멸

CMFCProject20210513View::CMFCProject20210513View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CMFCProject20210513View::~CMFCProject20210513View()
{
	if (BmInfo) delete BmInfo;
}

BOOL CMFCProject20210513View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCProject20210513View 그리기

void CMFCProject20210513View::OnDraw(CDC* pDC)
{
#define WIDTHBYTES(bits)	(((bits)+31)/32*4);        //이미지 가로 바이트 길이는 4바이트의 배수
	//BmInfo;
	int rwsize;
	CMFCProject20210513Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (pDoc->m_InImg == NULL)
		return;

	//24비트 비트맵 파일의 영상출력
	if (pDoc->dibHi.biBitCount == 24) {
		rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount * pDoc->dibHi.biWidth);
		BmInfo->bmiHeader = pDoc->dibHi;	
		SetDIBitsToDevice(pDC->GetSafeHdc(), 5, 5, pDoc->width, pDoc->height, 0, 0, 0, pDoc->height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
		BmInfo->bmiHeader = pDoc->re_dibHi;
		SetDIBitsToDevice(pDC->GetSafeHdc(), 10 + pDoc->width, 5, pDoc->re_width, pDoc->re_height, 0, 0, 0, pDoc->re_height, pDoc->m_OutImg, BmInfo, DIB_RGB_COLORS);

	}
	else	//8비트 컬러일 경우
	{
		int index;
		rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount * pDoc->dibHi.biWidth);

		//팔레트를 읽어들이며 반복출력
		for (int i = 0; i < pDoc->dibHi.biHeight; i++)
			for (int j = 0; j < pDoc->dibHi.biWidth; j++) {
				index = pDoc->m_InImg[i * rwsize + j];
				BYTE R = pDoc->palRGB[index].rgbRed;
				BYTE G = pDoc->palRGB[index].rgbGreen;
				BYTE B = pDoc->palRGB[index].rgbBlue;
				//pDC->SetPixel(j,i,RGB(R,G,B));
				 //영상 반전출력
				pDC->SetPixel(j, pDoc->dibHi.biHeight - i - 1, RGB(R, G, B));
			}
	}
}


// CMFCProject20210513View 인쇄


void CMFCProject20210513View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCProject20210513View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCProject20210513View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCProject20210513View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFCProject20210513View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCProject20210513View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCProject20210513View 진단

#ifdef _DEBUG
void CMFCProject20210513View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCProject20210513View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCProject20210513Doc* CMFCProject20210513View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCProject20210513Doc)));
	return (CMFCProject20210513Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCProject20210513View 메시지 처리기


void CMFCProject20210513View::OnGrayscale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCProject20210513Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGrayscale();

	Invalidate(TRUE);
}


void CMFCProject20210513View::OnDownsampling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCProject20210513Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnDownsampling();

	Invalidate(TRUE);
}


void CMFCProject20210513View::OnBackground()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCProject20210513Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBackground();

	Invalidate(TRUE);
}
