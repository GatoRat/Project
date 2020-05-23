// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

namespace Ui { class ReportBugDlg; }

class ReportBugDlg : public QDialog
{
    Q_OBJECT

private:
    Ui::ReportBugDlg *ui;

public:
    explicit ReportBugDlg(QWidget *parent = 0);
    ~ReportBugDlg();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
