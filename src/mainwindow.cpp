#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controlmenu.h"
#include "ui_controlmenu.h"
#include "controlmenumail.h"
#include "ui_controlmenumail.h"
#include "controlmenumain.h"
#include "ui_controlmenumain.h"
#include "mywebenginepage.h"
#include <QDesktopWidget>
#include <string>
#include <QRect>
#include <QWindow>
#include <QDockWidget>
#include <QTreeWidget>
#include <map>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QWebEngineProfile>
#include <QWebChannel>
#include <QWebEngineUrlRequestInfo>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QtWebEngineWidgets>
#include <QObject>
#include <QPalette>
#include <QSettings>
#include <QTextToSpeech>
#include <QFileInfo>

std::map<std::string, QString> QStringMap;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
#if _WIN32
    , m_speech(nullptr)
#endif
{

#ifdef __linux__
    QMainWindow::setWindowIcon(QIcon(":/Images/EasyMenu_Icone.svg"));
#elif _WIN32
    QMainWindow::setWindowIcon(QIcon(":/Images/EasyMenu_Icone.ico"));
#endif

    ui->setupUi(this);

#if _WIN32
    m_speech = new QTextToSpeech(this);
    m_speech->setLocale(QLocale::French);
#endif
    screens = QGuiApplication::screens();
    screen = screens.first();
    screenGeometry = screen->geometry();
    HEIGHT = screenGeometry.height();
    WIDTH = screenGeometry.width();
    WIDTHCONTROL = int(WIDTH * 720 / 3840);
    WIDTHMAIN = WIDTH -WIDTHCONTROL;
    FenG = new QWidget;
    menuG = new ControlMenuMain();
    myWid = this->winId();
    ma_fenetreG = QWindow::fromWinId(myWid);
    FenApp = QWidget::createWindowContainer(ma_fenetreG);
    FenApp->setMinimumWidth(WIDTHMAIN);
    FenApp->setMinimumHeight(HEIGHT);
    myLayout = new QHBoxLayout(FenG);
    menuG->setMaximumWidth(WIDTHCONTROL);
    menuG->setMaximumHeight(HEIGHT);
    myLayout->addWidget(FenApp);
    myLayout->addWidget(menuG);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    pal.setColor(QPalette::WindowText, Qt::white);
    FenG->setAutoFillBackground(true);
    FenG->setPalette(pal);
    FenG->setStyleSheet("background-color:black; color:#fff");
    FenG->showFullScreen();

    fontC = ui->Calculatrice->font();
    fontE = ui->Email->font();
    fontI = ui->Internet->font();
    fontN = ui->Notes->font();
    fontD = ui->Discord->font();
    fontM = ui->Music->font();

    double dpi = QGuiApplication::primaryScreen()->physicalDotsPerInch();

    myScale = QString::number(int(2.5 * (dpi /159.856) * (WIDTH / 3840)));
    myScale2 = QString::number(int(4 * (dpi /159.856) * (WIDTH / 3840)));

    QS1 = QSize((int)(HEIGHT*.3),(int)(HEIGHT*.3));
    fSize1 = (int)((21*HEIGHT/2160)*72/dpi);
    QS2 = QSize((int)(HEIGHT*.05),(int)(HEIGHT*.05));
    fSize2 = (int)((120*HEIGHT/2160)*72/dpi);

    mySettings.beginGroup("ChoixMusic");
    int myInt = mySettings.value("Music").toInt();
    mySettings.endGroup();

    switch ( myInt )
    {
        case 1:
            musicText = "Ecouter de\nla musique\nsur YouTube";
            music->load(QUrl("https://www.youtube.com/"));
            serviceMusic = "Écouter de la musique grâce à YouTube";
            break;
        case 2:
            musicText = "Ecouter de\nla musique\nsur Deezer";
            music->load(QUrl("https://www.deezer.com/fr/"));
            serviceMusic = "Écouter de la musique grâce à Deezer";
            break;
        default:
            musicText = "Ecouter de\nla musique\nsur Jamendo";
            music->load(QUrl("https://www.jamendo.com/start"));
            serviceMusic = "Écouter de la musique grâce à Jamendo";
            break;
    }

    QString calculatriceText = "Calculatrice";
    QString emailText = "Gestion \ne-mail";
    QString internetText = "Accédez\nà internet";
    QString notesText = "Texte,\nCalcul ou \nPrésentation";
    QString discordText = "Nous\nrejoindre\nsur Discord";

    QStringMap["Calculatrice"] = calculatriceText;
    QStringMap["Email"] = emailText;
    QStringMap["Internet"] = internetText;
    QStringMap["Notes"] = notesText;
    QStringMap["Discord"] = discordText;
    QStringMap["Music"] = musicText;

    ui->Calculatrice->setText("");
    ui->Email->setText("");
    ui->Internet->setText("");
    ui->Notes->setText("");
    ui->Discord->setText("");
    ui->Music->setText("");

    ui->Calculatrice->setIconSize(QS1);
    ui->Email->setIconSize(QS1);
    ui->Internet->setIconSize(QS1);
    ui->Notes->setIconSize(QS1);
    ui->Discord->setIconSize(QS1);
    ui->Music->setIconSize(QS1);
    fontC.setPointSize(fSize1);
    fontE.setPointSize(fSize1);
    fontI.setPointSize(fSize1);
    fontN.setPointSize(fSize1);
    fontD.setPointSize(fSize1);
    fontM.setPointSize(fSize1);
    ui->Calculatrice->setFont(fontC);
    ui->Email->setFont(fontE);
    ui->Internet->setFont(fontI);
    ui->Notes->setFont(fontN);
    ui->Discord->setFont(fontD);
    ui->Music->setFont(fontM);

    ui->Calculatrice->setStyleSheet("background-color: rgb(41, 182, 71);border-radius: 10px;border:  8PX solid red;color : white;");
    ui->Email->setStyleSheet("background-color: rgb(240, 120, 80);border-radius: 10px;border:  8PX solid red;color : white;");
    ui->Internet->setStyleSheet("background-color: rgb(88, 70, 55);border-radius: 10px;border:  8PX solid red;color : white;");
    ui->Notes->setStyleSheet("background-color: rgb(0, 88, 132);border-radius: 10px;border:  8PX solid red;color : white;");
    ui->Discord->setStyleSheet("background-color: rgb(114, 137, 218);border-radius: 10px;border:  8PX solid red;color : white;");
    ui->Music->setStyleSheet("background-color: rgb(212, 115, 212);border-radius: 10px;border:  8PX solid red;color : white;");

    ui->Calculatrice->installEventFilter(this);
    ui->Email->installEventFilter(this);
    ui->Internet->installEventFilter(this);
    ui->Notes->installEventFilter(this);
    ui->Discord->installEventFilter(this);
    ui->Music->installEventFilter(this);

    profileC = KCalculatrice->page()->profile();
    profileC->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");
    profileE = mail->page()->profile();
    profileE->setHttpUserAgent("Mozilla/5.0 ({os_info}; rv:71.0) Gecko/20100101 Firefox/71.0");
    profileI = web->page()->profile();
    profileI->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");
    profileN = office->page()->profile();
    profileN->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");
    profileD = DiscordLauncher->page()->profile();
    profileD->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");
    profileM = music->page()->profile();
    profileM->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");

    myWid = this->winId();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() != QEvent::WindowDeactivate) {
        if (watched == ui->Calculatrice && event->type() == QEvent::HoverEnter) {
            ui->Calculatrice->setText(QStringMap.at("Calculatrice"));
            ui->Calculatrice->setIcon(QIcon(":/Images/0-Categorie/calculator-color.svg"));
            ui->Calculatrice->setStyleSheet("background-color: rgb(0, 0, 0);border-radius: 10px;border:  16PX solid rgb(41, 182, 71);color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            m_process->start("createWaveFromItem \"Ouvrir la calculatrice.\"");
#elif _WIN32
            m_speech->say("Ouvrir la calculatrice.");
#endif

            ui->Calculatrice->setIconSize(QS2);
            fontC.setPointSize(fSize2);
            ui->Calculatrice->setFont(fontC);
        }
        if (watched == ui->Calculatrice && event->type() == QEvent::HoverLeave) {
            ui->Calculatrice->setText("");
            ui->Calculatrice->setIcon(QIcon(":/Images/0-Categorie/calculator.svg"));
            ui->Calculatrice->setStyleSheet("background-color: rgb(41, 182, 71);border-radius: 10px;border:  8PX solid red;color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            play=false;
#elif _WIN32
            m_speech->stop();
#endif
            ui->Calculatrice->setIconSize(QS1);
            fontC.setPointSize(fSize1);
            ui->Calculatrice->setFont(fontC);
        }
    }
    if (event->type() != QEvent::WindowDeactivate) {
        if (watched == ui->Email && event->type() == QEvent::HoverEnter) {
            ui->Email->setText(QStringMap.at("Email"));
            ui->Email->setIcon(QIcon(":/Images/0-Categorie/envelope-color.svg"));
            ui->Email->setStyleSheet("background-color: rgb(0, 0, 0);border-radius: 10px;border:  16PX solid rgb(240, 120, 80);color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            m_process->start("createWaveFromItem \"Ouvrir le client email.\"");
#elif _WIN32
            m_speech->say("Ouvrir le client email.");
#endif
            ui->Email->setIconSize(QS2);
            fontE.setPointSize(fSize2);
            ui->Email->setFont(fontE);
        }
        if (watched == ui->Email && event->type() == QEvent::HoverLeave) {
            ui->Email->setText("");
            ui->Email->setIcon(QIcon(":/Images/0-Categorie/envelope.svg"));
            ui->Email->setStyleSheet("background-color: rgb(240, 120, 80);border-radius: 10px;border:  8PX solid red;color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            play=false;
#elif _WIN32
            m_speech->stop();
#endif
            ui->Email->setIconSize(QS1);
            fontE.setPointSize(fSize1);
            ui->Email->setFont(fontE);
        }
    }
    if (event->type() != QEvent::WindowDeactivate) {
        if (watched == ui->Internet && event->type() == QEvent::HoverEnter) {
            ui->Internet->setText(QStringMap.at("Internet"));
            ui->Internet->setIcon(QIcon(":/Images/0-Categorie/globe-africa-color.svg"));
            ui->Internet->setStyleSheet("background-color: rgb(0, 0, 0);border-radius: 10px;border:  16PX solid rgb(88, 70, 55);color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            m_process->start("createWaveFromItem \"Ouvrir le navigateur internet.\"");
#elif _WIN32
            m_speech->say("Ouvrir le navigateur internet.");
#endif
            ui->Internet->setIconSize(QS2);
            fontI.setPointSize(fSize2);
            ui->Internet->setFont(fontI);
        }
        if (watched == ui->Internet && event->type() == QEvent::HoverLeave) {
            ui->Internet->setText("");
            ui->Internet->setIcon(QIcon(":/Images/0-Categorie/globe-africa.svg"));
            ui->Internet->setStyleSheet("background-color: rgb(88, 70, 55);border-radius: 10px;border:  8PX solid red;color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            play=false;
#elif _WIN32
            m_speech->stop();
#endif
            ui->Internet->setIconSize(QS1);
            fontI.setPointSize(fSize1);
            ui->Internet->setFont(fontI);
        }
    }
    if (event->type() != QEvent::WindowDeactivate) {
        if (watched == ui->Notes && event->type() == QEvent::HoverEnter) {
            ui->Notes->setText(QStringMap.at("Notes"));
            ui->Notes->setIcon(QIcon(":/Images/0-Categorie/clipboard-color.svg"));
            ui->Notes->setStyleSheet("background-color: rgb(0, 0, 0);border-radius: 10px;border:  16PX solid rgb(0, 88, 132);color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            m_process->start("createWaveFromItem \"Ouvrir la suite bureautique.\"");
#elif _WIN32
            m_speech->say("Ouvrir la suite bureautique.");
#endif
            ui->Notes->setIconSize(QS2);
            fontN.setPointSize(fSize2);
            ui->Notes->setFont(fontN);
        }
        if (watched == ui->Notes && event->type() == QEvent::HoverLeave) {
            ui->Notes->setText("");
            ui->Notes->setIcon(QIcon(":/Images/0-Categorie/clipboard.svg"));
            ui->Notes->setStyleSheet("background-color: rgb(0, 88, 132);border-radius: 10px;border:  8PX solid red;color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            play=false;
#elif _WIN32
            m_speech->stop();
#endif
            ui->Notes->setIconSize(QS1);
            fontN.setPointSize(fSize1);
            ui->Notes->setFont(fontN);
        }
    }
    if (event->type() != QEvent::WindowDeactivate) {
        if (watched == ui->Discord && event->type() == QEvent::HoverEnter) {
            ui->Discord->setText(QStringMap.at("Discord"));
            ui->Discord->setIcon(QIcon(":/Images/0-Categorie/discord-color.svg"));
            ui->Discord->setStyleSheet("background-color: rgb(0, 0, 0);border-radius: 10px;border:  16PX solid rgb(114, 137, 218);color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            m_process->start("createWaveFromItem \"Discuter grâce à discord.\"");
#elif _WIN32
            m_speech->say("Discuter grâce à discord.");
#endif
            ui->Discord->setIconSize(QS2);
            fontD.setPointSize(fSize2);
            ui->Discord->setFont(fontD);
        }
        if (watched == ui->Discord && event->type() == QEvent::HoverLeave) {
            ui->Discord->setText("");
            ui->Discord->setIcon(QIcon(":/Images/0-Categorie/discord.svg"));
            ui->Discord->setStyleSheet("background-color: rgb(114, 137, 218);border-radius: 10px;border:  8PX solid red;color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            play=false;
#elif _WIN32
            m_speech->stop();
#endif
            ui->Discord->setIconSize(QS1);
            fontD.setPointSize(fSize1);
            ui->Discord->setFont(fontD);
        }
    }
    if (event->type() != QEvent::WindowDeactivate) {
        if (watched == ui->Music && event->type() == QEvent::HoverEnter) {
            ui->Music->setText(QStringMap.at("Music"));
            ui->Music->setIcon(QIcon(":/Images/0-Categorie/music-color.svg"));
            ui->Music->setStyleSheet("background-color: rgb(0, 0, 0);border-radius: 10px;border:  16PX solid rgb(212, 115, 212);color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            m_process->start("createWaveFromItem \"" + serviceMusic + "\"");
#elif _WIN32
            m_speech->say(serviceMusic);
#endif
            ui->Music->setIconSize(QS2);
            fontM.setPointSize(fSize2);
            ui->Music->setFont(fontM);
        }
        if (watched == ui->Music && event->type() == QEvent::HoverLeave) {
            ui->Music->setText("");
            ui->Music->setIcon(QIcon(":/Images/0-Categorie/music.svg"));
            ui->Music->setStyleSheet("background-color: rgb(212, 115, 212);border-radius: 10px;border:  8PX solid red;color : white;");
#ifdef __linux__
            player->pause();
            player->stop();
            m_process->kill();
            play=false;
#elif _WIN32
            m_speech->stop();
#endif
            ui->Music->setIconSize(QS1);
            fontM.setPointSize(fSize1);
            ui->Music->setFont(fontM);
        }
    }
#ifdef __linux__
    if (!play) {
        player->pause();
        player->stop();
        player->setVolume(50);
        player->setMedia(QUrl("file:// + env.value(\"HOME\") + \"/.local/share/dvkbuntu/sonEnCours.wav\"\""));
        player->play();
        play=true;
    }
#endif
    return false;
}

void MainWindow::handleStateChanged(QProcess *procss, QWidget *widget, QWidget *testkill)
{
    if (procss->state() == QProcess::NotRunning)
    {
        widget->close();
        testkill->close();
    }
}

void MainWindow::OpenNewWindows(MyWebEnginePage *myPage)
{
    mail->setPage(myPage);
    mail->setZoomFactor(myScale.toInt());
    mail->setMinimumWidth(WIDTHMAIN);
    mail->setMinimumHeight(HEIGHT);
    FenE = new QWidget;
    myLayout = new QHBoxLayout(FenE);
    myLayout->addWidget(mail);
    menuE = new ControlMenuMail();
    menuE->setMaximumWidth(WIDTHCONTROL);
    menuE->setMaximumHeight(HEIGHT);
    myLayout->addWidget(menuE);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    pal.setColor(QPalette::WindowText, Qt::white);
    FenE->setAutoFillBackground(true);
    FenE->setPalette(pal);
    FenE->setStyleSheet("background-color:black; color:#fff");
    FenE->showFullScreen();
}

void MainWindow::on_Calculatrice_clicked()
{
    if (KCalculatrice->isVisible()) {
        FenC->showFullScreen();
    } else {
#ifdef __linux__
        KCalculatrice->load(QUrl::fromLocalFile("/usr/share/Calculator/index.html"));
#elif _WIN32
        KCalculatrice->load(QUrl::fromLocalFile(QFileInfo("Calculator/index.html").absoluteFilePath()));
#endif
        KCalculatrice->setZoomFactor(2 * myScale.toInt());
        KCalculatrice->setMinimumWidth(WIDTHMAIN);
        KCalculatrice->setMinimumHeight(HEIGHT);
        FenC = new QWidget;
        myLayout = new QHBoxLayout(FenC);
        myLayout->addWidget(KCalculatrice);
        menuC = new ControlMenu();
        menuC->setMaximumWidth(WIDTHCONTROL);
        menuC->setMaximumHeight(HEIGHT);
        myLayout->addWidget(menuC);
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::black);
        pal.setColor(QPalette::WindowText, Qt::white);
        FenC->setAutoFillBackground(true);
        FenC->setPalette(pal);
        FenC->setStyleSheet("background-color:black; color:#fff");
        FenC->showFullScreen();
    }
}

void MainWindow::on_Email_clicked()
{
    MyWebEnginePage *page = new MyWebEnginePage();
    mySettings.beginGroup("ChoixMail");
    ServiceMail = mySettings.value("Mail").toInt();
    mySettings.endGroup();
    switch ( ServiceMail )
    {
        case 1:
            page->load(QUrl("https://outlook.live.com/mail/0/inbox"));
            break;
        case 2:
            page->load(QUrl("https://login.yahoo.com/?.src=ym&lang=fr-FR&done=https%3A%2F%2Ffr.mail.yahoo.com"));
            break;
        case 3:
            page->load(QUrl("https://login.orange.fr/?return_url=https://rms.orange.fr/mail/inbox%3F"));
            break;
        case 4:
            page->load(QUrl("https://www.sfr.fr/cas/login?service=https%3A%2F%2Fwebmail.sfr.fr%2Fwebmail%2Fj_spring_cas_security_check"));
            break;
        case 5:
            page->load(QUrl("https://zimbra.free.fr/"));
            break;
        case 6:
            page->load(QUrl("https://www.mon-compte.bouyguestelecom.fr/cas/login?service=https%3A%2F%2Foauth2.bouyguestelecom.fr%2Fcallback%2Fpicasso%2Fprotocol%2Fcas%3Fid%3Dar-33df094b-d8fb-4aca-afba-fb33f42ab763%26client_id%3Dwebmail.bouyguestelecom.fr"));
            break;
        case 7:
            page->load(QUrl("https://www.laposte.net/accueil"));
            break;
        default:
            page->load(QUrl("https://mail.google.com/"));
    }
    OpenNewWindows(page);
 }

void MainWindow::on_Notes_clicked()
{
    if (office->isVisible()) {
        FenN->showFullScreen();
    } else {
        office->load(QUrl("https://personal.onlyoffice.com"));
        office->setZoomFactor(myScale.toInt());
        office->setMinimumWidth(WIDTHMAIN);
        office->setMinimumHeight(HEIGHT);
        FenN = new QWidget;
        myLayout = new QHBoxLayout(FenN);
        myLayout->addWidget(office);
        menuN = new ControlMenu();
        menuN->setMaximumWidth(WIDTHCONTROL);
        menuN->setMaximumHeight(HEIGHT);
        myLayout->addWidget(menuN);
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::black);
        pal.setColor(QPalette::WindowText, Qt::white);
        FenN->setAutoFillBackground(true);
        FenN->setPalette(pal);
        FenN->setStyleSheet("background-color:black; color:#fff");
        FenN->showFullScreen();
    }
}

void MainWindow::on_Internet_clicked()
{
    if (web->isVisible()) {
        FenI->showFullScreen();
    } else {
        web->load(QUrl("https://www.search.handy-open-source.org/search.php"));
        web->setZoomFactor(myScale.toInt());
        web->setMinimumWidth(WIDTHMAIN);
        web->setMinimumHeight(HEIGHT);
        FenI = new QWidget;
        myLayout = new QHBoxLayout(FenI);
        myLayout->addWidget(web);
        menuI = new ControlMenu();
        menuI->setMaximumWidth(WIDTHCONTROL);
        menuI->setMaximumHeight(HEIGHT);
        myLayout->addWidget(menuI);
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::black);
        pal.setColor(QPalette::WindowText, Qt::white);
        FenI->setAutoFillBackground(true);
        FenI->setPalette(pal);
        FenI->setStyleSheet("background-color:black; color:#fff");
        FenI->showFullScreen();
}
}

void MainWindow::on_Music_clicked()
{
    if (music->isVisible()) {
        FenM->showFullScreen();
    } else {
        music->setZoomFactor(myScale.toInt());
        music->setMinimumWidth(WIDTHMAIN);
        music->setMinimumHeight(HEIGHT);
        FenM = new QWidget;
        myLayout = new QHBoxLayout(FenM);
        myLayout->addWidget(music);
        menuM = new ControlMenu();
        menuM->setMaximumWidth(WIDTHCONTROL);
        menuM->setMaximumHeight(HEIGHT);
        myLayout->addWidget(menuM);
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::black);
        pal.setColor(QPalette::WindowText, Qt::white);
        FenM->setAutoFillBackground(true);
        FenM->setPalette(pal);
        FenM->setStyleSheet("background-color:black; color:#fff");
        FenM->showFullScreen();
    }
}

void MainWindow::on_Discord_clicked()
{
    profileD = DiscordLauncher->page()->profile();
    if (profileD->httpUserAgent() != "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36") {
        profileD->setHttpUserAgent(
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");
    }
    if (DiscordLauncher->isVisible()) {
        FenD->showFullScreen();
    } else {
        DiscordLauncher->setPage(new MyWebEnginePage);
        DiscordLauncher->setUrl(QUrl("https://discord.com/channels/@me"));
        DiscordLauncher->setZoomFactor(myScale.toInt());
        DiscordLauncher->setMinimumWidth(WIDTHMAIN);
        DiscordLauncher->setMinimumHeight(HEIGHT);
        FenD = new QWidget;
        myLayout = new QHBoxLayout(FenD);
        myLayout->addWidget(DiscordLauncher);
        menuD = new ControlMenu();
        menuD->setMaximumWidth(WIDTHCONTROL);
        menuD->setMaximumHeight(HEIGHT);
        myLayout->addWidget(menuD);
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::black);
        pal.setColor(QPalette::WindowText, Qt::white);
        FenD->setAutoFillBackground(true);
        FenD->setPalette(pal);
        FenD->setStyleSheet("background-color:black; color:#fff");
        FenD->showFullScreen();
    }
}
