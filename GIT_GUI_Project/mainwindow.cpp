#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDir>
#include<QDebug>
#include <QMessageBox>
#include<QFileDialog>
#include<QTabBar>
#include<QInputDialog>

// Constructor for MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  // Initializing the parent class QMainWindow with the given parent
    , ui(new Ui::MainWindow)  // Allocating memory for the user interface
    , gitProcess(new QProcess(this))  // Creating a new QProcess object for handling Git commands
{
    ui->setupUi(this);  // Setting up the user interface

    // Connecting signals and slots for handling Git process outputs and errors
    connect(gitProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::readProcessOutput);
    connect(gitProcess, &QProcess::readyReadStandardError, this, &MainWindow::readProcessError);

    // Loading and setting images to labels in the UI
    QPixmap pix("D:\\ITIDA_Scholarship\\Task_Qt\\photo.jpeg");
    ui->label_image1->setPixmap(pix.scaled(800,800,Qt::KeepAspectRatio));  // Setting the first image

    QPixmap pix2("D:\\ITIDA_Scholarship\\Task_Qt\\hqdefault.jpg");
    ui->label_image3->setPixmap(pix2.scaled(200,200,Qt::KeepAspectRatio));  // Setting the second image

    QPixmap pix3("D:\\ITIDA_Scholarship\\Task_Qt\\Capture3.PNG");
    ui->label_image4->setPixmap(pix3.scaled(300,200,Qt::KeepAspectRatio));  // Setting the third image

    // Setting the initial tab and styling the tab bar
    ui->tabWidget_gui->setCurrentIndex(1);
    ui->tabWidget_gui->tabBar()->setStyleSheet("background-color: red;");
}

// Destructor for MainWindow
MainWindow::~MainWindow()
{
    delete ui;  // Deleting the UI object to free up memory
}

// Slot for creating a new Git repository
void MainWindow::on_createRepoButton_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // Getting the repository path from the input field

    QDir dir(path);  // Creating a QDir object with the specified path
    if (dir.exists()) {  // Checking if the directory exists
        gitProcess->setWorkingDirectory(path);  // Setting the working directory for the Git process

        gitProcess->start("git", QStringList() << "init");  // Starting the Git init command

        if (!gitProcess->waitForStarted()) {  // Waiting for the process to start
            QMessageBox::critical(this, "Error", "Failed to start git process.");
            return;
        }

        if (!gitProcess->waitForFinished()) {  // Waiting for the process to finish
            QMessageBox::critical(this, "Error", "Git process did not finish.");
            return;
        }

        QMessageBox::information(this, "Success", "Initialized empty Git repository in " + path);
    } else {
        QMessageBox::warning(this, "Warning", "Please enter a valid path.");
    }
}

// Slot for browsing and selecting a directory
void MainWindow::on_BrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {  // Checking if a directory was selected
        ui->lineEdit_RepoPath->setText(dir);  // Setting the selected directory to the input field
    }
}

// Slot for cloning a Git repository
void MainWindow::on_CloneButton_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // Getting the repository path from the input field
    QString repoUrl = ui->lineEdit_clone->text();  // Getting the repository URL from the input field

    if (path.isEmpty() || repoUrl.isEmpty()) {  // Checking if the path or URL is empty
        QMessageBox::warning(this, "Warning", "Please enter a valid path and repository URL.");
        return;
    }

    QDir dir(path);  // Creating a QDir object with the specified path
    if (!dir.exists()) {  // Checking if the directory exists
        QMessageBox::critical(this, "Error", "The specified path does not exist.");
        return;
    }

    gitProcess->setWorkingDirectory(path);  // Setting the working directory for the Git process

    QString command = "git";
    QStringList arguments;
    arguments << "clone" << repoUrl;  // Preparing the arguments for the Git clone command

    gitProcess->start(command, arguments);  // Starting the Git clone command

    if (!gitProcess->waitForStarted()) {  // Waiting for the process to start
        QMessageBox::critical(this, "Error", "Failed to start git process.");
        return;
    }

    if (!gitProcess->waitForFinished()) {  // Waiting for the process to finish
        QMessageBox::critical(this, "Error", "Git process did not finish.");
        return;
    }

    if (gitProcess->exitStatus() == QProcess::CrashExit) {  // Checking if the process crashed
        QMessageBox::critical(this, "Error", "Git process crashed.");
        return;
    }

    if (gitProcess->exitCode() != 0) {  // Checking if the process exited with an error
        QString errorOutput = gitProcess->readAllStandardError();  // Reading the error output
        QMessageBox::critical(this, "Error", "Git process failed with exit code " + QString::number(gitProcess->exitCode()) + ": " + errorOutput);
        return;
    }

    QMessageBox::information(this, "Success", "Cloned Git repository from " + repoUrl);
}

// Slot for reading standard output from the Git process
void MainWindow::readProcessOutput()
{
    ui->textEdit_1->clear();  // Clearing the text edit

    QByteArray output = gitProcess->readAllStandardOutput();  // Reading the standard output
    ui->textEdit_1->setText(output);  // Setting the output to the text edit
}

// Slot for reading standard error from the Git process
void MainWindow::readProcessError()
{
    ui->textEdit_1->clear();  // Clearing the text edit
    QByteArray errorOutput = gitProcess->readAllStandardError();  // Reading the standard error
    ui->textEdit_1->setText(errorOutput);  // Setting the error output to the text edit
}

void MainWindow::on_commitButton_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // Retrieve the repository path from the input field

    bool ok;
    QString commitMessage = QInputDialog::getText(this, tr("Commit Message"),
                                                  tr("Enter your commit message:"), QLineEdit::Normal,
                                                  "", &ok);  // Prompt the user to enter a commit message
    if (!ok || commitMessage.isEmpty())  // Check if the commit message was provided
    {
        QMessageBox::critical(this, "Error", "Commit message is required");  // Show an error message if no commit message was provided
        return;  // Exit the function
    }

    if (path.isEmpty() || commitMessage.isEmpty()) {  // Check if the path or commit message is empty
        QMessageBox::warning(this, "Warning", "Please enter a valid repository path and commit message.");  // Show a warning message
        return;  // Exit the function
    }

    QDir dir(path);  // Create a QDir object with the specified path
    if (!dir.exists()) {  // Check if the directory exists
        QMessageBox::warning(this, "Warning", "Specified path does not exist.");  // Show a warning message
        return;  // Exit the function
    }

    gitProcess->setWorkingDirectory(path);  // Set the working directory for the Git process
    gitProcess->start("git", QStringList() << "commit" << "-m" << commitMessage);  // Start the Git commit command with the specified commit message

    if (!gitProcess->waitForStarted()) {  // Wait for the process to start
        QMessageBox::critical(this, "Error", "Failed to start git commit process.");  // Show an error message if the process failed to start
        return;  // Exit the function
    }

    if (!gitProcess->waitForFinished()) {  // Wait for the process to finish
        QMessageBox::critical(this, "Error", "Git commit process did not finish.");  // Show an error message if the process did not finish
        return;  // Exit the function
    }

    if (gitProcess->exitCode() != 0) {  // Check if the process exited with an error
        QString errorOutput = gitProcess->readAllStandardError();  // Read the error output
        QMessageBox::critical(this, "Error", "Git commit process failed: " + errorOutput);  // Show an error message with the error output
        return;  // Exit the function
    }

    QMessageBox::information(this, "Success", "Committed changes to the repository.");  // Show a success message
}

void MainWindow::on_addAllButton_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // Retrieve the repository path from the input field

    if (path.isEmpty()) {  // Check if the path is empty
        QMessageBox::warning(this, "Warning", "Please enter a valid repository path.");  // Show a warning message
        return;  // Exit the function
    }

    QDir dir(path);  // Create a QDir object with the specified path
    if (!dir.exists()) {  // Check if the directory exists
        QMessageBox::warning(this, "Warning", "Specified path does not exist.");  // Show a warning message
        return;  // Exit the function
    }

    gitProcess->setWorkingDirectory(path);  // Set the working directory for the Git process
    gitProcess->start("git", QStringList() << "add" << ".");  // Start the Git add command to stage all changes

    if (!gitProcess->waitForStarted()) {  // Wait for the process to start
        QMessageBox::critical(this, "Error", "Failed to start git add process.");  // Show an error message if the process failed to start
        return;  // Exit the function
    }

    if (!gitProcess->waitForFinished()) {  // Wait for the process to finish
        QMessageBox::critical(this, "Error", "Git add process did not finish.");  // Show an error message if the process did not finish
        return;  // Exit the function
    }

    QMessageBox::information(this, "Success", "Added all files to the staging area.");  // Show a success message
}

void MainWindow::on_statusButton_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // Retrieve the repository path from the input field

    if (path.isEmpty()) {  // Check if the path is empty
        QMessageBox::warning(this, "Warning", "Please enter a valid repository path.");  // Show a warning message
        return;  // Exit the function
    }

    QDir dir(path);  // Create a QDir object with the specified path
    if (!dir.exists()) {  // Check if the directory exists
        QMessageBox::warning(this, "Warning", "Specified path does not exist.");  // Show a warning message
        return;  // Exit the function
    }

    gitProcess->setWorkingDirectory(path);  // Set the working directory for the Git process
    gitProcess->start("git", QStringList() << "status");  // Start the Git status command to check the repository status

    if (!gitProcess->waitForStarted()) {  // Wait for the process to start
        QMessageBox::critical(this, "Error", "Failed to start git status process.");  // Show an error message if the process failed to start
        return;  // Exit the function
    }

    if (!gitProcess->waitForFinished()) {  // Wait for the process to finish
        QMessageBox::critical(this, "Error", "Git status process did not finish.");  // Show an error message if the process did not finish
        return;  // Exit the function
    }

    QMessageBox::information(this, "Success", "Git status command executed.");  // Show a success message
}

void MainWindow::on_push_Button_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // The path to the local repository
    QString remote = ui->lineEdit_clone->text();  // The remote repository name

    bool ok;
    QString branch = QInputDialog::getText(this, tr("Push Request"),
                                                  tr("Enter branch name to push into:"), QLineEdit::Normal,
                                                  "", &ok);
    if (!ok || branch.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Branch name to push into is required");
        return;
    }


    if (path.isEmpty() || remote.isEmpty() || branch.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a valid repository path, remote name, and branch name.");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Warning", "Specified path does not exist.");
        return;
    }

    gitProcess->setWorkingDirectory(path);
    gitProcess->start("git", QStringList() << "push" << remote << branch);

    if (!gitProcess->waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start git push process.");
        return;
    }

    if (!gitProcess->waitForFinished()) {
        QMessageBox::critical(this, "Error", "Git push process did not finish.");
        return;
    }

    if (gitProcess->exitCode() != 0) {
        QString errorOutput = gitProcess->readAllStandardError();
        QMessageBox::critical(this, "Error", "Git push process failed: " + errorOutput);
        return;
    }

    QMessageBox::information(this, "Success", "Pushed changes to the remote repository.");
}


void MainWindow::on_creatBranchButton_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // The path to the local repository
    QString remote = ui->lineEdit_clone->text();  // The remote repository name

    bool ok;
    QString branchName = QInputDialog::getText(this, tr("Create Branch Request"),
                                           tr("Enter branch name to create:"), QLineEdit::Normal,
                                           "", &ok);
    if (!ok || branchName.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Branch name is required");
        return;
    }

    if (path.isEmpty() || branchName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a valid repository path and branch name.");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        qInfo() << "Specified path does not exist.";
        QMessageBox::warning(this, "Warning", "Specified path does not exist.");
        return;
    }

    gitProcess->setWorkingDirectory(path);
    gitProcess->start("git", QStringList() << "branch" << branchName);

    if (!gitProcess->waitForStarted()) {
        qInfo() << "Failed to start git branch process.";
        QMessageBox::critical(this, "Error", "Failed to start git branch process.");
        return;
    }

    if (!gitProcess->waitForFinished()) {
        qInfo() << "Git branch process did not finish.";
        QMessageBox::critical(this, "Error", "Git branch process did not finish.");
        return;
    }

    if (gitProcess->exitCode() != 0) {
        QString errorOutput = gitProcess->readAllStandardError();
        QMessageBox::critical(this, "Error", "Git branch process failed: " + errorOutput);
        return;
    }

    // Push the branch to the remote repository
    gitProcess->start("git", QStringList() << "push" << remote << branchName);

    if (!gitProcess->waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start git push process.");
        return;
    }

    if (!gitProcess->waitForFinished()) {
        QMessageBox::critical(this, "Error", "Git push process did not finish.");
        return;
    }

    if (gitProcess->exitCode() != 0) {
        QString errorOutput = gitProcess->readAllStandardError();
        QMessageBox::critical(this, "Error", "Git push process failed: " + errorOutput);
        return;
    }

    QMessageBox::information(this, "Success", "Created and pushed new branch: " + branchName + " to remote repository.");
}


void MainWindow::on_pullButton_clicked()
{
    QString path = ui->lineEdit_RepoPath->text();  // The path to the local repository
    QString remote = ui->lineEdit_clone->text();  // The remote repository name


    bool ok;
    QString branch = QInputDialog::getText(this, tr("Pull Request"),
                                               tr("Enter branch name to pull from:"), QLineEdit::Normal,
                                               "", &ok);
    if (!ok || branch.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Branch name to pull from is required");
        return;
    }

    if (path.isEmpty() || remote.isEmpty() || branch.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a valid repository path, remote name, and branch name.");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Warning", "Specified path does not exist.");
        return;
    }

    gitProcess->setWorkingDirectory(path);
    gitProcess->start("git", QStringList() << "pull" << remote << branch);

    if (!gitProcess->waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start git pull process.");
        return;
    }

    if (!gitProcess->waitForFinished()) {
        QMessageBox::critical(this, "Error", "Git pull process did not finish.");
        return;
    }

    if (gitProcess->exitCode() != 0) {
        QString errorOutput = gitProcess->readAllStandardError();
        QMessageBox::critical(this, "Error", "Git pull process failed: " + errorOutput);
        return;
    }

    QMessageBox::information(this, "Success", "Pulled changes from the remote repository.");
}


void MainWindow::on_git_branch_Button_clicked()
{
    {
        QString path = ui->lineEdit_RepoPath->text();  // The path to the local repository

        if (path.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please enter a valid repository path.");
            return;
        }

        QDir dir(path);
        if (!dir.exists()) {
            QMessageBox::warning(this, "Warning", "Specified path does not exist.");
            return;
        }

        gitProcess->setWorkingDirectory(path);
        gitProcess->start("git", QStringList() << "branch");

        if (!gitProcess->waitForStarted()) {
            QMessageBox::critical(this, "Error", "Failed to start git branch process.");
            return;
        }

        if (!gitProcess->waitForFinished()) {
            QMessageBox::critical(this, "Error", "Git branch process did not finish.");
            return;
        }

        if (gitProcess->exitCode() != 0) {
            QString errorOutput = gitProcess->readAllStandardError();
            QMessageBox::critical(this, "Error", "Git branch process failed: " + errorOutput);
            return;
        }

        QByteArray branchOutput_Byte = gitProcess->readAllStandardOutput();

    }

}


void MainWindow::on_Switch_branch_Button_clicked()
{
    {
        QString path = ui->lineEdit_RepoPath->text();  // The path to the local repository

        bool ok;
        QString branch = QInputDialog::getText(this, tr("Switch Branch"),
                                               tr("Enter branch name to switch to:"), QLineEdit::Normal,
                                               "", &ok);
        if (!ok || branch.isEmpty()) {
            QMessageBox::critical(this, "Error", "Branch name is required to switch.");
            return;
        }

        if (path.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please enter a valid repository path.");
            return;
        }

        QDir dir(path);
        if (!dir.exists()) {
            QMessageBox::warning(this, "Warning", "Specified path does not exist.");
            return;
        }

        gitProcess->setWorkingDirectory(path);
        gitProcess->start("git", QStringList() << "checkout" << branch);

        if (!gitProcess->waitForStarted()) {
            QMessageBox::critical(this, "Error", "Failed to start git checkout process.");
            return;
        }

        if (!gitProcess->waitForFinished()) {
            QMessageBox::critical(this, "Error", "Git checkout process did not finish.");
            return;
        }

        if (gitProcess->exitCode() != 0) {
            QString errorOutput = gitProcess->readAllStandardError();
            QMessageBox::critical(this, "Error", "Git checkout process failed: " + errorOutput);
            return;
        }

        QMessageBox::information(this, "Success", "Switched to branch: " + branch);
    }

}


void MainWindow::on_Merge_Button_clicked()
{
    {
        QString path = ui->lineEdit_RepoPath->text();  // The path to the local repository

        bool ok;
        QString branch = QInputDialog::getText(this, tr("Merge Branch"),
                                               tr("Enter branch name to merge into current branch:"), QLineEdit::Normal,
                                               "", &ok);
        if (!ok || branch.isEmpty()) {
            QMessageBox::critical(this, "Error", "Branch name is required to merge.");
            return;
        }

        if (path.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please enter a valid repository path.");
            return;
        }

        QDir dir(path);
        if (!dir.exists()) {
            QMessageBox::warning(this, "Warning", "Specified path does not exist.");
            return;
        }

        gitProcess->setWorkingDirectory(path);
        gitProcess->start("git", QStringList() << "merge" << branch);

        if (!gitProcess->waitForStarted()) {
            QMessageBox::critical(this, "Error", "Failed to start git merge process.");
            return;
        }

        if (!gitProcess->waitForFinished()) {
            QMessageBox::critical(this, "Error", "Git merge process did not finish.");
            return;
        }

        if (gitProcess->exitCode() != 0) {
            QString errorOutput = gitProcess->readAllStandardError();
            QMessageBox::critical(this, "Error", "Git merge process failed: " + errorOutput);
            return;
        }

        QMessageBox::information(this, "Success", "Merged branch: " + branch + " into the current branch.");
    }

}


void MainWindow::on_Log_Button_clicked()
{
    {
        QString path = ui->lineEdit_RepoPath->text();  // The path to the local repository

        if (path.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please enter a valid repository path.");
            return;
        }

        QDir dir(path);
        if (!dir.exists()) {
            QMessageBox::warning(this, "Warning", "Specified path does not exist.");
            return;
        }

        gitProcess->setWorkingDirectory(path);
        gitProcess->start("git", QStringList() << "log");

        if (!gitProcess->waitForStarted()) {
            QMessageBox::critical(this, "Error", "Failed to start git log process.");
            return;
        }

        if (!gitProcess->waitForFinished()) {
            QMessageBox::critical(this, "Error", "Git log process did not finish.");
            return;
        }

        if (gitProcess->exitCode() != 0) {
            QString errorOutput = gitProcess->readAllStandardError();
            QMessageBox::critical(this, "Error", "Git log process failed: " + errorOutput);
            return;
        }

        QString logOutput = gitProcess->readAllStandardOutput();
        QMessageBox::information(this, "Git Log", "Commit Logs:\n" + logOutput);
    }

}




void MainWindow::on_GetStarted_PB_clicked()
{
    ui->tabWidget_gui->setCurrentIndex(0);
}

