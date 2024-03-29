/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef tutorial_TYPES_H
#define tutorial_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>




struct Operation {
  enum type {
    ADD = 1,
    SUBTRACT = 2,
    MULTIPLY = 3,
    DIVIDE = 4
  };
};

extern const std::map<int, const char*> _Operation_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const Operation::type& val);

class Work;

class InvalidOperation;

class TestSetMap;

class Simple;

class SimpleWrap;

typedef struct _Work__isset {
  _Work__isset() : num1(true), num2(false), op(false), comment(false) {}
  bool num1 :1;
  bool num2 :1;
  bool op :1;
  bool comment :1;
} _Work__isset;

class Work : public virtual ::apache::thrift::TBase {
 public:

  Work(const Work&);
  Work& operator=(const Work&);
  Work() : num1(0), num2(0), op((Operation::type)0), comment() {
  }

  virtual ~Work() throw();
  int32_t num1;
  int32_t num2;
  Operation::type op;
  std::string comment;

  _Work__isset __isset;

  void __set_num1(const int32_t val);

  void __set_num2(const int32_t val);

  void __set_op(const Operation::type val);

  void __set_comment(const std::string& val);

  bool operator == (const Work & rhs) const
  {
    if (!(num1 == rhs.num1))
      return false;
    if (!(num2 == rhs.num2))
      return false;
    if (!(op == rhs.op))
      return false;
    if (__isset.comment != rhs.__isset.comment)
      return false;
    else if (__isset.comment && !(comment == rhs.comment))
      return false;
    return true;
  }
  bool operator != (const Work &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Work & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(Work &a, Work &b);

std::ostream& operator<<(std::ostream& out, const Work& obj);

typedef struct _InvalidOperation__isset {
  _InvalidOperation__isset() : whatOp(false), why(false) {}
  bool whatOp :1;
  bool why :1;
} _InvalidOperation__isset;

class InvalidOperation : public ::apache::thrift::TException {
 public:

  InvalidOperation(const InvalidOperation&);
  InvalidOperation& operator=(const InvalidOperation&);
  InvalidOperation() : whatOp(0), why() {
  }

  virtual ~InvalidOperation() throw();
  int32_t whatOp;
  std::string why;

  _InvalidOperation__isset __isset;

  void __set_whatOp(const int32_t val);

  void __set_why(const std::string& val);

  bool operator == (const InvalidOperation & rhs) const
  {
    if (!(whatOp == rhs.whatOp))
      return false;
    if (!(why == rhs.why))
      return false;
    return true;
  }
  bool operator != (const InvalidOperation &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const InvalidOperation & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(InvalidOperation &a, InvalidOperation &b);

std::ostream& operator<<(std::ostream& out, const InvalidOperation& obj);

typedef struct _TestSetMap__isset {
  _TestSetMap__isset() : vInt(false), setInt(false), mStrInt(false) {}
  bool vInt :1;
  bool setInt :1;
  bool mStrInt :1;
} _TestSetMap__isset;

class TestSetMap : public virtual ::apache::thrift::TBase {
 public:

  TestSetMap(const TestSetMap&);
  TestSetMap& operator=(const TestSetMap&);
  TestSetMap() {
  }

  virtual ~TestSetMap() throw();
  std::vector<int32_t>  vInt;
  std::set<int32_t>  setInt;
  std::map<std::string, int32_t>  mStrInt;

  _TestSetMap__isset __isset;

  void __set_vInt(const std::vector<int32_t> & val);

  void __set_setInt(const std::set<int32_t> & val);

  void __set_mStrInt(const std::map<std::string, int32_t> & val);

  bool operator == (const TestSetMap & rhs) const
  {
    if (!(vInt == rhs.vInt))
      return false;
    if (!(setInt == rhs.setInt))
      return false;
    if (!(mStrInt == rhs.mStrInt))
      return false;
    return true;
  }
  bool operator != (const TestSetMap &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TestSetMap & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TestSetMap &a, TestSetMap &b);

std::ostream& operator<<(std::ostream& out, const TestSetMap& obj);

typedef struct _Simple__isset {
  _Simple__isset() : iVal(false), bFlag(false), str(false) {}
  bool iVal :1;
  bool bFlag :1;
  bool str :1;
} _Simple__isset;

class Simple : public virtual ::apache::thrift::TBase {
 public:

  Simple(const Simple&);
  Simple& operator=(const Simple&);
  Simple() : iVal(0), bFlag(0), str() {
  }

  virtual ~Simple() throw();
  int32_t iVal;
  bool bFlag;
  std::string str;

  _Simple__isset __isset;

  void __set_iVal(const int32_t val);

  void __set_bFlag(const bool val);

  void __set_str(const std::string& val);

  bool operator == (const Simple & rhs) const
  {
    if (!(iVal == rhs.iVal))
      return false;
    if (!(bFlag == rhs.bFlag))
      return false;
    if (!(str == rhs.str))
      return false;
    return true;
  }
  bool operator != (const Simple &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Simple & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(Simple &a, Simple &b);

std::ostream& operator<<(std::ostream& out, const Simple& obj);

typedef struct _SimpleWrap__isset {
  _SimpleWrap__isset() : tSimple(false), iId(false) {}
  bool tSimple :1;
  bool iId :1;
} _SimpleWrap__isset;

class SimpleWrap : public virtual ::apache::thrift::TBase {
 public:

  SimpleWrap(const SimpleWrap&);
  SimpleWrap& operator=(const SimpleWrap&);
  SimpleWrap() : iId(0) {
  }

  virtual ~SimpleWrap() throw();
  Simple tSimple;
  int32_t iId;

  _SimpleWrap__isset __isset;

  void __set_tSimple(const Simple& val);

  void __set_iId(const int32_t val);

  bool operator == (const SimpleWrap & rhs) const
  {
    if (!(tSimple == rhs.tSimple))
      return false;
    if (!(iId == rhs.iId))
      return false;
    return true;
  }
  bool operator != (const SimpleWrap &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SimpleWrap & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(SimpleWrap &a, SimpleWrap &b);

std::ostream& operator<<(std::ostream& out, const SimpleWrap& obj);



#endif
