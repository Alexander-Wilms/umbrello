#ifndef IDLIMPORT_H
#define IDLIMPORT_H

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *  copyright (C) 2005                                                     *
 *  Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>                  *
 ***************************************************************************/

#include <qstring.h>
#include <qstringlist.h>
#include "classimport.h"
#include "umlnamespace.h"

class UMLPackage;
class UMLClassifier;

/**
 * CORBA IDL code import
 * @author Oliver Kellogg
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class IDLImport : public ClassImport {
public:
    IDLImport();
    virtual ~IDLImport();

    /**
     * Implement abstract operation from ClassImport for IDL.
     */
    void importFiles(QStringList idlFiles);

protected:
    void skipStmt(QString until = ";");
    QString joinTypename();
    void scan(QString line);
    void parseFile(QString file);

    QStringList m_source;
    int m_srcIndex;
    UMLPackage *m_scope[32];
    int m_scopeIndex;  // index 0 is reserved for global scope
    UMLClassifier *m_klass;
    bool m_isAbstract, m_isOneway, m_isReadonly, m_isAttribute;
    Uml::Scope m_currentAccess;
    bool m_inComment;
    QString m_comment;
};

#endif

