// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <QtWidgets/QDialog>
#include "ui_AboutDlg.h"

class AboutDlg : public QDialog
{
	Q_OBJECT

private:
	Ui::AboutDlg *ui;

public:
	explicit AboutDlg(QWidget *parent = 0);
	~AboutDlg();

private slots:
	void on_closeButton_clicked();
	void on_licenseButton_clicked();
    void on_sendLogsButton_clicked();
};
