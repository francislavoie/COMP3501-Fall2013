//ArrayList.h
//Holds a list in an array
//Copyright Daniel Silverthorn 2012
//v1.2

#pragma once

template <class T>
class ArrayList
{
public:
	ArrayList(void);
	~ArrayList(void);

	void add(T* item);
	void insertAt(T* item, int index);
	T* get(int i);
	bool contains(T* item);
	T* remove(int i);
	void remove(T* item);
	int count();
	T** toArray(int& outLength);
	T* operator[](int i);

private:
	void growIfShould();
	void shrinkIfShould();

	int size;
	int arraySize;
	T** items;
};

template <class T>
ArrayList<T>::ArrayList(void)
{
	arraySize = 16;
	items = new T* [arraySize];
	size = 0;
}

template <class T>
ArrayList<T>::~ArrayList(void)
{
	delete [] items;
}

template <class T>
void ArrayList<T>::add(T* item)
{
	growIfShould();

	items[size] = item;
	size++;
}

template <class T>
void ArrayList<T>::insertAt(T* item, int index)
{
	if (index < 0 || index > size)
		return;

	growIfShould();

	for (int i=size; i>index; i--)
	{
		items[i] = items[i-1];
	}

	items[index] = item;
	size++;
}

template <class T>
T* ArrayList<T>::get(int i)
{
	if (i < 0 || i >= size)
		return 0;

	return items[i];
}

template <class T>
bool ArrayList<T>::contains(T* item)
{
	for (int i=0;i<size;i++)
	{
		if (items[i] == item)
			return true;
	}

	return false;
}

template <class T>
T* ArrayList<T>::remove(int i)
{
	if (i < 0 || i >= size)
		return 0;

	T* temp = items[i];
	items[i] = 0;

	int j;
	for (j = i; j < size - 1; j++)
	{
		items[j] = items[j + 1];
	}
	items[j] = 0;
	size--;

	shrinkIfShould();

	return temp;
}

template <class T>
void ArrayList<T>::remove(T* item)
{
	for (int i=0;i<size;i++)
	{
		if (items[i] == item)
		{
			remove(i);
			break;
		}
	}
}

template <class T>
int ArrayList<T>::count()
{
	return size;
}

template <class T>
void ArrayList<T>::growIfShould()
{
	if (size >= arraySize)
	{
		arraySize *= 2;
		T** objects = new T* [arraySize];
		for (int i=0;i<size;i++)
		{
			objects[i] = items[i];
		}

		delete [] items;
		items = objects;
	}
}

template <class T>
void ArrayList<T>::shrinkIfShould()
{
	//that way we don't have an empty array
	if (arraySize < 5)
		return;

	if (size < 2 * arraySize)
	{
		arraySize *= 2;
		T** objects = new T* [arraySize];
		for (int i=0;i<size;i++)
		{
			objects[i] = items[i];
		}

		delete [] items;
		items = objects;
	}
}

template <class T>
T** ArrayList<T>::toArray(int& outLength)
{
	outLength = size;

	T** retVal = new T* [size];

	for (int i=0; i<size; i++)
	{
		retVal[i] = items[i];
	}

	return retVal;
}

template <class T>
T* ArrayList<T>::operator[](int i)
{
	if (i < 0 || i >= size)
		return 0;

	return items[i];
}