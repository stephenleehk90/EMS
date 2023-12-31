#ifndef __IMVB_H__
#define __IMVB_H__

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CPicture;

/////////////////////////////////////////////////////////////////////////////
// CImvb wrapper class

class CImvb : public CWnd
{
protected:
	DECLARE_DYNCREATE(CImvb)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xeaba3c0, 0xd884, 0x101b, { 0x8c, 0xae, 0x0, 0x0, 0x6e, 0x44, 0xaa, 0xb2 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:
	CString GetPicture();
	void SetPicture(LPCTSTR);
	long GetImageWidth();
	void SetImageWidth(long);
	long GetImageHeight();
	void SetImageHeight(long);
	long GetImageColors();
	void SetImageColors(long);
	long GetScaleLeft();
	void SetScaleLeft(long);
	long GetScaleTop();
	void SetScaleTop(long);
	long GetScaleWidth();
	void SetScaleWidth(long);
	long GetScaleHeight();
	void SetScaleHeight(long);
	BOOL GetScrollbars();
	void SetScrollbars(BOOL);
	BOOL GetSelect();
	void SetSelect(BOOL);
	short GetMagnification();
	void SetMagnification(short);
	short GetDstLeft();
	void SetDstLeft(short);
	short GetDstTop();
	void SetDstTop(short);
	short GetDstRight();
	void SetDstRight(short);
	short GetDstBottom();
	void SetDstBottom(short);
	short GetSrcLeft();
	void SetSrcLeft(short);
	short GetSrcTop();
	void SetSrcTop(short);
	short GetSrcRight();
	void SetSrcRight(short);
	short GetSrcBottom();
	void SetSrcBottom(short);
	BOOL GetAutoDraw();
	void SetAutoDraw(BOOL);
	short GetImageXRes();
	void SetImageXRes(short);
	short GetImageYRes();
	void SetImageYRes(short);
	short GetExtensionCount();
	void SetExtensionCount(short);
	CString GetSaveOptions();
	void SetSaveOptions(LPCTSTR);
	short GetPaletteEntries();
	void SetPaletteEntries(short);
	short GetPages();
	void SetPages(short);
	short GetPageNumber();
	void SetPageNumber(short);
	long GetDitherMethod();
	void SetDitherMethod(long);
	long GetAutoScale();
	void SetAutoScale(long);
	long GetImageFlags();
	void SetImageFlags(long);
	BOOL GetUseDDB();
	void SetUseDDB(BOOL);
	long GetCompression();
	void SetCompression(long);
	long GetOverwrite();
	void SetOverwrite(long);
	short GetQuality();
	void SetQuality(short);
	CPicture GetVBPicture();
	void SetVBPicture(LPDISPATCH);
	long GetEmbedLength();
	void SetEmbedLength(long);
	long GetEmbedOffset();
	void SetEmbedOffset(long);
	OLE_COLOR GetBackColor();
	void SetBackColor(OLE_COLOR);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	short GetBorderStyle();
	void SetBorderStyle(short);
	OLE_HANDLE GetHWnd();
	void SetHWnd(OLE_HANDLE);
	CString GetExt();
	void SetExt(LPCTSTR);
	long GetScaleMethod();
	void SetScaleMethod(long);
	long GetDisplayColors();
	void SetDisplayColors(long);
	BOOL GetAppendImage();
	void SetAppendImage(BOOL);
	BOOL GetStatusEnabled();
	void SetStatusEnabled(BOOL);
	long GetMousePointer();
	void SetMousePointer(long);
	CPicture GetMouseIcon();
	void SetMouseIcon(LPDISPATCH);
	long GetHDC();
	void SetHDC(long);
	long GetHDIB();
	void SetHDIB(long);
	long GetHImage();
	void SetHImage(long);
	long GetHPalette();
	void SetHPalette(long);
	long GetPrnHdc();
	void SetPrnHdc(long);
	CString GetMemoryFormat();
	void SetMemoryFormat(LPCTSTR);
	long GetImageDataSize();
	void SetImageDataSize(long);
	long GetHImageData();
	void SetHImageData(long);

// Operations
public:
	CString GetExtensions(short index);
	void SetExtensions(short index, LPCTSTR lpszNewValue);
	short GetRed(short index);
	void SetRed(short index, short nNewValue);
	short GetGreen(short index);
	void SetGreen(short index, short nNewValue);
	short GetBlue(short index);
	void SetBlue(short index, short nNewValue);
	void Refresh();
	void Brightness(short Value);
	void PrintImage();
	void IncreaseTo(short nColors);
	void Invert();
	void ReduceTo(short nColors, BOOL bGreyScale, BOOL bOptimizedPalette);
	void MirrorVertical();
	void MirrorHorizontal();
	void ImageCut();
	void ImageCopy();
	void ImagePaste();
	CString SaveAs(LPCTSTR SaveFileName);
	void Gamma(float Gamma);
	short Rotate(short nDegrees, unsigned long rgbBackColor);
	void RePaint();
	void ScaleImage(long nNewWidth, long nNewHeight);
	BOOL GetFilename();
	void PaintImage();
	void AboutBox();
};

#endif // __IMVB_H__
