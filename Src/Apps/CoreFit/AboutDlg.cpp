// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "AboutDlg.h"
#include "Util/System/SysInfo.h"
#include "CoreFit.h"
#include "Version.h"

#include "SendLogsDlg.h"

AboutDlg::AboutDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	//AppInfo appInfo;

	//ui->versionLabel->setText(appInfo.ToString());
	//if (license == License::Personal)
	{
		ui->licensedToLabel->hide();
		ui->licensedToNameValue->hide();
		ui->licensedToEmailValue->hide();
	}
	//else
	//{
	//	ui->licensedToNameValue->setText(licensedUsername);
	//	ui->licensedToEmailValue->setText(QCoreApplication::translate("About Dialog Licensed To Email", "(%1)").arg(licensedEmail));
	//}

	QString copyright = QLatin1String(VER_LEGALCOPYRIGHT_STR);
	copyright.replace(QStringLiteral("Joseph Woodbury"), QStringLiteral("<a href=\"http://www.") % QLatin1String(VER_COMPANYDOMAIN_STR) % QStringLiteral("\">Joseph Woodbury</a>"));
	ui->copyrightLabel->setText(copyright);

	//SysInfo sysInfo(false);
/*
	ui->actualOsLabel->setText(sysInfo.os);

#ifdef Q_OS_WIN
	ui->actualMemoryLabel->setText(FormatNumber(sysInfo.physicalMemory) % QStringLiteral(" (") % FormatNumber(sysInfo.availableMemory) % QStringLiteral(")"));
#else
	ui->actualMemoryLabel->setText(FormatNumber(sysInfo.physicalMemory));
#endif

	ui->actualProcessorLabel->setText(sysInfo.cpu);
*/
}

AboutDlg::~AboutDlg()
{
	delete ui;
}

void AboutDlg::on_closeButton_clicked()
{
	close();
}

void AboutDlg::on_licenseButton_clicked()
{
}

void AboutDlg::on_sendLogsButton_clicked()
{
	SendLogsDlg sendLogsDlg;
	sendLogsDlg.exec();
}
