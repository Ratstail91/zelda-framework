#include "node_base.hpp"

NodeBase* NodeBase::AddChild(NodeBase* ptr) {
	children.push_back(ptr);
	ptr->parent = this;
	return ptr;
}

NodeBase* NodeBase::GetChild(int index) {
	if (index >= 0) {
		std::list<NodeBase*>::iterator it;
		it = children.begin();
		std::advance(it, index);
		return *it;
	} else {
		std::list<NodeBase*>::reverse_iterator it;
		it = children.rbegin();
		std::advance(it, -index - 1);
		return *it;
	}
}

void NodeBase::RemoveChild(int index) {
	if (index >= 0) {
		std::list<NodeBase*>::iterator it;
		it = children.begin();
		std::advance(it, index);
		(*it)->parent = nullptr;
		children.erase(it);
	} else {
		std::list<NodeBase*>::reverse_iterator it;
		it = children.rbegin();
		std::advance(it, -index - 1);
		(*it)->parent = nullptr;
		children.erase(it.base());
	}
}

void deleteNode(NodeBase* root) {
	while (true) {
		NodeBase* child = root->GetChild(0);

		if (child == nullptr) {
			break;
		}

		deleteNode(child);

		root->RemoveChild(0);
		delete child;
	}
}