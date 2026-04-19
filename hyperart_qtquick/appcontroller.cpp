#include "appcontroller.h"
#include <QFile>
#include <QFileInfo>
#include "datareader.h"

AppController::AppController(QObject *parent)
    : QObject(parent), m_diagram(nullptr)
{
}

AppController::~AppController()
{
}

int AppController::numLayers() const
{
    return m_diagram ? m_diagram->numLayers() : 0;
}

int AppController::totalAnimationSteps() const
{
    return m_diagram ? m_diagram->animq.size() : 0;
}

int AppController::animationStep() const
{
    return m_animStep;
}

void AppController::setAnimationStep(int step)
{
    if (m_animStep != step) {
        m_animStep = step;
        emit animationStepChanged();
        emit diagramChanged();
    }
}

void AppController::nextAnimationStep()
{
    if (m_animStep < totalAnimationSteps()) {
        setAnimationStep(m_animStep < 0 ? 1 : m_animStep + 1);
    }
}

void AppController::prevAnimationStep()
{
    if (m_animStep > 0) {
        setAnimationStep(m_animStep - 1);
    }
}

void AppController::toggleLayer(int index, bool visible)
{
    if (index >= 0 && index < m_layerVisible.size()) {
        m_layerVisible[index] = visible;
        emit layersChanged();
        emit diagramChanged(); // Force canvas update
    }
}

bool AppController::isLayerVisible(int index) const
{
    if (index >= 0 && index < m_layerVisible.size()) {
        return m_layerVisible[index];
    }
    return false;
}

void AppController::openDesign(const QUrl &fileUrl)
{
    QString filePath = fileUrl.isLocalFile() ? fileUrl.toLocalFile() : fileUrl.toString();

    m_diagram.reset(DataReader::createDiagram(filePath));

    if (m_diagram) {
        // Initialize math state on the newly loaded diagram
        m_diagram->make();
        
        m_documentTitle = QFileInfo(filePath).fileName();
        m_animStep = -1;
        m_layerVisible.assign(m_diagram->numLayers(), true);
        
        emit documentTitleChanged();
        emit diagramChanged();
        emit animationStepChanged();
        emit layersChanged();
    } else {
        emit loadFailed("Failed to load or parse design file.");
    }
}
