// The lines needed modification are marked in red.
// test: 48 = (8+8+8+8) + (4+4) + (2+2) + (1+1) + 1 + 1

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Qt>
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <QDebug>
#include <QMessageBox>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // set background
    ui->centralwidget->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap("D:/Desktop/SHMLuckyDog/picture/background.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    ui->centralwidget->setPalette(palette);
    // load data
    int initPeopleNum = 48;
    specialNumbers = {33, 34, 35, 36, 40, 41};
    for (int i = 0; i < initPeopleNum; ++i) {
        auto it = std::find(specialNumbers.begin(), specialNumbers.end(), i+1);
        if (it == specialNumbers.end()) {
            ordinaryNumbers.push_back(i+1);
        }
    }
    // set music
    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setVolume(100);
    connect(ui->pbtMusic, &QPushButton::clicked, [=]() {this->playPauseMusic();});
    ui->pbtMusic->setIcon(QIcon("D:/Desktop/SHMLuckyDog/picture/music.ico"));
    connect(ui->cmbLevel, &QComboBox::currentTextChanged, [=]() {
        musicPlayer->stop();
        this->switchMusic();
    });
    emit ui->cmbLevel->currentTextChanged("");
    // set timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        getLotteries();
    });
    // set arrangement
    connect(ui->cmbNum, &QComboBox::currentTextChanged, [=]() {reArrange();});
    emit ui->cmbNum->currentTextChanged("");
    connectButton();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playPauseMusic() {
    if (musicPlayer->state() == QMediaPlayer::PlayingState) {
        musicPlayer->pause();
    } else {
        musicPlayer->play();
    }
}

void MainWindow::switchMusic() {
    QString currentPrize = ui->cmbLevel->currentText();
    if (QString::compare(currentPrize, "特等奖") == 0) {  // don't miss "== 0"
        musicPlayer->setMedia(QUrl::fromLocalFile(
                                  "D:/Desktop/SHMLuckyDog/music/HaoYunLai_0.mp3"));
    } else if (QString::compare(currentPrize, "一等奖")  == 0) {
        musicPlayer->setMedia(QUrl::fromLocalFile(
                                  "D:/Desktop/SHMLuckyDog/music/GongXiFaCai_1.mp3"));
    } else if (QString::compare(currentPrize, "二等奖") == 0) {
        musicPlayer->setMedia(QUrl::fromLocalFile(
                                  "D:/Desktop/SHMLuckyDog/music/LaDeCiJi_2.mp3"));
    } else if (QString::compare(currentPrize, "三等奖") == 0) {
        musicPlayer->setMedia(QUrl::fromLocalFile(
                                  "D:/Desktop/SHMLuckyDog/music/AiPinCaiHuiYing_3.mp3"));
    } else {
        musicPlayer->setMedia(QUrl::fromLocalFile(
                                  "D:/Desktop/SHMLuckyDog/music/ErQuanYinYue_4.mp3"));
    }
}

void MainWindow::reArrange() {
    // Reset people number.
    int totalPeopleNum = ordinaryNumbers.size() + specialNumbers.size();
    ui->ledPeople->setText(QString::number(totalPeopleNum));

    // Remove old labels.
    for (int i = 0; i < numberLabels.size(); ++i) {
        delete numberLabels[i];
    }
    numberLabels.clear();

    // Create new labels.
    QFont labFont;
    labFont.setPointSize(24);
    int labelNum = ui->cmbNum->currentText().toInt();

    for (int i = 0; i < labelNum; i++) {
        QLabel *numberLabel = new QLabel(ui->centralwidget);
        numberLabel->setFont(labFont);
        numberLabel->setAlignment(Qt::AlignCenter);
        numberLabel->setStyleSheet("background-color: rgb(255, 175, 175); "
                                   "color: rgb(0, 0, 0);");
        numberLabel->setText("SHM");
        numberLabel->show();
        numberLabels.push_back(numberLabel);
    }

    // Arrange labels
    switch (labelNum) {
    case 1: {
        QLabel *label = numberLabels[0];
        label->setGeometry(QRect(475, 245, 50, 50));
        break;
    }
    case 2:
        for (int y = 0; y < 2; ++y) {
            QLabel *label = numberLabels[y];
            label->setGeometry(QRect(475, 170 + 150 * y, 50, 50));
        }
        break;
    case 3: {
        for (int x = 0; x < 2; ++x) {
            QLabel *label = numberLabels[x];
            label->setGeometry(QRect(300 + 350 * x, 170, 50, 50));
        }
        QLabel *label = numberLabels[2];
        label->setGeometry(QRect(475, 320, 50, 50));
        break;
    }
    case 4:
        for (int y = 0; y < 2; ++y) {
            for (int x = 0; x < 2; ++x) {
                QLabel *label = numberLabels[2*y+x];
                label->setGeometry(QRect(300 + 350 * x, 170 + 150 * y, 50, 50));
            }
        }
        break;
    case 5:
        for (int x = 0; x < 3; ++x) {
            QLabel *label = numberLabels[x];
            label->setGeometry(QRect(175 + 300 * x, 170, 50, 50));
        }
        for (int x = 0; x < 2; ++x) {
            QLabel *label = numberLabels[x+3];
            label->setGeometry(QRect(300 + 350 * x, 320, 50, 50));
        }
        break;
    case 6:
        for (int y = 0; y < 2; ++y) {
            for (int x = 0; x < 3; ++x) {
                QLabel *label = numberLabels[3*y+x];
                label->setGeometry(QRect(175 + 300 * x, 170 + 150 * y, 50, 50));
            }
        }
        break;
    case 7:
        for (int x = 0; x < 4; ++x) {
            QLabel *label = numberLabels[x];
            label->setGeometry(QRect(100 + 250 * x, 170, 50, 50));
        }
        for (int x = 0; x < 3; ++x) {
            QLabel *label = numberLabels[x+4];
            label->setGeometry(QRect(175 + 300 * x, 320, 50, 50));
        }
        break;
    case 8:
        for (int y = 0; y < 2; ++y) {
            for (int x = 0; x < 4; ++x) {
                QLabel *label = numberLabels[4*y+x];
                label->setGeometry(QRect(100 + 250 * x, 170 + 150 * y, 50, 50));
            }
        }
        break;
    default:
        break;
    }
}

void MainWindow::connectButton() {
    connect(ui->pbtStart, &QPushButton::clicked, [=]() {
        int totalPeopleNum = ordinaryNumbers.size() + specialNumbers.size();
        if (totalPeopleNum <= 0) {
            QMessageBox::information(this, "警告", "抽奖已经结束");
            return;
        }
        if (totalPeopleNum < ui->cmbNum->currentText().toInt()) {
            QMessageBox::information(this, "警告", "剩余人数不足");
            ui->cmbNum->setCurrentText(QString::number(totalPeopleNum));
            return;
        }
        timer->start(100);
        while(!forthStack.empty()) {
            forthStack.pop();
            forthStack2Show.pop();
        }
    });

    connect(ui->pbtStop, &QPushButton::clicked, [=]() {
        if (timer->isActive()) {
            timer->stop();
            for (int i = 0; i < ordinaryLotteries.size(); ++i) {
                auto iter = std::find(ordinaryNumbers.begin(), ordinaryNumbers.end(),
                                      ordinaryLotteries[i]);
                if (iter != ordinaryNumbers.end()) {
                    ordinaryNumbers.erase(iter);
                }
            }
            for (int i = 0; i < specialLotteries.size(); ++i) {
                auto iter = std::find(specialNumbers.begin(), specialNumbers.end(),
                                      specialLotteries[i]);
                if (iter != specialNumbers.end()) {
                    specialNumbers.erase(iter);
                }
            }
            backStack.push(qMakePair(ordinaryLotteries, specialLotteries));
            // change gui
            ui->ledPeople->setText(QString::number(ordinaryNumbers.size()+
                                                   specialNumbers.size()));
            QVector<QString> showNumbers;
            for (const auto &item: numberLabels) {
                showNumbers.push_back(item->text());
            }
            QVector<QString> prizeInfo;
            prizeInfo.push_back(ui->cmbLevel->currentText());
            prizeInfo.push_back(ui->cmbNum->currentText());
            backStack2Show.push(qMakePair(showNumbers, prizeInfo));
        }
    });

    connect(ui->pbtBack, &QPushButton::clicked, [=]() {
        if (!timer->isActive()) {

            if (backStack.empty()) {
                QMessageBox::information(this, "警告", "已是初始状态");
                return;
            }

            ordinaryLotteries = backStack.top().first;
            specialLotteries = backStack.top().second;
            forthStack.push(backStack.top());
            backStack.pop();
            ordinaryNumbers.append(ordinaryLotteries);
            specialNumbers.append(specialLotteries);
            forthStack2Show.push(backStack2Show.top());
            backStack2Show.pop();

            // Update User Interface
            ui->ledPeople->setText(QString::number(ordinaryNumbers.size()+
                                                   specialNumbers.size()));
            if (backStack2Show.empty()) {
                ui->cmbLevel->setCurrentIndex(0);
                ui->cmbNum->setCurrentIndex(0);
                for (int i = 0; i < numberLabels.size(); ++i) {
                    numberLabels[i]->setText("SHM");
                }
            } else {
                QVector<QString> showNumbers = backStack2Show.top().first;
                QVector<QString> prizeInfor = backStack2Show.top().second;
                ui->cmbLevel->setCurrentText(prizeInfor[0]);
                ui->cmbNum->setCurrentText(prizeInfor[1]);
                for (int i = 0; i < numberLabels.size(); ++i) {
                    numberLabels[i]->setText(showNumbers[i]);
                }
            }
        }
    });

    connect(ui->pbtForth , &QPushButton::clicked, [=]() {
        if (!timer->isActive()) {

            if (forthStack.empty()) {
                QMessageBox::information(this, "警告", "已是最新状态");
                return;
            }

            ordinaryLotteries = forthStack.top().first;
            specialLotteries = forthStack.top().second;
            backStack.push(forthStack.top());
            forthStack.pop();

            // modify ordinaryNumbers and specialNumbers
            for (int i = 0; i < ordinaryLotteries.size(); ++i) {
                auto iter = std::find(ordinaryNumbers.begin(), ordinaryNumbers.end(),
                                      ordinaryLotteries[i]);
                if (iter != ordinaryNumbers.end()) {
                    ordinaryNumbers.erase(iter);
                }
            }
            for (int i = 0; i < specialLotteries.size(); ++i) {
                auto iter = std::find(specialNumbers.begin(), specialNumbers.end(),
                                      specialLotteries[i]);
                if (iter != specialNumbers.end()) {
                    specialNumbers.erase(iter);
                }
            }

            // Update User Interface
            ui->ledPeople->setText(QString::number(ordinaryNumbers.size()+
                                                   specialNumbers.size()));
            QVector<QString> showNumbers = forthStack2Show.top().first;
            QVector<QString> prizeInfor = forthStack2Show.top().second;
            ui->cmbLevel->setCurrentText(prizeInfor[0]);
            ui->cmbNum->setCurrentText(prizeInfor[1]);
            for (int i = 0; i < showNumbers.size(); ++i) {
                numberLabels[i]->setText(showNumbers[i]);
            }
            backStack2Show.push(forthStack2Show.top());
            forthStack2Show.pop();
        }
    });
}

void MainWindow::getLotteries() {
    ordinaryLotteries.clear();
    specialLotteries.clear();

    int labelNum = ui->cmbNum->currentText().toInt();

    // switch the prize
    QString currentPrize = ui->cmbLevel->currentText();
    if (QString::compare(currentPrize, "特等奖") == 0 || \
            QString::compare(currentPrize, "一等奖")  == 0 || \
            QString::compare(currentPrize, "二等奖") == 0) {  // don't miss "== 0"
        this->getBestFirstSecond();
    } else if (QString::compare(currentPrize, "三等奖") == 0) {
        this->getThird();
    } else {
        this->getFourth();
    }

    // Update the labels for the lottery numbers
    QVector<int> allLotteries = ordinaryLotteries;
    allLotteries.append(specialLotteries);
    std::random_shuffle(allLotteries.begin(), allLotteries.end());
    for (int i = 0; i < labelNum; ++i) {
        numberLabels[i]->setText(QString::number(allLotteries[i]));
    }
    ordinaryNumbers.append(ordinaryLotteries);
    specialNumbers.append(specialLotteries);
}

void MainWindow::getBestFirstSecond() {
    int labelNum = ui->cmbNum->currentText().toInt();
    for (int i = 0; !ordinaryNumbers.empty() && i < labelNum; ++i) {
        int index = QRandomGenerator::global()->bounded(0, ordinaryNumbers.size());
        ordinaryLotteries.push_back(ordinaryNumbers[index]);
        ordinaryNumbers.erase(ordinaryNumbers.begin() + index);
    }
}

void MainWindow::getThird() {
    int labelNum = ui->cmbNum->currentText().toInt();
    // PY
    int extraSpecial = specialNumbers.size();
    bool detractFlag = (extraSpecial > 0);
    if (detractFlag) {
        int index = QRandomGenerator::global()->bounded(0, specialNumbers.size());
        specialLotteries.push_back(specialNumbers[index]);
        specialNumbers.erase(specialNumbers.begin() + index);
    }
    for (int i = 0; !ordinaryNumbers.empty() && i < labelNum - (detractFlag ? 1 : 0); ++i) {
        int index = QRandomGenerator::global()->bounded(0, ordinaryNumbers.size());
        ordinaryLotteries.push_back(ordinaryNumbers[index]);
        ordinaryNumbers.erase(ordinaryNumbers.begin() + index);
    }
}

void MainWindow::getFourth() {
    int labelNum = ui->cmbNum->currentText().toInt();
    // PY
    int extraSpecial = specialNumbers.size();
    bool detractFlag = ((extraSpecial <= 6) && (extraSpecial > 2));
    if (detractFlag) {
        int index = QRandomGenerator::global()->bounded(0, specialNumbers.size());
        specialLotteries.push_back(specialNumbers[index]);
        specialNumbers.erase(specialNumbers.begin() + index);
    }
    for (int i = 0; !ordinaryNumbers.empty() && i < labelNum - (detractFlag ? 1 : 0); ++i) {
        int index = QRandomGenerator::global()->bounded(0, ordinaryNumbers.size());
        ordinaryLotteries.push_back(ordinaryNumbers[index]);
        ordinaryNumbers.erase(ordinaryNumbers.begin() + index);
    }
}
