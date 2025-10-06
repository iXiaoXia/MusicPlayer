# 这是一款音乐播放软件
## 界面
### 音乐列表未展开
<img width="1204" height="839" alt="image" src="https://github.com/user-attachments/assets/a844a6df-f33f-4431-9af3-748f8fd9b9ec" />

### 音乐列表展开
<img width="1204" height="839" alt="image" src="https://github.com/user-attachments/assets/42e4b058-b464-4982-952f-46a6f17bd6e5" />

## 功能介绍
1. 音频播放控制：播放/暂停/上一首/下一首，支持MP3格式
2. 播放模式：顺序播放、单曲循环、随机播放三种模式
3. 文件管理：支持文件夹选择，自动扫描并加载音频文件
4. 用户界面：自定义UI界面，支持列表展开/收起动画效果
5. 播放进度：可拖拽进度条控制播放位置，实时显示播放状态

## 技术亮点：
1. 使用QMediaPlayer和QMediaPlaylist实现音频播放和播放列表管理
2. 通过QPropertyAnimation实现流畅的UI动画效果
3. 采用信号槽机制实现组件间解耦，提高代码可维护性
4. 支持多种播放模式的状态管理和自动切换逻辑
