//
//  UpdateEntityOperator.h
//  libraries/entities/src
//
//  Created by Brad Hefta-Gaub on 8/11/2014.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_UpdateEntityOperator_h
#define hifi_UpdateEntityOperator_h

class UpdateEntityOperator : public RecurseOctreeOperator {
public:
    UpdateEntityOperator(EntityTree* tree, EntityTreeElement* containingElement, 
                            EntityItem* existingEntity, const EntityItemProperties& properties);
                            
    virtual bool PreRecursion(OctreeElement* element);
    virtual bool PostRecursion(OctreeElement* element);
    virtual OctreeElement* PossiblyCreateChildAt(OctreeElement* element, int childIndex);
private:
    EntityTree* _tree;
    EntityItem* _existingEntity;
    EntityTreeElement* _containingElement;
    EntityItemProperties _properties;
    EntityItemID _entityItemID;
    bool _foundOld;
    bool _foundNew;
    bool _removeOld;
    quint64 _changeTime;

    AACube _oldEntityCube;
    AACube _newEntityCube;

    AABox _oldEntityBox;
    AABox _newEntityBox;

    bool subTreeContainsOldEntity(OctreeElement* element);
    bool subTreeContainsNewEntity(OctreeElement* element);
};

#endif // hifi_UpdateEntityOperator_h
