// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

QTextStream output(stdout);
QTextStream errout(stderr);

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);

	QCoreApplication::setApplicationVersion("0.1.2.3");
	QCoreApplication::setOrganizationDomain("");
	QCoreApplication::setOrganizationName("Joe Woodbury");


    QTime t;
    t.setHMS(16, 37, 0);

    output << t.toString("h:mm") << endl;
    output << t.toString() << endl;


    //date.s
    //qDebug() << date;
    //qDebug() << date.toString();
    //qDebug() << date.toString("hh:mm");
    //qDebug() << date.toString("h:mm a");

	return 0;
}
