#pragma once

class Creep;

class CreepEventHandler
{
public:
	virtual void onDeath(Creep* creep) { };
	virtual void onLeak(Creep* creep) { };
};