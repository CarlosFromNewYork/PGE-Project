#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>

#undef main
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#undef main

/*!
 *  SDL Mixer wrapper
 */
namespace PGE_MusicPlayer
{
    Mix_Music *play_mus = NULL;
    Mix_MusicType type  = MUS_NONE;

    /*!
     * \brief Spawn warning message box with specific text
     * \param msg text to spawn in message box
     */
    static void error(QString msg)
    {
        QMessageBox::warning(nullptr, "SDL2 Mixer ext error", msg, QMessageBox::Ok);
    }

    /*!
     * \brief Stop music playing
     */
    void MUS_stopMusic()
    {
        Mix_HaltMusic();
    }

    /*!
     * \brief Get music title of current track
     * \return music title of current music file
     */
    QString MUS_getMusTitle()
    {
        if(play_mus)
            return QString(Mix_GetMusicTitle(play_mus));
        else
            return QString("[No music]");
    }

    /*!
     * \brief Get music artist tag text of current music track
     * \return music artist tag text of current music track
     */
    QString MUS_getMusArtist()
    {
        if(play_mus)
            return QString(Mix_GetMusicArtistTag(play_mus));
        else
            return QString("[Unknown Artist]");
    }

    /*!
     * \brief Get music album tag text of current music track
     * \return music ablum tag text of current music track
     */
    QString MUS_getMusAlbum()
    {
        if(play_mus)
            return QString(Mix_GetMusicAlbumTag(play_mus));
        else
            return QString("[Unknown Album]");
    }

    /*!
     * \brief Get music copyright tag text of current music track
     * \return music copyright tag text of current music track
     */
    QString MUS_getMusCopy()
    {
        if(play_mus)
            return QString(Mix_GetMusicCopyrightTag(play_mus));
        else
            return QString("");
    }

    /*!
     * \brief Start playing of currently opened music track
     */
    void MUS_playMusic()
    {
        if(play_mus)
        {
            if(Mix_PlayMusic(play_mus, -1)==-1)
            {
                error(QString("Mix_PlayMusic: %1").arg(Mix_GetError()));
                // well, there's no music, but most games don't break without music...
            }
            //QString("Music is %1\n").arg(Mix_PlayingMusic()==1?"Playing":"Silence");
        }
        else
        {
            error(QString("Play nothing: Mix_PlayMusic: %1").arg(Mix_GetError()));
        }
        //qDebug() << QString("Check Error of SDL: %1\n").arg(Mix_GetError());
    }

    /*!
     * \brief Sets volume level of current music stream
     * \param volume level of volume from 0 tp 128
     */
    void MUS_changeVolume(int volume)
    {
        Mix_VolumeMusic(volume);
        qDebug() << QString("Mix_VolumeMusic: %1\n").arg(volume);
    }

    /*!
     * \brief Open music file
     * \param musFile Full path to music file
     * \return true if music file was successfully opened, false if loading was failed
     */
    bool MUS_openFile(QString musFile)
    {
        if(play_mus!=NULL) {Mix_FreeMusic(play_mus);play_mus=NULL;}
        play_mus = Mix_LoadMUS( musFile.toUtf8() );
        if(!play_mus) {
            error(QString("Mix_LoadMUS(\"%1\"): %2").arg(musFile).arg(Mix_GetError()));
            return false;
        }

                type=Mix_GetMusicType(play_mus);
        qDebug() << QString("Music type: %1").arg(
                type==MUS_NONE?"MUS_NONE":
                type==MUS_CMD?"MUS_CMD":
                type==MUS_WAV?"MUS_WAV":
                /*type==MUS_MOD_MODPLUG?"MUS_MOD_MODPLUG":*/
                type==MUS_MOD?"MUS_MOD":
                type==MUS_MID?"MUS_MID":
                type==MUS_OGG?"MUS_OGG":
                type==MUS_MP3?"MUS_MP3":
                type==MUS_MP3_MAD?"MUS_MP3_MAD":
                type==MUS_FLAC?"MUS_FLAC":
                type==MUS_SPC?"MUS_SPC":
                "Unknown");
        return true;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->adlmidi_xtra->setVisible(false);
    ui->midi_setup->setVisible(false);
    ui->gme_setup->setVisible(false);
    ui->centralWidget->window()->setWindowFlags(
                Qt::WindowTitleHint|
                Qt::WindowSystemMenuHint|
                Qt::WindowCloseButtonHint|
                Qt::WindowMinimizeButtonHint);
    this->updateGeometry();
    this->window()->resize(100, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::openMusicByArg(QString musPath)
{
    currentMusic=musPath;
    Mix_HaltMusic();
    on_play_clicked();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    this->raise();
    this->setFocus(Qt::ActiveWindowFocusReason);

    foreach (const QUrl &url, e->mimeData()->urls()) {
        const QString &fileName = url.toLocalFile();
        currentMusic=fileName;
    }
    Mix_HaltMusic();
    on_play_clicked();
    this->raise();
}

void MainWindow::on_open_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open music file"),
                                                 (currentMusic.isEmpty() ? QApplication::applicationDirPath() : currentMusic), "All (*.*)");
    if(file.isEmpty()) return;
    currentMusic=file;
    Mix_HaltMusic();
    on_play_clicked();
}

void MainWindow::on_stop_clicked()
{
    PGE_MusicPlayer::MUS_stopMusic();
    ui->play->setText(tr("Play"));
}

void MainWindow::on_play_clicked()
{
    if(Mix_PlayingMusic())
    {
        if(Mix_PausedMusic())
        {
            Mix_ResumeMusic();
            ui->play->setText(tr("Pause"));
            return;
        }
        else
        {
            Mix_PauseMusic();
            ui->play->setText(tr("Resume"));
            return;
        }
    }

    ui->play->setText(tr("Play"));
    if(PGE_MusicPlayer::MUS_openFile(currentMusic+"|"+ui->trackID->text()))
    {
        PGE_MusicPlayer::MUS_changeVolume(128);
        PGE_MusicPlayer::MUS_playMusic();
        ui->play->setText(tr("Pause"));
    }
    ui->musTitle->setText(PGE_MusicPlayer::MUS_getMusTitle());
    ui->musArtist->setText(PGE_MusicPlayer::MUS_getMusArtist());
    ui->musAlbum->setText(PGE_MusicPlayer::MUS_getMusAlbum());
    ui->musCopyright->setText(PGE_MusicPlayer::MUS_getMusCopy());

    ui->gme_setup->setVisible(false);
    ui->adlmidi_xtra->setVisible(false);
    ui->midi_setup->setVisible(false);
    ui->frame->setVisible(false);
    this->window()->resize(100,100);
    ui->frame->setVisible(true);
    ui->gridLayout->update();

    switch(PGE_MusicPlayer::type)
    {
        case MUS_MID:
            ui->adlmidi_xtra->setVisible(ui->mididevice->currentIndex()==0);
            ui->midi_setup->setVisible(true);
            ui->frame->setVisible(true);
            break;
        case MUS_SPC:
            ui->gme_setup->setVisible(true);
            ui->frame->setVisible(true);
            break;
        default:
            break;
    }
    this->window()->updateGeometry();
    this->window()->resize(100,100);
}

void MainWindow::on_mididevice_currentIndexChanged(int index)
{
    ui->midi_setup->setVisible(false);
    ui->adlmidi_xtra->setVisible(false);
    this->window()->resize(100,100);
    ui->midi_setup->setVisible(true);
    switch(index)
    {
        case 0: MIX_SetMidiDevice(MIDI_ADLMIDI);
        ui->adlmidi_xtra->setVisible(true);
        break;
        case 1: MIX_SetMidiDevice(MIDI_Timidity);
        ui->adlmidi_xtra->setVisible(false);
        break;
        case 2: MIX_SetMidiDevice(MIDI_Native);
        ui->adlmidi_xtra->setVisible(false);
        break;
        default: MIX_SetMidiDevice(MIDI_ADLMIDI);
        ui->adlmidi_xtra->setVisible(true);
        break;
    }
    this->window()->resize(100,100);

    if(Mix_PlayingMusic())
    {
        if(PGE_MusicPlayer::type==MUS_MID)
        {
            Mix_HaltMusic();
            on_play_clicked();
        }
    }
}

void MainWindow::on_fmbank_currentIndexChanged(int index)
{
    MIX_ADLMIDI_setBankID(index);
    if(Mix_PlayingMusic())
    {
        if(PGE_MusicPlayer::type==MUS_MID)
        {
            Mix_HaltMusic();
            on_play_clicked();
        }
    }
}

void MainWindow::on_tremolo_clicked(bool checked)
{
    MIX_ADLMIDI_setTremolo((int)checked);
}

void MainWindow::on_vibrato_clicked(bool checked)
{
    MIX_ADLMIDI_setVibrato((int)checked);
}

void MainWindow::on_modulation_clicked(bool checked)
{
    MIX_ADLMIDI_setScaleMod((int)checked);
}



void MainWindow::on_trackID_editingFinished()
{
    if(Mix_PlayingMusic())
    {
        if(PGE_MusicPlayer::type==MUS_SPC)
        {
            Mix_HaltMusic();
            on_play_clicked();
        }
    }
}
