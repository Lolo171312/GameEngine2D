#pragma once

#include "string"

class Utils
{
public:
	static inline std::string extractPath(const std::string& filename)
	{
		std::string filenameCopy = filename;
		while (filenameCopy.find("\\") != std::string::npos)
		{
			filenameCopy.replace(filenameCopy.find("\\"), 1, "/");
		}
		filenameCopy = filenameCopy.substr(0, filenameCopy.rfind('/'));
		if (filenameCopy.size() > 0) filenameCopy += "/";
		return filenameCopy;
	}
};