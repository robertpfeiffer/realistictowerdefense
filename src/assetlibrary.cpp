// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#include <assetlibrary.h>
#include <string.h>
#include <osg/Node>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

AssetLibrary* AssetLibrary::instance()
{
	static AssetLibrary* _ptr;
	if(_ptr == NULL)
	{
		_ptr = new AssetLibrary();
	}
	return _ptr;
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


	osg::Image* image = osgDB::readImageFile(filename);
	if (image != NULL)
	{
		osg::Texture2D* texture = new osg::Texture2D(image);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		texture->setDataVariance(osg::Object::STATIC);
		//texture->setMaxAnisotropy(AF_LEVEL);

		
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

	CacheElement< osg::ref_ptr<osg::Node> > newElement;

	newElement.filename = filename;
	newElement.item = osgDB::readNodeFile(filename);
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

void AssetLibrary::_sweep()
{
	_textureCache.remove_if(isTextureUnused);
	_modelCache.remove_if(isModelUnused);
}

void AssetLibrary::_unmark()
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
