// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "MainView.h"
#include "LibraryView.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindowClass ui;

	MainView*		m_pMainView;
	LibraryView*	m_pLibraryView;
	QVBoxLayout*	m_pVLayout;
	QHBoxLayout*	m_pHLayout;
	QSplitter*		m_pVLayoutSplitter;
	QSplitter*		m_pHLayoutSplitter;
	QWidget*		m_pLeftContainer;

	//QMdiArea*		m_pMdiArea;

	void SetWindowTitle(const QString& name);
	void CreateNewSubWindow(const QString* pPathname);

public:
	MainWindow(const QString& pathname, QWidget *parent = 0);
	~MainWindow();

	void closeEvent(QCloseEvent* event);

private slots:
	void on_actionPreferences_triggered();
	void on_actionAbout_triggered();
	void on_actionWebSite_triggered();
	void on_actionShowMainView_triggered();
	void on_actionShowLibraryView_triggered();
	void on_actionShowOnlineView_triggered();
    void on_actionReport_Bug_triggered();
    void on_actionSend_Feedback_triggered();
    void on_actionSend_Logs_triggered();
    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
};
