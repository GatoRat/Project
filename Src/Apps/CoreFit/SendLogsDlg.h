// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

namespace Ui { class SendLogsDlg; }

class SendLogsDlg : public QDialog
{
    Q_OBJECT

private:
    Ui::SendLogsDlg *ui;

public:
    explicit SendLogsDlg(QWidget *parent = 0);
    ~SendLogsDlg();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_sendButton_clicked();
    void on_cancelButton_clicked();
};
