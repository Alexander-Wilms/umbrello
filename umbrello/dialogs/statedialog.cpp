/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qlayout.h>

//kde includes
#include <kiconloader.h>
#include <klocale.h>

//local includes
#include "../umlview.h"
#include "../statewidget.h"
#include "statedialog.h"

StateDialog::StateDialog( UMLView * pView, StateWidget * pWidget ) : KDialogBase(IconList, i18n("Properties"), Ok | Apply | Cancel | Help, Ok, pView, "_STATEDIALOG_", true, true) {
	m_pActivityPage = 0;
	m_pView = pView;
	m_pStateWidget = pWidget;
	m_bChangesMade = false;
	setupPages();
}

void StateDialog::slotOk() {
	applyPage( GeneralPage );
	applyPage( Activity_Page );
	applyPage( ColorPage );
	applyPage( FontPage );
	accept();
}

void StateDialog::slotApply() {
	applyPage( (Page) activePageIndex() );
}

void StateDialog::setupPages() {
	setupGeneralPage();
	if( m_pStateWidget -> getStateType() == StateWidget::Normal )
		setupActivityPage();
	setupColorPage();
	setupFontPage();
}

void StateDialog::applyPage( Page page ) {
	m_bChangesMade = true;
	switch( page ) {
		case GeneralPage:
			m_pStateWidget -> setName( m_GenPageWidgets.nameLE -> text() );
			m_pStateWidget -> setDoc( m_GenPageWidgets.docMLE -> text() );
			break;

		case Activity_Page:
			if( m_pActivityPage )
				m_pActivityPage -> updateActivities();
			break;

		case ColorPage:
			m_pColorPage -> updateUMLWidget();

		case FontPage:
			m_pStateWidget -> setFont( m_pChooser -> font() );
			break;
	}//end switch
}

void StateDialog::setupGeneralPage() {
	QString types[ ] = { i18n("Initial state"), i18n("State"), i18n("End state") };
	StateWidget::StateType type = m_pStateWidget -> getStateType();

	QVBox * page = addVBoxPage( i18n("General"), i18n("General properties"), DesktopIcon( "misc") );
	m_GenPageWidgets.generalGB = new QGroupBox( i18n( "Properties"), (QWidget *)page );

	QGridLayout * generalLayout = new QGridLayout( m_GenPageWidgets.generalGB, 2, 2 );
	generalLayout -> setSpacing( spacingHint() );
	generalLayout -> setMargin(  fontMetrics().height()  );

	m_GenPageWidgets.typeL = new QLabel( i18n("State type"), m_GenPageWidgets.generalGB );
	generalLayout -> addWidget( m_GenPageWidgets.typeL, 0, 0 );

	m_GenPageWidgets.typeLE = new QLineEdit( types[ (int)type ], m_GenPageWidgets.generalGB );
	generalLayout -> addWidget( m_GenPageWidgets.typeLE, 0, 1 );
	m_GenPageWidgets.typeLE -> setEnabled( false );

	m_GenPageWidgets.nameL = new QLabel( i18n("State name"), m_GenPageWidgets.generalGB );
	generalLayout -> addWidget( m_GenPageWidgets.nameL, 1, 0 );

	m_GenPageWidgets.nameLE = new QLineEdit( m_GenPageWidgets.generalGB );
	generalLayout -> addWidget( m_GenPageWidgets.nameLE, 1, 1 );

	m_GenPageWidgets.docGB = new QGroupBox( i18n( "Documentation"), (QWidget *)page );

	QHBoxLayout * docLayout = new QHBoxLayout( m_GenPageWidgets.docGB );
	docLayout -> setSpacing( spacingHint() );
	docLayout -> setMargin(  fontMetrics().height()  );

	m_GenPageWidgets.docMLE = new QMultiLineEdit( m_GenPageWidgets.docGB );
	m_GenPageWidgets.docMLE -> setText( m_pStateWidget -> getDoc() );
	docLayout -> addWidget( m_GenPageWidgets.docMLE );

	if( type != StateWidget::Normal ) {
		m_GenPageWidgets.nameLE -> setEnabled( false );
		m_GenPageWidgets.nameLE -> setText( "" );
	} else
		m_GenPageWidgets.nameLE -> setText( m_pStateWidget -> getName() );
}

void StateDialog::setupFontPage() {
	QVBox * page = addVBoxPage( i18n("Font"), i18n("Font settings"), DesktopIcon( "fonts")  );
	m_pChooser = new KFontChooser( (QWidget*)page, "font", false, QStringList(), false);
	m_pChooser -> setFont( m_pStateWidget -> getFont() );
	m_pChooser->setSampleText("Umbrello font");
}

void StateDialog::setupColorPage() {
	QFrame * colorPage = addPage( i18n("Colour"), i18n("Widget Colour"), DesktopIcon( "colors") );
	QHBoxLayout * m_pColorLayout = new QHBoxLayout(colorPage);
	m_pColorPage = new UMLWidgetColorPage( colorPage, m_pStateWidget );
	m_pColorLayout -> addWidget(m_pColorPage);
}

void StateDialog::setupActivityPage() {
	QFrame * activityPage = addPage( i18n("Activities"), i18n("Activities"), DesktopIcon( "misc") );
	QHBoxLayout * activityLayout = new QHBoxLayout( activityPage );
	m_pActivityPage = new ActivityPage( activityPage, m_pStateWidget );
	activityLayout -> addWidget( m_pActivityPage );
}





#include "statedialog.moc"
