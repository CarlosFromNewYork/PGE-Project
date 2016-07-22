#pragma once
#ifndef HISTORYELEMENTRESIZEWATER_H
#define HISTORYELEMENTRESIZEWATER_H

#include "ihistoryelement.h"
#include <PGE_File_Formats/file_formats.h>

class HistoryElementResizeWater : public QObject, public IHistoryElement
{
    Q_OBJECT
    Q_INTERFACES(IHistoryElement)

public:
    explicit HistoryElementResizeWater(LevelPhysEnv wt, const QRect &oldSize, const QRect &newSize, QObject *parent = 0);
    virtual ~HistoryElementResizeWater();
    virtual QString getHistoryName();
    virtual void undo();
    virtual void redo();

public slots:
    void historyUndoResizeWater(const LevelPhysEnv &orig, QGraphicsItem *item);
    void historyRedoResizeWater(const LevelPhysEnv &orig, QGraphicsItem *item);
private:
    LevelPhysEnv m_physEnv;
    QRect m_oldSize;
    QRect m_newSize;
};

#endif // HISTORYELEMENTRESIZEWATER_H
