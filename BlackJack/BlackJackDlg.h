
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
#define PLAYER_WIN 11
#define PLAYER_WIN_5CARDS 12
#define BANKER_WIN 13
#define TIE 14

#define TABLE_WIDTH 1002
#define TABLE_HEIGHT 450
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

	Point2f m_ptsTableTop[4];
	Point2f m_ptsPlayerOne[4];
	Point2f m_ptsPlayerTwo[4];
	Point2f m_ptsPlayerThree[4];
	Point2f m_ptsPlayerFour[4];
	Point2f m_ptsPlayerFive[4];
	Point2f m_ptsBankerOne[4];
	Point2f m_ptsBankerTwo[4];
	Point2f m_ptsBankerThree[4];
	Point2f m_ptsBankerFour[4];
	Point2f m_ptsBankerFive[4];

	Mat m_matPlayerPerspectiveOne;
	Mat m_matPlayerPerspectiveTwo;
	Mat m_matPlayerPerspectiveThree;
	Mat m_matPlayerPerspectiveFour;
	Mat m_matPlayerPerspectiveFive;
	Mat m_matBankerPerspectiveOne;
	Mat m_matBankerPerspectiveTwo;
	Mat m_matBankerPerspectiveThree;
	Mat m_matBankerPerspectiveFour;
	Mat m_matBankerPerspectiveFive;

	Mat m_matCard[52];								//52張牌圖形array
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
	void WarpPerspective (Mat matSrc, Mat matDst, Mat matTransform, Size size, int iFlags, int iBorder);
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
	afx_msg void OnBnClickedButtonDouble ();
	CButtonST m_btnShowStatus;
};
