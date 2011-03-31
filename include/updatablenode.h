#pragma once

/**
 * \class	UpdatableNode
 *
 * \brief	Updatable node. 
 */
class UpdatableNode
{
public:
	/**
	 * \fn	void UpdatableNode::onUpdate()
	 *
	 * \brief	Executed on update traversale each frame.
	 */
	void virtual onUpdate() {};
};