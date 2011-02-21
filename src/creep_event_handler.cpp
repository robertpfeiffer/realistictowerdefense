#include<creep_event_handler.h>
#include<world.h>
#include<graveyard.h>

CreepEventHandler::CreepEventHandler(World* world)
{
	_world = world;
}

void CreepEventHandler::onDeath(Creep* creep)
{
	_world->removeChild(creep);
}

void CreepEventHandler::onLeak(Creep* creep)
{
	Graveyard::instance()->killFrom(creep, _world);
}