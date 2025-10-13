#pragma once
#include "namespaces/Math.hpp"
#include <iostream>
class IRenderer
{
public:

	virtual	~IRenderer() = default;

	virtual void	renderAll()										= 0;
	virtual void	init()											= 0;

	virtual void			setWindowSize(const Math::Vector2u&)	= 0;
	virtual void			setWindowName(const STRING)				= 0;
	virtual Math::Vector2u	getWindowSize()							= 0;
	virtual STRING			getWindowName()							= 0;
	virtual BOOL			windowOpen()							= 0;

	virtual void pollEvents()	= 0;
	virtual void display()		= 0;
	virtual void draw()			= 0;
	virtual void clear()		= 0;

};

