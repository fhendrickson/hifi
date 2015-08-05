//
//  AnimBlendLinear.cpp
//
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "AnimBlendLinear.h"
#include "GLMHelpers.h"
#include "AnimationLogging.h"

AnimBlendLinear::AnimBlendLinear(const std::string& id, float alpha) :
    AnimNode(AnimNode::BlendLinearType, id),
    _alpha(alpha) {

}

AnimBlendLinear::~AnimBlendLinear() {

}

const std::vector<AnimPose>& AnimBlendLinear::evaluate(float dt) {

    if (_children.size() == 0) {
        for (auto&& pose : _poses) {
            pose = AnimPose::identity;
        }
    } else if (_children.size() == 1) {
        _poses = _children[0]->evaluate(dt);
    } else {
        float clampedAlpha = glm::clamp(_alpha, 0.0f, (float)(_children.size() - 1));
        size_t prevPoseIndex = glm::floor(clampedAlpha);
        size_t nextPoseIndex = glm::ceil(clampedAlpha);
        float alpha = glm::fract(clampedAlpha);
        if (prevPoseIndex != nextPoseIndex) {
            auto prevPoses = _children[prevPoseIndex]->evaluate(dt);
            auto nextPoses = _children[nextPoseIndex]->evaluate(dt);

            if (prevPoses.size() > 0 && prevPoses.size() == nextPoses.size()) {
                _poses.resize(prevPoses.size());
                for (size_t i = 0; i < _poses.size(); i++) {
                    const AnimPose& prevPose = prevPoses[i];
                    const AnimPose& nextPose = nextPoses[i];
                    _poses[i].scale = lerp(prevPose.scale, nextPose.scale, alpha);
                    _poses[i].rot = glm::normalize(glm::lerp(prevPose.rot, nextPose.rot, alpha));
                    _poses[i].trans = lerp(prevPose.trans, nextPose.trans, alpha);
                }
            }
        }
    }
    return _poses;
}

// for AnimDebugDraw rendering
const std::vector<AnimPose>& AnimBlendLinear::getPosesInternal() const {
    return _poses;
}
