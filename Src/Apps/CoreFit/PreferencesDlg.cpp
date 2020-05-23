// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "PreferencesDlg.h"
#include "ui_PreferencesDlg.h"

PreferencesDlg::PreferencesDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDlg)
{
    ui->setupUi(this);

    ui->categoriesListBox->addItem(tr("General"));
    ui->categoriesListBox->addItem(tr("Display"));

	ui->categoriesListBox->setCurrentRow(0);
}

PreferencesDlg::~PreferencesDlg()
{
    delete ui;
}

void PreferencesDlg::on_categoriesListBox_itemSelectionChanged()
{
	int row = ui->categoriesListBox->currentRow();
	switch (row)
	{
		case 0:	// General
			switch (preferences.general.textColor)
			{
				case TextColor::Black:
					ui->blackRadio->setChecked(true);
					break;

				case TextColor::Grey:
					ui->greyRadio->setChecked(true);
					break;

				default:
					ui->redRadio->setChecked(true);
					break;
			}

			//fillSoldRadio = new QRadioButton(fillStyleGroupBox);

   //     cuttingMatGroupBox = new QGroupBox(generalWidget);
   //     cuttingMatGroupBox->setObjectName(QStringLiteral("cuttingMatGroupBox"));
   //     cuttingMatGroupBox->setGeometry(QRect(10, 110, 231, 81));
   //     matAlwaysRadio = new QRadioButton(cuttingMatGroupBox);
   //     matAlwaysRadio->setObjectName(QStringLiteral("matAlwaysRadio"));
   //     matAlwaysRadio->setGeometry(QRect(20, 30, 82, 17));
   //     matSendingRadio = new QRadioButton(cuttingMatGroupBox);
   //     matSendingRadio->setObjectName(QStringLiteral("matSendingRadio"));
   //     matSendingRadio->setGeometry(QRect(20, 50, 201, 17));

			break;
		
		case 1:	// Display
			break;
	}
	ui->stackedWidget->setCurrentIndex(row);
}
