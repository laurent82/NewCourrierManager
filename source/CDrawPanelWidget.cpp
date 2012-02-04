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
    if (!m_fileToDraw.isNull()){
        m_fileToShow = m_fileToDraw.scaledToHeight(m_height);
        repaint();
    }
}

bool CDrawPanelWidget::setFile(QString strFileName){
    if (!strFileName.isNull() && !strFileName.isEmpty()){
        if(m_fileToDraw.load(strFileName)){
            m_fileToShow = m_fileToDraw.scaledToHeight(m_height);
            m_drawImage = true;
        }
        else{
            m_drawImage = false;
            repaint();
            return false;
        }
    }
    else
        m_drawImage = false;
    m_drawZoom = false;
    repaint();
    return true;
}

void CDrawPanelWidget::paintEvent(QPaintEvent *event){
    QPixmap buffer(m_width, m_height);
    QPainter painter(&buffer);
    painter.fillRect(0,0,m_width,m_height, Qt::white);
    if (m_drawImage)
        painter.drawPixmap(0,0, m_fileToShow);
    if (m_drawZoom)
        drawZoom(&painter);
    QPainter final(this);
    final.drawPixmap(0,0, buffer);
}

void CDrawPanelWidget::drawZoom(QPainter* painter){

    float rx, ry;
    rx = (float)m_zx/(float)m_width;
    ry = (float)m_zy/(float)m_height;
    int iX = (int)(rx*m_fileToDraw.width());
    int iY = (int)(ry*m_fileToDraw.height());
    m_zoom = m_fileToDraw.copy(iX-100, iY-50, 200, 100);
    painter->drawPixmap(m_zx, m_zy, m_zoom);
}

void CDrawPanelWidget::mouseMoveEvent( QMouseEvent * event ){
    if (m_drawImage){m_drawZoom = true;
        m_zx = event->x();
        m_zy = event->y();

        if (m_zx > m_width || m_zy > m_height)
            m_drawZoom = false;
        update();
    }
    //    qDebug() << "Hello World";
}

void CDrawPanelWidget::mousePressEvent( QMouseEvent * event ){
    // if (m_drawZoomMove
}
