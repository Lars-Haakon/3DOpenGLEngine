#ifndef HASHTABLE_H
#define HASHTABLE_H

#define NULL 0

struct HashNode
{
	char *key;
	int value;
};

template <class Type>
class HashTable
{
public:
	struct HashNode
	{
		char *key;
		Type *value;
	};
	
	HashTable(int m)
	{
		this->m = m;

		table = new HashNode[m];
		for (int i = 0; i < m; i++)
			table[i].key = NULL;
	}
	~HashTable()
	{
		for (int i = 0; i < m; i++)
		{
			if (table[i].key != NULL)
			{
				delete table[i].key;
				delete table[i].value;
			}
		}

		delete[] table;
	}

	void put(char *key, Type *value)
	{
		int size = strlen(key) + 1;

		int k = 0;
		for (int i = 0; i < size; i++)
			k += key[i] * (i + 1);

		int hash1 = h1(k);
		int hash2 = h2(k);

		for (int i = 0;; i++)
		{
			int index = (hash1 + i*hash2) % m;

			if (table[index].key == NULL)
			{
				table[index].key = new char[size];

				for (int j = 0; j < size; j++)
					table[index].key[j] = key[j];

				table[index].value = value;
				break;
			}
		}
	}

	void remove(char *key)
	{
		int size = strlen(key) + 1;

		int k = 0;
		for (int i = 0; i < size; i++)
			k += key[i] * (i + 1);

		int hash1 = h1(k);
		int hash2 = h2(k);

		for (int i = 0;; i++)
		{
			int index = (hash1 + i*hash2) % m;

			if (table[index].key != NULL)
			{
				if (strcmp(table[index].key, key) == 0)
				{
					delete table[index].key;
					table[index].key = NULL;
					delete table[index].value;
					break;
				}
			}
		}
	}

	Type *get(char *key)
	{
		int size = strlen(key) + 1;

		int k = 0;
		for (int i = 0; i < size; i++)
			k += key[i] * (i + 1);

		int hash1 = h1(k);
		int hash2 = h2(k);

		for (int i = 0; i < m; i++)
		{
			int index = (hash1 + i*hash2) % m;

			if (table[index].key != NULL)
				if (strcmp(table[index].key, key) == 0)
					return table[index].value;
		}

		return NULL;
	}

private:
	HashNode *table;
	int m;

	int h1(int k)
	{
		return k % m;
	}
	int h2(int k)
	{
		return k % (m - 1) + 1;
	}
};

#endif