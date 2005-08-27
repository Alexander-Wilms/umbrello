/*
 *  copyright (C) 2004
 *  Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>
 */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// own header
#include "classifierwidget.h"
// qt/kde includes
#include <qpainter.h>
#include <kdebug.h>
// app includes
#include "classifier.h"
#include "operation.h"
#include "template.h"
#include "associationwidget.h"
#include "umlview.h"
#include "umldoc.h"
#include "uml.h"
#include "listpopupmenu.h"

ClassifierWidget::ClassifierWidget(UMLView * view, UMLClassifier *c)
        : UMLWidget(view, c) {
    init();
    if (c != NULL && c->isInterface()) {
        WidgetBase::setBaseType(Uml::wt_Interface);
        m_bShowStereotype = true;
        m_bShowAttributes = false;
        updateSigs();
    }
}

ClassifierWidget::~ClassifierWidget() {
    if (m_pAssocWidget)
        m_pAssocWidget->removeAssocClassLine();
}

const int ClassifierWidget::MARGIN = 5;
const int ClassifierWidget::CIRCLE_SIZE = 30;

void ClassifierWidget::init() {
    WidgetBase::setBaseType(Uml::wt_Class);

    const Settings::OptionState& ops = m_pView->getOptionState();
    m_bShowAccess = ops.classState.showScope;
    m_bShowOperations = ops.classState.showOps;
    m_bShowPublicOnly = false;
    m_bShowPackage = ops.classState.showPackage;
    m_ShowAttSigs = Uml::st_ShowSig;
    /* setShowOpSigs( ops.classState.showOpSig );
      Cannot do that because we get "pure virtual method called". Open code:
     */
    if( !ops.classState.showOpSig ) {
        if (m_bShowAccess)
            m_ShowOpSigs = Uml::st_NoSig;
        else
            m_ShowOpSigs = Uml::st_NoSigNoScope;

    } else if (m_bShowAccess)
        m_ShowOpSigs = Uml::st_ShowSig;
    else
        m_ShowOpSigs = Uml::st_SigNoScope;

    m_bShowAttributes = ops.classState.showAtts;
    m_bShowStereotype = ops.classState.showStereoType;
    setShowAttSigs( ops.classState.showAttSig );
    m_pAssocWidget = NULL;
    m_bDrawAsCircle = false;
}

void ClassifierWidget::updateSigs() {
    //turn on scope
    if (m_bShowAccess) {
        if (m_ShowOpSigs == Uml::st_NoSigNoScope) {
            m_ShowOpSigs = Uml::st_NoSig;
        } else if (m_ShowOpSigs == Uml::st_SigNoScope) {
            m_ShowOpSigs = Uml::st_ShowSig;
        }
    } else { //turn off scope
        if (m_ShowOpSigs == Uml::st_ShowSig) {
            m_ShowOpSigs = Uml::st_SigNoScope;
        } else if (m_ShowOpSigs == Uml::st_NoSig) {
            m_ShowOpSigs = Uml::st_NoSigNoScope;
        }
    }
    if (m_bShowAccess) {
        if (m_ShowAttSigs == Uml::st_NoSigNoScope)
            m_ShowAttSigs = Uml::st_NoSig;
        else if (m_ShowAttSigs == Uml::st_SigNoScope)
            m_ShowAttSigs = Uml::st_ShowSig;
    } else {
        if (m_ShowAttSigs == Uml::st_ShowSig)
            m_ShowAttSigs = Uml::st_SigNoScope;
        else if(m_ShowAttSigs == Uml::st_NoSig)
            m_ShowAttSigs = Uml::st_NoSigNoScope;
    }
    calculateSize();
    update();
}

bool ClassifierWidget::getShowStereotype() const {
    return m_bShowStereotype;
}

void ClassifierWidget::setShowStereotype(bool _status) {
    m_bShowStereotype = _status;
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowStereotype()
{
    m_bShowStereotype = !m_bShowStereotype;
    updateSigs();
    calculateSize();
    update();
}

bool ClassifierWidget::getShowOps() const {
    return m_bShowOperations;
}

void ClassifierWidget::setShowOps(bool _show) {
    m_bShowOperations = _show;
    updateSigs();
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowOps() {
    m_bShowOperations = !m_bShowOperations;
    updateSigs();
    calculateSize();
    update();
}

bool ClassifierWidget::getShowPublicOnly() const {
    return m_bShowPublicOnly;
}

void ClassifierWidget::setShowPublicOnly(bool _status) {
    m_bShowPublicOnly = _status;
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowPublicOnly() {
    m_bShowPublicOnly = !m_bShowPublicOnly;
    calculateSize();
    update();
}

bool ClassifierWidget::getShowScope() const {
    return m_bShowAccess;
}

void ClassifierWidget::setShowScope(bool _scope) {
    m_bShowAccess = _scope;
    updateSigs();
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowScope() {
    m_bShowAccess = !m_bShowAccess;
    updateSigs();
    calculateSize();
    update();
}

Uml::Signature_Type ClassifierWidget::getShowOpSigs() const {
    return m_ShowOpSigs;
}

void ClassifierWidget::setShowOpSigs(bool _status) {
    if( !_status ) {
        if (m_bShowAccess)
            m_ShowOpSigs = Uml::st_NoSig;
        else
            m_ShowOpSigs = Uml::st_NoSigNoScope;
    } else if (m_bShowAccess)
        m_ShowOpSigs = Uml::st_ShowSig;
    else
        m_ShowOpSigs = Uml::st_SigNoScope;
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowOpSigs() {
    if (m_ShowOpSigs == Uml::st_ShowSig || m_ShowOpSigs == Uml::st_SigNoScope) {
        if (m_bShowAccess) {
            m_ShowOpSigs = Uml::st_NoSig;
        } else {
            m_ShowOpSigs = Uml::st_NoSigNoScope;
        }
    } else if (m_bShowAccess) {
        m_ShowOpSigs = Uml::st_ShowSig;
    } else {
        m_ShowOpSigs = Uml::st_SigNoScope;
    }
    calculateSize();
    update();
}

bool ClassifierWidget::getShowPackage() const {
    return m_bShowPackage;
}

void ClassifierWidget::setShowPackage(bool _status) {
    m_bShowPackage = _status;
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowPackage() {
    m_bShowPackage = !m_bShowPackage;
    updateSigs();
    calculateSize();
    update();
}

void ClassifierWidget::setOpSignature(Uml::Signature_Type sig) {
    m_ShowOpSigs = sig;
    updateSigs();
    calculateSize();
    update();
}

void ClassifierWidget::setShowAtts(bool _show) {
    m_bShowAttributes = _show;
    updateSigs();

    calculateSize();
    update();
}

void ClassifierWidget::setAttSignature(Uml::Signature_Type sig) {
    m_ShowAttSigs = sig;
    updateSigs();
    calculateSize();
    update();
}

void ClassifierWidget::setShowAttSigs(bool _status) {
    if( !_status ) {
        if (m_bShowAccess)
            m_ShowAttSigs = Uml::st_NoSig;
        else
            m_ShowAttSigs = Uml::st_NoSigNoScope;
    }
    else if (m_bShowAccess)
        m_ShowAttSigs = Uml::st_ShowSig;
    else
        m_ShowAttSigs = Uml::st_SigNoScope;
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowAtts()
{
    m_bShowAttributes = !m_bShowAttributes;
    updateSigs();
    calculateSize();
    update();
}

void ClassifierWidget::toggleShowAttSigs()
{
    if (m_ShowAttSigs == Uml::st_ShowSig ||
            m_ShowAttSigs == Uml::st_SigNoScope) {
        if (m_bShowAccess) {
            m_ShowAttSigs = Uml::st_NoSig;
        } else {
            m_ShowAttSigs = Uml::st_NoSigNoScope;
        }
    } else if (m_bShowAccess) {
        m_ShowAttSigs = Uml::st_ShowSig;
    } else {
        m_ShowAttSigs = Uml::st_SigNoScope;
    }
    calculateSize();
    update();
}

int ClassifierWidget::displayedMembers(Uml::Object_Type ot) {
    int count = 0;
    UMLClassifierListItemList list = getClassifier()->getFilteredList(ot);
    for (UMLClassifierListItem *m = list.first(); m; m = list.next()) {
        if (!(m_bShowPublicOnly && m->getScope() != Uml::Public))
            count++;
    }
    return count;
}

int ClassifierWidget::displayedOperations() {
    if (!m_bShowOperations)
        return 0;
    return displayedMembers(Uml::ot_Operation);
}

void ClassifierWidget::calculateSize() {
    if( !m_pObject )
        return;
    if (m_bDrawAsCircle) {
        calculateAsCircleSize();
        return;
    }

    const QFontMetrics &fm = getFontMetrics(UMLWidget::FT_NORMAL);
    const int fontHeight = fm.lineSpacing();
    // width is the width of the longest 'word'
    int width = 0, height = 0;

    // consider stereotype
    if (m_bShowStereotype && !m_pObject->getStereotype(false).isEmpty()) {
        height += fontHeight;
        // ... width
        const QFontMetrics &bfm = UMLWidget::getFontMetrics(UMLWidget::FT_BOLD);
        const int stereoWidth = bfm.size(0,m_pObject->getStereotype()).width();
        if (stereoWidth > width)
            width = stereoWidth;
    }

    // consider name
    height += fontHeight;
    // ... width
    QString displayedName;
    if (m_bShowPackage)
        displayedName = m_pObject->getFullyQualifiedName();
    else
        displayedName = m_pObject->getName();
    const UMLWidget::FontType nft = (m_pObject->getAbstract() ? FT_BOLD_ITALIC : FT_BOLD);
    //const int nameWidth = getFontMetrics(nft).boundingRect(displayName).width();
    const int nameWidth = UMLWidget::getFontMetrics(nft).size(0,displayedName).width();
    if (nameWidth > width)
        width = nameWidth;

    // consider attributes
    const int numAtts = displayedAttributes();
    if (numAtts == 0) {
        height += fontHeight / 2;  // no atts, so just add a bit of space
    } else {
        height += fontHeight * numAtts;
        // calculate width of the attributes
        UMLClassifierListItemList list = getClassifier()->getFilteredList(Uml::ot_Attribute);
        for (UMLClassifierListItem *a = list.first(); a; a = list.next()) {
            if (m_bShowPublicOnly && a->getScope() != Uml::Public)
                continue;
            const int attWidth = fm.size(0,a->toString(m_ShowAttSigs)).width();
            if (attWidth > width)
                width = attWidth;
        }
    }

    // consider operations
    const int numOps = displayedOperations();
    if (numOps == 0) {
        height += fontHeight / 2;  // no ops, so just add a bit of space
    } else {
        height += numOps * fontHeight;
        // ... width
        UMLOperationList list(getClassifier()->getOpList());
        for (UMLOperation* op = list.first(); op; op = list.next()) {
            if (m_bShowPublicOnly && op->getScope() != Uml::Public)
                continue;
            const QString displayedOp = op->toString(m_ShowOpSigs);
            UMLWidget::FontType oft;
            oft = (op->getAbstract() ? UMLWidget::FT_ITALIC : UMLWidget::FT_BOLD);
            const int w = UMLWidget::getFontMetrics(oft).size(0,displayedOp).width();
            if (w > width)
                width = w;
        }
    }

    // consider template box _as last_ !
    QSize templatesBoxSize = calculateTemplatesBoxSize();
    if (templatesBoxSize.width() != 0) {
        // add width to largest 'word'
        width += templatesBoxSize.width() / 2;
    }
    if (templatesBoxSize.height() != 0) {
        height += templatesBoxSize.height() - MARGIN;
    }


    // allow for height margin
    if (!m_bShowOperations && !m_bShowAttributes && !m_bShowStereotype) {
        height += MARGIN * 2;
    }

    // allow for width margin
    width += MARGIN * 2;

    setSize(width, height);
    adjustUnselectedAssocs( getX(), getY() );
}

void ClassifierWidget::slotMenuSelection(int sel) {
    ListPopupMenu::Menu_Type mt = (ListPopupMenu::Menu_Type)sel;
    switch (mt) {
    case ListPopupMenu::mt_Attribute:
    case ListPopupMenu::mt_Operation:
        {
            UMLDoc *doc = UMLApp::app()->getDocument();
            Uml::Object_Type ot = ListPopupMenu::convert_MT_OT(mt);
            if (doc->createChildObject(m_pObject, ot))
                doc->setModified();
            calculateSize();
            update();
            break;
        }
    case ListPopupMenu::mt_Show_Operations:
    case ListPopupMenu::mt_Show_Operations_Selection:
        toggleShowOps();
        break;

    case ListPopupMenu::mt_Show_Attributes:
    case ListPopupMenu::mt_Show_Attributes_Selection:
        toggleShowAtts();
        break;

    case ListPopupMenu::mt_Show_Public_Only:
    case ListPopupMenu::mt_Show_Public_Only_Selection:
        toggleShowPublicOnly();
        break;

    case ListPopupMenu::mt_Show_Operation_Signature:
    case ListPopupMenu::mt_Show_Operation_Signature_Selection:
        toggleShowOpSigs();
        break;

    case ListPopupMenu::mt_Show_Attribute_Signature:
    case ListPopupMenu::mt_Show_Attribute_Signature_Selection:
        toggleShowAttSigs();
        break;

    case ListPopupMenu::mt_Scope:
    case ListPopupMenu::mt_Scope_Selection:
        toggleShowScope();
        break;

    case ListPopupMenu::mt_Show_Packages:
    case ListPopupMenu::mt_Show_Packages_Selection:
        toggleShowPackage();
        break;

    case ListPopupMenu::mt_Show_Stereotypes:
    case ListPopupMenu::mt_Show_Stereotypes_Selection:
        toggleShowStereotype();
        break;

    case ListPopupMenu::mt_DrawAsCircle:
    case ListPopupMenu::mt_DrawAsCircle_Selection:
        toggleDrawAsCircle();
        break;

    case ListPopupMenu::mt_ChangeToClass:
    case ListPopupMenu::mt_ChangeToClass_Selection:
        WidgetBase::m_Type = Uml::wt_Class;
        getClassifier()->setInterface(false);
        calculateSize();
        update();
        break;

    case ListPopupMenu::mt_ChangeToInterface:
    case ListPopupMenu::mt_ChangeToInterface_Selection:
        WidgetBase::m_Type = Uml::wt_Interface;
        getClassifier()->setInterface(true);
        calculateSize();
        update();
        break;

    default:
        UMLWidget::slotMenuSelection(sel);
        break;
    }
}

QSize ClassifierWidget::calculateTemplatesBoxSize() {
    UMLTemplateList list = getClassifier()->getTemplateList();
    int count = list.count();
    if (count == 0) {
        return QSize(0, 0);
    }

    int width, height;
    height = width = 0;

    QFont font = UMLWidget::getFont();
    font.setItalic(false);
    font.setUnderline(false);
    font.setBold(false);
    QFontMetrics fm(font);

    height = count * fm.lineSpacing() + (MARGIN*2);

    for (UMLTemplate *t = list.first(); t; t = list.next()) {
        int textWidth = fm.size(0, t->toString() ).width();
        if (textWidth > width)
            width = textWidth;
    }

    width += (MARGIN*2);
    return QSize(width, height);
}

int ClassifierWidget::displayedAttributes() {
    if (!m_bShowAttributes)
        return 0;
    return displayedMembers(Uml::ot_Attribute);
}

void ClassifierWidget::setClassAssocWidget(AssociationWidget *assocwidget) {
    m_pAssocWidget = assocwidget;
    UMLAssociation *umlassoc = NULL;
    if (assocwidget)
        umlassoc = assocwidget->getAssociation();
    getClassifier()->setClassAssoc(umlassoc);
}

AssociationWidget *ClassifierWidget::getClassAssocWidget() {
    return m_pAssocWidget;
}

UMLClassifier *ClassifierWidget::getClassifier() {
    return static_cast<UMLClassifier*>(m_pObject);
}

void ClassifierWidget::draw(QPainter & p, int offsetX, int offsetY) {
    UMLWidget::setPen(p);
    if ( UMLWidget::getUseFillColour() )
        p.setBrush( UMLWidget::getFillColour() );
    else
        p.setBrush( m_pView->viewport()->backgroundColor() );

    if (m_bDrawAsCircle) {
        drawAsCircle(p, offsetX, offsetY);
        return;
    }

    // Draw the bounding rectangle
    QSize templatesBoxSize = calculateTemplatesBoxSize();
    m_bodyOffsetY = offsetY;
    if (templatesBoxSize.height() > 0)
        m_bodyOffsetY += templatesBoxSize.height() - MARGIN;
    int w = width();
    if (templatesBoxSize.width() > 0)
        w -= templatesBoxSize.width() / 2;
    int h = height();
    if (templatesBoxSize.height() > 0)
        h -= templatesBoxSize.height() - MARGIN;
    p.drawRect(offsetX, m_bodyOffsetY, w, h);

    QFont font = UMLWidget::getFont();
    font.setUnderline(false);
    font.setItalic(false);
    QFontMetrics fm = UMLWidget::getFontMetrics(UMLWidget::FT_NORMAL);
    const int fontHeight = fm.lineSpacing();

    //If there are any templates then draw them
    UMLTemplateList tlist = getClassifier()->getTemplateList();
    if ( tlist.count() > 0 ) {
        UMLWidget::setPen(p);
        QPen pen = p.pen();
        pen.setStyle(DotLine);
        p.setPen(pen);
        p.drawRect( offsetX + width() - templatesBoxSize.width(), offsetY,
                    templatesBoxSize.width(), templatesBoxSize.height() );
        p.setPen( QPen(black) );
        font.setBold(false);
        p.setFont(font);
        const int x = offsetX + width() - templatesBoxSize.width() + MARGIN;
        int y = offsetY + MARGIN;
        for ( UMLTemplate *t = tlist.first(); t; t = tlist.next() ) {
            QString text = t->toString();
            p.drawText(x, y, fm.size(0,text).width(), fontHeight, AlignVCenter, text);
            y += fontHeight;
        }
    }

    const int textX = offsetX + MARGIN;
    const int textWidth = w - MARGIN * 2;

    p.setPen(QPen(black));

    // draw stereotype
    font.setBold(true);
    QString stereo = m_pObject->getStereotype(false);
    /* if no stereotype is given we don't want to show the empty << >> */
    const bool showStereotype = (m_bShowStereotype && !stereo.isEmpty());
    const bool showNameOnly = (!m_bShowOperations && !m_bShowAttributes && !showStereotype);
    int nameHeight = fontHeight;
    if (showNameOnly) {
        nameHeight = h;
    } else if (showStereotype) {
        p.setFont(font);
        stereo = m_pObject->getStereotype(true);
        p.drawText(textX, m_bodyOffsetY, textWidth, fontHeight, AlignCenter, stereo);
        m_bodyOffsetY += fontHeight;
    }

    // draw name
    QString name;
    if (m_bShowPackage) {
        name = m_pObject->getFullyQualifiedName();
    } else {
        name = this->getName();
    }
    font.setItalic( m_pObject->getAbstract() );
    p.setFont(font);
    p.drawText(textX, m_bodyOffsetY, textWidth, nameHeight, AlignCenter, name);
    if (!showNameOnly) {
        m_bodyOffsetY += fontHeight;
        UMLWidget::setPen(p);
        p.drawLine(offsetX, m_bodyOffsetY, offsetX + w - 1, m_bodyOffsetY);
        p.setPen(QPen(black));
    }
    font.setBold(false);
    font.setItalic(false);
    p.setFont(font);

    // draw attributes
    const int numAtts = displayedAttributes();
    if (m_bShowAttributes) {
        drawMembers(p, Uml::ot_Attribute, m_ShowAttSigs, textX,
                    m_bodyOffsetY, fontHeight);
    }

    // draw dividing line between attributes and operations
    if (!showNameOnly) {
        if (numAtts == 0)
            m_bodyOffsetY += fontHeight / 2;  // no atts, so just add a bit of space
        else
            m_bodyOffsetY += fontHeight * numAtts;
        UMLWidget::setPen(p);
        p.drawLine(offsetX, m_bodyOffsetY, offsetX + w - 1, m_bodyOffsetY);
        p.setPen(QPen(black));
    }

    // draw operations
    if (m_bShowOperations) {
        drawMembers(p, Uml::ot_Operation, m_ShowOpSigs, textX,
                    m_bodyOffsetY, fontHeight);
    }

    if (UMLWidget::m_bSelected)
        UMLWidget::drawSelected(&p, offsetX, offsetY);
}

void ClassifierWidget::drawAsCircle(QPainter& p, int offsetX, int offsetY) {
    int w = width();

    const QFontMetrics &fm = getFontMetrics(FT_NORMAL);
    const int fontHeight  = fm.lineSpacing();
    QString name;
    if ( m_bShowPackage ) {
        name = m_pObject->getFullyQualifiedName();
    } else {
        name = this -> getName();
    }

    p.drawEllipse(offsetX + w/2 - CIRCLE_SIZE/2, offsetY, CIRCLE_SIZE, CIRCLE_SIZE);
    p.setPen( QPen(black) );

    QFont font = UMLWidget::getFont();
    p.setFont(font);
    p.drawText(offsetX, offsetY + CIRCLE_SIZE, w, fontHeight, AlignCenter, name);

    if (m_bSelected) {
        drawSelected(&p, offsetX, offsetY);
    }
}

void ClassifierWidget::calculateAsCircleSize() {
    QFontMetrics &fm = UMLWidget::getFontMetrics(UMLWidget::FT_ITALIC_UNDERLINE);
    const int fontHeight = fm.lineSpacing();

    int height = CIRCLE_SIZE + fontHeight;

    int width = CIRCLE_SIZE;
    QString displayedName;
    if (m_bShowPackage) {
        displayedName = m_pObject->getFullyQualifiedName();
    } else {
        displayedName = m_pObject->getName();
    }
    const int nameWidth = fm.size(0,displayedName).width();
    if (nameWidth > width)
        width = nameWidth;
    width += MARGIN * 2;

    setSize(width, height);
    adjustAssocs( getX(), getY() );//adjust assoc lines
}

void ClassifierWidget::drawMembers(QPainter & p, Uml::Object_Type ot, Uml::Signature_Type sigType,
                                   int x, int y, int fontHeight) {
    QFont f = UMLWidget::getFont();
    f.setBold(false);
    UMLClassifierListItemList list = getClassifier()->getFilteredList(ot);
    for (UMLClassifierListItem *obj = list.first(); obj; obj = list.next()) {
        if (m_bShowPublicOnly && obj->getScope() != Uml::Public)
            continue;
        QString text = obj->toString(sigType);
        f.setItalic( obj->getAbstract() );
        f.setUnderline( obj->getStatic() );
        p.setFont( f );
        QFontMetrics fontMetrics(f);
        p.drawText(x, y, fontMetrics.size(0,text).width(), fontHeight, AlignVCenter, text);
        f.setItalic(false);
        f.setUnderline(false);
        p.setFont(f);
        y += fontHeight;
    }
}

void ClassifierWidget::setDrawAsCircle(bool drawAsCircle) {
    m_bDrawAsCircle = drawAsCircle;
    calculateSize();
    update();
}

bool ClassifierWidget::getDrawAsCircle() const {
    return m_bDrawAsCircle;
}

void ClassifierWidget::toggleDrawAsCircle() {
    m_bDrawAsCircle = !m_bDrawAsCircle;
    updateSigs();
    calculateSize();
    update();
}

bool ClassifierWidget::activate(IDChangeLog* ChangeLog /* = 0 */) {
    bool status = UMLWidget::activate(ChangeLog);
    if (status) {
        calculateSize();
    }
    return status;
}

void ClassifierWidget::saveToXMI(QDomDocument & qDoc, QDomElement & qElement) {
    QDomElement conceptElement;
    UMLClassifier *umlc = getClassifier();
    if (umlc->isInterface())
        conceptElement = qDoc.createElement("interfacewidget");
    else
        conceptElement = qDoc.createElement("classwidget");
    UMLWidget::saveToXMI( qDoc, conceptElement );
    conceptElement.setAttribute( "showoperations", m_bShowOperations );
    conceptElement.setAttribute( "showpubliconly", m_bShowPublicOnly );
    conceptElement.setAttribute( "showopsigs", m_ShowOpSigs );
    conceptElement.setAttribute( "showpackage", m_bShowPackage );
    conceptElement.setAttribute( "showstereotype", m_bShowStereotype );
    conceptElement.setAttribute( "showscope", m_bShowAccess );
    if (! umlc->isInterface()) {
        conceptElement.setAttribute("showattributes", m_bShowAttributes);
        conceptElement.setAttribute("showattsigs", m_ShowAttSigs);
    }
    if (umlc->isInterface() || umlc->getAbstract())
        conceptElement.setAttribute("drawascircle", m_bDrawAsCircle);
    qElement.appendChild( conceptElement );
}

bool ClassifierWidget::loadFromXMI(QDomElement & qElement) {
    if (!UMLWidget::loadFromXMI(qElement))
        return false;
    QString showatts = qElement.attribute( "showattributes", "0" );
    QString showops = qElement.attribute( "showoperations", "1" );
    QString showpubliconly = qElement.attribute( "showpubliconly", "0" );
    QString showattsigs = qElement.attribute( "showattsigs", "600" );
    QString showopsigs = qElement.attribute( "showopsigs", "600" );
    QString showpackage = qElement.attribute( "showpackage", "0" );
    QString showstereo = qElement.attribute( "showstereotype", "1" );
    QString showscope = qElement.attribute( "showscope", "0" );
    QString drawascircle = qElement.attribute("drawascircle", "0");

    m_bShowAttributes = (bool)showatts.toInt();
    m_bShowOperations = (bool)showops.toInt();
    m_bShowPublicOnly = (bool)showpubliconly.toInt();
    m_ShowAttSigs = (Uml::Signature_Type)showattsigs.toInt();
    m_ShowOpSigs = (Uml::Signature_Type)showopsigs.toInt();
    m_bShowPackage = (bool)showpackage.toInt();
    m_bShowStereotype = (bool)showstereo.toInt();
    m_bShowAccess = (bool)showscope.toInt();
    m_bDrawAsCircle = (bool)drawascircle.toInt();

    return true;
}

