# BlackJack
Using MFC to create a BlackJack game platform

# What You Can Learn in This Project
1. double buffer drawing strategy in MFC to prevent controls or numbers from blinking (see OnPaint ())
2. usage of CButtonST (Image Button with features of transparent background and no border) 
3. OpenCV perspective tranform to show slanted cards (see WarpPespective ())
4. dynamically show number changes (see code when win or lose)
5. dealing cards animation

# Interface
![image](https://user-images.githubusercontent.com/104763587/188288051-1a41785f-8329-4320-9ca6-277cc1948388.png)

# Demo Gif
![image](https://github.com/DennisLiu1993/BlackJack/blob/main/BlackJack/BlackJack.gif)

# How to Build
1.	Download Visual Studio 2017 or newer versions
2.	Check on the option of "x86 and x64 version of C++ MFC"
3.	Install
4.	Open BlackJack.vcxproj
5.	Upgrade if it is required
6.	Open this project's property page
7.	Choose the SDK version you have in "General-Windows SDK Version"
8.	Choose the right toolset you have in "General-Platform Toolset" (for me, it is Visual Studio 2017 (v141))
9.	Go to "C/C++ - General", and type in "Additional Include Directories" for your own OpenCV include headers (e.g. C:\OpenCV3.1\opencv\build\include or C:\OpenCV4.0\opencv\build\include)
11.	Type in "Library Directories" for your own OpenCV's library path (the directory where your opencv_worldXX.lib locates)
12.	Go to "Linker-Input", and type in library name (e.g. C:\OpenCV3.1\build2017_worldx64\lib\opencv_world310d_vs2017_x64.lib)
13.	Make sure that your opencv_worldXX.dll and is in the same directory as .exe of this project (after building)

# Extentions
With Perspective Tranform, we can make the animation of deal more fluent and sophistocated.

If we want to build a full action of flipping cards, we need to divide this action into several steps.

For instance, now I break flipping cards into 20 actions with 20 different perspective matrix:

In this way, showing all cards continually can simulate the action of flipping

![2](https://user-images.githubusercontent.com/104763587/188575917-806237ab-1918-4f6d-b32d-9834fbcf6663.jpg)
![3](https://user-images.githubusercontent.com/104763587/188575931-00db9670-9c5b-4729-9e52-c25a78f3c66a.jpg)
![4](https://user-images.githubusercontent.com/104763587/188575933-2f38e80f-93f1-47ce-b647-eda11ca15c5a.jpg)
![5](https://user-images.githubusercontent.com/104763587/188575935-768acafc-80da-4b40-9170-90207f42df79.jpg)
![6](https://user-images.githubusercontent.com/104763587/188575938-21c20297-5794-47b0-afde-7a3ca9578f23.jpg)
![7](https://user-images.githubusercontent.com/104763587/188575941-0ba6d4c4-878b-46ff-9787-0af49ab9798c.jpg)
![8](https://user-images.githubusercontent.com/104763587/188575946-f83b76e9-30fe-4042-b4c3-6ce0294fb503.jpg)
![9](https://user-images.githubusercontent.com/104763587/188575949-2068043c-8de0-488f-ac38-976a2222c869.jpg)
![10](https://user-images.githubusercontent.com/104763587/188575950-a8302702-ccef-4165-9a12-b958db2a1e8e.jpg)
![11](https://user-images.githubusercontent.com/104763587/188575954-f5a280b9-7c5b-406d-8e16-dccd10ae2c89.jpg)
![12](https://user-images.githubusercontent.com/104763587/188575957-d6e3c397-b6c8-4ab4-8309-70e47b945731.jpg)
![13](https://user-images.githubusercontent.com/104763587/188575958-c802ad1c-7fec-4e5c-9be8-0a7b0c7b2935.jpg)
![14](https://user-images.githubusercontent.com/104763587/188575959-7d224321-7d85-4a44-81f8-d29363e1cb51.jpg)
![15](https://user-images.githubusercontent.com/104763587/188575960-527780cd-ebda-4894-864d-272248e6160c.jpg)
![16](https://user-images.githubusercontent.com/104763587/188575965-4bfd50f9-78b9-414c-8b61-b63acb559552.jpg)
![17](https://user-images.githubusercontent.com/104763587/188575969-f789481f-eebf-463d-bd32-12d58c762180.jpg)
![18](https://user-images.githubusercontent.com/104763587/188575973-0d93cfe8-c8bb-454d-88e8-fe7f75df4cfc.jpg)
![19](https://user-images.githubusercontent.com/104763587/188575976-ba97626a-dda2-4e39-a35a-0b0e13a1bd6c.jpg)

code to output images like this:

```
Point2f vecLT2RT = PLAYER_CARD1_RT - PLAYER_CARD1_LT;
Point2f vecLB2RB = PLAYER_CARD1_RB - PLAYER_CARD1_LB;
for (int i = 1; i < 20; i++)
{
	Mat matShow = m_matShow.clone ();
	m_ptsPlayerOne[0] = PLAYER_CARD1_LT + vecLT2RT / 40 * i;//LT
	m_ptsPlayerOne[1] = PLAYER_CARD1_RT - vecLT2RT / 40 * i;//RT
	m_ptsPlayerOne[2] = PLAYER_CARD1_LB + vecLT2RT / 40 * i;//LB
	m_ptsPlayerOne[3] = PLAYER_CARD1_RB - vecLT2RT / 40 * i;//RB
	Mat matAction = getPerspectiveTransform (m_ptsTableTop, m_ptsPlayerOne);
	warpPerspective (m_matHoleCard, matShow, matAction, m_matTableTop.size (), 1, BORDER_TRANSPARENT);
	string s = format ("C:\\users\\user\\Downloads\\%d.jpg", i);
	imwrite (s, matShow (rect));
}
```
