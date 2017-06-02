#ifndef PAGECONTENT_H
#define PAGECONTENT_H

#include <QtCore>
#include <QtWidgets>

#include "exif.h"

/**
 * @brief The ImageContent struct
 * actual Image data and metadata
 */
struct ImageContent
{
public:
    /**
     * @brief Image is a pixmap of the image for viewing
     */
    QPixmap Image;
    /**
     * @brief ResizedImage is resized to actual view size from Image
     */
    QPixmap ResizedImage;
    /**
     * @brief BaseSize is original size of the image
     */
    QSize BaseSize;
    /**
     * @brief ImportSize is actual size of image for viewing
     */
    QSize ImportSize;
    /**
     * @brief Path is path of the image
     */
    QString Path;
    /**
     * @brief Info is Exif Information of the image(JPEG only)
     */
    easyexif::EXIFInfo Info;

    ImageContent(){}
    ImageContent(QPixmap image, QString path, QSize size, easyexif::EXIFInfo info)
        : Image(image), Path(path), BaseSize(size), ImportSize(image.size()), Info(info) {}
    ImageContent(const ImageContent& rhs)
        : Image(rhs.Image), ResizedImage(rhs.ResizedImage), Path(rhs.Path), BaseSize(rhs.BaseSize), ImportSize(rhs.ImportSize), Info(rhs.Info) {}
    inline ImageContent& operator=(const ImageContent &rhs)
    {
        Image = rhs.Image;
        ResizedImage = rhs.ResizedImage;
        Path = rhs.Path;
        BaseSize = rhs.BaseSize;
        ImportSize = rhs.ImportSize;
        Info = rhs.Info;
        return *this;
    }
    bool wideImage() const {return BaseSize.width() > BaseSize.height(); }
};

/**
 * @brief The ISizeNotifier class
 */
class ISizeNotifier
{
public:
    virtual QSize size()=0;
};


/**
 * @brief PageContent
 * contains the informations of a Page
 */
class PageContent : public QObject
{
    Q_OBJECT
public:
    QGraphicsScene *Scene;
    ImageContent Ic;
    /**
     * @brief GrItem
     * Page image is used as a QGraphicsItem. it will be registed to the scene
     */
    QGraphicsItem* GrItem;
//    /**
//     * @brief Resized
//     * Store the image changed to the specified size (newsize)
//     */
//    QPixmap ResizedPage;
    QFutureWatcher<QImage> generateWatcher;
    /**
     * @brief Rotate: rotation as digrees
     */
    int Rotate;
    enum Fitting {
        FitCenter,
        FitLeft,
        FitRight
    };
    PageContent(QObject* parent=nullptr);
    PageContent(QObject* parent, QGraphicsScene *s, ImageContent ic);
    PageContent(const PageContent& rhs);
    PageContent& operator=(const PageContent& rhs);

    QPoint Offset(int rotateOffset=0);
    QSize CurrentSize(int rotateOffset=0);

    /**
     * @brief setPageLayout set each image on the page
     * @param viewport: the image must be inscribed in the viewport area
     */
    QRect setPageLayoutFitting(QRect viewport, Fitting fitting, int rotateOffset=0);
    QRect setPageLayoutManual(QRect viewport, Fitting fitting, qreal scale, int rotateOffset=0);

    void applyResize(qreal scale, int rotateOffset, QPoint pos, QSize newsize);
    void initializePage(bool resetResized=false);
    void resetScene(QGraphicsScene* scene);
signals:
    void resizeFinished();
public slots:
    void on_resizeFinished_trigger();

private:
    int m_resizeGeneratingState;
};

#endif // PAGECONTENT_H
