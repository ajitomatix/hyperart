#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <vector>
#include "diagram.h"

// Bridges Qt UI interactions with the core backend Diagram logic.
class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasDiagram READ hasDiagram NOTIFY diagramChanged)
    Q_PROPERTY(int numLayers READ numLayers NOTIFY diagramChanged)
    Q_PROPERTY(int totalAnimationSteps READ totalAnimationSteps NOTIFY diagramChanged)
    Q_PROPERTY(int animationStep READ animationStep WRITE setAnimationStep NOTIFY animationStepChanged)
    Q_PROPERTY(QString documentTitle READ documentTitle NOTIFY documentTitleChanged)

public:
    explicit AppController(QObject *parent = nullptr);
    ~AppController();

    Q_INVOKABLE void openDesign(const QUrl &fileUrl);
    Q_INVOKABLE void toggleLayer(int index, bool visible);
    Q_INVOKABLE bool isLayerVisible(int index) const;

    int numLayers() const;
    int totalAnimationSteps() const;
    int animationStep() const;
    void setAnimationStep(int step);
    
    Q_INVOKABLE void nextAnimationStep();
    Q_INVOKABLE void prevAnimationStep();

    QString documentTitle() const { return m_documentTitle; }
    Diagram* getDiagram() const { return m_diagram.get(); }
    bool hasDiagram() const { return m_diagram != nullptr; }

signals:
    void loadFailed(const QString &message);
    void diagramChanged();
    void layersChanged();
    void animationStepChanged();
    void documentTitleChanged();

private:
    std::unique_ptr<Diagram> m_diagram;
    std::vector<bool> m_layerVisible;
    int m_animStep = -1;
    QString m_documentTitle;
};

#endif // APPCONTROLLER_H
