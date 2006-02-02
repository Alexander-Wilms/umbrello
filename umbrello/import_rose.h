/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *  copyright (C) 2006                                                     *
 *  Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>                  *
 ***************************************************************************/

#ifndef IMPORT_ROSE__H
#define IMPORT_ROSE__H

#include <qiodevice.h>

/**
 * Rose model import
 *
 * @author Oliver Kellogg
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
namespace Import_Rose {

    /**
     * Parse a file into the PetalNode internal tree representation
     * and then create Umbrello objects by traversing the tree.
     *
     * @return  True for success, false in case of error.
     */
    bool loadFromMDL(QIODevice & file);

}

#endif
