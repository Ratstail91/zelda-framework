#include "node_base.hpp"

#include <stdexcept>

NodeBase* NodeBase::AddChild(NodeBase* const ptr) {
	for (NodeBase* it = parent; it != nullptr; it = it->parent) {
		if (it == ptr) {
			std::logic_error("Looping hierarchy found");
		}
	}

	children.push_back(ptr);
	ptr->parent = this;
	return ptr;
}

NodeBase* NodeBase::GetChild(int index) {
	if (index >= 0) {
		auto it = children.begin();
		std::advance(it, index);
		return *it;
	} else {
		//backwards from the end
		auto it = children.rbegin();
		std::advance(it, -index - 1);
		return *it;
	}
}

void NodeBase::RemoveChild(int index) {
	if (index >= 0) {
		auto it = children.begin();
		std::advance(it, index);
		(*it)->parent = nullptr;
		removeDescendantsOfNode(*it);
		delete *it;
		children.erase(it);
	} else {
		//backwards from the end
		auto it = children.rbegin();
		std::advance(it, -index - 1);
		(*it)->parent = nullptr;
		removeDescendantsOfNode(*it.base());
		delete *it;
		children.erase(it.base());
	}
}

void removeDescendantsOfNode(NodeBase* const root) {
	for (auto childPtr : root->children) {
		removeDescendantsOfNode(childPtr);
		delete childPtr;
	}
}