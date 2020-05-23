// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "MainView.h"
#include "CoreFit.h"
#include "ui_MainView.h"

static int windowNumber = 1;

MainView::MainView(QWidget* pParent)
	: QWidget(pParent)
	, ui(new Ui::MainView)
	, m_windowNumStr(QString::number(windowNumber++))
{
    ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

//	if (!pFile)
//	{
//		m_title = tr("untitled ") % m_windowNumStr;
//	}
//	else
//	{
//		m_title = pFile->GetTitle();
//	}
//
//	setWindowTitle(m_title);
}

MainView::~MainView()
{
    delete ui;
}

void MainView::paintEvent(QPaintEvent* /*pEvent*/)
{
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.save();

	QPen blackPen = QPen(Qt::black);
	blackPen.setWidth(1);

	painter.setPen(blackPen);

	painter.drawLine(rect().topLeft(), rect().bottomRight());
	painter.drawLine(rect().topRight(), rect().bottomLeft());

	QFont textFont;
	textFont.setPixelSize(24);

	painter.setFont(textFont);
	painter.drawText(this->rect(), Qt::AlignCenter, QStringLiteral("Window ") % m_windowNumStr);

	painter.restore();
	painter.end();
}
