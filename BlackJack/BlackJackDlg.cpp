
// BlackJackDlg.cpp: 實作檔案
//

#include "pch.h"
#include "framework.h"
#include "BlackJack.h"
#include "BlackJackDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BANKER_POINT_POS Point (800, 60)
#define PLAYER_POINT_POS Point (800, 400)
#define PLAYER_CARD1_LT Point2f (144, 223)
#define PLAYER_CARD1_RT Point2f (211, 254)
#define PLAYER_CARD1_LB Point2f (102, 313)
#define PLAYER_CARD1_RB Point2f (169, 344)
// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBlackJackDlg 對話方塊



CBlackJackDlg::CBlackJackDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BlackJack_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	clrString = Scalar(255,255,255);
	m_iBankerPntDeal = 0;
	m_iBankerPnt = 0;
	m_iPlayerPnt = 0;
	m_iBankerPntA = 0;
	m_iPlayerPntA = 0;
	m_iBankerStatus = 0;
	m_iPlayerStatus = 0;
	m_iGameStatus = 0;
	m_iCardCount[CARD_NUMBER] = { 0 };

	m_bBankerBJ = false;
	m_bPlayerBJ = false;
	m_iBankerA = 0;
	m_iPlayerA = 0;

	vecBankerCard = {};
	vecPlayerCard = {};

	m_strBankerPoint = "";
	m_strPlayerPoint = "";
	m_strPlayerPointA = "";

	m_iChips = 5988;
	m_iStakes = 100;
	clrRed = Scalar (0, 0, 255);
}

void CBlackJackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange (pDX);
	DDX_Control (pDX, IDC_BUTTON_DEAL, m_btnDeal);
	DDX_Control (pDX, IDC_BUTTON_HIT, m_btnHit);
	DDX_Control (pDX, IDC_BUTTON_STAND, m_btnStand);
	DDX_Control (pDX, IDC_BUTTON_100, m_btn100);
	DDX_Control (pDX, IDC_BUTTON_1000, m_btn1000);
	DDX_Control (pDX, IDC_BUTTON_5000, m_btn5000);
	DDX_Control (pDX, IDC_BUTTON_DOUBLE, m_btnDouble);
	DDX_Control (pDX, IDC_BUTTON_SHOW_STATUS, m_btnShowStatus);
}

BEGIN_MESSAGE_MAP(CBlackJackDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED (IDC_BUTTON_DEAL, &CBlackJackDlg::OnBnClickedButtonDeal)
	ON_BN_CLICKED (IDC_BUTTON_HIT, &CBlackJackDlg::OnBnClickedButtonHit)
	ON_BN_CLICKED (IDC_BUTTON_STAND, &CBlackJackDlg::OnBnClickedButtonStand)
	ON_BN_CLICKED (IDC_BUTTON_100, &CBlackJackDlg::OnBnClickedButton100)
	ON_WM_ERASEBKGND ()
	ON_BN_CLICKED (IDC_BUTTON_1000, &CBlackJackDlg::OnBnClickedButton1000)
	ON_BN_CLICKED (IDC_BUTTON_5000, &CBlackJackDlg::OnBnClickedButton5000)
	ON_BN_CLICKED (IDC_BUTTON_DOUBLE, &CBlackJackDlg::OnBnClickedButtonDouble)
END_MESSAGE_MAP()


// CBlackJackDlg 訊息處理常式

BOOL CBlackJackDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定

	CString strTitle = L"Black Jack";
	SetWindowText (strTitle);

	//讀取牌桌圖檔
	m_matTableTop = imread ("res\\table top.jpg");

	//複製一份牌桌的原圖，避免原牌桌圖遭到修改，以便restart時重新讀取
	m_matShow = m_matTableTop.clone ();
	imshow ("Black Jack", m_matShow);

	//將牌桌圖形背景貼在具柄上
	namedWindow ("Black Jack");
	HWND hWnd = (HWND)cvGetWindowHandle ("Black Jack");//get opencv window's handle
	HWND hParent = ::GetParent (hWnd);
	::ShowWindow (hParent, SW_HIDE);
	::SetParent (hWnd, m_hWnd);


	//讀取52張牌
		for (size_t i = 0; i < CARD_NUMBER; i++)
		m_matCard[i] = imread (format("res\\%d.bmp",i+1));

	//讀取底牌
	m_matHoleCard = imread ("res\\53.bmp");

	//將牌進行縮放
	for (size_t i = 0; i < CARD_NUMBER; i++)
		resize (m_matCard[i], m_matCard[i], m_matTableTop.size ());
	resize (m_matHoleCard, m_matHoleCard, m_matTableTop.size ());

	//設定透視投影點
	SetPerspectivePoint ();
	//籌碼
	m_btn100.SetBitmaps (IDB_BITMAP_100, RGB (0, 128, 0));
	m_btn100.DrawBorder (FALSE);
	m_btn1000.SetBitmaps (IDB_BITMAP_1000, RGB (0, 128, 0));
	m_btn1000.DrawBorder (FALSE);
	m_btn5000.SetBitmaps (IDB_BITMAP_5000, RGB (0, 128, 0));
	m_btn5000.DrawBorder (FALSE);
	CRect rect;
	m_btn100.GetWindowRect (rect);
	ScreenToClient (rect);
	m_btn100.MoveWindow (rect.left, rect.top, 102, 102);

	m_btn1000.GetWindowRect (rect);
	ScreenToClient (rect);
	m_btn1000.MoveWindow (rect.left, rect.top, 102, 102);

	m_btn5000.GetWindowRect (rect);
	ScreenToClient (rect);
	m_btn5000.MoveWindow (rect.left, rect.top, 102, 102);
	//籌碼與賭金顯示
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->MoveWindow (0, TABLE_HEIGHT, TABLE_WIDTH, rect.Height ());
	//
	m_btnDeal.DrawBorder (FALSE);
	m_btnHit.DrawBorder (FALSE);
	m_btnStand.DrawBorder (FALSE);
	m_btnDouble.DrawBorder (FALSE);

	EnableFourButtons (TRUE, FALSE, FALSE, TRUE);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CBlackJackDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CBlackJackDlg::OnPaint()
{
	CPaintDC dc (this); // 繪製的裝置內容
	if (IsIconic())
	{
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rectDraw;
		GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rectDraw);
		ScreenToClient (rectDraw);

		CDC drawDC;//暫存圖形的DC ; 顯示設備對象
		CBitmap bitmapTemp, *pOldBitmap;
		bitmapTemp.CreateCompatibleBitmap (&dc, rectDraw.Width (), rectDraw.Height ());// 建立螢幕顯示兼容的點陣圖
		drawDC.CreateCompatibleDC (NULL);//繪圖與顯示設備之間的緩衝
		pOldBitmap = drawDC.SelectObject (&bitmapTemp);//將點陣圖選入內存顯示設備中(選入點陣圖的顯示設備才有地方繪圖)

		DrawNumber (&drawDC);

		dc.BitBlt(rectDraw.left, rectDraw.top, rectDraw.Width(), rectDraw.Height(), &drawDC, 0, 0, SRCCOPY);//將內存的圖複製到螢幕上顯示
		
		bitmapTemp.DeleteObject ();
		drawDC.DeleteDC ();

		
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CBlackJackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBlackJackDlg::OnBnClickedButtonDeal ()
{
	EnableFourButtons (0, 0, 0, 0);
	srand ((unsigned int)time (NULL));
	//莊家抽第一張牌
	Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
	//莊家抽第二張牌
	Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
	m_iBankerStatus = TWO_CARD;

	//玩家抽第一張牌
	Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
	//玩家抽第二張牌
	Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
	m_iPlayerStatus = TWO_CARD;

	//莊家Black Jack
	if (vecBankerCard[0] <= 3 && (vecBankerCard[1] >= 36 && vecBankerCard[1] <= 51) || (vecBankerCard[0] >= 36 && vecBankerCard[0] <= 51) && vecBankerCard[1] <= 3)
		m_bBankerBJ = true;

	//玩家BlackJack
	if (vecPlayerCard[0] <= 3 && (vecPlayerCard[1] >= 36 && vecPlayerCard[1] <= 51) || (vecPlayerCard[0] >= 36 && vecPlayerCard[0] <= 51) && vecPlayerCard[1] <= 3)
		m_bPlayerBJ = true;

	//莊家BlackJack 玩家沒有
	if (m_bBankerBJ && !m_bPlayerBJ) 
	{
		WarpPerspective (m_matCard[vecBankerCard[0]], m_matShow, m_matBankerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecBankerCard[1]], m_matShow, m_matBankerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		m_strBankerPoint = format ("%d", m_iBankerPntA);
		putText (m_matShow, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);

		WarpPerspective (m_matCard[vecPlayerCard[0]], m_matShow, m_matPlayerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecPlayerCard[1]], m_matShow, m_matPlayerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);

		if (m_iPlayerA != 0)
			m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
		else
			m_strPlayerPoint = format ("%d", m_iPlayerPnt);

		putText (m_matShow, m_strPlayerPoint, Point (800, 400), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		imshow ("Black Jack", m_matShow);

		m_iGameStatus = BANKER_WIN;
		ShowStatus ();
		//EnableFourButtons (FALSE, FALSE, FALSE, FALSE);
		//MessageBox (_T ("莊家Black Jack，你輸了!!!"), _T ("Black Jack"));
	}
	//玩家Black Jack 莊家沒有
	else if (!m_bBankerBJ  && m_bPlayerBJ) 
	{
		WarpPerspective (m_matCard[vecBankerCard[0]], m_matShow, m_matBankerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecBankerCard[1]], m_matShow, m_matBankerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		m_strBankerPoint = format ("%d", m_iBankerPntA);
		putText (m_matShow, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);

		WarpPerspective (m_matCard[vecPlayerCard[0]], m_matShow, m_matPlayerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecPlayerCard[1]], m_matShow, m_matPlayerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		m_strPlayerPoint = format ("%d", m_iPlayerPntA);
		putText (m_matShow, m_strPlayerPoint, Point (800, 400), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);

		imshow ("Black Jack", m_matShow);
		m_iGameStatus = PLAYER_WIN;
		ShowStatus ();
		//EnableFourButtons (FALSE, FALSE, FALSE, FALSE);
		//MessageBox (_T ("玩家Black Jack，你贏了!!!"), _T ("Black Jack"));
	}
	//兩邊都是Black Jack
	else if (m_bBankerBJ  && m_bPlayerBJ) 
	{
		WarpPerspective (m_matCard[vecBankerCard[0]], m_matShow, m_matBankerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecBankerCard[1]], m_matShow, m_matBankerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		m_strBankerPoint = format ("%d", m_iBankerPntA);
		putText (m_matShow, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);

		WarpPerspective (m_matCard[vecPlayerCard[0]], m_matShow, m_matPlayerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecPlayerCard[1]], m_matShow, m_matPlayerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		m_strPlayerPoint = format ("%d", m_iPlayerPntA);
		putText (m_matShow, m_strPlayerPoint, Point (800, 400), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		imshow ("Black Jack", m_matShow);
		m_iGameStatus = TIE;
		EnableFourButtons (FALSE, FALSE, FALSE, FALSE);
		ShowStatus ();
		//MessageBox (_T ("玩家&莊家Black Jack，平手!!!"), _T ("Black Jack"));
	}
	//沒有人BlackJack，所以印出莊家第二張牌，第一張牌不顯示
	else 
	{
		//莊家第一張牌為底牌不顯示
		WarpPerspective (m_matHoleCard, m_matShow, m_matBankerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		//顯示莊家第二張牌
		WarpPerspective (m_matCard[vecBankerCard[1]], m_matShow, m_matBankerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		
		if (vecBankerCard[1] >= 36 && vecBankerCard[1] <= 51)
			m_iBankerPntDeal += 10;
		else if (vecBankerCard[1] <= 3)
			m_iBankerPntDeal += 11;
		else if (vecBankerCard[1] >= 4 && vecBankerCard[1] <= 7)
			m_iBankerPntDeal += 2;
		else if (vecBankerCard[1] >= 8 && vecBankerCard[1] <= 11)
			m_iBankerPntDeal += 3;
		else if (vecBankerCard[1] >= 12 && vecBankerCard[1] <= 15)
			m_iBankerPntDeal += 4;
		else if (vecBankerCard[1] >= 16 && vecBankerCard[1] <= 19)
			m_iBankerPntDeal += 5;
		else if (vecBankerCard[1] >= 20 && vecBankerCard[1] <= 23)
			m_iBankerPntDeal += 6;
		else if (vecBankerCard[1] >= 24 && vecBankerCard[1] <= 27)
			m_iBankerPntDeal += 7;
		else if (vecBankerCard[1] >= 28 && vecBankerCard[1] <= 31)
			m_iBankerPntDeal += 8;
		else if (vecBankerCard[1] >= 32 && vecBankerCard[1] <= 35)
			m_iBankerPntDeal += 9;

		m_iBankerStatus = TWO_CARD;

		//顯示玩家的兩張牌
		WarpPerspective (m_matCard[vecPlayerCard[0]], m_matShow, m_matPlayerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecPlayerCard[1]], m_matShow, m_matPlayerPerspectiveTwo, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		
		//儲存發牌牌桌圖形(沒顯示兩者點數)
		m_matPlayerHitPntNoBankerPnt = m_matShow.clone ();
		Mat matTemp = m_matShow.clone ();
		
		m_strBankerPoint = format ("%d", m_iBankerPntDeal);
		putText (matTemp, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);

		//儲存發牌牌桌圖形(顯示莊家點數,無玩家點數)
		m_matBankerDealPnt = matTemp.clone ();

		if (m_iPlayerA != 0)
			m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
		else
			m_strPlayerPoint = format ("%d", m_iPlayerPnt);

		//玩家點數顯示在matTemp上
		putText (matTemp, m_strPlayerPoint, Point (800, 400), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		imshow ("Black Jack", matTemp);
		
		EnableFourButtons (FALSE, 1, 1, 1);
	}
}

void CBlackJackDlg::OnBnClickedButtonHit ()
{
	//EnableFourButtons (0, 0, 0, 0);
	srand ((unsigned int)time (NULL));
	//抽第三張牌
	if (m_iPlayerStatus == TWO_CARD)
	{
		//使用"顯示莊家點數,無玩家點數"牌桌圖形
		m_matShow = m_matBankerDealPnt.clone ();

		Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
		WarpPerspective (m_matCard[vecPlayerCard[2]], m_matShow, m_matPlayerPerspectiveThree, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		
		//儲存玩家抽第三張牌時牌桌圖形(顯示莊家點數,無玩家點數)
		Mat matTemp = m_matShow.clone ();
		m_matPlayerHitPnt = m_matShow.clone ();

		if (m_iPlayerPntA <= 21 && m_iPlayerA != 0)
			m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
		else
			m_strPlayerPoint = format ("%d", m_iPlayerPnt);

		//玩家點數顯示在matTemp上
		putText (matTemp, m_strPlayerPoint, Point (800, 400), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		
		imshow ("Black Jack", matTemp);
		m_iPlayerStatus = THREE_CARD;
	}
	//抽第四張牌
	else if (m_iPlayerStatus == THREE_CARD)
	{
		//使用"顯示莊家點數,無玩家點數"牌桌圖形
		m_matShow = m_matPlayerHitPnt.clone ();
		Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
		WarpPerspective (m_matCard[vecPlayerCard[3]], m_matShow, m_matPlayerPerspectiveFour, m_matTableTop.size (), 1, BORDER_TRANSPARENT);

		//儲存玩家抽第四張牌時牌桌圖形(顯示莊家點數,無玩家點數)
		Mat matTemp = m_matShow.clone ();
		m_matPlayerHitPnt = m_matShow.clone ();
		if (m_iPlayerPntA <= 21 && m_iPlayerA != 0)
			m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
		else
			m_strPlayerPoint = format ("%d", m_iPlayerPnt);

		//玩家點數顯示在matTemp上
		putText (matTemp, m_strPlayerPoint, Point (800, 400), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		imshow ("Black Jack", matTemp);
		m_iPlayerStatus = FOUR_CARD;
	}
	//抽第五張牌
	else if (m_iPlayerStatus == FOUR_CARD)
	{
		//使用"顯示莊家點數,無玩家點數"牌桌圖形
		m_matShow = m_matPlayerHitPnt.clone ();
		Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
		WarpPerspective (m_matCard[vecPlayerCard[4]], m_matShow, m_matPlayerPerspectiveFive, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		imshow ("Black Jack", m_matShow);

		//儲存玩家抽第五張牌時牌桌圖形(顯示莊家點數,無玩家點數)
		Mat matTemp = m_matShow.clone ();
		m_matPlayerHitPnt = m_matShow.clone ();
		if (m_iPlayerPntA <= 21 && m_iPlayerA != 0)
			m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
		else
			m_strPlayerPoint = format ("%d", m_iPlayerPnt);

		//玩家點數顯示在matTemp上
		putText (matTemp, m_strPlayerPoint, Point (800, 400), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		imshow ("Black Jack", matTemp);
		m_iPlayerStatus = FIVE_CARD;
	}

	if (m_iPlayerPnt > 21)
	{
		//MessageBox (_T ("玩家爆掉，你輸了!!!"), _T ("Black Jack"));
		m_iGameStatus = BANKER_WIN;
		ShowStatus ();
		EnableFourButtons (1, 0, 0, 1);
	}
	else if (m_iPlayerStatus == FIVE_CARD)
	{
		//MessageBox (_T ("過五關，你贏了!!!"), _T ("Black Jack"));
		m_iGameStatus = PLAYER_WIN_5CARDS;
		ShowStatus ();
		EnableFourButtons (1, 0, 0, 1);
	}
	else if (m_iPlayerPnt == 21)
		//玩家21點莊家自動抽牌
		OnBnClickedButtonStand ();
}


void CBlackJackDlg::OnBnClickedButtonStand ()
{
	EnableFourButtons (0, 0, 0, 0);
	//使用"雙方各發兩張牌,且無顯示兩者點數"牌桌圖形
	m_matShow = m_matPlayerHitPntNoBankerPnt.clone ();
	Mat matTmp;

	//從m_iPlayerStatus判斷並補足顯示牌數
	if (m_iPlayerStatus == THREE_CARD)
		WarpPerspective (m_matCard[vecPlayerCard[2]], m_matShow, m_matPlayerPerspectiveThree, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
	else if (m_iPlayerStatus == FOUR_CARD)
	{
		WarpPerspective (m_matCard[vecPlayerCard[2]], m_matShow, m_matPlayerPerspectiveThree, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecPlayerCard[3]], m_matShow, m_matPlayerPerspectiveFour, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
	}
	else if (m_iPlayerStatus == FIVE_CARD)
	{
		WarpPerspective (m_matCard[vecPlayerCard[2]], m_matShow, m_matPlayerPerspectiveThree, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecPlayerCard[3]], m_matShow, m_matPlayerPerspectiveFour, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		WarpPerspective (m_matCard[vecPlayerCard[4]], m_matShow, m_matPlayerPerspectiveFive, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
	}
	//顯示玩家點數
	if (m_iPlayerPntA <= 21 && m_iPlayerA != 0)
		m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
	else
		m_strPlayerPoint = format ("%d", m_iPlayerPnt);
	ShowPoint (m_matShow.clone ());


	if (m_iGameStatus == 0)
	{
		//印出蓋住的第一張牌
		WarpPerspective (m_matCard[vecBankerCard[0]], m_matShow, m_matBankerPerspectiveOne, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		if (m_iBankerA != 0)
			m_strBankerPoint = format ("%d", m_iBankerPntA);
		else
			m_strBankerPoint = format ("%d", m_iBankerPnt);

		ShowPoint (m_matShow.clone ());
		if (m_iBankerStatus == TWO_CARD)
		{	
			//未達17點則繼續抽牌
			if (m_iBankerPntA < 17 || (m_iBankerPnt < 17 && m_iBankerPntA>21))
				m_iBankerStatus = THREE_CARD;
			//達17點則顯示莊家點數並判斷勝負
			else
			{
				
				Judge (m_iBankerPnt, m_iPlayerPnt, m_iBankerPntA, m_iPlayerPntA);
			}
			//putText (m_matShow, m_strBankerPoint, BANKER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
			//imshow ("Black Jack", m_matShow);
		}
		if (m_iBankerStatus == THREE_CARD)
		{
			//莊家抽第三張牌
			Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
			//顯示莊家第三張牌
			WarpPerspective (m_matCard[vecBankerCard[2]], m_matShow, m_matBankerPerspectiveThree, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
			if (m_iBankerA != 0 && m_iBankerPntA <= 21)
				m_strBankerPoint = format ("%d", m_iBankerPntA);
			else
				m_strBankerPoint = format ("%d", m_iBankerPnt);
			ShowPoint (m_matShow.clone ());
			//未達17點則繼續抽牌
			if (m_iBankerPntA < 17 || (m_iBankerPnt < 17 && m_iBankerPntA > 21))
				m_iBankerStatus = FOUR_CARD;
			//超過21點爆掉玩家勝利
			else if (m_iBankerPnt > 21)
			{
				//putText (m_matShow, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
				//imshow ("Black Jack", m_matShow);
				//MessageBox (_T ("莊家爆掉，你贏了!!!"), _T ("Black Jack"));
				m_iGameStatus = PLAYER_WIN;
				ShowStatus ();
			}
			//莊家17~21點則顯示莊家點數並判斷勝負
			else
			{
				//putText (m_matShow, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
				//imshow ("Black Jack", m_matShow);
				Judge (m_iBankerPnt, m_iPlayerPnt, m_iBankerPntA, m_iPlayerPntA);
			}
			ShowPoint (m_matShow.clone ());
		}

		if (m_iBankerStatus == FOUR_CARD)
		{
			//莊家抽第四張牌
			Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
			//顯示莊家第四張牌
			WarpPerspective (m_matCard[vecBankerCard[3]], m_matShow, m_matBankerPerspectiveFour, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
			if (m_iBankerA != 0 && m_iBankerPntA <= 21)
				m_strBankerPoint = format ("%d", m_iBankerPntA);
			else
				m_strBankerPoint = format ("%d", m_iBankerPnt);
			ShowPoint (m_matShow.clone ());
			//未達17點則繼續抽牌
			if (m_iBankerPntA < 17 || (m_iBankerPnt < 17 && m_iBankerPntA > 21))
				m_iBankerStatus =FIVE_CARD;
			//超過21點爆掉玩家勝利
			else if (m_iBankerPnt > 21)
			{
				//putText (m_matShow, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
				//imshow ("Black Jack", m_matShow);
				//MessageBox (_T ("莊家爆掉，你贏了!!!"), _T ("Black Jack"));
				m_iGameStatus = PLAYER_WIN;
				ShowStatus ();
			}
			//莊家17~21點則顯示莊家點數並判斷勝負
			else
			{
				//putText (m_matShow, m_strBankerPoint, Point (800, 60), FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
				//imshow ("Black Jack", m_matShow);
				Judge (m_iBankerPnt, m_iPlayerPnt, m_iBankerPntA, m_iPlayerPntA);
			}
				
		}
		if (m_iBankerStatus == FIVE_CARD)
		{
			//莊家抽第五張牌
			Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
			//顯示莊家第五張牌
			WarpPerspective (m_matCard[vecBankerCard[4]], m_matShow, m_matBankerPerspectiveFive, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
			if (m_iBankerA != 0 && m_iBankerPntA <= 21)
				m_strBankerPoint = format ("%d", m_iBankerPntA);
			else
				m_strBankerPoint = format ("%d", m_iBankerPnt);

			ShowPoint (m_matShow.clone ());
			//超過21點爆掉玩家勝利
			if (m_iBankerPnt > 21)
			{
				//MessageBox (_T ("莊家爆掉，你贏了!!!"), _T ("Black Jack"));
				m_iGameStatus = PLAYER_WIN;
				ShowStatus ();
			}
			//莊家未達21點則顯示莊家點數並判斷勝負
			else
				Judge (m_iBankerPnt, m_iPlayerPnt, m_iBankerPntA, m_iPlayerPntA);
		}
	}
	
}

void CBlackJackDlg::WaitTime (int iTime)
{
	MSG msg;

	clock_t clk1 = clock ();
	clock_t clk2 = 0;
	while (1)
	{
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}

		clk2 = clock ();
		if (clk2 - clk1 >= iTime)
			break;
	}
}

void CBlackJackDlg::WarpPerspective (Mat matSrc, Mat matDst, Mat matTransform, Size size, int iFlags, int iBorder)
{
	warpPerspective (matSrc, matDst, matTransform, size, iFlags, iBorder);
	cvWaitKey (700);
	imshow ("Black Jack", matDst);
}
 
void CBlackJackDlg::DrawNumber (CDC* pDC)
{
	CRect rect, rectChips, rectStakes;
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	ScreenToClient (rect);
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject (BLACK_BRUSH);

	rectChips = CRect (0, 0, rect.Width () / 2, rect.Height ());
	rectStakes = CRect (0 + rect.Width () / 2, 0, rect.Width (), rect.Height ());
	CString strNumber;
	strNumber.Format (L"$%03d,%03d,%03d", m_iChips / 1000000, m_iChips / 1000 - (m_iChips / 1000000) * 1000000, m_iChips - (m_iChips / 1000) * 1000);

	pDC->SetBkColor (RGB (0, 0, 0));
	//
	CFont font;
	font.CreatePointFont (400/*nPointSize*/, L"Arial", pDC);
	HFONT pOldFont = (HFONT)pDC->SelectObject (HFONT (font));
	//local_font.h
	pDC->SelectObject (HFONT (font));

	

	pDC->SetTextColor (RGB (255, 215, 0));
	UINT format = DT_LEFT;
	pDC->DrawText (strNumber, rectChips, format);

	strNumber.Format (L"Bets:%03d,%03d,%03d", m_iStakes / 1000000, m_iStakes / 1000 - m_iStakes / 1000000 * 1000000, m_iStakes - m_iStakes / 1000 * 1000);
	pDC->DrawText (strNumber, rectStakes, format);


	pDC->SelectObject (font);
	pDC->SelectObject (oldBrush);
	pDC->SetBkColor (GetSysColor (COLOR_WINDOW));
	//
}
void CBlackJackDlg::SetPerspectivePoint ()
{
	//設定待投影牌桌座標
	m_ptsTableTop[0] = Point (0, 0);//LT
	m_ptsTableTop[1] = Point (m_matTableTop.cols - 1, 0);//RT
	m_ptsTableTop[2] = Point (0, m_matTableTop.rows - 1);//LB
	m_ptsTableTop[3] = Point (m_matTableTop.cols - 1, m_matTableTop.rows - 1);//RB

	//莊家第一張牌投影位置
	m_ptsBankerOne[0] = Point (262, 25);//LT
	m_ptsBankerOne[1] = Point (333, 25);//RT
	m_ptsBankerOne[2] = Point (262, 121);//LB
	m_ptsBankerOne[3] = Point (333, 121);//RB
	m_matBankerPerspectiveOne = getPerspectiveTransform (m_ptsTableTop, m_ptsBankerOne);

	//莊家第二張牌投影位置
	m_ptsBankerTwo[0] = Point (362, 25);//LT
	m_ptsBankerTwo[1] = Point (433, 25);//RT
	m_ptsBankerTwo[2] = Point (362, 121);//LB
	m_ptsBankerTwo[3] = Point (433, 121);//RB
	m_matBankerPerspectiveTwo = getPerspectiveTransform (m_ptsTableTop, m_ptsBankerTwo);

	//莊家第三張牌投影位置
	m_ptsBankerThree[0] = Point (462, 25);//LT
	m_ptsBankerThree[1] = Point (533, 25);//RT
	m_ptsBankerThree[2] = Point (462, 121);//LB
	m_ptsBankerThree[3] = Point (533, 121);//RB
	m_matBankerPerspectiveThree = getPerspectiveTransform (m_ptsTableTop, m_ptsBankerThree);

	//莊家第四張牌投影位置
	m_ptsBankerFour[0] = Point (562, 25);//LT
	m_ptsBankerFour[1] = Point (633, 25);//RT
	m_ptsBankerFour[2] = Point (562, 121);//LB
	m_ptsBankerFour[3] = Point (633, 121);//RB
	m_matBankerPerspectiveFour = getPerspectiveTransform (m_ptsTableTop, m_ptsBankerFour);

	//莊家第五張牌投影位置
	m_ptsBankerFive[0] = Point (662, 25);//LT
	m_ptsBankerFive[1] = Point (733, 25);//RT
	m_ptsBankerFive[2] = Point (662, 121);//LB
	m_ptsBankerFive[3] = Point (733, 121);//RB
	m_matBankerPerspectiveFive = getPerspectiveTransform (m_ptsTableTop, m_ptsBankerFive);

	//玩家第一張牌投影位置
	m_ptsPlayerOne[0] = PLAYER_CARD1_LT;//LT
	m_ptsPlayerOne[1] = PLAYER_CARD1_RT;//RT
	m_ptsPlayerOne[2] = PLAYER_CARD1_LB;//LB
	m_ptsPlayerOne[3] = PLAYER_CARD1_RB;//RB
	m_matPlayerPerspectiveOne = getPerspectiveTransform (m_ptsTableTop, m_ptsPlayerOne);

	//DDD
	//Point2f vecLT2RT = PLAYER_CARD1_RT - PLAYER_CARD1_LT;
	//Point2f vecLB2RB = PLAYER_CARD1_RB - PLAYER_CARD1_LB;
	//vector<Point2f> v; 
	//v.push_back (m_ptsPlayerOne[0]);
	//v.push_back (m_ptsPlayerOne[1]);
	//v.push_back (m_ptsPlayerOne[2]);
	//v.push_back (m_ptsPlayerOne[3]);
	//Rect rect = boundingRect (v);
	//for (int i = 1; i < 20; i++)
	//{
	//	Mat matShow = m_matShow.clone ();
	//	m_ptsPlayerOne[0] = PLAYER_CARD1_LT + vecLT2RT / 40 * i;//LT
	//	m_ptsPlayerOne[1] = PLAYER_CARD1_RT - vecLT2RT / 40 * i;//RT
	//	m_ptsPlayerOne[2] = PLAYER_CARD1_LB + vecLT2RT / 40 * i;//LB
	//	m_ptsPlayerOne[3] = PLAYER_CARD1_RB - vecLT2RT / 40 * i;//RB
	//	Mat matAction = getPerspectiveTransform (m_ptsTableTop, m_ptsPlayerOne);
	//	warpPerspective (m_matHoleCard, matShow, matAction, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
	//	string s = format ("C:\\users\\user\\Downloads\\%d.jpg", i);
	//	imwrite (s, matShow (rect));
	//}
	//DDD

	//玩家第二張牌投影位置
	m_ptsPlayerTwo[0] = Point (297, 283);//LT
	m_ptsPlayerTwo[1] = Point (371, 299);//RT
	m_ptsPlayerTwo[2] = Point (276, 380);//LB
	m_ptsPlayerTwo[3] = Point (349, 397);//RB
	m_matPlayerPerspectiveTwo = getPerspectiveTransform (m_ptsTableTop, m_ptsPlayerTwo);

	//玩家第三張牌投影位置
	m_ptsPlayerThree[0] = Point (461, 311);//LT
	m_ptsPlayerThree[1] = Point (535, 311);//RT
	m_ptsPlayerThree[2] = Point (461, 410);//LB
	m_ptsPlayerThree[3] = Point (536, 410);//RB
	m_matPlayerPerspectiveThree = getPerspectiveTransform (m_ptsTableTop, m_ptsPlayerThree);

	//玩家第四張牌投影位置
	m_ptsPlayerFour[0] = Point (627, 299);//LT
	m_ptsPlayerFour[1] = Point (700, 283);//RT
	m_ptsPlayerFour[2] = Point (648, 397);//LB
	m_ptsPlayerFour[3] = Point (722, 381);//RB
	m_matPlayerPerspectiveFour = getPerspectiveTransform (m_ptsTableTop, m_ptsPlayerFour);

	//玩家第五張牌投影位置
	m_ptsPlayerFive[0] = Point (786, 254);//LT
	m_ptsPlayerFive[1] = Point (854, 223);//RT
	m_ptsPlayerFive[2] = Point (829, 344);//LB
	m_ptsPlayerFive[3] = Point (896, 312);//RB
	m_matPlayerPerspectiveFive = getPerspectiveTransform (m_ptsTableTop, m_ptsPlayerFive);
}

void  CBlackJackDlg::Drawcard (vector<int>&vecCard, int iCounter[], int &iPoint, int &iPointA, int &A) 
{
	int iNum = 0;

	do
		iNum = rand () % CARD_NUMBER;
	while (iCounter[iNum] != 0);

	vecCard.push_back (iNum);
	iCounter[iNum]++;

	if (iNum >= 36 && iNum <= 51)
	{
		iPoint += 10;
		iPointA += 10;
	}
	else if (iNum >= 0 && iNum <= 3)
	{
		iPoint += 1;
		A++;

		if (A == 1)
			iPointA += 11;
		else
			iPointA += 1;
	}
	else if (iNum >= 4 && iNum <= 7)
	{
		iPoint += 2;
		iPointA += 2;
	}
	else if (iNum >= 8 && iNum <= 11)
	{
		iPoint += 3;
		iPointA += 3;
	}
	else if (iNum >= 12 && iNum <= 15)
	{
		iPoint += 4;
		iPointA += 4;
	}
	else if (iNum >= 16 && iNum <= 19)
	{
		iPoint += 5;
		iPointA += 5;
	}
	else if (iNum >= 20 && iNum <= 23)
	{
		iPoint += 6;
		iPointA += 6;
	}
	else if (iNum >= 24 && iNum <= 27)
	{
		iPoint += 7;
		iPointA += 7;
	}
	else if (iNum >= 28 && iNum <= 31)
	{
		iPoint += 8;
		iPointA += 8;
	}
	else if (iNum >= 32 && iNum <= 35)
	{
		iPoint += 9;
		iPointA += 9;
	}
}
void  CBlackJackDlg::Judge (int iBankerPnt, int iPlayerPoint, int iBankerPntA, int iPlayerPointA) 
{
	if (iBankerPntA > 21 && iPlayerPointA > 21)
	{
		if (iBankerPnt == iPlayerPoint && iBankerPnt <= 21 && iPlayerPoint <= 21) {
			//MessageBox (_T ("平手!!!"), _T ("Black Jack"));
			m_iGameStatus = TIE;
		}
		else if (iPlayerPoint > iBankerPnt && iPlayerPoint <= 21) {
			//MessageBox (_T ("你贏了!!!"), _T ("Black Jack"));
			m_iGameStatus = PLAYER_WIN;
		}
		else if (iBankerPnt > iPlayerPoint && iBankerPnt <= 21) {
			//MessageBox (_T ("你輸了!!!"), _T ("Black Jack"));
			m_iGameStatus = BANKER_WIN;
		}
	}
	else if (iBankerPntA <= 21 && iPlayerPointA <= 21)
	{
		if (iBankerPntA == iPlayerPointA) {
			//MessageBox (_T ("平手!!!"), _T ("Black Jack"));
			m_iGameStatus = TIE;
		}
		else if (iBankerPntA > iPlayerPointA) {
			//MessageBox (_T ("你輸了!!!"), _T ("Black Jack"));
			m_iGameStatus = BANKER_WIN;
		}
		else if (iPlayerPointA > iBankerPntA) {
			//MessageBox (_T ("你贏了!!!"), _T ("Black Jack"));
			m_iGameStatus = PLAYER_WIN;
		}
	}
	else if (iBankerPntA > 21 && iPlayerPointA <= 21)
	{
		if (iBankerPnt == iPlayerPointA) {
			//MessageBox (_T ("平手!!!"), _T ("Black Jack"));
			m_iGameStatus = TIE;
		}
		else if (iPlayerPointA > iBankerPnt) {
			//MessageBox (_T ("你贏了!!!"), _T ("Black Jack"));
			m_iGameStatus = PLAYER_WIN;
		}
		else if (iBankerPnt > iPlayerPointA) {
			//MessageBox (_T ("你輸了!!!"), _T ("Black Jack"));
			m_iGameStatus = BANKER_WIN;
		}
	}
	else if (iBankerPntA <= 21 && iPlayerPointA > 21)
	{
		if (iBankerPntA == iPlayerPoint) {
			//MessageBox (_T ("平手!!!"), _T ("Black Jack"));
			m_iGameStatus = TIE;
		}
		else if (iBankerPntA > iPlayerPoint) {
			//MessageBox (_T ("你輸了!!!"), _T ("Black Jack"));
			m_iGameStatus = BANKER_WIN;
		}
		else if (iPlayerPoint > iBankerPntA) {
			//MessageBox (_T ("你贏了!!!"), _T ("Black Jack"));
			m_iGameStatus = PLAYER_WIN;
		}
	}
	ShowStatus ();
}






void CBlackJackDlg::OnBnClickedButton100 ()
{
	if (m_iChips < 100)
		return;
	CRect rect;
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	ScreenToClient (rect);
	for (int i = 0; i < 100 ; i++)
	{
		m_iChips -= 1;
		m_iStakes += 1;
		InvalidateRect (rect, 0);
		WaitTime (5);
	}
	
}


BOOL CBlackJackDlg::OnEraseBkgnd (CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialogEx::OnEraseBkgnd (pDC);
	return TRUE;
}


void CBlackJackDlg::OnBnClickedButton1000 ()
{
	if (m_iChips < 1000)
		return;
	CRect rect;
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	ScreenToClient (rect);
	for (int i = 0; i < 1000; i+=2)
	{
		m_iChips -= 2;
		m_iStakes += 2;
		InvalidateRect (rect, 0);
		WaitTime (2);
	}
}


void CBlackJackDlg::OnBnClickedButton5000 ()
{
	if (m_iChips < 5000)
		return;
	CRect rect;
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	ScreenToClient (rect);
	for (int i = 0; i < 5000; i += 5)
	{
		m_iChips -= 5;
		m_iStakes += 5;
		InvalidateRect (rect, 0);
		WaitTime (2);
	}
}

void CBlackJackDlg::EnableFourButtons (BOOL bEnable1, BOOL bEnable2, BOOL bEnable3, BOOL bEnable4)
{
	//m_btnDeal.SetBitmaps (bEnable1 ? IDB_BITMAP_PLAY : IDB_BITMAP_PLAY_GREY, RGB (0, 0, 0));
	m_btnDeal.SetBitmaps (bEnable1 ? IDB_BITMAP_PLAY : IDB_BITMAP_PLAY_GREY, RGB (0, 0, 0));
	m_btnHit.SetBitmaps (bEnable2 ? IDB_BITMAP_HIT : IDB_BITMAP_HIT_GREY, RGB (0, 0, 0));
	m_btnStand.SetBitmaps (bEnable3 ? IDB_BITMAP_STAND : IDB_BITMAP_STAND_GREY, RGB (0, 0, 0));
	m_btnDouble.SetBitmaps (bEnable4 ? IDB_BITMAP_DOUBLE : IDB_BITMAP_DOUBLE_GREY, RGB (0, 0, 0));
	m_btnDeal.EnableWindow (bEnable1);
	m_btnHit.EnableWindow (bEnable2);
	m_btnStand.EnableWindow (bEnable3);
	m_btnDouble.EnableWindow (bEnable4);
}

void CBlackJackDlg::Restart ()
{
	m_iBankerPntDeal = 0;
	m_iBankerPnt = 0;
	m_iPlayerPnt = 0;
	m_iBankerPntA = 0;
	m_iPlayerPntA = 0;
	m_iBankerStatus = 0;
	m_iPlayerStatus = 0;
	m_iGameStatus = 0;
	m_iBankerA = 0;
	m_iPlayerA = 0;

	m_bBankerBJ = false;
	m_bPlayerBJ = false;

	memset (m_iCardCount, 0, sizeof (m_iCardCount));

	vecBankerCard.clear ();
	vecPlayerCard.clear ();

	m_strBankerPoint = "";
	m_strPlayerPoint = "";
	m_strPlayerPointA = "";

	m_matShow = m_matTableTop.clone ();
	imshow ("Black Jack", m_matTableTop);

	CRect rect;
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	ScreenToClient (rect);
	for (int i = 0; i < 100; i += 1)
	{
		m_iChips += -1;
		m_iStakes += 1;
		InvalidateRect(rect, 0);
		WaitTime (5);
	}

	EnableFourButtons (TRUE, FALSE, FALSE, TRUE);
}

void CBlackJackDlg::ShowStatus ()
{
	int iRadius = 50;
	Point ptOffset (30, 0);
	putText (m_matShow, m_strPlayerPoint, PLAYER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	putText (m_matShow, m_strBankerPoint, BANKER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	switch (m_iGameStatus)
	{
	case PLAYER_WIN:
		circle (m_matShow, PLAYER_POINT_POS + ptOffset, iRadius, clrRed, 5);
		break;
	case PLAYER_WIN_5CARDS:
		circle (m_matShow, PLAYER_POINT_POS + ptOffset, iRadius, clrRed, 5);
		break;
	case BANKER_WIN:
		circle (m_matShow, BANKER_POINT_POS + ptOffset, iRadius, clrRed, 5);
		break;
	case TIE:
		circle (m_matShow, PLAYER_POINT_POS + ptOffset, iRadius, clrRed, 5);
		circle (m_matShow, BANKER_POINT_POS + ptOffset, iRadius, clrRed, 5);
		break;
	}
	
	
	imshow ("Black Jack", m_matShow);
	HWND hWnd = (HWND)cvGetWindowHandle ("Black Jack");

	WaitTime (1500);
	::ShowWindow (hWnd, SW_HIDE);

	int iOrgChips = m_iChips;
	int iNewChips = m_iChips;
	switch (m_iGameStatus)
	{
	case PLAYER_WIN:
		iNewChips += m_iStakes * 2;
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_WIN, RGB (0, 128, 0));
		circle (m_matShow, Point (800, 400), iRadius, clrRed, 5);
		break;
	case PLAYER_WIN_5CARDS:
		iNewChips += m_iStakes * 4;
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_WIN, RGB (0, 128, 0));
		circle (m_matShow, Point (800, 400), iRadius, clrRed, 5);
		break;
	case BANKER_WIN:
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_LOSE, RGB (0, 128, 0));
		circle (m_matShow, Point (800, 60), iRadius, clrRed, 5);
		break;
	case TIE:
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_TIE, RGB (0, 128, 0));
		circle (m_matShow, Point (800, 60), iRadius, clrRed, 5);
		circle (m_matShow, Point (800, 400), iRadius, clrRed, 5);
		break;
	}
	m_btnShowStatus.ShowWindow (SW_SHOW);

	CRect rect;
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	ScreenToClient (rect);
	for (int i = 0; i < iNewChips - iOrgChips; i += 1)
	{
		m_iChips += 1;
		if (m_iStakes > 0)
			m_iStakes -= 1;
		InvalidateRect (rect, 0);
		WaitTime (2);
	}
	WaitTime (2000);
	::ShowWindow (hWnd, SW_SHOW);
	m_btnShowStatus.ShowWindow (SW_HIDE);

	m_iStakes = 0;
	InvalidateRect (rect, 0);
	Restart ();
}

void CBlackJackDlg::ShowPoint (Mat matShow)
{
	putText (matShow, m_strPlayerPoint, PLAYER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	putText (matShow, m_strBankerPoint, BANKER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	imshow ("Black Jack", matShow);
}



void CBlackJackDlg::OnBnClickedButtonDouble ()
{
	if (m_iChips < m_iStakes)
		return;
	CRect rect;
	GetDlgItem (IDC_PIC_NUM_DISPLAY)->GetWindowRect (rect);
	ScreenToClient (rect);
	int iStake = m_iStakes;
	for (int i = 0; i < iStake; i += 1)
	{
		m_iChips -= 1;
		m_iStakes += 1;
		InvalidateRect (rect, 0);
		WaitTime (2);
	}
}
