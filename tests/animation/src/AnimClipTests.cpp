//
//  AnimClipTests.cpp
//
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "AnimClipTests.h"
#include "AnimNodeLoader.h"
#include "AnimClip.h"
#include "AnimBlendLinear.h"
#include "AnimationLogging.h"

#include <../QTestExtensions.h>

QTEST_MAIN(AnimClipTests)

const float EPSILON = 0.001f;

void AnimClipTests::initTestCase() {
    auto animationCache = DependencyManager::set<AnimationCache>();
    auto resourceCacheSharedItems = DependencyManager::set<ResourceCacheSharedItems>();
}

void AnimClipTests::cleanupTestCase() {
    DependencyManager::destroy<AnimationCache>();
}

void AnimClipTests::testAccessors() {
    std::string id = "my anim clip";
    std::string url = "foo";
    float startFrame = 2.0f;
    float endFrame = 20.0f;
    float timeScale = 1.1f;
    bool loopFlag = true;

    AnimClip clip(id, url, startFrame, endFrame, timeScale, loopFlag);

    QVERIFY(clip.getID() == id);
    QVERIFY(clip.getType() == AnimNode::ClipType);

    QVERIFY(clip.getURL() == url);
    QVERIFY(clip.getStartFrame() == startFrame);
    QVERIFY(clip.getEndFrame() == endFrame);
    QVERIFY(clip.getTimeScale() == timeScale);
    QVERIFY(clip.getLoopFlag() == loopFlag);

    std::string url2 = "bar";
    float startFrame2 = 22.0f;
    float endFrame2 = 100.0f;
    float timeScale2 = 1.2f;
    bool loopFlag2 = false;

    clip.setURL(url2);
    clip.setStartFrame(startFrame2);
    clip.setEndFrame(endFrame2);
    clip.setTimeScale(timeScale2);
    clip.setLoopFlag(loopFlag2);

    QVERIFY(clip.getURL() == url2);
    QVERIFY(clip.getStartFrame() == startFrame2);
    QVERIFY(clip.getEndFrame() == endFrame2);
    QVERIFY(clip.getTimeScale() == timeScale2);
    QVERIFY(clip.getLoopFlag() == loopFlag2);
}

static float framesToSec(float secs) {
    const float FRAMES_PER_SECOND = 30.0f;
    return secs / FRAMES_PER_SECOND;
}

void AnimClipTests::testEvaulate() {
    std::string id = "my clip node";
    std::string url = "foo";
    float startFrame = 2.0f;
    float endFrame = 22.0f;
    float timeScale = 1.0f;
    float loopFlag = true;

    AnimClip clip(id, url, startFrame, endFrame, timeScale, loopFlag);

    clip.evaluate(framesToSec(10.0f));
    QCOMPARE_WITH_ABS_ERROR(clip._frame, 12.0f, EPSILON);

    // does it loop?
    clip.evaluate(framesToSec(11.0f));
    QCOMPARE_WITH_ABS_ERROR(clip._frame, 3.0f, EPSILON);

    // does it pause at end?
    clip.setLoopFlag(false);
    clip.evaluate(framesToSec(20.0f));
    QCOMPARE_WITH_ABS_ERROR(clip._frame, 22.0f, EPSILON);
}

void AnimClipTests::testLoader() {
    AnimNodeLoader loader;

#ifdef Q_OS_WIN
    auto node = loader.load("../../../tests/animation/src/data/test.json");
#else
    auto node = loader.load("../../../../tests/animation/src/data/test.json");
#endif

    QVERIFY((bool)node);
    QVERIFY(node->getID() == "blend");
    QVERIFY(node->getType() == AnimNode::BlendLinearType);

    auto blend = std::static_pointer_cast<AnimBlendLinear>(node);
    QVERIFY(blend->getAlpha() == 0.5f);

    QVERIFY(node->getChildCount() == 3);

    std::shared_ptr<AnimNode> nodes[3] = { node->getChild(0), node->getChild(1), node->getChild(2) };

    QVERIFY(nodes[0]->getID() == "test01");
    QVERIFY(nodes[0]->getChildCount() == 0);
    QVERIFY(nodes[1]->getID() == "test02");
    QVERIFY(nodes[1]->getChildCount() == 0);
    QVERIFY(nodes[2]->getID() == "test03");
    QVERIFY(nodes[2]->getChildCount() == 0);

    auto test01 = std::static_pointer_cast<AnimClip>(nodes[0]);
    QVERIFY(test01->getURL() == "test01.fbx");
    QVERIFY(test01->getStartFrame() == 1.0f);
    QVERIFY(test01->getEndFrame() == 20.0f);
    QVERIFY(test01->getTimeScale() == 1.0f);
    QVERIFY(test01->getLoopFlag() == false);

    auto test02 = std::static_pointer_cast<AnimClip>(nodes[1]);
    QVERIFY(test02->getURL() == "test02.fbx");
    QVERIFY(test02->getStartFrame() == 2.0f);
    QVERIFY(test02->getEndFrame() == 21.0f);
    QVERIFY(test02->getTimeScale() == 0.9f);
    QVERIFY(test02->getLoopFlag() == true);
}
