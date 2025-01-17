#ifndef __XML_DOCUMENT_H__
#define __XML_DOCUMENT_H__

#include "lion/thirdparty/include/tinyxml2.h"

#include "Xml_element.h"
#include "lion/thirdparty/include/logger.hpp"

class Xml_document
{
 public:
    //! Constructor
    //! @param[in] name: name of the file
    Xml_document(const std::string& name = "", bool load_file = false) : _name(name) { if (load_file) load();} 

    //! Destructor
    ~Xml_document() { _doc.Clear(); }

    //! Load an XML document from file
    //! @return true if OK, false if NOT OK
    void load();

    //! Save an XML document to file
    //! @return true if OK, false if NOT OK
    bool save() { return _doc.SaveFile(_name.c_str()) == tinyxml2::XML_SUCCESS; }

    //! Save an XML document to file, name given as input
    //! @param[in] name: name of the file;
    //! @return true if OK, false if NOT OK
    bool save(const std::string& name) { _name = name; return save(); }

    //! Parse an XML document from a string
    void parse(const std::string& contents) { _doc.Parse(contents.c_str()); }

    //! Get the root element
    Xml_element get_root_element() { return _doc.RootElement(); } 

    //! Get an element
    Xml_element get_element(const std::string& name);

    //! Add an element from its full path
    Xml_element add_element(const std::string& full_path);

    //! See if an element exists
    bool has_element(const std::string& name);

    //! Create a root element
    void create_root_element(const std::string& name) { _doc.Parse(std::string("<" + name + "></" + name + ">").c_str()); }

    //! Print
    void print(std::ostream&) const;

 private:
    std::string _name;              //! Name of the file on disk
    tinyxml2::XMLDocument _doc;     //! Tinyxml2 document object
};


inline void Xml_document::print(std::ostream& os) const
{
    tinyxml2::XMLPrinter printer;
    _doc.Accept(&printer);
    os << printer.CStr();
}


inline Xml_element Xml_document::get_element(const std::string& name)
{
    // Divide the string in root_name/the_rest
    std::string::size_type pos = name.find('/');
    std::string root_name;
    std::string the_rest;

    if (pos != std::string::npos)
    {
        root_name = name.substr(0, pos);
        the_rest = name.substr(pos+1);
    }
    else
    {
        root_name = name;
    }

    // Look for root_name, and make sure it is the only occurrence
    // Check that the root name coincides 
    if ( root_name != get_root_element().get_name() )
        throw lion_exception("Root name does not match");

    if ( the_rest.size() == 0 )
        return get_root_element();

    else
        return get_root_element().get_child(the_rest);
}

inline Xml_element Xml_document::add_element(const std::string& full_path)
{
    // Divide the string in root_name/the_rest
    std::string::size_type pos = full_path.find('/');
    std::string root_name;
    std::string the_rest;

    if (pos != std::string::npos)
    {
        root_name = full_path.substr(0, pos);
        the_rest = full_path.substr(pos+1);
    }
    else
    {
        root_name = full_path;
    }

    // Look for root_name, and make sure it is the only occurrence
    // Check that the root full_path coincides 
    if ( root_name != get_root_element().get_name() )
        throw lion_exception("Root full_path does not match");

    if ( the_rest.size() == 0 )
        throw lion_exception("Element already exists");

    else
        return get_root_element().add_child(the_rest);
}


inline void Xml_document::load() 
{ 
    switch (_doc.LoadFile(_name.c_str()))
    {
      case(tinyxml2::XML_SUCCESS):
        break;

      case(tinyxml2::XML_NO_ATTRIBUTE):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_NO_ATTRIBUTE");
        break;

      case(tinyxml2::XML_WRONG_ATTRIBUTE_TYPE):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_WRONG_ATTRIBUTE_TYPE");
        break;

      case(tinyxml2::XML_ERROR_FILE_NOT_FOUND):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_FILE_NOT_FOUND");
        break;

      case(tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_FILE_COULD_NOT_BE_OPENED");
        break;

      case(tinyxml2::XML_ERROR_FILE_READ_ERROR):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_FILE_READ_ERROR");
        break;

      case(tinyxml2::XML_ERROR_PARSING_ELEMENT):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING_ELEMENT");
        break;

      case(tinyxml2::XML_ERROR_PARSING_ATTRIBUTE):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING_ATTRIBUTE");
        break;

      case(tinyxml2::XML_ERROR_PARSING_TEXT):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING_TEXT");
        break;

      case(tinyxml2::XML_ERROR_PARSING_CDATA):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING_CDATA");
        break;

      case(tinyxml2::XML_ERROR_PARSING_COMMENT):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING_COMMENT");
        break;

      case(tinyxml2::XML_ERROR_PARSING_DECLARATION):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING_DECLARATION");
        break;

      case(tinyxml2::XML_ERROR_PARSING_UNKNOWN):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING_UNKNOWN");
        break;

      case(tinyxml2::XML_ERROR_EMPTY_DOCUMENT):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_EMPTY_DOCUMENT");
        break;

      case(tinyxml2::XML_ERROR_MISMATCHED_ELEMENT):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_MISMATCHED_ELEMENT");
        break;

      case(tinyxml2::XML_ERROR_PARSING):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_PARSING");
        break;

      case(tinyxml2::XML_CAN_NOT_CONVERT_TEXT):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_CAN_NOT_CONVERT_TEXT");
        break;

      case(tinyxml2::XML_NO_TEXT_NODE):
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_NO_TEXT_NODE");
        break;

	  case(tinyxml2::XML_ELEMENT_DEPTH_EXCEEDED):
	    throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ELEMENT_DEPTH_EXCEEDED");
        break;

	  case(tinyxml2::XML_ERROR_COUNT):
	    throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: XML_ERROR_COUNT");
        break;

      default:
        throw lion_exception("[ERROR] Xml_document::load() -> Tinyxml2 error: error code was not recognized");
    }
}

inline bool Xml_document::has_element(const std::string& name)
{
    // Divide the string in root_name/the_rest
    std::string::size_type pos = name.find('/');
    std::string root_name;
    std::string the_rest;

    if (pos != std::string::npos)
    {
        root_name = name.substr(0, pos);
        the_rest = name.substr(pos+1);
    }
    else
    {
        root_name = name;
    }

    // Look for root_name, and make sure it is the only occurrence
    // Check that the root name coincides 
    if ( root_name != get_root_element().get_name() )
        return false;

    if ( the_rest.size() == 0 )
        return true;

    else
        return get_root_element().has_child(the_rest);
}


inline std::ostream& operator<<(std::ostream& os, const Xml_document& doc)
{
    doc.print(os);
    return os;
}

#endif
