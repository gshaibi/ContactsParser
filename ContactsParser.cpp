#include <algorithm> //std::for_each
#include <boost/bind.hpp>

#include "base64.h"

#include "ContactsParser.hpp"

ContactsParser::ContactsParser(std::ifstream &data_, std::ofstream &dest_)
 : m_src(data_), m_dest(dest_), m_contacts()
{
	if (!m_dest.is_open())
	{
		throw std::runtime_error("Error creating destination file.");
	}

	// Get whole data
	while (!data_.eof())
	{
		std::string lineType = Read(4);
		GetLineData(lineType);
	}
	// Write data to destination stream.
	std::for_each(m_contacts.begin(), m_contacts.end(), boost::bind(&ContactsParser::WriteToFile, this, _1));
}

void ContactsParser::WriteToFile(std::pair<ContactID, ContactInfo> contact_)
{
	m_dest << contact_.second.m_firstName << std::endl;
	m_dest << contact_.second.m_lastName << std::endl;
	m_dest << contact_.second.m_phoneNumber << std::endl;
	m_dest << contact_.second.m_date << std::endl << std::endl;
}

void ContactsParser::GetLineData(LineType type_)
{
	if (type_.compare("86B7") == 0)
	{
		GetLine(&ContactsParser::ContactInfo::m_firstName);
	}
	else if (type_.compare("9E60") == 0)
	{
		GetLine(&ContactInfo::m_lastName);
	}
	else if (type_.compare("5159") == 0)
	{
		GetLine(&ContactInfo::m_phoneNumber);
	}
	else if (type_.compare("D812") == 0)
	{
		GetDateLine();
	}
	else if (type_.compare("6704") == 0)
	{
		GetImageLine();
	}
}

std::string ContactsParser::Read(size_t numBytes_)
{
	std::string ret;

	ret.reserve(numBytes_);
	std::copy_n(std::istreambuf_iterator<char>(m_src), numBytes_,
				std::back_inserter(ret));
	m_src.ignore(1); // Ignoring last copied character.
	return ret;
}

ContactsParser::ContactID ContactsParser::GetID()
{
	ContactID ret(Read(4));
	return ret;
}

long int ContactsParser::GetLen()
{
	std::string lenStr(Read(5));
	return std::strtol(lenStr.c_str(), NULL, 16);
}

ContactsParser::MetaData ContactsParser::GetMetaData()
{
	return MetaData{GetID(), GetLen()};
}

void ContactsParser::GetLine(std::string ContactInfo::*type_)
{
	while (std::isalnum(m_src.peek()))
	{
		MetaData md(GetMetaData());
		std::string data(Read(md.m_length));
		m_contacts[md.m_id].*type_ = data;
	}
	m_src.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ContactsParser::GetDateLine()
{
	while (std::isalnum(m_src.peek()))
	{
		MetaData md(GetMetaData());
		std::string dateStr(Read(md.m_length));
		time_t date(std::strtol(dateStr.c_str(), NULL, 10));
		m_contacts[md.m_id].m_date = ctime(&date);
	}
	m_src.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ContactsParser::GetImageLine()
{
	while (std::isalnum(m_src.peek()))
	{
		MetaData md(GetMetaData());
		std::string base64Img(Read(md.m_length));
		std::string binaryImg(base64_decode(base64Img));
		std::ofstream dest(std::string(m_contacts[md.m_id].m_firstName));
		dest << binaryImg;
	}
	m_src.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}