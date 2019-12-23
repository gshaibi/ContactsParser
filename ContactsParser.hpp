#ifndef CONTACTS_PARSER_HPP
#define CONTACTS_PARSER_HPP

#include <iostream>
#include <boost/noncopyable.hpp>
#include <map>
#include <fstream>

class ContactsParser : boost::noncopyable
{
public:
	// Contacts will be redirected to result_, 
	// except images that will be created in working directory.
	// Result will be in western ISO - 8859-1 encoding.
	explicit ContactsParser(std::ifstream& src_, std::ofstream &result_);
	//generated dtor.

private:
	typedef std::string ContactID;
	typedef std::string LineType;

	struct ContactInfo
	{
		std::string m_firstName;
		std::string m_lastName;
		std::string m_phoneNumber;
		std::string m_date;
	};

	struct MetaData
	{
		ContactID m_id;
		long int m_length;
	};

	void GetLineData(LineType type_);
	void GetDateLine();
	void GetImageLine();
	void GetLine(std::string ContactInfo::*type_);
	void WriteToFile(std::pair<ContactID, ContactInfo> contact_);

	std::string Read(size_t numBytes_);
	ContactID GetID();
	long int GetLen();
	MetaData GetMetaData();

	std::ifstream& m_src;
	std::ofstream&	m_dest;
	std::map<ContactID, ContactInfo> m_contacts;
};


#endif // CONTACTS_PARSER_HPP
