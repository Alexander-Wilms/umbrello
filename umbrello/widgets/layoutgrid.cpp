/***************************************************************************
 * Copyright (C) 2011 by Andi Fischer <andi.fischer@hispeed.ch>            *
 *                                                                         *
 * This is free software; you can redistribute it and/or modify            *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2, or (at your option)     *
 * any later version.                                                      *
 *                                                                         *
 * This software is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this package; see the file COPYING.  If not, write to        *
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,   *
 * Boston, MA 02110-1301, USA.                                             *
 ***************************************************************************/

#include "layoutgrid.h"

#include "debug_utils.h"
#include "umlscene.h"
#include "umlview.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QTextStream>

/**
 * Constructor.
 */
LayoutGrid::LayoutGrid(QGraphicsItem *parent, UMLScene *scene)
  : QGraphicsItem(parent, scene),
    m_gridSpacingX(25),
    m_gridSpacingY(25),
    m_gridRect(scene->sceneRect().toRect()),
    m_textFont(QFont()),
    m_gridDotColor(Qt::gray),
    m_gridCrossColor(Qt::gray),
    m_textColor(Qt::gray),
    m_isVisible(false),
    m_isTextVisible(false)
{
    DEBUG_REGISTER("LayoutGrid");

    m_coordLabel = new QGraphicsTextItem(QString("0, 0"), this);
}

/**
 * Destructor.
 */
LayoutGrid::~LayoutGrid()
{
}

QRectF LayoutGrid::boundingRect() const
{
    return QRectF(m_gridRect);
}

void LayoutGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item); Q_UNUSED(widget);
    DEBUG("LayoutGrid") << "painting...";
    if (m_isVisible) {
        painter->setPen(Qt::black);
        painter->setFont(m_textFont);
        QGraphicsView *view = scene()->views()[0];
        QRectF rect = view->mapToScene(view->viewport()->geometry()).boundingRect();
        DEBUG("LayoutGrid") << view->viewport()->geometry() << rect << rect.width()/m_gridSpacingX << rect.height()/m_gridSpacingY << rect.width()/m_gridSpacingX * rect.height()/m_gridSpacingY;
        bool showDots1 = rect.width() <= 1000;
        bool showDots2 = rect.width() <= 2000;
        bool showDots4 = rect.width() <= 4000;
        bool showDots5 = rect.width() <= 5000;

        for(int x = m_gridRect.left(); x < m_gridRect.right(); x += m_gridSpacingX) {
            for(int y = m_gridRect.top(); y < m_gridRect.bottom(); y += m_gridSpacingY) {
                // limit to visible area
                if (x <= rect.left() || y < rect.top() || x >= rect.right() || y >= rect.bottom())
                    continue;
                if (x % 100 == 0 && y % 100 == 0) {
                    // cross
                    painter->drawLine(x, y-2, x, y+2);
                    painter->drawLine(x-2, y, x+2, y);
                    // text
                    if (m_isTextVisible) {
                        painter->drawText(x,y, QString("%1,%2").arg(x).arg(y));
                    }
                } else if (showDots1){
                    //painter->drawEllipse(x, y, 1, 1);
                    painter->drawPoint(x, y);
                } else if (showDots2) {
                    if (x % (m_gridSpacingX*2) == 0 && y % (m_gridSpacingY*2) == 0)
                        painter->drawPoint(x, y);
                } else if (showDots4) {
                    if (x % (m_gridSpacingX*4) == 0 && y % (m_gridSpacingY*4) == 0)
                        painter->drawPoint(x, y);
                } else if (showDots5) {
                    if (x % (m_gridSpacingX*8) == 0 && y % (m_gridSpacingY*8) == 0)
                        painter->drawPoint(x, y);
                }
            }
        }
    }
}

QRect LayoutGrid::gridRect() const
{
    return m_gridRect;
}

void LayoutGrid::setGridRect(const QRect& rect)
{
    if (m_gridRect != rect) {
        DEBUG("LayoutGrid") << "to rect: " << rect;
        m_gridRect = rect;
        update();
    }
}

int LayoutGrid::gridSpacingX() const
{
    return m_gridSpacingX;
}

int LayoutGrid::gridSpacingY() const
{
    return m_gridSpacingY;
}

void LayoutGrid::setGridSpacing(int sizeX, int sizeY)
{
    DEBUG("LayoutGrid") << "x = " << sizeX << " / y = " << sizeY;
    m_gridSpacingX= sizeX;
    m_gridSpacingY= sizeY;
}

const QColor& LayoutGrid::gridDotColor() const
{
    return m_gridDotColor;
}

void LayoutGrid::setGridDotColor(const QColor& color)
{
    DEBUG("LayoutGrid") << "color = " << color;
    m_gridDotColor = color;
}

const QColor& LayoutGrid::gridCrossColor() const
{
    return m_gridCrossColor;
}

void LayoutGrid::setGridCrossColor(const QColor& color)
{
    DEBUG("LayoutGrid") << "color = " << color;
    m_gridCrossColor = color;
}

const QColor& LayoutGrid::textColor() const
{
    return m_textColor;
}

void LayoutGrid::setTextColor(const QColor& color)
{
    DEBUG("LayoutGrid") << "color = " << color;
    m_textColor = color;
}

QFont LayoutGrid::textFont() const
{
    return m_textFont;
}

void LayoutGrid::setTextFont(const QFont& font)
{
    DEBUG("LayoutGrid") << "font = " << font;
    m_textFont = font;
}

bool LayoutGrid::isVisible() const
{
    return m_isVisible;
}

void LayoutGrid::setVisible(bool visible)
{
    if (m_isVisible != visible) {
        DEBUG("LayoutGrid") << "visible = " << visible;
        m_isVisible = visible;
        update();
    }
}

bool LayoutGrid::isTextVisible() const
{
    return m_isTextVisible;
}

void LayoutGrid::setTextVisible(bool visible)
{
    DEBUG("LayoutGrid") << "visible = " << visible;
    m_isTextVisible = visible;
}

void LayoutGrid::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint pos = event->pos().toPoint();
    DEBUG("LayoutGrid") << "at position: " << pos;
    if (event->button() == Qt::LeftButton) {
        QString pointString;
        QTextStream stream(&pointString);
        stream << pos.x() << ", " << pos.y();
        m_coordLabel->setPlainText(pointString); 
        m_coordLabel->setPos(pos.x() + 10, pos.y() - 10);
        m_coordLabel->show();
    }
}

void LayoutGrid::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_coordLabel->isVisible()) {
        m_coordLabel->hide();
    }
}
