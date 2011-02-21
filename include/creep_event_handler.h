#pragma once

class Creep;
class World;

class CreepEventHandler
{
public:
	CreepEventHandler(World* world);
	void onDeath(Creep* creep);
	void onLeak(Creep* creep);

private:
	World* _world;
};