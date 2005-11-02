// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Size_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_VAR_SIZE_ARGUMENT_T_H
#define TAO_VAR_SIZE_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Var_Size_Argument_T
   *
   * @brief Template class for IN stub argument of fixed size IDL types.
   *
   */
  template<typename S, typename Insert_Policy>
  class In_Var_Size_Argument_T : public InArgument, private Insert_Policy
  {
  public:
    In_Var_Size_Argument_T (S const & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg (void) const;

  private:
    const S * x_;
  };

  /**
   * @class Inout_Var_Size_Argument_T
   *
   * @brief Template class for INOUT stub argument of fixed size IDL types.
   *
   */
  template<typename S, typename Insert_Policy>
  class Inout_Var_Size_Argument_T : public InoutArgument, private Insert_Policy
  {
  public:
    Inout_Var_Size_Argument_T (S & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S * x_;
  };

  /**
   * @class Out_Var_Size_Argument_T
   *
   * @brief Template class for OUT stub argument of fixed size IDL types.
   *
   */
  template<typename S, typename S_out, typename Insert_Policy>
  class Out_Var_Size_Argument_T : public OutArgument, private Insert_Policy
  {
  public:
    Out_Var_Size_Argument_T (S_out x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S *& x_;
  };

  /**
   * @class Ret_Var_Size_Argument_T
   *
   * @brief Template class for return stub value of fixed size IDL types.
   *
   */
  template<typename S, typename S_var, typename Insert_Policy>
  class Ret_Var_Size_Argument_T : public RetArgument, private Insert_Policy
  {
  public:
    Ret_Var_Size_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

    S * excp (void);
    S * retn (void);

  protected:
    S_var x_;
  };

  /**
   * @struct Basic_Tag
   *
   * @brief Struct for fixed size IDL type arguments id tag.
   *
   */
  struct TAO_Export Var_Size_Tag {};

  /**
   * @struct Var_Size_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of
   *  variable size IDL types.
   *
   */
  template<typename T, typename T_var, typename T_out, typename Insert_Policy>
  struct Var_Size_Arg_Traits_T
  {
    typedef T *                                         ret_type;
    typedef T const &                                   in_type;
    typedef T &                                         inout_type;
    typedef T_out                                       out_type;

    typedef In_Var_Size_Argument_T<T, Insert_Policy>          in_arg_val;
    typedef Inout_Var_Size_Argument_T<T, Insert_Policy>       inout_arg_val;
    typedef Out_Var_Size_Argument_T<T,T_out, Insert_Policy>   out_arg_val;
    typedef Ret_Var_Size_Argument_T<T,T_var, Insert_Policy>   ret_val;

    typedef Var_Size_Tag                                idl_tag;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Var_Size_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Var_Size_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Var_Size_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VAR_SIZE_ARGUMENT_T_H */
