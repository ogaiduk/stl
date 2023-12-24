#pragma once
#include <exception>
#include <string>


class InvalidValueError: public std::exception {
private:
	std::string m_error;
public:
	InvalidValueError(const std::string& p_error);

	const char* what() const;
};


class ItemNotFoundError : public std::exception {
private:
	std::string m_error;
public:
	ItemNotFoundError(const std::string& p_error);

	const char* what() const;
};


class IteratorError : public std::exception {
private:
	std::string m_error;
public:
	IteratorError(const std::string& p_error);

	const char* what() const;
};

