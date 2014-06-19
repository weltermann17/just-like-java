// Level.cpp

//********************************************************************

#ifndef Level_cpp
#define Level_cpp

//********************************************************************

#include <jlj/util/logging/Level.h>

//********************************************************************

#include <limits.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

LevelI::~LevelI() 
{}

//********************************************************************

LevelI::LevelI(const String& name,
			   int value) 
			   : name(name->toUpperCase())
			   , value(value)
{}

//********************************************************************

int LevelI::hashCode() const
{
	return name->hashCode();
}

//********************************************************************

bool LevelI::equals(const Object& object) const
{
	Level b;
	if (object->instanceOf(b))
	{
		return toString()->equals(b->toString());
	}
	else
	{
		return false;
	}
}

//********************************************************************

String LevelI::toString() const
{
	StringBuffer buf;
	buf->append(name);
	buf->append(L"@");
	buf->append(value);
	return buf->toString();
}

//********************************************************************

String LevelI::getName() const
{
	return name;
}

//********************************************************************

int LevelI::intValue() const
{
	return value;
}

//********************************************************************
//********************************************************************

Level Level::ALL;
Level Level::CONFIG;
Level Level::FINE;
Level Level::FINER;
Level Level::FINEST;
Level Level::INFO;
Level Level::OFF;
Level Level::SEVERE;
Level Level::WARNING;

//********************************************************************

Level Level::parse(const String& name,
				   int value)
{
	return new LevelI(name->toUpperCase(), value);
}

//********************************************************************

Level Level::parse(const String& name)
{
	return new LevelI(name->toUpperCase(), getValue(name));	
}

//********************************************************************

void Level::init()
{
	ALL = parse(L"ALL", INT_MIN);
	CONFIG = parse(L"CONFIG", 700);
	FINE = parse(L"FINE", 500);
	FINER = parse(L"FINER", 400);
	FINEST = parse(L"FINEST", 300);
	INFO = parse(L"INFO", 800);
	OFF = parse(L"OFF", INT_MAX);
	SEVERE = parse(L"SEVERE", 1000);
	WARNING = parse(L"WARNING", 900);
}

//********************************************************************
int Level::getValue(const String& level)
{
	String name = level->toUpperCase();
	if(name == L"ALL")
		return INT_MIN;
	else if (name == L"CONFIG")
		return 700;
	else if (name == L"FINE")
		return 500;
	else if (name == L"FINER")
		return 400;
	else if (name == L"FINEST")
		return 300;
	else if (name == L"INFO")
		return 800;
	else if (name == L"OFF")
		return INT_MAX;
	else if (name == L"SEVERE")
		return 1000;
	else if (name == L"WARNING")
		return 900;
	else
		return 1000;
}

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
