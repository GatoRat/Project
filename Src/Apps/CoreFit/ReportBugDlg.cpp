// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "ReportBugDlg.h"
#include "ui_ReportBugDlg.h"

ReportBugDlg::ReportBugDlg(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ReportBugDlg)
{
    ui->setupUi(this);
}

ReportBugDlg::~ReportBugDlg()
{
    delete ui;
}

void ReportBugDlg::on_buttonBox_accepted()
{
    accept();
}

void ReportBugDlg::on_buttonBox_rejected()
{
    reject();
}
