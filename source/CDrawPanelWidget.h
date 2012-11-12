#ifndef CDRAWPANEL_H
#define CDRAWPANEL_H

#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>

/**
  * Widget d'affichage de l'image.
  *
  */
class CDrawPanelWidget : public QFrame
{
    Q_OBJECT
public:
    CDrawPanelWidget(int , int , QWidget*);
    void setPanelSize(int, int);
    bool setImage(const QImage& image);
private:
    int m_width;
    int m_height;
    QImage m_image;
    QImage m_imageScaled;
    QImage m_zoom;
   void mousePressEvent( QMouseEvent * event );
 //   void mouseReleaseEvent( QMouseEvent * event );
    void mouseMoveEvent( QMouseEvent * event );
    bool m_drawZoom;
    bool m_drawZoomMove;
    int m_zx;
    int m_zy;
    void drawZoom(QPainter*);
     bool m_drawImage;

protected:
  void paintEvent(QPaintEvent *event);

};

#endif // CDRAWPANEL_H
