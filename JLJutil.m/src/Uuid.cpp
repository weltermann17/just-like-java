// Uuid.cpp
//********************************************************************

#ifndef Uuid_cpp
#define Uuid_cpp

//********************************************************************

#include <jlj/util/Uuid.h>
#include <jlj/lang/Throwable.h>
#include <jlj/lang/StringBuffer.h>
#include <jlj/util/Queue.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/Collections.h>
#include <jlj/util/concurrent/locks/ReentrantLock.h>
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>			 
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

#include <string.h>
#include "CATUuid.h"

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

UuidI::~UuidI() 
{}

//********************************************************************

UuidI::UuidI()
{
	CATUuid newuuid;
	char buffer[64];
	memset(buffer, 0, sizeof(buffer));
	unsigned char buf[16];
	newuuid.get(buf);
	sprintf(buffer, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
		buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], 
		buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
	uuid = String(buffer, strlen(buffer));
}

//********************************************************************

int UuidI::hashCode() const
{
	return uuid->hashCode();
}

//********************************************************************

bool UuidI::equals(const Object& object) const
{
	Uuid b;
	if (object->instanceOf(b))
	{
		return uuid->equals(b->toString());
	}
	else
	{
		return false;
	}
}

//********************************************************************

int UuidI::compareTo(const Object& object) const
{
	Uuid b;
	object->downcast(b);
	return uuid < b->uuid ? -1 : uuid->equals(b->uuid) ? 0 : 1;
}

//********************************************************************

String UuidI::toString() const
{
	return uuid;
}

//********************************************************************
//********************************************************************

Uuid Uuid::randomUuid() 
{
	const int BATCHSIZE = 16;
	SynchronizedList<Uuid> uuids;
	LifetimeManager::getInstance(L"jlj")->get(L"uuids", uuids);

	try
	{
		if (0 == uuids->size())
		{
			synchronized(uuids->getLock())
			{
				for (int i = 0; i < BATCHSIZE; ++i)
				{
					Uuid uuid = new UuidI(); 
					uuids->add(uuid);
				}
			}
		}
		return uuids->removeAt(0);
	}
	catch (const Exception&)
	{
		synchronized(uuids->getLock())
		{
			for (int i = 0; i < BATCHSIZE; ++i)
			{
				uuids->add(new UuidI());
			}
		}
	
		return uuids->removeAt(0);
	}
}

//********************************************************************

bool operator<(const Uuid& a, const Uuid& b)
{
	return a->toString() < b->toString();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
