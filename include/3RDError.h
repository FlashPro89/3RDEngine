#pragma once

#ifndef _3RDE_ERROR_H_
#define _3RDE_ERROR_H_

#include <string>

//base class
class gErrorHandler
{
public:
	virtual ~gErrorHandler();

	const std::string getErrorLocation() const;
	const std::string getErrorMessage() const;
	const std::string getFileName() const;
	const std::string getFileLine() const;

protected:
	gErrorHandler() {};
};

#endif
