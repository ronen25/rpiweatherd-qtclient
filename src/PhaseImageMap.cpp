#include "PhaseImageMap.h"

PhaseImageMap::PhaseImageMap(const QString &path, QTime startTime,
                             QTime endTime, int endIndex, QObject *parent) :
                             QObject(parent),
                             _currentPhaseIndex(0),
                             _imageLoaded(false) {
    // Load image and initialize indexes
    loadRawImage(path);
    initIndexTable(startTime, endTime, endIndex);
}

PhaseImageMap::~PhaseImageMap(void) {
    // Delete array
    delete[] colors;
}

void PhaseImageMap::loadRawImage(const QString &path) {
    QImage img;

    // Attempt to open image
    if (!img.load(path))
        return;

    // Get block count
    _blockCount = img.size().width() / DEFAULT_BLOCK_WIDTH;

    // Initialize colors array and get blocks
    colors = new QColor[_blockCount];
    for (int i = 0, x = 0, y = img.height() / 2; i < _blockCount; i++, x += DEFAULT_BLOCK_WIDTH) {
        // Sample color
        colors[i] = QColor(img.pixel(x, y));
    }

    // Finally, set boolean
    _imageLoaded = true;
}

void PhaseImageMap::initIndexTable(QTime startTime, QTime endTime, int sunsetIndex) {
    // Get day length
    int dayLength = startTime.secsTo(endTime) / 60;
    int nightLength = 1440 - dayLength;
    int i;
    QTime currentTime = startTime;

    // Calculate blocks and delta
    int blocksDaylight = sunsetIndex,
        blocksNight = _blockCount - sunsetIndex,
        daylightDelta = dayLength / blocksDaylight * 60,
        nightDelta = nightLength / blocksNight * 60;

    // Populate
    for (i = 0; i < blocksDaylight; i++, currentTime = currentTime.addSecs(daylightDelta))
        _phaseMapping.insert(i, currentTime);

    currentTime = endTime;
    for (int ii = 0; ii < blocksNight; ii++, currentTime = currentTime.addSecs(nightDelta))
        _phaseMapping.insert(ii + i, currentTime);

    // DONE!
}

QColor &PhaseImageMap::getPhaseColor(int phaseIndex) const {
    return colors[phaseIndex];
}

QColor &PhaseImageMap::getPhaseColor(QTime timeIndex) const {
    QTime ptr = _phaseMapping.value(0);
    QMap<int, QTime>::const_iterator iter, endIter;
    bool forward = true;

    // Go through every key value pair.
    // Stop when the pair tested value is bigger then the timeIndex.
    forward = timeIndex > ptr;

    for (forward ? iter = _phaseMapping.constBegin() : iter = _phaseMapping.constEnd();
         iter != endIter; forward ? ++iter : --iter)
        if (*iter > timeIndex) {
            ptr = *iter;
            break;
        }

    return colors[this->_phaseMapping.key(ptr)];
}

QColor &PhaseImageMap::getCurrentPhaseColor() const {
    return colors[this->_currentPhaseIndex];
}

int PhaseImageMap::blockCount(void) const {
    return this->_blockCount;
}

int PhaseImageMap::blockWidth(void) const {
    return DEFAULT_BLOCK_WIDTH;
}

QTime PhaseImageMap::getPhaseTimeByIndex(int index) {
    return _phaseMapping.value(index);
}

bool PhaseImageMap::imageLoaded() const {
    return _imageLoaded;
}

void PhaseImageMap::setCurrentPhase(int index) {
    _currentPhaseIndex = index;
}

void PhaseImageMap::nextPhase(void) {
    if (_currentPhaseIndex == _blockCount)
        _currentPhaseIndex = 0;
    else
        _currentPhaseIndex++;

    // Emit signal
    emit phaseChaged();
}
