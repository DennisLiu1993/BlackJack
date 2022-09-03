# BlackJack
Using MFC to create a BlackJack game platform

# What You Can Learn in This Project
1. double buffer drawing strategy in MFC
2. usage of CButtonST
3. OpenCV perspective tranform to show slanted cards
4. dynamically show number changes
5. how to design user interface

# Interface
![image](https://user-images.githubusercontent.com/104763587/188288051-1a41785f-8329-4320-9ca6-277cc1948388.png)

# Demo Gif
![image](https://github.com/DennisLiu1993/BlackJack/blob/main/BlackJack/BlackJack.gif)

# How to Build
1.	Download Visual Studio 2017 or newer versions
2.	Check on the option of "x86 and x64 version of C++ MFC"
3.	Install
4.	Open MatchTool.vcxproj
5.	Upgrade if it is required
6.	Open this project's property page
7.	Choose the SDK version you have in "General-Windows SDK Version"
8.	Choose the right toolset you have in "General-Platform Toolset" (for me, it is Visual Studio 2017 (v141))
9.	Go to "C/C++ - General", and type in "Additional Include Directories" for your own OpenCV include headers (e.g. C:\OpenCV3.1\opencv\build\include or C:\OpenCV4.0\opencv\build\include)
11.	Type in "Library Directories" for your own OpenCV's library path (the directory where your opencv_worldXX.lib locates)
12.	Go to "Linker-Input", and type in library name (e.g. C:\OpenCV3.1\build2017_worldx64\lib\opencv_world310d_vs2017_x64.lib)
13.	Make sure that your opencv_worldXX.dll and is in the same directory as .exe of this project (after building)
