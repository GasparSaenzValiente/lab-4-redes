//
// Generated file, do not edit! Created by opp_msgtool 6.0 from distanceVector.msg.
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
#include <memory>
#include <type_traits>
#include "distanceVector_m.h"

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
    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) {
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

DistanceEntry::DistanceEntry()
{
}

void __doPacking(omnetpp::cCommBuffer *b, const DistanceEntry& a)
{
    doParsimPacking(b,a.destination);
    doParsimPacking(b,a.cost);
}

void __doUnpacking(omnetpp::cCommBuffer *b, DistanceEntry& a)
{
    doParsimUnpacking(b,a.destination);
    doParsimUnpacking(b,a.cost);
}

class DistanceEntryDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_destination,
        FIELD_cost,
    };
  public:
    DistanceEntryDescriptor();
    virtual ~DistanceEntryDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(DistanceEntryDescriptor)

DistanceEntryDescriptor::DistanceEntryDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(DistanceEntry)), "")
{
    propertyNames = nullptr;
}

DistanceEntryDescriptor::~DistanceEntryDescriptor()
{
    delete[] propertyNames;
}

bool DistanceEntryDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DistanceEntry *>(obj)!=nullptr;
}

const char **DistanceEntryDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *DistanceEntryDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int DistanceEntryDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int DistanceEntryDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_destination
        FD_ISEDITABLE,    // FIELD_cost
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *DistanceEntryDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "destination",
        "cost",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int DistanceEntryDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "cost") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *DistanceEntryDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_destination
        "int",    // FIELD_cost
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **DistanceEntryDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DistanceEntryDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DistanceEntryDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void DistanceEntryDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'DistanceEntry'", field);
    }
}

const char *DistanceEntryDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DistanceEntryDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        case FIELD_destination: return long2string(pp->destination);
        case FIELD_cost: return long2string(pp->cost);
        default: return "";
    }
}

void DistanceEntryDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        case FIELD_destination: pp->destination = string2long(value); break;
        case FIELD_cost: pp->cost = string2long(value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DistanceEntry'", field);
    }
}

omnetpp::cValue DistanceEntryDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        case FIELD_destination: return pp->destination;
        case FIELD_cost: return pp->cost;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'DistanceEntry' as cValue -- field index out of range?", field);
    }
}

void DistanceEntryDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        case FIELD_destination: pp->destination = omnetpp::checked_int_cast<int>(value.intValue()); break;
        case FIELD_cost: pp->cost = omnetpp::checked_int_cast<int>(value.intValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DistanceEntry'", field);
    }
}

const char *DistanceEntryDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr DistanceEntryDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void DistanceEntryDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceEntry *pp = omnetpp::fromAnyPtr<DistanceEntry>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DistanceEntry'", field);
    }
}

Register_Class(DistanceVector)

DistanceVector::DistanceVector(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

DistanceVector::DistanceVector(const DistanceVector& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

DistanceVector::~DistanceVector()
{
    delete [] this->entries;
}

DistanceVector& DistanceVector::operator=(const DistanceVector& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void DistanceVector::copy(const DistanceVector& other)
{
    this->senderId = other.senderId;
    delete [] this->entries;
    this->entries = (other.entries_arraysize==0) ? nullptr : new DistanceEntry[other.entries_arraysize];
    entries_arraysize = other.entries_arraysize;
    for (size_t i = 0; i < entries_arraysize; i++) {
        this->entries[i] = other.entries[i];
    }
}

void DistanceVector::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->senderId);
    b->pack(entries_arraysize);
    doParsimArrayPacking(b,this->entries,entries_arraysize);
}

void DistanceVector::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->senderId);
    delete [] this->entries;
    b->unpack(entries_arraysize);
    if (entries_arraysize == 0) {
        this->entries = nullptr;
    } else {
        this->entries = new DistanceEntry[entries_arraysize];
        doParsimArrayUnpacking(b,this->entries,entries_arraysize);
    }
}

int DistanceVector::getSenderId() const
{
    return this->senderId;
}

void DistanceVector::setSenderId(int senderId)
{
    this->senderId = senderId;
}

size_t DistanceVector::getEntriesArraySize() const
{
    return entries_arraysize;
}

const DistanceEntry& DistanceVector::getEntries(size_t k) const
{
    if (k >= entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    return this->entries[k];
}

void DistanceVector::setEntriesArraySize(size_t newSize)
{
    DistanceEntry *entries2 = (newSize==0) ? nullptr : new DistanceEntry[newSize];
    size_t minSize = entries_arraysize < newSize ? entries_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        entries2[i] = this->entries[i];
    delete [] this->entries;
    this->entries = entries2;
    entries_arraysize = newSize;
}

void DistanceVector::setEntries(size_t k, const DistanceEntry& entries)
{
    if (k >= entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    this->entries[k] = entries;
}

void DistanceVector::insertEntries(size_t k, const DistanceEntry& entries)
{
    if (k > entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    size_t newSize = entries_arraysize + 1;
    DistanceEntry *entries2 = new DistanceEntry[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        entries2[i] = this->entries[i];
    entries2[k] = entries;
    for (i = k + 1; i < newSize; i++)
        entries2[i] = this->entries[i-1];
    delete [] this->entries;
    this->entries = entries2;
    entries_arraysize = newSize;
}

void DistanceVector::appendEntries(const DistanceEntry& entries)
{
    insertEntries(entries_arraysize, entries);
}

void DistanceVector::eraseEntries(size_t k)
{
    if (k >= entries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)entries_arraysize, (unsigned long)k);
    size_t newSize = entries_arraysize - 1;
    DistanceEntry *entries2 = (newSize == 0) ? nullptr : new DistanceEntry[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        entries2[i] = this->entries[i];
    for (i = k; i < newSize; i++)
        entries2[i] = this->entries[i+1];
    delete [] this->entries;
    this->entries = entries2;
    entries_arraysize = newSize;
}

class DistanceVectorDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_senderId,
        FIELD_entries,
    };
  public:
    DistanceVectorDescriptor();
    virtual ~DistanceVectorDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(DistanceVectorDescriptor)

DistanceVectorDescriptor::DistanceVectorDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(DistanceVector)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

DistanceVectorDescriptor::~DistanceVectorDescriptor()
{
    delete[] propertyNames;
}

bool DistanceVectorDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DistanceVector *>(obj)!=nullptr;
}

const char **DistanceVectorDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *DistanceVectorDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int DistanceVectorDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int DistanceVectorDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_senderId
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISRESIZABLE,    // FIELD_entries
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *DistanceVectorDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderId",
        "entries",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int DistanceVectorDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "senderId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "entries") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *DistanceVectorDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_senderId
        "DistanceEntry",    // FIELD_entries
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **DistanceVectorDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DistanceVectorDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DistanceVectorDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        case FIELD_entries: return pp->getEntriesArraySize();
        default: return 0;
    }
}

void DistanceVectorDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        case FIELD_entries: pp->setEntriesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'DistanceVector'", field);
    }
}

const char *DistanceVectorDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DistanceVectorDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        case FIELD_senderId: return long2string(pp->getSenderId());
        case FIELD_entries: return "";
        default: return "";
    }
}

void DistanceVectorDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        case FIELD_senderId: pp->setSenderId(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DistanceVector'", field);
    }
}

omnetpp::cValue DistanceVectorDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        case FIELD_senderId: return pp->getSenderId();
        case FIELD_entries: return omnetpp::toAnyPtr(&pp->getEntries(i)); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'DistanceVector' as cValue -- field index out of range?", field);
    }
}

void DistanceVectorDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        case FIELD_senderId: pp->setSenderId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DistanceVector'", field);
    }
}

const char *DistanceVectorDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_entries: return omnetpp::opp_typename(typeid(DistanceEntry));
        default: return nullptr;
    };
}

omnetpp::any_ptr DistanceVectorDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        case FIELD_entries: return omnetpp::toAnyPtr(&pp->getEntries(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void DistanceVectorDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    DistanceVector *pp = omnetpp::fromAnyPtr<DistanceVector>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DistanceVector'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

