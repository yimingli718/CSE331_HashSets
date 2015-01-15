/**
 * A hash set.
 * Stub written by James Daly, 8 Oct 2014
 * Completed by Yiming Li
 */
#pragma once

#ifndef HASH_SET_H
#define HASH_SET_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <list>

template<class T>
class HashSet
{
public:
	HashSet(std::function<unsigned int(const T&)> hash, double loadFactor, unsigned int bucketSize = 1);
	HashSet(HashSet<T> &other);
	virtual ~HashSet();
	
	// Supplied Methods
	bool IsEmpty() const { return Size() == 0; }
	unsigned int Size() const { return size; }
	double GetLoadFactor() const { return loadFactor; }
	void DisplayItems() const { ForEach([](const T& item) { std::cout << item << std::endl; }); }
	double GetLoad() const { return ((double)Size()) / (double)NumBuckets(); }
	
	// Methods that you must complete
	unsigned int NumBuckets() const { /* TODO */ return bucketSize; }
	bool Contains(const T& item) const;
	bool Insert(const T &item);
	bool Remove(const T &item);
	void Clear();
	void ForEach(std::function<void (const T&)> func) const;
	unsigned int MaxBucketSize() const;
	double PercentEmptyBuckets() const;

private:
	unsigned int size;
	double loadFactor;
	std::function<unsigned int (const T&)> hash;
	// You need some data source to store items
	// a vector to store the bucket
	std::vector<std::list<T>> set;
	//the number of bucket in the set
	unsigned int bucketSize;

public:
	// You can put any helper methods here
	void ReHash();
	void Display();
};

/**
 * Default constructor
 */
template<class T>
HashSet<T>::HashSet(std::function<unsigned int(const T&)> hash, double loadFactor, unsigned int bucketSize):
	size(0),
	loadFactor(loadFactor),
	hash(hash)
{
	// Empty: You can put other initialization information here
	//Set the number of bucket
	this->bucketSize = bucketSize;
	std::list<T> lst;
	//Add the num of bucket into the set
	for (unsigned int i = 0; i < bucketSize; i++){
		this->set.push_back(lst);
	}
}

/**
 * Copy Constructor
 * Uses uniform instantiation to initialize itself
 * and then copies all of the items from the other set
 */
template<class T>
HashSet<T>::HashSet(HashSet<T> &other) :
	HashSet(other.hash, other.loadFactor)
{
	auto insertItem = [=](const T& x){ this->Insert(x); };
	other.ForEach(insertItem);
}

/**
 * Destructor
 * Any cleanup you need to do can be done here
 */
template<class T>
HashSet<T>::~HashSet()
{
	// Empty: You can put any steps necessary for cleanup here
	this->Clear();
}

/**
 * Returns true iff the item is in the set
 */
template<class T>
bool HashSet<T>::Contains(const T &item) const
{
	// TODO
	//Calculate the hashcode and the bucket number
	unsigned int afterHash = this->hash(item);
	unsigned int bucketNum = afterHash % (this->bucketSize);
	//Check the corresponding bucket to find the item and return the result
	auto findResult = std::find(this->set[bucketNum].begin(), this->set[bucketNum].end(), item);
	if (findResult != this->set[bucketNum].end()){
		return true;
	}
	else{
		return false;
	}
}

/**
 * Inserts the item into the set.
 * Only one copy can exist in the set at a time.
 * Returns true iff the item was successfully added.
 */
template<class T>
bool HashSet<T>::Insert(const T &item)
{
	// TODO
	//Calculate the hashcode and the bucket number
	unsigned int afterHash = this->hash(item);
	unsigned int bucketNum = afterHash % (this->bucketSize);
	//Check if the bucket contains the item, if not, insert it
	if (!this->Contains(item)){
		(this->set[bucketNum]).push_back(item);
		this->size++;
		//Check if it needs to be rehashed
		if((double)this->size / (double)this->bucketSize >= loadFactor){
			this->ReHash();
		}
		return true;
	}
	return false;
}

/**
 * Removes the item from the set.
 * Returns true iff the item was successfully removed.
 */
template<class T>
bool HashSet<T>::Remove(const T &item)
{
	// TODO
	//Calculate the hashcode and the bucket number
	unsigned int afterHash = this->hash(item);
	unsigned int bucketNum = afterHash % (this->bucketSize);
	//Check if it contains the item, then remove it
	if (this->Contains(item)){
		this->set[bucketNum].remove(item);
		this->size--;
		return true;
	}
	return false;
}

/**
 * Removes all elements from the set.
 */
template<class T>
void HashSet<T>::Clear()
{
	//Clear each item from each bucket from the set
	for (auto temp = set.begin(); temp != set.end(); temp++){
		temp->clear();
	}
	this->size = 0;
}

/**
 * Invokes the function once on each item in the set.
 * The exact order is undefined.
 */
template<class T>
void HashSet<T>::ForEach(std::function<void (const T&)> func) const
{
	//Take function func on each item in the set
	for (auto tempList = this->set.begin(); tempList != this->set.end(); tempList++){
		for (auto tempItem = tempList->begin(); tempItem != tempList->end(); tempItem++){
			func(*tempItem);
		}
	}
}

/**
 * Finds the maximum number of items in a bucket.
 */
template<class T>
unsigned int HashSet<T>::MaxBucketSize() const
{
	// TODO
	unsigned int max = 0;
	//Find the bucket which has the most items in it and return the number of the item
	for (auto list = this->set.begin(); list != this->set.end(); list++){
		if (list->size() > max){
			max = list->size();
		}
	}
	return max;
}

/**
 * Finds which fraction of the buckets are empty
 * The result is returned as a percent
 */
template<class T>
double HashSet<T>::PercentEmptyBuckets() const
{
	// TODO
	int count = 0;
	//Count how many bucket has no item in it and calculate the percentage
	for (auto list = this->set.begin(); list != this->set.end(); list++){
		if (list->empty()){
			count++;
		}
	}
	return 100.0 * count / this->set.size();
}

/**
* ReHash Function, double the BucketNum and rehash 
* all the element in the HashSet
*/
template<class T>
void HashSet<T>::ReHash()
{
	// TODO
	std::list<T> lst;
	//Make a new set with double the size as the old one
	HashSet<T> temp(this->hash, this->loadFactor, this->bucketSize * 2);
	temp.size = 0;
	unsigned int afterHash = 0;
	unsigned int bucketNum = 0;
	//copy each item from the old set and rehash into the new one
	for (auto tempList = this->set.begin(); tempList != this->set.end(); tempList++){
		for (auto tempItem = tempList->begin(); tempItem != tempList->end(); tempItem++){
			afterHash = temp.hash(*tempItem);
			bucketNum = afterHash % (temp.bucketSize);
			(temp.set[bucketNum]).push_back(*tempItem);
			temp.size++;
		}
	}	
	//copy the new one to the old set and clear the new one
	*this = temp;
	temp.Clear();
}

#endif