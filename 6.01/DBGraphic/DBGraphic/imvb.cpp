// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "imvb.h"

// Dispatch interfaces referenced by this interface
#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CImvb

IMPLEMENT_DYNCREATE(CImvb, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CImvb properties

CString CImvb::GetPicture()
{
	CString result;
	GetProperty(0x17, VT_BSTR, (void*)&result);
	return result;
}

void CImvb::SetPicture(LPCTSTR propVal)
{
	SetProperty(0x17, VT_BSTR, propVal);
}

long CImvb::GetImageWidth()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetImageWidth(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long CImvb::GetImageHeight()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetImageHeight(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long CImvb::GetImageColors()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetImageColors(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long CImvb::GetScaleLeft()
{
	long result;
	GetProperty(0x18, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetScaleLeft(long propVal)
{
	SetProperty(0x18, VT_I4, propVal);
}

long CImvb::GetScaleTop()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetScaleTop(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}

long CImvb::GetScaleWidth()
{
	long result;
	GetProperty(0x1a, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetScaleWidth(long propVal)
{
	SetProperty(0x1a, VT_I4, propVal);
}

long CImvb::GetScaleHeight()
{
	long result;
	GetProperty(0x1b, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetScaleHeight(long propVal)
{
	SetProperty(0x1b, VT_I4, propVal);
}

BOOL CImvb::GetScrollbars()
{
	BOOL result;
	GetProperty(0x1c, VT_BOOL, (void*)&result);
	return result;
}

void CImvb::SetScrollbars(BOOL propVal)
{
	SetProperty(0x1c, VT_BOOL, propVal);
}

BOOL CImvb::GetSelect()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CImvb::SetSelect(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

short CImvb::GetMagnification()
{
	short result;
	GetProperty(0x1d, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetMagnification(short propVal)
{
	SetProperty(0x1d, VT_I2, propVal);
}

short CImvb::GetDstLeft()
{
	short result;
	GetProperty(0x1e, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetDstLeft(short propVal)
{
	SetProperty(0x1e, VT_I2, propVal);
}

short CImvb::GetDstTop()
{
	short result;
	GetProperty(0x1f, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetDstTop(short propVal)
{
	SetProperty(0x1f, VT_I2, propVal);
}

short CImvb::GetDstRight()
{
	short result;
	GetProperty(0x20, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetDstRight(short propVal)
{
	SetProperty(0x20, VT_I2, propVal);
}

short CImvb::GetDstBottom()
{
	short result;
	GetProperty(0x21, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetDstBottom(short propVal)
{
	SetProperty(0x21, VT_I2, propVal);
}

short CImvb::GetSrcLeft()
{
	short result;
	GetProperty(0x22, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetSrcLeft(short propVal)
{
	SetProperty(0x22, VT_I2, propVal);
}

short CImvb::GetSrcTop()
{
	short result;
	GetProperty(0x23, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetSrcTop(short propVal)
{
	SetProperty(0x23, VT_I2, propVal);
}

short CImvb::GetSrcRight()
{
	short result;
	GetProperty(0x24, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetSrcRight(short propVal)
{
	SetProperty(0x24, VT_I2, propVal);
}

short CImvb::GetSrcBottom()
{
	short result;
	GetProperty(0x25, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetSrcBottom(short propVal)
{
	SetProperty(0x25, VT_I2, propVal);
}

BOOL CImvb::GetAutoDraw()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CImvb::SetAutoDraw(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

short CImvb::GetImageXRes()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetImageXRes(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}

short CImvb::GetImageYRes()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetImageYRes(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}

short CImvb::GetExtensionCount()
{
	short result;
	GetProperty(0x26, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetExtensionCount(short propVal)
{
	SetProperty(0x26, VT_I2, propVal);
}

CString CImvb::GetSaveOptions()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}

void CImvb::SetSaveOptions(LPCTSTR propVal)
{
	SetProperty(0x8, VT_BSTR, propVal);
}

short CImvb::GetPaletteEntries()
{
	short result;
	GetProperty(0x27, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetPaletteEntries(short propVal)
{
	SetProperty(0x27, VT_I2, propVal);
}

short CImvb::GetPages()
{
	short result;
	GetProperty(0x28, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetPages(short propVal)
{
	SetProperty(0x28, VT_I2, propVal);
}

short CImvb::GetPageNumber()
{
	short result;
	GetProperty(0x29, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetPageNumber(short propVal)
{
	SetProperty(0x29, VT_I2, propVal);
}

long CImvb::GetDitherMethod()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetDitherMethod(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long CImvb::GetAutoScale()
{
	long result;
	GetProperty(0x2a, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetAutoScale(long propVal)
{
	SetProperty(0x2a, VT_I4, propVal);
}

long CImvb::GetImageFlags()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetImageFlags(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

BOOL CImvb::GetUseDDB()
{
	BOOL result;
	GetProperty(0x2b, VT_BOOL, (void*)&result);
	return result;
}

void CImvb::SetUseDDB(BOOL propVal)
{
	SetProperty(0x2b, VT_BOOL, propVal);
}

long CImvb::GetCompression()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetCompression(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long CImvb::GetOverwrite()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetOverwrite(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

short CImvb::GetQuality()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetQuality(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}

CPicture CImvb::GetVBPicture()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2c, VT_DISPATCH, (void*)&pDispatch);
	return CPicture(pDispatch);
}

void CImvb::SetVBPicture(LPDISPATCH propVal)
{
	SetProperty(0x2c, VT_DISPATCH, propVal);
}

long CImvb::GetEmbedLength()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetEmbedLength(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long CImvb::GetEmbedOffset()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetEmbedOffset(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

OLE_COLOR CImvb::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

BOOL CImvb::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CImvb::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

short CImvb::GetBorderStyle()
{
	short result;
	GetProperty(DISPID_BORDERSTYLE, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetBorderStyle(short propVal)
{
	SetProperty(DISPID_BORDERSTYLE, VT_I2, propVal);
}

OLE_HANDLE CImvb::GetHWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I2, (void*)&result);
	return result;
}

void CImvb::SetHWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I2, propVal);
}

CString CImvb::GetExt()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void CImvb::SetExt(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

long CImvb::GetScaleMethod()
{
	long result;
	GetProperty(0x11, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetScaleMethod(long propVal)
{
	SetProperty(0x11, VT_I4, propVal);
}

long CImvb::GetDisplayColors()
{
	long result;
	GetProperty(0x2d, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetDisplayColors(long propVal)
{
	SetProperty(0x2d, VT_I4, propVal);
}

BOOL CImvb::GetAppendImage()
{
	BOOL result;
	GetProperty(0x12, VT_BOOL, (void*)&result);
	return result;
}

void CImvb::SetAppendImage(BOOL propVal)
{
	SetProperty(0x12, VT_BOOL, propVal);
}

BOOL CImvb::GetStatusEnabled()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}

void CImvb::SetStatusEnabled(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}

long CImvb::GetMousePointer()
{
	long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetMousePointer(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}

CPicture CImvb::GetMouseIcon()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2e, VT_DISPATCH, (void*)&pDispatch);
	return CPicture(pDispatch);
}

void CImvb::SetMouseIcon(LPDISPATCH propVal)
{
	SetProperty(0x2e, VT_DISPATCH, propVal);
}

long CImvb::GetHDC()
{
	long result;
	GetProperty(0x2f, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetHDC(long propVal)
{
	SetProperty(0x2f, VT_I4, propVal);
}

long CImvb::GetHDIB()
{
	long result;
	GetProperty(0x30, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetHDIB(long propVal)
{
	SetProperty(0x30, VT_I4, propVal);
}

long CImvb::GetHImage()
{
	long result;
	GetProperty(0x31, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetHImage(long propVal)
{
	SetProperty(0x31, VT_I4, propVal);
}

long CImvb::GetHPalette()
{
	long result;
	GetProperty(0x32, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetHPalette(long propVal)
{
	SetProperty(0x32, VT_I4, propVal);
}

long CImvb::GetPrnHdc()
{
	long result;
	GetProperty(0x33, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetPrnHdc(long propVal)
{
	SetProperty(0x33, VT_I4, propVal);
}

CString CImvb::GetMemoryFormat()
{
	CString result;
	GetProperty(0x13, VT_BSTR, (void*)&result);
	return result;
}

void CImvb::SetMemoryFormat(LPCTSTR propVal)
{
	SetProperty(0x13, VT_BSTR, propVal);
}

long CImvb::GetImageDataSize()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetImageDataSize(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long CImvb::GetHImageData()
{
	long result;
	GetProperty(0x34, VT_I4, (void*)&result);
	return result;
}

void CImvb::SetHImageData(long propVal)
{
	SetProperty(0x34, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CImvb operations

CString CImvb::GetExtensions(short index)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		index);
	return result;
}

void CImvb::SetExtensions(short index, LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 index, lpszNewValue);
}

short CImvb::GetRed(short index)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms,
		index);
	return result;
}

void CImvb::SetRed(short index, short nNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 index, nNewValue);
}

short CImvb::GetGreen(short index)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms,
		index);
	return result;
}

void CImvb::SetGreen(short index, short nNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 index, nNewValue);
}

short CImvb::GetBlue(short index)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms,
		index);
	return result;
}

void CImvb::SetBlue(short index, short nNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 index, nNewValue);
}

void CImvb::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::Brightness(short Value)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

void CImvb::PrintImage()
{
	InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::IncreaseTo(short nColors)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nColors);
}

void CImvb::Invert()
{
	InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::ReduceTo(short nColors, BOOL bGreyScale, BOOL bOptimizedPalette)
{
	static BYTE parms[] =
		VTS_I2 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nColors, bGreyScale, bOptimizedPalette);
}

void CImvb::MirrorVertical()
{
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::MirrorHorizontal()
{
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::ImageCut()
{
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::ImageCopy()
{
	InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::ImagePaste()
{
	InvokeHelper(0x3e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString CImvb::SaveAs(LPCTSTR SaveFileName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3f, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		SaveFileName);
	return result;
}

void CImvb::Gamma(float Gamma)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Gamma);
}

short CImvb::Rotate(short nDegrees, unsigned long rgbBackColor)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x41, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nDegrees, rgbBackColor);
	return result;
}

void CImvb::RePaint()
{
	InvokeHelper(0x42, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::ScaleImage(long nNewWidth, long nNewHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x43, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nNewWidth, nNewHeight);
}

BOOL CImvb::GetFilename()
{
	BOOL result;
	InvokeHelper(0x44, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CImvb::PaintImage()
{
	InvokeHelper(0x45, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CImvb::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
