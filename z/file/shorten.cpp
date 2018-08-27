#include "shorten.h"

namespace z
{
	namespace file
	{
		core::string<utf8> shorten(const core::string<utf8>& dir)
		{
			core::string<utf8> output = dir;
			output.replace("\\", "/");
			output.cutDuplicates("/");
			output.replace("/./", "/");

			core::string<utf8> lastDir = "/../";
			core::string<utf8> slash = "/";
			int index = output.find(lastDir);

			while (index >= 0)
			{
				if (index)
				{
					int last = output.findBefore(slash, index-1);
					if (last < 0)
						output.replace(0, index-last+lastDir.length(), "./");
					else
						output.replace(last, index-last+lastDir.length(), slash);
				}

				index = output.findAfter(lastDir, index+1);
			}

			lastDir = "/..";
			if ((output.length() > lastDir.length()) && output.endsWith(lastDir))
			{
				int last = output.findBefore(slash, output.length()-lastDir.length()-1);
				if (last < 0)
					output = ".";
				else
					output.remove(last, index-last+lastDir.length());
			}

			return output;
		}
	}
}
