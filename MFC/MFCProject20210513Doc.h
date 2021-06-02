
// MFCProject20210513Doc.h: CMFCProject20210513Doc 클래스의 인터페이스
//


#pragma once


class CMFCProject20210513Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMFCProject20210513Doc() noexcept;
	DECLARE_DYNCREATE(CMFCProject20210513Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	unsigned char* m_InImg;       // bmp파일의 이미지 내용 저장
	unsigned char* m_OutImg;     // bmp이미지 출력용 배열
	BITMAPFILEHEADER dibHf;    // 비트맵 파일헤드 구조체
	BITMAPINFOHEADER dibHi;    // 비트맵 영상헤드 구조체
	BITMAPFILEHEADER re_dibHf;
	BITMAPINFOHEADER re_dibHi;
	BITMAPFILEHEADER temp_dibHf;
	BITMAPINFOHEADER temp_dibHi;
	RGBQUAD palRGB[256];        // 팔레트 정보 구조체 배열
	virtual ~CMFCProject20210513Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnGrayscale();
	int height;
	int width;
	int ImgSize;
	int re_height;
	int re_width;
	void OnDownsampling();
	int re_ImSize;
	void OnBackground();
	unsigned char* MakeMask(unsigned char* m_Target);
};
