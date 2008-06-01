/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2007-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "cmd_changeFontSelection.h"

// app includes
#include "umlwidgetcontroller.h"
#include "umlwidget.h"
#include "umlwidgetlist.h"
#include "umlnamespace.h"
#include "uml.h"
#include "umldoc.h"
#include "umlview.h"
#include "umlobject.h"
#include "classifierwidget.h"
#include "associationwidget.h"
#include "messagewidget.h"

#include <klocale.h>

namespace Uml
{
    CmdChangeFontSelection::CmdChangeFontSelection(UMLDoc * doc,UMLView *view,QFont fon)
    {

        UMLWidget * widget = view->getFirstMultiSelectedWidget();
        setText(i18n("Change Font")+ " : " + widget->getName());/*+ widget->getName()*/
        pDoc=doc;
        pView=view;
        newFont = fon;
        oldFont = widget -> getFont() ;
    }

    void CmdChangeFontSelection::undo()
    {
        pView -> selectionSetFont( oldFont );
        pDoc -> setModified(true);
    }

    void CmdChangeFontSelection::redo()
    {
        pView -> selectionSetFont( newFont );
        pDoc -> setModified(true);
    }

}
