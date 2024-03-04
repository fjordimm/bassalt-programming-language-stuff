
// There is a more efficient and more explanatory implementation of LLTest.taul in LLTest.cpp

#include <iostream>
#include <memory>
#include <string>

struct Node;

/* COMPILER MEM STUFF */
  static Node* cur_LATEST_VALUE;
  static Node** cur_VARIABLE;
  #define cur_USED_AFTER_head_WAS_TAMPERED_WITH 1
/**********************/

struct Node
{
	int data;
	Node* next;

	Node(int _data, Node* _next)
	{
		data = _data;
		next = _next;
	}

	~Node()
	{
		delete next;

		/* COMPILER MEM STUFF */
		  #ifdef cur_USED_AFTER_head_WAS_TAMPERED_WITH
		    if (this == cur_LATEST_VALUE)
		    { *cur_VARIABLE = nullptr; }
		  #endif
		/**********************/
		// ...this could be more efficient if there were multiple destructors, and you only do this checking stuff for when you need to
	}
};

void recPrintStack(Node* node)
{
	if (node != nullptr)
	{
		std::cout << "  Node(" << std::to_string(node->data) << ")" << std::endl;
		recPrintStack(node->next);
	}
	else
	{ std::cout << "  NullNode" << std::endl; }
}

int main(void)
{
	std::cout << "=== Starting Program ===" << std::endl;

	Node* head = new Node(1, nullptr);
	Node* cur = head;
	/* COMPILER MEM STUFF */
	  cur_VARIABLE = &cur;
	/**********************/
	for (int i = 2; i < 16; i++)
	{
		cur->next = new Node(i, nullptr);
		cur = cur->next;
		/* COMPILER MEM STUFF */
		  cur_LATEST_VALUE = cur;
		/**********************/
	}

	std::cout << "Stack:" << std::endl;
	recPrintStack(head);

	if (cur != nullptr)
	{ std::cout << "cur = Node(" << std::to_string(cur->data) << ")" << std::endl; }
	else
	{ std::cout << "cur is null" << std::endl; }
	std::cout << std::endl;

	delete head->next->next->next;
	head->next->next->next = nullptr;

	std::cout << "Stack:" << std::endl;
	recPrintStack(head);

	if (cur != nullptr)
	{ std::cout << "cur = Node(" << std::to_string(cur->data) << ")" << std::endl; }
	else
	{ std::cout << "cur is null" << std::endl; }
	std::cout << std::endl;
	
	std::cout << "=== Ending Program ===" << std::endl;

	/* COMPILER MEM STUFF */
	  delete head;
	/**********************/
}
