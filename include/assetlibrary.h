// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup" -*-

#pragma once

#include <string>
#include <vector>
#include <queue>
#include <osg/Texture2D>
#include <osg/Node>


class AssetLibrary {
public:

	AssetLibrary* instance();

	osg::Texture2D* _getTexture(const std::string filename);
	osg::Node* _getModel(const std::string filename);

	void _sweep();
	void _unmark();

	template<class T>
    struct _cache{
		std::string filename;
		T item;
		bool used;
	};

	std::list< _cache< osg::ref_ptr<osg::Texture2D> > > _textureCache;
	std::list< _cache< osg::ref_ptr<osg::Node> > > _modelCache;


};
