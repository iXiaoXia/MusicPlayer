#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMediaPlayer>
#include <QListWidgetItem>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
enum PLAYMODE
{
    ORDER_MODE,
    RANDOM_MODE,
    CIRCLE_MODE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    /*处理播放暂停*/
    void handlePlaySlot();
    void handleMusicClickSlot();
    void handleChangeModeSlot();
    void handleNextSlot();
    void handlePrevSlot();
    void handleMusicListSlot();
    void handleChooseDirSlot();
    void onDurationChanged(qint64 ms);
    void onPositionChanged(qint64 ms);
    void onSeekStart();
    void onSeekPreview(int value);
    void onSeekCommit();
    void onPlaylistIndexChanged(int index);
private:
    void setBackground(const QString & filename);
    void setButtonStyle(QPushButton * button,const QString & filename);
    void initButton();
    void loadAppointMusicDir(const QString & filepath);

    QString mergeMP3Path(QString MusicName);
    void ChoiceMusic();
    //显示动画
    void showAnimation(QWidget * window);

    //隐藏动画
    void hideAnimation(QWidget * window);

    //刷新文件夹信息
    void refreshDir();

    //直接播放音乐
    void playMusic();

    //初始化进度条
    void initPositionSlider();

    QString formatMs(qint64 ms) const;

private:
    Ui::MainWindow *ui;
    //音乐播放器
    QMediaPlayer * m_player;
    //音乐文件夹
    QString m_musicDir;

    //当前播放模式
    PLAYMODE  m_mode;
    //是否显示音乐列表
    bool isShowMusicList;

    //音乐列表
    QMediaPlaylist *m_playlist;

    bool m_seeking = false;
};
#endif // MAINWINDOW_H
