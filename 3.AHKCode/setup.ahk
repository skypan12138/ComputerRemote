DetectHiddenWindows,On

;=================静音======================
^!z::
SoundGet, iSmute, , mute
if(iSmute="Off")
  SoundSet, +1, , mute
return

;=================音量++====================
^!u::
	Send,{Volume_Up} 
Send,{Volume_Up} 
Send,{Volume_Up} 
Send,{Volume_Up} 
	SoundPlay *16
return

;================音量--=====================
^!d::
	Send,{Volume_Down} 
Send,{Volume_Down}  
Send,{Volume_Down}  
Send,{Volume_Down}   
	SoundPlay *16
return



; ========================Ctrl+Alt+M 快捷打开/停止网易云音乐======================
^!m::
Process,Exist,cloudmusic.exe
if (%ErrorLevel% == 0){ ; 如果未运行，则启动
    Run "C:\Program Files (x86)\Netease\CloudMusic\cloudmusic.exe"
    WinWait,ahk_class OrpheusBrowserHost
    Sleep, 10000
    send, ^!p
} else { ; 已启动则停止
; 停止后，系统托盘中网易云的图标仍然还在，需要鼠标飘过才能消失
; 于是在系统托盘范围内寻找网易云图标的位置，并将鼠标移动过去，之后再移动回来
    Run, taskkill /PID %ErrorLevel% /F,,Hide
    Sleep, 500
    CoordMode,Pixel,Screen
    CoordMode,Mouse,Screen
    MouseGetPos, MX, MY
    WinGetPos, Task_X, Task_Y, Task_W, Task_H, ahk_class Shell_TrayWnd
    ImageSearch, X, Y, Task_X, Task_Y/3, A_ScreenWidth, A_ScreenHeight, cloud_music_icon.bmp
    MouseMove, X, Y
    MouseMove, MX, MY
    CoordMode,Pixel,Relative
    CoordMode,Mouse,Relative
}
return
;


; =======================测试一下图片搜索功能====================
^!q::


CoordMode,Pixel,Screen

ImageSearch, FoundX, FoundY,0, 0, A_ScreenWidth, A_ScreenHeight, Path


if (ErrorLevel = 2)
    MsgBox Could not conduct the search.
else if (ErrorLevel = 1)
    MsgBox Icon could not be found on the screen.
else
    MsgBox The icon was found at %FoundX%x%FoundY%.
    
FoundX:=FoundX
FoundY:=FoundY

CoordMode,Mouse,Screen

MouseMove, FoundX,FoundY

Click 




CoordMode,Pixel,Screen

ImageSearch, FoundX, FoundY,0, 0, A_ScreenWidth, A_ScreenHeight, C:\Users\Pansky\Desktop\Image\2.bmp


if (ErrorLevel = 2)
    MsgBox Could not conduct the search.
else if (ErrorLevel = 1)
    MsgBox Icon could not be found on the screen.
else
    MsgBox The icon was found at %FoundX%x%FoundY%.
    
FoundX:=FoundX
FoundY:=FoundY

CoordMode,Mouse,Screen

MouseMove, FoundX,FoundY

Click 




CoordMode,Pixel,Screen

ImageSearch, FoundX, FoundY,0, 0, A_ScreenWidth, A_ScreenHeight, C:\Users\Pansky\Desktop\Image\3.bmp


if (ErrorLevel = 2)
    MsgBox Could not conduct the search.
else if (ErrorLevel = 1)
    MsgBox Icon could not be found on the screen.
else
    MsgBox The icon was found at %FoundX%x%FoundY%.
    
FoundX:=FoundX
FoundY:=FoundY

CoordMode,Mouse,Screen

MouseMove, FoundX,FoundY

Click 




return 


; =======================Ctrl+Alt+W 打开微信====================

!w::
Process,Exist,wechat.exe
if (%ErrorLevel% == 0){ ; 如果未运行，则启动
    Run "C:\Program Files (x86)\Tencent\WeChat\WeChat.exe"
	WinWait, ahk_class WeChatLoginWndForPC
	Sleep, 500
	Send {Enter}
} else { 
	send, ^!w
}
return

;======================== Ctrl+Alt+O 打开Onenote================

^!o::
Run "C:\Users\Pansky\Desktop\OneNote for Windows 10"

return

!o::
Run "C:\Users\Pansky\Desktop\OneNote for Windows 10"

return

; =======================Ctrl+Alt+B 打开Bilibili===================

^!b::
Run "https://t.bilibili.com/?spm_id_from=333.1007.0.0"
Sleep, 2000
click, 1024 708
Sleep, 3000
click, 1024 708

return

; ========================Ctrl+Alt+L 打开Leetcode====================

^!l::
Run "https://leetcode-cn.com/problemset/all/"


return


!l::
Run "https://leetcode-cn.com/problemset/all/"
return

; ========================Ctrl+Alt+L 打开google====================

!g::
Run "https://www.google.com/"

return




; =======================Ctrl+Alt+G 打开关机=====================

^!g::
Shutdown, 1
return

; =======================Ctrl+Alt+V 打开VPN======================

^!v::

Run "C:\Program Files\ClashforWindows\Clash.for.Windows-0.15.2-win\Clash for Windows.exe"

WinGetActiveStats, Title, Width, Height, X, Y
EndX := X+Width
EndY := Y+Height

Sleep, 600
Click, 960 501

WinClose , Clash for Windows


return




;================鼠标右键+滚轮：调整音量大小=====================

~RButton & WheelUp::
	Send,{Volume_Up} 
	SoundPlay *16
return

~RButton & WheelDown::
	Send,{Volume_Down}  
	SoundPlay *16
return



;=====================替换字符=============================
:*:ddrk::www.ddrk.me


