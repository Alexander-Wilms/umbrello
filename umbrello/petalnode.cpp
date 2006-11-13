/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006                                                    *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "petalnode.h"

PetalNode::PetalNode(NodeType nt) {
    m_type = nt;
}

PetalNode::~PetalNode() {
}

PetalNode::NodeType PetalNode::type() const {
    return m_type;
}

QStringList PetalNode::initialArgs() const {
    return m_initialArgs;
}

QString PetalNode::name() const {
    if (m_initialArgs.count() == 0)
        return QString::null;
    return m_initialArgs.first();
}

PetalNode::NameValueList PetalNode::attributes() const {
    return m_attributes;
}

/*
void PetalNode::setType(PetalNode::NodeType t) {
    m_type = t;
}
 */

void PetalNode::setInitialArgs(QStringList args) {
    m_initialArgs = args;
}

void PetalNode::setAttributes(PetalNode::NameValueList vl) {
    m_attributes = vl;
}

PetalNode::StringOrNode PetalNode::findAttribute(QString name) const {
    for (uint i = 0; i < m_attributes.count(); i++) {
        if (m_attributes[i].first == name)
            return m_attributes[i].second;
    }
    return StringOrNode();
}

