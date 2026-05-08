#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrl>

class QAudioOutput;
class QMediaPlayer;

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void on_fileBtn_clicked();

    void on_palyBtn_clicked();

    void on_lastBtn_clicked();

    void on_nextBtn_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QList<QUrl> playList;
    int curPlayIndex = 0;
    QAudioOutput* audioOutput;
    QMediaPlayer* mediaPlayer;
};
#endif // WIDGET_H
