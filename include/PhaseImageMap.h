#ifndef PHASEIMAGEMAP_H
#define PHASEIMAGEMAP_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QMap>
#include <QTime>
#include <QColor>
#include <QDebug>

#define DEFAULT_BLOCK_WIDTH 2
#define DEFAULT_MAP_SUNSET_BLOCK 90

class PhaseImageMap : public QObject {
    Q_OBJECT
public:
    // Cnstr.
    explicit PhaseImageMap(const QString& path, QTime startTime,
                           QTime endTime, int endIndex, QObject *parent = 0);

    // Dstr.
    ~PhaseImageMap(void);

    // Getters
    QColor &getPhaseColor(int phaseIndex) const;
    QColor &getPhaseColor(QTime timeIndex) const;
    QColor &getCurrentPhaseColor(void) const;

    int blockCount(void) const;
    int blockWidth(void) const;

    int getPhaseIndexByTime(QTime t);
    QTime getPhaseTimeByIndex(int index);

    bool imageLoaded(void) const;

signals:
    void phaseChaged(void);

public slots:
    void nextPhase(void);

    void setCurrentPhase(int index);

private:
    // Methods
    void initIndexTable(QTime startTime, QTime endTime, int sunsetIndex);
    void loadRawImage(const QString &path);

    // Properties
    QMap<int, QTime> _phaseMapping;
    QColor *colors;
    int _blockCount;
    int _currentPhaseIndex;
    bool _imageLoaded;
};

#endif // PHASEIMAGEMAP_H
