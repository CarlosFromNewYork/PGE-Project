#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "config_packs_repos.h"
#include "xml_parse/xml_cpack_list.h"
#include <common_features/app_path.h>
#include <QtDebug>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    downloader(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    curstep = 0;
    totalSteps = 0;
    tempDir = AppPathManager::userAppDir() + "/.ManagerTemp";
    ui->cpack_info_box->setEnabled(false);
    connect(&downloader, SIGNAL(finished()), this, SLOT(downloadSuccess()));
    connect(&downloader, SIGNAL(canceled()), this, SLOT(downloadAborted()));
    connect(&downloader, SIGNAL(failed(QString)), this, SLOT(downloadFailed(QString)));
    connect(&downloader, SIGNAL(progress(qint64, qint64)), this, SLOT(setProgress(qint64, qint64)));
    ui->progressBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRepositories_triggered()
{
    ConfPacksRepos box;
    box.exec();
}

void MainWindow::on_refresh_clicked()
{
    curstep = 0;
    totalSteps = 0;
    cpacks_List.clear();
    ActionQueueItem act1;
    act1.type = ACT_LOCK_CONFIG_PAGE;
    queue.push_back(act1);

    for(int i = 0; i < cpacks_reposList.size(); i++)
    {
        if(!cpacks_reposList[i].enabled) continue;

        ActionQueueItem act2;
        act2.type = ACT_DOWNLOAD_FILE;
        act2.param1 = cpacks_reposList[i].url + "/configs.index";
        //act2.param1="http://download.gna.org/pgewohlstand/configs/A2MBXT/Raocow_talkhaus_full.zip";
        act2.param2 = tempDir;
        act2.param3 = "0"; //Skip actions on fail
        queue.push_back(act2);
        ActionQueueItem act4;
        act4.type = ACT_PARSE_CPACK_LIST_XML;
        act4.param1 = "configs.index";
        queue.push_back(act4);
        totalSteps++;
    }

    ActionQueueItem actmsg;
    actmsg.type = ACT_SHOWMSG;
    actmsg.param1 = "Configuration packages list has been refreshed!";
    actmsg.param2 = "5000";
    queue.push_back(actmsg);
    ActionQueueItem rebuild;
    rebuild.type = ACT_REBUILD_CPACK_LIST;
    queue.push_back(rebuild);
    ActionQueueItem act3;
    act3.type = ACT_UNLOCK_CONFIG_PAGE;
    queue.push_back(act3);
    queueStepBegin();
    //downloader.downloadFile("http://download.gna.org/pgewohlstand/configs/configs.index", tempDir);
    //disable "update" button
    //run QtConcurrent updateConfigPacksList()
}


void MainWindow::updateConfigPacksList()
{
    //Try to get index file from each repo
    //try to get each index file of each config pack (invalid or not exist config packs must be skiped)
    //find already downloaded config packs and compare update time
    //enable "update" button
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(downloader.isBusy())
    {
        downloader.cancelDownload();
        e->ignore();
        return;
    }

    ManagerSettings::save();
    e->accept();
}

void MainWindow::buildConfigPackList()
{
    clearCPACKList();

    for(int i = 0; i < cpacks_List.size(); i++)
        addItemToCPACKList(cpacks_List[i]);
}

void MainWindow::addItemToCPACKList(ConfigPackInfo &rp)
{
    //lockAdd=true;
    int itemFlags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QListWidgetItem *x = new QListWidgetItem(ui->cpackList);
    x->setText(rp.name);
    x->setFlags(static_cast<Qt::ItemFlag>(itemFlags));
    x->setData(Qt::UserRole, QVariant::fromValue<void * >(&rp));
    ui->cpackList->addItem(x);
    //lockAdd=false;
}

void MainWindow::clearCPACKList()
{
    QList<QListWidgetItem *> items = ui->cpackList->findItems("*", Qt::MatchWildcard);

    foreach(QListWidgetItem *it, items)
        delete it;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::queueStepBegin()
{
    if(queue.isEmpty()) return;

    currentAct = queue.dequeue();

    switch(currentAct.type)
    {
    case ACT_LOCK_CONFIG_PAGE:
        qDebug() << "ACT: lock config page";
        ui->refresh->setEnabled(false);
        statusBar()->clearMessage();
        ui->progressBar->show();
        queueStepBegin();
        break;

    case ACT_DOWNLOAD_FILE:
        qDebug() << "ACT: Download file" << currentAct.param1 << currentAct.param2 << currentAct.param3;
        curstep++;
        downloader.downloadFile(currentAct.param1, currentAct.param2);
        statusBar()->showMessage(tr("Step %1/%2 Downloading file %3...").arg(curstep).arg(totalSteps).arg(currentAct.param1));
        break;

    case ACT_PARSE_CPACK_LIST_XML:
    {
        qDebug() << "ACT: parse XML file";
        XMLCpackList handler;
        QXmlSimpleReader reader;
        reader.setContentHandler(&handler);
        reader.setErrorHandler(&handler);
        QFile file(tempDir + "/configs.index");

        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            queueStepBegin();
            break;
        }

        QXmlInputSource xmlInputSource(&file);

        if(reader.parse(xmlInputSource))
        {}

        queueStepBegin();
    }
    break;

    case ACT_REBUILD_CPACK_LIST:
        buildConfigPackList();
        queueStepBegin();
        break;

    case ACT_UNLOCK_CONFIG_PAGE:
        qDebug() << "ACT: unlock config page";
        ui->refresh->setEnabled(true);
        ui->progressBar->hide();
        queueStepBegin();
        break;

    case ACT_SHOWMSG:
        qDebug() << "ACT: show msg" << currentAct.param1 << currentAct.param2;
        statusBar()->showMessage(currentAct.param1, currentAct.param2.toInt());
        queueStepBegin();
        break;

    default:
        queueStepBegin();
        break;
    }
}

void MainWindow::downloadSuccess()
{
    statusBar()->clearMessage();
    qDebug() << "download successful";
    qApp->processEvents();
    queueStepBegin();
}

void MainWindow::downloadAborted()
{
    qDebug() << "download aborted";
    queue.clear();
    ActionQueueItem act1;
    act1.type = ACT_UNLOCK_CONFIG_PAGE;
    queue.push_back(act1);
    queueStepBegin();
}

void MainWindow::downloadFailed(QString reason)
{
    qDebug() << "download failed" << reason << downloader.isBusy();

    if(currentAct.param3.isEmpty())
    {
        //Abort process
        queue.clear();
        ActionQueueItem act1;
        act1.type = ACT_UNLOCK_CONFIG_PAGE;
        queue.push_back(act1);
    }
    else
    {
        //Skip defined number of actions
        for(int i = currentAct.param3.toInt(); i > 0 && !queue.isEmpty(); i--)
            currentAct = queue.dequeue();
    }

    statusBar()->showMessage(reason, 5000);
    //Continue process
    queueStepBegin();
}

void MainWindow::setProgress(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBar->setMaximum(static_cast<int>(totalBytes));
    ui->progressBar->setValue(static_cast<int>(bytesRead));
}
