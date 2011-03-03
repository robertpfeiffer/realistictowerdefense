// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#pragma once

#include <string>
#include <vector>
#include <queue>
#include <osg/Texture2D>
#include <osg/Node>


class AssetLibrary {
public:

	static AssetLibrary* instance();

	osg::Texture2D* getTexture(const std::string filename);
	osg::Node* getModel(const std::string filename);

	template<class T>
    struct CacheElement{
		std::string filename;
		T item;
		bool used;
	};

private:
	void _sweep();
	void _unmark();

	std::list< CacheElement< osg::ref_ptr<osg::Texture2D> > > _textureCache;
	std::list< CacheElement< osg::ref_ptr<osg::Node> > > _modelCache;


};
