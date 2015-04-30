//
//  FaceTracker.h
//  interface/src/devices
//
//  Created by Andrzej Kapolka on 4/9/14.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_FaceTracker_h
#define hifi_FaceTracker_h

#include <QObject>
#include <QVector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/// Base class for face trackers (Faceshift, DDE).
class FaceTracker : public QObject {
    Q_OBJECT
    
public:
    virtual bool isActive() const { return false; }
    virtual bool isTracking() const { return false; }
    
    virtual void init();
    virtual void update(float deltaTime);
    virtual void reset();
    
    float getFadeCoefficient() const;
    
    const glm::vec3 getHeadTranslation() const;
    const glm::quat getHeadRotation() const;
    
    float getEstimatedEyePitch() const { return _estimatedEyePitch; }
    float getEstimatedEyeYaw() const { return _estimatedEyeYaw; } 
    
    int getNumBlendshapes() const { return _blendshapeCoefficients.size(); }
    bool isValidBlendshapeIndex(int index) const { return index >= 0 && index < getNumBlendshapes(); }
    const QVector<float>& getBlendshapeCoefficients() const;
    float getBlendshapeCoefficient(int index) const;

    bool isMuted() const { return _isMuted; }
    void setIsMuted(bool isMuted) { _isMuted = isMuted; }
    void toggleMute();

signals:
    void muteToggled();
    
protected:
    FaceTracker();
    virtual ~FaceTracker() {};

    glm::vec3 _headTranslation = glm::vec3(0.0f);
    glm::quat _headRotation = glm::quat();
    float _estimatedEyePitch = 0.0f;
    float _estimatedEyeYaw = 0.0f;
    QVector<float> _blendshapeCoefficients;
    
    float _relaxationStatus = 0.0f; // Between 0.0f and 1.0f
    float _fadeCoefficient = 0.0f; // Between 0.0f and 1.0f

    bool _isMuted;

    void countFrame();

private slots:
    void startFPSTimer();
    void finishFPSTimer();

private:
    bool _isCalculatingFPS;
    int _frameCount;
};

#endif // hifi_FaceTracker_h
