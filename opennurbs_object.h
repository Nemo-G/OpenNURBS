/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2012 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////
*/

////////////////////////////////////////////////////////////////
//
//   virtual base class for all openNURBS objects
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_OBJECT_INC_)
#define OPENNURBS_OBJECT_INC_


class ON_ClassId; // used for runtime class identification

////////////////////////////////////////////////////////////////
//
//
/*
 Description:
   OpenNURBS classes derived from ON_Object use ON_ClassId to
   store run-time type information  
   
   The ON_OBJECT_DECLARE and ON_OBJECT_IMPLEMENT macros generate
   the code that creates and initializes the ON_ClassId for each
   class.

   The ON_Object::IsKindOf() and ON_Object::Cast() functions
   use this run-time type information.
*/
class ON_CLASS ON_ClassId
{
public:

  // Description:
  //   This constructor is called to initialize each class id.
  //   The call is generated by the ON_OBJECT_IMPLEMENT macro.
  //
  // Parameters:
  //   sClassName - [in] name of the class (like ON_Geometry)
  //   sBaseClassName - [in] name of baseclass (like ON_Object)
  //   create - [in] function to create a new object(like CreateNewON_Geometry())
  //   sUUID - [in] UUID in registry format from Windows guidgen.exe
  ON_ClassId( 
          const char* sClassName,
          const char* sBaseClassName,
          class ON_Object* (*create)(),
          const char* sUUID
           );

  ~ON_ClassId();

  // Description: 
  //   Gets a class's ON_ClassId from the class's name.
  // Parameters:
  //   sClassName - [in] name of class
  // Returns: 
  //   Pointer to the class's ON_ClassId.
  // Example:
  //   const ON_ClassId* brep_id = ON_CLassId::ClassId("ON_Brep");
  static const ON_ClassId* ClassId( 
          const char* sClassName
          );

  // Description: 
  //   Gets a class's ON_ClassId from the class's uuid.
  // Parameters:
  //   class_uuid - [in] uuid for the class
  // Returns: 
  //   Pointer to the class's ON_ClassId.
  // Example:
  //   ON_UUID brep_uuid = ON_UuidFromString("60B5DBC5-E660-11d3-BFE4-0010830122F0");
  //   const ON_ClassId* brep_id = ON_ClassId::ClassId(brep_uuid);
  static const ON_ClassId* ClassId( 
          ON_UUID class_uuid
          );

  // Description:
  //   Each class derived from ON_Object has a corresponding ON_ClassId
  //   stored in a linked list and the class is marked with an integer
  //   value.  ON_ClassId::IncrementMark() increments the value used to
  //   mark new classes and returns the new marking value.
  // Returns:
  //   Value that will be used to mark all future ON_ClassIds.
  static int IncrementMark();
  static int CurrentMark();
  static const ON_ClassId* LastClassId();

  // Description:
  //   Each class derived from ON_Object has a corresponding
  //   ON_ClassId stored in a linked list.  If a class definition
  //   is going to disappear (which happens when the derived object
  //   definition is in a DLL that uses openNURBS as a DLL and the 
  //   DLL containing the derived object's definition is unloaded),
  //   then the class's ON_ClassId needs to be removed from the class 
  //   list.  ON_ClassId::Purge( mark ) removes all ON_ClassIds with a
  //   a prescribed mark and returns the number of classes that
  //   were purged.  
  // Parameters:
  //   mark - [in] All ON_ClassIds with this mark will be purged.
  // Returns:
  //   Number of classes that were purged.
  // Example:
  //   // Call ON_ClassId::IncrementMark() BEFORE loading MY.DLL.
  //   int my_dll_classid_mark = ON_ClassId::IncrementMark();
  //   load MY.DLL with classes derived from ON_Object
  //   ...
  //   // Call ON_ClassId::Purge() BEFORE unloading MY.DLL.
  //   ON_ClassId::Purge( my_dll_classid_mark );
  //   unload MY.DLL
  static int Purge(int mark);
  static bool PurgeAfter(const ON_ClassId* pClassId);

  // Description:
  //   Dumps the ON_ClassId list
  // Parameters:
  //   dump - [in] destination for the text dump.
  static void Dump( 
    ON_TextLog& dump
    );

  // Returns:
  //   class name
  const char* ClassName() const;

  // Returns:
  //   base class name
  const char* BaseClassName() const;
  
  // Returns:
  //   base class id
  const ON_ClassId* BaseClass() const;

  // Description:
  //   Determine if the class associated with this ON_ClassId
  //   is derived from another class.
  // Parameters:
  //   potential_parent - [in] Class to test as parent.
  // Returns:
  //   true if this is derived from potential_parent.
	bool IsDerivedFrom( 
    const ON_ClassId* potential_parent
    ) const;

  // Descrption:
  //   Use the default constructor to create an instance of the
  //   class on the heap.
  // Returns:
  //   Null or a pointer to an instance of the class created
  //   using new and the class's default constructor.
  ON_Object* Create() const;

  // Returns:
  //   class uuid
  ON_UUID Uuid() const;

  /*
  Description:
    Opennurbs classes have a mark value of 0.  Core Rhino
    classes have a mark value of 1.  Rhino plug-in classes
    have a mark value of > 1.
  Returns:
    Class mark value
  */
  int Mark() const;

  unsigned int ClassIdVersion() const;

private:
  static ON_ClassId* m_p0;     // first id in the linked list of class ids
  static ON_ClassId* m_p1;     // last id in the linked list of class ids
  static int m_mark0;  // current mark value
  ON_ClassId* m_pNext;         // next in the linked list of class ids
  const ON_ClassId* m_pBaseClassId;  // base class id
  char m_sClassName[80];              
  char m_sBaseClassName[80];
  // m_create points to a function that calls the default constuctor.
  // m_create() is used to create classes from uuids when reading files.
  ON_Object* (*m_create)(); 
  ON_UUID m_uuid;
  int m_mark; // bit 0x80000000 is used to indicate new extensions

private:
  // There are no implementaions of the default constructor, copy constructor
  // or operator=() to prohibit use.
  ON_ClassId();
  ON_ClassId( const ON_ClassId&);
  ON_ClassId& operator=( const ON_ClassId&);

private:
  void ConstructorHelper( 
          const char* sClassName, 
          const char* sBaseClassName, 
          const char* sUUID
          );

  // The m_f[] pointers provide a way add a "virtual" function to
  // a class derived from ON_Object without breaking the SDK.
  // At each SDK breaking relase, any functions that use this
  // mechanism are made into C++ virtual functions on the appropriate
  // classes. Currently, none of these are in use.
  unsigned int m_class_id_version; 
  void* m_f1;
  void* m_f2;
  void* m_f3;
  void* m_f4;
  void* m_f5;
  void* m_f6;
  void* m_f7;
  void* m_f8;
};

/*
Description:
  ON_CLASS_RTTI is a macro to get the class's run-time type
  information from class name.
Example:
        // Get the ON_Brep class's run-time type information.
        const ON_ClassId& brep_rtti = ON_CLASS_RTTI(ON_Brep);
*/
#define ON_CLASS_RTTI( cls ) cls::m_##cls##_class_rtti

/*
Description:
  ON_CLASS_ID is a macro to get the class's uuid from
  a class name.
Example:
        // Get the class id for ON_Brep.
        ON_UUID brep_class_id = ON_CLASS_ID(ON_Brep);
*/
#define ON_CLASS_ID( cls ) ON_CLASS_RTTI( cls ).Uuid()

/*
Description:
  Expert user function to get the value of ON_ClassId::m_uuid
  of the last instance of ON_ClassId to call ON_ClassId::Create().
  This function was created to support Rhino's .NET SDK.
  This function returns the value of a static id in
  opennurbs_object.cpp and is NOT thread safe.
Returns:
  Value of ON_ClassId::m_uuid of the instance of ON_ClassId that
  most recently called ON_ClassId::Create().
*/
ON_DECL
ON_UUID ON_GetMostRecentClassIdCreateUuid();


#define ON_OBJECT_DECLARE_VIRTUAL
#define ON_OBJECT_DECLARE_OVERRIDE override

/*
All classes derived from ON_Object must have the declaration macro
  ON_OBJECT_DECLARE( <classname> );
as the first line in their class definition, must have a robust
operator=(), should have a robust copy constructory, and must
have exactly one of the following implementation macros in 
a .cpp file.
  Classes with a pure virtual function:
    ON_VIRTUAL_OBJECT_IMPLEMENT( <classname>, <basclassname>, <classuuid> )
  Classes with an operator= and copy constructor.
    ON_OBJECT_IMPLEMENT( <classname>, <basclassname>, <classuuid> )
  Classes with an operator=, but no copy constructor.
    ON_OBJECT_IMPLEMENT_NO_COPYCTOR( <classname>, <basclassname>, <classuuid> )
*/
#define ON_OBJECT_DECLARE( cls )                          \
  protected:                                              \
    static void* m_s_##cls##_ptr;                         \
                                                          \
  public:                                                 \
    /* OpenNURBS class run-time type information */       \
    static const ON_ClassId m_##cls##_class_rtti;         \
                                                          \
    /*OpenNURBS platfrom independent dynamic cast*/       \
    static cls * Cast( ON_Object* );                      \
                                                          \
    /*OpenNURBS platfrom independent dynamic cast*/       \
    static const cls * Cast( const ON_Object* );          \
                                                          \
    /*Returns: OpenNURBS run-time type information.*/     \
    ON_OBJECT_DECLARE_VIRTUAL const ON_ClassId* ClassId() const ON_OBJECT_DECLARE_OVERRIDE; \
                                                          \
  public:                                                 \
    /*Description:                                     */ \
    /*  Uses a virtual function to create a deep copy. */ \
    /*Returns:                                         */ \
    /*  null or a deep copy with type this->ClassId(). */ \
    /*See Also:                                        */ \
    /*  ON_Curve::DuplicateCurve()                     */ \
    /*  ON_Surface::DuplicateSurface()                 */ \
    /*  CRhinoObject::DuplicateRhinoObject()           */ \
    cls * Duplicate() const;                              \
                                                          \
    /*Description:                                     */ \
    /*  Uses operator= to copy src to this.            */ \
    /*Returns:                                         */ \
    /*  True if successful.                            */ \
    /*  False if src is null or an incompatible type.  */ \
    ON_OBJECT_DECLARE_VIRTUAL bool CopyFrom(const ON_Object*) ON_OBJECT_DECLARE_OVERRIDE; \
                                                          \
  private:                                                \
    /* Duplicate() uses this virtual helper function. */  \
    ON_OBJECT_DECLARE_VIRTUAL ON_Object* Internal_DeepCopy() const ON_OBJECT_DECLARE_OVERRIDE \
                                                          

/*
Classes derived from ON_Object that are pure virtual classes,
or do not have a valid default constructor, valid operator new
or valid operator= must use ON_VIRTUAL_OBJECT_IMPLEMENT in their
implementation. Classes implemented with ON_VIRTUAL_OBJECT_IMPLEMENT
cannot be serialized using ON_BinaryArchive::ReadObject()/WriteObject()
or duplicated using ON_Object::Duplicate().
The Cast() and ClassId() members work on classes implemented with
ON_VIRTUAL_OBJECT_IMPLEMENT, ON_OBJECT_IMPLEMENT or
ON_OBJECT_IMPLEMENT_NO_COPYCTOR
*/
#define ON_VIRTUAL_OBJECT_IMPLEMENT( cls, basecls, uuid ) \
  void* cls::m_s_##cls##_ptr = nullptr; \
  const ON_ClassId cls::m_##cls##_class_rtti(#cls,#basecls,0,uuid);\
  cls * cls::Cast( ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast< cls *>(p):nullptr;} \
  const cls * cls::Cast( const ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast<const cls *>(p):nullptr;} \
  const ON_ClassId* cls::ClassId() const {return &cls::m_##cls##_class_rtti;} \
  bool cls::CopyFrom(const ON_Object*) {return false;} \
  cls * cls::Duplicate() const {return static_cast< cls *>(this->Internal_DeepCopy());} \
  ON_Object* cls::Internal_DeepCopy() const {return nullptr;}

/*
Classes derived from ON_Object that have a valid default constructor,
valid copy constructor, operator new and operator= can use
ON_OBJECT_IMPLEMENT in their implementation.  Classes implemented
with ON_OBJECT_IMPLEMENT can be created from their run-time type
information id and their Duplicate() function will use the class's
copy constructor to create a deep copy.
*/
#define ON_OBJECT_IMPLEMENT( cls, basecls, uuid ) \
  void* cls::m_s_##cls##_ptr = nullptr; \
  static ON_Object* CreateNew##cls() {return new cls();} \
  const ON_ClassId cls::m_##cls##_class_rtti(#cls,#basecls,CreateNew##cls,uuid);\
  cls * cls::Cast( ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast< cls *>(p):nullptr;} \
  const cls * cls::Cast( const ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast<const cls *>(p):nullptr;} \
  const ON_ClassId* cls::ClassId() const {return &cls::m_##cls##_class_rtti;} \
  bool cls::CopyFrom( const ON_Object* src){const cls * s=cls::Cast(src); if ( nullptr != this && nullptr != s) {*this = *s; return true;}return false;} \
  cls * cls::Duplicate() const {return static_cast< cls *>(this->Internal_DeepCopy());} \
  ON_Object* cls::Internal_DeepCopy() const {return new cls (*this);}

/*
Classes derived from ON_Object that have a valid default constructor,
operator new and operator=, but do not have a valid copy constructor,
can use ON_OBJECT_IMPLEMENT_NO_COPYCTOR in their implementation.
Classes implemented with ON_OBJECT_IMPLEMENT_NO_COPYCTOR can be created 
from their run-time type information id and their Duplicate() function
will use the class's default constructor and operator= to create a
deep copy.
*/
#define ON_OBJECT_IMPLEMENT_NO_COPYCTOR( cls, basecls, uuid ) \
  void* cls::m_s_##cls##_ptr = nullptr; \
  static ON_Object* CreateNew##cls() {return new cls();} \
  const ON_ClassId cls::m_##cls##_class_rtti(#cls,#basecls,CreateNew##cls,uuid);\
  cls * cls::Cast( ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast< cls *>(p):nullptr;} \
  const cls * cls::Cast( const ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast<const cls *>(p):nullptr;} \
  const ON_ClassId* cls::ClassId() const {return &cls::m_##cls##_class_rtti;} \
  bool cls::CopyFrom( const ON_Object* src){const cls* s=cls::Cast(src); if ( 0 != this && 0 != s) {*this = *s; return true;}return false;} \
  cls * cls::Duplicate() const {return static_cast< cls *>(this->Internal_DeepCopy());} \
  ON_Object* cls::Internal_DeepCopy() const { cls* p = new cls();if (p) {*p = *this; return p;}return nullptr;}

/*
Classes derived from ON_Object that have a valid default constructor,
operator new and operator=, but do not have a valid copy constructor or assignment operator,
can use ON_OBJECT_IMPLEMENT_NO_COPY in their implementation.
Classes implemented with ON_OBJECT_IMPLEMENT_NO_COPY can be created
from their run-time type information id and their Duplicate() function
will silently return a nullptr.  CopyFrom will return false.
*/
#define ON_OBJECT_IMPLEMENT_NO_COPY( cls, basecls, uuid ) \
  void* cls::m_s_##cls##_ptr = nullptr; \
  static ON_Object* CreateNew##cls() {return new cls();} \
  const ON_ClassId cls::m_##cls##_class_rtti(#cls,#basecls,CreateNew##cls,uuid);\
  cls * cls::Cast( ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast< cls *>(p):nullptr;} \
  const cls * cls::Cast( const ON_Object* p) {return(p&&p->IsKindOf(&cls::m_##cls##_class_rtti))?static_cast<const cls *>(p):nullptr;} \
  const ON_ClassId* cls::ClassId() const {return &cls::m_##cls##_class_rtti;} \
  bool cls::CopyFrom( const ON_Object* src){return false;} \
  cls * cls::Duplicate() const {return nullptr;} \
  ON_Object* cls::Internal_DeepCopy() const { return nullptr;}

#define ON__SET__THIS__PTR(ptr) if (ptr) *((void**)this) = ptr

class ON_CLASS ON_UserString
{
public:
  ON_UserString();
  ~ON_UserString();
  ON_wString m_key;
  ON_wString m_string_value;

  void Dump(ON_TextLog& text_log) const;
  bool Write(ON_BinaryArchive&) const;
  bool Read(ON_BinaryArchive&);
};

#if defined(ON_DLL_TEMPLATE)
ON_DLL_TEMPLATE template class ON_CLASS ON_ClassArray<ON_UserString>;
#endif

/*
Description:
  When ON_Object::IsValid() fails and returns false, ON_IsNotValid()
  is called.  This way, a developer can put a breakpoint in
  ON_IsNotValid() and stop execution at the exact place IsValid()
  fails.
Returns:
  false;
*/
ON_DECL
bool ON_IsNotValid();

////////////////////////////////////////////////////////////////

// Description:
//   Pure virtual base class for all classes that must provide
//   runtime class id or support object level 3DM serialization
class ON_CLASS ON_Object
{
#undef ON_OBJECT_DECLARE_VIRTUAL
#undef ON_OBJECT_DECLARE_OVERRIDE
#define ON_OBJECT_DECLARE_VIRTUAL virtual
#define ON_OBJECT_DECLARE_OVERRIDE
  // This is the base class
  ON_OBJECT_DECLARE(ON_Object);
  // Every other use of ON_OBJECT_DECLARE() is in derived class
#undef ON_OBJECT_DECLARE_VIRTUAL
#undef ON_OBJECT_DECLARE_OVERRIDE
#define ON_OBJECT_DECLARE_VIRTUAL
#define ON_OBJECT_DECLARE_OVERRIDE override

public:
  ON_Object() ON_NOEXCEPT;
  virtual ~ON_Object();
  ON_Object( const ON_Object& );
  ON_Object& operator=( const ON_Object& );

#if defined(ON_HAS_RVALUEREF)
  // rvalue copy constructor
  ON_Object( ON_Object&& ) ON_NOEXCEPT;

  // The rvalue assignment operator calls this->PurgeUserData()
  // which calls unknown destructors that could throw exceptions.
  ON_Object& operator=( ON_Object&& );
#endif

public:




  /*
  Description:
    Sets m_user_data_list = 0.
  */
  void EmergencyDestroy();

  /*
  Description:
    The MemoryRelocate() function is called when an 
    object's location in memory is changed.  For 
    example, if an object resides in a chunk of 
    memory that is grown by calling a realloc
    that has to allocate a new chunk and
    copy the contents of the old chunk to the
    new chunk, then the location of the object's
    memory changes.  In practice this happens when
    classes derived from ON_Object are stored
    in dynamic arrays, like the default implementation
    of ON_ObjectArray<>'s that use realloc to grow
    the dynamic array.
  */
  virtual 
  void MemoryRelocate();

  /*
  Description:
    Low level tool to test if an object is derived
    from a specified class.
  Parameters:
    pClassId - [in] use classname::ClassId()
  Returns:
    true if the instantiated object is derived from the
    class whose id is passed as the argument.
  Example:

          ON_Object* p = ....;
          if ( p->IsKindOf( ON_NurbsCurve::ClassId() ) )
          {
            it's a NURBS curve
          }

  Remarks:
    The primary reason for IsKindOf() is to support the
    static Cast() members declared in the ON_OBJECT_DECLARE
    macro.  If we determine that dynamic_cast is properly 
    supported and implemented by all supported compilers, 
    then IsKindOf() may dissappear.  If an application needs
    to determine if a pointer points to a class derived from
    ON_SomeClassName, then call 
    ON_SomeClassName::Cast(mystery pointer) and check for 
    a non-null return.
  */
  bool IsKindOf( 
        const ON_ClassId* pClassId
        ) const;

  /*
  Description:
    Tests an object to see if its data members are correctly
    initialized.
  Parameters:
    text_log - [in] if the object is not valid and text_log
        is not nullptr, then a brief englis description of the
        reason the object is not valid is appened to the log.
        The information appended to text_log is suitable for 
        low-level debugging purposes by programmers and is 
        not intended to be useful as a high level user 
        interface tool.
  Returns:
    @untitled table
    true     object is valid
    false    object is invalid, uninitialized, etc.
  */
  virtual bool IsValid( class ON_TextLog* text_log = nullptr ) const;

    /*
  Description:
    Check for corrupt data values that are likely to cause crashes.
  Parameters:
    bRepair - [in]
      If true, const_cast<> will be used to change the corrupt data
      so that crashes are less likely.
    bSilentError - [in]
      If true, ON_ERROR will not be called when corruption is detected.
    text_log - [out]
      If text_log is not null, then a description of corruption 
      is printed using text_log.
  Remarks:
    Ideally, IsCorrupt() would be a virtual function on ON_Object,
    but doing that at this point would break the public SDK.
  */
  bool IsCorrupt(
    bool bRepair,
    bool bSilentError,
    class ON_TextLog* text_log
  ) const;

  /*
  Description:
    Creates a text dump of the object.
  Remarks:
    Dump() is intended for debugging and is not suitable
    for creating high quality text descriptions of an
    object.

    The default implementations of this virtual function 
    prints the class's name.
  */
  virtual
  void Dump( ON_TextLog& ) const;

  /*
  Returns:
    An estimate of the amount of memory the class uses in bytes.
  */
  virtual
  unsigned int SizeOf() const;

  /*
  Description:
    Returns a CRC calculated from the information that defines
    the object.  This CRC can be used as a quick way to see
    if two objects are not identical.
  Parameters:
    current_remainder - [in];
  Returns:
    CRC of the information the defines the object.
  */
  virtual
  ON__UINT32 DataCRC(ON__UINT32 current_remainder) const;

  /*
  Description:
    Low level archive writing tool used by ON_BinaryArchive::WriteObject().
  Parameters:
    binary_archive - archive to write to
  Returns:
    Returns true if the write is successful.
  Remarks:
    Use ON_BinaryArchive::WriteObject() to write objects.
    This Write() function should just write the specific definition of
    this object.  It should not write and any chunk typecode or length
    information.  

    The default implementation of this virtual function returns 
    false and does nothing.
  */
  virtual
  bool Write(
         ON_BinaryArchive& binary_archive
       ) const;

  /*
  Description:
    Low level archive writing tool used by ON_BinaryArchive::ReadObject().
  Parameters:
    binary_archive - archive to read from
  Returns:
    Returns true if the read is successful.
  Remarks:
    Use ON_BinaryArchive::ReadObject() to read objects.
    This Read() function should read the objects definition back into
    its data members.

    The default implementation of this virtual function returns 
    false and does nothing.
  */
  virtual
  bool Read(
         ON_BinaryArchive& binary_archive
       );

  /*
  Description:
    Useful for switch statements that need to differentiate
    between basic object types like points, curves, surfaces,
    and so on.

  Returns: 
    ON::object_type enum value.

  Remarks:
    The default implementation of this virtual function returns
    ON::unknown_object_type
  */
  virtual
  ON::object_type ObjectType() const;



  /*
  Description:
    All objects in an opennurbs model have an id
    ( ON_Layer.m_layer_id, ON_Font.m_font_id, 
      ON_Material.m_material_id, ON_3dmObjectAttributes.m_uuid
      ).
  Returns:
    The id used to identify the object in the openurbs model.
  */
  virtual
  ON_UUID ModelObjectId() const;

  //////////////////////////////////////////////////////////////////
  //
  // BEGIN: User string support
  //

  /*
  Description:
    Attach a user string to the object.  This information will
    perisist through copy construction, operator=, and file IO.
  Parameters:
    key - [in] id used to retrieve this string.
    string_value - [in] 
      If nullptr, the string with this id will be removed.
  Returns:
    True if successful.
  */
  bool SetUserString( 
    const wchar_t* key, 
    const wchar_t* string_value 
    );

  /*
  Description:
    Append entries to the user string list
  Parameters:
    count - [in]
      number of element in us[] array
    user_strings - [in]
      entries to append.
    bReplace - [in]
      If bReplace is true, then existing entries with the same key are
      updated with the new entry's value.  If bReplace is false, then
      existing entries are not updated.
  Returns:
    Number of entries added, deleted, or modified.
  */
  int SetUserStrings( int count, const ON_UserString* user_strings, bool bReplace );

  /*
  Description:
    Get user string from the object.
  Parameters:
    key - [in] id used to retrieve the string.
    string_value - [out]
  Returns:
    True if a string with id was found.
  */
  bool GetUserString( 
    const wchar_t* key, 
    ON_wString& string_value 
    ) const;

  /*
  Description:
    Get a list of all user strings on the object.
  Parameters:
    user_strings - [out]
      user strings are appended to this list.
  Returns:
    Number of elements appended to the user_strings list.
  */
  int GetUserStrings( 
    ON_ClassArray<ON_UserString>& user_strings 
    ) const;

  /*
  Description:
    Get a list of all user string keys on the object.
  Parameters:
    user_string_keys - [out]
      user string keys are appended to this list.
  Returns:
    Number of elements appended to the user_strings list.
  */
  int GetUserStringKeys( 
    ON_ClassArray<ON_wString>& user_string_keys 
    ) const;

  /*
  Returns:
    Number of user strings on the object.
  */
  int UserStringCount() const;

  //
  // END: User string support
  //
  //////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
  //
  // User data provides a standard way for extra information to
  // be attached to any class derived from ON_Object.  The attached
  // information can persist and be transformed.  If you use user
  // data, please carefully read all the comments from here to the
  // end of the file.
  //

  /*
  Description:
    Attach user data to an object.
  Parameters:
    pUserData - [in] user data to attach to object.
        The ON_UserData pointer passed to AttachUserData()
        must be created with new.  
  Returns:
    If true is returned, then ON_Object will delete the user
    data when appropriate.  If false is returned, then data 
    could not be attached and caller must delete.  
  Remarks:
    AttachUserData() will fail if the user data's m_userdata_uuid
    field is nil or not unique.
  */
  bool AttachUserData( 
          class ON_UserData* pUserData 
          );

  /*
  Description:
    Remove user data from an object.
  Parameters:
    pUserData - [in] user data to attach to object.
        The ON_UserData pointer passed to DetachUserData()
        must have been previously attached using 
        AttachUserData().  
  Returns:
    If true is returned, then the user data was
    attached to this object and it was detached.  If false
    is returned, then the user data was not attached to this
    object to begin with.  In all cases, you can be assured
    that the user data is no longer attached to "this".
  Remarks:
    Call delete pUserData if you want to destroy the user data.
  */
  bool DetachUserData(
          class ON_UserData* pUserData 
          );


  /*
  Description:
    Get a pointer to user data.
  Parameters:
    userdata_uuid - [in] value of the user data's 
       m_userdata_uuid field.
  Remarks:
    The returned user data is still attached to the object.
    Deleting the returned user data will automatically remove
    the user data from the object.
  */
  class ON_UserData* GetUserData( 
          const ON_UUID& userdata_uuid
          ) const;

  /*
  Description:
    PurgeUserData() removes all user data from object.  
  Remarks:
    Use delete GetUserData(...) to destroy a single piece
    of user data.
  */
  void PurgeUserData();

  /*
  Description:
    User data is stored as a linked list of ON_UserData
    classes.  FirstUserData gets the first item in the
    linked list.  This is the most recent item attached
    using AttachUserData().
  Remark:
    To iterate through all the user data on an object, 
    call FirstUserData() and then use ON_UserData::Next()
    to traverse the list.
  */
  class ON_UserData* FirstUserData() const;

  /*
  Description:
    Objects derived from ON_Geometry must call
    TransformUserData() in their Transform() member function.
  Parameters:
    xform - [in] transformation to apply to user data
  */
  void TransformUserData( 
    const class ON_Xform& xform
    );

  /*
  Description:
    When a userdata item is copied or moved from a source object to
    a destination object, the ON_Object::UserDataConflictResolution
    enum values specify how conficts are resolved.
  Remark:
    A userdata item "conflict" occurs when both the destination
    and source object have a user data item with the same
    value of ON_UserData::m_userdata_uuid.
  */
  enum class UserDataConflictResolution : unsigned char
  {
    destination_object = 0,       // use destination item
    source_object = 1,            // use source item
    source_copycount_gt = 2,      // use source item if source copycount > destination copy count
    source_copycount_ge = 3,      // use source item if source copycount >= destination copy count
    destination_copycount_gt = 4, // use destination item if destination copycount > source copy count
    destination_copycount_ge = 5, // use destination item if destination copycount >= source copy count
    delete_item = 6               // delete item from the destination object
  };

  /*
  Description:
    Expert user tool that copies user data items with positive values of
    ON_UserData.m_userdata_copycount from source_object to "this.
  Parameters:
    source_object - [in] 
      source of user data to copy
    source_userdata_item_id - [in]
      If source_userdata_item_id  is not nil, then only the user data item
      with a matching ON_UserData.m_userdata_uuid value will be copied.
    userdata_conflict_resolution - [in]
      method to resolve userdata item conficts.
  Remarks:
    Generally speaking you don't need to use CopyUserData().
    Simply rely on ON_Object::operator=() or the copy constructor
    to do the right thing.
  Returns:
    Number of user data items that were copied.
  */
  unsigned int CopyUserData( 
    const ON_Object& source_object,
    ON_UUID source_userdata_item_id,
    ON_Object::UserDataConflictResolution userdata_conflict_resolution
    );

  /*
  Description:
    Expert user tool that moves user data items from source_object to "this.
  Parameters:
    source_object - [in] 
      source of user data to copy
    source_userdata_item_id - [in]
      If source_userdata_item_id  is not nil, then only the user data item
      with a matching ON_UserData.m_userdata_uuid value will be moved.
    userdata_conflict_resolution - [in]
      method to resolve userdata item conficts.
    bDeleteAllSourceItems - [in]
      If bDeleteAllSourceItems is true, then any userdata items
      that are not copied from source_object are deleted.
  Remarks:
    Generally speaking you don't need to use MoveUserData().
    Simply rely on ON_Object::operator=() or the copy constructor
    to do the right thing.
  Returns:
    Number of user data items that were moved.
  */
  unsigned int MoveUserData( 
    ON_Object& source_object,
    ON_UUID source_userdata_item_id,
    ON_Object::UserDataConflictResolution userdata_conflict_resolution,
    bool bDeleteAllSourceItems
    );

  /*
  Description:
    Calls CopyUserData(source_object,ON_Object::UserDataConflictResolution::source_object).
  Parameters:
    source_object - [in] 
  */
  void CopyUserData( 
    const ON_Object& source_object 
    );

  /*
  Description:
    Calls MoveUserData(source_object,ON_Object::UserDataConflictResolution::source_object,true).
  Parameters:
    source_object - [in] 
  */
  void MoveUserData( 
    ON_Object& source_object 
    );
  
  /*
  Description:
    Uses the destination_manifest to update references to other components.
    This is typically done when a component's references came from a "source" 
    context and are being updated to the "destination" context. For example,
    inserting one model into another when index, id, and name conflicts 
    need to be resolved at the time of insertion.
  Parameters:
    source_manifest - [in]
      A manifest of the source context with indices and ids 
      corresponding to the current component references.
      If this manifest is not available, pass ON_ComponentManifest::Empty.
    destination_manifest - [in]
      A manifest of the destination context with indices and ids 
      corresponding to the desired component references.
      If this manifest is not available, pass ON_ComponentManifest::Empty.
    manifest_map - [in]
      A map from the source (current) referenced component index/id values 
      to the destination (desired) component index/id values.
  Returns:
    True if successful.
    False indicates a referenced component was not found in the manifest
    and the reference was changed to a default value.
  Example:
    If this object is an ON_Layer, the line pattern and render material references
    are updated. 
    If this object is an ON_DimStyle, the text style reference is updated. 
    If this object is an ON_3dmObjectAttributes, the layer,
    material, line pattern, and group references are updated. 
  */
  virtual bool UpdateReferencedComponents(
    const class ON_ComponentManifest& source_manifest,
    const class ON_ComponentManifest& destination_manifest,
    const class ON_ManifestMap& manifest_map
    );
  
  /////////////////////////////////////////////////////////////////
  //
  // Component status interface
  //
  //  Currently implemnented on ON_SubD and ON_Brep
  //

  /*
  Description:
    Set all active level component states to ON_ComponentStatus::NoneSet.
  Returns:
    Number of components where a state setting chanaged.
  */
  unsigned int ClearAllComponentStates() const;

  /*
  Description:
    Clear the specified states on every component.
  Parameters:
    states_to_clear - [in]
      States to clear.
  Returns:
    Number of components where a state setting chanaged.
  */
  virtual
  unsigned int ClearComponentStates(
    ON_ComponentStatus states_to_clear
    ) const;

  /*
  Parameters:
    states_filter - [in]

    bAllEqualStates - [in]
      If a state is set in states_filter, all active level components
      with the same state set will be included in the 
      components_with_set_states[] array.  

      If bAllEqualStates is true, then ON_ComponentStatus::AllEqualStates()
      is used to test for inclusion.  

      If bAllEqualStates is false, then ON_ComponentStatus::SomeEqualStates()
      is used to test for inclusion.

    components_with_set_states - [out]
  Returns:
    Number of returned components.
  */
  virtual
  unsigned int GetComponentsWithSetStates(
    ON_ComponentStatus states_filter,
    bool bAllEqualStates,
    ON_SimpleArray< ON_COMPONENT_INDEX >& components
    ) const;
  
  /*
  Description:
    Set states on an individual component.
  Parameters:
    component_index - [in]
      The states will be set on this component.
    states_to_set - [in]
      If a state is set in the states_to_set parameter, the same
      state will be set on the component.      
  Returns:
    0: no state settings changed on the component.
    1: some state setting changed on the component.
  */
  virtual
  unsigned int SetComponentStates(
    ON_COMPONENT_INDEX component_index,
    ON_ComponentStatus states_to_set
    ) const;

  /*
  Description:
    Clear states on an individual component.
  Parameters:
    component_index - [in]
      The states will be cleared on this component.
    states_to_clear - [in]
      If a state is set in the states_to_clear parameter, the same
      state will be cleared on the component.      
  Returns:
    0: no state settings changed on the component.
    1: some state setting changed on the component.
  */
  virtual
  unsigned int ClearComponentStates(
    ON_COMPONENT_INDEX component_index,
    ON_ComponentStatus states_to_clear
    ) const;
  
  /*
  Description:
    Copy status settings to an individual component.
  Parameters:
    component_index - [in]
      The states will be copied to this component.
    status_to_copy - [in]
  Returns:
    0: no state settings changed on the component.
    1: some state setting changed on the component.
  */
  virtual
  unsigned int SetComponentStatus(
    ON_COMPONENT_INDEX component_index,
    ON_ComponentStatus status_to_copy
    ) const;

  /*
  Description:
    Call whenever a component status setting is modifed 
    by directly changing it on a component in a way that
    will result in any saved information about the parent
    object's aggretate component status becoming invalid.

  Returns:
    Aggregate information about the object's component states.
  */
  virtual
  ON_AggregateComponentStatus AggregateComponentStatus() const;

  /*
  Description:
    Call whenever a component status setting is modifed 
    by directly changing it on a component in a way that
    will result in any saved information about the parent
    object's aggretate component status becoming invalid.

  Remarks:
    The implementations of this function are nearly instant.
    and this function may be called as frequently as needed. 
    The next time AggregateComponentStatus()
    is called the information used to return the value
    will be updated.
  */
  virtual
  void MarkAggregateComponentStatusAsNotCurrent() const;

  /*
  Description:
    Delete the portions of the object identified in ci_list[].
  Parameters:
    ci_list - [in]
      List of components to delete.
    ci_list_count - [in]
      Number of elements in the ci_list[] array.
  Returns:
    True: succesful
    False: failure - no changes.
  */
  virtual
  bool DeleteComponents(
    const ON_COMPONENT_INDEX* ci_list,
    size_t ci_count
    );

  /////////////////////////////////////////////////////////////////
  //
  // Expert interface
  //

  /*
  Description:
    Expert user function.  If you are using openNURBS in its
    default configuration to read and write 3dm archives,
    you never need to call this function.
    Many objects employ lazy creation of (runtime) caches
    that save information to help speed geometric calculations.
    This function will destroy all runtime information.
  Parameters:
    bDelete - [in] if true, any cached information is properly
                   deleted.  If false, any cached information
                   is simply discarded.  This is useful when
                   the cached information may be in alternate
                   memory pools that are managed in nonstandard
                   ways.
  */
  virtual void DestroyRuntimeCache( bool bDelete = true );

private:
  class ON_UserData* m_userdata_list;
  class ON_UserData* TransferUserDataItem(
    const class ON_UserData* source_ud_copy_this,
    class ON_UserData* source_ud_move_this,
    bool bPerformConflictCheck,
    ON_Object::UserDataConflictResolution userdata_conflict_resolution
    );
};

#endif

