#pragma once

#include "stdafx.h"

class UTL_Cmd
{
public:
	enum ARGUMENT_TYPE { _STRING, _TRUE, _BOOL, _INT, _COLOR };
	struct ARGUMENT {
		vector<wstring>	text;
		ARGUMENT_TYPE	type;
		void*			var;
		wstring			help;
	};

private:
	vector<ARGUMENT> arguments;

public:
	UTL_Cmd();
	~UTL_Cmd();
	void Add(ARGUMENT_TYPE type, void* var, int num, ...);

	int ParseCommandLinbe(int argc, _TCHAR* argv[], int& iCorrectParameters);

	void Help();
};
