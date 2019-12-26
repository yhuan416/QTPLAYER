# 基于mplayer的嵌入式音视频播放器

## 基本功能：
	基于mplayer实现了音频视频的播放
	视频播放：
		1. 进度条拖动
		2. 音量调节
		3. 切换上一首/下一首
		4. 全屏播放
	
	音频播放：
		1. 进度条拖动
		2. 音量调节
		3. 切换歌曲
		4. 歌词进度匹配
		
## 说明
	程序启动后会自动检索程序所在目录下的video文件夹和music文件夹(可以在musicplayer.cpph和videoplayer.cpp中修改),  
	并将其中的素材文件添加进播放列表  
	
	ps:当前版本,如果不能够正常加载播放素材,在运行时回出现段错误
	
	点击两个按钮分别启动视频/音频播放界面  
	
	主页下面还有一个调节背景颜色的按钮，点击后进入调色板，  
	选择指定颜色后播放器的背景会转变为指定的颜色  