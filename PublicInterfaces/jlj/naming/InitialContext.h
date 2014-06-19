// InitialContext.h

//********************************************************************

#ifndef jlj_naming_InitialContext_h
#define jlj_naming_InitialContext_h

//********************************************************************

#include <jlj/naming/AbstractContext.h>
#include <jlj/util/AbstractNamedSingleton.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

/**
*
* configuration : a JSON string containing information of which concrete InitialContext classes should be loaded
*
*/

/** sample

{
    "java.naming.factory.initial": [
        {
            "class" : "jmstools::naming::JMSInitialContext",
            "library" : "JMSTOOLSutil",
            "configuration" : {
                "jms" : {
                    "reactor" : {
                        "queueconnectionfactory1" : {
                            "class" : "jmstools::implementation::reactor::QueueConnectionFactory",
                            "library" : "JMSreactorimplementation",
                            "configuration" : {
                                "providerhost" : "mndemucappt2" 
                            } 
                        } ,
                        "queueconnectionfactory2" : {
                            "class" : "jmstools::implementation::reactor::QueueConnectionFactory",
                            "library" : "JMSreactorimplementation",
                            "configuration" : {
                                "providerhost" : "mndemucpc042750" 
                            } 
                        } ,
                        "jmslogger" : {
                            "name" : "JMS_LOGGER",
                            "level" : "OFF" 
                        } ,
                        "reactorlogger" : {
                            "name" : "REACTORS_LOGGER",
                            "level" : "OFF" 
                        } 
                    } ,
                    "activemq" : {
                        "queueconnectionfactory1" : {
                            "class" : "jmstools::implementation::activemq::QueueConnectionFactory",
                            "library" : "JMSactivemqimplementation",
                            "configuration" : "tcp:localhost:61616" 
                        } ,
                        "jmslogger" : {
                            "name" : "JMS_LOGGER",
                            "level" : "FINEST" 
                        } 
                    } 
                } 
            } 
        } 
    ] ,
    "globallogger" : {
        "name" : "GLOBAL_LOGGER",
        "level" : "OFF" 
    } 
}

*/

//********************************************************************

class ExportedByJLJnaming InitialContextI
	: public AbstractContextI
{
protected:
	~InitialContextI() throw (Exception);
	friend class TypedReference<InitialContextI>;

public:
	InitialContextI() throw (Exception);

	void bind(const String& name, Object) throw (NamingException);
	bool isBound(const String& name) const;
	Object lookup(const String& name) const throw (NamingException);
	template< typename T > void lookup(const String& name, T&) const throw (NamingException);
	Map<String, Class> list(const String& contextname) const throw (NamingException);
	Map<String, Object> listBindings(const String& contextname) const throw (NamingException);
	void unbind(const String& name) throw (NamingException);

	void loadBindings(const String& configuration) throw (Exception);
	void unloadBindings() throw (Exception);
	Object lookupClass(const String& name, const String& clazzname) const throw (NamingException);

protected:
	template< typename T > void lookupURL(const String& name, T&) const throw (NamingException);

private:
	void loadEnvironmentBindings() throw (Exception);
	String loadConfiguration(const String& defaultconfiguration) throw (Exception);
	List<AbstractContext> loadedcontexts;

};

//********************************************************************

template< typename T > 
void InitialContextI::lookup(const String& name, T& value) const throw (NamingException)
{
	AbstractContextI::lookup(name, value);
}

//********************************************************************

template< typename T > 
void InitialContextI::lookupURL(const String& name, T& value) const throw (NamingException)
{
	AbstractContextI::lookupURL(name, value);
}

//********************************************************************

struct ExportedByJLJnaming InitialContext 
	: StrongReference<InitialContextI>
	, AbstractNamedSingleton<InitialContext, InitialContextI>
{
	StrongReferenceCommonDerivateImplementation(InitialContext);
};


//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
