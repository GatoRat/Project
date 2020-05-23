// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "SendFeedbackDlg.h"
#include "ui_SendFeedbackDlg.h"

SendFeedbackDlg::SendFeedbackDlg(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::SendFeedbackDlg)
{
    ui->setupUi(this);
}

SendFeedbackDlg::~SendFeedbackDlg()
{
    delete ui;
}
