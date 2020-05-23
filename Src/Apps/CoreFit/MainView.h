// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

namespace Ui { class MainView; }

class MainView : public QWidget
{
    Q_OBJECT

private:
    Ui::MainView *ui;

public:
	QString		m_title;
	QString		m_windowNumStr;

public:
    explicit MainView(QWidget* pParent = 0);
    ~MainView();

protected:
	virtual void paintEvent(QPaintEvent* pEvent);
};
