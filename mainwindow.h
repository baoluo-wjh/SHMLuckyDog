#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTimer>
#include <QLabel>
#include <QStack>
#include <QPair>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
    void playPauseMusic();
    void switchMusic();
    void reArrange();
    void connectButton();
    void getLotteries();
    void getBestFirstSecond();
    void getThird();
    void getFourth();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *musicPlayer;
    QTimer *timer;
    QVector<QLabel *> numberLabels;
    QVector<int> ordinaryNumbers;
    QVector<int> ordinaryLotteries;
    // PY
    QVector<int> specialNumbers;
    QVector<int> specialLotteries;
    // stack
    QStack<QPair<QVector<int>, QVector<int> > > backStack;
    QStack<QPair<QVector<int>, QVector<int> > > forthStack;
    QStack<QPair<QVector<QString>, QVector<QString> > > backStack2Show;
    QStack<QPair<QVector<QString>, QVector<QString> > > forthStack2Show;
};
#endif // MAINWINDOW_H
