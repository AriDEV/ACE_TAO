// $Id$

#include "CEC_ProxyPullSupplier.h"
#include "CEC_Dispatching.h"
#include "CEC_EventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_ProxyPullSupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_ProxyPullSupplier, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

TAO_CEC_ProxyPullSupplier::TAO_CEC_ProxyPullSupplier (TAO_CEC_EventChannel* ec)
  : event_channel_ (ec),
    refcount_ (1),
    wait_not_empty_ (queue_lock_)
{
  this->lock_ =
    this->event_channel_->create_supplier_lock ();

  this->default_POA_ =
    this->event_channel_->supplier_poa ();
}

TAO_CEC_ProxyPullSupplier::~TAO_CEC_ProxyPullSupplier (void)
{
  this->event_channel_->destroy_supplier_lock (this->lock_);
}

void
TAO_CEC_ProxyPullSupplier::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Save the consumer we where connected to, we need to send a
  // disconnect message to it.
  CosEventComm::PullConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return;

    consumer = this->consumer_._retn ();

    this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  this->_decr_refcnt ();

  ACE_TRY
    {
      consumer->disconnect_pull_consumer (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions, we must isolate other clients from
      // problems on this one.
    }
  ACE_ENDTRY;
}

CORBA::Boolean
TAO_CEC_ProxyPullSupplier::consumer_non_existent (
      CORBA::Boolean_out disconnected,
      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK_RETURN (0);

  disconnected = 0;
  if (this->is_connected_i () == 0)
    {
      disconnected = 1;
      return 0;
    }

#if !defined(TAO_HAS_MINIMUM_CORBA)
  return this->consumer_->_non_existent (ACE_TRY_ENV);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_ProxyPullSupplier::push (const CORBA::Any &event,
                                 CORBA::Environment &)
{
  if (this->is_connected () == 0)
    return;

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->queue_lock_);

  // Ignore errors
  (void) this->queue_.enqueue_tail (event);

  this->wait_not_empty_.signal ();
}

CORBA::Any *
TAO_CEC_ProxyPullSupplier::pull (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventComm::Disconnected))
{
  if (this->is_connected () == 0)
    ACE_THROW_RETURN (CosEventComm::Disconnected (), 0);

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, queue_lock_, 0);
  while (this->queue_.is_empty ())
    {
      this->wait_not_empty_.wait ();
    }
  CORBA::Any any;
  if (this->queue_.dequeue_head (any) != 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }
  return new CORBA::Any (any);
}

CORBA::Any *
TAO_CEC_ProxyPullSupplier::try_pull (CORBA::Boolean_out has_event,
                                     CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException,CosEventComm::Disconnected))
{
  has_event = 0;
  if (this->is_connected () == 0)
    ACE_THROW_RETURN (CosEventComm::Disconnected (), 0);

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, queue_lock_, 0);
  CORBA::Any any;
  if (this->queue_.is_empty ())
    {
      any <<= CORBA::Long(0);
      return new CORBA::Any (any);
    }

  if (this->queue_.dequeue_head (any) != 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }
  has_event = 1;
  return new CORBA::Any (any);
}

void
TAO_CEC_ProxyPullSupplier::cleanup_i (void)
{
  this->consumer_ =
    CosEventComm::PullConsumer::_nil ();
}

void
TAO_CEC_ProxyPullSupplier::deactivate (CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::ULong
TAO_CEC_ProxyPullSupplier::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_CEC_ProxyPullSupplier::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->event_channel_->destroy_proxy_pull_supplier (this);
  return 0;
}

void
TAO_CEC_ProxyPullSupplier::connect_pull_consumer (
      CosEventComm::PullConsumer_ptr pull_consumer,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected))
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      {
        if (this->event_channel_->consumer_reconnect () == 0)
          ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());

        // Re-connections are allowed....
        this->cleanup_i ();

        this->consumer_ =
          CosEventComm::PullConsumer::_duplicate (pull_consumer);

        TAO_CEC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_CEC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->reconnected (this, ACE_TRY_ENV);
          ACE_CHECK;
        }
        return;
      }

    this->consumer_ =
      CosEventComm::PullConsumer::_duplicate (pull_consumer);
  }

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_CEC_ProxyPullSupplier::disconnect_pull_supplier (
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventComm::PullConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      ACE_THROW (CORBA::BAD_INV_ORDER ());

    consumer = this->consumer_._retn ();

    this->cleanup_i ();
  }

  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  // Notify the event channel....
  this->event_channel_->disconnected (this, ACE_TRY_ENV);
  ACE_CHECK;

  if (this->event_channel_->disconnect_callbacks ())
    {
      ACE_TRY
        {
          consumer->disconnect_pull_consumer (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore exceptions, we must isolate other clients from
          // problems on this one.
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "ProxySupplier::disconnect_pull_supplier");
        }
      ACE_ENDTRY;
    }

  this->_decr_refcnt ();
}

PortableServer::POA_ptr
TAO_CEC_ProxyPullSupplier::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPullSupplier::_add_ref (CORBA::Environment &)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPullSupplier::_remove_ref (CORBA::Environment &)
{
  this->_decr_refcnt ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Queue<CORBA_Any>;
template class ACE_Node<CORBA_Any>;
template class ACE_Unbounded_Queue_Iterator<CORBA_Any>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Queue<CORBA_Any>
#pragma instantiate ACE_Node<CORBA_Any>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA_Any>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
