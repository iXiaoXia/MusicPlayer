#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QTime>
#include <QStandardPaths>
#include <QWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_mode(ORDER_MODE),
    isShowMusicList(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*设置固定大小*/
    setFixedSize(1200,792);

    setBackground("../../Images/background4.png");
    /*设置标题*/
    setWindowTitle("MusicPlayer");
    setWindowIcon(QIcon("../../Icons/musicIcon.png"));

    initButton();

    m_player = new QMediaPlayer(this);

    //加载音乐列表
    m_musicDir = "E:\\Code\\QtProject\\MusicPlayer\\Musics";
    QDir dir(m_musicDir);
    QString defaultMusicPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    if(!dir.exists()){
        m_musicDir=defaultMusicPath;
    }
    //刷新文件夹
    refreshDir();
    // 1) 创建 m_player
    m_player = new QMediaPlayer(this);

    // 2) 创建 m_playlist 并加载目录（填充 QListWidget 和 playlist）
    m_playlist = new QMediaPlaylist(this);
    loadAppointMusicDir(m_musicDir);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_player->setPlaylist(m_playlist);
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged,
            this, &MainWindow::onPlaylistIndexChanged);
    // 3) 列表非空时再选中/设置媒体
    if (ui->musicList->count() > 0) {
        ui->musicList->setCurrentRow(0);
        ChoiceMusic();
    }
    //初始隐藏列表
    ui->musicList->hide();
    initPositionSlider();

}

MainWindow::~MainWindow()
{
    delete ui;
}
//设置按钮样式
void MainWindow::setButtonStyle(QPushButton *button, const QString &filename)
{
    button->setFixedSize(50,50);
    button->setIcon(QIcon(filename));
    button->setIconSize(QSize(button->width(),button->height()));
    button->setStyleSheet("background-color:transparent");
}
//初始化按钮
void MainWindow::initButton()
{
    setButtonStyle(ui->prevBtn,"../../Icons/prev.png");
    setButtonStyle(ui->playBtn,"../../Icons/play.png");
    setButtonStyle(ui->nextBtn,"../../Icons/next.png");
    setButtonStyle(ui->modeBtn,"../../Icons/order.png");
    setButtonStyle(ui->listBtn,"../../Icons/music.png");
    setButtonStyle(ui->dirBtn,"../../Icons/dir.png");

    ui->musicList->setStyleSheet("QListWidget{"
                                 "border:none;"
                                 "background-color:rgba(255,255,255,0.7);}");

    connect(ui->playBtn,&QPushButton::clicked,this,&MainWindow::handlePlaySlot);
    connect(ui->modeBtn,&QPushButton::clicked,this,&MainWindow::handleChangeModeSlot);
    connect(ui->nextBtn,&QPushButton::clicked,this,&MainWindow::handleNextSlot);
    connect(ui->prevBtn,&QPushButton::clicked,this,&MainWindow::handlePrevSlot);
    connect(ui->listBtn,&QPushButton::clicked,this,&MainWindow::handleMusicListSlot);
    connect(ui->dirBtn,&QPushButton::clicked,this,&MainWindow::handleChooseDirSlot);
}

//播放音乐
void MainWindow::handlePlaySlot()
{
    if(m_player->state() == QMediaPlayer::PlayingState){
        m_player->pause();
        ui->playBtn->setIcon(QIcon("../../Icons/play.png"));
    }else{
        m_player->play();
        ui->playBtn->setIcon(QIcon("../../Icons/stop.png"));
    }
}


//双击播放音乐
void MainWindow::handleMusicClickSlot()
{
    ChoiceMusic();
    playMusic();
}
//切换模式
void MainWindow::handleChangeModeSlot()
{
    if(m_mode==ORDER_MODE){
        m_mode=CIRCLE_MODE;
        ui->modeBtn->setIcon(QIcon("../../Icons/circle.png"));
    }else if(m_mode==CIRCLE_MODE){
        m_mode=RANDOM_MODE;
        ui->modeBtn->setIcon(QIcon("../../Icons/random.png"));
    }else if(m_mode==RANDOM_MODE){
        m_mode=ORDER_MODE;
        ui->modeBtn->setIcon(QIcon("../../Icons/order.png"));
    }

    if (m_mode == ORDER_MODE) {
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    } else if (m_mode == CIRCLE_MODE) {
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); // 列表循环
    } else if (m_mode == RANDOM_MODE) {
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
}
//下一首
void MainWindow::handleNextSlot()
{
    int currentRow = ui->musicList->currentRow();
    int next=0;
    next = (currentRow+1)%ui->musicList->count();
    ui->musicList->setCurrentRow(next);
    m_playlist->next();
    ChoiceMusic();
    playMusic();
}
//上一首
void MainWindow::handlePrevSlot()
{
    int currentRow = ui->musicList->currentRow();
    int next=0;
    next = (currentRow-1+ui->musicList->count())%ui->musicList->count();
    ui->musicList->setCurrentRow(next);
    m_playlist->previous();
    ChoiceMusic();
    playMusic();
}
//音乐列表状态切换
void MainWindow::handleMusicListSlot()
{
    if(isShowMusicList){
        isShowMusicList=false;
        hideAnimation(ui->musicList);
        ui->musicList->hide();
    }else{
        isShowMusicList=true;
        ui->musicList->show();
        //动画方式出现，渐入效果
        showAnimation(ui->musicList);
    }
}
//选择文件夹
void MainWindow::handleChooseDirSlot()
{
    QFileDialog fileDialog;
    QString strTargetFile = fileDialog.getExistingDirectory(this, tr("选择保存路径"), m_musicDir);
    if(strTargetFile.isEmpty())
        return;
    m_musicDir = strTargetFile;
    refreshDir();
    loadAppointMusicDir(m_musicDir);
}
//设置界面背景
void MainWindow::setBackground(const QString & filename)
{
    /*创建照片*/
    QPixmap pixmap(filename);
    QSize windowsize = this->size();
    QPixmap scalePixmap = pixmap.scaled(windowsize);

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(scalePixmap));
    this->setPalette(palette);
    connect(ui->musicList,&QListWidget::itemDoubleClicked,this,&MainWindow::handleMusicClickSlot);

}

//加载文件夹音乐
void MainWindow::loadAppointMusicDir(const QString &filepath)
{
    ui->musicList->clear();

    QDir dir(filepath);
    if(dir.exists()==false){
        QMessageBox::warning(this,"文件夹","文件夹路径不存在！");
        return;
    }
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    for(auto element : fileList){
        if(element.suffix()=="mp3"){
            ui->musicList->addItem(element.baseName());
        }
    }
    // 把当前 ui->musicList 的歌曲灌入到 m_playlist
    m_playlist->clear();
    for (int i = 0; i < ui->musicList->count(); ++i) {
        const QString name = ui->musicList->item(i)->text();
        m_playlist->addMedia(QUrl::fromLocalFile(mergeMP3Path(name)));
    }
    m_playlist->setCurrentIndex(0);
    return;
}
//合并音乐路径
QString MainWindow::mergeMP3Path(QString MusicName)
{
    return m_musicDir+"\\"+MusicName+".mp3";
}
//选择音乐
void MainWindow::ChoiceMusic()
{
    if (ui->musicList->count() == 0 || ui->musicList->currentItem() == nullptr) return;
    int row = ui->musicList->currentRow();
    if (row >= 0) m_playlist->setCurrentIndex(row%ui->musicList->count());
}
//显示动画
void MainWindow::showAnimation(QWidget *window)
{
    QPropertyAnimation animation(window,"pos");
    animation.setDuration(500);
    //起始坐标
    animation.setStartValue(QPoint(this->width(),0));
    //结束坐标
    animation.setEndValue(QPoint(this->width()-ui->musicList->width(),0));

    animation.start();

    QEventLoop loop;

    connect(&animation,&QPropertyAnimation::finished,&loop,&QEventLoop::quit);
    loop.exec();
}
//隐藏动画
void MainWindow::hideAnimation(QWidget *window)
{
    QPropertyAnimation animation(window,"pos");
    animation.setDuration(500);
    //起始坐标
    animation.setStartValue(QPoint(this->width()-ui->musicList->width(),0));
    //结束坐标
    animation.setEndValue(QPoint(this->width(),0));

    animation.start();

    QEventLoop loop;

    connect(&animation,&QPropertyAnimation::finished,&loop,&QEventLoop::quit);
    loop.exec();
}
void MainWindow::onDurationChanged(qint64 ms)
{
    // 空媒体或未知时长
    if (ms <= 0) {
        ui->positionSlider->setRange(0, 0);
        ui->positionSlider->setValue(0);
        ui->positionSlider->setEnabled(false);
        return;
    }
    ui->positionSlider->setEnabled(true);
    ui->positionSlider->setRange(0, static_cast<int>(ms));
    ui->totalTimeLabel->setText(formatMs(ms));
}

void MainWindow::onPositionChanged(qint64 ms)
{
    // 拖动中不回写，避免“抖动”
    if (!m_seeking) {
        ui->positionSlider->setValue(static_cast<int>(ms));
    }
    ui->currentTimeLabel->setText(formatMs(ms));
}

void MainWindow::onSeekStart()
{
    m_seeking = true;
}

void MainWindow::onSeekPreview(int value)
{
    // 可选：预览时间显示
    ui->currentTimeLabel->setText(formatMs(value));
}

void MainWindow::onSeekCommit()
{
    const int target = ui->positionSlider->value();
    m_player->setPosition(target);
    m_seeking = false;
}
//Index变化时，musicList选中也要变化
void MainWindow::onPlaylistIndexChanged(int index)
{
    if (index < 0 || index >= ui->musicList->count()) return;
    ui->musicList->setCurrentRow(index);
}
//刷新文件夹信息
void MainWindow::refreshDir()
{
    ui->dirLineEdit->setText(m_musicDir);
}

void MainWindow::playMusic()
{
    m_player->play();
    ui->playBtn->setIcon(QIcon("../../Icons/stop.png"));
}

void MainWindow::initPositionSlider()
{
    // 连接 QMediaPlayer 信号
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &MainWindow::onDurationChanged);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &MainWindow::onPositionChanged);

    // 连接进度条交互信号
    connect(ui->positionSlider, &QSlider::sliderPressed,
            this, &MainWindow::onSeekStart);
    connect(ui->positionSlider, &QSlider::sliderReleased,
            this, &MainWindow::onSeekCommit);
    connect(ui->positionSlider, &QSlider::sliderMoved,
            this, &MainWindow::onSeekPreview);
}

QString MainWindow::formatMs(qint64 ms) const
{
    QTime t(0, 0);
    t = t.addMSecs(static_cast<int>(ms));
    // 超过一小时显示 hh:mm:ss，否则 mm:ss
    return ms >= 3600000 ? t.toString("hh:mm:ss")
                         : t.toString("mm:ss");
}



