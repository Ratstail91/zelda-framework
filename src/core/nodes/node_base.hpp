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
	void RemoveChild(int index);
	friend void removeDescendantsOfNode(NodeBase* const);

	NodeBase* GetParent() const { return parent; }

	template<typename T>
	std::list<T*> GetDescendantsByType();

	template<typename T>
	T* GetFirstChildByType();

protected:
	NodeBase* parent = nullptr;
	std::list<NodeBase*> children;
};

//template definitions
template<typename T>
std::list<T*> NodeBase::GetDescendantsByType() {
	std::list<T*> result;

	//get children & children's children
	for (auto child : children) {
		T* ptr = dynamic_cast<T*>(child);
		if (ptr != nullptr) {
			result.push_back(ptr);
		}

		//recurse to find all decendants
		auto c = child->GetDescendantsByType<T>();
		result.splice(result.end(), c);
	}

	return result;
}

template<typename T>
T* NodeBase::GetFirstChildByType() {
	//find only the first immediate child of a specified type
	for (auto child : children) {
		T* ptr = dynamic_cast<T*>(child);
		if (ptr != nullptr) {
			return ptr;
		}
	}

	//not found
	return nullptr;
}