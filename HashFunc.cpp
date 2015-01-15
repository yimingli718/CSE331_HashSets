#include "HashFunc.h"
#include <cmath>

using namespace std;

/**
 * The HashA comes from the Java string hash code
 */
unsigned int HashA(const std::string& s)
{
	unsigned int result = 0;
	for (unsigned int i = 0; i < s.length(); i++){
		//result = s[i] + 31;
		result += s[i] * pow(31, s.length() - 1 - i);
	}
	return result;
}

/**
 * HashB borrows the idea of Java string hash code,
 * but is much simplifier
 */
unsigned int HashB(const std::string& s)
{
	// TODO
	unsigned int result = 0;
		for (unsigned int i = 0; i < s.length(); i++){
			result = s[i];
		}
	return result;
}

/**
 * HashC only consider the first and last character of the string,
 * which is much simple than HashA and HashB
 */
unsigned int HashC(const std::string& s)
{
	unsigned int result = s[0] + s[10];
	return result;
}