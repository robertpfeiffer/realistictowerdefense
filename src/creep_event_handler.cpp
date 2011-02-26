#include<creep_event_handler.h>
#include<world.h>
#include<graveyard.h>

CreepEventHandler::CreepEventHandler(World* world)
{
	_world = world;
}

void CreepEventHandler::onDeath(Creep* creep)
{
	Graveyard::instance()->killChild(_world, creep);
	_world->dropCreep();
}

void CreepEventHandler::onLeak(Creep* creep)
{
	Graveyard::instance()->killChild(_world, creep);
	_world->dropCreep();
}