// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#include <assetlibrary.h>
#include <string.h>
#include <world.h>
#include <osg/Node>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <constants.h>

#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ParticleSystem>

AssetLibrary* AssetLibrary::instance()
{
	static AssetLibrary s_library;
	return &s_library;
}

osg::Texture2D* AssetLibrary::getTexture(const std::string filename)
{
	std::list< CacheElement< osg::ref_ptr<osg::Texture2D> > >::iterator it;
	for(it = _textureCache.begin(); it != _textureCache.end(); it++)
	{
		if (it->filename.compare(filename) == 0)
		{
			it->used = true;
			return it->item;
		}
	}

	std::string textureFilename = MAP_DIRECTORY_TEXTURES;
	textureFilename.append(filename);

	osg::Image* image = osgDB::readImageFile(textureFilename);
	if (image != NULL)
	{
		osg::Texture2D* texture = new osg::Texture2D(image);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		texture->setDataVariance(osg::Object::STATIC);

		if (getenv("NETBOOK") == NULL)
		{
			texture->setMaxAnisotropy(AF_LEVEL);
		}

		
		CacheElement< osg::ref_ptr<osg::Texture2D> > newElement;

		newElement.filename = filename;
		newElement.item = texture;
		newElement.used = true;
		_textureCache.push_back(newElement);
		return texture;
	}
	return NULL;
}

void AssetLibrary::_addParticleEffects(osg::Node* currNode)
{
	if (!currNode)	return;

	osg::Group* currGroup;
	
	// We have a valid node, check to see if this is the node we 
	// are looking for. If so, return the current node.	
	osgParticle::ParticleEffect *pe = dynamic_cast<osgParticle::ParticleEffect*>(currNode);
	if ( pe != NULL ) {
		pe->getParticleSystem()->setParticleScaleReferenceFrame(osgParticle::ParticleSystem::LOCAL_COORDINATES);
		if (!pe->getUseLocalParticleSystem()) {
			World::instance()->addParticleEffect(pe->getParticleSystem());
			osg::Geode* geode = new osg::Geode;
			geode->addDrawable(pe->getParticleSystem());

			for (unsigned int i = 0 ; i < pe->getNumParents(); i ++) {
				if(pe->getParent(i))
				{
					pe->getParent(i)->addChild(geode);
				}
			}
		}
		return;
	}


	currGroup = currNode->asGroup(); // returns NULL if not a group.
	if (currGroup)
	{
		for (unsigned int i = 0; i < currGroup->getNumChildren(); i++)
		{ 
			_addParticleEffects(currGroup->getChild(i));
		}
	}
}

void AssetLibrary::_addAnimations(osg::Node* currNode)
{
	if (!currNode)	return;

	osgAnimation::Animation* anim= dynamic_cast<osgAnimation::Animation*>(currNode->getUserData());

	if (anim != NULL) {
		World::instance()->addAnimation(anim);
		return;
	}

	osg::Group* currGroup = currNode->asGroup();
    // returns NULL if not a group.
	if (currGroup)
	{
		for (unsigned int i = 0; i < currGroup->getNumChildren(); i++)
		{ 
			_addAnimations(currGroup->getChild(i));
		}
	}
}

osg::Node* AssetLibrary::getModel(const std::string filename)
{
	std::list< CacheElement< osg::ref_ptr<osg::Node> > >::iterator it;
	for(it = _modelCache.begin(); it != _modelCache.end(); it++)
	{
		if (it->filename.compare(filename) == 0)
		{
			it->used = true;
			return it->item;
		}
	}

	std::string modelFilename = MAP_DIRECTORY_MODELS;
	modelFilename.append(filename);

	CacheElement< osg::ref_ptr<osg::Node> > newElement;

	newElement.filename = filename;
	newElement.item = osgDB::readNodeFile(modelFilename);
	newElement.used = true;
	_addParticleEffects(newElement.item);
	_addAnimations(newElement.item);
	_modelCache.push_back(newElement);
	return newElement.item;
}

//callback for remove_if
bool isTextureUnused(AssetLibrary::CacheElement<osg::ref_ptr<osg::Texture2D> > item) {
    return !item.used;
}

//callback for remove_if
bool isModelUnused(AssetLibrary::CacheElement<osg::ref_ptr<osg::Node> > item) {
    return !item.used;
}

void AssetLibrary::sweep()
{
	_textureCache.remove_if(isTextureUnused);
	_modelCache.remove_if(isModelUnused);
}

void AssetLibrary::unmark()
{
	//mark all textures as not used on current map
	{
		std::list< CacheElement< osg::ref_ptr<osg::Texture2D> > >::iterator it;
		for(it = _textureCache.begin(); it != _textureCache.end(); it++)
		{
			it->used = false;
		}
	}

	//mark all models as not used on current map
	{
		std::list< CacheElement< osg::ref_ptr<osg::Node> > >::iterator it;
		for(it = _modelCache.begin(); it != _modelCache.end(); it++)
		{
			it->used = false;
		}
	}

}
