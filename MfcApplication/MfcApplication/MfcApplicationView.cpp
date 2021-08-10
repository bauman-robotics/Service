
// MfcApplicationView.cpp : implementation of the CMfcApplicationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MfcApplication.h"
#endif

#include "MfcApplicationDoc.h"
#include "MfcApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMfcApplicationView

IMPLEMENT_DYNCREATE(CMfcApplicationView, CView)

BEGIN_MESSAGE_MAP(CMfcApplicationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMfcApplicationView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMfcApplicationView construction/destruction

CMfcApplicationView::CMfcApplicationView()
{
	// TODO: add construction code here

}

CMfcApplicationView::~CMfcApplicationView()
{
}

BOOL CMfcApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMfcApplicationView drawing

void CMfcApplicationView::OnDraw(CDC* /*pDC*/)
{
	CMfcApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMfcApplicationView printing


void CMfcApplicationView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMfcApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMfcApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMfcApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMfcApplicationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMfcApplicationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMfcApplicationView diagnostics

#ifdef _DEBUG
void CMfcApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcApplicationDoc* CMfcApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcApplicationDoc)));
	return (CMfcApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CMfcApplicationView message handlers
