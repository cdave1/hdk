/****************************************************************************
 **
 ** Copyright (C) 2005-2005 Trolltech AS. All rights reserved.
 **
 ** This file is part of the documentation of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://www.trolltech.com/products/qt/opensource.html
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://www.trolltech.com/products/qt/licensing.html or contact the
 ** sales department at sales@trolltech.com.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

#include <QtGui>


#include "glwidget.h"
#include "window.h"

Window::Window()
{
	glWidget = new GLWidget;
	
	xSlider = createSlider(SIGNAL(xRotationChanged(int)),
						   SLOT(setXRotation(int)));
	ySlider = createSlider(SIGNAL(yRotationChanged(int)),
						   SLOT(setYRotation(int)));
	zSlider = createSlider(SIGNAL(zRotationChanged(int)),
						   SLOT(setZRotation(int)));
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->addWidget(glWidget);
        mainLayout->addWidget(xSlider);
        mainLayout->addWidget(ySlider);
        mainLayout->addWidget(zSlider);
	setLayout(mainLayout);
	
        xSlider->setValue(15 * 16);
        ySlider->setValue(345 * 16);
        zSlider->setValue(0 * 16);
        setWindowTitle(tr("Hello GL"));
	
	
}

QSlider *Window::createSlider(const char *changedSignal, const char *setterSlot)
{
	QSlider *slider = new QSlider(Qt::Vertical);
        slider->setRange(0, 360 * 16);
        slider->setSingleStep(16);
        slider->setPageStep(15 * 16);
        slider->setTickInterval(15 * 16);
        slider->setTickPosition(QSlider::TicksRight);
	connect(slider, SIGNAL(valueChanged(int)), glWidget, setterSlot);
	connect(glWidget, changedSignal, slider, SLOT(setValue(int)));
	return slider;
}
