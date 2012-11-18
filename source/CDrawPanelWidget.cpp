#include "CDrawPanelWidget.h"

#include <QtDebug>


CDrawPanelWidget::CDrawPanelWidget(int _width, int _height, QWidget* parent):QFrame(parent)
{
    setFocusPolicy ( Qt::StrongFocus);
    setMouseTracking(true);
    this->setFrameStyle(QFrame::StyledPanel);
    m_width = _width;
    m_height = _height;
    m_drawZoom = false;
    m_drawZoomMove = true;
    m_drawImage = false;
}


void CDrawPanelWidget::setPanelSize (int _w, int _h){
    m_width = _w;
    m_height = _h;
    this->setGeometry(15,10, m_width, m_height);
    if (!m_image.isNull()){
        m_imageScaled = m_image.scaledToHeight(m_height);
        update();
    }
}

bool CDrawPanelWidget::setImage(const QImage& image)
{
    if (!image.isNull()) {
        m_image = image;
        m_imageScaled = m_image.scaledToHeight(m_height);
        m_drawImage = true;
    } else {
        m_drawImage = false;
        m_drawZoom = false;
    }
    update();
    return true;
}

void CDrawPanelWidget::paintEvent(QPaintEvent *event)
{
    QPixmap buffer(m_width, m_height);
    QPainter painter(&buffer);
    painter.fillRect(0,0,m_width,m_height, Qt::white);
    if (m_drawImage)
        painter.drawImage(0, 0, m_imageScaled);
    if (m_drawZoom)
        drawZoom(&painter);
    QPainter final(this);
    final.drawPixmap(0,0, buffer);
}

void CDrawPanelWidget::drawZoom(QPainter* painter)
{
    float rx, ry;
    rx = (float)m_zx/(float)m_width;
    ry = (float)m_zy/(float)m_height;
    int iX = (int)(rx*m_image.width());
    int iY = (int)(ry*m_image.height());
    m_zoom = m_image.copy(iX-100, iY-50, 200, 100);
    painter->drawImage(m_zx, m_zy, m_zoom);
}

void CDrawPanelWidget::mouseMoveEvent( QMouseEvent * event ){
    if (m_drawImage){m_drawZoom = true;
        m_zx = event->x();
        m_zy = event->y();

        if (m_zx > m_width || m_zy > m_height)
            m_drawZoom = false;
        update();
    }
}

void CDrawPanelWidget::mousePressEvent( QMouseEvent * event ){
}
