//
// Generated file, do not edit! Created by nedtool 5.7 from message/InfoMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "InfoMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(InfoMessage)

InfoMessage::InfoMessage(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->packetId = 2;
    this->senderId = 0;
    this->receiverId = 0;
    this->LQI = 0;
    this->RSSI = 0;
    this->day = 0;
    this->timePeriod = 0;
    this->timestamp = 0;
}

InfoMessage::InfoMessage(const InfoMessage& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

InfoMessage::~InfoMessage()
{
}

InfoMessage& InfoMessage::operator=(const InfoMessage& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void InfoMessage::copy(const InfoMessage& other)
{
    this->packetId = other.packetId;
    this->senderId = other.senderId;
    this->receiverId = other.receiverId;
    this->LQI = other.LQI;
    this->RSSI = other.RSSI;
    this->day = other.day;
    this->timePeriod = other.timePeriod;
    this->timestamp = other.timestamp;
}

void InfoMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->packetId);
    doParsimPacking(b,this->senderId);
    doParsimPacking(b,this->receiverId);
    doParsimPacking(b,this->LQI);
    doParsimPacking(b,this->RSSI);
    doParsimPacking(b,this->day);
    doParsimPacking(b,this->timePeriod);
    doParsimPacking(b,this->timestamp);
}

void InfoMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->packetId);
    doParsimUnpacking(b,this->senderId);
    doParsimUnpacking(b,this->receiverId);
    doParsimUnpacking(b,this->LQI);
    doParsimUnpacking(b,this->RSSI);
    doParsimUnpacking(b,this->day);
    doParsimUnpacking(b,this->timePeriod);
    doParsimUnpacking(b,this->timestamp);
}

int InfoMessage::getPacketId() const
{
    return this->packetId;
}

void InfoMessage::setPacketId(int packetId)
{
    this->packetId = packetId;
}

int InfoMessage::getSenderId() const
{
    return this->senderId;
}

void InfoMessage::setSenderId(int senderId)
{
    this->senderId = senderId;
}

int InfoMessage::getReceiverId() const
{
    return this->receiverId;
}

void InfoMessage::setReceiverId(int receiverId)
{
    this->receiverId = receiverId;
}

int InfoMessage::getLQI() const
{
    return this->LQI;
}

void InfoMessage::setLQI(int LQI)
{
    this->LQI = LQI;
}

int InfoMessage::getRSSI() const
{
    return this->RSSI;
}

void InfoMessage::setRSSI(int RSSI)
{
    this->RSSI = RSSI;
}

int InfoMessage::getDay() const
{
    return this->day;
}

void InfoMessage::setDay(int day)
{
    this->day = day;
}

int InfoMessage::getTimePeriod() const
{
    return this->timePeriod;
}

void InfoMessage::setTimePeriod(int timePeriod)
{
    this->timePeriod = timePeriod;
}

::omnetpp::simtime_t InfoMessage::getTimestamp() const
{
    return this->timestamp;
}

void InfoMessage::setTimestamp(::omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

class InfoMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    InfoMessageDescriptor();
    virtual ~InfoMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(InfoMessageDescriptor)

InfoMessageDescriptor::InfoMessageDescriptor() : omnetpp::cClassDescriptor("InfoMessage", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

InfoMessageDescriptor::~InfoMessageDescriptor()
{
    delete[] propertynames;
}

bool InfoMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<InfoMessage *>(obj)!=nullptr;
}

const char **InfoMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *InfoMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int InfoMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int InfoMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *InfoMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "packetId",
        "senderId",
        "receiverId",
        "LQI",
        "RSSI",
        "day",
        "timePeriod",
        "timestamp",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int InfoMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderId")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "receiverId")==0) return base+2;
    if (fieldName[0]=='L' && strcmp(fieldName, "LQI")==0) return base+3;
    if (fieldName[0]=='R' && strcmp(fieldName, "RSSI")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "day")==0) return base+5;
    if (fieldName[0]=='t' && strcmp(fieldName, "timePeriod")==0) return base+6;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamp")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *InfoMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "simtime_t",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **InfoMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *InfoMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int InfoMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    InfoMessage *pp = (InfoMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *InfoMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    InfoMessage *pp = (InfoMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string InfoMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    InfoMessage *pp = (InfoMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketId());
        case 1: return long2string(pp->getSenderId());
        case 2: return long2string(pp->getReceiverId());
        case 3: return long2string(pp->getLQI());
        case 4: return long2string(pp->getRSSI());
        case 5: return long2string(pp->getDay());
        case 6: return long2string(pp->getTimePeriod());
        case 7: return simtime2string(pp->getTimestamp());
        default: return "";
    }
}

bool InfoMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    InfoMessage *pp = (InfoMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketId(string2long(value)); return true;
        case 1: pp->setSenderId(string2long(value)); return true;
        case 2: pp->setReceiverId(string2long(value)); return true;
        case 3: pp->setLQI(string2long(value)); return true;
        case 4: pp->setRSSI(string2long(value)); return true;
        case 5: pp->setDay(string2long(value)); return true;
        case 6: pp->setTimePeriod(string2long(value)); return true;
        case 7: pp->setTimestamp(string2simtime(value)); return true;
        default: return false;
    }
}

const char *InfoMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *InfoMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    InfoMessage *pp = (InfoMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


