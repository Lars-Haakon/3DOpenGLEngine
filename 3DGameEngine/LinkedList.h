#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define NULL 0

template <class Type>
class LinkedList
{
public:
	struct Node
	{
		Type *t;
		Node *next;

		Node()
		{
			t = NULL;
			next = NULL;
		}

		Node(Type *iT)
		{
			t = iT;
			next = NULL;
		}

		~Node()
		{
			delete t;
		}
	};

	LinkedList()
	{
		head = new Node();
		length = 0;
	}

	~LinkedList()
	{
		empty();

		delete head;
	}

	void add(Type *iT)
	{
		current = head;
		while (current->next != NULL)
			current = current->next;

		current->next = new Node(iT);
		
		length++;
	}

	void remove(int element)
	{
		current = head;

		for (int i = 0; i < element; i++)
			current = current->next;

		Node *temp = current;
		current = current->next->next;
		delete temp->next;
		temp->next = current;

		length--;
	}

	Type *get(int element)
	{
		current = head;

		for (int i = 0; i <= element; i++)
			current = current->next;

		return current->t;
	}

	void set(int element, Type *newT)
	{
		current = head;

		for (int i = 0; i <= element; i++)
			current = current->next;

		current->t = newT;
	}

	Type *toArray()
	{
		current = head;

		Type *tA = new Type[length];
		for (int i = 0; i < length; i++)
		{
			tA[i] = *current->next->t;
			current = current->next;
		}

		return tA;
	}

	void empty()
	{
		if (length != 0)
		{
			current = head->next;

			while (current->next != NULL)
			{
				Node *temp = current;
				current = current->next;
				delete temp;
			}

			delete current;
			length = 0;
			head->next = NULL;
		}
	}

	int getLength()
	{
		return length;
	}

private:
	Node *head;
	Node *current;
	int length;
};

#endif