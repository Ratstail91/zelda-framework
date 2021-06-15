#pragma once

#include <list>
#include <typeinfo>

class NodeBase {
public:
	NodeBase() = default;
	virtual ~NodeBase() = default;

	//node tree
	NodeBase* AddChild(NodeBase* const);
	NodeBase* GetChild(int index);
	friend void deleteNode(NodeBase* const);

	NodeBase* GetParent() const { return parent; }

	template<typename T>
	std::list<T*> GetChildrenByType();

	template<typename T>
	T* GetFirstChildByType();

protected:
	NodeBase* parent = nullptr;
	std::list<NodeBase*> children; //TODO: change this to a vector of shared pointers

private:
	void RemoveChild(int index); //WARNING: dangerous, use `deleteNode(root->GetChild(index))` instead
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
		}

		std::list<T*> c = (*it)->GetChildrenByType<T>();
		result.splice(result.end(), c);
	}

	return result;
}

template<typename T>
T* NodeBase::GetFirstChildByType() {
	//find only the first immediate child of a type
	for (std::list<NodeBase*>::iterator it = children.begin(); it != children.end(); it++) {
		T* ptr = dynamic_cast<T*>(*it);
		if (ptr != nullptr) {
			return ptr;
		}
	}

	return nullptr;
}