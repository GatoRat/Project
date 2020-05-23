// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "MainWindow.h"
#include "AboutDlg.h"
#include "PreferencesDlg.h"
#include "Version.h"
#include "SendFeedbackDlg.h"
#include "ReportBugDlg.h"
#include "SendLogsDlg.h"
#include "MainView.h"

MainWindow::MainWindow(const QString& pathname, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pMainView = new MainView(this);
	m_pLibraryView = new LibraryView(ui.actionShowLibraryView, this);
	//m_pOnlineView = new OnlineView(ui.actionShowOnlineView, this);

	m_pVLayoutSplitter = new QSplitter(this);
	m_pVLayoutSplitter->setOrientation(Qt::Vertical);
	m_pVLayoutSplitter->addWidget(m_pLibraryView);
	//m_pVLayoutSplitter->addWidget(m_pOnlineView);

    m_pVLayout = new QVBoxLayout;
	m_pVLayout->setSpacing(0);
	m_pVLayout->setMargin(0);
    m_pVLayout->addWidget(m_pVLayoutSplitter);
	m_pVLayoutSplitter->show();

	m_pLeftContainer = new QWidget;
	m_pLeftContainer->setLayout(m_pVLayout);

	m_pHLayoutSplitter = new QSplitter(this);
	m_pHLayoutSplitter->setOrientation(Qt::Horizontal);
	m_pHLayoutSplitter->addWidget(m_pLeftContainer);
	m_pHLayoutSplitter->addWidget(m_pMainView);

    m_pHLayout = new QHBoxLayout;
	m_pHLayout->setSpacing(0);
	m_pHLayout->setMargin(0);
    m_pHLayout->addWidget(m_pHLayoutSplitter);

	ui.centralWidget->setLayout(m_pHLayout);
	
	ui.actionShowMainView->setChecked(true);
	ui.actionShowLibraryView->setChecked(true);
	ui.actionShowOnlineView->setChecked(true);

    //restoreGeometry(pSettings->value(QStringLiteral("mainWindowGeometry")).toByteArray());
    //restoreState(pSettings->value(QStringLiteral("mainWindowState")).toByteArray(), SAVED_STATE_VERSION);

	connect(ui.actionExit, &QAction::triggered, this, &MainWindow::close);

	//for (const QString& pathname : pathnames)
	{
		CreateNewSubWindow(&pathname);
	}

	//if (!ui.mdiArea->currentSubWindow())
	//{
	//	CreateNewSubWindow(nullptr);
	//}

	//setWindowTitle(GetMainWindowTitle(ui.mdiArea->currentSubWindow()->windowTitle()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::CreateNewSubWindow(const QString* /*pPathname*/)
{
	//if (!pPathname || pPathname->isEmpty())
	//{
	//	MainView* pDesignWindow = new MainView;//(nullptr);
	//	ui.mdiArea->addSubWindow(pDesignWindow);
	//	pDesignWindow->showMaximized();
	//}
	//else
	//{
	//	try
	//	{
	//		MainView* pDesignWindow = new MainView;//(new File(*pPathname));
	//		ui.mdiArea->addSubWindow(pDesignWindow);
	//		pDesignWindow->showMaximized();
	//	}
	//	catch (const FileException& fileException)
	//	{
	//		fileException.Log(LogLevel::Warning, "File passed on command line");
	//	}
	//}
}

void MainWindow::closeEvent(QCloseEvent* /*event*/) 
{
	pSettings->setValue(QStringLiteral("mainWindowGeometry"), saveGeometry());
	pSettings->setValue(QStringLiteral("mainWindowState"), saveState(SAVED_STATE_VERSION));
}

void MainWindow::on_actionPreferences_triggered()
{
	PreferencesDlg preferencesDlg(this);
	preferencesDlg.exec();
}

void MainWindow::on_actionAbout_triggered()
{
	AboutDlg aboutDlg(this);
	aboutDlg.exec();
}

void MainWindow::on_actionWebSite_triggered()
{
	QDesktopServices::openUrl(QUrl(QStringLiteral("www.") % QLatin1String(VER_COMPANYDOMAIN_STR))); 
}

void MainWindow::on_actionShowMainView_triggered()
{
	if (ui.actionShowMainView->isChecked())
	{
		m_pMainView->show();
	}
	else
	{
		m_pMainView->hide();
	}
}

void MainWindow::on_actionShowLibraryView_triggered()
{
	if (ui.actionShowLibraryView->isChecked())
	{
		m_pLibraryView->show();
	}
	else
	{
		m_pLibraryView->hide();
	}
}

void MainWindow::on_actionShowOnlineView_triggered()
{
	//if (ui.actionShowOnlineView->isChecked())
	//{
	//	m_pOnlineView->show();
	//}
	//else
	//{
	//	m_pOnlineView->hide();
	//}
}

void MainWindow::on_actionReport_Bug_triggered()
{
	ReportBugDlg reportBugDlg(this);
	reportBugDlg.exec();
}

void MainWindow::on_actionSend_Feedback_triggered()
{
	SendFeedbackDlg sendFeedbackDlg(this);
	sendFeedbackDlg.exec();
}

void MainWindow::on_actionSend_Logs_triggered()
{
	SendLogsDlg sendLogsDlg;
	sendLogsDlg.exec();
}

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow* pSubWindow)
{
	if (pSubWindow)
		setWindowTitle("SubWindow");// GetMainWindowTitle(pSubWindow->windowTitle()));
	else
        setWindowTitle("Main2");// GetMainWindowTitle(nullptr));
}

void MainWindow::on_actionNew_triggered()
{
	CreateNewSubWindow(nullptr);
}

void MainWindow::on_actionOpen_triggered()
{
}
