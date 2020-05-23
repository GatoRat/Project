// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "SendLogsDlg.h"
#include "ui_SendLogsDlg.h"

//#include <Encrypt.h>

/*
static QString GenerateCode()
{
	char buffer[13];
	buffer[12] = 0;
	uchar checksum = 0;

	for (int i = 0; i < 10; i++)
	{
		if (i == 2 || i == 4 || i == 7)
		{
			buffer[i] = '0';
		}
		else
		{
			int value; 
			if (i < 2)
			{
				value = 1 + (Rand32() % 9);
			}
			else
			{
				value = (Rand32() % 10);
			}
			buffer[i] = (char)('0' + value);
			checksum = (uchar)(checksum + value);
		}
	}

	checksum = (uchar)(0xff & ((~checksum) + 1));

	int i = 7;
	do
	{
		buffer[i] = (char) ((checksum % 10) + '0');
		checksum /= 10;

		if (i == 7)
		{
			i = 4;
		}
		else if (i == 4)
		{
			i = 2;
		}

	} while (checksum > 0);

	return QString(buffer);
}
*/

static int DaysSinceEpoch()
{
	qint64 seconds = QDateTime::currentMSecsSinceEpoch() / 1000;
	int days = (int)(seconds / 86400);
	return days;
}

static QString GenerateCode()
{
	int days = DaysSinceEpoch();
	uchar checksum = 0;
	char buffer[12];

	int i = 1;
	for (; i <= 5; i++)
	{
		int val = days % 10;
		checksum = (uchar)(checksum + val);
		buffer[i] = (char)(val + '0');
		days /= 10;
	}

	//for (; i <= 8; i++)
	//{
	//	int val = Rand32() % 10;
	//	checksum = (uchar)(checksum + val);
	//	buffer[i] = (char)(val + '0');
	//}

	checksum = (uchar)(0xff & ((~checksum) + 1)) % 128;

	buffer[0] = (char)((checksum % 10) + '0');
	buffer[9] = (char)((checksum / 10) + '0');

	return QString::fromLatin1(buffer, 10);
}

static bool CheckCode(const QString& codeStr)
{
	if (codeStr.size() != 10)
		return false;

	int chVal = codeStr[0].digitValue();
	if (chVal < 0)
		return false;

	uchar expectedCheckSum = (uchar) chVal;

	chVal = codeStr[9].digitValue();
	if (chVal < 0)
		return false;

	expectedCheckSum += (uchar)(chVal * 10);

	int days = 0;
	uchar checksum = 0;

	for (int i = 5; i >= 1; i--)
	{
		chVal = codeStr[i].digitValue();
		if (chVal < 0)
			return false;

		days *= 10;
		days += chVal;

		checksum = (uchar)(checksum + chVal);
	}

	for (int i = 6; i <= 8; i++)
	{
		chVal = codeStr[i].digitValue();
		if (chVal < 0)
			return false;

		checksum = (uchar)(checksum + chVal);
	}

	checksum = (uchar)(0xff & ((~checksum) + 1)) % 128;

	if (checksum != expectedCheckSum)
		return false;

	return days == DaysSinceEpoch();
}

SendLogsDlg::SendLogsDlg(QWidget *parent)
	: QDialog(parent)
    , ui(new Ui::SendLogsDlg)
{
    ui->setupUi(this);
	ui->sendButton->setEnabled(false);

    [[maybe_unused]] QString s = GenerateCode();
	//s = s;
}

SendLogsDlg::~SendLogsDlg()
{
    delete ui;
}

void SendLogsDlg::on_lineEdit_textChanged(const QString& codeStr)
{
	ui->sendButton->setEnabled(CheckCode(codeStr));
}

void SendLogsDlg::on_sendButton_clicked()
{
    Log::String("Sending logs.");

    QProgressDialog progress("Sending logs...", "Abort Send", 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumDuration(0);
	
    for (int i = 0; i < 100; i++)
	{
        progress.setValue(i);

        if (progress.wasCanceled())
            break;

		Sleep(25);
        //... copy one file
    }
    progress.setValue(100);

	accept();

}

void SendLogsDlg::on_cancelButton_clicked()
{
	reject();
}
