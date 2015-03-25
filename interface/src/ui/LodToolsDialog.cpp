//
//  LodToolsDialog.cpp
//  interface/src/ui
//
//  Created by Brad Hefta-Gaub on 7/19/13.
//  Copyright 2013 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include <QCheckBox>
#include <QColor>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QPalette>
#include <QPushButton>
#include <QSlider>
#include <QString>

#include <LODManager.h>

#include "Menu.h"
#include "ui/LodToolsDialog.h"


LodToolsDialog::LodToolsDialog(QWidget* parent) :
    QDialog(parent, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint) 
{
    this->setWindowTitle("LOD Tools");
    auto lodManager = DependencyManager::get<LODManager>();

    // Create layouter
    QFormLayout* form = new QFormLayout(this);

    // Create a label with feedback...
    _feedback = new QLabel(this);
    QPalette palette = _feedback->palette();
    const unsigned redish  = 0xfff00000;
    palette.setColor(QPalette::WindowText, QColor::fromRgb(redish));
    _feedback->setPalette(palette);
    _feedback->setText(lodManager->getLODFeedbackText());
    const int FEEDBACK_WIDTH = 350;
    _feedback->setFixedWidth(FEEDBACK_WIDTH);
    form->addRow("You can see... ", _feedback);

    form->addRow("Automatic LOD Adjustment:", _automaticLODAdjust = new QCheckBox(this));
    _automaticLODAdjust->setChecked(lodManager->getAutomaticLODAdjust());
    connect(_automaticLODAdjust, SIGNAL(toggled(bool)), SLOT(updateAutomaticLODAdjust()));
    
    form->addRow("Desktop Decrease LOD Below FPS:", _desktopLODDecreaseFPS = new QDoubleSpinBox(this));
    _desktopLODDecreaseFPS->setValue(lodManager->getDesktopLODDecreaseFPS());
    _desktopLODDecreaseFPS->setDecimals(0);
    connect(_desktopLODDecreaseFPS, SIGNAL(valueChanged(double)), SLOT(updateLODValues()));
    
    form->addRow("Desktop Increase LOD Above FPS:", _desktopLODIncreaseFPS = new QDoubleSpinBox(this));
    _desktopLODIncreaseFPS->setValue(lodManager->getDesktopLODIncreaseFPS());
    _desktopLODIncreaseFPS->setDecimals(0);
    connect(_desktopLODIncreaseFPS, SIGNAL(valueChanged(double)), SLOT(updateLODValues()));

    form->addRow("HMD Decrease LOD Below FPS:", _hmdLODDecreaseFPS = new QDoubleSpinBox(this));
    _hmdLODDecreaseFPS->setValue(lodManager->getHMDLODDecreaseFPS());
    _hmdLODDecreaseFPS->setDecimals(0);
    connect(_hmdLODDecreaseFPS, SIGNAL(valueChanged(double)), SLOT(updateLODValues()));
    
    form->addRow("HMD Increase LOD Above FPS:", _hmdLODIncreaseFPS = new QDoubleSpinBox(this));
    _hmdLODIncreaseFPS->setValue(lodManager->getHMDLODIncreaseFPS());
    _hmdLODIncreaseFPS->setDecimals(0);
    connect(_hmdLODIncreaseFPS, SIGNAL(valueChanged(double)), SLOT(updateLODValues()));
    
    form->addRow("Avatar LOD:", _avatarLOD = new QDoubleSpinBox(this));
    _avatarLOD->setDecimals(3);
    _avatarLOD->setRange(1.0 / MAXIMUM_AVATAR_LOD_DISTANCE_MULTIPLIER, 1.0 / MINIMUM_AVATAR_LOD_DISTANCE_MULTIPLIER);
    _avatarLOD->setSingleStep(0.001);
    _avatarLOD->setValue(1.0 / lodManager->getAvatarLODDistanceMultiplier());
    connect(_avatarLOD, SIGNAL(valueChanged(double)), SLOT(updateAvatarLODValues()));

    _lodSize = new QSlider(Qt::Horizontal, this);
    const int MAX_LOD_SIZE = MAX_LOD_SIZE_MULTIPLIER;
    const int MIN_LOD_SIZE = ADJUST_LOD_MIN_SIZE_SCALE;
    const int STEP_LOD_SIZE = 1;
    const int PAGE_STEP_LOD_SIZE = 100;
    const int SLIDER_WIDTH = 300;
    _lodSize->setMaximum(MAX_LOD_SIZE);
    _lodSize->setMinimum(MIN_LOD_SIZE);
    _lodSize->setSingleStep(STEP_LOD_SIZE);
    _lodSize->setTickInterval(PAGE_STEP_LOD_SIZE);
    _lodSize->setTickPosition(QSlider::TicksBelow);
    _lodSize->setFixedWidth(SLIDER_WIDTH);
    _lodSize->setPageStep(PAGE_STEP_LOD_SIZE);
    int sliderValue = lodManager->getOctreeSizeScale() / TREE_SCALE;
    _lodSize->setValue(sliderValue);
    form->addRow("Non-Avatar Content LOD:", _lodSize);
    connect(_lodSize,SIGNAL(valueChanged(int)),this,SLOT(sizeScaleValueChanged(int)));
    
    // Add a button to reset
    QPushButton* resetButton = new QPushButton("Reset", this);
    form->addRow("", resetButton);
    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetClicked(bool)));

    this->QDialog::setLayout(form);
    
    updateAutomaticLODAdjust();
}

void LodToolsDialog::reloadSliders() {
    auto lodManager = DependencyManager::get<LODManager>();
    _lodSize->setValue(lodManager->getOctreeSizeScale() / TREE_SCALE);
    _feedback->setText(lodManager->getLODFeedbackText());

    _avatarLOD->setValue(1.0 / lodManager->getAvatarLODDistanceMultiplier());

}

void LodToolsDialog::updateAutomaticLODAdjust() {
    auto lodManager = DependencyManager::get<LODManager>();
    lodManager->setAutomaticLODAdjust(_automaticLODAdjust->isChecked());
}

void LodToolsDialog::updateLODValues() {
    auto lodManager = DependencyManager::get<LODManager>();

    lodManager->setAutomaticLODAdjust(_automaticLODAdjust->isChecked());

    lodManager->setDesktopLODDecreaseFPS(_desktopLODDecreaseFPS->value());
    lodManager->setDesktopLODIncreaseFPS(_desktopLODIncreaseFPS->value());
    lodManager->setHMDLODDecreaseFPS(_hmdLODDecreaseFPS->value());
    lodManager->setHMDLODIncreaseFPS(_hmdLODIncreaseFPS->value());

    lodManager->setAvatarLODDistanceMultiplier(1.0 / _avatarLOD->value());
}

void LodToolsDialog::sizeScaleValueChanged(int value) {
    auto lodManager = DependencyManager::get<LODManager>();
    float realValue = value * TREE_SCALE;
    lodManager->setOctreeSizeScale(realValue);
    
    _feedback->setText(lodManager->getLODFeedbackText());
}

void LodToolsDialog::resetClicked(bool checked) {

    int sliderValue = DEFAULT_OCTREE_SIZE_SCALE / TREE_SCALE;
    _lodSize->setValue(sliderValue);
    _automaticLODAdjust->setChecked(true);
    _desktopLODDecreaseFPS->setValue(DEFAULT_DESKTOP_LOD_DOWN_FPS);
    _desktopLODIncreaseFPS->setValue(DEFAULT_DESKTOP_LOD_UP_FPS);
    _hmdLODDecreaseFPS->setValue(DEFAULT_HMD_LOD_DOWN_FPS);
    _hmdLODIncreaseFPS->setValue(DEFAULT_HMD_LOD_UP_FPS);

    _avatarLOD->setValue(1.0 / DEFAULT_AVATAR_LOD_DISTANCE_MULTIPLIER);
    
    updateLODValues(); // tell our LOD manager about the reset
}

void LodToolsDialog::reject() {
    // Just regularly close upon ESC
    this->QDialog::close();
}

void LodToolsDialog::closeEvent(QCloseEvent* event) {
    this->QDialog::closeEvent(event);
    emit closed();
}


