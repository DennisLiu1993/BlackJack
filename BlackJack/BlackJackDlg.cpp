
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

#define BANKER_CARD1_LT Point (262, 25)
#define BANKER_CARD1_RT Point (333, 25)
#define BANKER_CARD1_LB Point (262, 121)
#define BANKER_CARD1_RB Point (333, 121)

#define BANKER_CARD2_LT Point (362, 25)
#define BANKER_CARD2_RT Point (433, 25)
#define BANKER_CARD2_LB Point (362, 121)
#define BANKER_CARD2_RB Point (433, 121)

#define BANKER_CARD3_LT Point (462, 25)
#define BANKER_CARD3_RT Point (533, 25)
#define BANKER_CARD3_LB Point (462, 121)
#define BANKER_CARD3_RB Point (533, 121)

#define BANKER_CARD4_LT Point (562, 25)
#define BANKER_CARD4_RT Point (633, 25)
#define BANKER_CARD4_LB Point (562, 121)
#define BANKER_CARD4_RB Point (633, 121)

#define BANKER_CARD5_LT Point (662, 25)
#define BANKER_CARD5_RT Point (733, 25)
#define BANKER_CARD5_LB Point (662, 121)
#define BANKER_CARD5_RB Point (733, 121)

#define PLAYER_CARD1_LT Point2f (144, 223)
#define PLAYER_CARD1_RT Point2f (211, 254)
#define PLAYER_CARD1_LB Point2f (102, 313)
#define PLAYER_CARD1_RB Point2f (169, 344)

#define PLAYER_CARD2_LT Point (297, 283)
#define PLAYER_CARD2_RT Point (371, 299)
#define PLAYER_CARD2_LB Point (276, 380)
#define PLAYER_CARD2_RB Point (349, 397)

#define PLAYER_CARD3_LT Point (461, 311)
#define PLAYER_CARD3_RT Point (535, 311)
#define PLAYER_CARD3_LB Point (461, 410)
#define PLAYER_CARD3_RB Point (536, 410)

#define PLAYER_CARD4_LT Point (627, 299)
#define PLAYER_CARD4_RT Point (700, 283)
#define PLAYER_CARD4_LB Point (648, 397)
#define PLAYER_CARD4_RB Point (722, 381)

#define PLAYER_CARD5_LT Point (786, 254)
#define PLAYER_CARD5_RT Point (854, 223)
#define PLAYER_CARD5_LB Point (829, 344)
#define PLAYER_CARD5_RB Point (896, 312)
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
	m_iGameStatus = STATUS_NONE;
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
	//從空牌桌開始
	Mat matTmp = m_matTableTop.clone ();
	//莊家抽第一張牌/第二張牌
	Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
	Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
	m_iBankerStatus = TWO_CARD;
	WarpPerspective (m_matHoleCard, matTmp, m_matBankerPerspective[0], TRUE, -1);//底牌
	WarpPerspective (m_matCard[vecBankerCard[1]], matTmp, m_matBankerPerspective[1], TRUE, 1);

	//玩家抽第一張牌/第二張牌
	Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
	Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
	m_iPlayerStatus = TWO_CARD;
	WarpPerspective (m_matCard[vecPlayerCard[0]], matTmp, m_matPlayerPerspective[0], FALSE, 0);
	WarpPerspective (m_matCard[vecPlayerCard[1]], matTmp, m_matPlayerPerspective[1], FALSE, 1);
	//無點數複製一張
	Mat matNoPnts = matTmp.clone ();
	//先顯示莊家明牌點數
	m_strBankerPoint = format ("%d", GetCardPoint (vecBankerCard[1]));
	putText (matTmp, m_strBankerPoint, BANKER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	imshow ("Black Jack", matTmp);
	WaitTime (300);
	//顯示閒家點數
	if (m_iPlayerA != 0)
		m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
	else
		m_strPlayerPoint = format ("%d", m_iPlayerPnt);
	putText (matTmp, m_strPlayerPoint, PLAYER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	imshow ("Black Jack", matTmp);
	WaitTime (800);
	//莊家Black Jack
	if (vecBankerCard[0] <= 3 && (vecBankerCard[1] >= 36 && vecBankerCard[1] <= 51) || (vecBankerCard[0] >= 36 && vecBankerCard[0] <= 51) && vecBankerCard[1] <= 3)
		m_bBankerBJ = true;

	//玩家BlackJack
	if (vecPlayerCard[0] <= 3 && (vecPlayerCard[1] >= 36 && vecPlayerCard[1] <= 51) || (vecPlayerCard[0] >= 36 && vecPlayerCard[0] <= 51) && vecPlayerCard[1] <= 3)
		m_bPlayerBJ = true;

	if ( (m_bBankerBJ && !m_bPlayerBJ) ||
		 (!m_bBankerBJ && m_bPlayerBJ) ||
		 (m_bBankerBJ  && m_bPlayerBJ) )
	{
		matTmp = m_matTableTop.clone ();
		//翻底牌前先還原到莊家有一張明牌 閒家兩張牌的桌面 (共3張)
		WarpPerspective (m_matCard[vecBankerCard[1]], matTmp, m_matBankerPerspective[1], TRUE, 1, FALSE);
		WarpPerspective (m_matCard[vecPlayerCard[0]], matTmp, m_matPlayerPerspective[0], TRUE, 0, FALSE);
		WarpPerspective (m_matCard[vecPlayerCard[1]], matTmp, m_matPlayerPerspective[1], TRUE, 1, FALSE);
		putText (matTmp, m_strBankerPoint, BANKER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		putText (matTmp, m_strPlayerPoint, PLAYER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
		//翻開底牌
		WarpPerspective (m_matCard[vecBankerCard[0]], matTmp, m_matBankerPerspective[0], TRUE, 0);
		m_matShow = matTmp.clone ();

		m_iGameStatus = 
			(m_bBankerBJ && !m_bPlayerBJ) ? BANKER_WIN :
			(!m_bBankerBJ && m_bPlayerBJ) ? PLAYER_WIN : TIE;
		ShowStatus ();
	}
	//沒有人BlackJack，保持第一張牌不顯示
	else 
	{
		m_matShow = matNoPnts.clone ();
		EnableFourButtons (0, 1, 1, 1);
	}
}

void CBlackJackDlg::OnBnClickedButtonHit ()
{
	EnableFourButtons (0, 0, 0, 0);
	srand ((unsigned int)time (NULL));
	//上一階段牌桌(無點)
	Mat matLastStatus = m_matShow.clone ();
	Mat matCurrentStatus = m_matShow.clone ();
	Mat matForNext = m_matShow.clone ();
	//上一階段點數
	putText (matLastStatus, m_strPlayerPoint, PLAYER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	putText (matLastStatus, m_strBankerPoint, BANKER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	//抽第三或第四或第五張牌

	//抽第m_iPlayerStatus + 1 張牌時使用舊點數背景
	Drawcard (vecPlayerCard, m_iCardCount, m_iPlayerPnt, m_iPlayerPntA, m_iPlayerA);
	WarpPerspective (m_matCard[vecPlayerCard[m_iPlayerStatus]], matLastStatus, m_matPlayerPerspective[m_iPlayerStatus], FALSE, m_iPlayerStatus);
	//並把第m_iPlayerStatus + 1 張牌放到即將顯示新點數的牌桌與下一階段牌桌
	WarpPerspective (m_matCard[vecPlayerCard[m_iPlayerStatus]], matForNext, m_matPlayerPerspective[m_iPlayerStatus], FALSE, m_iPlayerStatus, FALSE);
	WarpPerspective (m_matCard[vecPlayerCard[m_iPlayerStatus]], matCurrentStatus, m_matPlayerPerspective[m_iPlayerStatus], FALSE, m_iPlayerStatus, FALSE);
	if (m_iPlayerPntA <= 21 && m_iPlayerA != 0)
		m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
	else
		m_strPlayerPoint = format ("%d", m_iPlayerPnt);

	//玩家新點數 莊家舊點數
	putText (matCurrentStatus, m_strBankerPoint, BANKER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	putText (matCurrentStatus, m_strPlayerPoint, PLAYER_POINT_POS, FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, clrString, 3);
	imshow ("Black Jack", matCurrentStatus);
	m_iPlayerStatus++;
	//保留當前階段牌桌(無點)
	m_matShow = matForNext.clone ();
	if (m_iPlayerPnt > 21)
	{
		m_iGameStatus = BANKER_WIN;
		ShowStatus ();
		EnableFourButtons (1, 0, 0, 1);
	}
	else if (m_iPlayerStatus == FIVE_CARD)
	{
		m_iGameStatus = PLAYER_WIN_5CARDS;
		ShowStatus ();
		EnableFourButtons (1, 0, 0, 1);
	}
	else if (m_iPlayerPnt == 21)//玩家21點莊家自動抽牌
		OnBnClickedButtonStand ();
	else
		EnableFourButtons (0, 1, 1, 1);
}


void CBlackJackDlg::OnBnClickedButtonStand ()
{
	EnableFourButtons (0, 0, 0, 0);
	//複製一張空牌桌，將顯示莊家第二張及閒家所有牌
	Mat matStand = m_matTableTop.clone ();
	Mat matStandNoPnts;
	WarpPerspective (m_matCard[vecBankerCard[1]], matStand, m_matBankerPerspective[1], FALSE, 1, FALSE);
	for (int i = 0; i < m_iPlayerStatus ; i++)
		WarpPerspective (m_matCard[vecPlayerCard[i]], matStand, m_matPlayerPerspective[i], FALSE, i, FALSE);

	matStandNoPnts = matStand.clone ();
	//玩家/閒家點數
	if (m_iPlayerPntA <= 21 && m_iPlayerA != 0)
		m_strPlayerPoint = format ("%d/%d", m_iPlayerPnt, m_iPlayerPntA);
	else
		m_strPlayerPoint = format ("%d", m_iPlayerPnt);
	ShowPoint (matStand);

	if (m_iGameStatus == STATUS_NONE)
	{
		//印出蓋住的第一張牌 (此時matStand已經有點數
		WarpPerspective (m_matCard[vecBankerCard[0]], matStand, m_matBankerPerspective[0], TRUE, 0);
		WarpPerspective (m_matCard[vecBankerCard[0]], matStandNoPnts, m_matBankerPerspective[0], TRUE, 0, FALSE);
		matStand = matStandNoPnts.clone ();
		if (m_iBankerA != 0)
			m_strBankerPoint = format ("%d", m_iBankerPntA);
		else
			m_strBankerPoint = format ("%d", m_iBankerPnt);

		ShowPoint (matStand);
		imshow ("Black Jack", matStand);
		WaitTime (1000);
		if (m_iBankerStatus == TWO_CARD)
		{	
			//未達17點則繼續抽牌
			if (m_iBankerPntA < 17 || (m_iBankerPnt < 17 && m_iBankerPntA > 21))
				m_iBankerStatus = THREE_CARD;
			//達17點則顯示莊家點數並判斷勝負
			else
			{
				m_matShow = matStand.clone ();
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
			WarpPerspective (m_matCard[vecBankerCard[2]], matStand, m_matBankerPerspective[2], TRUE, 2);
			WarpPerspective (m_matCard[vecBankerCard[2]], matStandNoPnts, m_matBankerPerspective[2], TRUE, 2, FALSE);
			matStand = matStandNoPnts.clone ();
			if (m_iBankerA != 0 && m_iBankerPntA <= 21)
				m_strBankerPoint = format ("%d", m_iBankerPntA);
			else
				m_strBankerPoint = format ("%d", m_iBankerPnt);
			ShowPoint (matStand);
			imshow ("Black Jack", matStand);
			WaitTime (1000);
			//未達17點則繼續抽牌
			if (m_iBankerPntA < 17 || (m_iBankerPnt < 17 && m_iBankerPntA > 21))
				m_iBankerStatus = FOUR_CARD;
			//超過21點爆掉玩家勝利
			else if (m_iBankerPnt > 21)
			{
				m_matShow = matStand.clone ();
				m_iGameStatus = PLAYER_WIN;
				ShowStatus ();
			}
			//莊家17~21點則顯示莊家點數並判斷勝負
			else
			{
				m_matShow = matStand.clone ();
				Judge (m_iBankerPnt, m_iPlayerPnt, m_iBankerPntA, m_iPlayerPntA);
			}
		}

		if (m_iBankerStatus == FOUR_CARD)
		{
			//莊家抽第四張牌
			Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
			//顯示莊家第四張牌
			WarpPerspective (m_matCard[vecBankerCard[3]], matStand, m_matBankerPerspective[3], TRUE, 3);
			WarpPerspective (m_matCard[vecBankerCard[3]], matStandNoPnts, m_matBankerPerspective[3], TRUE, 3, FALSE);
			matStand = matStandNoPnts.clone ();
			if (m_iBankerA != 0 && m_iBankerPntA <= 21)
				m_strBankerPoint = format ("%d", m_iBankerPntA);
			else
				m_strBankerPoint = format ("%d", m_iBankerPnt);
			ShowPoint (matStand);
			imshow ("Black Jack", matStand);
			WaitTime (1000);
			//未達17點則繼續抽牌
			if (m_iBankerPntA < 17 || (m_iBankerPnt < 17 && m_iBankerPntA > 21))
				m_iBankerStatus = FIVE_CARD;
			//超過21點爆掉玩家勝利
			else if (m_iBankerPnt > 21)
			{
				m_matShow = matStand.clone ();
				m_iGameStatus = PLAYER_WIN;
				ShowStatus ();
			}
			//莊家17~21點則顯示莊家點數並判斷勝負
			else
			{
				m_matShow = matStand.clone ();
				Judge (m_iBankerPnt, m_iPlayerPnt, m_iBankerPntA, m_iPlayerPntA);
			}
				
		}
		if (m_iBankerStatus == FIVE_CARD)
		{
			//莊家抽第五張牌
			Drawcard (vecBankerCard, m_iCardCount, m_iBankerPnt, m_iBankerPntA, m_iBankerA);
			//顯示莊家第五張牌
			WarpPerspective (m_matCard[vecBankerCard[4]], matStand, m_matBankerPerspective[4], TRUE, 4);
			WarpPerspective (m_matCard[vecBankerCard[4]], matStandNoPnts, m_matBankerPerspective[4], TRUE, 4, FALSE);
			matStand = matStandNoPnts.clone ();
			if (m_iBankerA != 0 && m_iBankerPntA <= 21)
				m_strBankerPoint = format ("%d", m_iBankerPntA);
			else
				m_strBankerPoint = format ("%d", m_iBankerPnt);

			ShowPoint (matStand);
			imshow ("Black Jack", matStand);
			WaitTime (1000);
			//超過21點爆掉玩家勝利
			if (m_iBankerPnt > 21)
			{
				m_matShow = matStand.clone ();
				m_iGameStatus = PLAYER_WIN;
				ShowStatus ();
			}
			//莊家未達21點則顯示莊家點數並判斷勝負
			else
			{
				m_matShow = matStand.clone ();
				Judge (m_iBankerPnt, m_iPlayerPnt, m_iBankerPntA, m_iPlayerPntA);
			}
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

void CBlackJackDlg::WarpPerspective (Mat matSrc, Mat matDst, Mat matTransform, BOOL bBanker, int iPos, BOOL bFlip)
{
	if (iPos == -1 || !bFlip)//不翻
	{
		warpPerspective (matSrc, matDst, matTransform, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		if (bFlip)
		{
			imshow ("Black Jack", matDst);
			WaitTime (500);
		}
	}
	else
	{
		Point2f ptCard_4Corners[4];
		for (int i = 0; i < 4; i++)
			ptCard_4Corners[i] = bBanker ? m_ptsBankerPos[iPos][i] : m_ptsPlayerPos[iPos][i];
		Point2f vecLT2RT = ptCard_4Corners[1] - ptCard_4Corners[0];//RT-LT
		Point2f vecLB2RB = ptCard_4Corners[3] - ptCard_4Corners[2];//RB-LB

		Point2f ptsNewCorner[4];
		int iSteps = 20;
		//back
		for (int i = 1; i < iSteps; i++)
		{
			Mat matShow = matDst.clone ();
			ptsNewCorner[0] = ptCard_4Corners[0] + vecLT2RT / (iSteps * 2) * i;//LT
			ptsNewCorner[1] = ptCard_4Corners[1] - vecLT2RT / (iSteps * 2) * i;//RT
			ptsNewCorner[2] = ptCard_4Corners[2] + vecLT2RT / (iSteps * 2) * i;//LB
			ptsNewCorner[3] = ptCard_4Corners[3] - vecLT2RT / (iSteps * 2) * i;//RB
			Mat matAction = getPerspectiveTransform (m_ptsCard, ptsNewCorner);
			warpPerspective (m_matHoleCard, matShow, matAction, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
			imshow ("Black Jack", matShow);
			WaitTime (15);
		}
		//front
		for (int i = iSteps; i > 0; i--)
		{
			Mat matShow = matDst.clone ();
			ptsNewCorner[0] = ptCard_4Corners[0] + vecLT2RT / (iSteps * 2) * i;//LT
			ptsNewCorner[1] = ptCard_4Corners[1] - vecLT2RT / (iSteps * 2) * i;//RT
			ptsNewCorner[2] = ptCard_4Corners[2] + vecLT2RT / (iSteps * 2) * i;//LB
			ptsNewCorner[3] = ptCard_4Corners[3] - vecLT2RT / (iSteps * 2) * i;//RB
			Mat matAction = getPerspectiveTransform (m_ptsCard, ptsNewCorner);
			warpPerspective (matSrc, matShow, matAction, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
			imshow ("Black Jack", matShow);
			WaitTime (15);
		}
		warpPerspective (matSrc, matDst, matTransform, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
		imshow ("Black Jack", matDst);
	}
		
	
	/*warpPerspective (matSrc, matDst, matTransform, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
	imshow ("Black Jack", matDst);*/
	//if (bBanker && (iPos == -1 || iPos == 0))
		WaitTime (200);
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
	m_ptsCard[0] = Point (0, 0);//LT
	m_ptsCard[1] = Point (CARD_WIDTH - 1, 0);//RT
	m_ptsCard[2] = Point (0, CARD_HEIGHT - 1);//LB
	m_ptsCard[3] = Point (CARD_WIDTH - 1, CARD_HEIGHT- 1);//RB

	//莊家第一張牌投影位置
	m_ptsBankerPos[0][0] = BANKER_CARD1_LT;//LT
	m_ptsBankerPos[0][1] = BANKER_CARD1_RT;//RT
	m_ptsBankerPos[0][2] = BANKER_CARD1_LB;//LB
	m_ptsBankerPos[0][3] = BANKER_CARD1_RB;//RB
	m_matBankerPerspective[0] = getPerspectiveTransform (m_ptsCard, m_ptsBankerPos[0]);

	//莊家第二張牌投影位置
	m_ptsBankerPos[1][0] = BANKER_CARD2_LT;//LT
	m_ptsBankerPos[1][1] = BANKER_CARD2_RT;//RT
	m_ptsBankerPos[1][2] = BANKER_CARD2_LB;//LB
	m_ptsBankerPos[1][3] = BANKER_CARD2_RB;//RB
	m_matBankerPerspective[1] = getPerspectiveTransform (m_ptsCard, m_ptsBankerPos[1]);

	//莊家第三張牌投影位置
	m_ptsBankerPos[2][0] = BANKER_CARD3_LT;//LT
	m_ptsBankerPos[2][1] = BANKER_CARD3_RT;//RT
	m_ptsBankerPos[2][2] = BANKER_CARD3_LB;//LB
	m_ptsBankerPos[2][3] = BANKER_CARD3_RB;//RB
	m_matBankerPerspective[2] = getPerspectiveTransform (m_ptsCard, m_ptsBankerPos[2]);

	//莊家第四張牌投影位置
	m_ptsBankerPos[3][0] = BANKER_CARD4_LT;//LT
	m_ptsBankerPos[3][1] = BANKER_CARD4_RT;//RT
	m_ptsBankerPos[3][2] = BANKER_CARD4_LB;//LB
	m_ptsBankerPos[3][3] = BANKER_CARD4_RB;//RB
	m_matBankerPerspective[3] = getPerspectiveTransform (m_ptsCard, m_ptsBankerPos[3]);

	//莊家第五張牌投影位置
	m_ptsBankerPos[4][0] = BANKER_CARD5_LT;//LT
	m_ptsBankerPos[4][1] = BANKER_CARD5_RT;//RT
	m_ptsBankerPos[4][2] = BANKER_CARD5_LB;//LB
	m_ptsBankerPos[4][3] = BANKER_CARD5_RB;//RB
	m_matBankerPerspective[4] = getPerspectiveTransform (m_ptsCard, m_ptsBankerPos[4]);

	//玩家第一張牌投影位置
	m_ptsPlayerPos[0][0] = PLAYER_CARD1_LT;//LT
	m_ptsPlayerPos[0][1] = PLAYER_CARD1_RT;//RT
	m_ptsPlayerPos[0][2] = PLAYER_CARD1_LB;//LB
	m_ptsPlayerPos[0][3] = PLAYER_CARD1_RB;//RB
	m_matPlayerPerspective[0] = getPerspectiveTransform (m_ptsCard, m_ptsPlayerPos[0]);

	//玩家第二張牌投影位置
	m_ptsPlayerPos[1][0] = PLAYER_CARD2_LT;//LT
	m_ptsPlayerPos[1][1] = PLAYER_CARD2_RT;//RT
	m_ptsPlayerPos[1][2] = PLAYER_CARD2_LB;//LB
	m_ptsPlayerPos[1][3] = PLAYER_CARD2_RB;//RB
	m_matPlayerPerspective[1] = getPerspectiveTransform (m_ptsCard, m_ptsPlayerPos[1]);

	//玩家第三張牌投影位置
	m_ptsPlayerPos[2][0] = PLAYER_CARD3_LT;//LT
	m_ptsPlayerPos[2][1] = PLAYER_CARD3_RT;//RT
	m_ptsPlayerPos[2][2] = PLAYER_CARD3_LB;//LB
	m_ptsPlayerPos[2][3] = PLAYER_CARD3_RB;//RB
	m_matPlayerPerspective[2] = getPerspectiveTransform (m_ptsCard, m_ptsPlayerPos[2]);

	//玩家第四張牌投影位置
	m_ptsPlayerPos[3][0] = PLAYER_CARD4_LT;//LT
	m_ptsPlayerPos[3][1] = PLAYER_CARD4_RT;//RT
	m_ptsPlayerPos[3][2] = PLAYER_CARD4_LB;//LB
	m_ptsPlayerPos[3][3] = PLAYER_CARD4_RB;//RB
	m_matPlayerPerspective[3] = getPerspectiveTransform (m_ptsCard, m_ptsPlayerPos[3]);

	//玩家第五張牌投影位置
	m_ptsPlayerPos[4][0] = PLAYER_CARD5_LT;//LT
	m_ptsPlayerPos[4][1] = PLAYER_CARD5_LT;//RT
	m_ptsPlayerPos[4][2] = PLAYER_CARD5_LB;//LB
	m_ptsPlayerPos[4][3] = PLAYER_CARD5_RB;//RB
	m_matPlayerPerspective[4] = getPerspectiveTransform (m_ptsCard, m_ptsPlayerPos[4]);
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
	m_iGameStatus = STATUS_NONE;
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
		if (stoi (m_strPlayerPoint) == 21)
			iNewChips += m_iStakes * 2.5;
		else
			iNewChips += m_iStakes * 2;
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_WIN, RGB (0, 128, 0));
		circle (m_matShow, PLAYER_POINT_POS, iRadius, clrRed, 5);
		break;
	case PLAYER_WIN_5CARDS:
		iNewChips += m_iStakes * 4;
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_WIN, RGB (0, 128, 0));
		circle (m_matShow, PLAYER_POINT_POS, iRadius, clrRed, 5);
		break;
	case BANKER_WIN:
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_LOSE, RGB (0, 128, 0));
		circle (m_matShow, BANKER_POINT_POS, iRadius, clrRed, 5);
		break;
	case TIE:
		m_btnShowStatus.SetBitmaps (IDB_BITMAP_TIE, RGB (0, 128, 0));
		circle (m_matShow, BANKER_POINT_POS, iRadius, clrRed, 5);
		circle (m_matShow, PLAYER_POINT_POS, iRadius, clrRed, 5);
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
	//imshow ("Black Jack", matShow);
}

int CBlackJackDlg::GetCardPoint (int iCard)
{
	int iPoint = iCard / 4 + 1;
	if (iPoint > 10)
		iPoint = 10;
	return iPoint;
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
