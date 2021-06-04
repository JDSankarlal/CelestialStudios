#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include "Utilities.h"

class Component
{
public:
	//enum COMP_TYPE{UNKNOWN};
	typedef std::string COMP_TYPE;
	typedef uint CompID;
	static bool m_exit;

private:
	std::vector<Component* >m_children;
	Component* m_parent;

protected:
	Component(Component* parent = nullptr, CompID id = 0);
	Component(COMP_TYPE type, Component* parent = nullptr, CompID id = 0);
	virtual ~Component();

	COMP_TYPE m_type;
	CompID m_ID = 0;
	static uint m_countID;
	static std::list<std::pair<Component::COMP_TYPE, Component*>>
		m_compList;

public:
	virtual	COMP_TYPE getCompType()
	{
		return m_type;
	}

	static const std::list<std::pair<COMP_TYPE, Component*>>& getComponentList();

	virtual void addChild(Component* child);
	virtual void removeChild(Component* child);
	virtual void removeChild(unsigned index);

	virtual void setParent(Component* parent);
	virtual void removeParent(Component* parent);

	virtual Component* getChild(unsigned int index);
	virtual Component* getParent();
	virtual std::vector<Component*>& getChildren();

	CompID getID();
};

