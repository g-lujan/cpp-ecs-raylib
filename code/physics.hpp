#ifndef _PHYSICS_H__
#define _PHYSICS_H__

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

namespace Physics {
  b2Body *create_body(b2World *world)
  {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody; 
    body_def.position.Set(0, 20);   
    body_def.angle = 0;             
    b2Body *dynamicBody = world->CreateBody(&body_def);
    b2PolygonShape boxShape;
    boxShape.SetAsBox(1, 1);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    dynamicBody->CreateFixture(&boxFixtureDef);
  }
}

#endif // !_PHYSICS_H__
