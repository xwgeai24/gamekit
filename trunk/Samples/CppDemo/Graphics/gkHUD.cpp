/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
    This is a replacement candidate for Engine/Graphics
    - For now it wraps Ogre::Overlays to gk usable objects.
    - Later it will be superseded by custom Ogre vertex buffer usage, 
    - abstracted for external GameKit external rendering.
*/
#include "gkHUD.h"
#include "gkHUDElement.h"
#include "gkLogger.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayManager.h"



gkHUD::gkHUD(gkResourceManager* creator, const gkResourceName &name, const gkResourceHandle& handle)
	:	gkResource(creator, name, handle), m_overlay(0)
{
	newImpl();
}


gkHUD::~gkHUD()
{
	if (m_overlay)
		m_overlay->clear();

	ChildNodes::Iterator it = m_children.iterator();
	while (it.hasMoreElements())
		delete it.getNext();

	m_children.clear();
}


void gkHUD::show(bool v)
{
	if (m_overlay)
	{
		if (v)
			m_overlay->show();
		else
			m_overlay->hide();
	}
}



void gkHUD::addChild(gkHUDElement *hud)
{
	m_children.push_back(hud);
}


gkHUDElement* gkHUD::getChild(const gkString &name)
{
	ChildNodes::Iterator it = m_children.iterator();
	while (it.hasMoreElements())
	{
		gkHUDElement *ele = it.getNext();
		if (ele->getName() == name)
			return ele;
	}
	return 0;
}

		
Ogre::Overlay* gkHUD::_getOverlay(void)
{
	return m_overlay;
}


void gkHUD::_setOverlay(Ogre::Overlay* over)
{
	m_overlay = over;
}


void gkHUD::newImpl(void)
{

	try {

		m_overlay = Ogre::OverlayManager::getSingleton().getByName(m_name.str());

		// Auto create if it's here
		if (m_overlay)
		{
			Ogre::Overlay::Overlay2DElementsIterator elements = m_overlay->get2DElementsIterator();

			while (elements.hasMoreElements())
			{
				Ogre::OverlayElement *ele = (Ogre::OverlayElement*)elements.getNext();

				gkHUDElement *hele = new gkHUDElement(ele->getName());
				addChild(hele);
			}

		}
	}
	catch (Ogre::Exception &e)
	{
		gkLogMessage("HUD: " << e.getDescription());
		m_overlay = 0;
	}
}
