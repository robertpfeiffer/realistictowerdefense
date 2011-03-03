// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#include <assetlibrary.h>
#include <string.h>
#include <osg/Node>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <constants.h>

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
