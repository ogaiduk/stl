#include "Exceptions.h"


InvalidValueError::InvalidValueError(const std::string& p_error) : m_error(p_error) {}

const char* InvalidValueError::what() const {
	return m_error.c_str();
}


ItemNotFoundError::ItemNotFoundError(const std::string& p_error) : m_error(p_error) {}

const char* ItemNotFoundError::what() const {
	return m_error.c_str();
}


IteratorError::IteratorError(const std::string& p_error) : m_error(p_error) {}

const char* IteratorError::what() const {
	return m_error.c_str();
}

