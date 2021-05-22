#pragma once

#include <list>
#include <typeinfo>

#include <iostream>

class NodeBase {
public:
	NodeBase() = default;
	virtual ~NodeBase() = default;

	//node tree
	void AddChild(NodeBase*);
	NodeBase* GetChild(int index);
	void RemoveChild(int index);

	template<typename T>
	std::list<T*> GetChildrenByType();

protected:
	std::list<NodeBase*> children;
};

//template definitions
template<typename T>
std::list<T*> NodeBase::GetChildrenByType() {
	std::list<T*> result;

	//get children & children's children
	for (std::list<NodeBase*>::iterator it = children.begin(); it != children.end(); it++) {
		T* ptr = dynamic_cast<T*>(*it);
		if (ptr != nullptr) {
			result.push_back(ptr);

			std::list<T*> c = ((NodeBase*)ptr)->GetChildrenByType<T>();
			result.splice(result.end(), c);
		}
	}

	return result;
}
