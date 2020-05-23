#include "stdafx.h"

QTextStream output(stdout);
QTextStream errout(stderr);

int main(int argc, char *argv[])
{
	QCoreApplication application(argc, argv);

	application.setApplicationVersion("0.1.2.3");
	application.setOrganizationDomain("gamingarts.com");
	application.setOrganizationName("Gaming Arts, LLC");


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
