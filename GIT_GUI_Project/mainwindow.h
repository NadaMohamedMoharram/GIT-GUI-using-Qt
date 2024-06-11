#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QLabel>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readProcessError();
    void readProcessOutput();

private slots:
    void on_createRepoButton_clicked();

    void on_BrowseButton_clicked();

    void on_CloneButton_clicked();

    void on_commitButton_clicked();

    void on_addAllButton_clicked();

    void on_statusButton_clicked();

    void on_push_Button_clicked();

    void on_creatBranchButton_clicked();

    void on_pullButton_clicked();

    void on_git_branch_Button_clicked();

    void on_Switch_branch_Button_clicked();

    void on_Merge_Button_clicked();

    void on_Log_Button_clicked();


    void on_GetStarted_PB_clicked();

private:
    Ui::MainWindow *ui;
       QProcess *gitProcess;
    QString RemoteURL;
        QLabel *label;
};
#endif // MAINWINDOW_H
