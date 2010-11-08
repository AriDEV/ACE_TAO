// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v1.8.2
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

#include "MultiTopic_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_Sender_Impl
{
  //============================================================
    // pulse_Generator
    //============================================================
    pulse_Generator::pulse_Generator (Sender_exec_i &callback)
      : pulse_callback_ (callback)
    {
    }

    pulse_Generator::~pulse_Generator ()
    {
    }

    int
    pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
    {
      this->pulse_callback_.tick ();
      return 0;
    }


   /**
   * Component Executor Implementation Class: Sender_exec_i
   */
  Sender_exec_i::Sender_exec_i (void)
    : rate_ (0),
      max_x_ (0),
      max_y_ (0),
      max_size_ (0),
      resize_shape_ (false),
      ShapeNr (0)
  {
    this->ticker_ = new pulse_Generator (*this);
    square_ = new ShapeType;
    triangle_ = new ShapeType;
    cirque_ = new ShapeType;
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.
  ACE_Reactor*
  Sender_exec_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object = 
      this->ciao_context_->get_CCM_object();
    if (! ::CORBA::is_nil (ccm_object.in ())) 
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (! ::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }
    if (reactor == 0)
      {
        throw ::CORBA::INTERNAL ();
      }
    return reactor;
  }

  void
  Sender_exec_i::tick ()
    {
      try
        {
          if (this->ShapeNr == 0)
            {
              this->ShapeNr = 1;
              this->writer_sq_->write_one (*square_,
                                           this->instance_handle_sq_);
              ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("UPDATED Shape_info Square for <%C> %u:%u:%u\n"),
                  square_->color.in (),
                  square_->x,
                  square_->y,
                  square_->shapesize));
            }
          else if (this->ShapeNr == 1)
            {
              this->ShapeNr = 2;
              this->writer_tr_->write_one (*triangle_,
                                            this->instance_handle_tr_);
              ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("UPDATED Shape_info Triangle for <%C> %u:%u:%u\n"),
                  triangle_->color.in (),
                  triangle_->x,
                  triangle_->y,
                  triangle_->shapesize));
            }
          else if (this->ShapeNr == 2)
            {
              this->ShapeNr = 0;
              this->writer_cq_->write_one (*cirque_,
                                           this->instance_handle_cq_);
              ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("UPDATED Shape_info Circle for <%C> %u:%u:%u\n"),
                  cirque_->color.in (),
                  cirque_->x,
                  cirque_->y,
                  cirque_->shapesize));
             }
        }
      catch (const CCM_DDS::NonExistent& )
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Shape_info for <%C> not updated: <%C> didn't exist.\n"),
                      square_->color.in (), square_->color.in ()));
        }
      catch (const CCM_DDS::InternalError& )
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Internal Error while updating Shape_info for <%C>.\n"),
                      square_->color.in ()));
        }
    }

  // Component attributes and port operations.

  ::CORBA::ULong
  Sender_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Sender_exec_i::rate (
    const ::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::UShort
  Sender_exec_i::max_x (void)
  {
    return this->max_x_;
  }

  void
  Sender_exec_i::max_x (
    const ::CORBA::UShort max_x)
  {
    this->max_x_ = max_x;
  }

  ::CORBA::UShort
  Sender_exec_i::max_y (void)
  {
    return this->max_y_;
  }

  void
  Sender_exec_i::max_y (
    const ::CORBA::UShort max_y)
  {
    this->max_y_ = max_y;
  }

  ::CORBA::UShort
  Sender_exec_i::max_size (void)
  {
    return this->max_size_;
  }

  void
  Sender_exec_i::max_size (
    const ::CORBA::UShort max_size)
  {
    this->max_size_ = max_size;
  }

  ::CORBA::Boolean
  Sender_exec_i::resize_shape (void)
  {
    return this->resize_shape_;
  }

  void
  Sender_exec_i::resize_shape (
    const ::CORBA::Boolean resize_shape)
  {
    this->resize_shape_ = resize_shape;
  }


  void
  Sender_exec_i::start (void)
  {
    // calculate the interval time
    long const usec = 1000000 / this->rate_;

    if (this->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    try
      {
        this->writer_sq_->unregister_instance (*this->square_,
                                               this->instance_handle_sq_);
        this->writer_tr_->unregister_instance (*this->triangle_,
                                               this->instance_handle_tr_);
        this->writer_cq_->unregister_instance (*this->cirque_,
                                               this->instance_handle_cq_);
      }
    catch (const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Shape_info for <%C> not deleted: <%C> didn't exist.\n"),
            square_->color.in (), square_->color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Internal Error while deleting Shape_info for <%C>.\n"),
            square_->color.in ()));
      }
    delete this->ticker_;
    delete this->square_;
    delete this->triangle_;
    delete this->cirque_;
  }

  // Operations from Components::SessionComponent.
  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (" 222  Sender_exec_i::set_session_context\n")));

    this->ciao_context_ =
      ::Shapes::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
     ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("  333 Sender_exec_i::configuration_complete\n")));

    this->writer_sq_ = this->ciao_context_->get_connection_info_write_sq_data ();
    this->writer_tr_ = this->ciao_context_->get_connection_info_write_tr_data ();
    this->writer_cq_ = this->ciao_context_->get_connection_info_write_cq_data ();
 }

  void
  Sender_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG,
       ACE_TEXT ("    444Sender_exec_i::ccm_activate\n")));

    square_->x = ACE_OS::rand () % this->max_x_;
    square_->y = ACE_OS::rand () % this->max_y_;
    square_->shapesize = max_size_;
    square_->color = CORBA::string_dup("GREEN");

    triangle_->x = ACE_OS::rand () % this->max_x_;
    triangle_->y = ACE_OS::rand () % this->max_y_;
    triangle_->shapesize = max_size_;
    triangle_->color = CORBA::string_dup("GREEN");

    cirque_->x = ACE_OS::rand () % this->max_x_;
    cirque_->y = ACE_OS::rand () % this->max_y_;
    cirque_->shapesize = max_size_;
    cirque_->color = CORBA::string_dup("GREEN");



    //Register shape with dds.
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("REGISTER Shape_info for <%C> %u:%u:%u\n"),
                  square_->color.in (),
                  square_->x,
                  square_->y,
                  square_->shapesize));
/*    try
      {
        this->instance_handle_sq_ = this->writer_sq_->register_instance (*square_);
        this->instance_handle_tr_ = this->writer_tr_->register_instance (*triangle_);
        this->instance_handle_cq_ = this->writer_cq_->register_instance (*cirque_);
      }
    catch (const CCM_DDS::AlreadyCreated& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Shape_info for <%C> already created.\n"),
                    square_->color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating Shape_info for <%C>.\n"),
                      square_->color.in ()));
      }
    this->start ();
*/
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_Shape_Sender_Impl (void)
  {

    ACE_DEBUG ((LM_DEBUG,
       ACE_TEXT ("111   Sender_exec_i::create_Shape_Sender_Impl\n")));
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
