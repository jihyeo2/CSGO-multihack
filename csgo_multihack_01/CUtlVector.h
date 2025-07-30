#pragma once

template <typename T>
struct CUtlVector
{
	T* m_pData;
	int m_nSize;
	int m_nCapacity;

	T& operator[](int i) {
		return m_pData[i];
	}

	// read-only
	const T& operator[](int i) const {
		return m_pData[i];
	}
};