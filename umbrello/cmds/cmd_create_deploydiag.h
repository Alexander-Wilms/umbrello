/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *  copyright (C) 2002-2008                                                *
 *  Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                   *
 ***************************************************************************/

#ifndef CMD_CREATE_DEPLOYDIAG_H
#define CMD_CREATE_DEPLOYDIAG_H

#include <QtGui/QUndoCommand>

#include "umldoc.h"
#include "umlview.h"

namespace Uml
{
    class CmdCreateDeployDiag : public QUndoCommand
    {
        public:
            explicit CmdCreateDeployDiag(UMLDoc* doc, const QString& name = "");
            ~CmdCreateDeployDiag();

            void redo();
            void undo();

        private:
            UMLDoc*     m_pUMLDoc;
            UMLView*    m_pUMLView;
            QString     m_Name;
    };
}

#endif
