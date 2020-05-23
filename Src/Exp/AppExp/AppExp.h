#pragma once

#include "CoreLib/Arguments.h"

extern QTextStream output;
extern QTextStream errOut;

QString TestUsage();
// If return is < 0, usage will be shown
int AppExp(QStringList args);