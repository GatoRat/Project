// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

namespace Ui { class SendFeedbackDlg; }

class SendFeedbackDlg : public QDialog
{
    Q_OBJECT

private:
    Ui::SendFeedbackDlg *ui;

public:
    explicit SendFeedbackDlg(QWidget *parent = 0);
    ~SendFeedbackDlg();

};
