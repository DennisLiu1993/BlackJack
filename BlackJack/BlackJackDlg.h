
// BlackJackDlg.h: 標頭檔
//
#include "opencv2/opencv.hpp"
#include "BtnST.h"
using namespace cv;
using namespace std;
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstring>

#define CARD_NUMBER 52
#define ONE_CARD 1
#define TWO_CARD 2
#define THREE_CARD 3
#define FOUR_CARD 4
#define FIVE_CARD 5
#define STATUS_NONE 10
#define PLAYER_WIN 11
#define PLAYER_WIN_5CARDS 12
#define BANKER_WIN 13
#define TIE 14

#define TABLE_WIDTH 1002
#define TABLE_HEIGHT 450
#define CARD_WIDTH 71
#define CARD_HEIGHT 96
// CBlackJackDlg 對話方塊
class CBlackJackDlg : public CDialogEx
{
// 建構
public:
	CBlackJackDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BlackJack_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CButtonST m_btnDeal;
	CButtonST m_btnHit;
	CButtonST m_btnStand;
	CButtonST m_btnDouble;

	Mat m_matTableTop;
	Mat m_matShow;
	Mat m_matBankerDealPnt;
	Mat m_matPlayerHitPnt;
	Mat m_matPlayerHitPntNoBankerPnt;

	Point2f m_ptsCard[4];
	Point2f m_ptsPlayerPos[5][4];//五張四個角
	Point2f m_ptsBankerPos[5][4];//五張四個角

	Mat m_matPlayerPerspective[5];//五張牌透視矩陣
	Mat m_matBankerPerspective[5];//五張牌透視矩陣

	Mat m_matCard[CARD_NUMBER];								//52張牌圖形array
	Mat m_matHoleCard;								//底牌圖形

	int m_iBankerPntDeal;							//莊家發牌的點數
	int m_iBankerPnt;								//莊家點數
	int m_iPlayerPnt;								//玩家點數
	int m_iBankerPntA;								//莊家A當11點的點數
	int m_iPlayerPntA;								//第一張點A當11點
	int m_iBankerStatus;							//莊家抽牌數
	int	m_iPlayerStatus;							//玩家抽牌數
	int m_iGameStatus;								//1玩家贏 2莊家贏 3平手
	int m_iCardCount[CARD_NUMBER];					//每張牌一開始都沒抽過所以=0

	bool m_bBankerBJ;								//莊家是否BJ
	bool m_bPlayerBJ;								//玩家是否BJ
	int m_iBankerA;									//莊家是否抽到A
	int m_iPlayerA;									//玩家是否抽到A

	vector<int> vecBankerCard;						//放置莊家牌vector
	vector<int> vecPlayerCard;						//放置玩家牌vector

	String m_strBankerPoint;						//莊家點數
	String m_strPlayerPoint;						//玩家點數
	String m_strPlayerPointA;						//玩家A當11時的點數

	Scalar clrString;
	Scalar clrRed;
; public:
	afx_msg void OnBnClickedButtonDeal ();
	void Drawcard (vector<int> &vecCard, int iCounter[], int &iPoint, int &iPointA, int &A);
	void Judge (int iBankerPnt, int iPlayerPoint, int iBankerPntA, int iPlayerPointA);
	void SetPerspectivePoint ();
	afx_msg void OnBnClickedButtonHit ();
	afx_msg void OnBnClickedButtonStand ();
	CButtonST m_btn100;
	CButtonST m_btn1000;
	CButtonST m_btn5000;
	void WaitTime (int iTime);
	void WarpPerspective (Mat matSrc, Mat matDst, Mat matTransform, BOOL bBanker, int iCardPosIndex, BOOL bFlip = TRUE);
	void DrawNumber (CDC* pdc);
	int m_iChips;
	int m_iStakes;
	afx_msg void OnBnClickedButton100 ();
	afx_msg BOOL OnEraseBkgnd (CDC* pDC);
	afx_msg void OnBnClickedButton1000 ();
	afx_msg void OnBnClickedButton5000 ();
	void EnableFourButtons (BOOL bEnable1, BOOL bEnable2, BOOL bEnable3, BOOL bEnable4);
	void Restart ();
	void ShowStatus ();
	void ShowPoint (Mat matShow);
	int GetCardPoint (int iCard);
	afx_msg void OnBnClickedButtonDouble ();
	CButtonST m_btnShowStatus;
};
