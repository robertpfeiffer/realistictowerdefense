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

/**
 * \fn	AssetLibrary* AssetLibrary::instance()
 *
 * \brief	Gets the instance (singleton).
 *
 * \return	The AssetLibrary.
 */
AssetLibrary* AssetLibrary::instance()
{
	static AssetLibrary s_library;
	return &s_library;
}

/**
 * \fn	osg::Texture2D* AssetLibrary::getTexture(const std::string filename)
 *
 * \brief	Gets a texture by its filename.
 * 			Load it from disc, if it is not cached, else return it from cache.
 *
 * \param	filename	Filename of the texture.
 *
 * \return	null if it fails, else the texture.
 */
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

		texture->setMaxAnisotropy(AF_LEVEL);

		
		CacheElement< osg::ref_ptr<osg::Texture2D> > newElement;

		newElement.filename = filename;
		newElement.item = texture;
		newElement.used = true;
		_textureCache.push_back(newElement);
		return texture;
	}
	return NULL;
}

/**
 * \fn	void AssetLibrary::_addParticleEffects(osg::Node* currNode)
 *
 * \brief	Adds a particle effects. 
 *
 * \param	currNode	If non-null, the curr node.
 */
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

/**
 * \fn	void AssetLibrary::_addAnimations(osg::Node* currNode)
 *
 * \brief	Adds an animations. 
 *
 * \paramcurrNode	If non-null, the curr node.
 */
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

/**
 * \fn	osg::Node* AssetLibrary::getModel(const std::string filename)
 *
 * \brief	Gets a model by its filename.
 * 			Load it from disc, if it is not cached, else return it from cache.
 *
 * \param	filename	Filename of the model.
 *
 * \return	null if it fails, else the model.
 */
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
	World::instance()->getChild(0)->asGroup()->addChild(newElement.item);
	return newElement.item;
}

/**
 * \fn	bool isTextureUnused(AssetLibrary::CacheElement<osg::ref_ptr<osg::Texture2D> > item)
 *
 * \brief	Query if texture is unused.
 *
 * \param	item	The item.
 *
 * \return	true if texture is unused, false if not.
 */
bool isTextureUnused(AssetLibrary::CacheElement<osg::ref_ptr<osg::Texture2D> > item) {
    return !item.used;
}

/**
 * \fn	bool isModelUnused(AssetLibrary::CacheElement<osg::ref_ptr<osg::Node> > item)
 *
 * \brief	Query if model unused.
 *
 * \param	item	The item.
 *
 * \return	true if model unused, false if not.
 */
bool isModelUnused(AssetLibrary::CacheElement<osg::ref_ptr<osg::Node> > item) {
    return !item.used;
}

/**
 * \fn	void AssetLibrary::sweep()
 *
 * \brief	Sweeps this model and texture cache.
 */
void AssetLibrary::sweep()
{
	_textureCache.remove_if(isTextureUnused);
	_modelCache.remove_if(isModelUnused);
}

/**
 * \fn	void AssetLibrary::unmark()
 *
 * \brief	Unmarks all texture and models as unused.
 */
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
