// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

namespace Ui 
{
class PreferencesDlg;
}

class PreferencesDlg : public QDialog
{
    Q_OBJECT

private:
    Ui::PreferencesDlg *ui;

public:
    explicit PreferencesDlg(QWidget *parent = 0);
    ~PreferencesDlg();

private slots:
    void on_categoriesListBox_itemSelectionChanged();
};
