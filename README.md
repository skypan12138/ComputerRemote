# CompterRemote

>本项目是一个电脑遥控器，将电视上的某些常用的遥控功能移植到电脑上。灵感源于某次不想下床关机的夜晚。于是基本的功能包括但不限于：关机、暂停/暂停、调节音量、静音、自动打开网易云、自动打开微信、自动打开B站并播放视频、自动打开虎牙并播放视频等基础功能。第一个正式写的小项目，希望大家可以一起完善相关功能。
>
>顺便能总结一下学习的过程供大家参考。
>
>视频介绍：[DIY电脑遥控器，把电脑变成电视！_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV17F411u7rH?spm_id_from=333.999.0.0)

![Display.png](https://github.com/skypan12138/ComputerRemote/blob/main/5.Docs/Image/Display.png?raw=true)



# 1、流程思路说明

1. 树莓派接红外传感器，接受红外遥控信号。* 可以用stm32进行首发
2. 树莓派通过USB转TTL向主机发送串口字符。
3. QT驱动收到串口字符，判断内容，模拟快捷键按下和松开。
4. AHK通过接受快捷键实现对应的功能操作。

* AHK的编程思路有些奇怪，所以部分功能实现的不是很稳定
* 可以使用python里的自动化，后续应该会在QT里嵌入这一部分。
* 会介绍一些AHK的使用小技巧，希望对大家有帮助
* QT驱动和AHK需要设置开机自启，树莓派可以7*24小时工作，当然不限于只是操作，我还在上面部署了一个人脸识别和轻量级服务器，后续有机会应该也会分享出来。



# 2、项目文件说明

## 2.1、Raspberry

就一个用于接受红外信号和按键信号的py程序。

TODO：添加人脸识别自动打开微信的程序



## 2.2、 QTCode

主要是写了一个常驻后台的驱动。

拿以前的串口收发程序改的。主要的功能就是，接收到串口指令然后模拟按键。

后续应该嵌入一些py的内容，直接走py的自动化路径，AHK的编程实在太迷离了。

一些参考的博客：

* QT最小化窗口到托盘：[(55条消息) Qt程序窗口关闭不退出而最小化到托盘的方法_jianwang16的博客-CSDN博客](https://blog.csdn.net/u010058695/article/details/100996029)
* QT模拟键盘快捷键输出：[(55条消息) QT按钮模拟键盘输出，模拟组合键输出_世纪末的小黑的博客-CSDN博客_qt模拟键盘输入](https://blog.csdn.net/qq_39750907/article/details/107205045)
* QT键盘码：[Virtual-Key Codes (Winuser.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)
* QT调用py文件：[(55条消息) Qt调用python_芒果黑的博客-CSDN博客_qt调用python代码](https://blog.csdn.net/a137748099/article/details/119217197?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_title~default-0.pc_relevant_antiscanv2&spm=1001.2101.3001.4242.1&utm_relevant_index=3)







